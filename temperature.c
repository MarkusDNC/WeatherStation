#include "temperature.h"
#include "main.h"
#include "at91sam3x8.h"
#include <stdio.h>
#include <stdlib.h>

#define SYSTICK_LOAD ((volatile unsigned int*)0xE000E014)
#define SYSTICK_VAL ((volatile unsigned int*)0xE000E018)

float logTemp[60] ;
int logTempCount = 0;
float tempMin = 100;
float tempMax = -100;
float temp;
int dayPos = 0;
int tempFlag = 0;

table* dayValues;
table* week[7];

void tempMeasure(){
  *AT91C_PIOB_OER = 1<<25;
  *AT91C_PIOB_CODR = 1<<25;
  delay(25);
  *AT91C_PIOB_SODR = 1<<25;
  *AT91C_PIOB_ODR = 1<<25;
  *AT91C_TC0_CCR = 1<<2;
  tempFlag = 1;
}

void updateDegrees(){
    sprintf(floatToChar,"%.1f C\0",value);
}

table* MinMaxAvg(float* tempVec){
  table* values = (table*) malloc(sizeof(table));
  float minTemp = 100;
  float maxTemp = -100;
  float avgTemp = 0;
  for(int i=0; i<60; i++){
    if(tempVec[i] <minTemp){
      minTemp = tempVec[i];
    }
    else if(tempVec[i] > maxTemp){
      maxTemp = tempVec[i];
    }
    avgTemp +=tempVec[i];
  }
  avgTemp = avgTemp / 60;
  values->max = maxTemp;
  values->min = minTemp;
  values->avg = avgTemp;
  
  return values;
}

void tempCalc(void){
  *AT91C_TC0_IDR = 1<<6;
  temp = *AT91C_TC0_RB;
  temp -= *AT91C_TC0_RA;
  temp = temp/42;
  if(temp < 5){return;}
  value =((temp/5.0)-273.15);
  //Add value to vector
  if(value < 100){
    logTemp[logTempCount++] = value;
    if(logTempCount >= 60){
      dayValues = MinMaxAvg(logTemp);
      logTempCount = 0;
      week[dayPos++] = dayValues;
      if(dayPos == 7){
        dayPos = 0;
      }
    }
  }
}



/*Fills table with random values for evaluation purposes*/
void fillTable(void){
  
}