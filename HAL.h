
#ifndef HEADER_HAL_H_
#define HEADER_HAL_H_


#include <stdint.h>
#include "general.h"

void initHAL(void);	
int wait(uint32_t uSecs);
int reset(void) ;
void setPin(int state, int pin);
void setPinMode(int mode, int pin);

int writeBit(BYTE out);
int writeByte(BYTE out);
int writeBytes(int numberOfBytes, BYTE out[numberOfBytes]);

int readByte(int numberOfBytes, BYTE input[numberOfBytes]);
int readBit(void);

#endif
