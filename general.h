/*
 * general.h
 *
 *  Created on: 26.11.2018
 *      Author: Linus
 */

#ifndef HEADER_GENERAL_H_
#define HEADER_GENERAL_H_

#include <stdint.h>

//--------------------- Data Types -----------------
typedef int8_t CHAR;
typedef int16_t SHORT;
typedef int32_t LONG;
typedef uint32_t DWORD;
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;

//---------------------------Structs -----------------------

typedef struct ROMNUM{
	BYTE FamilyCode;
	BYTE Serialnumber[6];
	BYTE crc;
}ROM;

typedef struct  __attribute__((__packed__)) SCRATCHPAD{
WORD Temp;
BYTE ThUsrByte;
BYTE TlUsrByte;
BYTE configR;
BYTE reserved[3];
BYTE crc;
}ScratchPad;


//------------------------ HAL Settings -----------------
#define BUS_PORT GPIOG

#define DATA_PIN 0
#define POWER_PIN 1

#define SET_REG BSRRL
#define RESET_REG BSRRH

#define INPUT_MODE 0x00
#define OUTPUT_MODE 0x01


#define ODrain 1
#define PP 0

//-------------------------- Temp Sensor Commands (DS18B20) --------------------
#define CMD_READ_SCRATCH 0xBE
#define CMD_READ_ROM 0x33

#define CMD_MATCH_ROM 0x55
#define CMD_SEARCH_ROM 0xF0

#define CMD_SEND_CONVERT 0x44

#define CMD_SKIP_ROM 0xCC




#endif /* HEADER_GENERAL_H_ */

