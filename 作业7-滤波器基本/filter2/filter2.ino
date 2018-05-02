//kalman滤波器
#include <Wire.h>
#define DEVICE (0x53)       //ADXL345 device address
#define TO_READ (6)         //num of bytes we are going to read each time (two bytes for each axis)
byte buff[TO_READ] ;        //6 bytes buffer for saving data read from the device
char str[512];              //string buffer to transform data before sending it to the serial port
int regAddress = 0x32;      //first axis-acceleration-data register on the ADXL345
int x, y, z;				        //three axis acceleration data
double roll = 0.00, pitch = 0.00;		//Roll & Pitch are the angles which rotate by the axis X and y 
double froll = 0.00, fpitch = 0.00;
//in the sequence of R(x-y-z),more info visit
void setup() {
  Wire.begin();         // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0);      
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
}

void loop() {

  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
                                              //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
                                              //thus we are converting both bytes in to one int
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];

  //we send the x y z values as a string to the serial port
//  Serial.print("The acceleration info of x, y, z are:");
//  sprintf(str, "%d %d %d", x, y, z);  
//  Serial.print(str);
//  Serial.write(10);

  //Roll & Pitch calculate
  RP_calculate();
  // Output
  Serial.print(pitch);
  Serial.print(":");
  Serial.print(roll);
  Serial.print(":");
  filter();
  // Output (filtered)
  Serial.print(fpitch);
  Serial.print(":");
  Serial.print(froll);
  Serial.println();
  //It appears that delay is needed in order not to clog the port
  delay(50);
}

//---------------- Functions
//Writes val to address register on device
void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        // send register address
  Wire.write(val);        // send value to write
  Wire.endTransmission(); //end transmission
}

//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission

    Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device

  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}

//calculate the Roll&Pitch
void RP_calculate(){
  double x_Buff = float(x);
  double y_Buff = float(y);
  double z_Buff = float(z);
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3;
}

//kalman滤波算法
void filter(){
   float kx = 0;
   float p = 1;
   float A = 1;
   float H = 1;
   float q = 2e2;//10e-6;  /* predict noise convariance */
   float r = 5e2;
   float gain;
   
   kx = A * kx;
   p = A * A * p + q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

   /* Measurement */
   gain = p * H / (p * H * H + r);
   kx = kx + gain * (z - H * kx);
   p = (1 - gain * H) * p;
  
   x = kx;
   
   float ky = 0;
   p = 1;
   A = 1;
   H = 1;
   q = 2e2;//10e-6;  /* predict noise convariance */
   r = 5e2;
   
   ky = A * ky;
   p = A * A * p + q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

   /* Measurement */
   gain = p * H / (p * H * H + r);
   ky = ky + gain * (z - H * ky);
   p = (1 - gain * H) * p;
  
   y = ky;

   float kz = 0;
   p = 1;
   A = 1;
   H = 1;
   q = 2e2;//10e-6;  /* predict noise convariance */
   r = 5e2;
   
   kz = A * kz;
   p = A * A * p + q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */

   /* Measurement */
   gain = p * H / (p * H * H + r);
   kz = kz + gain * (z - H * kz);
   p = (1 - gain * H) * p;
  
   z = kz;
    
   RP_calculate();
   froll = roll;
   fpitch = pitch;

}
