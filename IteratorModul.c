/*
 * IteratorModul.c
 *	
 *
 *  Created on: 03.12.2018
 *      Author: Linus
 */
 
 #include <stdio.h>
 #include <stdint.h>
 #include <stdlib.h>
 #include "general.h"
 #include "cmd_module.h"
 #include "HAL.h"
 #include "errorHandler.h"
 #include "IteratorModul.h"

static unsigned long long Oprotokoll = 0;
static int Oindex = -1; // O Index für die Protokolldatei
static int hasnext = 1;
static int firstRun = 1;

void initIterator(void){
	Oprotokoll = 0;
	Oindex = -1;
	hasnext = 1;
	firstRun = 1;
}
	
int hasNext(void){
	return hasnext;
}


int detectNextROM(unsigned long long *detectedROM){
	int err = reset();
		if (err) {
			return err;
		}
		
		if(Oprotokoll == 0 && firstRun == 0){
			hasnext = 0 ; 
			return 0;
		}
		firstRun = 0;
		
		// send Command
		writeByte(CMD_SEARCH_ROM);
		
		
		int index = 0; 	// O index des aktuellen durchlaufs
		unsigned long long currROM = 0;

		for (int i = 0; i < 64; i++) {

			BYTE bit = readBit();
			BYTE notBit = readBit();
			wait(10);	//TODO pause eingebaut.. Optimierungsmöglichkeit
			BYTE decision = 0;
			
			BYTE readCombination = (bit << 1) | notBit;
			
			switch(readCombination){
				case 0x0:	//conflicting bits in the current ROM code bit position.
				{

			//--------------Möglichkeit 1 (Entscheidung aus Protokolldatei Nehmen)	-------
					if (index < Oindex) {
						decision = (Oprotokoll & (0x01 << index)) >> index;

						index++;


			//--------------Möglichkeit 2 (Neuer Pfad) ----
					} else if (index > Oindex) {

						decision = 0x01;
						Oprotokoll = Oprotokoll | (decision << index);
						Oindex = index;
						index++;


			//--------------Möglichkeit 3 (Anderen Pfad Nehmen - Wendepunkt) ----------
					} else if (index == Oindex) {
						decision = (Oprotokoll ^ (0x01 << index))>>index; // Protokollierte entscheidung Invertieren

						Oprotokoll = Oprotokoll ^ (0x01 << index);	// Protkollierte entscheidung toggeln in Protokoll
						Oindex--;
						index++;
					}
					
					currROM = currROM | (decision << i);

				}
				break;
				case 0x01:	// All devices connected to the bus have a 0 in this bit position.
				{

					// CurrRom muss nicht geschrieben werden da 0 
					decision = 0x00;
				}
				break;
				case 0x02:	//All devices connected to the bus have a 1 in this bit position.
				{
					unsigned long long mask = 0x01;
					currROM = currROM | (mask << i);
					decision = 0x01;
				}
				break;
				case 0x03:
				//There are no devices connected to the 1-Wire bus.
				return NOSENSOR;				
				default:
				return UNKNOWNERROR;				

			}
			
			
			// Schreiben des Bits
			writeBit(decision);
			
			
		}

 		while((Oprotokoll & (0x01 << Oindex))== 0){
		
		}
		index = 0;
		*detectedROM = currROM;
		return 0;
}
 
