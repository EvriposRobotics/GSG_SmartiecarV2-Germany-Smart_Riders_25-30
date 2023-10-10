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

//--------------------------------------------

// Servo section libraries
#include <Servo.h>
//-------------------------------------------------

// RGB libraries
#include <Wire.h>
#include "rgb_lcd.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 ██████╗ ██╗      ██████╗ ██████╗  █████╗ ██╗         ██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗███████╗
██╔════╝ ██║     ██╔═══██╗██╔══██╗██╔══██╗██║         ██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
██║  ███╗██║     ██║   ██║██████╔╝███████║██║         ██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗  ███████╗
██║   ██║██║     ██║   ██║██╔══██╗██╔══██║██║         ╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
╚██████╔╝███████╗╚██████╔╝██████╔╝██║  ██║███████╗     ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗███████║
 ╚═════╝ ╚══════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝      ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// timer
unsigned long start_time;
//--------------------------------------------------

// speeds
int NormalSpeed = 120;
int SlowSpeed = 90;
int CurveSpeed = 120;
int StartSpeed = 120;

// LCD connection
int Button = 4;
int colorR = 255;
int colorG = 0;
int colorB = 0;
// LCD screen itself
rgb_lcd lcd;

// laps corners and quadrant
int corners = 0;
int laps = 0;
int quadrant = 0;

// Distances own funcs (calculate values)
int Distance_F;
int Distance_L;
int Distance_R;

float angle;
float danger;
float correction_L = 30.0;
float correction_R = 30.0;
float StraightAngle = 0.0;

// DrivingDirection is 'U' for uknown
char DD = 'U';

// last curve measured
unsigned long LastCurveTime = 0;
unsigned long NextCurveDelay = 2000;

// used for both Driving Directions (left and right)
int Walldistance = 30;

// include own modules from local library
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\DCmotor.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\gyro2.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\steering.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\ultrasonic_urm09.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\raspi.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 ██████╗ ██╗    ██╗███╗   ██╗    ███████╗██╗   ██╗███╗   ██╗ ██████╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
██╔═══██╗██║    ██║████╗  ██║    ██╔════╝██║   ██║████╗  ██║██╔════╝╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
██║   ██║██║ █╗ ██║██╔██╗ ██║    █████╗  ██║   ██║██╔██╗ ██║██║        ██║   ██║██║   ██║██╔██╗ ██║███████╗
██║   ██║██║███╗██║██║╚██╗██║    ██╔══╝  ██║   ██║██║╚██╗██║██║        ██║   ██║██║   ██║██║╚██╗██║╚════██║
╚██████╔╝╚███╔███╔╝██║ ╚████║    ██║     ╚██████╔╝██║ ╚████║╚██████╗   ██║   ██║╚██████╔╝██║ ╚████║███████║
 ╚═════╝  ╚══╝╚══╝ ╚═╝  ╚═══╝    ╚═╝      ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Stop programm
void ProgramStop()
{
  unsigned long DT; // driving time
  unsigned long tmpTime;

  runMotor(SlowSpeed); // slow down
  // slight countersteering to compensate for overshooting
  if (DD == 'R')
  {
    left(10);
  }

  else
  {
    right(10);
  }

  delay(400);
  center();

  // Go straight for at least 400 msec
  Gyro_steer_straight();
  tmpTime = millis();
  while (millis() < tmpTime + 1000)
  {
    Gyro_steer_straight();
  }

  // drive straight to finish and stop
  Distance_F = SpaceUS_F();
  while (Distance_F > 140)
  {
    Gyro_steer_straight();
    Distance_F = SpaceUS_F();
  }

  stopMotor();
  // save current time in milliseconds
  DT = millis() - start_time;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("time: ");
  lcd.print(DT);
  lcd.print(Distance_F);
  delay(9999999); // wait forever
}

// startnarrow Left (start close to left inside wall)
void StartNarrow_L()
{
  float angle;
  float TD = 360.0;
  int Speed = SlowSpeed;
  right(30);
  delay(500);
  center();
  delay(500);
  angle = IMU_getAngle();
  lcd.setCursor(0, 0);
  lcd.print(angle);
  left(30);
  while (angle > TD + correction_L)
  {

    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    delay(50);
  }
  center();
  runMotor(SlowSpeed);
}

// startnarrow Right (start close to right inside wall)
void StartNarrow_R()
{
  float angle;
  float TD = 0.0;
  int Speed;
  left(30);
  delay(500);
  center();
  delay(500);
  angle = IMU_getAngle();
  lcd.setCursor(0, 0);
  lcd.print(angle);
  right(30);
  while (angle < TD - correction_R)
  {

    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    delay(50);
  }
  center();
  runMotor(SlowSpeed);
}

/////////////////////////////////////////////////////////////////////
// alignCenter()
// aligns to the center of the track
/////////////////////////////////////////////////////////////////////

void alignCenter()
{
  int Steering;
  Steering = (Distance_L - Distance_R) * 0.3;
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

// funct to align to the left wall
void alignLeft()
{
  int Steering;
  Distance_L = SpaceUS_L();
  Steering = (Distance_L - Walldistance) * 0.9;
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

// alignRight
void alignRight()
{
  int Steering;

  Distance_R = SpaceUS_R();

  Steering = (Walldistance - Distance_R) * 0.9;
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

// Gyro_steer_straight
void Gyro_steer_straight()
{
  float angle;
  int Speed;
  int Steering;
  angle = IMU_getAngle();

  // Steering = (Distance_L - Walldistance)*0.9;
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

// Curve_L
void Curve_L()
{
  int Speed;
  float TD;
  Speed = CurveSpeed;
  left(40);
  lcd.setRGB(0, 0, 255);
  TD = StraightAngle - 90.0;
  angle = IMU_getAngle();
  StraightAngle = TD;
  runMotor(SlowSpeed);
  while (angle > TD + correction_L)
  {
    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    delay(50);
  }

  corners = corners + 1;
  StraightAngle = TD;
  center();
  runMotor(NormalSpeed);
  Distance_L = SpaceUS_L;

  // try to find the inside wall again
  while (Distance_L > 60)
  {
    Distance_L = SpaceUS_L();
    Gyro_steer_straight();
  }
  lcd.setRGB(0, 255, 0);
  LastCurveTime = millis();
}

void Curve_R()
{
  int Speed;
  float TD;
  Speed = CurveSpeed;
  right(40);
  lcd.setRGB(0, 0, 255);
  TD = StraightAngle + 90.0;
  angle = IMU_getAngle();
  StraightAngle = TD;
  runMotor(SlowSpeed);
  while (angle < TD - correction_R)
  {
    angle = IMU_getAngle();
    Speed = Speed + 5;
    if (Speed > CurveSpeed)
    {
      Speed = CurveSpeed;
    }
    runMotor(Speed);
    delay(50);
  }

  corners = corners + 1;
  StraightAngle = TD;
  center();
  runMotor(NormalSpeed);
  Distance_R = SpaceUS_R;

  // try to find the inside wall again
  while (Distance_R > 60)
  {
    Distance_R = SpaceUS_R();
    Gyro_steer_straight();
  }
  lcd.setRGB(0, 255, 0);
  LastCurveTime = millis();
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

  //////////////////////////////////////////////////////
  /*
   _       _ _           _
  (_)     (_) |         | |
   _ _ __  _| |_   _ __ | |__   __ _ ___  ___
  | | '_ \| | __| | '_ \| '_ \ / _` / __|/ _ \
  | | | | | | |_  | |_) | | | | (_| \__ \  __/
  |_|_| |_|_|\__| | .__/|_| |_|\__,_|___/\___|
                  | |
                  |_|
  init phase initalises all components and waits for button press
  */
  //////////////////////////////////////////////////////

  // Serial.begin(9600);
  Wire.begin();

  // LCD SETUP
  pinMode(Button, INPUT); // Button

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);
  lcd.print("Opening race");

  // setup servo
  servosetup();

  delay(5000); // 5 second stop in order for the gyro to calibrate

  // setup gyroscope IMU
  startGyroscope();

  // initalises motor pinmodes from DCmotor.h
  motorsetup();

  // distance to curve show current reading values
  Distance_F = SpaceUS_F();
  Distance_L = SpaceUS_L();
  Distance_R = SpaceUS_R();

  if (Distance_L < 10)
  {
    DD = 'L';
  }

  if (Distance_R < 10)
  {
    DD = 'R';
  }

  // count corners
  StraightAngle = IMU_getAngle();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(Distance_L);
  lcd.print("  ");
  lcd.print(Distance_F);
  lcd.print("  ");
  lcd.print(Distance_R);
  lcd.setCursor(0, 1);
  lcd.print(StraightAngle);
  lcd.print("  ");
  lcd.print(DD);

  // setup done - show green light
  lcd.setRGB(255, 130, 0);

  while (digitalRead(Button) == LOW)
  { // wait until button is pressed
    delay(50);
  }

  //////////////////////////////////////////////////////
  /*
        _             _           _
       | |           | |         | |
    ___| |_ __ _ _ __| |_   _ __ | |__   __ _ ___  ___
   / __| __/ _` | '__| __| | '_ \| '_ \ / _` / __|/ _ \
   \__ \ || (_| | |  | |_  | |_) | | | | (_| \__ \  __/
   |___/\__\__,_|_|   \__| | .__/|_| |_|\__,_|___/\___|
                           | |
                           |_|
  Start phase run:
  drive straight until first curve is detected,
  detect driving direction clockwise or counterclockwise
  run until first curve is completed
  */
  //////////////////////////////////////////////////////

  lcd.clear();

  // lcd green
  lcd.setRGB(0, 255, 0);

  // save current time
  start_time = millis();
  LastCurveTime = millis() - NextCurveDelay;

  // Steering center
  center();

  // find drivingdirection slowly
  if (DD == 'U')
  {
    runMotor(SlowSpeed);
    while ((Distance_L < 80.0) && (Distance_R < 80.0))
    {
      Distance_L = SpaceUS_L();
      Distance_R = SpaceUS_R();
      Gyro_steer_straight();
    }
    if (Distance_L >= 80.0)
    {
      DD = 'L';
      Curve_L();
    }
    else if (Distance_R >= 80.0)
    {
      DD = 'R';
      Curve_R();
    }
  }

  else if (DD == 'R')
  {
    runMotor(SlowSpeed);
    delay(100);
    StartNarrow_R();
    Distance_R = SpaceUS_R();
    while (Distance_R < 80.0)
    {
      Distance_R = SpaceUS_R();
      Gyro_steer_straight();
    }
    Curve_R();
  }

  else if (DD == 'L')
  {
    runMotor(SlowSpeed);
    delay(100);
    StartNarrow_L();
    Distance_L = SpaceUS_L();
    while (Distance_L < 80.0)
    {
      Distance_L = SpaceUS_L();
      Gyro_steer_straight();
    }
    Curve_L();
  }
}

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
  /////////////////////////////////////////////////////
  /*
                             _
                            | |
    _ __ _   _ _ __    _ __ | |__   __ _ ___  ___
   | '__| | | | '_ \  | '_ \| '_ \ / _` / __|/ _ \
   | |  | |_| | | | | | |_) | | | | (_| \__ \  __/
   |_|   \__,_|_| |_| | .__/|_| |_|\__,_|___/\___|
                      | |
                      |_|
  run phase:
  checks if curve is detected and calls curve(L/R) function
  if no curve alligned to inner wall with fixxed distance
  runs until 12 curves are counted
  */
  /////////////////////////////////////////////////////

  // Distance to Corner show current reading values
  if (DD == 'R')
  {
    while (corners < 12)
    {
      // clockwise running for right
      Distance_R = SpaceUS_R();

      // check for Right Turn
      if ((Distance_R > 80) && (millis() - LastCurveTime >= NextCurveDelay))
      {
        Curve_R();
      }
      else
      {
        alignRight();
      }
    }
  }

  else
  {
    while (corners < 12)
    {

      // counterclockwise running for left
      Distance_L = SpaceUS_L();
      // check for Left Turn
      if ((Distance_L > 80) && (millis() - LastCurveTime >= NextCurveDelay))
      {
        Curve_L();
      }
      else
      {

        alignLeft();
      }
    }
  }

  ////////////////////////////////////////////////////
  /*
                 _         _
                | |       | |
   ___ _ __   __| |  _ __ | |__   __ _ ___  ___
  / _ \ '_ \ / _` | | '_ \| '_ \ / _` / __|/ _ \
 |  __/ | | | (_| | | |_) | | | | (_| \__ \  __/
  \___|_| |_|\__,_| | .__/|_| |_|\__,_|___/\___|
                    | |
                    |_|
end phase:
stops car and shows counted time in milliseconds
*/
  ////////////////////////////////////////////////////

  ProgramStop();
}