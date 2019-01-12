
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "general.h"
#include "hardwareController.h"
#include "hal.h"
#include "errorHandling.h"
#include "crc.h"
#include "list.h"
#include "arrayList.h"

//---------------Functions----------------------
static void jumpToBranch(int index, uint64_t romBits);
static void detectSensorsRecursively(int index, uint64_t romBits);

/**
* Initialisiert das Modul und alle abhängigkeiten
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
* Führt den read cmd aus und schreibt den gemessenen wert in den ROM
**/
int readROM(ROM *readRom) {
	int err = reset();
	if (err) {
		
		return err;
	}
	writeByte(READ_ROM);
	readByte(8, (BYTE *) readRom);
	return 0;
}

/**
* Messung durchfuehren
**/
int measure() {
	// Send convert
	writeByte(SEND_CONVERT);

	//Set to Push pull mode
	DSetMode(PP);
	setDPin(1);

	// wait for measurement
	wait(750000);

	//Set Mode Back to OpenDrain
	DSetMode(ODrain);
	setDPin(1);
	return 0;
}
/**
* Führt den Skip rom cmd aus
**/
int skipRom() {
	int err = reset();
	if (err) {
		return err;
	}
	writeByte(SKIP_ROM);
	return 0;
}

/**
* Liest das Scratchpad aus
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

/**
* Erkennt alle angeschlossenen Sensoren und schreibt die erkannten in die Liste 
**/
int detectSensors(int size, ROM romList[size], int *numberOfElements) {
	initList();
	resetHardware();
	writeByte(SEARCH_ROM);
	int index = 0;
	uint64_t romBits = 0;
	detectSensorsRecursively(index, romBits);
	
	for(int i = 0; i < getCount() && i < size;i++){
		romList[i] = getROM(i);	
	}
	
	*numberOfElements = getCount();

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

static void detectSensorsRecursively(int index, uint64_t romBits){
	
	// Abbruchbedingung
	if(index >= 64){
		addROM(*((ROM *)(&romBits)));
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




