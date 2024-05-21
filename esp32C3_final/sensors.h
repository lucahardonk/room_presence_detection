#ifndef SENSORS_H
#define SENSORS_H
#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

static bool debug = true;

static VL53L0X sensor1;
static VL53L0X sensor2;

#define XSHUT1 3
#define XSHUT2 4

static unsigned short lettura[2];
static int distanzaPorta;

#endif