#include <WiFiS3.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <VL53L0X.h>

bool debug = true;
bool train = true;
bool notified = false;
void callback(char* topic, byte* payload, unsigned int length);

//wify credentials
char* ssid = "FavouriteGuest";
char* pass = "80546697237239223776";
char* mqtt_serv = "192.168.178.107";

const char* mqtt_username = "mqtt";  //MQTT username
const char* mqtt_password = "Domus!!";  //MQTT password
const char* clientID = "fotocellula1";  
IPAddress server(192, 168, 178, 107);


WiFiClient espClient;
long lastReconnectAttempt = 0;

PubSubClient client(mqtt_serv, 1883, callback, espClient);

VL53L0X sensor1;
VL53L0X sensor2;

#define XSHUT1 7
#define XSHUT2 6

unsigned short lettura[2];
int distanzaPorta;

int actionVector[5];
int saveIndex = 1;
int ripetizione = 0;
int numOfElementsInActionVector = sizeof(actionVector)/sizeof(int);
bool giaCalcolato = true;
enum Stato{ EMPTY, DX, SX, OCCUPIED};

struct Fsm{
  Stato stato;

};

Stato lastState;
Stato tmp;

void callback(char* topic, byte* payload, unsigned int length) {
  //String toWrite;

  String myPayload;
  String myTopic;
  if (debug) {
    Serial.print("topic: ");
    Serial.print(topic);
    Serial.print(" payload: ");
  }
  myTopic = topic;
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    myPayload += (char)payload[i];
  }
  if (debug) Serial.println(myPayload);
  //--------------------------------------> 
  
  if (myTopic == "/sensore1/train"){ 
    String risultato = "{";
    for(int i=0; i<numOfElementsInActionVector-1; i++){
      risultato += String(actionVector[i]); 
      risultato += ".0f,";
    }
    risultato += String(actionVector[numOfElementsInActionVector-1]); 
    risultato += ".0f},";
    if(myPayload == "dx" && !train){client.publish("/sensore1/train/risposta/dx",risultato.c_str(), false);resetActionVector();}
    else if(myPayload == "sx" && !train){client.publish("/sensore1/train/risposta/sx",risultato.c_str(), false);resetActionVector();}
    
  }
  if (myTopic == "/sensore1/calculate"){
    String result;
    Serial.println("calcolo");
    result = String(calculateResult(actionVector,numOfElementsInActionVector));
    client.publish("/sensore1/train/rusiltato", result.c_str());
    if(train) {resetActionVector();}
    
  }
 
}



boolean reconnect() {
  if (client.connect(clientID, mqtt_username, mqtt_password)) {}
  return client.connected();
}
// usata solo in setup, 
void updateLettura(){
  lettura[0] = sensor1.readRangeContinuousMillimeters();
  lettura[1] = sensor2.readRangeContinuousMillimeters();
  while(lettura[0] == 65535 || lettura[0] == 8190){lettura[0] = sensor1.readRangeContinuousMillimeters();}
  while(lettura[1] == 65535 || lettura[1] == 8190){lettura[1] = sensor2.readRangeContinuousMillimeters();}

  if(debug) Serial.println(String(lettura[0]) + " " + String(lettura[1]));

}

Stato getFsmStato(Fsm* fsm){

  if(sensor1.readRangeContinuousMillimeters() < distanzaPorta-200){
    if(sensor2.readRangeContinuousMillimeters() < distanzaPorta-200){fsm->stato = OCCUPIED;}
    else{fsm->stato = SX;}
  }
  else{
    if(sensor2.readRangeContinuousMillimeters() < distanzaPorta-200){fsm->stato = DX;}
    else{fsm->stato = EMPTY;}
  }

  return fsm->stato;

}

Fsm fotocellula1;

//----------------------------------------------------------------------------> ai code

//nodi d'ingresso
static const int numInputs = 5;
//neuroni hidden layer
static const int numHiddenNodes = 4;
//numero neuroni d'uscita
static const int numOutputs = 1;

//gli array con i valori di uscita e i pesi
float hiddenLayer[numHiddenNodes];
float hiddenLayerBias[numHiddenNodes];
float hiddenWeights[numInputs][numHiddenNodes];
float outputLayer[numOutputs];
float outputLayerBias[numOutputs];
float outputWeights[numHiddenNodes][numOutputs];

//learning rate (per non applicare le correzioni ai pesi, "intere" - darebbe instabilità)
float lr = 0.4;

//cosa vogliamo fare apprendere alla rete?
static const int numTrainingSets = 40;
static const long learning_cycles = 100000;
float training_inputs[numTrainingSets][numInputs] = { 
{0.0f,1.0f,3.0f,0.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,3.0f,0.0f,0.0f},
{0.0f,2.0f,3.0f,0.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,0.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,2.0f,1.0f,0.0f,0.0f},
{0.0f,2.0f,1.0f,0.0f,0.0f},
{0.0f,2.0f,3.0f,0.0f,0.0f},
{0.0f,2.0f,3.0f,0.0f,0.0f},
{0.0f,2.0f,1.0f,3.0f,0.0f},
{0.0f,1.0f,3.0f,0.0f,0.0f},
{0.0f,1.0f,3.0f,0.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,1.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,1.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,3.0f,1.0f,2.0f,0.0f},
{0.0f,1.0f,2.0f,0.0f,0.0f},
{0.0f,1.0f,2.0f,0.0f,0.0f},
{0.0f,1.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,2.0f,0.0f,0.0f},
{0.0f,3.0f,2.0f,0.0f,0.0f}
};
float training_outputs[numTrainingSets][numOutputs] = { 
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{1.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f},
{0.0f}
};

//la signoide e la sua derivata
float sigmoid(float x) {
  return (1/(1 + exp(-x)));
}
float dSigmoid(float x) {
  return (x*(1-x));
}

//----------------------------------------------------------------------------> ai code

void setup()

{

  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);

  digitalWrite(XSHUT2, LOW);
  digitalWrite(XSHUT1, LOW);

  delay(500);  // obbligatorio per stabilizzare i trasitori

  Wire.begin();
  Wire.setClock(400000);
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
        subscribeToTopic();
      }
    }
  } else {
    // Client connected
    client.loop();
    
  }
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
    finishAction(3);
}

void subscribeToTopic(){

  client.subscribe("/sensore1/train",false);
  client.subscribe("/sensore1/calculate",false);


}

void resetActionVector(){
  for(int i=0; i<numOfElementsInActionVector; i++){
    actionVector[i] = 0;
  }
}

void finishAction(int soglia){
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





//------------------------------------------------> ai functions


//inizializza i pesi e i bias con numeri casuali (tra 0 e 1)
void initw(){
  randomSeed(analogRead(A3));
  //livello hidden
  for (int j = 0; j < numHiddenNodes; j++) {
    hiddenLayerBias[j] = ((float)random(0,1000))/1000.0;
    for (int k = 0; k < numInputs; k++) {
      hiddenWeights[k][j] = ((float)random(0,1000))/1000.0;
    }
  }
  //livello di uscita
  for (int j = 0; j < numOutputs; j++) {
    outputLayerBias[j] = ((float)random(0,1000))/1000.0;
    for (int k = 0; k < numHiddenNodes; k++) {
      outputWeights[k][j] = ((float)random(0,1000))/1000.0;
    }
  }
}

void propagate(int i){ 
  // scelto il pattern i-esimo, lo propago 
  // Per tutti i nodi Hidden:
  for (int j = 0; j < numHiddenNodes; j++) {
    float activation = hiddenLayerBias[j];
    for (int k = 0; k < numInputs; k++){
      //sommatoria degli ingressi * i pesi
      activation += training_inputs[i][k] * hiddenWeights[k][j];
    }
    //applico la sigmoide
    hiddenLayer[j] = sigmoid(activation);
  }
      
  // per l'uscita (o le uscite)
  for (int j=0; j<numOutputs; j++) {
    float activation=outputLayerBias[j];
    for (int k=0; k<numHiddenNodes; k++) {
      activation += hiddenLayer[k] * outputWeights[k][j];
    }
    outputLayer[j] = sigmoid(activation);
  }
}

void learn(int i) {
  // scelto il pattern i, applico la backpropagation
  
  // 1. parto dall'uscita e propago all'indietro l'errore
  // array con gli errori sulle uscite
  float deltaOutput[numOutputs];
  for (int j = 0; j < numOutputs; j++) {
    //errore preso pari alla diff tra valore ottenuto nella propagazione e 
    //valore desiderato
    float error = (training_outputs[i][j] - outputLayer[j]);
    //questo è il valore da usare per correggere i pesi.
    //l'errore portato all'ingresso del nodo di uscita
    deltaOutput[j] = error * dSigmoid(outputLayer[j]);
  } 
  
  // propago sui nodi hidden
  // array con gli errori sui nodi hidden
  float deltaHidden[numHiddenNodes];
  for (int j=0; j<numHiddenNodes; j++) {
    float error = 0.0f;
    for (int k=0; k<numOutputs; k++) {
      //lo moltiplico per il peso per metterlo allo stesso livello
      //dell'uscita hidden
      error += deltaOutput[k] * outputWeights[j][k];
    }
    //qui riporto l'errore sul nodo hidden all'ingresso del nodo hidden
    deltaHidden[j] = error * dSigmoid(hiddenLayer[j]);
  }

  // 2. ora modifico i pesi
  // per le uscite
  for (int j=0; j<numOutputs; j++) {
    outputLayerBias[j] += deltaOutput[j] * lr;
    for (int k=0; k<numHiddenNodes; k++) {
      //il nuovo peso è pari al valore di uscita del nodo H moltiplicato per 
      //l'errore portato all'ingresso del nodo di uscita
      outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * lr;
    }
  }
  // per i pesi H
  for (int j=0; j<numHiddenNodes; j++) {
    hiddenLayerBias[j] += deltaHidden[j]*lr;
    for(int k=0; k<numInputs; k++) {
      //il nuovo peso è pari al valore presentato all'ingresso moltiplicato per 
      //l'errore H portato all'ingresso del nodo di ingresso
      hiddenWeights[k][j] += training_inputs[i][k] * deltaHidden[j]*lr;
    }
  }
}



float calculateResult(int vector[], int numOfElementsInActionVector){
    for(int i = 0; i < numOfElementsInActionVector; i++){
      training_inputs[0][i] = vector[i];
      Serial.println(vector[i]);
    }
    
    propagate(0);
    Serial.print("Y = ");
    Serial.println(outputLayer[0]);
    delay(100);
    return float(outputLayer[0]);
}
