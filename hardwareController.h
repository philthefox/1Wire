
#ifndef HEADER_CMD_MODULE_H_
#define HEADER_CMD_MODULE_H_

#include "general.h"

int readScratch(ScratchPad *SpadOut );

int skipRom(void);

int selectSlave(ROM slaveRom);

void initHardwareController(void);

int resetHardware(void);

int measure(void);

int detectSensors(int size, ROM romList[size], int *numberOfElements);

#endif /* HEADER_CMD_MODULE_H_ */
