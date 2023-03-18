#include <Wire.h>
#include "MAX30105.h"
#include <U8g2lib.h>
#include <ConsentiumThings.h>
#include "heartRate.h"


int buzzerPin = 13; // set the buzzer pin

MAX30105 particleSensor;
ConsentiumThings board;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

const int LM35_PIN = A0; // Pin where LM35 temperature sensor is connected
const char *ssid = "ABBE SALE"; // Add WiFi SSID
const char *pass = "1234alibaba.com"; // Add WiFi password
const char *key = "EZIIOZC8MTPDGWGSHEDSZA"; // Add Write API key
const long interval = 5; // Take 5 seconds of delay

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);



void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, 400000)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

 


  {
  
  pinMode(buzzerPin, OUTPUT); // set the buzzer pin as an output
  tone(buzzerPin, 2000); // play a 1 kHz tone on the buzzer
  delay(500); // wait for 1 second
  noTone(buzzerPin); // stop the tone
  
  }


   u8g2.begin();
   //___ Display "Heritage College" on the first page____
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(15, 20);
  u8g2.print("Heritage Institute ");
  u8g2.setCursor(25, 40);
  u8g2.print("DEPT OF AEIE");

  u8g2.sendBuffer();
  delay(10000);
  u8g2.clearBuffer();

  board.begin(); // Init IoT board
  board.initWiFi(ssid, pass); // Begin WiFi connection

}








void loop()
{
  long irValue = particleSensor.getIR();
  float tempC = (5.0 * analogRead(A0) * 100.0) / 2024.0;
       tempC=ceil(tempC);
      
  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  u8g2.clearBuffer(); // clear the internal memory of the OLED display

  u8g2.setFont(u8g2_font_ncenB08_tr); // set the font to use
  u8g2.setCursor(0, 20); // set the cursor position
  u8g2.print("Heart rate: "); // print the label for the heart rate reading
  u8g2.print(beatAvg); // print the actual heart rate value
  u8g2.print(" BPM"); // print the units for the heart rate value

  u8g2.setCursor(0, 40); // set the cursor position for the temperature reading
  u8g2.print("Temperature: "); // print the label for the temperature reading
  u8g2.print(tempC); // print the actual temperature value
  u8g2.print(" C"); // print the units for the temperature value

  u8g2.sendBuffer(); // transfer the data from the internal memory to the OLED display

  //Serial.print("IR=");
  //Serial.print(irValue);
  //Serial.print(", BPM=");
  //Serial.print(beatsPerMinute);
 
  Serial.print("Avg BPM=");
  Serial.print(beatAvg);
  Serial.print(", Temp=");
  Serial.print(tempC);

  float sensor_val[] = {(float)beatAvg, tempC};
  String info_buff[] = {"HR", "Temperature"};
  int sensor_num = sizeof(sensor_val)/sizeof(sensor_val[0]);

  board.sendREST(key, sensor_num, info_buff, sensor_val, LOW_PRE, interval);

  if (irValue < 50000)
    Serial.print(" No finger?");

  Serial.println();
}
