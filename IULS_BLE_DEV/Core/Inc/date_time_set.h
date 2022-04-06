#ifndef DATE_TIME_SET_H
#define DATE_TIME_SET_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
// #include "stm32l4xx_hal.h"

void ds_command(char *);
void ts_command(char *);

void date_set(char *); 
void time_set(char *); 

#endif 

