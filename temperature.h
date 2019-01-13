/*
 * TempSensor.h
 */

#ifndef HEADER_TEMPSENSOR_H_
#define HEADER_TEMPSENSOR_H_

#include "general.h"

void initTemperature(void);

Sensor createSensor(ROM rom);

int detectAllSensors(int arraySize, Sensor sensorList[arraySize], int *anzahlS);

//int TempMeasure(Sensor *sensor);

int readTemperatureFromScratchpad(Sensor *sensor);

int measureAllTemperatures(void);

ROM createROM(void);

#endif /* HEADER_TEMPSENSOR_H_ */
