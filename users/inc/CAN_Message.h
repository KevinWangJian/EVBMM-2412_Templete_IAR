/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: CAN_Message.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions which parsing recived CAN messages
  *                and raplying with the specified CAN messages to master.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    26-Sep-2015

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


#ifndef  __CAN_MESSAGE_H
#define  __CAN_MESSAGE_H

#include <stdint.h>
#include <stdlib.h>

#include "arm_cm0.h"
#include "SKEAZ1284.h"
#include "MSCAN_Driver.h"
#include "GPIO_Driver.h"


/* Exported types ------------------------------------------------------------*/

#define   CAN_MESSAGE_VERSION         (100)   /* Version1.0.0 */
	  
/* Global update data buffer size which will be used to store updated data */
#define   UPDATE_BUF_SIZE             (1024)

/* Global configuration table parameter buffer size which will be used to store configuration parameters */
#define   CONFIGTABLE_BUF_SIZE        (12)
	  
	  
/* Internal Flash base address which store the updated program data. */
#define   PROGRAM_BACKUP_BASEADDRESS  (0x00013800u)


/* Tagged word which is used in CAN frame ID area. */
#define   TAGWORD                         (0x18u)

/* EVBCM master device physical address. */
#define   EVBCM_ADDRESS                   (0xF5u)		


/* EVBCM master and EVBMM slaver transmission command definition. */
#define   M_SEND_CONFIGTABLE_CMD         (0x01u)		/* EVBCM master send configuration table to EVBMM slaver command. */
#define   S_CONFIRM_CONFIGTABLE_CMD      (0x02u)       
#define   M_REQUEST_CONFIGTABLE_CMD      (0x03u)
#define   S_REPLY_CONFIGTABLE_CMD        (0x04u)

#define   S_SEND_ADDRESSMATCH_CMD        (0x05u)
#define   M_REPLY_ADDRESSMATCH_CMD       (0x06u)

#define   M_REQUEST_CELLINFO_CMD         (0x10u)
#define   S_REPLY_CELLVOL_CMD         	 (0x11u)
#define   S_REPLY_CELLTEMP_CMD           (0x12u)
#define   S_REPLY_CELLCURRENT_CMD        (0x13u)
#define   S_REPLY_CELLINFO_DONE_CMD      (0x14u)

#define   M_REQUEST_SLAVER_VERSION_CMD   (0x20u)
#define   S_REPLY_SLAVER_VERSION_CMD     (0x21u)

#define   M_START_UPDATE_CMD      	     (0x22u)
#define   S_REQUEST_UPDATEDATA_CMD       (0x23u)
#define   M_SEND_UPDATEDATA_CMD          (0x24u)
#define   M_SEND_UPDATEDATA_DONE_CMD     (0x25u)

#define   M_SEND_BALANCECONTROL_CMD      (0x30u)
#define   S_REPLY_BALANCECONTROL_CMD     (0x31u)

#define   M_SEND_IOCONTROL_CMD           (0x32u)
#define   S_REPLY_IOCONTROL_CMD          (0x33u)

#define   M_REQUEST_DISTATUS_CMD         (0x34u)
#define   S_REPLY_DISTATUS_CMD           (0x35u)

#define   M_SEND_FAULTINQUIRY_CMD        (0x40u)
#define   S_REPLY_FAULTINQUIRY_CMD       (0x41u)



/* Configuration table parameters definition. */
#define   BASECONFIG_PARA_CODE1            (0x01u)
#define   BASECONFIG_PARA_CODE2            (0x02u)
#define   BASECONFIG_PARA_CODE3            (0x03u)
#define   BASECONFIG_PARA_CODE4            (0x04u)
#define   BASECONFIG_PARA_CODE5            (0x05u)
#define   BASECONFIG_PARA_CODE6            (0x06u)
#define   BASECONFIG_PARA_CODE7            (0x07u)
#define   BASECONFIG_PARA_CODE8            (0x08u)
#define   BASECONFIG_PARA_CODE9            (0x09u)
#define   BASECONFIG_PARA_CODE10           (0x0Au)
#define   BASECONFIG_PARA_CODE11           (0x0Bu)
#define   BASECONFIG_PARA_CODE12           (0x0Cu)
#define   BASECONFIG_PARA_CODE13           (0x0Du)
#define   BASECONFIG_PARA_CODE14           (0x0Eu)
#define   BASECONFIG_PARA_CODE15           (0x0Fu)

#define   EXTENDEDCONFIG_PARA_CODE1		   (0x10u)
#define	  EXTENDEDCONFIG_PARA_CODE2		   (0x11u)
#define   EXTENDEDCONFIG_PARA_CODE3		   (0x12u)
#define   EXTENDEDCONFIG_PARA_CODE4		   (0x13u)
#define   EXTENDEDCONFIG_PARA_CODE5		   (0x14u)
#define   EXTENDEDCONFIG_PARA_CODE6		   (0x15u)
#define   EXTENDEDCONFIG_PARA_CODE7		   (0x16u)
#define   EXTENDEDCONFIG_PARA_CODE8		   (0x17u)
#define   EXTENDEDCONFIG_PARA_CODE9		   (0x18u)
#define   EXTENDEDCONFIG_PARA_CODE10	   (0x19u)



typedef struct
{
	uint8_t StartUpdating_Flag;
	
	uint8_t UpdatedPacket_Length;
	
	uint8_t UpdatedPacket_Buffer[UPDATE_BUF_SIZE];
	
	uint16_t UpdatedPacket_WPointer;
}EVBMM_UpdateProperty_TypeDef;



typedef struct
{
	uint8_t ConfigTable_WPointer;
	
	uint8_t ConfigTable_Buffer[CONFIGTABLE_BUF_SIZE];
}EVBMM_ConfigTable_TypeDef;



typedef struct
{
	uint8_t Product_ID[CONFIGTABLE_BUF_SIZE];
	
	uint8_t Hardware_Version[3];
	
	uint16_t Product_Name;
	
	uint8_t EVBMM_Address;
	
	uint8_t Cell_Num;
	
	uint8_t Temp_Num;
	
	uint16_t Voltage_Upthreshold;
	
	uint16_t Voltage_Downthreshold;
	
}EVBMM_PartialConfigTable_TypeDef;



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */
	
void CAN_Transmission_Init(void);


void EVBMM_CANReceiveBufferParsing(void);


void EVBMM_ReadPartialConfigTableData(void);


void EVBMM_PreparePartialConfigTableData(void);




#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/