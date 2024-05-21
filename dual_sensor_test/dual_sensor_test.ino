#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor1;
VL53L0X sensor2;

#define XSHUT1 7
#define XSHUT2 6

unsigned short distance1;
unsigned short distance2;


void setup()

{

  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT1, LOW);

  delay(500); // obbligatorio per stabilizzare i trasitori

  Wire.begin();
  Serial.begin (115200);

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
  Serial.print("1: ");
  Serial.print(distance1);

  Serial.print(" ");

  distance2=sensor2.readRangeContinuousMillimeters();
  Serial.print("2: ");
  Serial.print(distance2);

  Serial.println(" ");


  //delay(100);

}
