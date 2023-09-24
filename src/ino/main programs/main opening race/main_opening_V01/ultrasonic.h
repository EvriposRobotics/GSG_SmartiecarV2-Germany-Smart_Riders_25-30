#define    VELOCITY_TEMP(temp) ( ( 331.5 + 0.6 * (float)( temp ) ) * 100 / 1000000.0 ) // The ultrasonic velocity (cm/us) compensated by temperature

int16_t trigechoPin_F = 11;
int16_t trigechoPin_R = 12;
int16_t trigechoPin_L = 13;
uint16_t distance;
uint32_t pulseWidthUs;

//empty function
void ultrasonicstart()
{

}

int SpaceUS_F()
{
  int16_t  dist, temp;
  pinMode(trigechoPin_F,OUTPUT);
  digitalWrite(trigechoPin_F,LOW);

  digitalWrite(trigechoPin_F,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigechoPin_F,LOW);

  pinMode(trigechoPin_F,INPUT);
  pulseWidthUs = pulseIn(trigechoPin_F,HIGH);

  distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;
  return int(distance);
}

int SpaceUS_L()
{
  int16_t  dist, temp;
  pinMode(trigechoPin_L,OUTPUT);
  digitalWrite(trigechoPin_L,LOW);

  digitalWrite(trigechoPin_L,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigechoPin_L,LOW);

  pinMode(trigechoPin_L,INPUT);
  pulseWidthUs = pulseIn(trigechoPin_L,HIGH);

  distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;
  return int(distance);
}

int SpaceUS_R()
{
  int16_t  dist, temp;
  pinMode(trigechoPin_R,OUTPUT);
  digitalWrite(trigechoPin_R,LOW);

  digitalWrite(trigechoPin_R,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigechoPin_R,LOW);

  pinMode(trigechoPin_R,INPUT);
  pulseWidthUs = pulseIn(trigechoPin_R,HIGH);

  distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;
  return int(distance);
}