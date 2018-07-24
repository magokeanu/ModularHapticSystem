#include "Keyboard.h"

struct xyz
{
  double x, y, z, r, p ,w;
};

int i = 0;
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
  Serial.print((posicion.y));
  Serial.print("\t");
  Serial.print((posicion.z));
  Serial.print("\t");
  Serial.print((posicion.r));
  Serial.print("\t");
  Serial.print((posicion.p));
  Serial.print("\t");
  Serial.print((posicion.w));
  Serial.print("\t");
  Serial.print((digitalRead(4)));
  Serial.print("\t");
  Serial.print((digitalRead(3)));
  Serial.print("\t");
  Serial.print((digitalRead(2)));
  Serial.print("\t");
  Serial.println((digitalRead(5)));
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

float map2(float x, float in_min, float in_max, float out_min, float out_max)
{
  float mapeo = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if(mapeo > out_max) out_max;
  if(mapeo < out_min) out_min;
  return mapeo;
}

void controlSides()
{
  if(q4 < -.2) Keyboard.press('d');
  if(q4 >  .2) Keyboard.press('a');
}


void setup() {
  Serial.begin(9600);
  q1 = map2(analogRead(A0), 0, 1023, -2.269, 2.269); // de -130º a 130º en radianes
  q2 = map2(analogRead(A1), 0, 1023, -2.269, 2.269);
  q3 = map2(analogRead(A1)-analogRead(A2), 0, 1023, -2, 2);
  q4 = map2(analogRead(A3), 0, 1023, -2.269, 2.269);
  q5 = map2(analogRead(A4), 0, 1023, -2.269, 2.269);
  q6 = map2(analogRead(A5), 0, 1023, -2.269, 2.269);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
}

void loop() {
  map2Data();
  posicion = finalPosition(q1, q2, q3, q4, q5, q6);
  if(posicion.x < -5) Keyboard.press('w');
  if(posicion.x > 5) Keyboard.press('s');
  if(posicion.x <= 5 && posicion.x >= -5)
  {
    Keyboard.release('w');
    Keyboard.release('s');
  }

  if(abs(q4) > .2 && i > 125) controlSides();
  else if(abs(q4) > .5 && i >  80) controlSides();
  else if(abs(q4) >  1 && i >  50) controlSides();
  else {
    Keyboard.release('d');
    Keyboard.release('a');
  }
  
  if(digitalRead(4) == HIGH) Keyboard.press('t');
  if(digitalRead(3) == HIGH) Keyboard.press('i');
  if(digitalRead(2) == HIGH) Keyboard.press('y');
  if(digitalRead(4) == LOW) Keyboard.release('t');
  if(digitalRead(3) == LOW) Keyboard.release('i');
  if(digitalRead(2) == LOW) Keyboard.release('y');
  //rawPos();
  i++;
  if(i == 151) i = 0;
}
