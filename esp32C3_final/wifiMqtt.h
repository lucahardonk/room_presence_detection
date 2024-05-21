#ifndef WIFIMQTT_H
#define WIFIMQTT_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include "fsm.h"
#include "neuralNetwork.h"
#include "sensors.h"
#include "menu_library.h" // Include menu library header

// Remove the static IPAddress server and static int mqttPort definitions

static WiFiClient espClient;

extern void callback(char* topic, byte* payload, unsigned int length);
extern PubSubClient client; // Declaration of the client object

extern boolean reconnect();
extern void subscribeToTopic();

#endif
