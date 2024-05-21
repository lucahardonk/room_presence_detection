#include "menu_library.h"
#include "wifiMqtt.h"
#include "sensors.h"
#include "neuralNetwork.h"
#include "fsm.h"
#include <esp_task_wdt.h>
//#include "esp_private/brownout.h"



#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

Stato lastState;
Stato tmp;
Fsm fotocellula1;

PubSubClient client(server, mqttPort, callback, espClient); // Use server and mqttPort from menu_library.h
void setup()

{
 
 
  //watchdog e brownout to be implemented if capable
  //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  /*Serial.println("Configuring WDT...");
  esp_int_wdt_init(esp_task_wdt_reset);//enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch*/
  
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT1, LOW);

  Serial.begin(115200);

  delay(500);  // obbligatorio per stabilizzare i trasitori
  
  //6 sda 7 scl
  Wire.setPins(6, 7); // Set the I2C pins before begin
  //Wire.setClock(400000);
  Wire.begin(); // join i2c bus (address optional for master)
  
  delay(100);
  

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
  //sensor1.setMeasurementTimingBudget(20000);
  delay(100);
  sensor2.startContinuous();
  //sensor2.setMeasurementTimingBudget(20000);
  delay(100);
  Serial.println("sensors_ready!");

  
  while(!Serial);
  ConfigMenu configMenu;
  configMenu.start();


  //Connect to WiFi network
  Serial.println("");
  Serial.println("Connecting to: ");
  Serial.println(configMenu.getSSID());

  WiFi.begin(configMenu.getSSID(), configMenu.getWiFiPassword());
  WiFi.mode(WIFI_STA); //Optional
  WiFi.setTxPower(WIFI_POWER_8_5dBm);

  Serial.println(WiFi.status());

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());

  //Set MQTT Server details
  client.setServer("192.168.10.55", mqttPort);
    // Set the MQTT callback function
  client.setCallback(callback);

  // Set the keep-alive interval for the MQTT client
  client.setKeepAlive(5);

  distanzaPorta = (sensor1.readRangeContinuousMillimeters()+sensor2.readRangeContinuousMillimeters())/2;
  
  fotocellula1.stato = EMPTY;
  subscribeToTopic();
  resetActionVector();

  //-------------------------------------------------------> ai setup
    //init pesi
  if(train){
    initw();
    
    Serial.println("Siate pazienti... ci vuole molto!");  
    //Fase di ISTRUZIONE della RETE
    for (long n = 0; n < learning_cycles; n++) { 
        //sorteggio un set di apprendimento
        int idSet = random(0, numTrainingSets);
        //lo propago
        propagate(idSet);
        //backpropagation
        learn(idSet);     
    }
    Serial.println("training Ready!");
  }
  //------------------------------------------------------------------> ai setup
  
}

 long lastReconnectAttempt = 0;

void loop()
{

 if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 1000) {
      lastReconnectAttempt = now;
      if (debug) Serial.println("cerco di riconettermi");
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
        if (debug) Serial.println("riconnesso ;)");
        // ---------------------------------------------------------------------->subscribe
        subscribeToTopic();
      }
    }
  } else {
    // Client connected*/
    client.loop();
    
  }

  /*
  
    tmp = getFsmStato(&fotocellula1);
    if(lastState != tmp){
      lastState = tmp;
      ripetizione = 0;
      giaCalcolato = false;
      actionVector[saveIndex] = int(tmp);
      if(saveIndex<numOfElementsInActionVector){
        saveIndex++;
      }
      switch(lastState){
        case EMPTY: Serial.println("EMPTY "); break;
        case DX: Serial.println("DESTRA "); break;
        case SX: Serial.println("SINISTRA"); break;
        case OCCUPIED: Serial.println("OCCUPIED"); break;
      }
    }
    finishAction(3, EMPTY); 
    
    tmp = getFsmStato(&fotocellula1);
    if(lastState != tmp){
    Serial.println(tmp);
    lastState = tmp;
    }
    */
    updateLettura();
    delay(10);

    //esp_task_wdt_reset();

}

