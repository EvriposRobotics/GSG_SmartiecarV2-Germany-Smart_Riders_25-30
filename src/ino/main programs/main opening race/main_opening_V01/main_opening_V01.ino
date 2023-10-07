//main program without obstacles 01

//Pin assignment
//button = 4
//camera = 10-13
//ultrasonic = 5-6
//I2C Bus = A4-A5
//MotorSpeed = 9
//Motor direction = 7-8
//distance sensor = A0-A1
//Servo steering = 3


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

//Servo section libraries
#include <Servo.h>
//-------------------------------------------------

//RGB libraries
#include <Wire.h>
#include "rgb_lcd.h"

//--------------------------------------------------

#include <NewPing.h>
#include "QuickMedianLib.h"

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



//timer
unsigned long start_time;
//--------------------------------------------------

 //speeds
  int NormalSpeed =  150;
  int SlowSpeed   =  100;
  int CurveSpeed  =  120; 
  int StartSpeed  =  120;


//LCD connection
 int Button = 4;
 int colorR = 255;
 int colorG = 0;
 int colorB = 0;
 //LCD screen itself
 rgb_lcd lcd;

//laps corners and quadrant
int corners  = 0;
int laps     = 0; 
int quadrant = 0;

//Distances own funcs (calculate values)
 int Distance_F;
 int Distance_L;
 int Distance_R;
 
 float angle;
 float danger;
 float correction_L = 25.0;
 float correction_R = 25.0;
 float StraightAngle = 0.0;

 
  //DrivingDirection is 'U' for uknown
  char DD = 'U';
  
  
 //last curve measured
 unsigned long LastCurveTime = 0;
 unsigned long NextCurveDelay = 2000;
  
  //both StraightAngles (L/R)
  int Walldistance = 25;
  
//include own modules from local library
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



//Stop programm
void PRGstop()
{
  unsigned long DT;  //driving time
  stopMotor();
  // save current time
  DT = millis() - start_time;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("time: ");
    lcd.print(DT);
    delay(9999999);

}

 
  // startnarrow
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
    lcd.setCursor(0,0);
    lcd.print(angle);
    left(30);
    while(angle > TD + correction_L)
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
    lcd.setCursor(0,0);
    lcd.print(angle);
    right(30);
    while(angle < TD - correction_R)
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


  //align
  void align()
  {
    int Steering;
    Steering = (Distance_L - Distance_R)*0.3;
    if (Steering > 30.0)
    {
      Steering = 30;
    }
    else if (Steering < -30.0)
    {
      Steering = -30;
    }

    if(Steering < 0)
    {
      Steering = Steering *(-1);
      right(Steering);
    }
    
    else
    {
      left(Steering);
    }

    delay(20);

  }

   //align_L
  void align_L()
  {
    int Steering;
    Distance_L = SpaceUS_L();
    Steering = (Distance_L - Walldistance)*0.9;
    if (Steering > 30.0)
    {
      Steering = 30;
    }
    else if (Steering < -30.0)
    {
      Steering = -30;
    }


    if(Steering < 0)
    {
      Steering = Steering *(-1);
      right(Steering);
    }
    
    else
    {
      left(Steering);
    }

    delay(20);

  }

   //align_R
  void align_R()
  {
    int Steering;
    Distance_R = SpaceUS_R();
    Steering = (Walldistance - Distance_R)*0.9;
    if (Steering > 30.0)
    {
      Steering = 30;
    }
    else if (Steering < -30.0)
    {
      Steering = -30;
    }


    if(Steering < 0)
    {
      Steering = Steering *(-1);
      right(Steering);
      lcd.setCursor(0, 1);
      lcd.print("R ");
      lcd.print(Steering);
    }
    
    else
    {
      left(Steering);
      lcd.setCursor(0, 1);
      lcd.print("L ");
      lcd.print(Steering);
    }

    delay(20);

  }


   //
  void Gyro_steer_straight()
  {
    float angle;
    int Speed;
    int Steering;
    angle = IMU_getAngle();

    //Steering = (Distance_L - Walldistance)*0.9;
    Steering = (angle - StraightAngle)*0.8; //0.8 = wiggle factor
    if (Steering > 15.0)
    {
      Steering = 15;
    }
    else if (Steering < -15.0)
    {
      Steering = -15;
    }


    if(Steering < 0)
    {
      Steering = Steering *(-1);
      right(Steering);
      lcd.setCursor(0, 1);
      lcd.print("R ");
      lcd.print(Steering);
    }
    
    else
    {
      left(Steering);
      lcd.setCursor(0, 1);
      lcd.print("L ");
      lcd.print(Steering);
    }

    delay(50);

  }


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
        while(angle > TD + correction_L)
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

      //try to find the inside wall again
      while(Distance_L > 60)
      {
        Distance_L = SpaceUS_L();
        angle = IMU_getAngle();
        delay(10);
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
        while(angle < TD - correction_R)
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

      //try to find the inside wall again
      while(Distance_R > 60)
      {
        Distance_R = SpaceUS_R();
        angle = IMU_getAngle();
        delay(10);
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
Wire.begin();

//LCD SETUP
  pinMode(Button, INPUT); // Button

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);


  //setup servo
  servosetup();

  delay(5000); // 5 second stop in order for the gyro to calibrate

  //setup gyroscope IMU
  startGyroscope();

  //initalises motor pinmodes from DCmotor.h
  motorsetup();
    
  //handshake with raspberry pi
  raspi_handshake();

 // distance to curve show current reading values
 Distance_F = SpaceUS_F(); 
 Distance_L = SpaceUS_L();
 Distance_R = SpaceUS_R();

if(Distance_L < 10)
{
  DD = 'L';
}

if(Distance_R < 10)
{
  DD = 'R';  
}

  
  //count corners
   StraightAngle = IMU_getAngle(); 

    lcd.setCursor(0,0);
    lcd.print(Distance_L);
    lcd.print("  ");
    lcd.print(Distance_F);
    lcd.print("  ");
    lcd.print(Distance_R);
    lcd.setCursor(0,1);
    lcd.print(StraightAngle);
    lcd.print("  ");    
    lcd.print(DD);  

  //setup fertig, AmpGe schalten == knopf drücken
  lcd.setRGB(255, 130, 0);
  
  while (digitalRead(Button) == LOW) 
  { // Warten, until Knopf gedrückt wird
    delay(50);
  }

  lcd.clear();
  
  //ampel grün weh
  lcd.setRGB(0, 255, 0);  

  // Speichere die aktuelle Zeit
  start_time = millis();
  LastCurveTime = millis() - NextCurveDelay;

  //Steering center  
  center();

  // find drivingdirection slowly
  if(DD == 'U')
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
    else if(Distance_R >= 80.0)
    {
      DD = 'R';
      Curve_R();
    }
    else
    {
      DD = 'U';
    }
    
  }

  else
  {
    runMotor(SlowSpeed);
    delay(100);
    if(Distance_L < 10)
    {
      StartNarrow_L();
    }

    else if(Distance_R < 10)
    {
      StartNarrow_R();
    }
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

 // Distance to Corner show current reading values
 Distance_L = SpaceUS_L();
 Distance_R = SpaceUS_R();
 
 //temporary value prints
 lcd.setCursor(0,0);
 lcd.print(Distance_L);
 lcd.print("  "); 
 lcd.print(Distance_R);
 lcd.print("  ");

  //Drivingdirection check
  if(DD == 'U')
  {
    if(Distance_L > 80)
    {
      DD = 'L';
    lcd.setCursor(0,0);
    lcd.print(Distance_L);
      Curve_L();

    }

    else if(Distance_R > 80)
    {
      DD = 'R';
    lcd.setCursor(0,1);
    lcd.print(Distance_R);
        Curve_R();

    }        
  }


  
  //check if L turn
  if((DD == 'L') && (Distance_L > 80) && (millis() - LastCurveTime >= NextCurveDelay))
  {
    Curve_L();
  }
 
    //check if R turn
  if((DD == 'R') && (Distance_R > 80) && (millis() - LastCurveTime >= NextCurveDelay))
  {
    Curve_R();
  }

 //calls allign function
 if(DD == 'U')
 {
  align();
  delay(20);
 }
 
 else if(DD == 'L')
 {
   align_L();
   delay(40);
 }

 else if(DD == 'R')
 {
   align_R();
   delay(40);
 }

  //all corners check
  if(corners == 12)
  {
    //slight countersteering to avoid crash with inside wall
    if(DD == 'R')
    {
      left(10);
      delay(400);
      center();
      delay(400);
    }
    else
    {
      right(10);
      delay(400);
      center();
      delay(400);
    }
    
    //drive straight to finish and stop
    Distance_F = SpaceUS_F();
    while (Distance_F > 140)
    {
      Distance_F = SpaceUS_F();
    }
    PRGstop();
  }
 
}