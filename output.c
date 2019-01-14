
#include <stdio.h>
#include "TI_Lib.h"
#include "tft.h"
#include "general.h"
#include "temperature.h"

static void printROM(Sensor sensor, int nr);

/**
* Initialisierung der Temperaturausgabe 
**/
void initTemperatureOutput(void) {
	TFT_gotoxy(22,1);
	TFT_puts("Temperatures:");
}

/**
* Ausgabe der Temperatur eines Sensors
**/ 
void printTemperature(Sensor sensor, int nr) {
	char stringBuff[20];
	snprintf(stringBuff, 20, "%.2f Grad C", sensor.temperature);
	TFT_gotoxy(22,1+nr);
	TFT_puts(stringBuff);
}

/**
* Ausgabe der ROM Codes aller erkannten Sensoren
**/
void printROMs(int *anzahlS, int maxSensors, Sensor sensorList[maxSensors]) {
	TFT_cursor_off();
	TFT_set_font_color(WHITE);
	TFT_gotoxy(1,1);	
	TFT_puts("Sensors: ");
	
	for(int i = 0; i < *anzahlS; i++){
		printROM(sensorList[i], i+1);
	}
}

/**
* Ausgabe des ROM Codes eines Sensors
**/
static void printROM(Sensor sensor, int nr) {
	char strBuff[100];
	snprintf(strBuff, sizeof(strBuff), "\n\r%d: %llx", nr, *((unsigned long long *) &(sensor.rom)));
	TFT_puts(strBuff);
}
