 /** flash.c
 * Description: Functions needed for initalization of flash, writing to flash, and more
 *
 * Author: Bryce Himebaugh and Brian Schwantes
 *
 **/

#include <string.h>
#include <stdio.h>
#include "main.h"
#include "flash.h"

extern int _edata;
extern int _sdata;
extern int __fini_array_end;
extern flash_status_t flash_status;

static uint32_t GetPage(uint32_t);
// static uint32_t GetBank(uint32_t);

// /**
//  * @brief   Determine the start of the user flash section, which
//  *          is the first page of flash that does not contain
//  *          program text or data
//  * @retval  Address of the first page of user flash
// **/
// uint32_t get_flash_start() {
//   /**
//    * The linker script defines the following sectors for binaries as
//    * residing in flash. Parenthetical values indicate global symbols
//    * which mark the start/end of each section. These symbols start at 
//    * the bottom of flash and grow up.
//    *    - .isr_vector     ()
//    *    - .text           (_etext)
//    *    - .rodata         ()
//    *    - .ARM.extab      (__exidx_start, __exidx_end)
//    *    - .preinit_array  (__preinit_array_start, __preinit_array_end)
//    *    - .init_array     (__init_array_start, __init_array_end)
//    *    - .fini_array     (__fini_array_end)
//    *    - .data           (_sdata, _edata)
//    * 
//    * All of the above sectors with the exception of .data are addressed
//    * with respect to flash. An easy way to determine where the program ends
//    * is to offset the end of the .fini_array sector by the size of .data
//    **/
//   extern int _sdata, _edata, __fini_array_end;
//   uint32_t program_end =  (uint32_t )&__fini_array_end + 
//                           ((uint32_t)&_edata - (uint32_t)&_sdata);

//   /* Quantize the program end and point to the following page */
//   uint32_t flash_start  = program_end - (program_end % FLASH_PAGE_SIZE) + FLASH_PAGE_SIZE;
//   return flash_start;
// }


void ef_command(char *args) {
  if (!args) {
    printf("NOK\n\r");
    return;
  }
  else {
    if (!strcmp("all",args)) {
      flash_erase();
      printf("Erase Here!\n\r");
    }
    else {
      printf("NOK\n\r");
      return;
    }
  }
  printf("OK\n\r");
}

void flash_info_command(char * args) {
  printf("Start of Flash Data: %p\n\r",flash_status.data_start);
  printf("End of Flash Data: %p\n\r",flash_status.data_end);
  printf("Next Address: %p\n\r",flash_status.next_address);
  printf("Total Records Possible: %d\n\r",((int)flash_status.data_start-(int)flash_status.data_end)/16);
  printf("Total Empty Records: %d\n\r",((int)flash_status.next_address-(int)flash_status.data_end)/16);
  printf("OK\n\r");
}

int flash_write_init(flash_status_t * fs) {
    sensordata_t * p;

    uint32_t top = 0x0800C000;
    // uint32_t bottom = 0x0803FFC0;
    // uint32_t bottom = 0x0803EF10;
    uint32_t bottom = 0x0803FFF0;

    fs->data_start = bottom;
    fs->data_end = top;


    p = (sensordata_t *) fs->data_start;
    while (p->watermark!=0xff) {
        p--;
        if (p<=((sensordata_t *)top)) return (-1);
    }
    fs->next_address = (raw_t *) p;
    return(0);
}

int write_raw(flash_status_t *fs, raw_t *data) {
  printf("\r\nFunc: Write Raw");
  HAL_StatusTypeDef status = 0;
  uint32_t addr;

  if ((!data) || (!fs)) {
    return(-1); 
  }
  HAL_FLASH_Unlock();
  addr = (uint32_t) fs->next_address;
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,addr, data->data0))) {
    HAL_FLASH_Lock();
    return(-4);
  }
  addr += 8;
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,addr, data->data1))) {
    HAL_FLASH_Lock();
    return(-4);
  }
  HAL_FLASH_Lock();
  fs->next_address--;
  // printf("\r\next addr %d\r\n", fs->next_address);
  return(0);
}

int erase_one_page(uint32_t addr) {
  uint32_t PAGEError = 0;
  static FLASH_EraseInitTypeDef EraseInitStruct;
  
  HAL_FLASH_Unlock();
  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
//   EraseInitStruct.Banks       = GetBank(addr);
  EraseInitStruct.Page        = GetPage(addr);
  EraseInitStruct.NbPages     = 1;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
    HAL_FLASH_Lock();
    return (-1);
  }
  HAL_FLASH_Lock();
  return(0);
}

static uint32_t GetPage(uint32_t Addr) {
  uint32_t page = 0;
  
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
  
  return page;
}

// static uint32_t GetBank(uint32_t Addr) {
//   return FLASH_BANK_1;
// }

int flash_erase(void) {
    uint32_t FirstPage = 0, LastPage = 0, NbOfPages = 0, BankNumber = 0;
    uint32_t PAGEError = 0;
    /* __IO uint32_t data32 = 0 , MemoryProgramStatus = 0; */

    static FLASH_EraseInitTypeDef EraseInitStruct;
    // uint32_t program_end = ((uint32_t )&__fini_array_end 
    //                 + (uint32_t)&_edata
    //                 - (uint32_t)&_sdata); 
      // uint32_t bottom  = (((uint32_t)program_end & ~0x7FF) + 0x800);
    // uint32_t bottom = FLASH_USER_START_ADDR;
    // uint32_t top = FLASH_USER_END_ADDR;
    // uint32_t top = ((uint32_t)&__fini_array_end + (uint32_t)&_edata - (uint32_t)&_sdata);
    // uint32_t bottom = 0x08000000;
    // uint32_t top = 0x080ff000;
    // uint32_t bottom = ADDR_FLASH_PAGE_126 + FLASH_PAGE_SIZE ;
    // uint32_t top = ADDR_FLASH_PAGE_16;

    uint32_t top = 0x0800C000;
    // uint32_t bottom = 0x0803FFC0;
    // uint32_t bottom = 0x0803FFF0;
    // uint32_t bottom = 0x0803EF10;
    uint32_t bottom = 0x0803FFF0;

    // fs->data_start = bottom;
    // fs->data_end = top;

    // fs->data_start = bottom;
    // fs->data_end = top;

    // uint32_t flash_start = get_flash_start();

    // uint32_t bottom = FLASH_END_ADDR - FLASH_PAGE_SIZE;
    // uint32_t top = flash_start;
    
    HAL_FLASH_Unlock();
    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
    /* Get the 1st page to erase */
    FirstPage = GetPage(top);
    LastPage = GetPage(bottom);
    /* Get the number of pages to erase from 1st page */
    NbOfPages = LastPage - FirstPage + 1;
    /* Get the bank */
    // BankNumber = GetBank(bottom);
    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    // EraseInitStruct.Banks       = BankNumber;
    EraseInitStruct.Page        = FirstPage;
    EraseInitStruct.NbPages     = NbOfPages;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
        you have to make sure that these data are rewritten before they are accessed during code
        execution. If this cannot be done safely, it is recommended to flush the caches by setting the
        DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
        HAL_FLASH_Lock();
        return (-1);
    }
    HAL_FLASH_Lock();
    flash_write_init(&flash_status);
    return(0);
} 
