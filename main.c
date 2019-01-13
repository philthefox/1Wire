
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "temperature.h"
#include "errorHandling.h"
#include "output.h"

// ---------------- Functionen ---------------
static void initROMList(int * anzahlS, int maxList, Sensor SensorList[maxList]);

int main(void) {
	
	// Initialisierungen
	Init_TI_Board();
	initTemperature();
	int maxList = 15;
	int anzahlS = 0;
	Sensor SensorList[maxList];
	initROMList(&anzahlS, maxList, SensorList);
	initTemperatureOutput();
	
	while (1) { // super loop
		
		while(anzahlS == 0){			
			initROMList(&anzahlS, maxList, SensorList);		
			initTemperatureOutput();
			if(anzahlS != 0){
				TFT_cls();
				initTemperatureOutput();
				initROMList(&anzahlS, maxList, SensorList);	
			}
		}
		

		// Durchfuehren der Temperaturmessungen (alle Sensoren gleichzeitig)
		int err = measureAllTemperatures();
		if(err) {
			errorHandler(err);
		}

		for (int i = 0; i < anzahlS; i++) {
			// Auslesen der Scratchpads
			int err = readTemperatureFromScratchpad(&(SensorList[i]));			
			if (err) {
				// Im Fehlerfall liste zurücksetzen und Roms erneut detecten
				errorHandler(err);
				TFT_cls();
				initROMList(&anzahlS, maxList, SensorList);
				initTemperatureOutput();
				break;
			}			
			
			// Ausgabe der gemessenen Temperaturen
			printTemperatures(SensorList[i], i+1);
		}	
	}
}

/**
* Initialisiert eine neue RomListe, und führt autodetect der Sensoren durch
**/
static void initROMList(int * anzahlS, int maxList, Sensor SensorList[maxList]) {
	// Init List;	
	*anzahlS = 0;
	for (int i = 0; i < maxList; i++) {		
		SensorList[i] = createSensor(createROM());
	}

	int err = detectAllSensors(maxList, SensorList, anzahlS);
	if(err) {
		errorHandler(err);
	}

	TFT_cursor_off();
	TFT_set_font_color(WHITE);
	TFT_gotoxy(1,1);	
	TFT_puts("ROMs: ");
	
	for(int i = 0; i < *anzahlS; i++){
		printROMs(SensorList[i], i+1);
	}
}


