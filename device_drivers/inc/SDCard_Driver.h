/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: SDCard_Driver.c
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions to help users to use SD card.
  * @Others: None
  * @History: 1. Created by Wangjian.
  *           2. Modify some variable and function name by Wangjian.
  * @version V1.0.1
  * @date    15-Sep-2015

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
#ifndef  __SDCARD_DRIVER_H
#define  __SDCARD_DRIVER_H


#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"
#include "GPIO_Driver.h"


/* Exported types ------------------------------------------------------------*/

#define    SDCARD_DRIVER_VERSION    (1.0.1)       /* SD Card driver code version */

#define    SDCARD_INIT_SUCCESS      (0)
#define    SDCARD_INIT_ERROR        (-1)
#define    SDCARD_PORTINIT_ERROR	(-2)
#define	   SDCARD_TRANSMIT_ERROR    (-3)
#define    SDCARD_EXCUTECMD_ERROR   (-4)

#define    SDCARD_RECEIVEDATA_SUCCESS  (0)
#define    SDCARD_RECEIVEDATA_ERROR    (-1)

#define    SDCARD_READDISK_SUCCESS  (0)
#define    SDCARD_READDISK_ERROR    (-1)

#define    SDCARD_WRITEDISK_SUCCESS  (0)
#define    SDCARD_WRITEDISK_ERROR    (-1)



#define    SDCARD_PORT        (SPI1)

#define    SD_CS_OUTPUT()     GPIO_PinInit(GPIO_PTG7, GPIO_PinOutput)
#define    SD_CS_HIGH()       GPIO_PinSet(GPIO_PTG7)		/* SD Card select pin disable */
#define    SD_CS_LOW()        GPIO_PinClear(GPIO_PTG7)		/* SD Card select pin enable  */


#define    DUMMY_BYTE	0xAA 


#define CMD0    0    
#define CMD1    1       
#define CMD8	8     
#define CMD9    9    
#define CMD10   10     
#define CMD12   12   
#define CMD16   16   
#define CMD17   17      
#define CMD18   18     
#define ACMD23  23     
#define CMD24   24      
#define CMD25   25      
#define ACMD41  41     
#define CMD55   55      
#define CMD58   58    
#define CMD59   59     


#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06



typedef enum
{
	CS_Release = 1,
	CS_Hold,
}CSStatus_TypeDef;



typedef struct                   /* SD Card specific data */
{
	uint8_t  CSDStruct;            /* CSD structure */
	uint8_t  SysSpecVersion;       /* System specification version */
	uint8_t  Reserved1;            /* Reserved */
	uint8_t  TAAC;                 /* Data read access-time 1 */
	uint8_t  NSAC;                 /* Data read access-time 2 in CLK cycles */
	uint8_t  MaxBusClkFrec;        /* Max. bus clock frequency */
	uint16_t CardComdClasses;      /* Card command classes */
	uint8_t  RdBlockLen;           /* Max. read data block length */
	uint8_t  PartBlockRead;        /* Partial blocks for read allowed */
	uint8_t  WrBlockMisalign;      /* Write block misalignment */
	uint8_t  RdBlockMisalign;      /* Read block misalignment */
	uint8_t  DSRImpl;              /* DSR implemented */
	uint8_t  Reserved2;            /* Reserved */
	uint32_t DeviceSize;           /* Device Size */
	uint8_t  MaxRdCurrentVDDMin;   /* Max. read current @ VDD min */
	uint8_t  MaxRdCurrentVDDMax;   /* Max. read current @ VDD max */
	uint8_t  MaxWrCurrentVDDMin;   /* Max. write current @ VDD min */
	uint8_t  MaxWrCurrentVDDMax;   /* Max. write current @ VDD max */
	uint8_t  DeviceSizeMul;        /* Device size multiplier */
	uint8_t  EraseGrSize;          /* Erase group size */
	uint8_t  EraseGrMul;           /* Erase group size multiplier */
	uint8_t  WrProtectGrSize;      /* Write protect group size */
	uint8_t  WrProtectGrEnable;    /* Write protect group enable */
	uint8_t  ManDeflECC;           /* Manufacturer default ECC */
	uint8_t  WrSpeedFact;          /* Write speed factor */
	uint8_t  MaxWrBlockLen;        /* Max. write data block length */
	uint8_t  WriteBlockPaPartial;  /* Partial blocks for write allowed */
	uint8_t  Reserved3;            /* Reserded */
	uint8_t  ContentProtectAppli;  /* Content protection application */
	uint8_t  FileFormatGrouop;     /* File format group */
	uint8_t  CopyFlag;             /* Copy flag (OTP) */
	uint8_t  PermWrProtect;        /* Permanent write protection */
	uint8_t  TempWrProtect;        /* Temporary write protection */
	uint8_t  FileFormat;           /* File Format */
	uint8_t  ECC;                  /* ECC code */
	uint8_t  CSD_CRC;              /* CSD CRC */
	uint8_t  Reserved4;            /* always 1*/
}SDCard_CSD;


typedef struct                   /*Card Identification Data*/
{
	uint8_t  ManufacturerID;       /* ManufacturerID */
	uint16_t OEM_AppliID;          /* OEM/Application ID */
	uint32_t ProdName1;            /* Product Name part1 */
	uint8_t  ProdName2;            /* Product Name part2*/
	uint8_t  ProdRev;              /* Product Revision */
	uint32_t ProdSN;               /* Product Serial Number */
	uint8_t  Reserved1;            /* Reserved1 */
	uint16_t ManufactDate;         /* Manufacturing Date */
	uint8_t  CID_CRC;              /* CID CRC */
	uint8_t  Reserved2;            /* always 1 */
}SDCard_CID;


typedef struct 
{
	SDCard_CSD CSD;              /* SD card CSD register */
	  
	SDCard_CID CID;              /* SD card CID register */
	  
	uint32_t Capacity;           /* SD card Capacity */
	  
	uint32_t BlockSize;          /* SD card Block Size */
	  
	uint16_t RCA;                /* SD relative card address register */
	  
	uint8_t CardType;            /* SD card type */
	  
	uint32_t SpaceTotal;         /* Total space size in file system */
	  
	uint32_t SpaceFree;          /* Free space size in file system */
}SDCardInfo,*PSDCardInfo;



#ifdef __cplusplus
extern "C" {
#endif


/* Exported functions ------------------------------------------------------- */


int SDCard_Init(void);


int SDCard_ReadDisk(uint8_t* R_buf, uint32_t sector, uint8_t count);


int SDCard_WriteDisk(uint8_t* W_Buf, uint32_t sector, uint8_t count);


#ifdef __cplusplus
}
#endif

#endif   /* __SDCARD_DRIVER_H */

/*****************************END OF FILE**************************************/


