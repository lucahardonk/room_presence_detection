#include "fsm.h"
#include "sensors.h"
#include "wifiMqtt.h"







void resetActionVector(){
  for(int i=0; i<numOfElementsInActionVector; i++){
    actionVector[i] = 0;
  }
}


void finishAction(int soglia, Stato tmp){
  if(tmp == EMPTY){
    ripetizione++;
    if(ripetizione >= soglia && giaCalcolato == false) {
      ripetizione = soglia; 
      saveIndex = 1;

      String result;
      Serial.println("calcolo");
      result = String(calculateResult(actionVector,numOfElementsInActionVector));
      client.publish("/sensore1/train/rusiltato", result.c_str());
      if(train){resetActionVector();}
      giaCalcolato = true;
    
    }

  }
}

Stato getFsmStato(Fsm* fsm){
  a = b = 0;
  //faster state calculation
  
  if(sensor2.readRangeContinuousMillimeters() < distanzaPorta-100){ b = 2;}
  if(sensor1.readRangeContinuousMillimeters() < distanzaPorta-100){ a = 1;}
  

  switch(a+b){
    case 1: return fsm->stato = DX; break;
    case 2: return fsm->stato = SX; break;
    case 3: return fsm->stato = OCCUPIED; break;
    default: return fsm->stato = EMPTY; break;
  }
}


