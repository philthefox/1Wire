
#include <stdio.h>

#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "TempSensor.h"
#include "errorHandler.h"
#include "output.h"

// ---------------- Functionen ---------------
static void initROMList(int * anzahlS, int maxList,tempSensor SensorList[maxList]);

int main(void) {
	Init_TI_Board(); 
	initTemp();
	int maxList = 15;
	int anzahlS = 0;
	tempSensor SensorList[maxList];
	
	initROMList(&anzahlS, maxList, SensorList);
	initTempOutput();
	while (1) { // SuperLoop
		
		while(anzahlS == 0){			
			initROMList(&anzahlS, maxList, SensorList);		
			initTempOutput();
			if(anzahlS != 0){
				TFT_cls();
				initTempOutput();
				initROMList(&anzahlS, maxList, SensorList);	
			}
		}
		

		// --- Messungen Durchführen --
		
			// Alle sensoren eine Messung ausführen lassen
		int err = TempMeasureAll();
		if(err){errorHandler(err);}
		
		
		
		
		
		for (int i = 0; i < anzahlS; i++) {
			
			// Scratchpads auslesen
			int err = TempReadTemp(&(SensorList[i]));			
			if (err) {
				// Im Fehlerfall liste zurücksetzen und Roms erneut detecten
				errorHandler(err);
				TFT_cls();
				initROMList(&anzahlS, maxList, SensorList);
				initTempOutput();
				break;
			}			
			
			// Messungen Ausgeben
			printTemp(SensorList[i], i+1);
		}	
		

				
		

	}
}

/**
* Initialisiert eine neue RomListe, und führt autodetect der Sensoren durch
**/
static void initROMList(int * anzahlS, int maxList, tempSensor SensorList[maxList]) {
	// Init List;	
	*anzahlS = 0;
	for (int i = 0; i < maxList; i++) {		
		SensorList[i] = TempNewInstance(getEmptyROM());
	}

	int err = TempAutoDetect(maxList, SensorList, anzahlS);
	if(err) {
		errorHandler(err);
	}

	TFT_cursor_off();
	//TFT_set_font_color(GREEN);
	TFT_gotoxy(1,1);	
	TFT_puts("ROMs: ");

	for(int i = 0; i < *anzahlS; i++){
		printROM(SensorList[i], i+1);
	}
}


