/*
 * HAL.h
 *
 *  Created on: 26.11.2018
 *  Author: Linus
 */



#ifndef HEADER_HAL_H_
#define HEADER_HAL_H_


#include <stdint.h>
#include "general.h"

void initHAL(void);	
int wait(uint32_t uSecs);
void setDPin(int state);
void DSetMode(int mode);
void setRW(int read);
int reset(void) ;

int writeBit(BYTE out);
int writeByte(BYTE out);
int writeBytes(int numberOfBytes, BYTE out[numberOfBytes]);

int readByte(int numberOfBytes, BYTE input[numberOfBytes]);
int readBit(void);
#endif
