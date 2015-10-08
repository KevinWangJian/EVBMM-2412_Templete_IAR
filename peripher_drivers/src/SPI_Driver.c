/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: SPI_Driver.c
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

/* Includes ------------------------------------------------------------------*/

#include "SPI_Driver.h"



/**
  * @brief      Initialize SPI ports.
  * @param      SPIx, The specified SPI port.
  *             *p_Config, Data buffer which store SPI configure parameters.
  *             speed_mode, SPI transfer speed mode. 
  * @Attention  Users should call this function first before others.
  * @returns    0 means success, -1 means failure.
  */
int32_t SPI_Init(SPI_Type* SPIx, SPIx_ConfigType *p_Config, SPIxSpeed_Typedef speed_mode)
{
	if ((SPIx != SPI0) && (SPIx != SPI1))return -1;
	
	if (p_Config == NULL)return -1;
	
	if ((speed_mode < High_Speed) || (speed_mode > Low_Speed))return -1;
	
	if (SPIx == SPI0)
	{
		if (p_Config->Signal_Pins == SPI0_PortBPins)
		{
			/* SPI0_SCK, SPI0_MOSI, SPI0_MISO, and SPI0_PCS are mapped on PTB2, PTB3, PTB4, and PTB5. */
			SIM->PINSEL &= ~SIM_PINSEL_SPI0PS_MASK;
		}
		else if (p_Config->Signal_Pins == SPI0_PortEPins)
		{
			/* SPI0_SCK, SPI0_MOSI, SPI0_MISO, and SPI0_PCS are mapped on PTE0, PTE1, PTE2, and PTE3. */
			SIM->PINSEL |= SIM_PINSEL_SPI0PS_MASK;
		}
		else 
		{
			return -1;
		}
		
		/* Bus clock to the SPI0 module is enabled. */
		SIM->SCGC |= SIM_SCGC_SPI0_MASK;
	}
	else
	{
		if (p_Config->Signal_Pins == SPI1_PortDPins)
		{
			/* SPI1_SCK, SPI1_MOSI, SPI1_MISO, and SPI1_PCS are mapped on PTD0, PTD1, PTD2, and PTD3. */
			SIM->PINSEL1 &= ~SIM_PINSEL1_SPI1PS_MASK;
		}
		else if (p_Config->Signal_Pins == SPI1_PortGPins)
		{
			/* SPI1_SCK, SPI1_MOSI, SPI1_MISO, and SPI1_PCS are mapped on PTG4, PTG5, PTG6, and PTG7. */
			SIM->PINSEL1 |= SIM_PINSEL1_SPI1PS_MASK;
		}
		else
		{
			return -1;
		}
		
		/* Bus clock to the SPI1 module is enabled. */
		SIM->SCGC |= SIM_SCGC_SPI1_MASK;
	}
	
	/* Enable SPI module */
	SPIx->C1 |= SPI_C1_SPE_MASK;
	
	SPIx->C2 &= ~(SPI_C2_MODFEN_MASK | SPI_C2_SPC0_MASK);
	SPIx->C1 |= SPI_C1_SSOE_MASK;
	
	/* SPI clocks stop when the MCU enters Wait mode. */
	SPIx->C2 |= SPI_C2_SPISWAI_MASK;
	
	/* Judge whether available data in receive buffer interrupt or mode fault error interrupt is enabled. */
	if (p_Config->SPRF_MODF_INT_Enable == 0)
	{
		/* Disabled */
		SPIx->C1 &= ~SPI_C1_SPIE_MASK;	
	}
	else
	{
		/* Enabled */
		SPIx->C1 |= SPI_C1_SPIE_MASK;
	}
	
	/* Judge whether SPI transmit interrupt is enabled. */
	if (p_Config->Transmit_INT_Enable == 0)
	{
		SPIx->C1 &= ~SPI_C1_SPTIE_MASK;
	}
	else
	{
		SPIx->C1 |= SPI_C1_SPTIE_MASK;
	}
	
	/* Judge whether SPI Match Interrupt is enabled. */
	if (p_Config->Match_INT_Enable == 0)
	{
		SPIx->C2 &= ~SPI_C2_SPMIE_MASK;
	}
	else
	{
		SPIx->C2 |= SPI_C2_SPMIE_MASK;
	}
	
	
	if (p_Config->MasterOrSlaveMode == 0)
	{
		/* SPI module configured as a slave SPI device. */
		SPIx->C1 &= ~SPI_C1_MSTR_MASK;
	}
	else
	{
		/* SPI module configured as a master SPI device */
		SPIx->C1 |= SPI_C1_MSTR_MASK;
	}
	
	
	if (p_Config->CPOL == 0)
	{
		/* Active-high SPI clock (idles low) */
		SPIx->C1 &= ~SPI_C1_CPOL_MASK;
	}
	else
	{
		/* Active-low SPI clock (idles high) */
		SPIx->C1 |= SPI_C1_CPOL_MASK;
	}
	
	
	if (p_Config->CPHA == 0)
	{
		/* First edge on SPSCK occurs at the middle of the first cycle of a data transfer. */
		SPIx->C1 &= ~SPI_C1_CPHA_MASK;
	}
	else
	{
		/* First edge on SPSCK occurs at the start of the first cycle of a data transfer. */
		SPIx->C1 |= SPI_C1_CPHA_MASK;
	}
	
	if (p_Config->LSBFE == 0)
	{
		/* SPI serial data transfers start with the most significant bit. */
		SPIx->C1 &= ~SPI_C1_LSBFE_MASK;
	}
	else
	{
		/* SPI serial data transfers start with the least significant bit. */
		SPIx->C1 |= SPI_C1_LSBFE_MASK;
	}
	
	
	/* Configure the SPI transfer baud rate.
	   BaudRateDivisor = (SPPR + 1)*2^(SPR + 1)
	   Baud Rate = BusClock / BaudRateDivisor
	*/
	if (speed_mode == High_Speed)
	{
//		SPIx->BR = 0x41u;						/* baudrate = 1Mbps(BUS_CLK = 20MHz) */
		SPIx->BR = 0x51u;						/* baudrate = 1Mbps(BUS_CLK = 24MHz) */
	}
	else
	{
//		SPIx->BR = 0x43u;						/* baudrate = 250Kbps(BUS_CLK = 20MHz) */
		SPIx->BR = 0x53u;						/* baudrate = 250Kbps(BUS_CLK = 24MHz) */
	}
	
	return 0;
}



/**
  * @brief   Write one byte data by SPI port.
  * @param   SPIx, The specified SPI port.
  *          Tx_data, The data which will be written. 
  * @returns 0 means success, -1 means failure.
  */
int32_t SPI_WriteByteData(SPI_Type* SPIx, uint8_t Tx_data)
{	
    uint32_t Timeout_Count = 0;
	
	if ((SPIx != SPI0) && (SPIx != SPI1))return -1;
	
	while (!(SPIx->S & SPI_S_SPTEF_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			/* Time out,return error */
			return -1;
		}
	}
	
	Timeout_Count = 0;
	
	(void)(SPIx->S);
	
	SPIx->D = Tx_data;
	
	while (!(SPIx->S & SPI_S_SPTEF_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			/* Time out,return error */
			return -1;
		}
	}
	
	/* Dummy read one time */
	(void)(SPIx->D);
	
	return 0;
}



/**
  * @brief   Read one byte data by SPI port.
  * @param   SPIx, The specified SPI port.
  *          dummy_data, The data which will be sent.Because it's a dummy written data,
  *          user can send any data to generate 8 SCK signals.
  *          *Rx_data, Data buffer which store the read data.
  * @returns 0 means success, -1 means failure.
  */
int32_t SPI_ReadByteData(SPI_Type* SPIx, uint8_t dummy_data, uint8_t* Rx_data)
{
	uint32_t Timeout_Count = 0;
	
	if ((SPIx != SPI0) && (SPIx != SPI1))return -1;
	
	if (Rx_data == NULL)return -1;
	
	while (!(SPIx->S & SPI_S_SPTEF_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			/* Time out,return error */
			return -1;
		}
	}
	
	Timeout_Count = 0;
	
	(void)(SPIx->S);
	
	SPIx->D = dummy_data;
	
	while (!(SPIx->S & SPI_S_SPTEF_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			/* Time out,return error */
			return -1;
		}
	}
	
	/* Load the received data to received buffer */
	*Rx_data = SPIx->D;
	
	return 0;
}



/**
  * @brief   Write and read one byte data by SPI port.
  * @param   SPIx, The specified SPI port.
  *          Tx_data, The data which will be sent.
  *          *Rx_data, Data buffer which store the received data.
  * @returns 0 means success, -1 means failure.
  */
int32_t SPI_WriteReadByteData(SPI_Type* SPIx, uint8_t Tx_data, uint8_t* Rx_data)
{
	uint32_t Timeout_Count = 0;
	
	if ((SPIx != SPI0) && (SPIx != SPI1))return -1;
	
	if (Rx_data == NULL)return -1;
	
	while (!(SPIx->S & SPI_S_SPTEF_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			/* Time out,return error */
			return -1;
		}
	}
	
	Timeout_Count = 0;
	
	SPIx->D = Tx_data;
	
	while (!(SPIx->S & SPI_S_SPTEF_MASK))
	{
		if (++Timeout_Count == 0xFFFFu)
		{
			/* Time out,return error */
			return -1;
		}
	}
	
	/* Load the received data to received buffer */
	*Rx_data = SPIx->D;
	
	return 0;	
	
}

/*****************************END OF FILE**************************************/