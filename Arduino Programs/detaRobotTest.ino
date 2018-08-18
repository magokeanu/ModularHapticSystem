#include <Servo.h>

Servo servo1     ;
Servo servo2     ;
Servo servo3     ;

//double a   =  3  ;
//double b   =  3  ;
//double r1  =  3  ;
//double r2  =  3  ;
//double px  =  1  ;
//double py  =  1  ;
//double pz  =  3.5+.2*(px+py);

double x;
double y;
double z;

double phi1b[3]  ; //b
double phi2[3]   ;
double phi3[3]   ;

double w[3]      ;
double A[3]    ;
double B[3]  ;
double C[3] ;

double  theta[] = {0, 120*3.14159/180, -120*3.14159/180};

void setup() {
  Serial.begin(9600);
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
}

void loop() 
{
  for(double i = 0; i < 10; i += .1)
  {
    x = .4*cos(2*3.14159*i);
    y = .4*sin(2*3.14159*i);
    z = .6*cos(2*3.14159*i);
    degs(3, 3, 3, 3, 0, 0, 2.5+z);
    servo1.write(phi1b[0]);
    servo2.write(phi1b[1]);
    servo3.write(phi1b[2]);
    delay(50);
  }
}

void degs(double a, double b, double r1, double r2, double px, double py, double pz)
{
  //  Ecuacciones para phi3[i]
  
  phi3[0] = acos(py/b);
  phi3[1] = acos((py*cos(theta[1]) - px*sin(theta[1]))/b);
  phi3[2] = acos((py*cos(theta[2]) - px*sin(theta[2]))/b);

  //  Ecuacciones para phi2[i]

  w[0]    = pow(px*cos(0) + py*sin(0) + r2 - r1, 2) + pow(py*cos(0) - px*sin(0),2) + pow(pz, 2);
  w[1]    = pow(px*cos(theta[1]) + py*sin(theta[1]) + r2 - r1, 2) + pow(py*cos(theta[1]) - px*sin(theta[1]), 2) + pow(pz, 2);
  w[2]    = pow(px*cos(theta[2]) + py*sin(theta[2]) + r2 - r1, 2) + pow(py*cos(theta[2]) - px*sin(theta[2]), 2) + pow(pz, 2);

  phi2[0] = acos((w[0] - pow(a, 2) - pow(b, 2))/(2*a*b*sin(phi3[0])));
  phi2[1] = acos((w[1] - pow(a, 2) - pow(b, 2))/(2*a*b*sin(phi3[1])));
  phi2[2] = acos((w[2] - pow(a, 2) - pow(b, 2))/(2*a*b*sin(phi3[2])));

  //  Ecuacciones para phi1[i]

  w[0]      = px*(cos(0) - sin(0)) + py*(cos(0) + sin(0)) + pz + r2 - r1;
  w[1]      = px*(cos(theta[1]) - sin(theta[1])) + py*(cos(theta[1]) + sin(theta[1])) + pz + r2 - r1;
  w[2]      = px*(cos(theta[2]) - sin(theta[2])) + py*(cos(theta[2]) + sin(theta[2])) + pz + r2 - r1;

  A[0]    = -(a + b*sin(phi3[0])*cos(phi2[0]) + b*sin(phi3[0])*sin(phi2[0]) + w[0]);
  A[1]    = -(a + b*sin(phi3[1])*cos(phi2[1]) + b*sin(phi3[1])*sin(phi2[1]) + w[1]);
  A[2]    = -(a + b*sin(phi3[2])*cos(phi2[2]) + b*sin(phi3[2])*sin(phi2[2]) + w[2]);

  B[0]  = 2*(a + b*sin(phi3[0])*cos(phi2[0]) - b*sin(phi3[0])*sin(phi2[0]));
  B[1]  = 2*(a + b*sin(phi3[1])*cos(phi2[1]) - b*sin(phi3[1])*sin(phi2[1]));
  B[2]  = 2*(a + b*sin(phi3[2])*cos(phi2[2]) - b*sin(phi3[2])*sin(phi2[2]));

  C[0] = a + b*sin(phi3[0])*cos(phi2[0]) + b*sin(phi3[0])*sin(phi2[0]) + b*cos(phi3[0]) - w[0];
  C[1] = a + b*sin(phi3[1])*cos(phi2[1]) + b*sin(phi3[1])*sin(phi2[1]) + b*cos(phi3[1]) - w[1];
  C[2] = a + b*sin(phi3[2])*cos(phi2[2]) + b*sin(phi3[2])*sin(phi2[2]) + b*cos(phi3[2]) - w[2];

  phi1b[0]  = 2*atan2((2*A[0]),(-B[0] + sqrt(pow(B[0], 2) - 4*A[0]*C[0])));
  phi1b[1]  = 2*atan2((2*A[1]),(-B[1] + sqrt(pow(B[1], 2) - 4*A[1]*C[1])));
  phi1b[2]  = 2*atan2((2*A[2]),(-B[2] + sqrt(pow(B[2], 2) - 4*A[2]*C[2])));

  phi1b[0] =  180 + int(phi1b[0]*180/3.14159);
  phi1b[1] =  180 + int(phi1b[1]*180/3.14159);
  phi1b[2] =  180 + int(phi1b[2]*180/3.14159);

  //Serial data
  
  Serial.print(phi1b[0]);
  Serial.print('\t');
  Serial.print(phi1b[1]);
  Serial.print('\t');
  Serial.println(phi1b[2]);
 
}

