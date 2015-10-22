/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: M95160_Driver.h
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
#include <stdlib.h>

#include "SKEAZ1284.h"
#include "GPIO_Driver.h"
#include "SPI_Driver.h"

 
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


/* M95160 EEPROM device memory start address. */
#define   MEMORY_STARTADDRESS    (0x0000u)

/* M95160 EEPROM device memory end address. */
#define   MEMORY_ENDADDRESS      (0x07FFu)

/* M95160 EEPROM device memory page size is 32 bytes. */
#define   PAGE_SIZE   (32)

/* M95160 EEPROM device total memory size is 2048 bytes,2KB. */
#define   TOTAL_SIZE   (2048)


/* SPI Port which communicate with M95160 EEPROM device. */
#define   EEPROM_PORT   (SPI0)



/* EEPROM address definition which store all configuration table data. */
#define   PRODUCT_ID_BASEADDRESS    		(0x0000u)		// 12Bytes

#define   HARDWARE_VERSION_BASEADDRESS    	(0x000Cu)		// 3Bytes

#define   PRODUCT_NAME_BASEADDRESS          (0x000Fu)		// 2Bytes

#define   CAN_BAUDRATE_BASEADDRESS          (0x0011u)		// 1Byte

#define   EVBMM_ADDRESS_BASEADDRESS         (0x0012u)       // 1Byte

#define   CELL_NUMBER_BASEADDRESS           (0x0013u)       // 1Bytes

#define   TEMP_NUMBER_BASEADDRESS           (0x0014u)		// 1Byte

#define   VOL_UPTHRESHOLD_BASEADDRESS		(0x0015u)		// 2Bytes

#define   VOL_DOWNTHRESHOLD_BASEADDRESS     (0x0017u)       // 2Bytes

#define   TEMP_UPTHRESHOLD_BASEADDRESS      (0x0019u)       // 1Byte

#define   TEMP_DOWNTHRESHOLD_BASEADDRESS    (0x001Au)		// 1Byte

#define   SAMPLE_MODE_BASEADDRESS           (0x001Bu)       // 1Byte

#define   CURRENT_RANGE_BASEADDRESS         (0x001Cu)		// 2Bytes

#define   CURRENT_K_BASEADDRESS             (0x001Eu)		// 2Bytes

#define   CURRENT_B_BASEADDRESS             (0x0020u)		// 2Bytes


#define   EXTENDED_PARA1_BASEADDRESS        (0x0022u)		// 2Bytes

#define   EXTENDED_PARA2_BASEADDRESS        (0x0024u)		// 2Bytes

#define   EXTENDED_PARA3_BASEADDRESS        (0x0026u)		// 2Bytes

#define   EXTENDED_PARA4_BASEADDRESS 		(0x0028u)		// 2Bytes

#define   EXTENDED_PARA5_BASEADDRESS		(0x002Au)		// 2Bytes

#define   EXTENDED_PARA6_BASEADDRESS		(0x002Cu)		// 2Bytes

#define	  EXTENDED_PARA7_BASEADDRESS		(0x003Eu)		// 2Bytes

#define   EXTENDED_PARA8_BASEADDRESS		(0x0030u)		// 2Bytes

#define   EXTENDED_PARA9_BASEADDRESS		(0x0032u)		// 2Bytes

#define   EXTENDED_PARA10_BASEADDRESS		(0x0034u)		// 2Bytes


	  
#define   WRITE_EEPROM_STATUS_BASEADDRESS   (0x07FEu)		// 1Byte
	  
#define	  ADDRESSMATCH_FLAG_BASEADDRESS     (0x07FFu)		// 1Byte   
	  
	  
	  
#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

void M95160_Init(void);


void M95160_WriteStatusRegister(uint8_t W_Value);


int M95160_ReadStatusRegister(uint8_t* R_Value);


int M95160_WriteSingleByteData(uint16_t Memory_Addr, uint8_t W_Data);


int M95160_WriteSequenceBytesData(uint16_t Memory_Addr, uint8_t* W_Data, uint8_t W_Length);


int M95160_ReadSequenceBytesData(uint16_t Memory_Addr, uint8_t* R_Data, uint16_t R_Length);


#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/