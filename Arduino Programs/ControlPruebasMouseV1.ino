#include "Mouse.h"

struct xyz
{
  double x, y, z, r, p ,w;
};

int centerState = 0;
int centerX = 0;               // output range of X or Y movement
int centerY = 0;
int xReading;
int yReading;
int wheel[50];
float q1 = 0,q2 = 0, q3 = 0, q4 = 0, q5 = 0, q6 = 0;
float pi = 3.1516;
xyz   posicion;

xyz finalPosition(float ang1, float ang2, float ang3, float ang4, float ang5, float ang6)
{
  double r11,r21,r31,r32,r33;
  xyz pos;
  pos.x   = (pi*sin(ang1)*sin(ang2))/2 - 8*cos(ang1)*cos(ang2 + pi/2) - (pi*sin(ang5)*(cos(ang1)*cos(ang3)*sin(ang2 + pi/2) + cos(ang1)*cos(ang2 + pi/2)*sin(ang3)))/2 + 8*cos(ang1)*cos(ang3)*sin(ang2 + pi/2) + 8*cos(ang1)*cos(ang2 + pi/2)*sin(ang3) - (pi*cos(ang1)*sin(ang3)*sin(ang2 + pi/2))/2;
  pos.y   = (pi*sin(ang1))/2 - 8*cos(ang2 + pi/2)*sin(ang1) - (pi*sin(ang5)*(cos(ang3)*sin(ang1)*sin(ang2 + pi/2) + cos(ang2 + pi/2)*sin(ang1)*sin(ang3)))/2 + 8*cos(ang3)*sin(ang1)*sin(ang2 + pi/2) + 8*cos(ang2 + pi/2)*sin(ang1)*sin(ang3) - (pi*cos(ang1)*sin(ang2))/2 - (pi*sin(ang1)*sin(ang3)*sin(ang2 + pi/2))/2;
  pos.z   = (pi*sin(ang5)*(cos(ang3)*cos(ang2 + pi/2) + sin(ang3)*sin(ang2 + pi/2)))/2 - 8*sin(ang3)*sin(ang2 + pi/2) - 8*cos(ang3)*cos(ang2 + pi/2) + (pi*cos(ang2 + pi/2)*sin(ang3))/2;
  r11     = -sin(ang6)*(cos(ang4)*sin(ang1) - sin(ang4)*(cos(ang1)*sin(ang3)*sin(ang2 + pi/2) - cos(ang1)*cos(ang3)*cos(ang2 + pi/2))) - cos(ang6)*(cos(ang5)*(sin(ang1)*sin(ang4) + cos(ang4)*(cos(ang1)*sin(ang3)*sin(ang2 + pi/2) - cos(ang1)*cos(ang3)*cos(ang2 + pi/2))) + sin(ang5)*(cos(ang1)*cos(ang3)*sin(ang2 + pi/2) + cos(ang1)*cos(ang2 + pi/2)*sin(ang3)));
  r21     = sin(ang6)*(cos(ang1)*cos(ang4) + sin(ang4)*(sin(ang1)*sin(ang3)*sin(ang2 + pi/2) - cos(ang3)*cos(ang2 + pi/2)*sin(ang1))) + cos(ang6)*(cos(ang5)*(cos(ang1)*sin(ang4) - cos(ang4)*(sin(ang1)*sin(ang3)*sin(ang2 + pi/2) - cos(ang3)*cos(ang2 + pi/2)*sin(ang1))) - sin(ang5)*(cos(ang3)*sin(ang1)*sin(ang2 + pi/2) + cos(ang2 + pi/2)*sin(ang1)*sin(ang3)));
  r31     = cos(ang6)*(sin(ang5)*(cos(ang3)*cos(ang2 + pi/2) + sin(ang3)*sin(ang2 + pi/2)) - cos(ang4)*cos(ang5)*(cos(ang3)*sin(ang2 + pi/2) - cos(ang2 + pi/2)*sin(ang3))) + sin(ang4)*sin(ang6)*(cos(ang3)*sin(ang2 + pi/2) - cos(ang2 + pi/2)*sin(ang3));
  r32     = cos(ang6)*sin(ang4)*(cos(ang3)*sin(ang2 + pi/2) - cos(ang2 + pi/2)*sin(ang3)) - sin(ang6)*(sin(ang5)*(cos(ang3)*cos(ang2 + pi/2) + sin(ang3)*sin(ang2 + pi/2)) - cos(ang4)*cos(ang5)*(cos(ang3)*sin(ang2 + pi/2) - cos(ang2 + pi/2)*sin(ang3)));
  r33     = -cos(ang5)*(cos(ang3)*cos(ang2 + pi/2) + sin(ang3)*sin(ang2 + pi/2)) - cos(ang4)*sin(ang5)*(cos(ang3)*sin(ang2 + pi/2) - cos(ang2 + pi/2)*sin(ang3));
  pos.r   = atan2(r32, r33);
  pos.p   = atan2(-r31, sqrt(pow(r32,2)+pow(r33,2)));
  pos.w = atan2(r21, r11); 
  return pos;
}

void rawPos()
{
  Serial.print(posicion.x);
  Serial.print("\t");
  Serial.print(posicion.y);
  Serial.print("\t");
  Serial.print(posicion.z);
  Serial.print("\t");
  Serial.print(posicion.r);
  Serial.print("\t");
  Serial.print(posicion.p);
  Serial.print("\t");
  Serial.print(posicion.w);
  Serial.print("\t");
  Serial.print(digitalRead(5));
  Serial.print(digitalRead(4));
  Serial.print(digitalRead(3));
  Serial.println(digitalRead(2));
}

float map2(float x, float in_min, float in_max, float out_min, float out_max)
{
  float mapeo = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if(mapeo > out_max) out_max;
  if(mapeo < out_min) out_min;
  return mapeo;
}

void map2Data()
{
  q1 = map2(analogRead(A0), 0, 1023, -2.269, 2.269);
  q2 = map2(analogRead(A1), 0, 1023, -2.269, 2.269);
  q3 = map2(analogRead(A1)-analogRead(A2), 0, 1023, -2, 2);
  q4 = map2(analogRead(A3), 0, 1023, -2.269, 2.269);
  q5 = map2(analogRead(A4), 0, 1023, -2.269, 2.269);
  q6 = map2(analogRead(A5), 0, 1023, -2.269, 2.269);
}

int readAxis(double pos, double pos_min, double pos_max) {
  int distance = int(map2(pos, pos_min, pos_max, -12, 12));
  return distance;
}

void centerMode(int count)
{
  map2Data();
  posicion = finalPosition(q1, q2, q3, q4, q5, q6);
  xReading = readAxis(posicion.y,-6,6);
  yReading = readAxis(posicion.z,7,0);
  if(digitalRead(4) == HIGH) Mouse.press(MOUSE_LEFT);
  if(digitalRead(3) == HIGH) Mouse.press(MOUSE_RIGHT);
  if(digitalRead(2) == HIGH && q4 > .5) wheel[49] = 1;
  if(digitalRead(2) == HIGH && q4 < -.5) wheel[49] = -1;
  if(digitalRead(4) == LOW) Mouse.release(MOUSE_LEFT);
  if(digitalRead(3) == LOW) Mouse.release(MOUSE_RIGHT);
  if(digitalRead(2) == LOW) wheel[49] = 0;
  Mouse.move(xReading, yReading, wheel[count]);
}

void pointMode(int count)
{
  map2Data();
  posicion = finalPosition(q1, q2, q3, q4, q5, q6);
  if(centerState == 0)
  {
    centerX = posicion.y;
    centerY = posicion.z;
    centerState = 1;
  }
  while(digitalRead(5) == HIGH)
  {
    map2Data();
    posicion = finalPosition(q1, q2, q3, q4, q5, q6);
    xReading = readAxis(posicion.y,centerX-6,centerX+6);
    yReading = readAxis(posicion.z,centerY+3.5,centerY-3.5);
    if(digitalRead(4) == HIGH) Mouse.press(MOUSE_LEFT);
    if(digitalRead(3) == HIGH) Mouse.press(MOUSE_RIGHT);
    if(digitalRead(2) == HIGH && q4 > .5) wheel[49] = 1;
    if(digitalRead(2) == HIGH && q4 < -.5) wheel[49] = -1;
    if(digitalRead(4) == LOW) Mouse.release(MOUSE_LEFT);
    if(digitalRead(3) == LOW) Mouse.release(MOUSE_RIGHT);
    if(digitalRead(2) == LOW) wheel[49] = 0;
    /*Serial.print(centerX);
    Serial.print('\t');
    Serial.print(centerY);
    Serial.print('\t');
    Serial.print(xReading);
    Serial.print('\t');
    Serial.println(yReading);*/
    Mouse.move(xReading, yReading, wheel[count]);
  }
  centerState = 0;
}

void setup() {
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Mouse.begin();
  q1 = map2(analogRead(A0), 0, 1023, -2.269, 2.269); // de -130º a 130º en radianes
  q2 = map2(analogRead(A1), 0, 1023, -2.269, 2.269);
  q3 = map2(analogRead(A2), 0, 1023, -2.269, 2.269);
  q4 = map2(analogRead(A3), 0, 1023, -2.269, 2.269);
  q5 = map2(analogRead(A4), 0, 1023, -2.269, 2.269);
  q6 = map2(analogRead(A5), 0, 1023, -2.269, 2.269);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  for(int i = 0; i < 50; i++)
  {
    wheel[i] = 0;
  }
}

void loop() {
  for(int i = 0; i < 50; i++)
  {
    centerMode(i);
    //pointMode(i);
    //rawPos();
  }
}
