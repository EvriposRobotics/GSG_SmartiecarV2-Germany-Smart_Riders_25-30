#include <Servo.h>
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\steering.h"

// create servo object to control a servo
Servo servo;

unsigned long start_time;

void setup() 
{
  // initalise serial communication
  servo.attach(3);

  servo.write(85);
}

void loop() 
{
  //write servo to center
  center();
  delay(3000);

  //write servo to left
  left(40);
  delay(3000);

  //write servo to right
  right(40);
  delay(3000);


}
