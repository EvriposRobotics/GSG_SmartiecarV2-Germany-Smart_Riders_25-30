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



//timer
unsigned long start_time;
//--------------------------------------------------

 //speeds
  int NormalSpeed =  120;
  int SlowSpeed   =  120;
  int CurveSpeed  =  120; 
  int StartSpeed  =  120;


//LCD connection
 int Button = 4;
 int colorR = 255;
 int colorG = 0;
 int colorB = 0;
 //LCD screen itself
 rgb_lcd lcd;

//laps/corners
int corners  = 0;
int laps     = 0; 
int quadrant = 0;

//Distances own funcs (calc values)
 int Distance;
 int Distance_L;
 int Distance_R;
 
 float angle;
 float danger;
 float correction_L = 25.0;
 float correction_R = 25.0;
 float Alignment;

 
  //DrivingDirection
  char DD = 'U';

  //obstacles
  char Block = 'U';

 //last curve measured
 unsigned long LastCurveTime = 0;
 unsigned long NextCurveDelay = 2000;
  
  //both alignments (L/R)
  int Walldistance = 25;

  
//own Modules
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\DCmotor.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\gyro.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\steering.h"
#include "C:\Users\WRO_FE2\Desktop\GSG_SmartiecarV2\src\ino\smartiecar_libs\ultrasonic_urm09.h"

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

//Stop programm
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


   //align gyro
  void align_gyro()
  {
    float angle;
    int Speed;
    int Steering;
    angle = IMU_getAngle();

    //Steering = (Distance_L - Walldistance)*0.9;
    Steering = (angle - Alignment)*0.8;
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
         Speed = SlowSpeed;
        left(40);
        lcd.setRGB(0, 0, 255);
        angle = IMU_getAngle();
        if(quadrant == 0)
        {
          TD = 270.0;
        }
        else if(quadrant == 1)
        {
          TD = 180.0;
        }
        else if(quadrant == 2)
        {
          TD = 90.0;
        }
        else if(quadrant == 3)
        {
          TD = 0.0;
        }
        Alignment = TD;
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
      center();
    runMotor(NormalSpeed);
    Distance_L = SpaceUS_L;
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
         Speed = SlowSpeed;
        right(40);
        lcd.setRGB(0, 0, 255);
        angle = IMU_getAngle();
        if(quadrant == 0)
        {
          TD = 90.0;
        }
        else if(quadrant == 1)
        {
          TD = 180.0;
        }
        else if(quadrant == 2)
        {
          TD = 270.0;
        }
        else if(quadrant == 3)
        {
          TD = 360.0;
        }
        Alignment = TD;
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
      center();
    runMotor(NormalSpeed);
    Distance_R = SpaceUS_R;
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


 //von dem Steering.h
 servosetup();


  //setup von ultraschall.h
  startGyroscope();

  //initalisiert motor pinmodes von DCmotor.h
  motorsetup();

  delay(5000); // 5 Sekunden warten

  //gyro setup gyro.h
  startGyroscope();  
    
 // Distance to Kurve ZEIGE MESSWERTE
 Distance = SpaceUS_F(); 
 Distance_L = SpaceUS_L();
 Distance_R = SpaceUS_R();

if(Distance_L < 10)
{
  TD = 'L';
}

if(Distance_R < 10)
{
  TD = 'R';  
}

  
  //corners zählen
   Alignment = IMU_getAngle(); 

    lcd.setCursor(0,0);
    lcd.print(Distance_L);
    lcd.print("  ");
    lcd.print(Distance);
    lcd.print("  ");
    lcd.print(Distance_R);
    lcd.setCursor(0,1);
    lcd.print(Alignment);
    lcd.print("  ");    
    lcd.print(TD);  

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

  // Langsam erste Kurve finden
  if(TD == 'U')
  {
    runMotor(StartSpeed);
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
 Distance = SpaceUS_F();
 Distance_L = SpaceUS_L();
 Distance_R = SpaceUS_R();


  //TargedDirection check
  if(TD == 'U')
  {
    if(Distance_L > 80)
    {
      TD = 'L';
    lcd.setCursor(0,0);
    lcd.print(Distance_L);
      Curve_L();

    }

    else if(Distance_R > 80)
    {
      TD = 'R';
    lcd.setCursor(0,1);
    lcd.print(Distance_R);
        Curve_R();

    }        
  }


  
  //check if L turn
  if((TD == 'L') && (Distance_L > 80) && (millis() - LastCurveTime >= NextCurveDelay))
  {
    Curve_L();
  }
 
    //check if R turn
  if((TD == 'R') && (Distance_R > 80) && (millis() - LastCurveTime >= NextCurveDelay))
  {
    Curve_R();
  }

 //calls allign function
 if(TD == 'U')
 {
  align();
  delay(20);
 }
 
 else if(TD == 'L')
 {
   align_L();
   //align_gyro();
   delay(40);
 }

 else if(TD == 'R')
 {
   align_R();
   //align_gyro();
   delay(40);
 }


  //drive slow if distance to wall is small
  if(Distance < 90)
  {
    runMotor(SlowSpeed);
  }

  //all corners check
  if(corners == 12)
  {
    //slight countersteering to avoid crash with inside wall
    if(TD == 'R')
    {
      left(10);
      delay(200);
      center();
      delay(400);
    }
    else
    {
      right(10);
      delay(200);
      center();
      delay(400);
    }
    // Drive until to center of the side
    Distance = SpaceUS_F(); 
    while (Distance > 140)
    {
      Distance = SpaceUS_F(); 
    }
    PRGstop();
  }
 
}