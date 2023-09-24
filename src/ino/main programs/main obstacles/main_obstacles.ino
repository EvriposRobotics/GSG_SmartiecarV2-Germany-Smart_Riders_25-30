//Assignment of the pins
// buttons = 4
//camera = 10-13
//Ultrasound = 5-6
//I2C Bus = A4-A5 -> LCD, Gyro
//MotorSpeed = 9
//Moto direction = 7-8
//Distance meter (Lox's) = A0-A1
//Servo steering = 3
//empty = A2-A3

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


//Servo section libraries
#include <Servo.h>
//-------------------------------------------------

//RGB libraries
#include <Wire.h>
#include "rgb_lcd.h"

/* Pixy2 Cam
#include "Pixy2.h"
*/


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

//LCD connections
 int Button = 4;
 int colorR = 255;
 int colorG = 0;
 int colorB = 0;
 //LCD screen itself
 rgb_lcd lcd;

// corner/edge detection
int rounds;
int corner;
int orientation;

//own variables for distances
 int Distance;
 int Distance_L;
 int Distance_R;
 
 float angle;
 float danger;
 float correction_L = -5.0;
 float correction_R = 15.0;
 
 //target direction
 char TD = 'K';

 //Speeds
  int NormG = 145;
  int LongG = 145;
  int Cornspeed = 195; 

  //obstacle
  char Block = 'K';


 //Block
 int B_ID = 0;
 int B_x = 0;
 int B_y = 0;
 int B_hoch = 0;
 int B_breit = 0;
 int B_Unterkante = 0;
 int B_Distance = 0;

 //last counted corner
 unsigned long Mcorner;
 unsigned long Zcorner = 5000;
  
 //both allignments (L/R)
  int walldistance = 40;

//own Module
#include "DCmotor.h"
#include "gyro.h"
#include "steering.h"
#include "ultrasonic.h"
#include "cam.h"


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

//calculate Targetangle for corner
float calcTD()
{
  float angle;


  float calcA = 0.0;
  angle = IMU_getAngle();
  if(TD == 'R')
  {

   if ((angle > 315) || (angle <= 45))
   {
     calcA = 90.0;
   } 
   else if((angle > 45) && (angle <= 135))
   {
     calcA = 180.0;
   }
   else if((angle > 135) && (angle <= 225))
   {
     calcA = 270.0;
   }
   else if((angle > 225) && (angle <= 315))
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
   else if((angle > 225) && (angle <= 315))
   {
     calcA = 180.0;
   }
   else if((angle > 135) && (angle <= 225))
   {
     calcA = 90.0;
   }

   else if((angle > 45) && (angle <= 135))
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
    lcd.setRGB(125,0,125);
    Speed = LongG;
     //backwards turn
		 right(35);
		 runMotor_R(LongG);
      angle = IMU_getAngle();
      //replaces complicated quadrantSYS
      TD = calcTD();
      //Find_Block();
       //turn until 90 degrees or block in sight             
        while((angle > TD - correction_L))
        {

           angle = IMU_getAngle();
           //Find_Block();
            Speed = Speed + 5;
            if (Speed > Cornspeed)
            {
              Speed = Cornspeed;
            }
              runMotor_R(Speed);
              //delay(20);
        }
   stopMotor();
   corner = corner + 1;
   middle();
	 Distance_L = SpaceUS_L();
   Distance_R = SpaceUS_R();
   lcd.setRGB(255,255,255);
   Mcorner = millis();
	 runMotor(LongG);

 }
 
  void turn_R()
 {
    float TD;
	  int Speed;
    lcd.setRGB(125,0,125);
    Speed = LongG;
     // back turn
		 left(35);
		 runMotor_R(LongG);
      angle = IMU_getAngle();
      //replaces complicated quadrantSYS
      TD = calcTD();
      //Find_Block();
      //turn until 90 degrees or block in sight             
      while(angle < TD - correction_R)
      {
           //turn until 90 degrees or block in sight
           angle = IMU_getAngle();
           //Find_Block();
            Speed = Speed + 5;
            if (Speed > Cornspeed)
            {
              Speed = Cornspeed;
            }
              runMotor_R(Speed);
              //delay(20);
      }
    stopMotor();
    corner = corner + 1;
    middle();
	  Distance_L = SpaceUS_L();
    Distance_R = SpaceUS_R();
    lcd.setRGB(255,255,255);
    Mcorner = millis();
    runMotor(LongG);

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

  }

   //align
  void align_L()
  {
    int Steering;
    Steering = (Distance_L - walldistance)*0.8;
    if (Steering > 20.0)
    {
      Steering = 20;
    }
    else if (Steering < -20.0)
    {
      Steering = -20;
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

  }

   //ausrichten
  void align_R()
  {
    int Steering;
    Steering = (walldistance - Distance_R)*0.8;
    if (Steering > 20.0)
    {
      Steering = 20;
    }
    else if (Steering < -20.0)
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

  }

  //corner left Main obstacle
  void corner_L_MO()
  {
         int Speed;
         float TD;
         Speed = LongG;
         lcd.setRGB(0, 0, 255);
         left(40);
        angle = IMU_getAngle();
      //replaces quadrantenSYS
      TD = calcTD();
       runMotor(LongG);
       //Find_Block();
       //turn until 90 degrees or block in sight             
        while(angle > TD - correction_L)
        {

           angle = IMU_getAngle();
            Speed = Speed + 5;
            if (Speed > Cornspeed)
            {
              Speed = Cornspeed;
            }
              runMotor(Speed);
              delay(20);
        }
        middle();
     corner = corner + 1;
      stopMotor();
      delay(20);
    runMotor_R(LongG);
    delay(250);
    stopMotor();
    runMotor(LongG);
    Distance_L = SpaceUS_L();
    Distance_R = SpaceUS_R();
    lcd.setRGB(255, 255, 255);
    Mcorner = millis();
  }


void Corner_R_MO()
    {
         int Speed;
         float TD;
         Speed = LongG;
         lcd.setRGB(0, 0, 255);
          right(40);
        angle = IMU_getAngle();
      //replaces complicated quadrantSYS
      TD = calcTD();
        while(angle < TD - correction_R)
        {
           angle = IMU_getAngle();
           Speed = Speed + 5;
            if (Speed > Cornspeed)
            {
              Speed = Cornspeed;
            }
              runMotor(Speed);
              delay(20);
        }
        middle();
      corner = corner + 1;
      stopMotor();
      delay(20);
    runMotor_R(LongG);
    delay(250);
    stopMotor();
    runMotor(LongG);
    Distance_L = SpaceUS_L();
    Distance_R = SpaceUS_R();
    lcd.setRGB(255, 255, 255);
    Mcorner = millis();
  }

void to_Red()
{
   
   Find_Block();
   if (B_x > Bild - 10)
   {
      right(25);
      while (B_x > Picturemiddle_x -10)
      {
        delay(20);
        Find_Block();
      }
      
   }
   else if (B_x < Picturemiddle_x)
   {
      left(25);
      while (B_x > Picturemiddle_x -10)
      {
        delay(20);
        Find_Block();
      }
   }
   middle();
   Distance = SpaceUS_F();
   while (Distance > 30)
   {
     delay(50);
     Distance = SpaceUS_F();
   }
}

void to_Green()
{
   
   Find_Block();
   if (B_x > Picturemiddle_x - 10)
   {
      left(25);
      while (B_x > Picturemiddle_x -10)
      {
        delay(20);
        Find_Block();
      }
      
   }
   else if (B_x < Picturemiddle_x)
   {
      right(25);
      while (B_x > Picturemiddle_x -10)
      {
        delay(20);
        Find_Block();
      }
   }
   middle();
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
  runMotor(Cornspeed);
  // turn until block not in sight
  while(B_ID == 2)
  {
    delay(20);
    Find_Block();
  }
  runMotor(LongG);
  middle();
  
  delay(40);    //straight after delay
  right(25);
  runMotor(Cornspeed);
  // steer until orientation reached
  //TD = IMU_TD_gerade();
  angle = IMU_getAngle();
  while(angle < TD )
  {
      angle = IMU_getAngle();
      delay(20);
  }
  middle();
  runMotor(LongG);
  // drive past
  delay (200);
  lcd.setRGB(255,255,255);

}  



void Evade_R()
{
  float angle;
  float TD;
  int Speed;

  // save orientation and quadrant
  TD = IMU_getAngle();
  right(25);
  runMotor(Cornspeed);
  // drive until block not in sight
  while(B_ID == 1)
  {
    delay(20);
    Find_Block();
  }
  runMotor(LongG);
  middle();
  
  delay(50);    //straight after delay
  left(25);
  runMotor(Cornspeed);
  //TD = IMU_TD_gerade();
  angle = IMU_getAngle();
  while(angle > TD /*- correction_L*/)
  {
      angle = IMU_getAngle();
      delay(20);
  }
  middle();
  runMotor(LongG);
  // drive past
  delay (200);
  lcd.setRGB(255,255,255);
  
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

void setup() {
Wire.begin();
Serial.begin(115200);
//LCD SETUP
  pinMode(Button, INPUT); // button
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);

//Kamera initialiesieren
  Serial.println("Cam Start");
  Kamera_Start();
  Serial.println("Cam Started");

//------------------------------------------------
//from Steering.h
servosetup();

  //setup from ultraschall.h
  ultraschallstart();

  //initalises motor pinmodes from DCmotor.h
  motorsetup();

  delay(5000); // wait for 5 seconds

  //gyro setup gyro.h
  gyrostart();  


 // distance to wall
 Distance = SpaceUS_F(); 
 Distance_L = SpaceUS_L();
 Distance_R = SpaceUS_R();

  //detects block and prints it on the LCD
  Find_Block();
  lcd.clear();



  
  //corner count
   angle = IMU_getAngle(); 

    lcd.setCursor(0,0);
    lcd.print(Distance_L);
    lcd.print("  ");
    lcd.print(Distance);
    lcd.print("  ");
    lcd.print(Distance_R);
    lcd.setCursor(0,1);
    lcd.print(angle);
    lcd.print("  ");    
    lcd.print(TD);
    lcd.print("  ");
    lcd.print(B_ID);

  //setup Done, switch AmpGe == press the button
  lcd.setRGB(255, 130, 0);

  while (digitalRead(Button) == LOW) 
  { 
    // wait for button press
    delay(50);
  }

  lcd.clear();
  
  //light goes out
  lcd.setRGB(255, 255, 255);  

  // saves current time
  start_time = millis();
  Mcorner = millis() - ZKurve;

  //Steering middle  
  middle();
  
  //slow start
   runMotor(LongG);

 }
//Done

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

  int zufall;
  // check for block
  Find_Block();
  Distance = SpaceUS_F();
  
  // show block on LCD 
  if (B_ID == 1)
  {
	 lcd.setRGB(255,0,0);
  }
  else if (B_ID == 2)
  {
	  lcd.setRGB(0,255,0);
  }
  else
  {
	  lcd.setRGB(255,255,255);
  }

   // red: check if block in sight
  if ((B_ID == 1) && (B_x > 80) && (B_Unterkante > 150))
  {
     //if too close = slight reverse
    if (B_Unterkante > 200)
    {
      runMotor_R(180);
      delay(500);
      stopMotor();
      runMotor(LongG);
    }
     //red: evade right
	  Evade_R();
  }

  else if ((B_ID == 2) && (B_x < 240) && (B_Unterkante > 150))
  {
	  //if too close = slight reverse
	  if (B_Unterkante > 200)
    {
      runMotor_R(180);
      delay(500);
      stopMotor();
      runMotor(LongG);
    }
      //green: evade left
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
	
    //infront wall
    if (Distance < 25) // close to wall
    {
      
      stopMotor();
      
        
         if(millis() - Mcorner > ZKurve) // corner detected
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
              zufall = random(2);
              if (zufall == 0)
              {
                TD = 'R';
              }
              else
              {
                TD = 'L';
              }
          }
      }

      lcd.setCursor(0,0);
      lcd.print(TD);
      delay(500);
      // check: turning or curve
      if (TD == 'R')
      {
        Distance_L = SpaceUS_L();
        if (Distance_L > 35)
        {
          wenden_R();
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
          Corner_R_MO();
          
        }
      }
      else // TD == L
      {
        
        Distance_R = SpaceUS_R();
        if (Distance_R > 35)
        {
          wenden_L();
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
          corner_L_MO();
        }
        stopMotor();
        lcd.setRGB(255,255,255);  //End of curve, turning
        delay(100);
        runMotor(LongG);
        }
      }//end of curve detected
      
      else
      {
        //emergency brake if obstacle not detected
        runMotor_R(180);
        delay(2000);
        stopMotor();
        delay(500);
        runMotor(NormG);
      }

    } //end of distance < 25   

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