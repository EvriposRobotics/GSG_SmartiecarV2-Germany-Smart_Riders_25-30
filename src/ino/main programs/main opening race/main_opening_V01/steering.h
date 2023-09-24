//Servo section libraries
#include <Servo.h>
//---------------------------------------------------------------

//servo pin
int Servopin = 3;
int centered = 63;
int max_left = 25;
int max_right = 120;

// Create a servo object
Servo servo;

//---------------------------------------------------------------


//straight steering
 void middle () 
 {
 servo.write(centered);
 }


void servosetup()
{
  //SERVO SETUP
  // connect the servo to pin 9
  servo.attach(Servopin);
  middle();
}


//Servo Steering

//left steering  
 void left(int angle) 
 {
   int P1;
   P1 = centered - angle;
   //if p1 is smaller than max left limit it to max left
    if(P1 < max_left)
    {
      P1 = max_left;
    }
  servo.write(P1);
 }

//right steering
 void right(int angle) 
 {
  
  int P2;
  P2 = centered + angle;
  //if p2 is bigger than max right limit it to max right
    if(P2 > max_right)
    {
      P2 = max_right;
    }
  servo.write(P2);
 } 