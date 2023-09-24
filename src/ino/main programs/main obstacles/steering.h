//Servo section libraries
#include <Servo.h>
//---------------------------------------------------------------

//servo pin
int Servopin = 3;
int centered = 87;
// Create a servo object
Servo servo;

//---------------------------------------------------------------


//straight steering
 void middle () {
 servo.write(centered);
 }



void servosetup()
{
  //SERVO SETUP
  // Verbinde das Servo an Pin 9
  servo.attach(Servopin);
  middle();
}


//Servo Steering

//left steering  
 void left(int angle) 
 {
   int P1;
   P1 = centered + angle;
  servo.write(P1);
 }

//right steering
 void right(int angle) 
 {
   int P2;
   if(angle > 30)
   {
     angle = 30;
   }
   P2  = centered - angle;
 servo.write(P2);
 } 
