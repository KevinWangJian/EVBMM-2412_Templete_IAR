/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: SPI_Driver.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions to help users to use KEA serious 
  *                MCU SPI module.This functions include initializing SPI ports,
  *                reading and writing data by SPI ports.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    07-Nov-2015

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
#ifndef __SPI_DRIVER_H
#define __SPI_DRIVER_H


#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"
#include "arm_cm0.h"

 

 /* Exported types ------------------------------------------------------------*/  

/* KEA serious SPI driver version: 1.0.0 */
#define   SPI_DRIVER_VERSION     (100)



/* SPI ports signal remap declaration */
typedef enum
{
    SPI0_PortBPins = 1,       /* SPI0 remap to Port B */
    SPI0_PortEPins,           /* SPI0 remap to Port E */
    
    SPI1_PortDPins,           /* SPI1 remap to Port D */
    SPI1_PortGPins,           /* SPI1 remap to Port G */
}SPIx_PinsRemap;



/* SPI transport properties declaration */
typedef struct
{
    uint8_t SPRF_MODF_INT_Enable      :1;       /* SPI total interrupt. 0, Interrupts from SPRF and MODF are inhibited—use polling; 1, Request a hardware interrupt when SPRF or MODF is 1; */
    uint8_t Transmit_INT_Enable       :1;       /* SPI transport interrupt. 0, disable transport interrupt; 1, enable transport interrupt; */
	uint8_t Match_INT_Enable          :1;       /* SPI match interrupt. 0, Interrupts from SPMF inhibited (use polling); 1, When SPMF is 1, requests a hardware interrupt; */
    uint8_t MasterOrSlaveMode         :1;       /* SPI works in master mode or slave mode. 0, SPI works in slave mode; 1, SPI works in master mode; */
    uint8_t CPOL                      :1;       /* SPI clock polarity. 0, SCK in idle status is low; 1, SCK in idle status is high; */
    uint8_t CPHA                      :1;       /* SPI clock phase. 0, sampling of data occurs at odd edges of the SCK clock; 1, sampling of data occurs at even edges of the SCK clock; */
    uint8_t LSBFE                     :1;       /* SPI data transport sequence.0, MSB first; 1, LSB first. */
	
	SPIx_PinsRemap Signal_Pins;
}SPIx_ConfigType;



/* SPI transport speed mode */
typedef enum
{
	High_Speed = 1,                  /* 200Kbps */
	Low_Speed,                       /* 1Mbps */
}SPIxSpeed_Typedef;


#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */


int32_t SPI_Init(SPI_Type* SPIx, SPIx_ConfigType *p_Config, SPIxSpeed_Typedef speed_mode);

int32_t SPI_WriteByteData(SPI_Type* SPIx, uint8_t Tx_data);

int32_t SPI_ReadByteData(SPI_Type* SPIx, uint8_t dummy_data, uint8_t* Rx_data);

int32_t SPI_WriteReadByteData(SPI_Type* SPIx, uint8_t Tx_data, uint8_t* Rx_data);


#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/