//includes
#include "rgb_lcd.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define BNO055_SAMPLERATE_DELAY_MS (10)

//gyroscope
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);
sensors_event_t event;

//lcd object
 rgb_lcd lcd;

int rounds;
int orientation;
int corners;
int quadrant;

//target direction
char TD = 'K';


/************************* Imu Functions *************************/

byte IMU_start()
{
  if(!bno.begin())
  {
    lcd.setCursor(0,0);
    lcd.print("IMU ERR!");
    while(1);
  }  
   return 0;
}

void IMU_calibrate()
{
  uint8_t system, gyroscope, accel, mag;
  system = gyroscope = accel = mag = 0;
  bno.getCalibration(&system, &gyroscope, &accel, &mag);
  bno.setMode(OPERATION_MODE_IMUPLUS);
  bno.setExtCrystalUse(true);
}

float IMU_getAngle()
{
  float orientation = 0;
  float offset;
  float angle = 0;
  bno.getEvent(&event);

  rounds = corners/4;
  quadrant = corners % 4;
  orientation = event.orientation.x;

  if(TD == 'R')
  {    
    if((quadrant == 0) && (orientation > 270.0))
    {
      angle = orientation - 360.0;
    }    
    else if((quadrant == 3) && (orientation < 90.0))
    {
      angle = orientation + 360.0;
    }    
    else
    {
      angle = orientation;
    }
  }
  else
  {
    if((quadrant == 0) && (orientation < 90.0))
    {
      angle = orientation + 360.0;
    }    
    else if((quadrant == 3) && (orientation > 270.0))
    {
      angle = orientation - 360.0;
    }    
    else
    {
      angle = orientation;
    }
  }
  return angle;
}

float IMU_straight_angle_TD()
{
  // calculate straight angle in the current quadrant
  float TD = 0.0;
  quadrant = corners % 4;
  
  if(quadrant == 0)
  {
    TD = 360.0;
  }
  else if(quadrant == 1)
  {
    TD = 90.0;
  }
  else if(quadrant == 2)
  {
    TD = 180.0;
  }
  else if(quadrant == 3)
  {
    TD = 270.0;
  }    
  return TD; 
}

void startGyroscope()
{    
  byte status = IMU_start();
  while(status!=0){delay(10);} // stop everything if could not connect to MPU6050
  IMU_calibrate(); // gyroscope and accelerometer
}
