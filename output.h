
#ifndef HEADER_OUTPUT_H_
#define HEADER_OUTPUT_H_

#include <stdint.h>
#include "general.h"

void initTemperatureOutput(void);
void printTemperatures(Sensor sensor, int nr);
void printROMs(Sensor sensor, int nr);

#endif /* HEADER_OUTPUT_H_ */
