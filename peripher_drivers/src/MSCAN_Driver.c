/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: MSCAN_Driver.c
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

/* Includes ------------------------------------------------------------------*/

#include "arm_cm0.h"
#include "MSCAN_Driver.h"



/**
 * @brief   Configure the filters about the MSCAN module when receiving CAN frames.
 * &attention  The CAN module is configured to two 32-bit acceptance filters in default.
 * @param   filter_config: The buffer which point to MSCAN module acceptance parameters when receiving CAN frames.
 * @returns 0: Calling succeeded.
 * 			-1: Calling failed.
 */
static int32_t MSCAN_ConfigIDFilter(MSCAN_FilterConfigure* filter_config)
{
	if (filter_config == NULL)return -1;
	
	/* Judge whether CAN module filter is enabled or disabled */
	if (filter_config->Filter_Enable == 0)
	{
		MSCAN->CANIDMR_BANK_1[0] = 0xFFu;
		MSCAN->CANIDMR_BANK_1[1] = 0xFFu;
		MSCAN->CANIDMR_BANK_1[2] = 0xFFu;
		MSCAN->CANIDMR_BANK_1[3] = 0xFFu;
		MSCAN->CANIDMR_BANK_2[0] = 0xFFu;
		MSCAN->CANIDMR_BANK_2[1] = 0xFFu;
		MSCAN->CANIDMR_BANK_2[2] = 0xFFu;
		MSCAN->CANIDMR_BANK_2[3] = 0xFFu;			/* If CAN filter is disabled,set CAN identifier mask register */
		
		return 0;
	}
	else
	{
		MSCAN->CANIDAC &= ~MSCAN_CANIDAC_IDAM_MASK;	/* Configure CAN filters as Two 32-bit acceptance filters */
	}
	
	if (filter_config->Filter_Channel == 0)			/* Select group 0 filters. */
	{
		/* First,checking the received frame ID format. */ 
		if (filter_config->id_format == OnlyAcceptStandardID)
		{
			MSCAN->CANIDMR_BANK_1[0] = (uint8_t)(filter_config->id_mask >> 3);
			MSCAN->CANIDAR_BANK_1[0] = (uint8_t)(filter_config->id_accept >> 3);

			MSCAN->CANIDMR_BANK_1[1] = ((uint8_t)(filter_config->id_mask)) << 5;
			MSCAN->CANIDAR_BANK_1[1] = (uint8_t)(filter_config->id_accept) << 5;
		}
		else if (filter_config->id_format == OnlyAcceptExtendedID)
		{
			MSCAN->CANIDMR_BANK_1[0] = (uint8_t)(filter_config->id_mask >> 21);
			MSCAN->CANIDAR_BANK_1[0] = (uint8_t)(filter_config->id_accept >> 21);

			MSCAN->CANIDMR_BANK_1[1] = ((uint8_t)(filter_config->id_mask >> 18)) << 5
					                  | ((uint8_t)(filter_config->id_mask >> 15) & 0x07u);

			MSCAN->CANIDAR_BANK_1[1] = ((uint8_t)(filter_config->id_accept >> 18)) << 5
					                  | ((uint8_t)(filter_config->id_accept >> 15) & 0x07u);

			MSCAN->CANIDAR_BANK_1[1] |= ((1 << 4) | (1 << 3));					/* Extended id format */ 

			MSCAN->CANIDMR_BANK_1[2] = (uint8_t)(filter_config->id_mask >> 7);
			MSCAN->CANIDAR_BANK_1[2] = (uint8_t)(filter_config->id_accept >> 7);

			MSCAN->CANIDMR_BANK_1[3] = ((uint8_t)(filter_config->id_mask & 0x7Fu) << 1);
			MSCAN->CANIDAR_BANK_1[3] = (uint8_t)(filter_config->id_accept & 0x7Fu) << 1;
		}
		else
		{
			return -1;								/* The frame id format is wrong,return error. */
		}
		
		/* Second,checking the received frame type. */
		if (filter_config->frame_type == OnlyAcceptRemoteFrame)
		{
			if (filter_config->id_format == OnlyAcceptStandardID) 
			{
				MSCAN->CANIDAR_BANK_1[1] |= (1 << 4);
			}
			else if (filter_config->id_format == OnlyAcceptExtendedID) 
			{
				MSCAN->CANIDAR_BANK_1[3] |= (1 << 0);
			} 
			else 
			{
				return -1;							/* The frame id type is wrong,return error. */
			}
		}
		else if (filter_config->frame_type == OnlyAcceptDataFrame)
		{
			if (filter_config->id_format == OnlyAcceptStandardID) 
			{
				MSCAN->CANIDAR_BANK_1[1] &= ~(1 << 4);
			} 
			else if (filter_config->id_format == OnlyAcceptExtendedID) 
			{
				MSCAN->CANIDAR_BANK_1[3] &= ~(1 << 0);
			} 
			else 
			{
				return -1;
			}			
		}
		else if (filter_config->frame_type == AcceptBothFrame)
		{
			if (filter_config->id_format == OnlyAcceptStandardID)
			{
				MSCAN->CANIDMR_BANK_1[1] |= (1 << 4);
			}
			else if (filter_config->id_format == OnlyAcceptExtendedID)
			{
				MSCAN->CANIDMR_BANK_1[3] |= (1 << 0);
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
		
		return 0;
	}
	else if (filter_config->Filter_Channel == 1)				/* Select group 1 filters. */
	{
		if (filter_config->id_format == OnlyAcceptStandardID) /* Check whether the frame id type is standardID */
		{
			MSCAN->CANIDMR_BANK_2[0] = (uint8_t)(filter_config->id_mask >> 3);
			MSCAN->CANIDAR_BANK_2[0] = (uint8_t)(filter_config->id_accept >> 3);

			MSCAN->CANIDMR_BANK_2[1] = ((uint8_t)(filter_config->id_mask)) << 5;
			MSCAN->CANIDAR_BANK_2[1] = (uint8_t)(filter_config->id_accept) << 5;
		} 
		else if (filter_config->id_format  == OnlyAcceptExtendedID) /* Check whether the frame id type is ExtendedID */
		{
			MSCAN->CANIDMR_BANK_2[0] = (uint8_t)(filter_config->id_mask >> 21);
			MSCAN->CANIDAR_BANK_2[0] = (uint8_t)(filter_config->id_accept >> 21);

			MSCAN->CANIDMR_BANK_2[1] = ((uint8_t)(filter_config->id_mask >> 18)) << 5
					                 | (((uint8_t)(filter_config->id_mask >> 15)) & 0x07);

			MSCAN->CANIDAR_BANK_2[1] = ((uint8_t)(filter_config->id_accept >> 18)) << 5
					                 | (((uint8_t)(filter_config->id_accept >> 15)) & 0x07);
			
			MSCAN->CANIDAR_BANK_2[1] |= ((1 << 4) | (1 << 3));					/* Extended id format */ 
			

			MSCAN->CANIDMR_BANK_2[2] = (uint8_t)(filter_config->id_mask >> 7);
			MSCAN->CANIDAR_BANK_2[2] = (uint8_t)(filter_config->id_accept >> 7);

			MSCAN->CANIDMR_BANK_2[3] = ((uint8_t)(filter_config->id_mask & 0x7F) << 1);
			MSCAN->CANIDAR_BANK_2[3] = (uint8_t)(filter_config->id_accept & 0x7F) << 1;
		} 
		else 
		{
			return -1; /* The frame id type is wrong,return error */
		}

		if (filter_config->frame_type == OnlyAcceptRemoteFrame) /* Check whether received frame type is a remote frame */
		{
			if (filter_config->id_format == OnlyAcceptStandardID) 
			{
				MSCAN->CANIDAR_BANK_2[1] |= 0x10u;
			} 
			else if (filter_config->id_format  == OnlyAcceptExtendedID) 
			{
				MSCAN->CANIDAR_BANK_2[3] |= 0x01u;
			} 
			else 
			{
				return -1;
			}
		} 
		else if (filter_config->frame_type == OnlyAcceptDataFrame) /* Check whether received frame type is a data frame */
		{
			if (filter_config->id_format == OnlyAcceptStandardID) 
			{
				MSCAN->CANIDAR_BANK_2[1] &= ~0x10u;
			} 
			else if (filter_config->id_format == OnlyAcceptExtendedID) 
			{
				MSCAN->CANIDAR_BANK_2[3] &= 0xFEu;
			} 
			else 
			{
				return -1;
			}
		} 
		else if (filter_config->frame_type == AcceptBothFrame) 
		{
			if (filter_config->id_format == OnlyAcceptStandardID)
			{
				MSCAN->CANIDMR_BANK_2[1] |= (1 << 4);
			}
			else if (filter_config->id_format == OnlyAcceptExtendedID)
			{
				MSCAN->CANIDMR_BANK_2[3] |= (1 << 0);
			}
			else
			{
				return -1;
			}
		} 
		else 
		{
			return -1; /* The frame type is wrong,return error */
		}

		return 0; /* When all of above is setting OK,return succeeded. */
	}		

	return -1;
}



/**
 * @brief   Initialize the MSCAN module with a given parameters by users.
 * @param   CAN_Config: The pointer which point to the MSCAN module initialization features.
 * 			Filter_Config: CAN module filters parameters.
 * @returns 0: Calling succeeded.
 * 			-1: Calling failed.
 */
int32_t MSCAN_Init(MSCAN_ParametersConfig* CAN_Config, MSCAN_FilterConfigure* Filter_Config)
{
	uint16_t Timeout_Count = 0;
	
	if (CAN_Config == NULL)return -1;

	if (Filter_Config == NULL)return -1;

	/* Enable MSCAN module clock */
	SIM->SCGC |= SIM_SCGC_MSCAN_MASK;
	
	if (CAN_Config->MSCAN_SignalPinsRemap == 0)
	{
		/* Selects the MSCAN pin,CAN_TX on PTC7, CAN_RX on PTC6 */
		SIM->PINSEL1 &= ~SIM_PINSEL1_MSCANPS_MASK;		
	}
	else
	{
		/* Selects the MSCAN pin,CAN_TX on PTE7, CAN_RX on PTH2 */
		SIM->PINSEL1 |= SIM_PINSEL1_MSCANPS_MASK;		
	}

	/* MSCAN module is enabled */
	MSCAN->CANCTL1 |= MSCAN_CANCTL1_CANE_MASK;

	/* Disable MSCAN wake-up function. */
	if (CAN_Config->MSCAN_WakeUpEnable == 0)
	{
		MSCAN->CANCTL0 &= ~MSCAN_CANCTL0_WUPE_MASK;
	}
	else
	{
		/* Enable MSCAN wake-up function,this bit must be configured before sleep mode entry. */
		MSCAN->CANCTL0 |= MSCAN_CANCTL0_WUPE_MASK;
	}
	
	Timeout_Count = 0;
	
	/* Second,CPU request MSCAN module enter INIT mode by writing 
	   a 1 to the INITRQ bit in CAN0CTL0 register. */
	MSCAN->CANCTL0 |= MSCAN_CANCTL0_INITRQ_MASK;

	/* Wait until initialization mode active */
	while (!(MSCAN->CANCTL1 & MSCAN_CANCTL1_INITAK_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			return -1;
		}		
	}
	
	Timeout_Count = 0;
	
	if (CAN_Config->MSCAN_ClockSource == 0)
	{
		/* MSCAN clock source is oscillator clock(8MHz) */
		MSCAN->CANCTL1 &= ~MSCAN_CANCTL1_CLKSRC_MASK;
	}
	else
	{
		/* MSCAN clock source is system BUS clock(20MHz) */
		MSCAN->CANCTL1 |= MSCAN_CANCTL1_CLKSRC_MASK;
	}
	
	if (CAN_Config->MSCAN_LoopbackMode == 0)
	{
		/* Loopback self test disabled. */
		MSCAN->CANBTR1 &= ~MSCAN_CANCTL1_LOOPB_MASK;
	}
	else
	{
		/* Loopback self test enabled. */
		MSCAN->CANBTR1 |= MSCAN_CANCTL1_LOOPB_MASK;
	}
	
	if (CAN_Config->MSCAN_ListenOnlyMode == 0)
	{
		/* Normal operation */
		MSCAN->CANCTL1 &= ~MSCAN_CANCTL1_LISTEN_MASK;
	}
	else
	{
		/* Listen only mode */
		MSCAN->CANCTL1 &= ~MSCAN_CANCTL1_LISTEN_MASK;
	}
	
	if (CAN_Config->MSCAN_BusoffRecoveryMode == 0)
	{
		/* Automatic bus-off recovery (see Bosch CAN 2.0A/B protocol specification). */
		MSCAN->CANCTL1 &= ~MSCAN_CANCTL1_BORM_MASK;
	}
	else
	{
		/* Bus-off recovery upon user request. */
		MSCAN->CANCTL1 |= MSCAN_CANCTL1_BORM_MASK;
	}
	
	if (CAN_Config->MSCAN_WakeUpMode == 0)
	{
		/* MSCAN wakes on any dominant level on the CAN bus. */
		MSCAN->CANCTL1 &= ~MSCAN_CANCTL1_WUPM_MASK;
	}
	else
	{
		/* MSCAN wakes only in case of a dominant pulse on the CAN bus that has a length of Twup. */
		MSCAN->CANCTL1 |= MSCAN_CANCTL1_WUPM_MASK;
	}
	
	/* Initialize the baud rate of MSCAN module.
	   Attention: The following setting MSCAN module baud rate program is only apply to
				  the CANCLK source frequency is 20MHz(BUS clock).If the CANCLK frequency is changed,
				  user should modify the corresponding configure registers. 
	*/
	/* CAN baudrate = F(clk) / ((3 + TSEG1 + TSEG2)*(BRP + 1)) */ 
	switch (CAN_Config->baudrate) 
	{
		case MSCAN_Baudrate_50K: 
		{
			MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x27);

			MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x03)  
			              | MSCAN_CANBTR1_TSEG1(0x04);
		}
		break;

		case MSCAN_Baudrate_100K: 
		{
			MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x13);

			MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x03)
			  	          | MSCAN_CANBTR1_TSEG1(0x04);
		}
		break;

		case MSCAN_Baudrate_125K: 
		{
			MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x0F);

			MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x03) 
				          | MSCAN_CANBTR1_TSEG1(0x04);
		}
		break;

		case MSCAN_Baudrate_250K:  
		{
				MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x07);

				MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x03)
				    	  | MSCAN_CANBTR1_TSEG1(0x04);
//				MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x05);

//				MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x06)
//				    	  | MSCAN_CANBTR1_TSEG1(0x07);
		}
		break;

		case MSCAN_Baudrate_500K: 
		{
			MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x03);

			MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x03) 
				          | MSCAN_CANBTR1_TSEG1(0x04);
		}
		break;

		case MSCAN_Baudrate_1M: 
		{
			MSCAN->CANBTR0 = MSCAN_CANBTR0_SJW(0x01) | MSCAN_CANBTR0_BRP(0x01);

			MSCAN->CANBTR1 = MSCAN_CANBTR1_SAMP_MASK | MSCAN_CANBTR1_TSEG2(0x03) 
				          | MSCAN_CANBTR1_TSEG1(0x04);
		}
		break;

		default: break;
	}

	/* Configure the ID filters for receiving frame. */
	if (MSCAN_ConfigIDFilter(Filter_Config) != 0)return -1;
	
	/* Finally, CPU request MSCAN module to exit INIT mode and enter normal mode. */
	MSCAN->CANCTL0 &= ~MSCAN_CANCTL0_INITRQ_MASK;

	/* Wait until normal mode active */
	while (MSCAN->CANCTL1 & MSCAN_CANCTL1_INITAK_MASK)
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			return -1;
		}		
	}
	
	if (CAN_Config->MSCAN_StopInWaitMode == 0)
	{
		/* The module is not affected during wait mode. */
		MSCAN->CANCTL0 &= ~MSCAN_CANCTL0_CSWAI_MASK;
	}
	else
	{
		/* The module ceases to be clocked during wait mode. */
		MSCAN->CANCTL0 |= MSCAN_CANCTL0_CSWAI_MASK;
	}
	
	if (CAN_Config->MSCAN_TimeStampEnable == 0)
	{
		/* Disable internal MSCAN timer. */
		MSCAN->CANCTL0 &= ~MSCAN_CANCTL0_TIME_MASK;
	}
	else
	{
		/* Enable internal MSCAN timer. */
		MSCAN->CANCTL0 |= MSCAN_CANCTL0_TIME_MASK;
	}
	
	if (CAN_Config->MSCAN_WakeUpINTEnable == 0)
	{
		/* No interrupt request is generated from this event. */
		MSCAN->CANRIER &= ~MSCAN_CANRIER_WUPIE_MASK;
	}
	else
	{
		/* A wake-up event causes a Wake-Up interrupt request. */
		MSCAN->CANRIER |= MSCAN_CANRIER_WUPIE_MASK;
	}
	
	if (CAN_Config->MSCAN_OverrunINTEnable == 0)
	{
		/* No interrupt request is generated from this event. */
		MSCAN->CANRIER &= ~MSCAN_CANRIER_OVRIE_MASK;
	}
	else
	{
		/* An overrun event causes an error interrupt request. */
		MSCAN->CANRIER |= MSCAN_CANRIER_OVRIE_MASK;
	}
	
	if (CAN_Config->MSCAN_ReceiveFullINTEnable == 0)
	{
		/* No interrupt request is generated from this event. */
		MSCAN->CANRIER &= ~MSCAN_CANRIER_RXFIE_MASK;
	}
	else
	{
		/* A receive buffer full (successful message reception) event causes a receiver interrupt request. */
		MSCAN->CANRIER |= MSCAN_CANRIER_RXFIE_MASK;
	}
	
	if (CAN_Config->MSCAN_TransEmptyINTEnable == 0)
	{
		MSCAN->CANTIER &= ~MSCAN_CANTIER_TXEIE_MASK;
	}
	else
	{
		/* A transmitter empty (transmit buffer available for transmission) event causes a 
		   transmitter empty interrupt request. */
		MSCAN->CANTIER |= CAN_Config->MSCAN_TransEmptyINTEnable;
	}

	MSCAN->CANCTL1 |= MSCAN_CANCTL1_CANE_MASK;

	return 0;
}



/**
 * @brief   MSCAN send a message by MSCAN module.
 * @param   *W_Framebuff: Data buffer that storing one CAN message which will be sent.
 * @returns 0: Calling succeeded.
 * 			-1: Calling failed.
 */
int32_t MSCAN_SendFrame(MSCAN_Message_TypeDef* W_Framebuff)
{
	uint8_t reg_val, i;

	uint8_t temp1, temp2, temp3;
	
	if (W_Framebuff != NULL)
	{
		/* Ensure the length of the data not to be greater than 8 bytes. */
		if (W_Framebuff->data_length > 8) 
		{
			return -1;
		}

		/* Ensure there is at least one Empty Transmission Buffer. */
		if ((reg_val = (MSCAN->CANTFLG & MSCAN_CANTFLG_TXE_MASK)) == 0) 
		{
			return -1;
		}

		/*
		 * According to the KEAZ128RM, select the Empty Transmission Buffer
		 * by passing the value of CANx->CANTFLG to CANx->CANTBSEL. NOTICE,
		 * after doing this, the value of CANx->CANTBSEL will be the index
		 * of the Empty Transmission Buffer been selected.
		 */
		MSCAN->CANTBSEL = reg_val;

		if ((W_Framebuff->frame_type == RemoteFrameWithStandardId)
		 || (W_Framebuff->frame_type == DataFrameWithStandardId)) 
		{
			/* indicate the frame is standard format */
			MSCAN->TSIDR1 &= ~MSCAN_TSIDR1_TSIDE_MASK;

			/* If caller wants to send a packet with standard id, firstly
			 * ensure the id not to be exceed 11 bits.
			 */
			if (W_Framebuff->frame_id > 0x7FFu) 
			{
				return -1;
			}

			/* Load the id into the respective registers. */
			MSCAN->TSIDR0 = (uint8_t)(W_Framebuff->frame_id >> 3);
			MSCAN->TSIDR1 = (uint8_t)(W_Framebuff->frame_id << 5);

			if (W_Framebuff->frame_type == RemoteFrameWithStandardId) 
			{
				/*
				 * If caller wants to send a remote packet, set the
				 * respective bit in CANx->SIDR1, and set the TDLR
				 * to be 0 (the remote packet has no data).
				 */
				MSCAN->TSIDR1 |= MSCAN_TSIDR1_TSRTR_MASK;

				/* The data length should be 0 */
				MSCAN->TDLR = 0;
			} 
			else 
			{
				MSCAN->TSIDR1 &= ~MSCAN_TSIDR1_TSRTR_MASK;
				/*
				 * If caller wants to send a data packet, set the
				 * TDLR to be equal to the actual data length
				 */
				MSCAN->TDLR = W_Framebuff->data_length;

				/* Load the actual data to the Transmit Extended Data Segment Register */
				for (i = 0; i < W_Framebuff->data_length; i++) 
				{
					MSCAN->TEDSR[i] = W_Framebuff->data[i];
				}
			}
		} 
		else if ((W_Framebuff->frame_type == RemoteFrameWithExtendedId)
			  || (W_Framebuff->frame_type == DataFrameWithExtendedId)) 
		{
			/* When caller wants to send a frame with ExtendedID,
			 * the TEIDE bit of MSCAN_TEIDR1 register should be set to 1 */
			MSCAN->TEIDR1 = MSCAN_TEIDR1_TEIDE_MASK;

			/* If caller wants to send a packet with standard id, firstly
			 * ensure the id not to be exceed 29 bits.
			 */
			if (W_Framebuff->frame_id > 0x1FFFFFFF) 
			{
				return -1;
			}

			MSCAN->TEIDR0 = (uint8_t)((W_Framebuff->frame_id) >> 21);

			temp1 = (uint8_t)((W_Framebuff->frame_id) >> 18) & 0x07;
			temp2 = (uint8_t)((W_Framebuff->frame_id) >> 15) & 0x07;
			/* Attention:The TSRR bit is used only in extended format.
			 * It must be set to 1 by the user for transmission buffers
			 */
			MSCAN->TEIDR1 |= (temp1 << 5) | temp2 | MSCAN_TEIDR1_TSRR_MASK;

			MSCAN->TEIDR2 = (uint8_t)((W_Framebuff->frame_id) >> 7);

			temp3 = (uint8_t)W_Framebuff->frame_id & 0x7Fu;
			MSCAN->TEIDR3 = MSCAN_TEIDR3_TEID6_TEID0(temp3);

			if (W_Framebuff->frame_type == RemoteFrameWithExtendedId) 
			{
				MSCAN->TEIDR3 |= MSCAN_TEIDR3_TERTR_MASK;

				MSCAN->TDLR = 0;
			} 
			else 
			{
				MSCAN->TEIDR3 &= ~MSCAN_TEIDR3_TERTR_MASK;

				MSCAN->TDLR = W_Framebuff->data_length;

				for (i = 0; i < W_Framebuff->data_length; i++) 
				{
					MSCAN->TEDSR[i] = W_Framebuff->data[i];
				}
			}
		} 
		else 
		{
			return -1;
		}

		/*
		 * The purpose is to clear the respective bit,
		 * according to the KEAZ128RM, it should be done
		 * by writing 1 but not 0. 
		 */
		MSCAN->CANTFLG = MSCAN->CANTBSEL;

		return 0;
	}
	
	return -1;
}



/**
 * @brief   MSCAN receive a message.
 * @param   *R_Framebuff: Data buffer that storing the received CAN message
 * @returns 0: Calling succeeded.
 * 			-1: Calling failed.
 */
int32_t MSCAN_ReceiveFrame(MSCAN_Message_TypeDef* R_Framebuff)
{
	uint8_t i;
	uint8_t temp1, temp2, temp3, temp4;
	uint8_t StandardOrExtendedID;
	uint8_t DataOrRemoteFrame;
	uint32_t reg_val1, reg_val2, reg_val3, reg_val4;
	
	/* Judge whether the receiver FIFO is not empty */
	if (MSCAN->CANRFLG & MSCAN_CANRFLG_RXF_MASK) 
	{
		/* Read corresponding register and get corresponding bit,
		 * then judge the received frame type.
		 */
		/* The temp1 variable indicate the received frame is standard or extended format */
		temp1 = (MSCAN->REIDR1 & MSCAN_REIDR1_REIDE_MASK) >> 3;
		/* The temp2 variable indicate the received frame is data frame or remote frame */
		temp2 = MSCAN->REIDR3 & MSCAN_REIDR3_RERTR_MASK;
		/* The temp3 variable indicate the received frame is data frame or remote frame */
		temp3 = (MSCAN->RSIDR1 & MSCAN_RSIDR1_RSRTR_MASK) >> 4;
		/* The temp4 variable indicate the received frame is standard or extended format */
		temp4 = (MSCAN->RSIDR1 & MSCAN_RSIDR1_RSIDE_MASK) >> 3;

		/* Attention:the corresponding bits should be AND to judge the frame type */
		StandardOrExtendedID = temp1 | temp4;

		if (StandardOrExtendedID != 0) 
		{
			DataOrRemoteFrame = temp2 & temp3;
		} 
		else 
		{
			DataOrRemoteFrame = temp2 | temp3;
		}

		/* When StandardOrExtendedID is set,it means the received frame is with extended ID,
		 * or it means the received frame is with standard ID */
		if (StandardOrExtendedID != 0) 
		{
			/* When DataOrRemoteFrame is set,it means the received frame is a remote frame,
			 * or it means the received frame is a data frame */
			if (DataOrRemoteFrame != 0) 
			{
				R_Framebuff->frame_type = RemoteFrameWithExtendedId;
			} 
			else 
			{
				R_Framebuff->frame_type = DataFrameWithExtendedId;
			}
		} 
		else 
		{
			if (DataOrRemoteFrame != 0) 
			{
				R_Framebuff->frame_type = RemoteFrameWithStandardId;
			} 
			else 
			{
				R_Framebuff->frame_type = DataFrameWithStandardId;
			}
		}

		/* Get the received frame ID and write to FrameBuff->id */
		if ((R_Framebuff->frame_type == RemoteFrameWithStandardId)
				|| (R_Framebuff->frame_type == DataFrameWithStandardId)) 
		{
			reg_val1 = (uint32_t)(MSCAN->RSIDR0) << 3;
			reg_val2 = (uint32_t)(MSCAN->RSIDR1 >> 5);
			R_Framebuff->frame_id = reg_val1 | reg_val2;
		} 
		else 
		{
			reg_val1 = (uint32_t)(MSCAN->REIDR0) << 21;
			reg_val2 = (uint32_t)(MSCAN->REIDR1 >> 5) << 18;
			reg_val3 = (uint32_t)(MSCAN->REIDR1 & 0x07) << 15;
			reg_val4 = (uint32_t)(MSCAN->REIDR2) << 7;
			R_Framebuff->frame_id = (reg_val1 | reg_val2 | reg_val3 | reg_val4
					| (MSCAN->REIDR3 >> 1));
		}

		if ((R_Framebuff->frame_type == RemoteFrameWithStandardId)
		 || (R_Framebuff->frame_type == RemoteFrameWithExtendedId)) 
		{
			R_Framebuff->data_length = 0;
		} 
		else 
		{
			/* Read BDLR register and write to FrameBuff->length.
			 * Attention:When read RDLR register,it should AND with 0x0F.
			 */
			R_Framebuff->data_length = MSCAN->RDLR & 0x0Fu;

			for (i = 0; i < R_Framebuff->data_length; i++) 
			{
				R_Framebuff->data[i] = MSCAN->REDSR[i];
			}
		}

		/* Clear the RXF bits by writing 1 to the corresponding bits */
		MSCAN->CANRFLG |= MSCAN_CANRFLG_RXF_MASK;

		return 0;
	}
	
	return -1;
}



/**
 * @brief   Checking the specified MSCAN module whether have enough
 *          hard transmission buffer to send CAN messages.
 * @param   None.
 * @returns 0: Calling succeeded.User can load CAN messages to send.
 * 			-1: Calling failed.User can not load CAN messages to send.
 */
int32_t MSCAN_HardTxBufferCheck(void)
{
	uint8_t ret_val;
	
	ret_val = MSCAN->CANTFLG & MSCAN_CANTFLG_TXE_MASK;
	
	if (ret_val == 0)return -1;  /* All the TX buffer is full,that means user can not load CAN messages to send. */
	else 
		return 0;  /* If CANTFLG register value is not zero,that means there is at least one empty TX buffer,user 
	                  can load CAN messages to send. */
}

/*****************************END OF FILE**************************************/