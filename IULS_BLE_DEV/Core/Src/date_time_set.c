/**
 *  Author: Brian Schwantes
 *  Date: 04/6/2022
 * 
 *  File contains the functionality to set the date and time as well as retreieve them
 **/

#include "date_time_set.h"

extern RTC_HandleTypeDef hrtc;

// New implementation of weak ds_command in command.c
void ds_command(char *args) {
    if (args) {
        printf("NOK\n\r");
        return;
    }
    else {
        printf("ds command\n\r");
        date_set(args);
  }
  printf("OK\n\r");
}

// New implementation of weak ts_command in command.c
void ts_command(char *args) {
    if (args) {
        printf("NOK\n\r");
        return;
    }
    else {
        printf("ts command\n\r");
        time_set(args);
  }
  printf("OK\n\r");
}

/**
 *  date_set: Sets the date of RTC based on user input
 */
void date_set(char *args) {
    RTC_DateTypeDef date = {0};
    uint8_t max_date_values[] = {12,31,2099};


    if(args == NULL) {
        printf("NOK");
        return;
    }

    char *token = strtok(args, ",");
    uint8_t *head = &date.WeekDay;
    uint8_t a_value = 0;
    for (uint8_t i = 0; i < 3; i++){
        if (token == NULL)
        {
            printf("NOK");
            return;
        }
        a_value = atoi(token);
        if (a_value == 0 || a_value > max_date_values[i]){
            printf("NOK");
            return; 
        }
        *(++head) = a_value;
        token = strtok(NULL, ",");
    }

    // Set date and check for HAL status
    HAL_StatusTypeDef status = HAL_RTC_SetDate(&hrtc, &date, RTC_HOURFORMAT_24);
    switch (status)
    {
    case HAL_OK:
        printf("Ok");
        break;
    case HAL_ERROR:
        printf("Error");
        break;
    case HAL_BUSY:
        printf("Busy");
        break;
    case HAL_TIMEOUT:
        printf("Timeout");
        break;
    default:
        printf("Unknown");
        break;
    }
}

/**
 *  time_set: Sets the date of RTC based on user input
 */
void time_set(char *args) {
    RTC_TimeTypeDef time = {0};
    uint8_t max_time_values[] = {24,60,60};


    if(args == NULL) {
        printf("NOK: args = NULL");
        return;
    }

    char *token = strtok(args, ",");
    uint8_t *head = &time.Hours;
    uint8_t a_value = 0;
    for (uint8_t i = 0; i < 3; i++){
        if (token == NULL)
        {
            printf("NOK: token == NULL");
            return;
        }
        a_value = atoi(token);
        if (a_value == 0 || a_value > max_time_values[i]){
            printf("NOK: token == 0 or > max");
            return; 
        }
        *(++head) = a_value;
        token = strtok(NULL, ",");
    }

    // Set date and check for HAL status
    HAL_StatusTypeDef status = HAL_RTC_SetTime(&hrtc, &time, RTC_HOURFORMAT_24);
    switch (status)
    {
    case HAL_OK:
        printf("Ok");
        break;
    case HAL_ERROR:
        printf("Error");
        break;
    case HAL_BUSY:
        printf("Busy");
        break;
    case HAL_TIMEOUT:
        printf("Timeout");
        break;
    default:
        printf("Unknown");
        break;
    }
}