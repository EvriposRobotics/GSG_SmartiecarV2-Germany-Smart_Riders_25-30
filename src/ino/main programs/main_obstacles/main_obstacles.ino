/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
██████╗ ██╗███╗   ██╗     █████╗ ███████╗███████╗██╗ ██████╗ ███╗   ██╗███╗   ███╗███████╗███╗   ██╗████████╗
██╔══██╗██║████╗  ██║    ██╔══██╗██╔════╝██╔════╝██║██╔════╝ ████╗  ██║████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
██████╔╝██║██╔██╗ ██║    ███████║███████╗███████╗██║██║  ███╗██╔██╗ ██║██╔████╔██║█████╗  ██╔██╗ ██║   ██║
██╔═══╝ ██║██║╚██╗██║    ██╔══██║╚════██║╚════██║██║██║   ██║██║╚██╗██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║
██║     ██║██║ ╚████║    ██║  ██║███████║███████║██║╚██████╔╝██║ ╚████║██║ ╚═╝ ██║███████╗██║ ╚████║   ██║
╚═╝     ╚═╝╚═╝  ╚═══╝    ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// button                      = D4
// Ultrasonic F,R,L            = A0-A2
// I2C Bus                     = A4-A5 -> LCD, Gyro
// MotorSpeed                  = D5
// Moto direction              = D6
// Servo steering (pwm signal) = D3

///////////////////////////////////////////////////////////////
/*
██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗███████╗███████╗
██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║██╔════╝██╔════╝
██║     ██║██████╔╝██████╔╝███████║██████╔╝██║█████╗  ███████╗
██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗██║██╔══╝  ╚════██║
███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║██║███████╗███████║
╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝╚══════╝
*/
///////////////////////////////////////////////////////////////

// Servo section libraries
#include <Servo.h>
//-------------------------------------------------

// RGB libraries
#include <Wire.h>
#include "rgb_lcd.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗         ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗███████╗
██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║         ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
██║  ███╗██║     ██║   ██║██████╔╝███████║██║         ██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗  ███████╗
██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║         ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗     ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗███████║
 ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝      ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// timer
unsigned long start_time;
//--------------------------------------------------

// LCD connections
int Button = 4;
int colorRed = 255;
int colorGreen = 0;
int colorBlue = 0;
// LCD screen itself
rgb_lcd lcd;

// corners/edge detection
int corners;

// own variables for distances
// int Distance;
int Distance_Front;
int Distance_Left;
int Distance_Right;

float angle;
float danger;
float correction_Left = -5.0;
float correction_Right = 15.0;

// Driving direction 'U' for Unknown
char TargetDirection = 'U';

// Speeds
int NormalSpeed = 120;
int SlowSpeed = 90;
int CurveSpeed = 120;
int StartSpeed = 120;

// obstacle block 'U' for unknown
char Block = 'U';

// Block
char P_color = 'U'; // Unknown
int P_x = 0;
int P_y = 0;
int P_height = 0;
int P_width = 0;
int P_Bottom_edge = 0;
int P_Distance = 0;

// last counted corners
unsigned long LastCurveTime;
unsigned long NextCurveDelay = 5000;

// both allignments (L/R)
int walldistance = 20;

// own Module
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\DCmotor.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\gyro2.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\steering.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\ultrasonic_urm09.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\raspi.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\cam.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 ██████╗ ██╗    ██╗███╗   ██╗    ███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
██╔═══██╗██║    ██║████╗  ██║    ██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
██║   ██║██║ █╗ ██║██╔██╗ ██║    █████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
██║   ██║██║███╗██║██║╚██╗██║    ██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
╚██████╔╝╚███╔███╔╝██║ ╚████║    ██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
 ╚═════╝  ╚══╝╚══╝ ╚═╝  ╚═══╝    ╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ProgramStop()
// stops the program and saves current time in millis
/////////////////////////////////////////////////////////////////////
void ProgramStop()
{
  unsigned long DrivingTime;
  stopMotor();
  // save time
  DrivingTime = millis() - start_time;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("time: ");
  lcd.print(DrivingTime);
  delay(9999999);
}

/////////////////////////////////////////////////////////////////////
// calcTargedDirection()
// calculates the target direction for the next curve
/////////////////////////////////////////////////////////////////////
float CalculateTargetDirection()
{
  float angle;
  float CalculateAngle = 0.0;
  angle = IMU_getAngle();
  if (TargetDirection == 'R')
  {

    if ((angle > 315) || (angle <= 45))
    {
      CalculateAngle = 90.0;
    }
    else if ((angle > 45) && (angle <= 135))
    {
      CalculateAngle = 180.0;
    }
    else if ((angle > 135) && (angle <= 225))
    {
      CalculateAngle = 270.0;
    }
    else if ((angle > 225) && (angle <= 315))
    {
      CalculateAngle = 360.0;
    }
  }

  else
  {
    if ((angle > 315) || (angle <= 45))
    {
      CalculateAngle = 270.0;
    }
    else if ((angle > 225) && (angle <= 315))
    {
      CalculateAngle = 180.0;
    }
    else if ((angle > 135) && (angle <= 225))
    {
      CalculateAngle = 90.0;
    }

    else if ((angle > 45) && (angle <= 135))
    {
      CalculateAngle = 0.0;
    }
  }

  return CalculateAngle;
}

/////////////////////////////////////////////////////////////////////
// TurnLeft()
//  turns left until 90 degrees
/////////////////////////////////////////////////////////////////////
void TurnLeft()
{
  float TargetDirection;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = SlowSpeed;
  right(35);
  runMotor_R(SlowSpeed); // backwards turn
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = CalculateTargetDirection();
  // turn until 90 degrees or block in sight
  while ((angle > TargetDirection - correction_Left))
  {

    angle = IMU_getAngle();
    //
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor_R(Speed);
    // delay(20);
  }
  stopMotor();
  corners = corners + 1;
  center();
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
  runMotor(SlowSpeed);
}

/////////////////////////////////////////////////////////////////////
// TurnRight()
// turns right until 90 degrees
/////////////////////////////////////////////////////////////////////
void TurnRight()
{
  float TargetDirection;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = SlowSpeed;
  // back turn
  left(35);
  runMotor_R(SlowSpeed);
  angle = IMU_getAngle();
  // replaces complicated quadrantsystem
  TargetDirection = CalculateTargetDirection();
  // turn until 90 degrees or block in sight
  while (angle < TargetDirection - correction_Right)
  {
    // turn until 90 degrees or block in sight
    angle = IMU_getAngle();
    //
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor_R(Speed);
    // delay(20);
  }
  stopMotor();
  corners = corners + 1;
  center();
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
  runMotor(SlowSpeed);
}

/////////////////////////////////////////////////////////////////////
// alignCenter()
// aligns to the center of the track
/////////////////////////////////////////////////////////////////////
void alignCenter()
{
  int Steering;
  Steering = (Distance_Left - Distance_Right) * 0.3;
  if (Steering > 30.0)
  {
    Steering = 30;
  }
  else if (Steering < -30.0)
  {
    Steering = -30;
  }

  if (Steering < 0)
  {
    Steering = Steering * (-1);
    right(Steering);
  }

  else
  {
    left(Steering);
  }

  delay(20);
}

/////////////////////////////////////////////////////////////////////
// alignLeft()
// aligns to the left wall
/////////////////////////////////////////////////////////////////////
void alignLeft()
{
  int Steering;
  Distance_Left = SpaceUS_L();
  Steering = (Distance_Left - walldistance) * 0.9;
  if (Steering > 30.0)
  {
    Steering = 30;
  }
  else if (Steering < -30.0)
  {
    Steering = -30;
  }

  if (Steering < 0)
  {
    Steering = Steering * (-1);
    right(Steering);
  }

  else
  {
    left(Steering);
  }

  delay(20);
}

/////////////////////////////////////////////////////////////////////
// alignRight()
// aligns to the right wall
/////////////////////////////////////////////////////////////////////
void alignRight()
{
  int Steering;

  Distance_Right = SpaceUS_R();

  Steering = (walldistance - Distance_Right) * 0.9;
  if (Steering > 30.0)
  {
    Steering = 30;
  }
  else if (Steering < -30.0)
  {
    Steering = -30;
  }

  if (Steering < 0)
  {
    Steering = Steering * (-1);
    right(Steering);
  }

  else
  {
    left(Steering);
  }

  delay(20);
}

/////////////////////////////////////////////////////////////////////
// Corners_Left_Until_Block_In_Sight()
// makes left turn until 90 degrees or block in sight to adjust speed
/////////////////////////////////////////////////////////////////////
void Corners_Left_Until_Block_In_Sight()
{
  int Speed;
  float TargetDirection;
  Speed = SlowSpeed;
  lcd.setRGB(0, 0, 255);
  left(40);
  angle = IMU_getAngle();
  // replaces quadrantenSYS
  TargetDirection = CalculateTargetDirection();
  runMotor(SlowSpeed);
  //
  // turn until 90 degrees or block in sight
  while (angle > TargetDirection - correction_Left)
  {

    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    delay(20);
  }
  center();
  corners = corners + 1;
  stopMotor();
  delay(20);
  runMotor_R(SlowSpeed);
  delay(250);
  stopMotor();
  runMotor(SlowSpeed);
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

/////////////////////////////////////////////////////////////////////
// Corners_Right_Until_Block_In_Sight()
// makes right turn until 90 degrees or block in sight to adjust speed
/////////////////////////////////////////////////////////////////////
void Corners_Right_Until_Block_In_Sight()
{
  int Speed;
  float TargetDirection;
  Speed = SlowSpeed;
  lcd.setRGB(0, 0, 255);
  right(40);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = CalculateTargetDirection();
  while (angle < TargetDirection - correction_Right)
  {
    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    delay(20);
  }
  center();
  corners = corners + 1;
  stopMotor();
  delay(20);
  runMotor_R(SlowSpeed);
  delay(250);
  stopMotor();
  runMotor(SlowSpeed);
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

/////////////////////////////////////////////////////////////////////
// Red_Block_In_Sight()
// drives towards red block and tries to drive right
/////////////////////////////////////////////////////////////////////
void Red_Block_In_Sight()
{
  findNextPillar();
  if (P_x > Picturemiddle_x - 10)
  {
    right(25);
    while (P_x > Picturemiddle_x - 10)
    {
      delay(20);
      findNextPillar();
    }
  }
  else if (P_x < Picturemiddle_x)
  {
    left(25);
    while (P_x > Picturemiddle_x - 10)
    {
      delay(20);
      findNextPillar();
    }
  }
  center();
  Distance_Front = SpaceUS_F();
  while (Distance_Front > 30)
  {
    delay(50);
    Distance_Front = SpaceUS_F();
  }
}

/////////////////////////////////////////////////////////////////////
// Green_Block_In_Sight()
// drives towards green block and tries to drive left
/////////////////////////////////////////////////////////////////////
void Green_Block_In_Sight()
{
  findNextPillar();
  if (P_x > Picturemiddle_x - 10)
  {
    left(25);
    while (P_x > Picturemiddle_x - 10)
    {
      delay(20);
      findNextPillar();
    }
  }
  else if (P_x < Picturemiddle_x)
  {
    right(25);
    while (P_x > Picturemiddle_x - 10)
    {
      delay(20);
      findNextPillar();
    }
  }
  center();
  Distance_Front = SpaceUS_F();
  while (Distance_Front > 30)
  {
    delay(50);
    Distance_Front = SpaceUS_F();
  }
}

/////////////////////////////////////////////////////////////////////
// Evade_Left()
// evades left
void Evade_Left()
{
  float angle;
  float TargetDirection;
  int Speed;

  // save orientation and quadrant
  TargetDirection = IMU_getAngle();
  left(25);
  runMotor(CurveSpeed);
  // turn until block not in sight
  while (P_color == 2)
  {
    delay(20);
    findNextPillar();
  }
  runMotor(SlowSpeed);
  center();

  delay(40); // straight after delay
  right(25);
  runMotor(CurveSpeed);
  // steer until orientation reached
  // TargetDirection = IMU_TD_gerade();
  angle = IMU_getAngle();
  while (angle < TargetDirection)
  {
    angle = IMU_getAngle();
    delay(20);
  }
  center();
  runMotor(SlowSpeed);
  // drive past
  delay(200);
  lcd.setRGB(255, 255, 255);
}

void Evade_Right()
{
  float angle;
  float TargetDirection;
  int Speed;

  // save orientation and quadrant
  TargetDirection = IMU_getAngle();
  right(25);
  runMotor(CurveSpeed);
  // drive until block not in sight
  while (P_color == 1)
  {
    delay(20);
    findNextPillar();
  }
  runMotor(SlowSpeed);
  center();

  delay(50); // straight after delay
  left(25);
  runMotor(CurveSpeed);
  // TargetDirection = IMU_TD_gerade();
  angle = IMU_getAngle();
  while (angle > TargetDirection /*- correction_Left*/)
  {
    angle = IMU_getAngle();
    delay(20);
  }
  center();
  runMotor(SlowSpeed);
  // drive past
  delay(200);
  lcd.setRGB(255, 255, 255);
}

/////////////////////////////////////////////////////////////////////
// measureAllDistances()
// measures all distances and saves them in global variables
/////////////////////////////////////////////////////////////////////
void measureAllCurrentDistances()
{
  Distance_Front = SpaceUS_F();
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
}

/////////////////////////////////////////////////////////////////////
// printAllDistancesToLCD()
// prints all distances to the LCD aswell as DrivingDirection
/////////////////////////////////////////////////////////////////////
void printAllCurrentDistancesToLCD()
{
  lcd.setCursor(0, 0);
  lcd.print(Distance_Left);
  lcd.print("  ");
  lcd.print(Distance_Front);
  lcd.print("  ");
  lcd.print(Distance_Right);
  lcd.setCursor(0, 1);
  lcd.print(angle);
  lcd.print("  ");
  lcd.print(TargetDirection);
  lcd.print("  ");
  lcd.print(P_color);
}

/////////////////////////////////////////////////////////////////////
// waitOnButtonPress()
// waits until the button is pressed
/////////////////////////////////////////////////////////////////////
void waitOnButtonPress()
{
  // switch lcd to yellow and wait on button press
  lcd.setRGB(255, 130, 0);
  while (digitalRead(Button) == LOW)
  {
    delay(50);
  }
}

/////////////////////////////////////////////////////////////////////
// saveCurrentTime()
// saves the current time in milliseconds subtracting NextCurveDelay
/////////////////////////////////////////////////////////////////////

void saveCurrentTime()
{
  start_time = millis();
  LastCurveTime = millis() - NextCurveDelay;
}

/////////////////////////////////////////////////////////////////////
// Get_Current_Block()
// gets the current block and cleares the LCD
/////////////////////////////////////////////////////////////////////
void Get_Current_Block()
{
  findNextPillar();
}

///////////////////////////////////////////
/*
███████╗███████╗████████╗██╗   ██╗██████╗
██╔════╝██╔════╝╚══██╔══╝██║   ██║██╔══██╗
███████╗█████╗     ██║   ██║   ██║██████╔╝
╚════██║██╔══╝     ██║   ██║   ██║██╔═══╝
███████║███████╗   ██║   ╚██████╔╝██║
╚══════╝╚══════╝   ╚═╝    ╚═════╝ ╚═╝
*/
///////////////////////////////////////////

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  // LCD SETUP
  pinMode(Button, INPUT); // button
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);
  lcd.print("obstacle race");
  //------------------------------------------------
  // from Steering.h
  servosetup();

  // setup from ultrasonic_urm09.h
  ultrasonicstart();

  // initalises motor pinmodes from DCmotor.h
  motorsetup();

  delay(5000); // wait for 5 seconds

  // gyro setup gyro2.h
  startGyroscope();

  // raspi handshake
  raspi_handshake();

  // distance to wall
  measureAllCurrentDistances();

  Get_Current_Block();

  // count corners
  angle = IMU_getAngle();

  // prints all distances to LCD
  printAllCurrentDistancesToLCD();

  // waits for button press
  waitOnButtonPress();

  lcd.clear();

  // LCD goes White
  lcd.setRGB(255, 255, 255);

  // save time in millis
  saveCurrentTime();

  // Steering centered
  center();

  // slow start
  runMotor(SlowSpeed);
}
// Done

//////////////////////////////////
/*
██╗      ██████╗  ██████╗ ██████╗
██║     ██╔═══██╗██╔═══██╗██╔══██╗
██║     ██║   ██║██║   ██║██████╔╝
██║     ██║   ██║██║   ██║██╔═══╝
███████╗╚██████╔╝╚██████╔╝██║
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝
 */
//////////////////////////////////

void loop()
{

  int coincidence;
  // check for block

  findNextPillar();
  Distance_Front = SpaceUS_F();

  // show block on LCD
  if (P_color == 1)
  {
    lcd.setRGB(255, 0, 0);
  }
  else if (P_color == 2)
  {
    lcd.setRGB(0, 255, 0);
  }
  else
  {
    lcd.setRGB(255, 255, 255);
  }

  // red: check if block in sight
  if ((P_color == 1) && (P_x > 80) && (P_Bottom_edge > 150))
  {
    // if too close = slight reverse
    if (P_Bottom_edge > 200)
    {
      runMotor_R(180);
      delay(500);
      stopMotor();
      runMotor(SlowSpeed);
    }
    // red: evade right
    Evade_Right();
  }

  else if ((P_color == 2) && (P_x < 240) && (P_Bottom_edge > 150))
  {
    // if too close = slight reverse
    if (P_Bottom_edge > 200)
    {
      runMotor_R(180);
      delay(500);
      stopMotor();
      runMotor(SlowSpeed);
    }
    // green: evade left
    Evade_Left();
  }
  else // no block in sight to evade
  {

    Distance_Front = SpaceUS_F();
    Distance_Left = SpaceUS_L();
    Distance_Right = SpaceUS_R();

    if (TargetDirection == 'K')
    {
      if (Distance_Left > 80)
      {
        TargetDirection = 'L';
      }
      else if (Distance_Right > 80)
      {
        TargetDirection = 'R';
      }
    }

    // infront wall
    if (Distance_Front < 25) // close to wall
    {

      stopMotor();

      if (millis() - LastCurveTime > NextCurveDelay) // corners detected
      {
        if (TargetDirection == 'K')
        {
          Distance_Left = SpaceUS_L();
          Distance_Right = SpaceUS_R();
          if (Distance_Left > 60)
          {
            TargetDirection = 'L';
          }
          else if (Distance_Right > 60)
          {
            TargetDirection = 'R';
          }
          else
          {

            // if TargetDirection still unclear, select at random
            coincidence = random(2);
            if (coincidence == 0)
            {
              TargetDirection = 'R';
            }
            else
            {
              TargetDirection = 'L';
            }
          }
        }

        lcd.setCursor(0, 0);
        lcd.print(TargetDirection);
        delay(500);
        // check: turning or curve
        if (TargetDirection == 'R')
        {
          Distance_Left = SpaceUS_L();
          if (Distance_Left > 35)
          {
            TurnRight();
          }
          else
          {
            // back and curve
            runMotor_R(180);
            while (Distance_Front < 70)
            {
              delay(20);
              Distance_Front = SpaceUS_F();
            }
            stopMotor();
            Corners_Right_Until_Block_In_Sight();
          }
        }
        else // TargetDirection == L
        {

          Distance_Right = SpaceUS_R();
          if (Distance_Right > 35)
          {
            TurnLeft();
          }
          else
          {
            // back and curve
            runMotor_R(180);
            while (Distance_Front < 60)
            {
              delay(20);
              Distance_Front = SpaceUS_F();
            }
            stopMotor();
            Corners_Left_Until_Block_In_Sight();
          }
          stopMotor();
          lcd.setRGB(255, 255, 255); // End of curve, turning
          delay(100);
          runMotor(SlowSpeed);
        }
      } // end of curve detected

      else
      {
        // emergency brake if obstacle not detected
        runMotor_R(180);
        delay(2000);
        stopMotor();
        delay(500);
        runMotor(NormalSpeed);
      }

    } // end of distance < 25

    else
    {
      // orientate towards the outer wall
      Distance_Left = SpaceUS_L();
      Distance_Right = SpaceUS_R();
      if (TargetDirection == 'R')
      {
        alignLeft();
      }
      else if (TargetDirection == 'L')
      {
        alignRight();
      }

      delay(20);
    }

  } // End not a block to the evade in sight
}