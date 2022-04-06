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

// extern uint32_t period; 
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
    sd.timestamp=12345;
//   sd.battery_voltage = (uint16_t) read_vrefint();
//   sd.temperature = (uint16_t) read_temp();
//   sd.sensor_period = period;
    sd.battery_voltage = 3;
    sd.temperature = 50;
    sd.sensor_period = 2000;
    write_raw(&flash_status,(raw_t *) &sd);
    return(0);
}

int read_data_records(flash_status_t *fs) {
  sensordata_t * p;
  int count = 1;
  p = (sensordata_t *) fs->data_start;
  while (p>((sensordata_t *)fs->next_address)) {
    if (p->status==1) {
      printf("%d,b=%d.%d,t=%d,p=%d\n\r",
             count,
             p->battery_voltage/1000,
             p->battery_voltage%1000,
             p->temperature,
             (int) p->sensor_period);
      count++;
    }
    p--;
  }
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