/* sample.c
 * Description: 
 *
 * Author: Bryce Himebaugh and Brian schwantes
 *
 */

#include <stdio.h>
#include "main.h"
#include "flash.h"
#include "sample.h"
#include "date_time_set.h"
#include <time.h>

extern uint32_t period; 
extern flash_status_t flash_status;

/**
 *  Function called from command.c function structure. Checks arguments are not passed.
 **/
void sample_command(char *args) {
  if (args) {
    printf("NOK\n\r");
    return;
  }
  else {
    printf("Sample Sensors\n\r");
    sample();
  }
  printf("OK\n\r");
}  

/**
 *  Function called from command.c function structure. Calls log function
 **/
void log_command(char *args) {
  printf("Sample Sensors\n\r");
  log((uint8_t *)args);
  printf("OK\n\r");
}  

/**
 *  Function called from command.c function structure. Checks if arguments are passed.
 *  Calls read data records function
 **/
void data_command(char *args) {
  if (args) {
    printf("NOK\n\r");
    return;
  }
  else {
    read_data_records(&flash_status);
  }
  printf("OK\n\r");
}  

/**
 *  Function called from command.c function structure. Checks if arguments are passed.
 *  Calls read log records function.
 **/
void print_log_command(char *args) {
  if (args) {
    printf("NOK\n\r");
    return;
  }
  else {
    read_log_records(&flash_status);
  }
  printf("OK\n\r");
} 

/**
 *  Functions populates sampledata struct and writes it to flash
 **/
int sample(void) {
    sensordata_t sd;
    sd.watermark = 1;
    sd.status=1;
    sd.timestamp=create_sample_time();
    sd.battery_voltage = (uint16_t) read_vrefint();
    sd.temperature = (uint16_t) read_temp();
    sd.sensor_period = period;
    write_raw(&flash_status,(raw_t *) &sd);
    return(0);
}

/**
 *  Functions populates logdata struct and writes it to flash
 **/
int log(uint8_t * msg) {
    int len = strlen((char *) msg);
    logdata_t ld;
    ld.watermark = 1;
    ld.status=2;
    ld.timestamp=create_sample_time();
    memset(ld.msg, 0 , 8);
    memcpy(ld.msg, msg, len);
    // ld.msg = msg;
    write_raw(&flash_status,(raw_t *) &ld);
    return(0);
}

/**
 * Arguments: flash_status_t structure pointer for flash address information for the program
 * 
 * Function iterates through all user flash memory (addresses are defined in flash_status).
 * Checks status, if status = 1 (data entry), samapled data records are printed in correct format.
 **/
int read_data_records(flash_status_t *fs) {
  sensordata_t * p;
  char samp[124];
  struct tm * recover;
  struct tm * temp;
  int count = 1;
  p = (sensordata_t *) fs->data_start;

  uint32_t epoch_time;
  char date_time[32];
  char * test;
  time_t t_of_day;

  while (p>((sensordata_t *)fs->next_address)) {
    if (p->status==1) {
      memset(date_time, 0 , 32);
      t_of_day = p->timestamp;

      struct tm * recover;
      recover = localtime(&t_of_day);

      sprintf(date_time,
            "%02d/%02d/%02d,%02d:%02d:%02d",
            // Weekdays[curDate->tm_wday],
            recover->tm_mon+1,
            recover->tm_mday,
            // recover->tm_mon+1,
            recover->tm_year+1900,
            recover->tm_hour,
            recover->tm_min,
            recover->tm_sec);

      sprintf(samp, "D,%d,%s,%d.%d,%d,%d", 
            count, 
            date_time,
            p->battery_voltage/1000,
            p->battery_voltage%1000,
            p->temperature,
            (int) p->sensor_period);
  
      printf("%s\r\n", samp);
      // fflush;
      count++;
    }
    p--;
  }
  free(recover);
  return(0);
}

/**
 * Arguments: flash_status_t structure pointer for flash address information for the program
 * 
 * Function iterates through all user flash memory (addresses are defined in flash_status).
 * Checks status, if status = 2 (log entry), log records are printed in correct format.
 **/
int read_log_records(flash_status_t *fs) {
  logdata_t * p;
  char samp[124];
  struct tm * recover;
  struct tm * temp;
  int count = 1;
  p = (sensordata_t *) fs->data_start;

  uint32_t epoch_time;
  char date_time[32];
  char * test;
  time_t t_of_day;

  while (p>((sensordata_t *)fs->next_address)) {
    if (p->status==2) {
      memset(date_time, 0 , 32);
      t_of_day = p->timestamp;

      struct tm * recover;
      recover = localtime(&t_of_day);

      sprintf(date_time,
            "%02d/%02d/%02d,%02d:%02d:%02d",
            recover->tm_mon+1,
            recover->tm_mday,
            recover->tm_year+1900,
            recover->tm_hour,
            recover->tm_min,
            recover->tm_sec);

      sprintf(samp, "L,%d,%s,%s%c", 
            count, 
            date_time,
            p->msg,
            '\0');
  
      printf("%s\r\n", samp);
      // fflush;
      count++;
    }
    p--;
  }
  free(recover);
  return(0);
}