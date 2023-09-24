/*temporarily deactivated
#include "ultrasonic.h*/

// The ultrasonic velocity (cm/us) compensated by temperature
#define    VELOCITY_TEMP(temp)       ( ( 331.5 + 0.6 * (float)( temp ) ) * 100 / 1000000.0 )


//US sensor pin definiton
#define trigechoPin_F 8

#define trigechoPin_L A2

#define trigechoPin_R A0


uint16_t distance;

uint32_t pulseWidthUs;



int SpaceUS_F()
{
        // Sending the trigger pulse
        int16_t  dist, temp;
        pinMode(trigechoPin_F,OUTPUT);
        digitalWrite(trigechoPin_F,LOW);
        delayMicroseconds(2);
        digitalWrite(trigechoPin_F,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigechoPin_F,LOW);
        //wait for echo pin to go high
        pinMode(trigechoPin_F,INPUT);
        pulseWidthUs = pulseIn(trigechoPin_F,HIGH);
        distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;
        //gives back as an integer in cm
        return int(distance);
    
}

int SpaceUS_L()
{
        // Sending the trigger pulse
        int16_t  dist, temp;
        pinMode(trigechoPin_L,OUTPUT);
        digitalWrite(trigechoPin_L,LOW);
        delayMicroseconds(2);
        digitalWrite(trigechoPin_L,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigechoPin_L,LOW);
        //wait for echo pin to go high
        pinMode(trigechoPin_L,INPUT);
        pulseWidthUs = pulseIn(trigechoPin_L,HIGH);
        distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;
        //gives back as an integer in cm
        return int(distance);
    
}

int SpaceUS_R()
{
        // Sending the trigger pulse
        int16_t  dist, temp;
        pinMode(trigechoPin_R,OUTPUT);
        digitalWrite(trigechoPin_R,LOW);
        delayMicroseconds(2);
        digitalWrite(trigechoPin_R,HIGH);
        delayMicroseconds(10);
        digitalWrite(trigechoPin_R,LOW);
        //wait for echo pin to go high
        pinMode(trigechoPin_R,INPUT);
        pulseWidthUs = pulseIn(trigechoPin_R,HIGH);
        distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;
        //gives back as an integer in cm
        return int(distance);
}

//empty for sensor type compatibility
void ultrasonicstart()
{
        return;
}