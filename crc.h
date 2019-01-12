/*
 * CRC_calc.h
 *
 *  Created on: 25.11.2018
 *      Author: Linus
 */

#ifndef HEADER_CRC_CALC_H_
#define HEADER_CRC_CALC_H_
//#include <stdint.h>



//--------------------------- Functions -----------------

int isValidCRC(int size, BYTE data[size]);
int calcCRC(int size, BYTE data[size], BYTE* result);


#endif /* HEADER_CRC_CALC_H_ */
