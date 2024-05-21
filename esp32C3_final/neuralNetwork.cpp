#include "neuralNetwork.h"
#include "Arduino.h"

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


//la signoide e la sua derivata
float sigmoid(float x) {
  return (1/(1 + exp(-x)));
}
float dSigmoid(float x) {
  return (x*(1-x));
}