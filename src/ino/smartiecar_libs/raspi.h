//communication funcs with raspberry pi and arduino

void raspi_handshake()
{
    Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.setRGB(255, 0, 0);
    lcd.setCursor(0,0);
    lcd.print("Wait for Raspi");
    Serial.print("ready");
    delay(1000);

        //while no data is available
        while (Serial.available() == 0)
        {
            //delay
            delay(100);
        }

    //read answer
    lcd.clear();
    String data = Serial.readStringUntil('\n');
    lcd.setCursor(0,0);
    lcd.print(data);
    data = "";
}

String raspi_get_data()
{
    //empty string
    String data = "";
    
    Serial.print("req"); //request data from raspi

        //while no data is available
        while (Serial.available() == 0)
        {
            //delay
            delay(10);
        }

    //read answer
    data = Serial.readStringUntil('\n');
    return data;    //blocked reading functions
}