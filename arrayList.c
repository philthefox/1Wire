#include <stdio.h>
#include "general.h"
#include "temperature.h"
#include "arrayList.h"
#define maxSize 20

static int count = 0;
static ROM romList[maxSize];



void initList(){
	count = 0;
for(int i = 0; i < maxSize; i++){
	romList[i] = createROM();
}
}

int addROM(ROM romCode){
	if(count >= maxSize){
	return 55;
	}
	romList[count] = romCode;
	count++;
	
return 0;	
}

ROM getROM(int index){
	if(index >= count){
	return createROM();
	}	
	return romList[index];
}

int getCount(){
	return count;
}
