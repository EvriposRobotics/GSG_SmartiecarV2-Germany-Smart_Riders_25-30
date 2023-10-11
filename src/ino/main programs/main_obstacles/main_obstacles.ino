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
int Distance;
int Distance_Left;
int Distance_Right;

float angle;
float danger;
float correction_Left = -5.0;
float correction_Right = 15.0;

// Driving direction 'Uknown' for uknown
char TargetDirection = 'Uknown';

// Speeds
int NormG = 145;
int LongG = 145;
int CurveSpeed = 195;

// obstacle block 'Uknown' for unknown
char Block = 'Uknown';

// Block
char P_color = 'Uknown'; // uknown
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

// Stop programm
void PRGstop()
{
  unsigned long FZ;
  stopMotor();
  // save time
  FZ = millis() - start_time;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("time: ");
  lcd.print(FZ);
  delay(9999999);
}

// calculate Targetangle for corners
float calcTD()
{
  float angle;
  float calcA = 0.0;
  angle = IMU_getAngle();
  if (TargetDirection == 'R')
  {

    if ((angle > 315) || (angle <= 45))
    {
      calcA = 90.0;
    }
    else if ((angle > 45) && (angle <= 135))
    {
      calcA = 180.0;
    }
    else if ((angle > 135) && (angle <= 225))
    {
      calcA = 270.0;
    }
    else if ((angle > 225) && (angle <= 315))
    {
      calcA = 360.0;
    }
  }

  else
  {
    if ((angle > 315) || (angle <= 45))
    {
      calcA = 270.0;
    }
    else if ((angle > 225) && (angle <= 315))
    {
      calcA = 180.0;
    }
    else if ((angle > 135) && (angle <= 225))
    {
      calcA = 90.0;
    }

    else if ((angle > 45) && (angle <= 135))
    {
      calcA = 0.0;
    }
  }

  return calcA;
}

// turn left
void turn_L()
{
  float TargetDirection;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = LongG;
  // backwards turn
  right(35);
  runMotor_R(LongG);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = calcTD();
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
  runMotor(LongG);
}

void turn_R()
{
  float TargetDirection;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = LongG;
  // back turn
  left(35);
  runMotor_R(LongG);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = calcTD();
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
  runMotor(LongG);
}

// align
void align()
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
}

// align
void align_L()
{
  int Steering;
  Steering = (Distance_Left - walldistance) * 0.8;
  if (Steering > 20.0)
  {
    Steering = 20;
  }
  else if (Steering < -20.0)
  {
    Steering = -20;
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

// align_r
void align_R()
{
  int Steering;
  Steering = (walldistance - Distance_Right) * 0.8;
  if (Steering > 20.0)
  {
    Steering = 20;
  }
  else if (Steering < -20.0)
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

// corners left Main obstacle
void corners_L_MO()
{
  int Speed;
  float TargetDirection;
  Speed = LongG;
  lcd.setRGB(0, 0, 255);
  left(40);
  angle = IMU_getAngle();
  // replaces quadrantenSYS
  TargetDirection = calcTD();
  runMotor(LongG);
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
  runMotor_R(LongG);
  delay(250);
  stopMotor();
  runMotor(LongG);
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

// corners right main obstacle
void corners_R_MO()
{
  int Speed;
  float TargetDirection;
  Speed = LongG;
  lcd.setRGB(0, 0, 255);
  right(40);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TargetDirection = calcTD();
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
  runMotor_R(LongG);
  delay(250);
  stopMotor();
  runMotor(LongG);
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

void to_Red()
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
  Distance = SpaceUS_F();
  while (Distance > 30)
  {
    delay(50);
    Distance = SpaceUS_F();
  }
}

void to_Green()
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
  Distance = SpaceUS_F();
  while (Distance > 30)
  {
    delay(50);
    Distance = SpaceUS_F();
  }
}

void Evade_L()
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
  runMotor(LongG);
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
  runMotor(LongG);
  // drive past
  delay(200);
  lcd.setRGB(255, 255, 255);
}

void Evade_R()
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
  runMotor(LongG);
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
  runMotor(LongG);
  // drive past
  delay(200);
  lcd.setRGB(255, 255, 255);
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
  Distance = SpaceUS_F();
  Distance_Left = SpaceUS_L();
  Distance_Right = SpaceUS_R();

  // get block
  findNextPillar();
  lcd.clear();

  // corners count
  angle = IMU_getAngle();

  lcd.setCursor(0, 0);
  lcd.print(Distance_Left);
  lcd.print("  ");
  lcd.print(Distance);
  lcd.print("  ");
  lcd.print(Distance_Right);
  lcd.setCursor(0, 1);
  lcd.print(angle);
  lcd.print("  ");
  lcd.print(TargetDirection);
  lcd.print("  ");
  lcd.print(P_color);

  // setup Done, switch LCD Green == press the button
  lcd.setRGB(255, 130, 0);

  while (digitalRead(Button) == LOW)
  {
    // wait for button press
    delay(50);
  }

  lcd.clear();

  // light goes out
  lcd.setRGB(255, 255, 255);

  // saves current time
  start_time = millis();
  LastCurveTime = millis() - NextCurveDelay;

  // Steering middle
  center();

  // slow start
  runMotor(LongG);
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
  Distance = SpaceUS_F();

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
      runMotor(LongG);
    }
    // red: evade right
    Evade_R();
  }

  else if ((P_color == 2) && (P_x < 240) && (P_Bottom_edge > 150))
  {
    // if too close = slight reverse
    if (P_Bottom_edge > 200)
    {
      runMotor_R(180);
      delay(500);
      stopMotor();
      runMotor(LongG);
    }
    // green: evade left
    Evade_L();
  }
  else // no block in sight to evade
  {

    Distance = SpaceUS_F();
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
    if (Distance < 25) // close to wall
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
            turn_R();
          }
          else
          {
            // back and curve
            runMotor_R(180);
            while (Distance < 70)
            {
              delay(20);
              Distance = SpaceUS_F();
            }
            stopMotor();
            corners_R_MO();
          }
        }
        else // TargetDirection == L
        {

          Distance_Right = SpaceUS_R();
          if (Distance_Right > 35)
          {
            turn_L();
          }
          else
          {
            // back and curve
            runMotor_R(180);
            while (Distance < 60)
            {
              delay(20);
              Distance = SpaceUS_F();
            }
            stopMotor();
            corners_L_MO();
          }
          stopMotor();
          lcd.setRGB(255, 255, 255); // End of curve, turning
          delay(100);
          runMotor(LongG);
        }
      } // end of curve detected

      else
      {
        // emergency brake if obstacle not detected
        runMotor_R(180);
        delay(2000);
        stopMotor();
        delay(500);
        runMotor(NormG);
      }

    } // end of distance < 25

    else
    {
      // orientate towards the outer wall
      Distance_Left = SpaceUS_L();
      Distance_Right = SpaceUS_R();
      if (TargetDirection == 'R')
      {
        align_L();
      }
      else if (TargetDirection == 'L')
      {
        align_R();
      }

      delay(20);
    }

  } // End not a block to the evade in sight
}