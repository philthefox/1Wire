
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "general.h"
#include "hardwareController.h"
#include "temperature.h"
#include "hal.h"
#include "errorHandling.h"
#include "crc.h"

#define MAX_NUMBER_ROMS 10

static ROM romList[MAX_NUMBER_ROMS];
static int numberOfROMs;

static void jumpToBranch(int index, uint64_t romBits);
static void detectSensorsRecursively(int index, uint64_t romBits);
static void initRomList(void);
static int getNumberOfRoms(void);
static int addRomToList(ROM rom);
static ROM getRomFromList(int index);

/**
* Initialisierung
**/
void initHardwareController() {
	initHAL();
	reset();
}

int resetHardware() {
	return reset();
}

/**
* Führt den Selct slave cmd aus, und selektiert den Sensor mit dem angegebenen ROM
**/
int selectSlave(ROM slaveRom) {
	// reset
	int err = reset();
	if (err) {		
		return err;
	}

	// match rom senden
	writeByte(MATCH_ROM);
	BYTE * P_RomBytes = (BYTE *) &slaveRom;
	writeBytes(sizeof(slaveRom), P_RomBytes);

	return 0;
}

/**
* Messung durchfuehren
**/
int measure() {
	// send convert command senden
	writeByte(SEND_CONVERT);

	// push pull mode setzen
	setPinMode(PUSH_PULL, DATA_PIN);
	setPin(1, DATA_PIN);

	wait(750000);

	// open drain mode setzen
	setPinMode(OPEN_DRAIN, DATA_PIN);
	setPin(1, DATA_PIN);
	return 0;
}
/**
* Ausführung des skip roms
**/
int skipRom() {
	int e = reset();
	if (e != EOK) {
		return e;
	}
	writeByte(SKIP_ROM);
	return EOK;
}

/**
* Auslesen des Scratch Pads
**/
int readScratch(ScratchPad *SpadOut) {
	
	//Send Read CMD
	writeByte(READ_SCRATCH);

	// Direktes Lesen in Scratchpad Pointer
	readByte(9, (BYTE *) SpadOut);

	if(!isValidCRC(sizeof(*SpadOut), (BYTE*) SpadOut)){
		return INVALIDCRC;
	}else{
		return 0;
	}
}

int detectSensors(int size, ROM romList[size], int *numberOfElements) {
	initRomList();
	resetHardware();
	writeByte(SEARCH_ROM);
	int index = 0;
	uint64_t romBits = 0;
	detectSensorsRecursively(index, romBits);
	
	for(int i = 0; i < getNumberOfRoms() && i < size;i++){
		romList[i] = getRomFromList(i);	
	}
	
	*numberOfElements = getNumberOfRoms();

	return 0;
}

static void jumpToBranch(int index, uint64_t romBits){
	resetHardware();
	writeByte(SEARCH_ROM);
	readBit();
	readBit();

	for(int i = 0; i < index; i++){		
		writeBit((romBits & (1 << i))>>i);
		readBit();
		readBit();			
	}
}

static void detectSensorsRecursively(int index, uint64_t romBits) {
	
	// Abbruchbedingung
	if(index >= 64){
		addRomToList(*((ROM *)(&romBits)));
		return;
	}

	int bit, bitKomplement;
	bit = readBit();
	bitKomplement = readBit();

	if (bit == !bitKomplement) {
		writeBit(bit); // Bestätigung des Bits
		romBits |= ((uint64_t)bit << index);
		detectSensorsRecursively(index + 1, romBits);        
	}
	else if ((bit == 0) && (bitKomplement == 0)) { // Uneinheitliches Bit der Sensoren
		writeBit(0); // Auswaehlen aller Sensoren, die 0 gesendet haben
		romBits |= (0 << index);
		detectSensorsRecursively(index + 1, romBits);

		jumpToBranch(index, romBits); // Zurueckspringen zum Entscheidungspunkt

		writeBit(1); // Auswaehlen aller Sensoren, die 1 gesendet haben
		romBits |= (1 << index);
		detectSensorsRecursively(index + 1, romBits);   
	}
	else if ((bit == 1) && (bitKomplement == 1)) { // Fehlerfall
		return;
	}
}

static void initRomList() {
	numberOfROMs = 0;
	for (int i = 0; i < MAX_NUMBER_ROMS; i++) {
		romList[i] = createROM();
	}
}

static int getNumberOfRoms() {
	return numberOfROMs;
}

static int addRomToList(ROM rom) {
	if (numberOfROMs > MAX_NUMBER_ROMS) {
		return -1;
	}
	romList[numberOfROMs] = rom;
	numberOfROMs++;
	return EOK;
}

static ROM getRomFromList(int index) {
	if (index >= numberOfROMs) {
		return createROM();
	}
	return romList[index];
}




