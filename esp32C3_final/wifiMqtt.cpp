#include "wifiMqtt.h"
#include "menu_library.h" // Include the header file for your configuration menu

void callback(char* topic, byte* payload, unsigned int length) {
  // String toWrite;
  String myPayload;
  String myTopic;
  if (debug) {
    Serial.print("topic: ");
    Serial.print(topic);
    Serial.print(" payload: ");
  }
  myTopic = topic;
  for (int i = 0; i < length; i++) {
    // Serial.print((char)payload[i]);
    myPayload += (char)payload[i];
  }
  if (debug) Serial.println(myPayload);
  //--------------------------------------> 

  if (myTopic == "/sensore1/train") { 
    String risultato = "{";
    for (int i = 0; i < numOfElementsInActionVector - 1; i++) {
      risultato += String(actionVector[i]);
      risultato += ".0f,";
    }
    risultato += String(actionVector[numOfElementsInActionVector - 1]);
    risultato += ".0f},";
    if (myPayload == "dx" && !train) {
      client.publish("/sensore1/train/risposta/dx", risultato.c_str(), false);
      resetActionVector();
    } else if (myPayload == "sx" && !train) {
      client.publish("/sensore1/train/risposta/sx", risultato.c_str(), false);
      resetActionVector();
    }
  }
  if (myTopic == "/sensore1/calculate") {
    String result;
    Serial.println("calcolo");
    result = String(calculateResult(actionVector, numOfElementsInActionVector));
    client.publish("/sensore1/train/rusiltato", result.c_str());
    if (train) {
      resetActionVector();
    }
  }
}

boolean reconnect() {
  if (client.connect(clientID, mqtt_username, mqtt_password)) {}
  return client.connected();
}



void subscribeToTopic() {
  client.subscribe("/sensore1/train", false);
  client.subscribe("/sensore1/calculate", false);
}
