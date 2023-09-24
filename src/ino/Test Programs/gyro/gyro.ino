// Required libraries
#include "rgb_lcd.h"
#include ""

void setup()
{
    lcd.begin(16,2);    
    
    // Set the color of the lcd
    lcd.setRGB(255,255,255);
    
    // Initialize and calibrate the gyroscope
    startGyroscope(); 
    
}

void loop()
{
    // get the angle from the IMU
    float angle = IMU_getAngle();
    // Display the angle on the LCD
    lcd.setCursor(0,0);
    lcd.print("Angle: ");
    lcd.print(angle);
    lcd.print("   ");

}
