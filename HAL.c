#include <stdio.h>
#include <stdint.h>
#include "TI_Lib.h"
#include "TI_memory_map.h"
#include "timer.h"
#include "general.h"
#include "hal.h"
#include "errorHandling.h"

static void setPin(int state, int pin);
static void setRW_pin(int read, int pin);
static void pinSetMode(int mode, int pin);

void initHAL() {
	// Setzen des OpenDrain modes und Output
	timerinit();

	// -- Config and Turn on POWER Pin
	pinSetMode(PUSH_PULL, POWER_PIN);
	setRW_pin(0, POWER_PIN);
	setPin(1, POWER_PIN);

	// ------ Enable DATA Pin
	DSetMode(OPEN_DRAIN);
	setRW(0);

	setDPin(1);	//schreibe 1

	wait(100);

}


int readDuring(uint32_t usSec) {
	setRW(1);
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
	
	setRW(0);
	//DSetMode(OPEN_DRAIN);	
	setDPin(1);
	int info = BUS_PORT->IDR;
	info = info;
	
	return Data;
}

int reset() {
	setRW(0);
	setDPin(0);
	wait(480);
	setDPin(1);
	wait(10);
	BYTE data = readDuring(480);
	
	
	wait(10); // Sicherheitsabstand --
	
	if (data == 1) {
		return NOSENSOR;
	}
	return 0;
}

/**
 *Enable /Disable
 **/
void setDPin(int state) {
	setPin(state, DATA_PIN);
}

static void setPin(int state, int pin) {
	if (state == 0) {
		BUS_PORT->RESET_REG = 0x01 << pin;
	} else {
		BUS_PORT->SET_REG = 0x01 << pin;
	}
}

/**
 * mode =1 -> OpenDrain | mode =0 -> PushPull
 * 
 */
void DSetMode(int mode) {
	pinSetMode(mode, DATA_PIN);
}

/**
 * mode =1 -> OpenDrain | mode =0 -> PushPull
 * 
 */
static void pinSetMode(int mode, int pin) {
	if (mode == 1) {
		// Open Drain mode
		BUS_PORT->OTYPER = BUS_PORT->OTYPER | 0x01 << pin;
	} else {
		// Push Pull mode
		BUS_PORT->OTYPER = BUS_PORT->OTYPER & ~(0x01 << pin);
	}
}

void setRW(int read) {
	setRW_pin(read, DATA_PIN);
}

static void setRW_pin(int read, int pin) {
	if (read == 1) {
		// Input Mode (read)
		BUS_PORT->MODER = BUS_PORT->MODER & ~(0x03 << (pin * 2));
	} else {
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
	setRW(0); // Write Mode

	setDPin(0);	// SET 0 for 6us
	wait(6);

	setDPin(1); // Release

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
	return 0;
}

/**
 * Schreibt ein BIT (Es wird nur das Erste Bit des Bytes Interpretiert)
 */
int writeBit(BYTE out) {
	setRW(0);
	if ((out & 0x01) == 1) {
		// write 1
		setDPin(0);
		wait(6);	// Write 0 for 6 uS
		setDPin(1);
		wait(64);	// release for 64uS

	} else {
		// write 0
		setDPin(0);
		wait(60);
		setDPin(1);
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





