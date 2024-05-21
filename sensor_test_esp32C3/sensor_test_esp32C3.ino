/*#include <ESPAsync_WiFiManager.h>
#include <ESPAsync_WiFiManager.hpp>
*/

#include <Wire.h>
#include <VL53L0X.h>
#include <PubSubClient.h>


//#include <esp_wifi.h>  
//wify credentials
/*
char* ssid = "FavouriteGuest";
char* pass = "80546697237239223776";
char* mqtt_serv = "192.168.178.107";
*/
const char* mqtt_username = "mqtt";  //MQTT username
const char* mqtt_password = "Domus!!";  //MQTT password
const char* clientID = "fotocellula1";  
IPAddress server(192, 168, 178, 107);

//WiFiClient espClient;
long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length);
//PubSubClient client(mqtt_serv, 1883, callback, espClient);

VL53L0X sensor1;
VL53L0X sensor2;

#define XSHUT1 A0
#define XSHUT2 A1

unsigned short distance1;
unsigned short distance2;


void setup()
{  
  Serial.begin (115200);
  Wire.begin();
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT1, LOW);

  delay(500); // obbligatorio per stabilizzare i trasitori

  
  //Wire.setClock(400000);
  

  //------------------------------------------------------------>
  
  digitalWrite(XSHUT2, HIGH);

  delay(150);

  Serial.println("inizializzo 2");

  sensor2.init(true);

  Serial.println("set adress 2");

  delay(100);

  sensor2.setAddress((uint8_t)02);

  //---------------------------------------------------------------->
  delay(150); 
  
  digitalWrite(XSHUT1, HIGH);

  delay(150);

  sensor1.init(true);

  Serial.println("inizializzo 1");

  delay(100);

  sensor1.setAddress((uint8_t)01);

  Serial.println("set adress 1");

//-------------------------------------------------------->
  Serial.println("set and ready ;)");

  sensor1.startContinuous();
  delay(100);
  sensor2.startContinuous();
  delay(100);
  Serial.println("Go!");



}

void loop()

{

 

  distance1=sensor1.readRangeContinuousMillimeters();
  //Serial.print("1: ");
  Serial.print(distance1);

  Serial.print(" ");

  distance2=sensor2.readRangeContinuousMillimeters();
  //Serial.print("2: ");
  Serial.print(distance2);

  //delay(5);
  Serial.println();


  //delay(100);

}
