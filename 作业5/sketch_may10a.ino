#include <IRremote.h>  // 使用IRRemote函数库

const int irReceiverPin = 8;  // 红外接收器的 OUTPUT 引脚接在 PIN8 接口 定义irReceiverPin变量为PIN2接口

IRrecv irrecv(irReceiverPin); // 设置irReceiverPin定义的端口为红外信号接收端口
decode_results results;    // 定义results变量为红外结果存放位置

void setup()
{
  Serial.begin(9600);   // 开启串口，波特率为9600
  irrecv.enableIRIn();   // 启动红外解码
}

void loop() 
{
  if (irrecv.decode(&results)) {   // 解码成功，把数据放入results变量中
    // 把数据输入到串口
//    Serial.print("irCode: ");
    if(results.value==16716015){
      Serial.write(1);
    }
    else if(results.value==16734885){
      Serial.write(2);
    }
//    Serial.println(results.value,DEC); // 显示红外编码
//    Serial.print(",  bits: ");           
//    Serial.println(results.bits); // 显示红外编码位数
    irrecv.resume();    // 继续等待接收下一组信号
  }  
}

