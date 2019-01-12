/*
 * errorHandler.h
 *
 *  Created on: 26.11.2018
 *      Author: Linus
 */

#ifndef HEADER_ERRORHANDLER_H_
#define HEADER_ERRORHANDLER_H_


void errorHandler(int ErrorNumber);

//--------------- Error Codes ------------------
#define ILLEGALARGUMENT 1
#define ARRAYINDEXOUTOFBOUNDS 2
#define ILLEGALSTATE 3
#define NULLPOINTER 4
#define NOSENSOR 5
#define UNKNOWNERROR 6
#define ROMREADERROR 7
#define INVALIDCRC 8

#endif /* HEADER_ERRORHANDLER_H_ */
