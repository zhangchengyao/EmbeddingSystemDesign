import processing.serial.*;
Serial duankou;
int data;

int numBalls =13;
float spring = 0.8;
float friction = -0.9;
float gravity = 0.4;
Ball[] balls = new Ball[numBalls];
float pitch,roll;

void setup() 
{
  size(700, 250);
  noStroke();
  smooth();
  
  duankou = new Serial(this,"COM3",9600);

  for (int i = 0; i < numBalls; i++) {
    balls[i] = new Ball(random(width), random(height), random(20, 40), i, balls);}
}

void draw() 
{
   if(duankou.available()>0)
  {
    
    pitch=duankou.read();
    roll = duankou.read();
    
    println(pitch+" "+roll);

  if(pitch>47 && roll>47)  {      

    for (int i = 0; i < numBalls; i++) {
    balls[i] = new Ball(random(width), random(height), random(20, 40), i, balls);
    }
  }
  
  }
  background(128);
  for (int i = 0; i < numBalls; i++) {
    balls[i].collide();
    balls[i].move();
    balls[i].display();
  }
}

class Ball {

  float x, y;
  float diameter;
  float vx = 0;
  float vy = 0;
  int id;
  Ball[] others;

  Ball(float xin, float yin, float din, int idin, Ball[] oin) {
    x = xin;
    y = yin;
    diameter = din;
    id = idin;
    others = oin;
  } 

  void collide() {
    for (int i = id + 1; i < numBalls; i++) {
      float dx = others[i].x - x;
      float dy = others[i].y - y;
      float distance = sqrt(dx*dx + dy*dy);
      float minDist = others[i].diameter/2 + diameter/2;
      if (distance < minDist) { 
        float angle = atan2(dy, dx);
        float targetX = x + cos(angle) * minDist;
        float targetY = y + sin(angle) * minDist;
        float ax = (targetX - others[i].x) * spring;
        float ay = (targetY - others[i].y) * spring;
        vx -= ax;
        vy -= ay;
        others[i].vx += ax;
        others[i].vy += ay;
      }
    }   
  }

  void move() {
    vy += gravity;
    x += vx;
    y += vy;
    if (x + diameter/2 > width) {
      x = width - diameter/2;
      vx *= friction; 
    }
    else if (x - diameter/2 < 0) {
      x = diameter/2;
      vx *= friction;
    }
    if (y + diameter/2 > height) {
      y = height - diameter/2;
      vy *= friction; 
    } 
    else if (y - diameter/2 < 0) {
      y = diameter/2;
      vy *= friction;
    }
  }

  void display() {
    //fill(255, 204);
    fill(255,255,0,100);
    ellipse(x, y, diameter, diameter);
  }
}