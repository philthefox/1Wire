
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "general.h"
#include "errorHandling.h"
#include "tft.h"


void errorHandler(int ErrorNumber){
	TFT_set_font_color(RED);
	TFT_gotoxy(1,14);
	if(ErrorNumber != EOK){
		switch(ErrorNumber){
			case ARRAYINDEXOUTOFBOUNDS:
				TFT_puts("Array index out of Bounds");
				break;
			case ILLEGALSTATE:
				TFT_puts("IllegalStateException");
				break;
			case NULLPOINTER:
				TFT_puts("Null Pointer übergabe");
				break;
			case ILLEGALARGUMENT:
				TFT_puts("Illegal Argument exception");
				break;
			case NOSENSOR:
				TFT_puts("No Sensor detected");
				break;
			case ROMREADERROR:
				TFT_puts("ERROR while reading ROM codes");
				break;
			case INVALIDCRC:
				TFT_puts("Problem while receiving data.. CRC Error");
				break;
			default:
				TFT_puts("Unknown Error");
		}
	}
}
