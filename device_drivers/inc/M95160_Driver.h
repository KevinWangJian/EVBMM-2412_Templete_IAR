/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: M95160_Driver.c
  * @author: Wangjian
  * @Descriptiuon: Provide a set of functions about initialize M95160 chip,
  *                writting data to M95160 and reading data from M95160.
  * @Others: None
  * @History: 1. Created by Wangjian.
  *           2. a. Removed Enable M95160 chip Write functon;
  *              b. Changed all API function's name by adding SPI port which 
  *                 user can specified.
  * @version: V1.0.1
  * @date:    11-Nov-2015

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
  
#ifndef  __M95160_DRIVER_H
#define  __M95160_DRIVER_H

#include <stdint.h>

#include "SKEAZ1284.h"
#include "GPIO_Driver.h"

 
/* Exported types ------------------------------------------------------------*/  

/* EEPROM chip M95160 driver version: Rev1.0.1 */
#define   M95160_DRIVER_VERSION     (101)


#define   M95160_CS_Output()    GPIO_PinInit(GPIO_PTE3, GPIO_PinOutput)

#define   M95160_CS_HIGH()      GPIO_PinSet(GPIO_PTE3)    

#define   M95160_CS_LOW()       GPIO_PinClear(GPIO_PTE3)


/* EEPROM command */
#define   WREN    (0x06u)       /* Write Enable */
#define   WRDI    (0x04u)       /* Write Disable */
#define   RDSR    (0x05u)       /* Read Status Register */
#define   WRSR    (0x01u)       /* Write Status Register */
#define   READ    (0x03u)       /* Read from memory array */
#define   WRITE   (0x02u)       /* Write to memory array */


#define   MEMORY_STARTADDRESS    (0x0000u)

#define   MEMORY_ENDADDRESS      (0x07FFu)

#define   PAGEZ_SIZE   (32)

#define   TOTAL_SIZE   (2048)



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

void M95160_Init(void);


void M95160_WriteStatusRegister(SPI_Type* pSPIx, uint8_t W_Value);


int M95160_ReadStatusRegister(SPI_Type* pSPIx, uint8_t* R_Value);


int M95160_WriteSingleByteData(SPI_Type* pSPIx, uint16_t Memory_Addr, uint8_t W_Data);


int M95160_WriteSequenceBytesData(SPI_Type* pSPIx, uint16_t Memory_Addr, uint8_t* W_Data, uint8_t W_Length);


int M95160_ReadSequenceBytesData(SPI_Type* pSPIx, uint16_t Memory_Addr, uint8_t* R_Data, uint16_t R_Length);

#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/