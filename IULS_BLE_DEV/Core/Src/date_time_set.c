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
    if (!args) {
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
    if (!args) {
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
        printf("NOK: args == NULL");
        return;
    }

    char *token = strtok(args, ",");
    uint8_t *head = &date.WeekDay;
    uint8_t a_value = 0;
    for (uint8_t i = 0; i < 3; i++){
        if (token == NULL)
        {
            printf("NOK: token == NULL");
            return;
        }
        a_value = atoi(token);
        if (a_value == 0 || a_value > max_date_values[i]){
            printf("NOK: token == 0 or > max");
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
        *(head++) = a_value;
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

uint32_t create_sample_time(void){
    /* Function that reads the RTC time, converts it to epoch time using
     mktime, and then converts it back using localtime
   */
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  struct tm t = {0};
  struct tm * recover;
  time_t t_of_day;
  
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);
  printf("RTC Reports the following Date: %02d/%02d/20%02d\n\r",
         current_date.Month,
         current_date.Date,
         current_date.Year);

  printf("RTC Reports the following Time: %02d:%02d:%02d\n\r",
         current_time.Hours,
         current_time.Minutes,
         current_time.Seconds);

  t.tm_year = current_date.Year + 100;  
  t.tm_mon = current_date.Month - 1; 
  t.tm_mday = current_date.Date; 
  t.tm_hour = current_time.Hours;
  t.tm_min = current_time.Minutes;
  t.tm_sec = current_time.Seconds;
  t.tm_isdst = -1;    
  t_of_day = mktime(&t);
//   return(t_of_day);
  printf("Current Epoch time Converted from RTC: %d\n\r", (int) t_of_day);
  recover = localtime(&t_of_day);
  printf("Epoch Time Recovered Date: %02d/%02d/20%02d\n\r",
         recover->tm_mon + 1,
         recover->tm_mday,
        //  recover->tm_mon + 1,
         recover->tm_year - 100);

  printf("Epoch Time Recovered Time: %02d:%02d:%02d\n\r",
         recover->tm_hour,
         recover->tm_min,
         recover->tm_sec);
  free(recover);
  return(t_of_day);
//   return (0);
}

/**
 * @brief   Converts an epoch time to a human readable format
 * @param   epoch_time: Time in seconds since 2000
 * @retval  None
**/
char* date_time_recover(uint32_t epoch_time){
//   uint16_t year, month, day, hour, minute, second;
//   uint16_t days, leap_year;
//   static char dateTime[32];

  
time_t curSec;
struct tm *curDate;
char dateString[32];

// curSec = time(NULL);
// curDate = localtime(&curSec);
// sprintf(dateString,
//     "%02d/%02d/%02d,%02d:%02d:%02d",
//     // Weekdays[curDate->tm_wday],
//     curDate->tm_mday,
//     curDate->tm_mon+1,
//     curDate->tm_year+1900,
//     curDate->tm_hour,
//     curDate->tm_min,
//     curDate->tm_sec);

// printf("Date StringL %s\r\n",dateString);
// return 0;

time_t t_of_day;
t_of_day = epoch_time;

struct tm * recover;
recover = localtime(&t_of_day);
printf("Epoch Time Recovered Date: %02d/%02d/20%02d\n\r",
         recover->tm_mon + 1,
         recover->tm_mday,
        //  recover->tm_mon + 1,
         recover->tm_year - 100);

  printf("Epoch Time Recovered Time: %02d:%02d:%02d\n\r",
         recover->tm_hour,
         recover->tm_min,
         recover->tm_sec);

sprintf(dateString,
    "%02d/%02d/%02d,%02d:%02d:%02d",
    // Weekdays[curDate->tm_wday],
    recover->tm_mday,
    recover->tm_mon+1,
    recover->tm_year+1900,
    recover->tm_hour,
    recover->tm_min,
    recover->tm_sec);
printf("DATE STRING: %s\r\n", dateString);
return dateString;
}

//   month = 1;
//   year = epoch_time / 31556926;
//   epoch_time -= year*31556926;
//   leap_year = year % 4 == 0 ? 1 : 0;
//   days = epoch_time/86400 + 1;
//   epoch_time %= 86400;
    
//   if (days > 334+leap_year){
//     month = 12;
//     days -= (334+leap_year);
//   }
  
//   else if (days > 304 + leap_year){
//     month = 11;
//     days -= (304 + leap_year);
//   }
  
//   else if (days > 273+leap_year){
//     month = 10;
//     days -= (273+leap_year);
//   }
  
//   else if (days > 243 + leap_year){
//     month = 9;
//     days -= (243 + leap_year);
//   }
  
//   else if (days > 212+leap_year){
//     month = 8;
//     days -= (212+leap_year);
//   }
//   else if (days > 181 + leap_year){
//     month = 7;
//     days -= (181 + leap_year);
//   }
//   else if (days > 151+leap_year){
//     month = 6;
//     days -= (151+leap_year);
//   }
//   else if (days > 120 + leap_year){
//     month = 5;
//     days -= (120 + leap_year);
//   } 
//   else if (days > 90 + leap_year){
//     month = 4;
//     days -= (90 + leap_year);
//   }
//   else if (days > 59+leap_year){
//     month = 3;
//     days -= (59+leap_year);
//   }
//   else if (days > 31){
//     month = 2;
//     days -= (31);
//   }
//   else if (days > 0){
//     month = 1;
//   }
   
//   day = days;
//   hour = epoch_time/3600;
//   epoch_time %= 3600;
//   minute = epoch_time/60;
//   second = epoch_time%60;

//   sprintf(dateTime,"%02u/%02u/%02u,%02u:%02u:%02u",month,day,year,hour,minute,second);
//   return(dateTime);
// }