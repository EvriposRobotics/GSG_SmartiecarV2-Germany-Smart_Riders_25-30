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
int colorR = 255;
int colorG = 0;
int colorB = 0;
// LCD screen itself
rgb_lcd lcd;

// corners/edge detection
int corners;

// own variables for distances
int Distance;
int Distance_L;
int Distance_R;

float angle;
float danger;
float correction_L = -5.0;
float correction_R = 15.0;

// Driving direction 'U' for uknown
char TD = 'U';

// Speeds
int NormG = 145;
int LongG = 145;
int CurveSpeed = 195;

// obstacle block 'U' for unknown
char Block = 'U';

// Block
char P_color = 'U'; // uknown
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
  if (TD == 'R')
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
  float TD;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = LongG;
  // backwards turn
  right(35);
  runMotor_R(LongG);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TD = calcTD();
  // turn until 90 degrees or block in sight
  while ((angle > TD - correction_L))
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
  Distance_L = SpaceUS_L();
  Distance_R = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
  runMotor(LongG);
}

void turn_R()
{
  float TD;
  int Speed;
  lcd.setRGB(125, 0, 125);
  Speed = LongG;
  // back turn
  left(35);
  runMotor_R(LongG);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TD = calcTD();
  // turn until 90 degrees or block in sight
  while (angle < TD - correction_R)
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
  Distance_L = SpaceUS_L();
  Distance_R = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
  runMotor(LongG);
}

// align
void align()
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
}

// align
void align_L()
{
  int Steering;
  Steering = (Distance_L - walldistance) * 0.8;
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
  Steering = (walldistance - Distance_R) * 0.8;
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
  float TD;
  Speed = LongG;
  lcd.setRGB(0, 0, 255);
  left(40);
  angle = IMU_getAngle();
  // replaces quadrantenSYS
  TD = calcTD();
  runMotor(LongG);
  //
  // turn until 90 degrees or block in sight
  while (angle > TD - correction_L)
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
  Distance_L = SpaceUS_L();
  Distance_R = SpaceUS_R();
  lcd.setRGB(255, 255, 255);
  LastCurveTime = millis();
}

// corners right main obstacle
void corners_R_MO()
{
  int Speed;
  float TD;
  Speed = LongG;
  lcd.setRGB(0, 0, 255);
  right(40);
  angle = IMU_getAngle();
  // replaces complicated quadrantSYS
  TD = calcTD();
  while (angle < TD - correction_R)
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
  Distance_L = SpaceUS_L();
  Distance_R = SpaceUS_R();
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
  float TD;
  int Speed;

  // save orientation and quadrant
  TD = IMU_getAngle();
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
  // TD = IMU_TD_gerade();
  angle = IMU_getAngle();
  while (angle < TD)
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
  float TD;
  int Speed;

  // save orientation and quadrant
  TD = IMU_getAngle();
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
  // TD = IMU_TD_gerade();
  angle = IMU_getAngle();
  while (angle > TD /*- correction_L*/)
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
  Distance_L = SpaceUS_L();
  Distance_R = SpaceUS_R();

  // get block
  findNextPillar();
  lcd.clear();

  // corners count
  angle = IMU_getAngle();

  lcd.setCursor(0, 0);
  lcd.print(Distance_L);
  lcd.print("  ");
  lcd.print(Distance);
  lcd.print("  ");
  lcd.print(Distance_R);
  lcd.setCursor(0, 1);
  lcd.print(angle);
  lcd.print("  ");
  lcd.print(TD);
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
    Distance_L = SpaceUS_L();
    Distance_R = SpaceUS_R();

    if (TD == 'K')
    {
      if (Distance_L > 80)
      {
        TD = 'L';
      }
      else if (Distance_R > 80)
      {
        TD = 'R';
      }
    }

    // infront wall
    if (Distance < 25) // close to wall
    {

      stopMotor();

      if (millis() - LastCurveTime > NextCurveDelay) // corners detected
      {
        if (TD == 'K')
        {
          Distance_L = SpaceUS_L();
          Distance_R = SpaceUS_R();
          if (Distance_L > 60)
          {
            TD = 'L';
          }
          else if (Distance_R > 60)
          {
            TD = 'R';
          }
          else
          {

            // if TD still unclear, select at random
            coincidence = random(2);
            if (coincidence == 0)
            {
              TD = 'R';
            }
            else
            {
              TD = 'L';
            }
          }
        }

        lcd.setCursor(0, 0);
        lcd.print(TD);
        delay(500);
        // check: turning or curve
        if (TD == 'R')
        {
          Distance_L = SpaceUS_L();
          if (Distance_L > 35)
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
        else // TD == L
        {

          Distance_R = SpaceUS_R();
          if (Distance_R > 35)
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
      Distance_L = SpaceUS_L();
      Distance_R = SpaceUS_R();
      if (TD == 'R')
      {
        align_L();
      }
      else if (TD == 'L')
      {
        align_R();
      }

      delay(20);
    }

  } // End not a block to the evade in sight
}