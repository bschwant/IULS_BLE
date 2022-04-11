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

uint32_t create_sample_time(void);
char* date_time_recover(uint32_t epoch_time);

#endif 

