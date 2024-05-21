#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <Arduino.h>

static bool train = !true;
//nodi d'ingresso
static const int numInputs = 5;
//neuroni hidden layer
static const int numHiddenNodes = 4;
//numero neuroni d'uscita
static const int numOutputs = 1;

//gli array con i valori di uscita e i pesi
static float hiddenLayer[numHiddenNodes];
static float hiddenLayerBias[numHiddenNodes];
static float hiddenWeights[numInputs][numHiddenNodes];
static float outputLayer[numOutputs];
static float outputLayerBias[numOutputs];
static float outputWeights[numHiddenNodes][numOutputs];

//learning rate (per non applicare le correzioni ai pesi, "intere" - darebbe instabilità)
static float lr = 0.4;
//cosa vogliamo fare apprendere alla rete?
static const int numTrainingSets = 40;
static const long learning_cycles = 100000;

extern void initw();
extern void propagate(int i);
extern void learn(int i);
extern float calculateResult(int vector[], int numOfElementsInActionVector);
extern float sigmoid(float x);
extern float dSigmoid(float x);

static float training_inputs[numTrainingSets][numInputs] = { 
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
static float training_outputs[numTrainingSets][numOutputs] = { 
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

#endif
