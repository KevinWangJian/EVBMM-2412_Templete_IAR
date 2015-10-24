/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: CAN_Message.c
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

/* Includes ------------------------------------------------------------------*/

#include "CAN_Message.h"
#include "CAN_SoftBuffer.h"
#include "ISR.h"
#include "PIT_Driver.h"
#include "System_Init.h"
#include "MSCAN_Driver.h"
#include "M95160_Driver.h"
#include "Flash_Driver.h"
#include "LTC6804_Driver.h"




static volatile EVBMM_UpdateProperty_TypeDef g_Update_Data;

static volatile EVBMM_ConfigTable_TypeDef g_ConfigurationTable_Data;

static volatile EVBMM_PartialConfigTable_TypeDef g_PartialConfigData;


static volatile uint8_t g_AddressMatchFlag = 0;


/* EVBMM software version: V1.0.0 */
static const uint8_t EVBMM_SoftWare_Version[3] = {0x01u, 0x00u, 0x00u};




const uint8_t auchCRCHi[]={
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
const uint8_t auchCRCLo[]={
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

static uint16_t CRC16_Calc(uint8_t *puchMsg, uint32_t usDataLen)
{	
    uint8_t uchCRCHi = 0xFFu;
    uint8_t uchCRCLo = 0xFFu;
    uint8_t uIndex;
	
    while (usDataLen--)
    {
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
    }
	
    return ((uint16_t)(uchCRCHi << 8) | uchCRCLo) ;
}



/**
 * @brief   EVBMM slaver reply with cell voltage data to EVBCM master and send 
 *          CAN messages.
 * @param   None
 * @returns None
 */
static void EVBMM_ReplyCellVoltage(void)
{
	uint8_t i, j, m, n;
	uint8_t sta_val = 0;
	
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_CELLVOL_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	i = Para_Struct.CellNum / 3;
	j = Para_Struct.CellNum % 3;
	
	m = 0;
	
	for (; m < i; m++)
	{
		/* Assign cell serial number. */
		Sd_Message.data[0] = 3*m + 1;
		
		for (n = 0; n < 3; n++)
		{
			if (Collect_Data.Vol_Flag[Sd_Message.data[0] - 1 + n] == 0)
				sta_val &= ~(uint8_t)(1 << n);
			else
				sta_val |= (uint8_t)(1 << n);
		}
		
		/* Assign cell voltage status value. */
		Sd_Message.data[1] = sta_val;
		
		/* Assign real cell voltage value. */
		for (n = 0; n < 3; n++)
		{
			Sd_Message.data[2 + 2*n] = (uint8_t)((Collect_Data.Vol[Sd_Message.data[0] - 1 + n] >> 8) & 0x00FFu);
			Sd_Message.data[3 + 2*n] = (uint8_t)(Collect_Data.Vol[Sd_Message.data[0] - 1 + n] & 0x00FFu);					
		}
		
		Sd_Message.data_length = 8;
								
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
	
	sta_val = 0x00u;
	
	if (j != 0)
	{	
		/* Assign cell serial number. */
		Sd_Message.data[0] = 3*m + 1;
		
		for (n = 0; n < j; n++)
		{
			if (Collect_Data.Vol_Flag[Sd_Message.data[0] - 1 + n] == 0)
				sta_val &= ~(uint8_t)(1 << n);
			else
				sta_val |= (uint8_t)(1 << n);					
		}
		
		/* Assign cell voltage status value. */
		Sd_Message.data[1] = sta_val;
		
		/* Assign real cell voltage value. */
		for (n = 0; n < j; n++)
		{
			Sd_Message.data[2 + 2*n] = (uint8_t)((Collect_Data.Vol[Sd_Message.data[0] - 1 + n] >> 8) & 0x00FFu);
			Sd_Message.data[3 + 2*n] = (uint8_t)( Collect_Data.Vol[Sd_Message.data[0] - 1 + n] & 0x00FFu);
		}
		
		switch (j)
		{
			case 1:Sd_Message.data_length = 4;break;
			case 2:Sd_Message.data_length = 6;break;
			default:return;
		}
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
}



/**
 * @brief   EVBMM slaver reply with cell temperature data to EVBCM master and send
 *          CAN messages.
 * @param   None
 * @returns None
 */
static void EVBMM_ReplyCellTemperature(void)
{
	uint8_t i, j, m, n;
	uint8_t sta_val = 0;
	
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_CELLTEMP_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8)
					      |(g_PartialConfigData.EVBMM_Address));
	
	i = Para_Struct.TempNum / 6;
	j = Para_Struct.TempNum % 6;
	
	m = 0;
	
	for (; m < i; m++)
	{
		/* Assign temperature sensor serial number. */
		Sd_Message.data[0] = 6*m + 1;
		
		for (n = 0; n < 6; n++)
		{
			if (Collect_Data.CellTemp_Flag[Sd_Message.data[0] - 1 + n] == 0)
				sta_val &= ~(uint8_t)(1 << n);
			else 
				sta_val |= (uint8_t)(1 << n);
		}
		
		/* Assign temperature sensor status value. */
		Sd_Message.data[1] = sta_val;
		
		/* Assign real cell temperature value. */
		for (n = 0; n < 6; n++)
		{
			Sd_Message.data[2 + n] = Collect_Data.CellTemp[Sd_Message.data[0] - 1 + n];
		}
		
		Sd_Message.data_length = 8;
				
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
	
	sta_val = 0x00u;
	
	if (j != 0)
	{	
		Sd_Message.data[0] = 6*m + 1;
		
		for (n = 0; n < j; n++)
		{
			if (Collect_Data.CellTemp_Flag[Sd_Message.data[0] - 1 + n] == 0)
				sta_val &= ~(uint8_t)(1 << n);
			else
				sta_val |= (uint8_t)(1 << n);
		}
		
		Sd_Message.data[1] = sta_val;
		
		for (n = 0; n < j; n++)
		{
			Sd_Message.data[2 + n] = Collect_Data.CellTemp[Sd_Message.data[0] - 1 + n];
		}
		
		switch (j)
		{
			case 1:Sd_Message.data_length = 3;break;
			case 2:Sd_Message.data_length = 4;break;
			case 3:Sd_Message.data_length = 5;break;
			case 4:Sd_Message.data_length = 6;break;
			case 5:Sd_Message.data_length = 7;break;
			default:return;
		}
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}			
}



/**
 * @brief   EVBMM slaver reply with cell current data to EVBCM master.
 * @param   None
 * @returns None
 */
static void EVBMM_ReplyCellCurrent(void)
{	
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_CELLCURRENT_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	Sd_Message.data[0] = 0x03u;
	Sd_Message.data[1] = 0xE8u;
	Sd_Message.data[2] = 0x00u;
	
	Sd_Message.data_length = 3;
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);
}



/**
 * @brief     EVBMM slaver reply with all cell information data to EVBCM master finished.
 * @attention When any cell information has completely sent out,this function should be 
 *            called to notice EVBCM master that all data has been transmitted finished.
 * @param     None
 * @returns   None
 */
static void EVBMM_ReplyCellInfoFinished(void)
{
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	/* EVBMM slaver send cell information finished command code to EVBCM master. */
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_CELLINFO_DONE_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	/* The CAN message data field is reserved by default.So we do not give any data to transmit. */

	Sd_Message.data_length = 0;
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);	
}



/**
 * @brief   EVBCM master request EVBMM slaver software version value.
 * @param   None
 * @returns None
 */
static void EVBCM_RequestSlaverVersion(void)
{
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	/* EVBMM slaver send software version to EVBCM master. */
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_SLAVER_VERSION_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	/* Software version data,the following must be modified according to the program. */
	Sd_Message.data[0] = EVBMM_SoftWare_Version[0];
	Sd_Message.data[1] = EVBMM_SoftWare_Version[1];
	Sd_Message.data[2] = EVBMM_SoftWare_Version[2];
	
	Sd_Message.data_length = 3;
	
	Fill_CANSendBuffer(&Sd_Message);		
}



/**
 * @brief   EVBCM master request EVBMM slaver to send cell infomation which including
 *          cell voltage, cell temperature and cell current.
 * @param   *Rd_Message, Data buffer which store EVBMM slaver received CAN message.
 * @returns None
 */
static void EVBCM_RequestCellInfo(MSCAN_Message_TypeDef* Rd_Message)
{
	/* EVBCM master request EVBMM slaver to response with cell voltage,cell temperature and cell current together. */
	if (Rd_Message->data[0] == 0xFFu)
	{
		EVBMM_ReplyCellVoltage();
		
		EVBMM_ReplyCellTemperature();
		
		EVBMM_ReplyCellCurrent();
	}
	/* EVBCM master request EVBMM slaver to response with cell voltage,cell temperature and cell current step by step. */
	else
	{
		/* Just response cell voltage information to EVBCM master. */
		if (Rd_Message->data[0] == 0x01u)   	
		{
			EVBMM_ReplyCellVoltage();
		}
		/* Just response cell temperature information to EVBCM master. */
		else if (Rd_Message->data[0] == 0x02u)	
		{
			EVBMM_ReplyCellTemperature();	
		}
		/* Just response cell current information to EVBCM master. 
		   Attention: This function is not ready in current version,
		   so the following program is reserved. */
		else if (Rd_Message->data[0] == 0x03u)  
		{
			EVBMM_ReplyCellCurrent();
		}
		else
		{
			return;
		}
	}
	
	/* EVBMM slaver send cell information finished command code to EVBCM master. */
	EVBMM_ReplyCellInfoFinished();
}



/**
 * @brief   EVBMM slaver write the received configuration table data to EEPROM and reply with confirmation command to EVBCM master.
 * @param   Bytes_Count, The received data bytes quantity.
 *          W_MemoryAddress, The specified EEPROM memory address.
 *          Para_Code, The specified parameter about configuration table.
 *          *R_Message, CAN message buffer which is EVBMM slaver received.
 * @returns None
 */
static void EVBMM_WriteConfigTable(uint8_t Bytes_Count, uint16_t W_MemoryAddress, 
								   uint8_t Para_Code, MSCAN_Message_TypeDef* R_Message)
{
	uint8_t i, j, Sta_val;
	
	uint8_t Temp_Buff[12];
	
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;

	/* Get the received parameters data and store them. */
	for (i = 0; i < Bytes_Count; i++)
	{
		g_ConfigurationTable_Data.ConfigTable_Buffer[g_ConfigurationTable_Data.ConfigTable_WPointer++] = R_Message->data[2 + i];
	}
	
	/* Before data writting,clear EEPROM writting error flag. */
	Sta_val = 0x00u;
	
	if (Para_Code != MODIFY_PARA_CODE1)
	{
		/* Loop while for three times if Writting EEPROM failed. */
		for (j = 0; j < 3; j++)
		{
			/* Write the specified parameters value to EEPROM. */
			for (i = 0; i < g_ConfigurationTable_Data.ConfigTable_WPointer; i++)
			{
				(void)M95160_WriteSingleByteData((W_MemoryAddress + i), (uint8_t)g_ConfigurationTable_Data.ConfigTable_Buffer[i]);
			}
			
			/* If write successfully,then read them out and comparing. */
			(void)M95160_ReadSequenceBytesData(W_MemoryAddress, Temp_Buff, g_ConfigurationTable_Data.ConfigTable_WPointer);
			
			/* Judge whether the parameter is Product ID. */
			if (R_Message->data[0] != BASECONFIG_PARA_CODE1)
			{
				for (i = 0; i < Bytes_Count; i++)
				{
					if (Temp_Buff[i] != g_ConfigurationTable_Data.ConfigTable_Buffer[i])
					{
						Sta_val = 0xCCu;
						
						break;
					}
					else
					{
						Sta_val = 0x00u;
					}
				}		
			}
			/* If yes,the parameter length must be 12 bytes. */
			else
			{
				for (i = 0; i < (2*Bytes_Count); i++)
				{
					if (Temp_Buff[i] != g_ConfigurationTable_Data.ConfigTable_Buffer[i])
					{
						Sta_val = 0xCCu;
						
						break;
					}
					else
					{
						Sta_val = 0x00u;
					}
				}
			}

			/* If the data is checked with no problem,jump out the loop while process. */
			if (Sta_val == 0x00u)break;
		}
	}
	
	/* If writting parameters data to EEPROM successfully,EVBMM slaver should reply with confirm command. */
	if (Sta_val == 0x00u)
	{
		Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
							  |((uint32_t)(S_CONFIRM_CONFIGTABLE_CMD) << 16) 
							  |((uint16_t)(EVBCM_ADDRESS) << 8) 
							  |(g_PartialConfigData.EVBMM_Address));
		
		Sd_Message.data[0] = Para_Code;
		
		Sd_Message.data_length = 1;
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
	
	/* Save writting EEPROM status flag. */
	(void)M95160_WriteSingleByteData(WRITE_EEPROM_STATUS_BASEADDRESS, Sta_val);
	
	g_ConfigurationTable_Data.ConfigTable_WPointer = 0;
}



/**
 * @brief   EVBMM slaver read configuration table data from EEPROM and send them to EVBCM master.
 * @param   Bytes_Count, The data length which will be read.
 *          R_MemoryAddress, The specified EEPROM memory address.
 *          Para_Code, The specified parameter about configuration table.
 * @returns None
 */
static void EVBMM_ReadSendConfigTable(uint8_t Bytes_Count, uint16_t R_MemoryAddress, uint8_t Para_Code)
{
	uint8_t i;
	
	uint8_t Temp_Buff[12];
	
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_CONFIGTABLE_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	/* Reading the specified memory address in EEPROM which store the configuration table. */
	(void)M95160_ReadSequenceBytesData(R_MemoryAddress, Temp_Buff, Bytes_Count);
	
	/* Judge whether the specified parameter is product ID. */
	if (Para_Code != BASECONFIG_PARA_CODE1)
	{
		Sd_Message.data[0] = Para_Code;
		Sd_Message.data[1] = 0xFFu;
		
		for(i = 0; i < Bytes_Count; i++)
		{
			Sd_Message.data[2 + i] = Temp_Buff[i]; 
		}
		
		Sd_Message.data_length = 2 + Bytes_Count;		
	}
	/* If yes,send the product id value with 12 bytes. */
	else
	{
		Sd_Message.data[0] = BASECONFIG_PARA_CODE1;
		Sd_Message.data[1] = 1;			
		
		for(i = 0; i < 6; i++)
		{
			Sd_Message.data[2 + i] = Temp_Buff[i]; 
		}
		
		Sd_Message.data_length = 8;
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
		
		
		Sd_Message.data[1] = 0xFFu;			
		
		for(i = 0; i < 6; i++)
		{
			Sd_Message.data[2 + i] = Temp_Buff[i + 6]; 
		}
		
		Sd_Message.data_length = 8;
	}
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);
}



/**
 * @brief   EVBCM master send configuration table data to EVBMM slaver.
 * @param   *Rd_Message, Data buffer which store EVBMM slaver received CAN message.
 * @returns None
 */
static void EVBCM_SendAllConfigTableData(MSCAN_Message_TypeDef* Rd_Message)
{
	uint8_t i, Para_Code;
	uint16_t dummy_address = 0;

	/* Read and get configuration table parameters code value. */
	Para_Code = Rd_Message->data[0];     
	
	/* Judge whether the current message is the last one or not. */
	if (Rd_Message->data[1] == 0xFFu)
	{
		switch (Para_Code)
		{
		case BASECONFIG_PARA_CODE1:      EVBMM_WriteConfigTable(6, PRODUCT_ID_BASEADDRESS, BASECONFIG_PARA_CODE1, Rd_Message);          break;		/* Product ID value. */
		case BASECONFIG_PARA_CODE2: 	 EVBMM_WriteConfigTable(3, HARDWARE_VERSION_BASEADDRESS, BASECONFIG_PARA_CODE2, Rd_Message);    break;		/* Hardware version. */
		case BASECONFIG_PARA_CODE3: 	 EVBMM_WriteConfigTable(2, PRODUCT_NAME_BASEADDRESS, BASECONFIG_PARA_CODE3, Rd_Message);        break;		/* Product name. */
		case BASECONFIG_PARA_CODE4: 	 EVBMM_WriteConfigTable(1, CAN_BAUDRATE_BASEADDRESS, BASECONFIG_PARA_CODE4, Rd_Message);        break;		/* CAN Transmission baud rate. */
		case BASECONFIG_PARA_CODE5: 	 EVBMM_WriteConfigTable(1, EVBMM_ADDRESS_BASEADDRESS, BASECONFIG_PARA_CODE5, Rd_Message);	    break;		/* EVBMM slaver address. */
		case BASECONFIG_PARA_CODE6: 	 EVBMM_WriteConfigTable(1, CELL_NUMBER_BASEADDRESS, BASECONFIG_PARA_CODE6, Rd_Message);         break;		/* Cell numbers. */
		case BASECONFIG_PARA_CODE7: 	 EVBMM_WriteConfigTable(1, TEMP_NUMBER_BASEADDRESS, BASECONFIG_PARA_CODE7, Rd_Message);         break;		/* Temperature sensor numbers. */
		case BASECONFIG_PARA_CODE8: 	 EVBMM_WriteConfigTable(2, VOL_UPTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE8, Rd_Message);     break;		/* Voltage Up threshold value. */
		case BASECONFIG_PARA_CODE9: 	 EVBMM_WriteConfigTable(2, VOL_DOWNTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE9, Rd_Message);   break;		/* Volatge Down threshold value. */
		case BASECONFIG_PARA_CODE10: 	 EVBMM_WriteConfigTable(1, TEMP_UPTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE10, Rd_Message);   break;		/* Temperature Up threshold value. */
		case BASECONFIG_PARA_CODE11: 	 EVBMM_WriteConfigTable(1, TEMP_DOWNTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE11, Rd_Message); break;		/* Temperature Down threshold value. */
		case BASECONFIG_PARA_CODE12: 	 EVBMM_WriteConfigTable(1, SAMPLE_MODE_BASEADDRESS, BASECONFIG_PARA_CODE12, Rd_Message);        break;		/* Sample mode. */
		case BASECONFIG_PARA_CODE13: 	 EVBMM_WriteConfigTable(2, CURRENT_RANGE_BASEADDRESS, BASECONFIG_PARA_CODE13, Rd_Message);      break;		/* Current sensor range. */
		case BASECONFIG_PARA_CODE14:     EVBMM_WriteConfigTable(2, CURRENT_K_BASEADDRESS, BASECONFIG_PARA_CODE14, Rd_Message);          break;		/* Current K value. */
		case BASECONFIG_PARA_CODE15:     EVBMM_WriteConfigTable(2, CURRENT_B_BASEADDRESS, BASECONFIG_PARA_CODE15, Rd_Message);          break;		/* Current B value. */
		
		case EXTENDEDCONFIG_PARA_CODE1:  EVBMM_WriteConfigTable(2, EXTENDED_PARA1_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE1, Rd_Message);  break;      /* Extended Parameter1. */
		case EXTENDEDCONFIG_PARA_CODE2:  EVBMM_WriteConfigTable(2, EXTENDED_PARA2_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE2, Rd_Message);  break;		/* Extended Parameter2. */
		case EXTENDEDCONFIG_PARA_CODE3:  EVBMM_WriteConfigTable(2, EXTENDED_PARA3_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE3, Rd_Message);  break;		/* Extended Parameter3. */
		case EXTENDEDCONFIG_PARA_CODE4:  EVBMM_WriteConfigTable(2, EXTENDED_PARA4_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE4, Rd_Message);  break;		/* Extended Parameter4. */
		case EXTENDEDCONFIG_PARA_CODE5:  EVBMM_WriteConfigTable(2, EXTENDED_PARA5_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE5, Rd_Message);  break;		/* Extended Parameter5. */
		case EXTENDEDCONFIG_PARA_CODE6:  EVBMM_WriteConfigTable(2, EXTENDED_PARA6_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE6, Rd_Message);  break;		/* Extended Parameter6. */
		case EXTENDEDCONFIG_PARA_CODE7:  EVBMM_WriteConfigTable(2, EXTENDED_PARA7_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE7, Rd_Message);  break;		/* Extended Parameter7. */
		case EXTENDEDCONFIG_PARA_CODE8:  EVBMM_WriteConfigTable(2, EXTENDED_PARA8_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE8, Rd_Message);  break;		/* Extended Parameter8. */
		case EXTENDEDCONFIG_PARA_CODE9:  EVBMM_WriteConfigTable(2, EXTENDED_PARA9_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE9, Rd_Message);  break;		/* Extended Parameter9. */
		case EXTENDEDCONFIG_PARA_CODE10: EVBMM_WriteConfigTable(2, EXTENDED_PARA10_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE10, Rd_Message);break;		/* Extended Parameter10. */
		
		case MODIFY_PARA_CODE1:          
		{
			EVBMM_WriteConfigTable(0, dummy_address, MODIFY_PARA_CODE1, Rd_Message);	
			
			(void)M95160_WriteSingleByteData(ADDRESSMATCH_FLAG_BASEADDRESS, 0x00u);
		
			g_AddressMatchFlag = 0x00u;
		}
		break;
		
		default:break;
		}
	}
	/* The current message is not the last one about the specified parameters. */
	else
	{
		if (Rd_Message->data[1] == 0x01u)
		{
			/* Before start saving parameters data,clear the writting pointer about configuration table buffer. */
			g_ConfigurationTable_Data.ConfigTable_WPointer = 0;
			
			if (BASECONFIG_PARA_CODE1 == Para_Code)
			{
				for (i = 0; i < 6; i++)
				{
					g_ConfigurationTable_Data.ConfigTable_Buffer[g_ConfigurationTable_Data.ConfigTable_WPointer++] = Rd_Message->data[2 + i];			
				}
			}
		}
	}
}



/**
 * @brief   EVBMM slaver reply configuration table data to EVBCM master.
 * @param   *Rd_Message, The CAN message buffer which EVBMM slaver received.
 * @returns None
 */
static void EVBCM_RequestConfigTable(MSCAN_Message_TypeDef* Rd_Message)
{
	uint8_t Para_Code;
	
	Para_Code = Rd_Message->data[0];
	
	switch (Para_Code)
	{
	case BASECONFIG_PARA_CODE1: 	 EVBMM_ReadSendConfigTable(12, PRODUCT_ID_BASEADDRESS, BASECONFIG_PARA_CODE1);      	break;
	case BASECONFIG_PARA_CODE2: 	 EVBMM_ReadSendConfigTable(3,  HARDWARE_VERSION_BASEADDRESS, BASECONFIG_PARA_CODE2); 	break;
	case BASECONFIG_PARA_CODE3: 	 EVBMM_ReadSendConfigTable(2,  PRODUCT_NAME_BASEADDRESS, BASECONFIG_PARA_CODE3);     	break;
	case BASECONFIG_PARA_CODE4: 	 EVBMM_ReadSendConfigTable(1,  CAN_BAUDRATE_BASEADDRESS, BASECONFIG_PARA_CODE4);		break;
	case BASECONFIG_PARA_CODE5: 	 EVBMM_ReadSendConfigTable(1,  EVBMM_ADDRESS_BASEADDRESS, BASECONFIG_PARA_CODE5);		break;
	case BASECONFIG_PARA_CODE6: 	 EVBMM_ReadSendConfigTable(1,  CELL_NUMBER_BASEADDRESS, BASECONFIG_PARA_CODE6);			break;
	case BASECONFIG_PARA_CODE7: 	 EVBMM_ReadSendConfigTable(1,  TEMP_NUMBER_BASEADDRESS, BASECONFIG_PARA_CODE7);			break;
	case BASECONFIG_PARA_CODE8: 	 EVBMM_ReadSendConfigTable(2,  VOL_UPTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE8);		break;
	case BASECONFIG_PARA_CODE9: 	 EVBMM_ReadSendConfigTable(2,  VOL_DOWNTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE9);	break;
	case BASECONFIG_PARA_CODE10: 	 EVBMM_ReadSendConfigTable(1,  TEMP_UPTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE10);	break;
	case BASECONFIG_PARA_CODE11: 	 EVBMM_ReadSendConfigTable(1,  TEMP_DOWNTHRESHOLD_BASEADDRESS, BASECONFIG_PARA_CODE11);	break;
	case BASECONFIG_PARA_CODE12: 	 EVBMM_ReadSendConfigTable(1,  SAMPLE_MODE_BASEADDRESS, BASECONFIG_PARA_CODE12);		break;
	case BASECONFIG_PARA_CODE13: 	 EVBMM_ReadSendConfigTable(2,  CURRENT_RANGE_BASEADDRESS, BASECONFIG_PARA_CODE13);		break;
	case BASECONFIG_PARA_CODE14: 	 EVBMM_ReadSendConfigTable(2,  CURRENT_K_BASEADDRESS, BASECONFIG_PARA_CODE14);			break;
	case BASECONFIG_PARA_CODE15:     EVBMM_ReadSendConfigTable(2,  CURRENT_B_BASEADDRESS, BASECONFIG_PARA_CODE15);          break;
	
	case EXTENDEDCONFIG_PARA_CODE1:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA1_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE1);   break;
	case EXTENDEDCONFIG_PARA_CODE2:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA2_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE2);   break;
	case EXTENDEDCONFIG_PARA_CODE3:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA3_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE3);   break;
	case EXTENDEDCONFIG_PARA_CODE4:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA4_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE4);   break;
	case EXTENDEDCONFIG_PARA_CODE5:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA5_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE5);   break;
	case EXTENDEDCONFIG_PARA_CODE6:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA6_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE6);   break;
	case EXTENDEDCONFIG_PARA_CODE7:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA7_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE7);   break;
	case EXTENDEDCONFIG_PARA_CODE8:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA8_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE8);   break;
	case EXTENDEDCONFIG_PARA_CODE9:  EVBMM_ReadSendConfigTable(2, EXTENDED_PARA9_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE9);   break;
	case EXTENDEDCONFIG_PARA_CODE10: EVBMM_ReadSendConfigTable(2, EXTENDED_PARA10_BASEADDRESS, EXTENDEDCONFIG_PARA_CODE10); break;
	default:break;
	}
}



/**
 * @brief   EVBCM master send IO control command to EVBMM slaver.
 * @param   *Rd_Message, CAN message buffer which store slaver received.
 * @returns None
 */
static void EVBCM_RequestIOControl(MSCAN_Message_TypeDef* Rd_Message)
{
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_IOCONTROL_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	Sd_Message.data_length = 0;
	
	if (Rd_Message->data[0] == 0x01u)
	{
		HEATING_IO_HIGH();
		COOL_IO_LOW();
	
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
	else if (Rd_Message->data[0] == 0x02u)
	{
		COOL_IO_HIGH();
		HEATING_IO_LOW();
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
	else
	{
		HEATING_IO_LOW();
		COOL_IO_LOW();
	}
}



/**
 * @brief   EVBCM msater request to read EVBMM slaver DI status value.
 * @param   None
 * @returns None
 */
static void EVBCM_ReadDIStatus(void)
{
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_DISTATUS_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	if (READ_DI_VALUE() == 0)
	{
		Sd_Message.data[0] = 0x00u;
	}
	else
	{
		Sd_Message.data[0] = 0x01u;
	}
	
	Sd_Message.data_length = 1;
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);	
}



/**
 * @brief   EVBCM master request EVBMM slaver to update program and EVBMM slaver
 *          reply with corresponding CAN message.
 *          Actually,this function is assign the corresponding variables.
 * @param   None
 * @returns None
 */
static void EVBCM_StartUpdating(void)
{
	MSCAN_Message_TypeDef Sd_Message;
	
	Sd_Message.frame_type = DataFrameWithExtendedId;
	
	g_Update_Data.StartUpdating_Flag = 0xCCu;

	g_Update_Data.UpdatedPacket_Length = 1;

	g_Update_Data.UpdatedPacket_WPointer = 0;	
	
	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24)
						  |((uint32_t)(S_REQUEST_UPDATEDATA_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	Sd_Message.data[0] = 0x00u;
	
	Sd_Message.data[1] = g_Update_Data.UpdatedPacket_Length;
	
	Sd_Message.data_length = 2;
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);
}



/**
 * @brief   EVBCM send update data and EVBMM receive update data.
 * @param   *Rd_Message, Data buffer which store received CAN meesage.
 * @returns None
 */
static void EVBCM_SendUpdateData(MSCAN_Message_TypeDef* Rd_Message)
{
	uint8_t i;
	
	if (g_Update_Data.StartUpdating_Flag == 0xCCu)
	{
		for (i = 0; i < 8; i++)
		{
			g_Update_Data.UpdatedPacket_Buffer[g_Update_Data.UpdatedPacket_WPointer++] = Rd_Message->data[i];
		}
	}
}



/**
 * @brief   EVBCM finish sending update data to EVBMM slaver.
 * @param   *Rd_Message, Data buffer which store received CAN meesage.
 * @returns None
 */
static void EVBCM_SendUpdateData_Done(MSCAN_Message_TypeDef* Rd_Message)
{
	uint16_t Rec_CRCValue, Calc_CRC;
	uint16_t Rec_Length;
	
	uint8_t i, Code;
	
	MSCAN_Message_TypeDef Sd_Message;
	
	Code = Rd_Message->data[0];

	Rec_CRCValue = ((uint16_t)Rd_Message->data[1] << 8) | (Rd_Message->data[2]);
	
	Rec_Length   = ((uint16_t)Rd_Message->data[3] << 8) | (Rd_Message->data[4]);
	
	Calc_CRC = CRC16_Calc((uint8_t*)g_Update_Data.UpdatedPacket_Buffer, Rec_Length);

	if (Rec_CRCValue == Calc_CRC)
	{
		/* Erase 1KB flash area data. */
		for (i = 0; i < 2; i++)
		{
			Flash_EarseSector(PROGRAM_BACKUP_BASEADDRESS + (g_Update_Data.UpdatedPacket_Length - 1)*1024 + 512*i);
		}
		
		/* Program received 1KB updated data */
		for (i = 0; i < 2; i++)
		{
			Flash_ProgramSector((PROGRAM_BACKUP_BASEADDRESS + (g_Update_Data.UpdatedPacket_Length - 1)*1024 + 512*i), 
								(uint8_t*)&g_Update_Data.UpdatedPacket_Buffer[512*i]);
		}
		
		if (Code == 0x55u)
		{
			g_Update_Data.UpdatedPacket_WPointer = 0;
			
			g_Update_Data.UpdatedPacket_Length++;
			
			if (g_Update_Data.UpdatedPacket_Length >= 0xFFu)g_Update_Data.UpdatedPacket_Length = 1;
			
			Sd_Message.frame_type = DataFrameWithExtendedId;
		
			Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
								  |((uint32_t)(S_REQUEST_UPDATEDATA_CMD) << 16) 
								  |((uint16_t)(EVBCM_ADDRESS) << 8) 
								  |(g_PartialConfigData.EVBMM_Address));
			
			Sd_Message.data[0] = 0x00u;
			
			Sd_Message.data[1] = g_Update_Data.UpdatedPacket_Length;
			
			Sd_Message.data_length = 2;
			
			/* Fill the prepared CAN message into sent CAN message buffers. */
			Fill_CANSendBuffer(&Sd_Message);
		}
		else if (Code == 0xAAu)
		{
			g_Update_Data.StartUpdating_Flag = 0x00u;
			
			g_Update_Data.UpdatedPacket_WPointer = 0;
		}
	}
}



/**
 * @brief   EVBCM master send partial configuration table data to EVBMM slaver.
 * @param   *Rd_Message, Data buffer which is EVBMM slaver received.
 * @returns None
 */
static void EVBCM_ReplyAddressMatch(MSCAN_Message_TypeDef* Rd_Message)
{
	if (Rd_Message->data[0] == 0xFFu)
	{
		(void)M95160_WriteSingleByteData(TEMP_UPTHRESHOLD_BASEADDRESS, Rd_Message->data[1]);
		
		
		(void)M95160_WriteSingleByteData(TEMP_DOWNTHRESHOLD_BASEADDRESS, Rd_Message->data[2]);
		
		
		(void)M95160_WriteSingleByteData(SAMPLE_MODE_BASEADDRESS, Rd_Message->data[3]);
		
		
		(void)M95160_WriteSingleByteData(CURRENT_RANGE_BASEADDRESS, Rd_Message->data[4]);
		
		(void)M95160_WriteSingleByteData(CURRENT_RANGE_BASEADDRESS + 1, Rd_Message->data[5]);
		
		
		(void)M95160_WriteSingleByteData(CURRENT_K_BASEADDRESS, Rd_Message->data[6]);
		
		(void)M95160_WriteSingleByteData(CURRENT_K_BASEADDRESS + 1, Rd_Message->data[7]);
		
		
		(void)M95160_WriteSingleByteData(ADDRESSMATCH_FLAG_BASEADDRESS, 0xCCu);
		
		g_AddressMatchFlag = 0xCCu;
	}
	else
	{
		g_PartialConfigData.EVBMM_Address = Rd_Message->data[1] + 0x80u;
		
		(void)M95160_WriteSingleByteData(EVBMM_ADDRESS_BASEADDRESS, g_PartialConfigData.EVBMM_Address);
		
		
		
		g_PartialConfigData.Cell_Num = Rd_Message->data[2];
		
		(void)M95160_WriteSingleByteData(CELL_NUMBER_BASEADDRESS, g_PartialConfigData.Cell_Num);
		
		
		
		g_PartialConfigData.Temp_Num = Rd_Message->data[3];
		
		(void)M95160_WriteSingleByteData(TEMP_NUMBER_BASEADDRESS, g_PartialConfigData.Temp_Num);
		
		
		
		g_PartialConfigData.Voltage_Upthreshold = ((uint16_t)Rd_Message->data[4] << 8) | Rd_Message->data[5];
		
		(void)M95160_WriteSingleByteData(VOL_UPTHRESHOLD_BASEADDRESS, Rd_Message->data[4]);
		
		(void)M95160_WriteSingleByteData(VOL_UPTHRESHOLD_BASEADDRESS + 1, Rd_Message->data[5]);
		
		
		
		g_PartialConfigData.Voltage_Downthreshold = ((uint16_t)Rd_Message->data[6] << 8) | Rd_Message->data[7];
		
		(void)M95160_WriteSingleByteData(VOL_DOWNTHRESHOLD_BASEADDRESS, Rd_Message->data[6]);
		
		(void)M95160_WriteSingleByteData(VOL_DOWNTHRESHOLD_BASEADDRESS + 1, Rd_Message->data[7]);
	}
}



/**
 * @brief   EVBCM master send balance control command to EVBMM slaver and slaver reply
 *          with corresponding CAN message to master.
 * @param   *Rd_Messag, Data buffer which store the received CAN message.
 * @returns None
 */
static void EVBCM_SendBalanceControl(MSCAN_Message_TypeDef* Rd_Message)
{
	MSCAN_Message_TypeDef Sd_Message;

	Sd_Message.frame_type = DataFrameWithExtendedId;

	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_BALANCECONTROL_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	Sd_Message.data_length = 0;
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);
}



/**
 * @brief   EVBCM master request EVBMM slaver to send fault code.
 * @param   None
 * @returns None
 */
static void EVBCM_RequestFaultCode(void)
{
	MSCAN_Message_TypeDef Sd_Message;

	Sd_Message.frame_type = DataFrameWithExtendedId;

	Sd_Message.frame_id = (((uint32_t)(TAGWORD) << 24) 
						  |((uint32_t)(S_REPLY_FAULTINQUIRY_CMD) << 16) 
						  |((uint16_t)(EVBCM_ADDRESS) << 8) 
						  |(g_PartialConfigData.EVBMM_Address));
	
	for (int i = 0; i < 8; ++i)
	{
		Sd_Message.data[i] = 0x00u;
	}

	Sd_Message.data_length = 8;
	
	/* Fill the prepared CAN message into sent CAN message buffers. */
	Fill_CANSendBuffer(&Sd_Message);
}



/**
 * @brief   Configure CAN0 bus for communicatting with master.
 * @param   None
 * @returns None
 */
void CAN_Transmission_Init(void)
{
	PIT_TimebaseInitTypeDef PIT_Config;
	
	MSCAN_ParametersConfig CAN_Module_Config;
	MSCAN_FilterConfigure CAN_Filter_Config;

	CAN_Module_Config.baudrate                   = MSCAN_Baudrate_250K;		/* Baud rate 250Kbps */
	CAN_Module_Config.MSCAN_SignalPinsRemap      = 0;                    	/* MSCAN signal pins remap on PTC7(TX) and PTC6(RX) */
	CAN_Module_Config.MSCAN_StopInWaitMode       = 1;						/* MSCAN module is normal in wait mode */
	CAN_Module_Config.MSCAN_TimeStampEnable      = 0;						/* Disable internal MSCAN timer stamp */
	CAN_Module_Config.MSCAN_WakeUpEnable         = 0;						/* Wake-up disabled */
	CAN_Module_Config.MSCAN_LoopbackMode         = 0;						/* Loop back self test disabled */
	CAN_Module_Config.MSCAN_ListenOnlyMode       = 0;						/* Normal operation */
	CAN_Module_Config.MSCAN_BusoffRecoveryMode   = 0;						/* Automatic bus-off recovery */
	CAN_Module_Config.MSCAN_WakeUpMode           = 0;						/* Wakes up on any dominant level on CAN bus */
	CAN_Module_Config.MSCAN_WakeUpINTEnable      = 0;						/* Wake-up interrupt disable */
	CAN_Module_Config.MSCAN_OverrunINTEnable     = 0;						/* Overrun interrupt disable */
	CAN_Module_Config.MSCAN_ReceiveFullINTEnable = 1;						/* Receive full interrupt enable */
	CAN_Module_Config.MSCAN_TransEmptyINTEnable  = 0;						/* Transmitter empty interrupt disable */
	
	CAN_Filter_Config.Filter_Enable  			 = 1;						/* Enable CAN module filters. If this value is cleared,the following parameters does not work */
	CAN_Filter_Config.Filter_Channel 			 = 0;						/* Select CAN module filter channel 0 */
	CAN_Filter_Config.frame_type     			 = OnlyAcceptDataFrame;		/* Enable CAN filter just receive data frame */
	CAN_Filter_Config.id_format      			 = OnlyAcceptExtendedID;	/* Enable CAN filter just receive extended ID */
	CAN_Filter_Config.id_mask        			 = 0xFFFFFFFFu; 			/* When this value is set to 0xFFFFFFFFu,the id_accept value does not work */
	CAN_Filter_Config.id_accept      			 = 0x18901212u;				/* Because the id_mask value has been set to 0xFFFFFFFFu,id_accept does not work and can be set to any value */
	
	(void)MSCAN_Init(&CAN_Module_Config, &CAN_Filter_Config);				/* Call MSCAN_Init function to initialize MSCAN module by specified properties */
	
	Set_Vector_Handler(MSCAN_RX_VECTORn, MSCAN_RX_Handler);					/* Specify MSCAN receive data irq handler function name */
	
	NVIC_EnableIRQ(MSCAN_RX_IRQn);											/* Enable MSCAN receive data NVIC interrupt */
	
	NVIC_SetPriority(MSCAN_RX_IRQn, 0);										/* Set MSCAN_RX interrupt priority to 0(The highest). */
	

	PIT_Config.channel   = PIT_CH1;											/* Choose PIT timer channel 1. */
	PIT_Config.INT_Func  = PIT_INT_ENABLE;									/* Enable PIT timer interrupt. */
	PIT_Config.freeze    = PIT_FreezeEnable;								/* Enable PIT timer when MCU enter freeze mode. */
	PIT_Config.loadvalue = 24000 - 1;           							/* Generate interrupt every 1ms. (1ms * 24000(BUS clock) = 24000) */

	(void)PIT_Init(&PIT_Config);											/* Initialize PIT timer with given property parameters. */
	
	PIT_EnableModule(PIT_CHANNEL1);											/* Enable PIT timer channel 1. */
	
	Set_Vector_Handler(PIT_CH1_VECTORn, PIT_CH1_Handler);					/* Specify PIT_CH1 irq handler function name. */
	
	NVIC_EnableIRQ(PIT_CH1_IRQn);											/* Enable PIT_CH1 NVIC interrupt. */
		
	NVIC_SetPriority(PIT_CH1_IRQn, 1);										/* Set the interrupt priority to 1. */	
}



/**
 * @brief   Parsing EVBMM slaver CAN received message buffers and check if there 
 *          is a valid CAN message.If yes, reading and parsing it!
 * @param   None
 * @returns None
 */
void EVBMM_CANReceiveBufferParsing(void)
{
	MSCAN_Message_TypeDef Rec_Message;
	
	uint8_t Cmd, SA;
	
	/* Check and judge whether there is a valid CAN message in receive buffer. */
	if (Check_CANReceiveBuffer(&Rec_Message) == 0)
	{
		/* Get the received command code value. */
		Cmd = (uint8_t)((Rec_Message.frame_id >> 16) & 0x000000FFu);	
		
		/* Get the received source address(SA) value. */
		SA  = (uint8_t)(Rec_Message.frame_id & 0x000000FFu);			
		
		/* Compare the received source address with EVBCM master address and judge it. */
		if (EVBCM_ADDRESS== SA)										
		{
			switch (Cmd)
			{
			case M_SEND_CONFIGTABLE_CMD:       EVBCM_SendAllConfigTableData(&Rec_Message); break;
			case M_REQUEST_CONFIGTABLE_CMD:    EVBCM_RequestConfigTable(&Rec_Message);     break;
			case M_REPLY_ADDRESSMATCH_CMD:     EVBCM_ReplyAddressMatch(&Rec_Message);      break;
			case M_REQUEST_CELLINFO_CMD:       EVBCM_RequestCellInfo(&Rec_Message);        break;
			case M_REQUEST_SLAVER_VERSION_CMD: EVBCM_RequestSlaverVersion(); 			   break;
			case M_START_UPDATE_CMD:           EVBCM_StartUpdating(); 				       break;
			case M_SEND_UPDATEDATA_CMD:        EVBCM_SendUpdateData(&Rec_Message); 	       break;
			case M_SEND_UPDATEDATA_DONE_CMD:   EVBCM_SendUpdateData_Done(&Rec_Message);    break;
			case M_SEND_BALANCECONTROL_CMD:    EVBCM_SendBalanceControl(&Rec_Message);	   break;
			case M_SEND_IOCONTROL_CMD:         EVBCM_RequestIOControl(&Rec_Message);       break;
			case M_REQUEST_DISTATUS_CMD:       EVBCM_ReadDIStatus(); 				       break;
			case M_SEND_FAULTINQUIRY_CMD:      EVBCM_RequestFaultCode();				   break;
			default: break;
			}
		}
	}
}



/**
 * @brief   EVBMM slaver read and get out partial configuration table data including 
 *          product id,slaver address,cell numbers, temperature numbers, up threshold
 *          and down threshold volatge value.
 * @param   None
 * @returns None
 */
void EVBMM_ReadPartialConfigTableData(void)
{
	uint8_t i, rec_val[12];
	
	(void)M95160_ReadSequenceBytesData(PRODUCT_ID_BASEADDRESS, rec_val, 12);
	
	for (i = 0; i < 12; i++)
	{
		g_PartialConfigData.Product_ID[i] = rec_val[i];
	}
	
	
	(void)M95160_ReadSequenceBytesData(HARDWARE_VERSION_BASEADDRESS, rec_val, 3);
	
	for (i = 0; i < 3; i++)
	{
		g_PartialConfigData.Hardware_Version[i] = rec_val[i];
	}
	
	
	(void)M95160_ReadSequenceBytesData(PRODUCT_NAME_BASEADDRESS, rec_val, 2);
	
	g_PartialConfigData.Product_Name = (((uint16_t)rec_val[0] << 8) | rec_val[1]);
	
	
	(void)M95160_ReadSequenceBytesData(EVBMM_ADDRESS_BASEADDRESS, rec_val, 1);
	
//	g_PartialConfigData.EVBMM_Address = rec_val[0];
	g_PartialConfigData.EVBMM_Address = 0xFFu;
	
	
	(void)M95160_ReadSequenceBytesData(CELL_NUMBER_BASEADDRESS, rec_val, 1);
	
	g_PartialConfigData.Cell_Num = rec_val[0];
	
	
	(void)M95160_ReadSequenceBytesData(TEMP_NUMBER_BASEADDRESS, rec_val, 1);
	
	g_PartialConfigData.Temp_Num = rec_val[0];
	
	
	(void)M95160_ReadSequenceBytesData(VOL_UPTHRESHOLD_BASEADDRESS, rec_val, 2);
	
	g_PartialConfigData.Voltage_Upthreshold = (((uint16_t)rec_val[0] << 8) | rec_val[1]);
	
	
	(void)M95160_ReadSequenceBytesData(VOL_DOWNTHRESHOLD_BASEADDRESS, rec_val, 2);
	
	g_PartialConfigData.Voltage_Downthreshold = (((uint16_t)rec_val[0] << 8) | rec_val[1]);
	
	
	(void)M95160_ReadSequenceBytesData(ADDRESSMATCH_FLAG_BASEADDRESS, rec_val, 1);

	g_AddressMatchFlag = rec_val[0];
}



/**
 * @brief      EVBMM slaver prepare partial configuration table data which including 
 *             product id, slaver address, cell numbers and temperature numbers.
 * @attention  This process is called periodically in TIMER1 ISR Routine until 
 *             EVBCM master reply it with specified CAN message.
 * @param      None
 * @returns    None
 */
void EVBMM_PreparePartialConfigTableData(void)
{
	uint8_t i, packet_length = 1;
	
	MSCAN_Message_TypeDef Sd_Message;
	
	if (g_AddressMatchFlag != 0xCCu)
	{
		/* The first sent CAN message. */
		Sd_Message.frame_type = DataFrameWithExtendedId;
		
		Sd_Message.frame_id   =  (((uint32_t)(TAGWORD) << 24) 
							     |((uint32_t)(S_SEND_ADDRESSMATCH_CMD) << 16) 
							     |((uint16_t)(EVBCM_ADDRESS) << 8) 
							     |(g_PartialConfigData.EVBMM_Address));

		Sd_Message.data[0] = packet_length;
		
		for (i = 0; i < 7; i++)
		{
			Sd_Message.data[i + 1] = g_PartialConfigData.Product_ID[i];
		}
		
		Sd_Message.data_length = 8;
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
		
		
		/* The second sent CAN message. */
		packet_length++;
		
		Sd_Message.data[0] = packet_length;
		
		for (i = 0; i < 5; i++)
		{
			Sd_Message.data[i + 1] = g_PartialConfigData.Product_ID[i + 7];
		}
		
		Sd_Message.data[6] = g_PartialConfigData.Hardware_Version[0];
		
		Sd_Message.data[7] = g_PartialConfigData.Hardware_Version[1];
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
		
		
		/* The third sent CAN message. */
		Sd_Message.data[0] = 0xFFu;			/* 0xFF means that this frame is the last one to be sent. */
		
		Sd_Message.data[1] = g_PartialConfigData.Hardware_Version[2];
		
		Sd_Message.data[2] = (uint8_t)((g_PartialConfigData.Product_Name >> 8) & 0x00FFu);
		
		Sd_Message.data[3] = (uint8_t)((g_PartialConfigData.Product_Name) & 0x00FFu);
		
		Sd_Message.data[4] = g_PartialConfigData.Cell_Num;
		
		Sd_Message.data[5] = g_PartialConfigData.Temp_Num;
		
		Sd_Message.data_length = 6;
		
		/* Fill the prepared CAN message into sent CAN message buffers. */
		Fill_CANSendBuffer(&Sd_Message);
	}
}
/*****************************END OF FILE**************************************/