
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
void initTemperatureOutput(void) {
	//TFT_set_font_color(GREEN);
	TFT_gotoxy(TEMPXOFFSET,TEMPYOFFSET);
	TFT_puts("Sensor Temps:");
}

/**
* Gibt einen Temperatur wert an der richtigen stelle auf dem Display aus
**/ 
void printTemperatures(Sensor sensor, int nr) {
	char stringBuff[20];
	//TFT_set_font_color(GREEN);
	snprintf(stringBuff, 20, "%d: %.3f Grad C",nr, sensor.temperature);
	TFT_gotoxy(TEMPXOFFSET,TEMPYOFFSET+nr);
	TFT_puts(stringBuff);
}

/**
* Gibt den Rom Code des Sensor auf dem Display aus
**/
void printROMs(Sensor sensor, int nr) {
	char strBuff[100];
	snprintf(strBuff, sizeof(strBuff), "\n\r%d: %llx", nr, *((unsigned long long *) &(sensor.rom)));
	TFT_puts(strBuff);
}
