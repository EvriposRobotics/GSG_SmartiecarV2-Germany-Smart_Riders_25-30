#include <Servo.h>
#include "rgb_lcd.h"
// Erstelle ein Servo-Objekt
Servo servo;
int colorR = 255;
 int colorG = 0;
 int colorB = 0;
  rgb_lcd lcd;
unsigned long start_time;

int mitte = 63;  //winkel eintragen//
int rechts = 120;
int links = 25;

void setup() 
{
  // Verbinde das Servo an Pin 9
   lcd.begin(16, 2);
  lcd.setRGB(255, 0, 0);
  servo.attach(3);
lcd.clear();
  servo.write(mitte);
}

void loop() 
{
delay(5000); 
servo.write(rechts);  //rechts
lcd.clear();
lcd.setCursor(0,0);
lcd.print(rechts);
delay(5000);

servo.write(mitte);  //mitte
lcd.clear();
lcd.setCursor(0,0);
lcd.print(mitte);
delay(5000);

servo.write(links);  //links
lcd.clear();
lcd.setCursor(0,0);
lcd.print(links);
}
