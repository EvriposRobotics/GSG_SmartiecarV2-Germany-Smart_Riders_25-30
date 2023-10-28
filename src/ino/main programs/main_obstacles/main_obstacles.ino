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

#include <math.h>
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
float correction_Left = 10.0;
float correction_Right = 10.0;
float StraightAngle = 0.0;

// Driving direction 'U' for Unknown
char DrivingDirection = 'U';

// Speeds
int NormalSpeed = 80;
int SlowSpeed = 90;
int CurveSpeed = 80;
int StartSpeed = 80;

// obstacle block 'U' for unknown
char Block = 'U';

// Block variables
char P_color = 'U'; // Unknown
char P_wall_L = 'N';
char P_wall_R = 'N';
int P_x = 0;
int P_height = 0;

// last counted corners
unsigned long LastCurveTime;
unsigned long NextCurveDelay = 5000;

// both allignments (L/R)
int walldistance = 40;

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
// Gyro_steer_straight()
// uses the gyro to orient itself straight to the track
////////////////////////////////////////////////////////////////////
void Gyro_steer_straight()
{
  float angle;
  int Speed;
  int Steering;
  angle = IMU_getAngle();

  Steering = (angle - StraightAngle) * 0.8; // 0.8 = wiggle factor
  if (Steering > 15.0)
  {
    Steering = 15;
  }
  else if (Steering < -15.0)
  {
    Steering = -15;
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

  // delay(20);
}

/////////////////////////////////////////////////////////////////////
// CalculateTargetDirection()
// calculates the target direction for the next curve
/////////////////////////////////////////////////////////////////////
float CalculateTargetDirection()
{
  float angle;
  // float StraightAngle;
  float CalculateAngle = 0.0;

  angle = IMU_getAngle();
  // StraightAngle = CalculateStraightAngle(angle);

  if (DrivingDirection == 'R')
  {
    CalculateAngle = StraightAngle + 90.0;
  }

  else if (DrivingDirection == 'L')
  {
    CalculateAngle = StraightAngle - 90.0;
  }

  return CalculateAngle;
}

/////////////////////////////////////////////////////
// CalculateStraightAngle()
// calculates the target angle for the next curve
/////////////////////////////////////////////////////
float CalculateStraightAngle(float angle)
{
  float StraightAngle = 0.0;
  int factor = 0;

  // calculate straight direction
  factor = int((angle + 44.0) / 90.0); // int cuts off fractions
  StraightAngle = factor * 90.0;

  return StraightAngle;
}

/////////////////////////////////////////////////////////////////////
// TurnLeft()
// turns left until 90 degrees
/////////////////////////////////////////////////////////////////////
void TurnLeft()
{
  float TargetDirection;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = SlowSpeed;
  right(45);

  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = CalculateTargetDirection();

  runMotor_R(SlowSpeed); // backwards turn
  // turn until 90 degrees or block in sight
  // turn backward for 45 degrees
  while (angle > StraightAngle - 45.0)
  {
    angle = IMU_getAngle();
    delay(20);
  }
  stopMotor();

  // turn forward
  left(45);
  runMotor(SlowSpeed);
  while (angle > TargetDirection)
  {
    angle = IMU_getAngle();
    delay(20);
  }
  stopMotor();

  corners = corners + 1;
  StraightAngle = TargetDirection;
  center();
  stopMotor();
  Distance_Left = SpaceUltraSonicLeft();
  Distance_Right = SpaceUltraSonicRight();
  Distance_Front = SpaceUltraSonicFront();
  if (Distance_Front < 20)
  {
    runMotor_R(SlowSpeed);
    delay(1000);
    stopMotor();
  }
  runMotor(SlowSpeed);
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
  left(45);

  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = CalculateTargetDirection();

  runMotor_R(SlowSpeed); // backwards turn
  // turn until 90 degrees or block in sight
  // turn backward for 45 degrees
  while (angle < StraightAngle + 45.0)
  {
    angle = IMU_getAngle();
    delay(20);
  }
  stopMotor();

  // turn forward
  right(45);
  runMotor(SlowSpeed);
  while (angle < TargetDirection)
  {

    angle = IMU_getAngle();
    /*
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor_R(Speed);
    */
    delay(20);
  }
  stopMotor();

  corners = corners + 1;
  StraightAngle = TargetDirection;
  center();
  runMotor(SlowSpeed);
  Distance_Left = SpaceUltraSonicLeft();
  Distance_Right = SpaceUltraSonicRight();
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
  Distance_Left = SpaceUltraSonicLeft();
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

  Distance_Right = SpaceUltraSonicRight();

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
// CurveLeftUntilBlock()
// makes left turn until 90 degrees or block in sight to adjust speed
/////////////////////////////////////////////////////////////////////
void CurveLeftUntilBlock()
{
  int Speed;
  float TargetDirection;
  Speed = SlowSpeed;
  lcd.setRGB(0, 0, 255);
  TargetDirection = CalculateTargetDirection();
  lcd.setCursor(0, 0);
  lcd.print(TargetDirection);
  Distance_Front = SpaceUltraSonicFront();
  // go straight to opposite wall
  while (Distance_Front > 50)
  {
    Gyro_steer_straight();
    Distance_Front = SpaceUltraSonicFront();
  }
  // wall reached
  stopMotor();
  // start turning
  left(45);
  runMotor(SlowSpeed);
  angle = IMU_getAngle();
  // replaces quadrantenSYS
  runMotor(SlowSpeed);

  while (angle > StraightAngle - 45.0)
  {
    angle = IMU_getAngle();
  }
  stopMotor();
  center();
  runMotor(SlowSpeed);
  delay(500);
  stopMotor();
  right(45);
  runMotor_R(SlowSpeed);
  while (angle > TargetDirection)
  {
    angle = IMU_getAngle();
  }
  stopMotor();
  center();
  runMotor(SlowSpeed);
  delay(500);
  corners = corners + 1;
  StraightAngle = TargetDirection;
  runMotor(SlowSpeed);
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

/////////////////////////////////////////////////////////////////////
// CurveRightUntilBlock()
// makes right turn until 90 degrees or block in sight to adjust speed
/////////////////////////////////////////////////////////////////////
void CurveRightUntilBlock()
{
  int Speed;
  float TargetDirection;
  Speed = SlowSpeed;
  lcd.setRGB(0, 0, 255);
  right(45);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = CalculateTargetDirection();
  findNextPillar();
  while (angle < TargetDirection - correction_Right)
  {
    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    findNextPillar();
    if ((P_color != 'U') && (P_x >= 160))
    {
      // Pillar is in view and in the middle -> end curve
      break;
    }
  }

  stopMotor();
  center();
  if ((P_color != 'U') && (P_height > 30))
  {
    // already too near, go back a little bit to better approach next pillar
    runMotor_R(SlowSpeed);
    delay(500);
    stopMotor();
    runMotor(SlowSpeed);
  }
  corners = corners + 1;
  StraightAngle = TargetDirection;
  runMotor(SlowSpeed);
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

/*
/////////////////////////////////////////////////////////////////////
// DriveUntilRedBlockInSight()
// drives towards red block and tries to drive right
/////////////////////////////////////////////////////////////////////
void DriveUntilRedBlockInSight()
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
  Distance_Front = SpaceUltraSonicFront();
  while (Distance_Front > 30)
  {
    delay(50);
    Distance_Front = SpaceUltraSonicFront();
  }
}

/////////////////////////////////////////////////////////////////////
// DriveUntilGreenBlockInSight()
// drives towards green block and tries to drive left
/////////////////////////////////////////////////////////////////////
void DriveUntilGreenBlockInSight()
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
  Distance_Front = SpaceUltraSonicFront();
  while (Distance_Front > 30)
  {
    delay(50);
    Distance_Front = SpaceUltraSonicFront();
  }
}
*/
/////////////////////////////////////////////////////////////////////
// DriveUntilFirstPillarInLane()
// drive straight until next pillar in lane is found
/////////////////////////////////////////////////////////////////////
void DriveUntilFirstPillarInLane()
{
  runMotor(SlowSpeed);
  findNextPillar();
  if (DrivingDirection == 'L')
  {
    // go straight, until pillar is in sight,
    // ignore pillars on the extrem left, we have already passed them,
    // they are in the last lane
    while ((P_color == 'U') || (P_x < 50))
    {
      Gyro_steer_straight();
      findNextPillar();
    }
  }
  else
  {
    // go straight, until pillar is in sight,
    // ignore pillars on the extrem right, we have already passed them,
    // they are in the last lane
    while ((P_color == 'U') || (P_x > 270))
    {
      Gyro_steer_straight();
      findNextPillar();
    }
  }
}

/////////////////////////////////////////////////////////////////////
// DriveUntilNextCurve()
// drive straight until curve is found
/////////////////////////////////////////////////////////////////////
void DriveUntilNextCurve()
{
  steerToLaneCenter();
  Distance_Front = SpaceUltraSonicFront();
  while (Distance_Front > 100)
  {
    Gyro_steer_straight();
    Distance_Front = SpaceUltraSonicFront();
  }
}
/////////////////////////////////////////////////////////////////////
// steerToLaneCenter()
// make a small move toward the lane center
/////////////////////////////////////////////////////////////////////

void steerToLaneCenter()
{

  float TargetDirection;
  unsigned long start;
  TargetDirection = StraightAngle;
  Distance_Front = SpaceUltraSonicFront();
  Distance_Left = SpaceUltraSonicLeft();
  Distance_Right = SpaceUltraSonicRight();

  if (Distance_Left < 25)
  {
    // steer to the middle of the lane
    right(25);
    delay(500);
    center();
    delay(500);
    // back to straight
    left(25);

    angle = IMU_getAngle();
    while (angle > TargetDirection /*- correction_Left*/)
    {
      angle = IMU_getAngle();
      delay(20);
    }
    center();
    runMotor(SlowSpeed);
  }
  else if (Distance_Right < 25)
  {
    // steer to the middle of the lane
    left(25);
    delay(500);
    center();
    delay(500);
    // back to straight
    right(25);

    angle = IMU_getAngle();
    while (angle > TargetDirection /*- correction_Left*/)
    {
      angle = IMU_getAngle();
      delay(20);
    }
    center();
    runMotor(SlowSpeed);
  }
  else
  {
    // just go straight for 500 msec
    start = millis();
    while (millis() < start + 500)
    {
      Gyro_steer_straight();
      delay(20);
    }
  }
}
/////////////////////////////////////////////////////////////////////
// ApproachPillar()
// drive towards green pillar
/////////////////////////////////////////////////////////////////////
void ApproachPillar()
{
  runMotor(SlowSpeed);
  findNextPillar();
  if (P_color == 'R')
  {
    lcd.setRGB(255, 0, 0);
  }
  else if (P_color == 'G')
  {
    lcd.setRGB(0, 255, 0);
  }

  while (P_height < 40)
  {
    SteerToPillar();
    findNextPillar();
    lcd.setCursor(0, 0);
    lcd.print(P_height);
    lcd.print("    ");
  }
}
/////////////////////////////////////////////////////////////////////
// SteerToPillar()
// steers towards pillar
/////////////////////////////////////////////////////////////////////
void SteerToPillar()
{
  int Steering;
  Steering = (160 - P_x) * 0.3;
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
}

/////////////////////////////////////////////////////////////////////
// EvadeGreenPillar()
// evades green block to the left until it is not in sight
/////////////////////////////////////////////////////////////////////
void EvadeGreenPillar()
{
  float angle;
  float TargetDirection;
  int Speed;
  lcd.setRGB(0, 255, 0);
  // save orientation
  angle = IMU_getAngle();
  // calculate next straight direction
  // TargetDirection = CalculateStraightAngle(angle);
  TargetDirection = StraightAngle;
  runMotor(SlowSpeed);
  left(35);
  // runMotor(CurveSpeed);
  findNextPillar();
  // turn until block is on the left
  while ((P_x < 290) && (P_color == 'G'))
  {
    findNextPillar();
    lcd.setCursor(0, 0);
    lcd.print(P_x);
    lcd.print("   ");
  }
  // Go past pillar, until it is not longer seen or wall collision
  // while ((P_color != 'U') && (P_wall_R == 'N'))
  center();
  delay(200); // go straigh minimum 200 msec

  while (P_color == 'G')
  {
    findNextPillar();
    lcd.setCursor(0, 0);
    lcd.print(P_color);
    lcd.print(" ");
    lcd.print(P_wall_L);
  }

  delay(400);
  // back to straight
  right(25);

  angle = IMU_getAngle();
  while (angle < TargetDirection)
  {
    angle = IMU_getAngle();
    lcd.setCursor(0, 0);
    lcd.print(angle);
    lcd.print("   ");
    // delay(20);
  }
  center();
  runMotor(SlowSpeed);
  // drive past
  delay(200);
  lcd.setRGB(255, 255, 255);
}

/////////////////////////////////////////////////////////////////////
// EvadeRedPillar()
// evades red block to the right until it is not in sight
/////////////////////////////////////////////////////////////////////
void EvadeRedPillar()
{
  float angle;
  float TargetDirection;
  int Speed;
  lcd.setRGB(255, 0, 0);
  // save orientation
  angle = IMU_getAngle();
  // calculate next straight direction
  // TargetDirection = CalculateStraightAngle(angle);
  TargetDirection = StraightAngle;
  runMotor(SlowSpeed);
  right(35);
  // runMotor(CurveSpeed);
  findNextPillar();
  // turn until block is on the right
  while ((P_x > 30) && (P_color == 'R'))
  {
    findNextPillar();
    lcd.setCursor(0, 0);
    lcd.print(P_x);
    lcd.print("   ");
  }
  // Go past pillar, until it is not longer seen or wall collision
  // while ((P_color != 'U') && (P_wall_R == 'N'))

  center();
  delay(200);

  while (P_color == 'R')
  {
    findNextPillar();
    lcd.setCursor(0, 0);
    lcd.print(P_color);
    lcd.print(" ");
    lcd.print(P_wall_R);
  }

  delay(400);
  // back to straight
  left(25);

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
// runCurve()
// runs a curve phase
/////////////////////////////////////////////////////////////////////
void runCurve()
{

  // Turn
  if (DrivingDirection == 'L')
  {

    Distance_Right = SpaceUltraSonicRight();
    if (Distance_Right < 25)
    {
      CurveLeftUntilBlock();
    }
    else
    {
      if (Distance_Right > 60)
      {
        steerToLaneCenter();
      }
      // Go straight to opposite wall
      while (Distance_Front > 10)
      {
        Gyro_steer_straight();
        Distance_Front = SpaceUltraSonicFront();
      }
      // wall reached
      stopMotor();
      TurnLeft();
    }
  }
  else
  {
    Distance_Left = SpaceUltraSonicLeft();
    if (Distance_Left < 25)
    {
      CurveRightUntilBlock();
    }
    else
    {
      // Go straight to opposite wall
      while (Distance_Front > 10)
      {
        Gyro_steer_straight();
        Distance_Front = SpaceUltraSonicFront();
      }
      // wall reached
      stopMotor();

      TurnRight();
    }
  }
}

/////////////////////////////////////////////////////////////////////
// runLane()
// run along one lane with obstacles
// minimum 1 obstacle, maximum 2 obstacles
/////////////////////////////////////////////////////////////////////
void runLane()
{
  bool check = false;
  DriveUntilFirstPillarInLane();
  ApproachPillar();
  if (P_color == 'R')
  {
    EvadeRedPillar();
  }
  else
  {

    EvadeGreenPillar();
  }

  // Another pillar in this lane?
  steerToLaneCenter();
  findNextPillar();
  // check if pillar is in lane
  if (P_color != 'U')
  {
    check = CheckPillarIsInLane();
  }
  else
  {
    check = false;
  }
  if ((P_color == 'R') && (check == true))
  {
    ApproachPillar();
    EvadeRedPillar();
  }
  else if ((P_color == 'G') && (check == true))
  {
    ApproachPillar();
    EvadeGreenPillar();
  }
  else
  {
    DriveUntilNextCurve();
  }
}

/////////////////////////////////////////////////////////////////////
// CheckPillarIsInLane()
// Checks, if a Pillar in view is in the current lane #
// or behind the next curve
/////////////////////////////////////////////////////////////////////
bool CheckPillarIsInLane()
{
  findNextPillar();
  if ((DrivingDirection == 'L') && (P_color != 'U') && (P_x < 100))
  {
    return true;
  }
  else if ((DrivingDirection == 'R') && (P_color != 'U') && (P_x > 210))
  {
    return true;
  }
  else
  {
    return false;
  }
}
/////////////////////////////////////////////////////////////////////
// measureAllDistances()
// measures all distances and saves them in global variables
/////////////////////////////////////////////////////////////////////
void measureAllCurrentDistances()
{
  Distance_Front = SpaceUltraSonicFront();
  Distance_Left = SpaceUltraSonicLeft();
  Distance_Right = SpaceUltraSonicRight();
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
  lcd.print(DrivingDirection);
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

///////////////////////////////////////////////////////////////////////
// evaluateRaspiData()
// evaluates the data from the raspi handshake
///////////////////////////////////////////////////////////////////////
void evaluateRaspiData()
{
  if (P_color != 'U')
  {
    if (P_x < 110)
    {
      DrivingDirection = 'L';
    }
    else if (P_x > 210)
    {
      DrivingDirection = 'R';
    }
  }
}

///////////////////////////////////////////////////////////
// startPhase()
// starts the phase
///////////////////////////////////////////////////////////
void startPhase()
{
  runMotor(SlowSpeed);

  // Check if pillar is right ahead
  if ((P_color == 'R') && (P_x < 210) && (P_x == 110))
  {
    ApproachPillar();
    EvadeRedPillar();
  }

  else if ((P_color == 'L') && (P_x > 210) && (P_x == 110))
  {
    ApproachPillar();
    EvadeGreenPillar();
  }

  else
  {
    // No pillar directly ahead
    while (Distance_Front > 10)
    {
      Gyro_steer_straight();
      Distance_Front = SpaceUltraSonicFront();
    }
    // wall reached
    stopMotor();

    // if driving direction is unclear, find out
    if (DrivingDirection == 'U')
    {
      Distance_Left = SpaceUltraSonicLeft();
      Distance_Right = SpaceUltraSonicRight();

      if (Distance_Left > 80)
      {
        DrivingDirection = 'L';
      }

      else if (Distance_Right > 80)
      {
        DrivingDirection = 'R';
      }
    }

    lcd.setCursor(0, 0);
    lcd.print(DrivingDirection);

    // Turn
    if (DrivingDirection == 'L')
    {
      TurnLeft();
    }
    else
    {
      TurnRight();
    }
  }
}

void start_test()
{
  // Test: Fixed sequence
  DrivingDirection = 'L';
  findNextPillar();
  ApproachPillar();
  findNextPillar();
  EvadeRedPillar();
  runCurve();
  findNextPillar();
  ApproachPillar();
  EvadeGreenPillar();
  ApproachPillar();
  EvadeRedPillar();
  runCurve();
  ApproachPillar();
  EvadeGreenPillar();
  DriveUntilNextCurve();
  runCurve();
  ApproachPillar();
  EvadeRedPillar();
  ApproachPillar();
  EvadeGreenPillar();
  runCurve();
  stopMotor();
  delay(1000000000);
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

  pinMode(Button, INPUT); // button

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);
  lcd.print("obstacle race");

  // raspi handshake
  raspi_handshake();

  // from Steering.h
  servosetup();

  // setup from ultrasonic_urm09.h
  ultrasonicstart();

  // initalises motor pinmodes from DCmotor.h
  motorsetup();

  delay(5000); // wait for 5 seconds

  // gyro setup gyro2.h
  startGyroscope();

  // Get initial image processing result
  findNextPillar();
  evaluateRaspiData();

  // distance to wall
  measureAllCurrentDistances();

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

  start_test();

  stopMotor();
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

  while (corners < 12)
  {
    runLane();
    runCurve();
  }

  // Finish after 12 corners/curves
  ProgramStop();
}