/*
 * TempSensor.c
 *
 *  Created on: 28.11.2018
 *      Author: Linus
 */
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "general.h"
#include "TempSensor.h"
#include "cmd_module.h"
#include "errorHandler.h"



//---------------Functions------------------
static double tempToDouble(WORD temp);






//-----------------------------------------

void initTemp(void){
	initCMD();
}


/**
* Gibt eine neue Instanz leere des Temperatursensors zurück
**/
tempSensor TempNewInstance(ROM rom){
	tempSensor sensor;
	sensor.ROMCode = rom;
	sensor.measuredTemp = 0;
	return sensor;
}

/**
* Erkennt automatisch alle angeschlossenen Sensoren und deren Rom codes, und die Liste mit diesen
**/
int TempAutoDetect(int arraySize, tempSensor sensorList[arraySize], int *anzahlS){
	ROM romList[arraySize];
	int elementCounter = 0;

	// Detect
	int err = CMDautoDetectROMS(arraySize, romList, &elementCounter);
	if(err){return err;}

	*anzahlS = elementCounter;

	// Write to Array
	for(int i = 0; i < elementCounter && i < arraySize; i++){
		sensorList[i] = TempNewInstance(romList[i]);
	}

	return 0;
}

/**
* Führt eine Temperturmessung auf dem Angegebenen Sensor aus, und schreibt die Temperatur in den Sensor
**/
int TempMeasure(tempSensor *sensor){
	int err = CMDselectSlave(sensor->ROMCode);
	if(err){return err;}

	CMDmeasure();
	
	err = TempReadTemp(sensor);
	if(err){return err;}
	return 0;
}

/**
* Gibt eine leere instanz des Roms wieder
**/
ROM getEmptyROM(void){
	ROM emptyRom;
	emptyRom.crc = 0;
	emptyRom.FamilyCode =0;
	for(int i = 0; i< sizeof(emptyRom.Serialnumber);i++){
	emptyRom.Serialnumber[i] = 0;
	}
	return emptyRom;
}

/**
* Lässt alle Sensoren gleichzeitig eine Messung durchführen
**/
int TempMeasureAll(void){
	int err = CMDreset();
	if(err){return err;}
	CMDskipRom();
	CMDmeasure();
	return 0;
}

/**
* Liest die Temperatur des Angegebenen Scratchpads und schreibt den Wert in den Sensor
**/
int TempReadTemp(tempSensor *sensor){		
	int readError = 0;
	int retry = 0;	
	int numberOfTries = 3;
	ScratchPad sPad;
	
	do{
		
	int err = CMDreset();
	if(err){return err;}

	err = CMDselectSlave(sensor->ROMCode);
	if(err){return err;}
	

	readError = CMDreadScratch(&sPad);
	if(readError == INVALIDCRC){retry = 1; numberOfTries--;}

}while( retry == 1 && numberOfTries >= 0);
	
	if(readError){return readError;}

	double temp = tempToDouble(sPad.Temp);
	sensor->measuredTemp = temp;
	
	return 0;
}


/**
* Wandelt die gemessene Temperatur aus dem Scratchpad in einen Double um
**/
static double tempToDouble(WORD temp){
    float f = 0;

    int16_t input = temp;
    int komma = input & 0x0F;

    //Komma nibble rausschieben
    input = input >> 4;

    for(int i = 0; i< 4;i++){
        if(komma & (0x08 >> i)){
        // für jede 1, 2^-(1+stelle)
        f = f + pow(2,-(i+1));
        }
    }

    return f+input;


}

