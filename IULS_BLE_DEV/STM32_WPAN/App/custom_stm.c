/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.c
  * @author  MCD Application Team
  * @brief   Custom Example Service.
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
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomIuls_BleHdle;                    /**< IULS_BLE_Service handle */
  uint16_t  CustomRead_Write_CharHdle;                  /**< IULS_READ_WRITE_CHAR handle */
  uint16_t  CustomIndication_CharHdle;                  /**< IULS_INDICATION_CHAR handle */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static const uint8_t SizeRead_Write_Char=8;
static const uint8_t SizeIndication_Char=64;
/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */
#define COPY_IULS_BLE_SERVICE_UUID(uuid_struct)          COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x01,0xcc,0x7a,0x48,0x2a,0x98,0x4a,0x7f,0x2e,0xd5,0xb3,0xe5,0x8f)
#define COPY_IULS_READ_WRITE_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x02,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)
#define COPY_IULS_INDICATION_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x03,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_attribute_modified_event_rp0 *attribute_modified;
  aci_gatt_write_permit_req_event_rp0   *write_perm_req;
  aci_gatt_read_permit_req_event_rp0    *read_req;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */

  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch(blecore_evt->ecode)
      {
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blecore_evt->data;

          APP_DBG_MSG("Attribute Modified: %d", attribute_modified->Attr_Handle);
          APP_DBG_MSG("Indication Attribute: %d", CustomContext.CustomIndication_CharHdle);
          APP_DBG_MSG("Read Write Attribute: %d",  CustomContext.CustomRead_Write_CharHdle);
          APP_DBG_MSG("Char Descriptor Attribute offset = 2");
          if(attribute_modified->Attr_Handle == (CustomContext.CustomIndication_CharHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2 */
              
            /* USER CODE END CUSTOM_STM_Service_1_Char_2 */

            switch(attribute_modified->Attr_Data[0])
            {
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_attribute_modified */
              

              /* USER CODE END CUSTOM_STM_Service_1_Char_2_attribute_modified */

              /* Disabled Indication management */
              // case (!(COMSVC_Indication)):
              case (!(COMSVC_Notification)):
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_Disabled_BEGIN */
                APP_DBG_MSG("Indications Disabled");
                /* USER CODE END CUSTOM_STM_Service_1_Char_2_attribute_modified */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_INDICATION_CHAR_INDICATE_DISABLED_EVT;
                Custom_STM_App_Notification(&Notification);
                /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_Disabled_END */

                /* USER CODE END CUSTOM_STM_Service_1_Char_2_Disabled_END */
                break;

                /* Enabled Indication management */
              // case COMSVC_Indication:
              case COMSVC_Notification:
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_COMSVC_Indication_BEGIN */
                APP_DBG_MSG("Indications Enabled");
              /* USER CODE END CUSTOM_STM_Service_1_Char_2_COMSVC_Indication_BEGIN */
                Notification.Custom_Evt_Opcode = CUSTOM_STM_INDICATION_CHAR_INDICATE_ENABLED_EVT;
                Custom_STM_App_Notification(&Notification);
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_COMSVC_Indication_END */

              /* USER CODE END CUSTOM_STM_Service_1_Char_2_COMSVC_Indication_END */
              break;

              default:
              /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_default */

              /* USER CODE END CUSTOM_STM_Service_1_Char_2_default */
              break;
            }
          }  /* if(attribute_modified->Attr_Handle == (CustomContext.CustomIULS_INDICATION_CHARHdle + CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET))*/

          else if(attribute_modified->Attr_Handle == (CustomContext.CustomRead_Write_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
            APP_DBG_MSG("Write Request?");
            Notification.Custom_Evt_Opcode = CUSTOM_STM_READ_WRITE_CHAR_WRITE_NO_RESP_EVT;
            Notification.DataTransfered.Length=attribute_modified->Attr_Data_Length;
            Notification.DataTransfered.pPayload=attribute_modified->Attr_Data;
            Custom_STM_App_Notification(&Notification);
            /* USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE */
          } /* if(attribute_modified->Attr_Handle == (CustomContext.CustomRead_Write_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          read_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          if(read_req->Attribute_Handle == (CustomContext.CustomRead_Write_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */
            APP_DBG_MSG("Read Request?");
            Notification.Custom_Evt_Opcode = CUSTOM_STM_READ_WRITE_CHAR_READ_EVT;
            Custom_STM_App_Notification(&Notification);
            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if(read_req->Attribute_Handle == (CustomContext.CustomRead_Write_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          
          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          write_perm_req = (aci_gatt_write_permit_req_event_rp0*)blecore_evt->data;
          if(write_perm_req->Attribute_Handle == (CustomContext.CustomRead_Write_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /* Allow or reject a write request from a client using aci_gatt_write_resp(...) function */
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE*/
          } /*if(write_perm_req->Attribute_Handle == (CustomContext.CustomRead_Write_CharHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/

          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;
        /* USER CODE BEGIN BLECORE_EVT */

        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /*
   *          IULS_BLE_Service
   *
   * Max_Attribute_Records = 1 + 2*2 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for IULS_BLE_Service +
   *                                2 for IULS_READ_WRITE_CHAR +
   *                                2 for IULS_INDICATION_CHAR +
   *                                1 for IULS_INDICATION_CHAR configuration descriptor +
   *                              = 6
   */
 
  COPY_IULS_BLE_SERVICE_UUID(uuid.Char_UUID_128);
  aci_gatt_add_service(UUID_TYPE_128,
                       (Service_UUID_t *) &uuid,
                       PRIMARY_SERVICE,
                       6,
                       &(CustomContext.CustomIuls_BleHdle));
  /**
   *  IULS_READ_WRITE_CHAR
   */
  COPY_IULS_READ_WRITE_CHAR_UUID(uuid.Char_UUID_128);
  aci_gatt_add_char(CustomContext.CustomIuls_BleHdle,
                    UUID_TYPE_128, &uuid,
                    SizeRead_Write_Char,
                    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RESP,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE| GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                    0x10,
                    8,
                    &(CustomContext.CustomRead_Write_CharHdle));
  /**
   *  IULS_INDICATION_CHAR
   */
  COPY_IULS_INDICATION_CHAR_UUID(uuid.Char_UUID_128);
  aci_gatt_add_char(CustomContext.CustomIuls_BleHdle,
                    UUID_TYPE_128, &uuid,
                    SizeIndication_Char,
                    CHAR_PROP_NOTIFY,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE,
                    0x10,
                    CHAR_VALUE_LEN_VARIABLE,
                    &(CustomContext.CustomIndication_CharHdle));

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */
  APP_DBG_MSG("Custom Service Created");
  APP_DBG_MSG("Custom Characteristics Added");
  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */
  APP_DBG_MSG("Characteristc Update");
  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch(CharOpcode)
  {

    case CUSTOM_STM_READ_WRITE_CHAR:
      result = aci_gatt_update_char_value(CustomContext.CustomIuls_BleHdle,
                                          CustomContext.CustomRead_Write_CharHdle,
                                          0, /* charValOffset */
                                          SizeRead_Write_Char, /* charValueLen */
                                          (uint8_t *)  pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1*/
      APP_DBG_MSG("Update Write Char");
      /* USER CODE END CUSTOM_STM_Service_1_Char_1*/
      break;

    case CUSTOM_STM_INDICATION_CHAR:
      result = aci_gatt_update_char_value(CustomContext.CustomIuls_BleHdle,
                                          CustomContext.CustomIndication_CharHdle,
                                          0, /* charValOffset */
                                          SizeIndication_Char, /* charValueLen */
                                          (uint8_t *)  pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2*/
      APP_DBG_MSG("Update Indication Char");
      /* USER CODE END CUSTOM_STM_Service_1_Char_2*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return result;
}
