
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "temperature.h"
#include "errorHandling.h"
#include "output.h"

static void initROMList(int *anzahlS, int maxSensors, Sensor sensorList[maxSensors]);

int main(void) {
	
	// Initialisierungen
	Init_TI_Board();
	initTemperature();
	int maxSensors = 15;
	int anzahlS = 0;
	Sensor sensorList[maxSensors];
	initROMList(&anzahlS, maxSensors, sensorList);
	initTemperatureOutput();
	int e = EOK;
	
	while (1) { // super loop
		
		while (anzahlS == 0) {			
			initROMList(&anzahlS, maxSensors, sensorList);		
			initTemperatureOutput();
			if (anzahlS != 0) {
				TFT_cls();
				initTemperatureOutput();
				initROMList(&anzahlS, maxSensors, sensorList);	
			}
		}
		

		// Durchfuehren der Temperaturmessungen (alle Sensoren gleichzeitig)
		e = measureAllTemperatures();
		if(e != EOK) {
			errorHandler(e);
		}

		for (int i = 0; i < anzahlS; i++) {
			// Auslesen der Scratchpads
			e = readTemperatureFromScratchpad(&(sensorList[i]));			
			if (e != EOK) {
				errorHandler(e);
				TFT_cls();
				initROMList(&anzahlS, maxSensors, sensorList);
				initTemperatureOutput();
				break;
			}			
			
			// Ausgabe der gemessenen Temperaturen
			printTemperature(sensorList[i], i+1);
		}	
	}
}

static void initROMList(int *anzahlS, int maxSensors, Sensor sensorList[maxSensors]) {
	int e = EOK;
	// Init List;	
	*anzahlS = 0;
	for (int i = 0; i < maxSensors; i++) {		
		sensorList[i] = createSensor(createROM());
	}

	e = detectAllSensors(maxSensors, sensorList, anzahlS);
	if (e != EOK) {
		errorHandler(e);
	}

	printROMs(anzahlS, maxSensors, sensorList);
}


