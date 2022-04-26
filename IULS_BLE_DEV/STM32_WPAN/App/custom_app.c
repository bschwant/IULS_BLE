/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"
#include "command.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <time.h>
#include <string.h>

uint8_t payload[8];
extern flash_status_t flash_status;
extern RTC_HandleTypeDef hrtc;
extern uint32_t period; 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* IULS_BLE_Service */
  uint8_t               Indication_char_Indication_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

/* USER CODE BEGIN PV */
uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

uint8_t SecureReadData;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
  /* IULS_BLE_Service */
static void Custom_Indication_char_Update_Char(void);
static void Custom_Indication_char_Send_Indication(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */
  memset(payload, NULL, 8);
  
  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch(pNotification->Custom_Evt_Opcode)
  {
    APP_DBG_MSG("pNotifcation: in switch");
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */
   
    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

  /* IULS_BLE_Service */
    case CUSTOM_STM_READ_WRITE_CHAR_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_READ_WRITE_CHAR_READ_EVT */
      APP_DBG_MSG("Event: Read from characteristic");
      /* USER CODE END CUSTOM_STM_READ_WRITE_CHAR_READ_EVT */
      break;

    case CUSTOM_STM_READ_WRITE_CHAR_WRITE_NO_RESP_EVT:
      /* USER CODE BEGIN CUSTOM_STM_READ_WRITE_CHAR_WRITE_NO_RESP_EVT */
      APP_DBG_MSG("Event recieved: %s\r\n", pNotification->DataTransfered.pPayload);
      uint8_t * command = pNotification->DataTransfered.pPayload;

      if(strncmp(command, "data", 4) == 0) {
        read_data_records_ble(&flash_status);
        APP_DBG_MSG("COMMAND: data");
        APP_DBG_MSG("Turning on datasend");
      }

      else if(strncmp(command, "log", 3) == 0) {
        uint8_t temp_line[12];
        memcpy(temp_line, command, 12);
        execute_command(temp_line);
        APP_DBG_MSG("COMMAND: log: writing log record");
      }
      
      // check for command rlog, which is read log
      else if(strncmp(command, "rlog", 4) == 0) {
        read_log_records_ble(&flash_status);
        APP_DBG_MSG("COMMAND: rlog: reading all log records");
      }

      // checks for command ts, which is time set
      else if (strncmp(command, "ts", 2) == 0 && strncmp(command, "tsl", 3) != 0) {
        uint8_t temp_line[11];
        memcpy(temp_line, command, 11);
        execute_command(temp_line);
      }

      // checks for command ds, which is date set
      else if (strncmp(command, "ds", 2) == 0){
        uint8_t temp_line[11];
        memcpy(temp_line, command, 11);
        execute_command(temp_line);
      }
      // tg: Time Get, functions returns notifcation of string with current time
      else if (strncmp(command, "tg", 2) == 0){
        get_time();
      }
      // dg: Date Get, functions returns notifcation of string with current date
      else if (strncmp(command, "dg", 2) == 0){
        get_date();
      }
      // sample: function records new sample to flash
      else if (strncmp(command, "sample", 6) == 0){
        sample_ble();
      }
      // sample: functions returns notifcation of string with light sensor frequency
      else if (strncmp(command, "tsl237", 6) == 0){
        tsl237_ble();
      }
      /* USER CODE END CUSTOM_STM_READ_WRITE_CHAR_WRITE_NO_RESP_EVT */
      break;

    case CUSTOM_STM_INDICATION_CHAR_INDICATE_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_INDICATION_CHAR_INDICATE_ENABLED_EVT */
      APP_DBG_MSG("Event: Notifications enabled");
      /* USER CODE END CUSTOM_STM_INDICATION_CHAR_INDICATE_ENABLED_EVT */
      break;

    case CUSTOM_STM_INDICATION_CHAR_INDICATE_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_INDICATION_CHAR_INDICATE_DISABLED_EVT */
      APP_DBG_MSG("Event: Notifications disabled");
      /* USER CODE END CUSTOM_STM_INDICATION_CHAR_INDICATE_DISABLED_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */
  APP_DBG_MSG("Connection handle event");
  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch(pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */
      APP_DBG_MSG("Device Connected");
      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */
      APP_DBG_MSG("Device Disconnected");
      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

  /* IULS_BLE_Service */
void Custom_Indication_char_Update_Char(void) /* Property Read */
{
  Custom_STM_App_Update_Char(CUSTOM_STM_INDICATION_CHAR, (uint8_t *)UpdateCharData);
  /* USER CODE BEGIN Indication_char_UC*/

  /* USER CODE END Indication_char_UC*/
  return;
}

void Custom_Indication_char_Send_Indication(void) /* Property Indication */
{
  if(Custom_App_Context.Indication_char_Indication_Status)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_INDICATION_CHAR, (uint8_t *)NotifyCharData);
    /* USER CODE BEGIN Indication_char_IS*/

    /* USER CODE END Indication_char_IS*/
  }
  else
  {
    APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  INDICATION DISABLED\n ");
  }
  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void send_data(void) {
}

/**
 *  Function computers current frequency of light sensor.
 *  One frequency reading is sent as notifcaiton to the app
 **/
void tsl237_ble(){
  float clock_period;
  float sensor_period;
  float sensor_frequency;
  
  uint8_t temp_freq[16];
  clock_period = SystemCoreClock;
  clock_period = 1/clock_period;
  sensor_period = clock_period * (float) period;
  sensor_frequency = 1/sensor_period;
  sprintf(temp_freq, "%0.3f\n\r", sensor_frequency);
  int len = strlen(temp_freq);
  memcpy(UpdateCharData, temp_freq, len);   // Copy data to UpdateCharData 
  Custom_Indication_char_Update_Char(); // Update notifcation characteristic

}

/**
 *  Function sends "sample" command to execute command function in command.c
 * 
 *  Nothing returned, no output.
 **/
void sample_ble() {
  char temp_line[6] = "sample";
  execute_command(temp_line);
 
}

/**
 *  Function to get the current time, and sends the current time in string format
 *  as a notification to an app.
 * 
 *  example output: "12:31:22"  - hour:minute:second
 * 
 *  Note: Must be listening to notifications to recieve time
 **/
void get_time() {
  uint8_t curr_time[8];

  RTC_TimeTypeDef current_time = {0};
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);

  sprintf(curr_time,"%02d:%02d:%02d", current_time.Hours,current_time.Minutes, current_time.Seconds);

  int len = strlen(curr_time);
  memcpy(UpdateCharData, curr_time, len);
  Custom_Indication_char_Update_Char();
}

/**
 *  Function to get the current date, and sends current the current date
 *  in string format as a notification to an app. 
 * 
 *  example output: "04/26/2022"  - month/day/year
 * 
 *  Note: Must be listening to notifications to recieve date
 **/
void get_date() {

  uint8_t curr_date[10];

  RTC_DateTypeDef current_date = {0};
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);

  sprintf(curr_date,"%02d/%02d/20%02d", current_date.Month,current_date.Date,current_date.Year);

  int len = strlen(curr_date);
  memcpy(UpdateCharData, curr_date, len);
  Custom_Indication_char_Update_Char();
}

/**
 * Function that reads all of the sample data from flash and sends each
 * as a notifcation to the app.
 * 
 * Format: "D,7,01/01/2000,00:00:02,3.313,28,331"
 * 
 * Note: Must be listening to notifications to recieve data 
 **/
int read_data_records_ble(flash_status_t *fs) {
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
            recover->tm_mon+1,
            recover->tm_mday,
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
      int len = strlen(samp);
      memcpy(UpdateCharData, samp, len);
      Custom_Indication_char_Update_Char();
      count++;
    }
    p--;
  }
  free(recover);
  return(0);
}


/**
 * Function that reads all of the log records from flash and sends each
 * as a notifcation to the app.
 * 
 * Format: "D,7,01/01/2000,00:00:02,3.313,28,331"
 * 
 * Note: Must be listening to notifications to recieve data 
 **/
int read_log_records_ble(flash_status_t *fs) {
  logdata_t * p;
  char samp[124];
  struct tm * recover;
  struct tm * temp;
  int count = 1;
  p = (logdata_t *) fs->data_start;

  uint32_t epoch_time;
  char date_time[32];
  char * test;
  time_t t_of_day;

  while (p>((logdata_t *)fs->next_address)) {
    if (p->status==2) {
      memset(date_time, 0 , 32);
      t_of_day = p->timestamp;

      struct tm * recover;
      recover = localtime(&t_of_day);

      uint8_t msg[8];

      memcpy(msg, p->msg, 8);

      sprintf(date_time,
            "%02d/%02d/%02d,%02d:%02d:%02d",
            // Weekdays[curDate->tm_wday],
            recover->tm_mon+1,
            recover->tm_mday,
            recover->tm_year+1900,
            recover->tm_hour,
            recover->tm_min,
            recover->tm_sec);

      sprintf(samp, "D,%d,%s,%s", 
            count, 
            date_time,
            msg);
  
      printf("%s\r\n", samp);
      int len = strlen(samp);
      memcpy(UpdateCharData, samp, len);
      Custom_Indication_char_Update_Char();
      count++;
    }
    p--;
  }
  free(recover);
  return(0);
}
/* USER CODE END FD_LOCAL_FUNCTIONS*/

