/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    p2p_server_app.c
  * @author  MCD Application Team
  * @brief   Peer to peer Server Application
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
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
#include "p2p_server_app.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
 typedef struct{
    uint8_t             Device_Led_Selection;
    uint8_t             Led1;
 }P2P_LedCharValue_t;

 typedef struct{
    uint8_t             Device_Button_Selection;
    uint8_t             ButtonStatus;
 }P2P_ButtonCharValue_t;

  typedef struct{
    uint8_t *           temp_data;
  }P2P_DataStruct_t;
// {
//   /* data */
// };

/**
 *   Light Sensor Data Struct
 */
// Sensor Data is 128 bits
// typedef struct sensordata {
//     uint8_t watermark;               // 0x01=populated, 0xFF=unpopulated
//     uint8_t status;                  // record type, 01=sensor data, 02=log data;
//     uint16_t record_number;          // Which number is this particular record
//     uint32_t timestamp;              // Time, bit packed into 32 bits
//     uint16_t battery_voltage;        // 16 bit battery voltage
//     int16_t temperature;             // STM32 Temperature sensor reading
//     uint32_t sensor_period;          // Reading from the light sensor
// } sensordata_t;
typedef enum
{
  DATA_TRANSFER_OFF,
  DATA_TRANSFER_ON,
  DATA_SEND,
  DATA_WAIT

} Data_Transfer_Status_t;

typedef struct
{
  uint8_t               Notification_Status; /* used to check if P2P Server is enabled to Notify */
  P2P_LedCharValue_t    LedControl;
  P2P_ButtonCharValue_t ButtonControl;
  sensordata_t *        SensorData;
  uint16_t              ConnectionHandle;
  Data_Transfer_Status_t DTFlowStatus;
} P2P_Server_App_Context_t;

typedef struct
{
  uint8_t *pPayload;
  uint32_t pPayload_n_1;
  uint32_t pPayload_n;
  uint32_t Length;
} DTS_STM_Payload_t;

#define MAX_LEN 16
static uint8_t Notification_Data_Buffer[MAX_LEN];
/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
int count = 0;
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static P2P_Server_App_Context_t P2P_Server_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

// static uint8_t Notification_Data_Buffer[DATA_NOTIFICATION_MAX_PACKET_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void P2PS_Send_Notification(void);
static void P2P_Send_Data(void);
static void P2PS_APP_LED_BUTTON_context_Init(void);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void P2PS_STM_App_Notification(P2PS_STM_App_Notification_evt_t *pNotification)
{
/* USER CODE BEGIN P2PS_STM_App_Notification_1 */

/* USER CODE END P2PS_STM_App_Notification_1 */
  switch(pNotification->P2P_Evt_Opcode)
  {
/* USER CODE BEGIN P2PS_STM_App_Notification_P2P_Evt_Opcode */
#if(BLE_CFG_OTA_REBOOT_CHAR != 0)
    case P2PS_STM_BOOT_REQUEST_EVT:
      APP_DBG_MSG("-- P2P APPLICATION SERVER : BOOT REQUESTED\n");
      APP_DBG_MSG(" \n\r");

      *(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
      NVIC_SystemReset();
      break;
#endif
/* USER CODE END P2PS_STM_App_Notification_P2P_Evt_Opcode */

    case P2PS_STM__NOTIFY_ENABLED_EVT:
/* USER CODE BEGIN P2PS_STM__NOTIFY_ENABLED_EVT */
      P2P_Server_App_Context.DTFlowStatus = DATA_TRANSFER_ON;
      APP_DBG_MSG("Data Flow Status Turned ON");
      P2P_Server_App_Context.Notification_Status = 1;
      APP_DBG_MSG("-- P2P APPLICATION SERVER : NOTIFICATION ENABLED\n"); 
      APP_DBG_MSG(" \n\r");
/* USER CODE END P2PS_STM__NOTIFY_ENABLED_EVT */
      break;

    case P2PS_STM_NOTIFY_DISABLED_EVT:
/* USER CODE BEGIN P2PS_STM_NOTIFY_DISABLED_EVT */
      P2P_Server_App_Context.DTFlowStatus = DATA_TRANSFER_OFF;
      APP_DBG_MSG("Data Flow Status Turned OFF");
      P2P_Server_App_Context.Notification_Status = 0;
      APP_DBG_MSG("-- P2P APPLICATION SERVER : NOTIFICATION DISABLED\n");
      APP_DBG_MSG(" \n\r");
/* USER CODE END P2PS_STM_NOTIFY_DISABLED_EVT */
      break;

    case P2PS_STM_WRITE_EVT:
/* USER CODE BEGIN P2PS_STM_WRITE_EVT */
      APP_DBG_MSG("Event recieved: %s\r\n", pNotification->DataTransfered.pPayload);
      
      uint8_t * command = pNotification->DataTransfered.pPayload;
      // uint8_t * data = "data";
      // APP_DBG_MSG("strlen %d", strlen(command));
      // APP_DBG_MSG("Command: %s", command);
  
      // COMMAND: data
      if (strncmp(command, "data", 4) == 0) {
        APP_DBG_MSG("COMMAND: data");
        APP_DBG_MSG("Turning on datasend");
        P2P_Send_Data();
        // for (int i = 0; i < 100; i ++){
        //   P2P_Send_Data();
        // }

      }
      // COMMAND: data
      else if (strncmp(command, "lof", 3) == 0) {
        BSP_LED_Off(LED_BLUE);
        APP_DBG_MSG("COMMAND: lof");
      }
      // COMMAND: data
      else if (strncmp(command, "lon", 3) == 0) {
        BSP_LED_On(LED_BLUE);
        APP_DBG_MSG("COMMAND: lon");
      }
      
      // // TURN ON LED
      // if(pNotification->DataTransfered.pPayload[0] == 0x00){ /* ALL Deviceselected - may be necessary as LB Routeur informs all connection */
      //   // TURN ON LED
      //   if(pNotification->DataTransfered.pPayload[1] == 0x01)
      //   {
      //     BSP_LED_On(LED_BLUE);
      //     APP_DBG_MSG("-- P2P APPLICATION SERVER  : LED1 ON\n"); 
      //     APP_DBG_MSG(" \n\r");
      //     P2P_Server_App_Context.LedControl.Led1=0x01; /* LED1 ON */
      //   }
      //   // TURN OFF LED
      //   if(pNotification->DataTransfered.pPayload[1] == 0x00)
      //   {
      //     BSP_LED_Off(LED_BLUE);
      //     APP_DBG_MSG("-- P2P APPLICATION SERVER  : LED1 OFF\n"); 
      //     APP_DBG_MSG(" \n\r");
      //     P2P_Server_App_Context.LedControl.Led1=0x00; /* LED1 OFF */
      //   }
      // }



/* USER CODE END P2PS_STM_WRITE_EVT */
      break;

  


    default:
/* USER CODE BEGIN P2PS_STM_App_Notification_default */
      
/* USER CODE END P2PS_STM_App_Notification_default */
      break;
  }
/* USER CODE BEGIN P2PS_STM_App_Notification_2 */

/* USER CODE END P2PS_STM_App_Notification_2 */
  return;
}

void P2PS_APP_Notification(P2PS_APP_ConnHandle_Not_evt_t *pNotification)
{
/* USER CODE BEGIN P2PS_APP_Notification_1 */

/* USER CODE END P2PS_APP_Notification_1 */
  switch(pNotification->P2P_Evt_Opcode)
  {
/* USER CODE BEGIN P2PS_APP_Notification_P2P_Evt_Opcode */

/* USER CODE END P2PS_APP_Notification_P2P_Evt_Opcode */
  case PEER_CONN_HANDLE_EVT :
/* USER CODE BEGIN PEER_CONN_HANDLE_EVT */
          
/* USER CODE END PEER_CONN_HANDLE_EVT */
    break;

    case PEER_DISCON_HANDLE_EVT :
/* USER CODE BEGIN PEER_DISCON_HANDLE_EVT */
       P2PS_APP_LED_BUTTON_context_Init();       
/* USER CODE END PEER_DISCON_HANDLE_EVT */
    break;

    default:
/* USER CODE BEGIN P2PS_APP_Notification_default */

/* USER CODE END P2PS_APP_Notification_default */
      break;
  }
/* USER CODE BEGIN P2PS_APP_Notification_2 */

/* USER CODE END P2PS_APP_Notification_2 */
  return;
}

void P2PS_APP_Init(void)
{
/* USER CODE BEGIN P2PS_APP_Init */
  UTIL_SEQ_RegTask( 1<< CFG_TASK_SW1_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, P2PS_Send_Notification );
  UTIL_SEQ_RegTask( 1<<CFG_TASK_DATA_TRANSFER_UPDATE_ID, UTIL_SEQ_RFU, P2P_Send_Data);
  /**
   * Initialize LedButton Service
   */
  P2P_Server_App_Context.Notification_Status=0; 
  P2PS_APP_LED_BUTTON_context_Init();
  P2P_Server_App_Context.DTFlowStatus = DATA_TRANSFER_OFF;
/* USER CODE END P2PS_APP_Init */
  return;
}

/* USER CODE BEGIN FD */
void P2PS_APP_LED_BUTTON_context_Init(void){
  
  BSP_LED_Off(LED_BLUE);
  
  #if(P2P_SERVER1 != 0)
  P2P_Server_App_Context.LedControl.Device_Led_Selection=0x01; /* Device1 */
  P2P_Server_App_Context.LedControl.Led1=0x00; /* led OFF */
  P2P_Server_App_Context.ButtonControl.Device_Button_Selection=0x01;/* Device1 */
  P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
#endif
// #if(P2P_SERVER2 != 0)
//   P2P_Server_App_Context.LedControl.Device_Led_Selection=0x02; /* Device2 */
//   P2P_Server_App_Context.LedControl.Led1=0x00; /* led OFF */
//   P2P_Server_App_Context.ButtonControl.Device_Button_Selection=0x02;/* Device2 */
//   P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
// #endif  
// #if(P2P_SERVER3 != 0)
//   P2P_Server_App_Context.LedControl.Device_Led_Selection=0x03; /* Device3 */
//   P2P_Server_App_Context.LedControl.Led1=0x00; /* led OFF */
//   P2P_Server_App_Context.ButtonControl.Device_Button_Selection=0x03; /* Device3 */
//   P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
// #endif
// #if(P2P_SERVER4 != 0)
//   P2P_Server_App_Context.LedControl.Device_Led_Selection=0x04; /* Device4 */
//   P2P_Server_App_Context.LedControl.Led1=0x00; /* led OFF */
//   P2P_Server_App_Context.ButtonControl.Device_Button_Selection=0x04; /* Device4 */
//   P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
// #endif  
//  #if(P2P_SERVER5 != 0)
//   P2P_Server_App_Context.LedControl.Device_Led_Selection=0x05; /* Device5 */
//   P2P_Server_App_Context.LedControl.Led1=0x00; /* led OFF */
//   P2P_Server_App_Context.ButtonControl.Device_Button_Selection=0x05; /* Device5 */
//   P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
// #endif
// #if(P2P_SERVER6 != 0)
//   P2P_Server_App_Context.LedControl.Device_Led_Selection=0x06; /* device6 */
//   P2P_Server_App_Context.LedControl.Led1=0x00; /* led OFF */
//   P2P_Server_App_Context.ButtonControl.Device_Button_Selection=0x06; /* Device6 */
//   P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
// #endif  
}

void P2PS_APP_SW1_Button_Action(void)
{
  UTIL_SEQ_SetTask( 1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);

  return;
}
/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void P2PS_Send_Notification(void)
{
 
  if(P2P_Server_App_Context.ButtonControl.ButtonStatus == 0x00){
    P2P_Server_App_Context.ButtonControl.ButtonStatus=0x01;
  } else {
    P2P_Server_App_Context.ButtonControl.ButtonStatus=0x00;
  }
  
   if(P2P_Server_App_Context.Notification_Status){ 
    APP_DBG_MSG("-- P2P APPLICATION SERVER  : INFORM CLIENT BUTTON 1 PUSHED \n ");
    APP_DBG_MSG(" \n\r");
    P2PS_STM_App_Update_Char(P2P_NOTIFY_CHAR_UUID, (uint8_t *)&P2P_Server_App_Context.ButtonControl);
   } else {
    APP_DBG_MSG("-- P2P APPLICATION SERVER : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n "); 
   }

  return;
}
void P2P_Send_Data(void)
{
  // sensordata_t temp_data = {0};
  // temp_data.watermark = 0x02;

  // temp_data.status = 00;
  // temp_data.record_number = 00;
  // temp_data.timestamp = 0;
  // temp_data.battery_voltage = (uint16_t) 0;
  // temp_data.temperature = (int16_t) 0;
  // temp_data.sensor_period = (uint32_t) 0;
  char temp_line[] ="01010001FFFFFFFF0000111100000000";
  // char temp_line[64];
  int counter = 0;
  for (int i = 0; i < 10; i++) {
    // itoa(i, temp_line, 10);
    // temp_data.record_number +=1;
    if(P2P_Server_App_Context.Notification_Status){ 
      APP_DBG_MSG("-- P2P APPLICATION SERVER  : SENDING DATA\n");
      APP_DBG_MSG(" \n\r");

      P2PS_STM_App_Update_Char(P2P_DATA_CHAR_UUID, (uint8_t *)&temp_line);
    } 
    else {
      APP_DBG_MSG("-- P2P APPLICATION SERVER : CAN'T SEND DATA -  NOTIFICATION DISABLED\n "); 
   }
  }
  return;
  // int MAX_LEN = 10;
  // uint8_t Notification_Data_Buffer[MAX_LEN];
  // if(P2P_Server_App_Context.DTFlowStatus == DATA_TRANSFER_OFF) {
  //   APP_DBG_MSG("FLOW OFF");
  // }
  // else if(P2P_Server_App_Context.DTFlowStatus == DATA_SEND) {
  //   APP_DBG_MSG("Function: Send Data")
  
  //   tBleStatus status = BLE_STATUS_INVALID_PARAMS;
  //   uint8_t crc_result;

  //   Notification_Data_Buffer[0] += 1;
  //   crc_result = APP_BLE_ComputeCRC8((uint8_t*) Notification_Data_Buffer, (MAX_LEN - 1));
  //   Notification_Data_Buffer[MAX_LEN - 1] = crc_result;

  //   status = P2PS_STM_App_Update_Char(P2P_DATA_CHAR_UUID, (uint8_t *) &temp_line);
  //   if (status == BLE_STATUS_INSUFFICIENT_RESOURCES)
  //   {
  //     P2P_Server_App_Context.DTFlowStatus = DATA_TRANSFER_OFF;
  //     (Notification_Data_Buffer[0])-=1;
  //   }
  //   else
  //   {
  //     UTIL_SEQ_SetTask(1 << CFG_TASK_DATA_TRANSFER_UPDATE_ID, CFG_SCH_PRIO_0);
  //   }
  //   }
  // return;
  // P2P_Server_App_Context.SensorData = temp_line;
  // crc_result = APP_BLE_ComputeCRC8((uint8_t*) Notification_Data_Buffer, (DATA_NOTIFICATION_MAX_PACKET_SIZE - 1));
}
/* USER CODE END FD_LOCAL_FUNCTIONS*/
