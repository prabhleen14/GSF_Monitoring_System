#include <SPI.h>
#include <ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ThingSpeak.h>
const char* ssid = "AndroidAP53C1";
const char* password = "bmav@123";
#include <ThingerESP8266.h>
ThingerESP8266 thing("prabhleen14", "NM2", "123456789");

//Dustbin reading via ultrasonic sensor
#define pingTrigPin D4
#define pingEchoPin D3
#define ledg D7
//Fire reading via flame sensor
  #define ledf D8
  #define flamePin D1
int Flame = HIGH;


// Sewage leakage reading via water level Sensor 
#define waterPin A0
#define ledw D0
// Value for storing water level
int water = 0;

WiFiClient client;
unsigned long myChannelNumber = 1348627;
const char* myWriteAPIKey = "HLHLBOAOLE22PVO4";
  
  void setup() {   
  Serial.begin(115200);  
  delay(10);
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client); 
  pinMode(ledg, OUTPUT);  //declare led as output
   thing.add_wifi("AndroidAP53C1", "bmav@123");
 //     thing.add_wifi("AndroidAP53C1", "bmav@123");
  Serial.println("entering the gates"); 

 pinMode(ledf, OUTPUT);
  pinMode(flamePin, INPUT);


 // pinMode(sensorPower, OUTPUT);
 pinMode(ledw,OUTPUT);
 pinMode(waterPin, INPUT);
  }   
  void loop()   
  {   
  long duration, cm;   
  pinMode(pingTrigPin, OUTPUT);   
  digitalWrite(pingTrigPin, LOW);   
  delayMicroseconds(2);   
  digitalWrite(pingTrigPin, HIGH);   
  delayMicroseconds(5);   
  digitalWrite(pingTrigPin, LOW);   
  pinMode(pingEchoPin, INPUT);   
  duration = pulseIn(pingEchoPin, HIGH);   
  cm = microsecondsToCentimeters(duration);   
  if(cm<=10 && cm>0)   
  {   
  int d= map(cm, 1, 100, 20, 2000);   
  digitalWrite(ledg, HIGH);   
  delay(100);   
  digitalWrite(ledg, LOW);   
  delay(d);  
  thing.handle();
        thing.call_endpoint("email_gmail");
       delay(5000);
  }   
  Serial.print(cm);    
  Serial.print("cm");   
  Serial.println();   
  delay(100);  

  Flame = digitalRead(flamePin);
  if (Flame== LOW)
  {
    Serial.println("Fire!!!");
    digitalWrite(ledf, HIGH);
    delay( 1000);
    thing.handle();
        thing.call_endpoint("email_fire");
       delay(5000);
  }
  else
  {
    Serial.println("No worries");
    digitalWrite(ledf, LOW);
  }


water = analogRead(waterPin);
  if (water>10)
  {
    Serial.println("Leakage Detected ");
    digitalWrite(ledw,HIGH);
    delay( 1000);
    thing.handle();
        thing.call_endpoint("email_water");
       delay(5000);
  }
  else
  {
    Serial.println("No Leakage detected ");
    digitalWrite(ledw, LOW);
    delay( 1000);
  }

    ThingSpeak.writeField(myChannelNumber, 1, cm , myWriteAPIKey);
    ThingSpeak.writeField(myChannelNumber, 2, Flame , myWriteAPIKey);
   ThingSpeak.writeField(myChannelNumber, 3, water, myWriteAPIKey);
    
   
   delay(100);
  }   
 long microsecondsToCentimeters(long microseconds)   
  {   
 return microseconds / 29 / 2;   
 } 

 
