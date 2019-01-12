
#include <stdio.h>
#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "temperature.h"

#define TEMPYOFFSET 1
#define TEMPXOFFSET 22

/**
* Initialisiert die Temperaturausgabe 
**/
void initTempOutput(void) {
	//TFT_set_font_color(GREEN);
	TFT_gotoxy(TEMPXOFFSET,TEMPYOFFSET);
	TFT_puts("Sensor Temps:");
}

/**
* Gibt einen Temperatur wert an der richtigen stelle auf dem Display aus
**/ 
void printTemp(tempSensor sensor, int nr) {
	char stringBuff[20];
	//TFT_set_font_color(GREEN);
	snprintf(stringBuff, 20, "%d: %.3f Grad C",nr, sensor.measuredTemp);
	TFT_gotoxy(TEMPXOFFSET,TEMPYOFFSET+nr);
	TFT_puts(stringBuff);
}

/**
* Gibt den Rom Code des Sensor auf dem Display aus
**/
void printROM(tempSensor sensor, int nr) {
	char strBuff[100];
	snprintf(strBuff, sizeof(strBuff), "\n\r%d: %llx", nr, *((unsigned long long *) &(sensor.ROMCode)));
	TFT_puts(strBuff);
}
