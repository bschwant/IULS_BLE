/* sample.c
 * Description: 
 *
 * Author: Bryce Himebaugh and Brian schwantes
 *
 */

#include <stdio.h>
#include "main.h"
// #include "temperature.h"
// #include "battery.h"
#include "flash.h"
#include "sample.h"
#include "date_time_set.h"
#include <time.h>

extern uint32_t period; 
extern flash_status_t flash_status;

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

int sample(void) {
    sensordata_t sd;
    sd.watermark = 1;
    sd.status=1;
    sd.timestamp=create_sample_time();
    sd.battery_voltage = (uint16_t) read_vrefint();
    sd.temperature = (uint16_t) read_temp();
    sd.sensor_period = period;
    // sd.battery_voltage = 3;
    // sd.temperature = 50;
    // sd.sensor_period = 2000;
    write_raw(&flash_status,(raw_t *) &sd);
    return(0);
}

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

// int read_data_records_ble(flash_status_t *fs, sensordata_t *prev) {

//   sensordata_t * p;
//   int count = 1;
//   p = (sensordata_t *) fs->data_start;
//   while (p>((sensordata_t *)fs->next_address)) {
//     if (p->status==1) {
//         return p;
//     //   printf("%d,b=%d.%d,t=%d,p=%d\n\r",
//     //          count,
//     //          p->battery_voltage/1000,
//     //          p->battery_voltage%1000,
//     //          p->temperature,
//     //          (int) p->sensor_period);
//     //   count++;
//     }
//     p--;
//   }
//   return(0);
// }