//Ultrasonic section

// Pin definitions

//Front sensor
#define TRIG_PIN_V  8
#define ECHO_PIN_V  9
#define BAUD_RATE  9600

//Left sensor
#define TRIG_PIN_L  A2
#define ECHO_PIN_L  A3

//Right sensor
#define TRIG_PIN_R  A0
#define ECHO_PIN_R  A1


//US custom function Front
int SpaceUS_F(){
    // Sending the trigger pulse
  digitalWrite(TRIG_PIN_V, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_V, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_V, LOW);
  
  // Measurement of the echo pulse duration
  const unsigned long duration = pulseIn(ECHO_PIN_V, HIGH);
  
  // Calculation of the distance based on the measured duration
  int distance = duration / 29 / 2;
  
  delay(20);

   return distance;
}

//US custom function Right
int SpaceUS_R(){
    // Sending the trigger pulse
  digitalWrite(TRIG_PIN_R, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_R, LOW);
  
  // Measurement of the echo pulse duration
  const unsigned long duration = pulseIn(ECHO_PIN_R, HIGH);
  
  // Calculation of the distance based on the measured duration
  int distance = duration / 29 / 2;

  delay(20);
  
   return distance;
}

//US self function left
int SpaceUS_L(){
    // Sending the trigger pulse
  digitalWrite(TRIG_PIN_L, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_L, LOW);
  
  // Measurement of the echo pulse duration
  const unsigned long duration = pulseIn(ECHO_PIN_L, HIGH);
  
  // Calculation of the distance based on the measured duration
  int distance = duration / 29 / 2;
  
  delay(20);

   return distance;
}

void ultrasonicstart()
{
// Initialisierung der US Pins Vorne
  pinMode(TRIG_PIN_V, OUTPUT);
  pinMode(ECHO_PIN_V, INPUT);

  // Initialisierung der US Pins Links
  pinMode(TRIG_PIN_L, OUTPUT);
  pinMode(ECHO_PIN_L, INPUT);

  // Initialisierung der US Pins Rechts
  pinMode(TRIG_PIN_R, OUTPUT);
  pinMode(ECHO_PIN_R, INPUT);
}
