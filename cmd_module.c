
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "general.h"
#include "cmd_module.h"
#include "HAL.h"
#include "errorHandler.h"
#include "CRC_calc.h"

#include "list.h"
#include "arrayList.h"

//---------------Functions----------------------
static void jump_to(int current_index, uint64_t romcode);
static void search_recursive(int current_index, uint64_t romcode);

/**
* Initialisiert das Modul und alle abhängigkeiten
**/
void initCMD() {
	initHAL();
	reset();
}

int CMDreset() {
	return reset();
}

/**
* Führt den Selct slave cmd aus, und selektiert den Sensor mit dem angegebenen ROM
**/
int CMDselectSlave(ROM slaveRom) {
	// -- reset Bus --
	int err = reset();
	if (err) {		
		return err;
	}

	// --- send Match rom CMD -
	writeByte(CMD_MATCH_ROM);
	BYTE * P_RomBytes = (BYTE *) &slaveRom;
	writeBytes(sizeof(slaveRom), P_RomBytes);

	return 0;
}

/**
* Führt den read cmd aus und schreibt den gemessenen wert in den ROM
**/
int CMDreadROM(ROM *readRom) {
	int err = reset();
	if (err) {
		
		return err;
	}
	writeByte(CMD_READ_ROM);
	readByte(8, (BYTE *) readRom);
	return 0;
}

/**
* Führt eine messung durch
**/
int CMDmeasure() {
	// Send convert CMD
	writeByte(CMD_SEND_CONVERT);

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
int CMDskipRom() {
	int err = reset();
	if (err) {
		return err;
	}
	writeByte(CMD_SKIP_ROM);
	return 0;
}

/**
* Liest das Scratchpad aus
**/
int CMDreadScratch(ScratchPad *SpadOut) {
	
	//Send Read CMD
	writeByte(CMD_READ_SCRATCH);

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
int CMDautoDetectROMS(int size, ROM romList[size], int *numberOfElements) {
	initList();
	CMDreset();
	writeByte(CMD_SEARCH_ROM);
	search_recursive(0,0);
	
	for(int i = 0; i < getCount() && i < size;i++){
		romList[i] = getROM(i);	
	}
	
	*numberOfElements = getCount();

	return 0;
}




static void jump_to(int current_index, uint64_t romcode){
    CMDreset();
    writeByte(CMD_SEARCH_ROM);
			readBit();
			readBit();
	
    for(int i = 0; i < current_index; i++){		
			writeBit((romcode & (1 << i))>>i);
			readBit();
			readBit();			
    }
}





																					/*** REKURSIVE ***/


static void search_recursive(int current_index, uint64_t romcode){
    
	if(current_index >= 64){
        // add sensor and quit recursion
        addROM(*((ROM *)(&romcode)));
        return;
    }
	
    int bit, inverse;
    bit = readBit();
    inverse = readBit();
    
    if(bit == !inverse){
        writeBit(bit);
        romcode |= ((uint64_t)bit << current_index);
        search_recursive(current_index + 1, romcode);        
    } else if( bit == 0 && inverse == 0){ // Das Bit der Sensoren ist nicht einheitlic
        // Need to decide
        // try 0 first
        writeBit(0);
        romcode |= (0 << current_index);
        search_recursive(current_index + 1, romcode);
        
        // restart from the beginning
        jump_to(current_index, romcode);
        
        // try 1
       writeBit(1);
        romcode |= (1 << current_index);
        search_recursive(current_index + 1, romcode);   
    } else {
        return;
    }
}




