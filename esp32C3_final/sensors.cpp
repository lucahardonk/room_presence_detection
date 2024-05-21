#include "sensors.h"
// usata solo in setup, 
void updateLettura(){
  lettura[0] = sensor1.readRangeContinuousMillimeters();
  lettura[1] = sensor2.readRangeContinuousMillimeters();
  if(lettura[0] == 65535 || lettura[0] == 8190){lettura[0] = sensor1.readRangeContinuousMillimeters();}
  if(lettura[1] == 65535 || lettura[1] == 8190){lettura[1] = sensor2.readRangeContinuousMillimeters();}

  if(debug) Serial.println(String(lettura[0]) + " " + String(lettura[1]));
  
}