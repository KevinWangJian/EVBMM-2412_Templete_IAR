/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: MSCAN_Driver.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about MSCAN module initialization,
  *                configure MSCAN module filters, send and receive CAN messages
  *                functions.
  * @Others:  None
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

#ifndef  __MSCAN_DRIVER_H
#define  __MSCAN_DRIVER_H

#include <stdint.h>
#include "SKEAZ1284.h"


/* Exported types ------------------------------------------------------------*/

/* Declaration MSCAN driver version */
#define   MSCAN_DRIVER_VERSION     (100)		/* Rev1.0.0 */

	  

/* MSCAN baud rate enumeration */
typedef enum 
{
	MSCAN_Baudrate_50K = 1,						/* Baud rate 50Kbps */
	MSCAN_Baudrate_100K,						/* Baud rate 100Kbps */
	MSCAN_Baudrate_125K,						/* Baud rate 125Kbps */
	MSCAN_Baudrate_250K,						/* Baud rate 250Kbps */
	MSCAN_Baudrate_500K,						/* Baud rate 500Kbps */
	MSCAN_Baudrate_1M,							/* Baud rate 1000Kbps */
}MSCAN_BaudRate_TypeDef;


	
/* MSCAN module properties parameter declaration */
typedef struct
{
	uint8_t MSCAN_SignalPinsRemap         :1;  /* 0:MSCAN signal pins remap on PTC7(TX) and PTC6(RX); 1:MSCAN signal pins remap on PTE7(TX) and PTH2(RX). */
	uint8_t MSCAN_StopInWaitMode          :1;  /* 0:MSCAN module is normal in wait mode; 1:MSCAN module is stop in wait mode. */
	uint8_t MSCAN_TimeStampEnable         :1;  /* 0:Disable internal MSCAN timer; 1:Enable internal MSCAN timer. */
	uint8_t MSCAN_WakeUpEnable            :1;  /* 0:Wake-up disabled; 1:Wake-up enabled. If MSCAN wake-up enabled,MSCAN_WakeUpINTEnable must be set. */
    uint8_t MSCAN_ClockSource             :1;  /* 0:MSCAN clock source is the oscillator clock; 1:MSCAN clock is the bus clock. */
    uint8_t MSCAN_LoopbackMode            :1;  /* 0:Loop back self test disabled. 1:Loop back self test enabled. */
    uint8_t MSCAN_ListenOnlyMode          :1;  /* 0:Normal operation;1:Listen only mode. */
    uint8_t MSCAN_BusoffRecoveryMode      :1;  /* 0:Automatic bus-off recovery; 1:bus-off recovery upon user request. */
    uint8_t MSCAN_WakeUpMode              :1;  /* 0:Wakes up on any dominant level on CAN bus;1:Wakes up only in case of a dominant pulse on the CAN bus that has a delay time. */
    uint8_t MSCAN_WakeUpINTEnable         :1;  /* 0:Wake-up interrupt disable; 1:Wake-up interrupt enable. */
    uint8_t MSCAN_OverrunINTEnable        :1;  /* 0:Overrun interrupt disable; 1:Overrun interrupt enable. */
    uint8_t MSCAN_ReceiveFullINTEnable    :1;  /* 0:Receive full interrupt disable; 1: Receive full interrupt enable. */
    uint8_t MSCAN_TransEmptyINTEnable     :3;  /* 0:Transmitter empty interrupt disable; !0:Transmitter empty interrupt enable. */
	
	MSCAN_BaudRate_TypeDef  baudrate;          /* MSCAN baud rate. */
}MSCAN_ParametersConfig;



/* CAN Frame type and ID format enumeration */
typedef enum 
{
	DataFrameWithStandardId = 1,
	RemoteFrameWithStandardId,
	DataFrameWithExtendedId,
	RemoteFrameWithExtendedId,
}MSCAN_FrameTypeAndIdFormat_Def;



/* CAN message format declaration */
typedef struct 
{
	uint8_t data[8];									/* 8 bytes data */
	uint8_t data_length;								/* CAN frame data length */
	uint32_t frame_id;									/* CAN frame ID value */
	MSCAN_FrameTypeAndIdFormat_Def frame_type;			/* CAN frame type and ID format */
} MSCAN_Message_TypeDef;



/* CAN filters accept ID format enumeration */ 
typedef enum 
{
	OnlyAcceptStandardID, 
	OnlyAcceptExtendedID,
} AcceptIDFormat;



/* CAN filters accept frame type enumeration */
typedef enum 
{
	OnlyAcceptRemoteFrame, 
	OnlyAcceptDataFrame, 
	AcceptBothFrame,
} AcceptFrameType;



/* CAN filters parameters declaration */
typedef struct 
{
	uint8_t Filter_Enable        :1;	/* 0,filter closed; 1,filter opened. If user close CAN filter,the following parameters does not work. */
	uint8_t Filter_Channel       :1;	/* Filter channel selection.0: channel0; 1: channel1; */
	AcceptIDFormat id_format;			/* CAN filters accept ID format. */
	AcceptFrameType frame_type;			/* CAN filters accept frame type. */
	
	uint32_t id_mask;					/* This variable is mask received ID value. This variable value should be inverted of id_accept value if 
	                                       user assign the special received ID value.If user set this variable to 0xFFFFFFFF,it means that the 
	                                       filter identifier mask register can't work. */ 
	uint32_t id_accept;					/* This variable is identifier acceptance received ID value,user can assign it */
} MSCAN_FilterConfigure;



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */
		
int32_t MSCAN_Init(MSCAN_ParametersConfig* CAN_Config, MSCAN_FilterConfigure* Filter_Config);


int32_t MSCAN_SendFrame(MSCAN_Message_TypeDef* W_Framebuff);


int32_t MSCAN_ReceiveFrame(MSCAN_Message_TypeDef* R_Framebuff);


int32_t MSCAN_HardTxBufferCheck(void);


#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/