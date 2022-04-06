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
#include "flash.h"
#include "sample.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
uint8_t payload[8];
extern flash_status_t flash_status;
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
  // strncpy(payload, pNotification->DataTransfered.pPayload, pNotification->DataTransfered.Length);
  // payload = pNotification->DataTransfered.pPayload;
  // payload[pNotification->DataTransfered.Length] = '\0';
// /  strncpy()
  
  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch(pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */
    // memset(payload, NULL, 8);
    // strncpy(payload, pNotification->DataTransfered.pPayload, pNotification->DataTransfered.Length);
    // APP_DBG_MSG("Notifcation recieved: %s\r\n", payload);
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
        // P2P_Send_Data()
      }

      // APP_DBG_MSG("Event: Write no response");
      // APP_DBG_MSG("Notifcation recieved: %s\r\n", payload);
      // if(strncmp(payload, "data", 4) == 0) {
      //   send_data();
      // }
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
void send_data(void) {
  // sensordata_t * temp = read_data_records_ble(&flash_status);
  // flash_status


}

int read_data_records_ble(flash_status_t *fs) {
  memset(NotifyCharData, 0, 247);
  uint8_t samp[124];
  sensordata_t * p;
  int mon = 4;
  int day = 6;
  int year = 2022;
  int hr = 10;
  int min = 20;
  int batt = 3;
  int count = 1;
  p = (sensordata_t *) fs->data_start;
  while (p>((sensordata_t *)fs->next_address)) {
    if (p->status==1) {
      sprintf(samp, "D,%d,%d/%d/%d,%d:%d:%d,%d.%d,%d,%d\n", 
            count, mon, day, year, hr, min, count,
            p->battery_voltage/1000,
            p->battery_voltage%1000,
            p->temperature,
            (int) p->sensor_period);
      printf("\r\n%s\r\n", samp);
      // UpdateCharData = samp;
      memcpy(UpdateCharData, samp, 36);
      Custom_Indication_char_Update_Char();
      printf("%d b=%d.%d,t=%d,p=%d\n\r",
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

/* USER CODE END FD_LOCAL_FUNCTIONS*/
