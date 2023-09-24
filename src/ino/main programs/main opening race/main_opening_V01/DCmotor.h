// DC Motor control

#include <CytronMotorDriver.h>

int GSM1 = 5;
int in1 = 6;

CytronMD motor(PWM_DIR, GSM1, in1);

// backward
 void runMotor_R(int Speed) 
{
  motor.setSpeed((-1)*Speed);
}


 // stop
 void stopMotor() 
{
  motor.setSpeed(0);
}



 //run motor
 void runMotor(int Speed)  
{
  motor.setSpeed(Speed);
}


void motorsetup()
{
  //pinmodes motor
  pinMode(GSM1, OUTPUT);    
  pinMode(in1, OUTPUT);
}