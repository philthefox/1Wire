
#ifndef HEADER_OUTPUT_H_
#define HEADER_OUTPUT_H_

#include <stdint.h>
#include "general.h"

void initTempOutput(void);
void printTemp(tempSensor sensor, int nr);
void printROM(tempSensor sensor, int nr);

#endif /* HEADER_OUTPUT_H_ */