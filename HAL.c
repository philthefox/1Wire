
#include <stdio.h>
#include <stdint.h>
#include "TI_Lib.h"
#include "TI_memory_map.h"
#include "timer.h"
#include "general.h"
#include "hal.h"
#include "errorHandling.h"

static void setPinReadOrWrite(int read, int pin);

void initHAL() {
	// Setzen des OpenDrain modes und Output
	timerinit();

	// -- Config and Turn on POWER Pin
	setPinMode(PUSH_PULL, POWER_PIN);
	setPinReadOrWrite(0, POWER_PIN);
	setPin(1, POWER_PIN);

	// ------ Enable DATA Pin
	setPinMode(OPEN_DRAIN, DATA_PIN);
	setPinReadOrWrite(0, DATA_PIN);

	setPin(1, DATA_PIN);

	wait(100);
}


int readDuring(uint32_t usSec) {
	setPinReadOrWrite(1, DATA_PIN);
	uint32_t startTime = getTimeStamp();
	unsigned long long waitTime = usSec * CONVERT2US;
	uint32_t timeDiff = 0;
	BYTE Data = 1;
	while ((timeDiff < waitTime) && Data == 1) {
		Data = (BUS_PORT->IDR & (0x01 << DATA_PIN)) >> DATA_PIN;

		timeDiff = (getTimeStamp() - startTime);
	}
	
	// Den Rest der Zeit warten
	if(timeDiff < waitTime){
		wait((waitTime - timeDiff) / CONVERT2US);
	}
	
	setPinReadOrWrite(0, DATA_PIN);
	//setPinMode(OPEN_DRAIN, DATA_PIN);
	setPin(1, DATA_PIN);
	int info = BUS_PORT->IDR;
	info = info;
	
	return Data;
}

int reset() {
	setPinReadOrWrite(0, DATA_PIN);
	setPin(0, DATA_PIN);
	wait(480);
	setPin(1, DATA_PIN);
	wait(10);
	BYTE data = readDuring(480);
	
	
	wait(10); // Sicherheitsabstand --
	
	if (data == 1) {
		return NOSENSOR;
	}
	return 0;
}

void setPin(int state, int pin) {
	if (state == 0) {
		BUS_PORT->BSRRH = 0x01 << pin;
	}
	else if (state == 1) {
		BUS_PORT->BSRRL = 0x01 << pin;
	}
}

void setPinMode(int mode, int pin) {
	if (mode == OPEN_DRAIN) {
		BUS_PORT->OTYPER = BUS_PORT->OTYPER | 0x01 << pin;
	}
	else if (mode == PUSH_PULL) {
		BUS_PORT->OTYPER = BUS_PORT->OTYPER & ~(0x01 << pin);
	}
}

static void setPinReadOrWrite(int read, int pin) {
	if (read == 1) {
		// Input Mode (read)
		BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (pin * 2));
	}
	else if (read == 0) {
		// clear Register
		BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (pin * 2));
		
		// Set Output Mode (write)
		BUS_PORT->MODER = BUS_PORT->MODER | (0x01 << (pin * 2));
	}
}

int wait(uint32_t usSecs) {

	uint32_t startTime = getTimeStamp();
	unsigned long long waitTime = usSecs * CONVERT2US;

	uint32_t timeDiff = 0;

	while (timeDiff < waitTime) { // Solange bis Zeit erreicht ist
		//wait
		timeDiff = (getTimeStamp() - startTime);
	}
	return 0;
}

int readBit() {
	setPinReadOrWrite(0, DATA_PIN); // write mode

	setPin(0, DATA_PIN); // SET 0 for 6us
	wait(6);

	setPin(1, DATA_PIN); // Release

	BYTE data = readDuring(15); // Read for 15us

	wait(50);	// Total time of 61us

	return data;
}

int readByte(int numberOfBytes, BYTE input[numberOfBytes]) {
	BYTE out = 0;

	for(int count = 0; count < numberOfBytes; count++){
		for (int i = 0; i < 8; i++) {
			out = out | ((readBit() & 0x01)<< i);			
			wait(1);
		}
		input[count] = out;
		out = 0;
	}
	return EOK;
}

/**
 * Schreibt ein Bit (Es wird nur das Erste Bit des Bytes Interpretiert)
 */
int writeBit(BYTE out) {
	setPinReadOrWrite(0, DATA_PIN);
	if ((out & 0x01) == 1) {
		// write 1
		setPin(0, DATA_PIN);
		wait(6);	// Write 0 for 6 uS
		setPin(1, DATA_PIN);
		wait(64);	// release for 64uS

	} else {
		// write 0
		setPin(0, DATA_PIN);
		wait(60);
		setPin(1, DATA_PIN);
		wait(10);
	}
	return 0;
}

int writeBytes(int numberOfBytes, BYTE out[numberOfBytes]) {
	for (int byteCount = 0; byteCount < numberOfBytes; byteCount++) {
		for (int i = 0; i < 8; i++) {
			wait(1); // 1 us Pause lassen
			writeBit(out[byteCount] >> i); // Bit für Bit schreiben - LSB first
		}
	}
	return 0;
}


int writeByte(BYTE out){
	writeBytes(1,&out);
	return 0;
}





