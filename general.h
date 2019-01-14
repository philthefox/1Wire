
#ifndef HEADER_GENERAL_H_
#define HEADER_GENERAL_H_

#include <stdint.h>

typedef int8_t CHAR;
typedef int16_t SHORT;
typedef int32_t LONG;
typedef uint32_t DWORD;
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;

typedef struct ROMNUM{
	BYTE familyCode;
	BYTE serialNumber[6];
	BYTE crc;
}ROM;

typedef struct SENSOR {
	ROM rom;
	double temperature;
} Sensor;

typedef struct  __attribute__((__packed__)) SCRATCHPAD{
	WORD temperature;
	BYTE ThUsrByte;
	BYTE TlUsrByte;
	BYTE configR;
	BYTE reserved[3];
	BYTE crc;
}ScratchPad;

// errors
#define EOK						0
#define ILLEGALARGUMENT 		1
#define ARRAYINDEXOUTOFBOUNDS 	2
#define ILLEGALSTATE 			3
#define NULLPOINTER 			4
#define NOSENSOR 				5
#define UNKNOWNERROR 			6
#define ROMREADERROR 			7
#define INVALIDCRC 				8

// hal
#define BUS_PORT 	GPIOG

#define DATA_PIN 	0
#define POWER_PIN 	1

#define OPEN_DRAIN 	1
#define PUSH_PULL 	0

// sensor commands
#define READ_SCRATCH 	0xBE
#define READ_ROM 		0x33
#define MATCH_ROM 		0x55
#define SEARCH_ROM 		0xF0
#define SEND_CONVERT 	0x44
#define SKIP_ROM 		0xCC

#endif /* HEADER_GENERAL_H_ */


