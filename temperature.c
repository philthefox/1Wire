
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "general.h"
#include "temperature.h"
#include "hardwareController.h"
#include "errorHandling.h"

static double temperatureToDouble(WORD temp);


void initTemperature(void){
	initHardwareController();
}

/**
* Erstellen eines leeren Sensors
**/
Sensor createSensor(ROM rom){
	Sensor sensor;
	sensor.rom = rom;
	sensor.temperature = 0;
	return sensor;
}

/**
* Erkennt automatisch alle angeschlossenen Sensoren und deren Rom codes, und die Liste mit diesen
**/
int detectAllSensors(int arraySize, Sensor sensorList[arraySize], int *anzahlS){
	ROM romList[arraySize];
	int elementCounter = 0;

	// Detect
	int err = detectSensors(arraySize, romList, &elementCounter);
	if (err) {
		return err;
	}

	*anzahlS = elementCounter;

	// Write to Array
	for(int i = 0; i < elementCounter && i < arraySize; i++) {
		sensorList[i] = createSensor(romList[i]);
	}

	return 0;
}

/**
* Führt eine Temperturmessung auf dem Angegebenen Sensor aus, und schreibt die Temperatur in den Sensor
**/
//int TempMeasure(Sensor *sensor){
//	int err = selectSlave(sensor->ROMCode);
//	if(err){return err;}
//
//	measure();
//	
//	err = TempReadTemp(sensor);
//	if(err){return err;}
//	return 0;
//}

/**
* Erstellen eines leeren ROMs
**/
ROM createROM(void){
	ROM rom;
	rom.crc = 0;
	rom.familyCode =0;
	for(int i = 0; i< sizeof(rom.serialNumber); i++) {
		rom.serialNumber[i] = 0;
	}
	return rom;
}

/**
* Lässt alle Sensoren gleichzeitig eine Messung durchführen
**/
int measureAllTemperatures(void){
	int err = resetHardware();
	if(err) {
		return err;
	}
	skipRom();
	measure();
	return 0;
}

/**
* Liest die Temperatur des Angegebenen Scratchpads und schreibt den Wert in den Sensor
**/
int readTemperatureFromScratchpad(Sensor *sensor){		
	int readError = 0;
	int retry = 0;	
	int numberOfTries = 3;
	ScratchPad sPad;
	
	do{
		int err = resetHardware();
		if(err){return err;}

		err = selectSlave(sensor->rom);
		if(err){return err;}
		

		readError = readScratch(&sPad);
		if (readError == INVALIDCRC) {
			retry = 1;
			numberOfTries--;
		}
	} while (retry == 1 && numberOfTries >= 0);
	
	if (readError) {
		return readError;
	}

	double temp = temperatureToDouble(sPad.temperature);
	sensor->temperature = temp;
	
	return 0;
}


/**
* Wandelt die gemessene Temperatur aus dem Scratchpad in einen Double um
**/
static double temperatureToDouble(WORD temp){
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

