/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Flash_Driver.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about Flash driver.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    05-Nov-2015

  ******************************************************************************
  * @attention
  *
  * Licensed under GPLv2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.gnu.org/licenses/gpl-2.0.html
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_DRIVER_H
#define __FLASH_DRIVER_H

#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"



/* Exported types ------------------------------------------------------------*/

#define  __StartOfAppArea   0x00000001u
#define  __EndOfFlash       0x0001FFFFu



#ifdef __cplusplus
extern "C" {
#endif


/* Exported functions ------------------------------------------------------- */

/* Function to initializes the flash controller's clock and flash Data Speculation */
void Flash_Init(void);

/* Function to erase a sector size of flash memory */
int Flash_EarseSector(uint32_t StartAddrOfSector);

/* Function to write a unit of data to the flash memory */
int Flash_ProgramUnit(uint32_t StartAddrOfSector, uint8_t *data);

/* Function to write a sector of data to the flash memory */
int Flash_ProgramSector(uint32_t StartAddrOfSector, uint8_t *data);


#ifdef __cplusplus
}
#endif

#endif /* __FLASHCI_H */

/*****************************END OF FILE**************************************/