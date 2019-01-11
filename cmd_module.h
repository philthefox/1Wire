/*
 * cmd_module.h
 *
 *  Created on: 27.11.2018
 *      Author: Linus
 */

#ifndef HEADER_CMD_MODULE_H_
#define HEADER_CMD_MODULE_H_

#include "general.h"

int CMDreadScratch(ScratchPad *SpadOut );

int CMDskipRom(void);

int CMDselectSlave(ROM slaveRom);

void initCMD(void);

int CMDreset(void);

int CMDreadROM(ROM *readRom);

int CMDmeasure(void);

int CMDautoDetectROMS(int size, ROM romList[size], int *numberOfElements);




#endif /* HEADER_CMD_MODULE_H_ */
