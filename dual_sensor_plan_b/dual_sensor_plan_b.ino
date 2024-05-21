#include <WiFiS3.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <VL53L0X.h>

bool debug = true;
bool notified = false;

//wify credentials
char* ssid = "FavouriteGuest";
char* pass = "80546697237239223776";
char* mqtt_serv = "192.168.178.107";

const char* mqtt_username = "mqtt";  //MQTT username
const char* mqtt_password = "Domus!!";  //MQTT password

const char* clientID = "fotocellula1";  //"first_message"
//const char* mqtt_server = "192.168.178.107";
//IPAddress ip(192, 168, 1, 85);
IPAddress server(192, 168, 178, 107);

WiFiClient espClient;
//PubSubClient client(espClient);

long lastReconnectAttempt = 0;

VL53L0X sensor1;
VL53L0X sensor2;

#define XSHUT1 7
#define XSHUT2 6

unsigned short lettura[2];
int distanzaPorta[2];

void callback(char* topic, byte* payload, unsigned int length) {
  //String toWrite;

  String myPayload;
  if (debug) {
    Serial.print("topic: ");
    Serial.print(topic);
    Serial.print(" payload: ");
  }
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    myPayload += (char)payload[i];
  }
  if (debug){ Serial.print(myPayload); Serial.println();}
  //smistaTopic(topic, myPayload);
}

PubSubClient client(mqtt_serv, 1883, callback, espClient);

boolean reconnect() {
  if (client.connect(clientID, mqtt_username, mqtt_password)) {}
  return client.connected();
}

void updateLettura(){
  lettura[0] = sensor1.readRangeContinuousMillimeters();
  lettura[1] = sensor2.readRangeContinuousMillimeters();
  while(lettura[0] == 65535 || lettura[0] == 8190){lettura[0] = sensor1.readRangeContinuousMillimeters();}
  while(lettura[1] == 65535 || lettura[1] == 8190){lettura[1] = sensor2.readRangeContinuousMillimeters();}

  if(debug) Serial.println(String(lettura[0]) + " " + String(lettura[1]));

}

void setup()

{

  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT1, LOW);

  delay(500);  // obbligatorio per stabilizzare i trasitori

  Wire.begin();
  Serial.begin(115200);

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


  //Connect to WiFi network
  Serial.println("");
  Serial.println("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  Serial.println(WiFi.status());

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi connected, IP: ");
  Serial.println(WiFi.localIP());

  //Set MQTT Server details
  client.setServer(mqtt_serv, 1883);


  updateLettura();
  distanzaPorta[0] = lettura[0];
  distanzaPorta[1] = lettura[1];
}


void loop()

{

  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (debug) Serial.println("cerco di riconettermi");
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
        if (debug) Serial.println("riconnesso ;)");
        // ---------------------------------------------------------------------->subscribe
        //subscribeToTopic();
      }
    }
  } else {
    // Client connected
    client.loop();
    
  }
    // entrato -> 0,0   0,1  1,1  1,0  0,0 -> da implememtare
    updateLettura();
    while(lettura[0] < distanzaPorta[0]-200 && notified == false)
    {
      updateLettura();
      if(lettura[1] < distanzaPorta[1]-200 ){
        client.publish("fotocellula1/direction", "entrato");
        notified = true;
        delay(100);

      }
      else{}
    }
    updateLettura();
    while(lettura[1] < distanzaPorta[1]-200 && notified == false)
    {
      updateLettura();
      if(lettura[0] < distanzaPorta[0]-200){
        client.publish("fotocellula1/direction", "uscito");
        notified = true;
        delay(100);

      }
    }
    if(lettura[1] > distanzaPorta[1]-200 && lettura[0] > distanzaPorta[0]-200 ){
      notified = false;
    }

}


