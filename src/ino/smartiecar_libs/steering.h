//servo library
#include <Servo.h>
//---------------------------------------------------------------

//servo pin
int Servopin = 3;
int centered = 72;

// create servo object
Servo servo;

//---------------------------------------------------------------



//Servo steering

//left steering  
 void right(int Angle) {
   int P1;
   P1 = centered + Angle;
  servo.write(P1);
 }

//right steering
 void left(int Angle) {
   int P2;
   P2  = centered - Angle;
 servo.write(P2);
 } 


//steering straight
 void center() 
 {
  //steering straight
 servo.write(centered);
 }


void servosetup()
{
  //SERVO SETUPs
  servo.attach(Servopin);
  center();
}
