
#ifndef HEADER_OUTPUT_H_
#define HEADER_OUTPUT_H_

#include <stdint.h>
#include "general.h"

void initTemperatureOutput(void);
void printTemperature(Sensor sensor, int nr);
void printROMs(int * anzahlS, int maxSensors, Sensor sensorList[maxSensors]);

#endif /* HEADER_OUTPUT_H_ */
