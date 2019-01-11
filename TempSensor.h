/*
 * TempSensor.h
 */

#ifndef HEADER_TEMPSENSOR_H_
#define HEADER_TEMPSENSOR_H_

typedef struct TSENSOR {
	ROM ROMCode;
	double measuredTemp;
} tempSensor;

void initTemp(void);

/**
* Gibt eine neue Instanz des Temperatursensors zur�ck
**/
tempSensor TempNewInstance(ROM rom);


/**
* Erkennt automatisch alle angeschlossenen Sensoren und deren Rom codes
**/
int TempAutoDetect(int arraySize, tempSensor sensorList[arraySize], int *anzahlS);

/**
* F�hrt eine Temperturmessung auf dem Angegebenen Sensor aus, und schreibt die Temperatur in den Sensor
**/
int TempMeasure(tempSensor *sensor);

/**
* Liest die Temperatur des Angegebenen Scratchpads und schreibt den Wert in den Sensor
**/
int TempReadTemp(tempSensor *sensor);


/**
* L�sst alle Sensoren gleichzeitig eine Messung durchf�hren
**/
int TempMeasureAll(void);

/**
* Gibt eine leere instanz des Roms wieder
**/
ROM getEmptyROM(void);

#endif /* HEADER_TEMPSENSOR_H_ */
