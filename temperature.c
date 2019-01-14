
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

Sensor createSensor(ROM rom){
	Sensor sensor;
	sensor.rom = rom;
	sensor.temperature = 0;
	return sensor;
}

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

ROM createROM(void){
	ROM rom;
	rom.crc = 0;
	rom.familyCode =0;
	for(int i = 0; i< sizeof(rom.serialNumber); i++) {
		rom.serialNumber[i] = 0;
	}
	return rom;
}

int measureAllTemperatures(void){
	int e = resetHardware();
	if(e != EOK) {
		return e;
	}
	skipRom();
	measure();
	return EOK;
}

int readTemperatureFromScratchpad(Sensor *sensor){		
	int readError = EOK;
	int retry = 0;	
	int numberOfTries = 3;
	ScratchPad sPad;
	
	do{
		int e = resetHardware();
		if (e != EOK) {
			return e;
		}

		e = selectSlave(sensor->rom);
		if (e != EOK) {
			return e;
		}

		readError = readScratch(&sPad);
		if (readError == INVALIDCRC) {
			retry = 1;
			numberOfTries--;
		}
	} while (retry == 1 && numberOfTries >= 0);
	
	if (readError != EOK) {
		return readError;
	}

	double temp = temperatureToDouble(sPad.temperature);
	sensor->temperature = temp;
	
	return EOK;
}

static double temperatureToDouble(WORD temp){
    float f = 0;

    int16_t input = temp;
    int komma = input & 0x0F;

    // Bits rechts des Kommas entfernen
    input = input >> 4;

    for (int i = 0; i < 4; i++) {
        if (komma & (0x08 >> i)) {
			// für jede 1, 2^-(1+stelle)
			f = f + pow(2,-(i+1));
        }
    }

    return f+input;
}

