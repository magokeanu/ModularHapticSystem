struct xyz
{
  double x, y, z, r, p ,w;
};

float q1 = 0,q2 = 0, q3 = 0, q4 = 0, q5 = 0, q6 = 0;
float pi = 3.1516;
xyz   posicion;

void setup() {
  Serial.begin(9600);
  q1 = map2(analogRead(A0), 0, 1023, -2.269, 2.269); // -130º to 130º en rad
  q2 = map2(analogRead(A1), 0, 1023, -2.269, 2.269);
  q3 = map2((analogRead(A2)-analogRead(A1)), -512, 512, -2, 2);
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
  rawPos();
}

xyz finalPosition(float ang1, float ang2, float ang3, float ang4, float ang5, float ang6)
{
  double r33,r32,r31,r21,r11;
  xyz pos;
  pos.x          = -8*cos(ang1)*(cos(ang2+pi/2)+cos(ang2+ang3+pi));
  pos.y          = -8*sin(ang1)*(cos(ang2+pi/2)+cos(ang2+ang3+pi));
  pos.z          = 8*(sin(ang2+pi/2)+sin(ang2+ang3+pi));
  r31            = - cos(ang6)*(cos(ang5)*(cos(ang2)*sin(ang3) + cos(ang3)*sin(ang2)) - cos(ang4)*sin(ang5)*(sin(ang2)*sin(ang3) - cos(ang2)*cos(ang3))) - sin(ang4)*sin(ang6)*(sin(ang2)*sin(ang3) - cos(ang2)*cos(ang3));
  r32            = sin(ang6)*(cos(ang5)*(cos(ang2)*sin(ang3) + cos(ang3)*sin(ang2)) - cos(ang4)*sin(ang5)*(sin(ang2)*sin(ang3) - cos(ang2)*cos(ang3))) - cos(ang6)*sin(ang4)*(sin(ang2)*sin(ang3) - cos(ang2)*cos(ang3));
  r33            = - sin(ang5)*(cos(ang2)*sin(ang3) + cos(ang3)*sin(ang2)) - cos(ang4)*cos(ang5)*(sin(ang2)*sin(ang3) - cos(ang2)*cos(ang3));
  r21            = cos(ang6)*(cos(ang5)*(cos(ang2)*cos(ang3)*sin(ang1) - sin(ang1)*sin(ang2)*sin(ang3)) - sin(ang5)*(cos(ang4)*(cos(ang2)*sin(ang1)*sin(ang3) + cos(ang3)*sin(ang1)*sin(ang2)) + cos(ang1)*sin(ang4))) + sin(ang6)*(sin(ang4)*(cos(ang2)*sin(ang1)*sin(ang3) + cos(ang3)*sin(ang1)*sin(ang2)) + cos(ang1)*cos(ang4));
  r11            = cos(ang6)*(sin(ang5)*(sin(ang1)*sin(ang4) - cos(ang4)*(cos(ang1)*cos(ang2)*sin(ang3) + cos(ang1)*cos(ang3)*sin(ang2))) + cos(ang5)*(cos(ang1)*cos(ang2)*cos(ang3) - cos(ang1)*sin(ang2)*sin(ang3))) + sin(ang6)*(sin(ang4)*(cos(ang1)*cos(ang2)*sin(ang3) + cos(ang1)*cos(ang3)*sin(ang2)) - cos(ang4)*sin(ang1));
  pos.r          = -asin(r31); 
  pos.p          = atan2(r32,r33);
  pos.w          = atan2(r21,r11);
  return pos;
}

void rawPos()
{
  Serial.flush();
  Serial.print(String(posicion.x));
  Serial.print("x");
  Serial.print(String(posicion.y));
  Serial.print("y");
  Serial.print(String(posicion.z));
  Serial.print("z");
  Serial.print(String(posicion.r));
  Serial.print("r");
  Serial.print(String(posicion.p));
  Serial.print("p");
  Serial.println(String(posicion.w));
  delay(5);
}

void map2Data()
{
  q1 = map2(analogRead(A0), 0, 1023, -2.269, 2.269);
  q2 = map2(analogRead(A1), 0, 1023, -2.269, 2.269);
  q3 = map2((analogRead(A2)-analogRead(A1)), -512, 512, -2.269, 2.269);
  q4 = map2(analogRead(A3), 0, 1023, -2.269, 2.269);
  q5 = map2(analogRead(A4), 0, 1023, -2.269, 2.269);
  q6 = map2(analogRead(A5), 0, 1023, -2.269, 2.269);
}

float map2(float x, float in_min, float in_max, float out_min, float out_max)
{
  float mapeo = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if(mapeo >  pi*.9/2)  pi*.9/2;
  if(mapeo < -pi*.9/2) -pi*.9/2;
  return mapeo;
}
