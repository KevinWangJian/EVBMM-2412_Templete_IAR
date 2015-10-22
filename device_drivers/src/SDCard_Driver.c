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

/* Includes ------------------------------------------------------------------*/
#include "SDCard_Driver.h"
#include "SPI_Driver.h"
#include "Systick_Driver.h"


/* Define a static global variable which indicate sd card type */
static uint8_t g_SDCard_Type = SD_TYPE_ERR;



/**
  * @brief   Host send command to SD card with releasing chip select signal CS.
  * @param   cmd£ºSD Command, arg£ºargument, crc£ºCRC value.
  * @Attention  The argument and CRC value is depend on different SD command,user
  *             can get the right value by reading SD datasheet. 
  * @returns 0 means success, -1 means failure.
  */
static uint8_t SDCard_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	uint8_t r1;
	uint16_t retry;
	
	/* Chip select disable */
	SD_CS_HIGH();
	
	(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
	
	/* Chip select enable */
	SD_CS_LOW();
	
	/* Command, argument and crc */
	(void)SPI_WriteByteData(SDCARD_PORT, (cmd | 0x40));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg >> 24));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg >> 16));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg >> 8));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg));
	(void)SPI_WriteByteData(SDCARD_PORT, crc);
	
	/* Wait response, quit until time out */
	for (retry = 0; retry < 800; retry++)
	{
		(void)SPI_ReadByteData(SPI1, DUMMY_BYTE, &r1);
		
		if (r1 != 0xFF)break;
	}
	
	/* Chip select disable */
	SD_CS_HIGH();
	
	/* Generate another 8 clocks to enable SD card finish the transfer and be stable */
	(void)SPI_WriteByteData(SPI1, DUMMY_BYTE);
	
	return (r1);
}



/**
  * @brief   Host send command to SD card with holding chip select signal CS.
  * @param   cmd£ºSD Command, arg£ºargument, crc£ºCRC value.
  * @Attention  The argument and CRC value is depend on different SD command,user
  *             can get the right value by reading SD data sheet. 
  * @returns 0 means success, -1 means failure.
  */
static uint8_t SDCard_SendCommand_NoDeassert(uint8_t cmd, uint32_t arg, uint8_t crc)
{
	uint8_t r1;
	uint16_t retry = 0;
	
	/* Chip select disable */
	SD_CS_HIGH();
	
	(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
	
	/* Chip select enable */
	SD_CS_LOW();
	
	/* Command, argument and crc */
	(void)SPI_WriteByteData(SDCARD_PORT, (cmd | 0x40));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg >> 24));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg >> 16));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg >> 8));
	(void)SPI_WriteByteData(SDCARD_PORT, (uint8_t)(arg));
	(void)SPI_WriteByteData(SDCARD_PORT, crc);
	
	/* Wait response, quit until time out */
	for (retry = 0; retry < 800; retry++)
	{
		(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
		
		if (r1 != 0xFF)break;
	}
	
	return (r1);
}



/**
  * @brief   Host receive data from SD card by SPI reading.
  * @param   *r_buff£ºData buffer which store the received data. 
  *          len£ºReceived data length. 
  *          sta£ºCS(Chip select signal) status after finish receiving data.
  * @returns 0 means success, -1 means failure.
  */
static int SDCard_ReceiveData(uint8_t* r_buff, uint16_t len, CSStatus_TypeDef sta)
{
	uint16_t retry = 0;
	uint8_t r1,dummy_crc;
	int16_t ret;
	
	if (r_buff == NULL)return (SDCARD_RECEIVEDATA_ERROR);
	
	if (len == 0)return (SDCARD_RECEIVEDATA_ERROR);
	
	if ((sta < CS_Release) || (sta > CS_Hold))return (SDCARD_RECEIVEDATA_ERROR);
	
	/* Chip select enable */
	SD_CS_LOW();
	
	/* Wait start-token 0xFE */
	do
	{
		ret = SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
		
		if (ret == -1)return (SDCARD_RECEIVEDATA_ERROR);
		
		retry++;
		
		if (retry > 4000)
		{
			/* Chip select disable */
			SD_CS_HIGH();
			
			return (SDCARD_RECEIVEDATA_ERROR);
		}
	}while (r1 != 0xFEu);
	
	/* Start reading */
	while (len--)
	{
		ret = SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, r_buff);
		
		r_buff++;
		
		if (ret == -1)
		{
			/* Chip select disable */
			SD_CS_HIGH();
			
			return (SDCARD_RECEIVEDATA_ERROR);
		}
	}
	
	/* Read 2 bytes dummy CRC data */
	(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &dummy_crc);
	(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &dummy_crc);
	
	if (sta == CS_Release)
	{
		/* Chip select disable */
		SD_CS_HIGH();
		
		/* Generate another 8 clocks to enable SD card finish the transfer and be stable */
		(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &dummy_crc);
	}
	
	return (SDCARD_RECEIVEDATA_SUCCESS);
}



/**
  * @brief   Initialize SD card communicate signal lines,actually this function is
  *          initializing SPI port.
  * @param   speedmode: SPI communicate speed mode.The value can be High_Speed or Low_Speed.
  * @returns 0 means success, -1 means failure.
  */
static int SDCard_SignalLine_Init(SPIxSpeed_Typedef speedmode)
{
	int ret;
	
	SPIx_ConfigType spi_init;
	
	spi_init.SPRF_MODF_INT_Enable = 0;   					/* Disable SPI interrupt. */
    spi_init.Match_INT_Enable     = 0;                    	/* SPI System Enabled. */
    spi_init.Transmit_INT_Enable  = 0;  					/* SPTEF interrupt disabled. */
    spi_init.MasterOrSlaveMode    = 1;         				/* SPI is in master mode. */
    spi_init.CPOL                 = 1;                      /* Active-low clocks selected. In idle state SCK is high. */
    spi_init.CPHA                 = 1;                      /* Sampling of data occurs at even edges (2,4,6,...) of the SCK clock. */
    spi_init.LSBFE                = 0;                  	/* Data is transferred most significant bit first. */
	spi_init.Signal_Pins          = SPI1_PortGPins;			
	
    ret = SPI_Init(SPI1, &spi_init, speedmode);
    
    SD_CS_OUTPUT();
    SD_CS_HIGH();                               			/* Pull PTG7 pin high and disable SD Card select pin */
    
    return (ret);
}



/**
  * @brief   Initialize SD card after system power on reset.
  * @param   None.
  * @attention This function must be called first if user wants to use other function to 
  *            operate SD card.
  * @returns 0 means success, other value means failure.
  */
int SDCard_Init(void) 
{
    int ret;
    uint8_t r1,i;
	uint32_t retry;
	uint8_t buff[4] = {0};

	/* Configure SPI port and setting low speed for initializing SD Card */
    ret = SDCard_SignalLine_Init(Low_Speed);                
	
	if (ret == -1)return (SDCARD_PORTINIT_ERROR);
    
    /* Power on and delay some times to wait stable */
    Delay1ms(20);           
    	
	for(retry = 0; retry < 0x1000; retry++);
	
	/* Start send 74 clocks at least */
	for(retry = 0; retry < 10; retry++)
	{
		ret = SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
		
		if (ret == -1)return (SDCARD_TRANSMIT_ERROR);
	}	
	
	/* Start send CMD0 until return 0x01 which means SD Card is in IDLE status */
	retry = 0;
	
	do 
	{
        r1 = SDCard_SendCommand(CMD0, 0, 0x95);
        
        retry++;
	}while ((r1 != 0x01) && (retry < 300));
	
	if ((r1 != 0x01) && (retry >= 300))
	{
		return (SDCARD_EXCUTECMD_ERROR);
	}
	
	/* Default SD Card type is error */
	g_SDCard_Type = SD_TYPE_ERR;
	
	/* If returned value is 0x01,it means the sd card is in idle status */
	if (r1 == 0x01)        
	{
		/* Send CMD8 command to get sd card type and version */
		r1 = SDCard_SendCommand_NoDeassert(CMD8, 0x1AA, 0x87);
		
	    if (r1 == 0x01)  					// V2.x Card.Read OCR register,check version
	    {
			/* Read OCR register and judge whether is SD2.0 or SD2.0HC card */
			/* The four bytes of OCR register value should be 0x00,0x00,0x01,0xAA */
			for (i = 0; i < 4; i++)
			{
				ret = SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &buff[i]);
				
				if (ret == -1)
				{
					/* Chip select disable */
					SD_CS_HIGH();
					
					return (SDCARD_TRANSMIT_ERROR);
				}
			}
			
			/* Chip select disable */	
			SD_CS_HIGH();
			
			/* Generate the other 8 clocks to enable SD card finish the transfer */
			ret = SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
			
			if (ret == -1)return (SDCARD_TRANSMIT_ERROR);
			
			/* Judge whether the sd card is support 2.7V~3.6V */
	        if ((buff[2] == 0x01) && (buff[3] == 0xAA)) 						/* If do not judge,it will support more kinds of SD Card. */
	        {
	            retry = 0xFFFE;
	            
	            do 
	            {
	                r1 = SDCard_SendCommand(CMD55, 0, 0x01);					/* should return 0x01 */
					
					if (r1 != 0x01)return (SDCARD_EXCUTECMD_ERROR);
	                
	                r1 = SDCard_SendCommand(ACMD41, 0x40000000, 0x01);			/* should return 0x00 */
	            }while ((r1 != 0) && (retry--));
				
				if ((r1 != 0) && (retry == 0))return (SDCARD_EXCUTECMD_ERROR);
	            
				/* Send CMD58 command to judge SD2.0 Card version */
	            r1 = SDCard_SendCommand_NoDeassert(CMD58, 0, 0);
	            
	            if (r1 != 0)return (SDCARD_EXCUTECMD_ERROR);
	            
				for (i = 0; i < 4; i++)
				{
					ret = SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &buff[i]);
					
					if (ret == -1)
					{
						/* Chip select disable */
						SD_CS_HIGH();
						
						return (SDCARD_TRANSMIT_ERROR);
					}
				}
	            
				/* Chip select disable */
				SD_CS_HIGH();
				/* Generate the other 8 clocks to enable SD card finish the transfer and be stable */
	           	ret = SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
				
				if (ret == -1)return (SDCARD_TRANSMIT_ERROR);
	           	
				/* OCR -> CCS(bit30)  1: SDV2HC	 0: SDV2 */
	           	if (buff[0] & 0x40)
	           	{
	           	    g_SDCard_Type = SD_TYPE_V2HC;
	           	} 
	           	else 
	           	{
	           	    g_SDCard_Type = SD_TYPE_V2;
	           	}
	           	
	           	/* Configure SPI port and setting high speed to communicate with SD Card */
			    ret = SDCard_SignalLine_Init(High_Speed); 
			
			    if (ret == -1)return (SDCARD_PORTINIT_ERROR);
	        }
	    } 
	    else if (r1 == 0x05)                // SD V1.x/MMC V3
	    {
			g_SDCard_Type = SD_TYPE_V1;
			
			/* Chip select disable */
			SD_CS_HIGH();
			
			/* Send another 8 clocks to end of CMD8 and enable SD card stable */
			ret = SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
			
			if (ret == -1)return (SDCARD_TRANSMIT_ERROR);
			
			/* Start initialize SD1.0 or MMC card */
			/* Send CMD55+ACMD41 command,if no response,it is a MMC card,otherwise is a SD1.0 card */
			for (retry = 0;retry < 0xFFF; retry++)
			{
				r1 = SDCard_SendCommand(CMD55, 0, 0);				/* Should return 0x01 */
				
				if (r1 != 0x01)return (SDCARD_INIT_ERROR);
				
				r1 = SDCard_SendCommand(ACMD41, 0, 0);				/* Should return 0x00 */
				
				if (r1 == 0x00)
				{
					retry = 0;
					
					break;
				}
			}
			
			/* MMC Card initialize starting */
			if (retry == 0xFFF)
			{
				for (retry = 0; retry < 0xFFF; retry++)
				{
					r1 = SDCard_SendCommand(CMD1, 0, 0);            /* should be return 0x00 */
					
					if (r1 == 0x00)
					{
						retry = 0;
						
						break;
					}
				}
				
				/* Time out return */
				if (retry == 0xFFF)
				{
					return (SDCARD_INIT_ERROR);
				}
				
				g_SDCard_Type = SD_TYPE_MMC;
			}
			else
			{
				g_SDCard_Type = SD_TYPE_V1;
			}
			
			/* Configure SPI port and setting high speed to write and read data with SD Card */
			ret = SDCard_SignalLine_Init(High_Speed); 
			
			if (ret == -1)return (SDCARD_TRANSMIT_ERROR);
			
			/* CRC disable */
			r1 = SDCard_SendCommand(CMD59, 0, 0x01);            /* Send CMD59 should return 0x00 */
			
			if (r1 != 0x00)return (SDCARD_INIT_ERROR);
			
			/* Set the block size */
			r1 = SDCard_SendCommand(CMD16, 512, 0xFF);
			
			if (r1 != 0x00)return (SDCARD_INIT_ERROR);
	    }
	} 

    return (SDCARD_INIT_SUCCESS);
}



/**
  * @brief   Read data from SD disk.
  * @param   *R_buf: Data buffer which store The received data.
  *          sector: SD card sector address. 
  *          count: sector quantity.
  * @attention User must call this function to read SD card data,
  *            and can transplant FatFs file system by using this function.
  * @returns 0 means success, -1 means failure.
  */
int SDCard_ReadDisk(uint8_t* R_buf, uint32_t sector, uint8_t count)
{
	uint8_t r1,i,dummy_crc;
	int16_t ret;
	
	if (R_buf == NULL)return (SDCARD_READDISK_ERROR);
	
	if (count < 1)return (SDCARD_READDISK_ERROR);
	
	if (g_SDCard_Type != SD_TYPE_V2HC)
	{
		sector <<= 9;					/* Convert to byte address */
	}
	
	if (count == 1)                     /* Read single sector data */
	{
		/* Send CMD17 to read single block data */
		r1 = SDCard_SendCommand(CMD17, sector, 0x01);
		
		if (r1 != 0x00)return (SDCARD_READDISK_ERROR);
		
		ret = SDCard_ReceiveData(R_buf, 512, CS_Release);
		
		if (ret == -1)return (SDCARD_READDISK_ERROR);
		
		/* Send stop data transmit command - CMD12 */
		//r1 = SDCard_SendCommand(CMD12, 0, 0x01);
		
		/* Chip select disable */
		SD_CS_HIGH();
	}
	else								/* Read Multiple sectors data */
	{
		r1 = SDCard_SendCommand(CMD18, sector, 0x01);
		
		if (r1 != 0x00)return (SDCARD_READDISK_ERROR);
		
		/* Start reading multiple block data */
		for (i = 0; i < count; i++)
		{
			ret = SDCard_ReceiveData(R_buf, 512, CS_Hold);
			
			if (ret == -1)
			{
				/* Send stop data transmit command---CMD12 */
				r1 = SDCard_SendCommand(CMD12, 0, 0x01);
				
				/* Chip select disable */
				SD_CS_HIGH();
				
				return (SDCARD_READDISK_ERROR);
			}
			
			/* Increase buffer address offset */
			R_buf += 512;
		}
		
		/* Send stop data transmit command---CMD12 */
		r1 = SDCard_SendCommand(CMD12, 0, 0x01);
		
		/* Chip select disable */
		SD_CS_HIGH();
		
		/* Generate another 8 clocks to enable SD card finish the transfer and be stable */
		(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &dummy_crc);
	}
	
	return (SDCARD_READDISK_SUCCESS);
}



/**
  * @brief   Write data to SD disk.
  * @param   *W_buf: Data buffer which store the written data.
  *          sector: SD card sector address.
  *          count: sector quantity.
  * @attention Users must call this function to write data to SD card, 
  *            and users can transplant FatFs file system by using this function.
  * @returns 0 means success, -1 means failure.
  */
int SDCard_WriteDisk(uint8_t* W_Buf, uint32_t sector, uint8_t count)
{
	uint8_t r1;
	uint32_t i,n,retry;
	int16_t ret;
	
	if (W_Buf == NULL)return (SDCARD_WRITEDISK_ERROR);
	
	if (count < 1)return (SDCARD_WRITEDISK_ERROR);
	
	if (g_SDCard_Type != SD_TYPE_V2HC)
	{
		sector <<= 9;					/* Convert to byte address */
	}
	
	if (count == 1)                     /* Write single sector data */
	{
		/* Send CMD17 to read single block data */
		r1 = SDCard_SendCommand(CMD24, sector, 0x01);
		
		if (r1 != 0x00)return (SDCARD_WRITEDISK_ERROR);
		
		/* SD Card select pin enable, send 3 bytes dummy data for Preparing to write data to SD Card */
		SD_CS_LOW();
		
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);

		/* Start send data write token: 0xFE */
		ret = SPI_WriteByteData(SDCARD_PORT, 0xFEu);

		if (ret == -1)
		{
			/* Chip select disable */
			SD_CS_HIGH();

			return (SDCARD_WRITEDISK_ERROR);
		}
		
		/* Start single block write the data buffer */
		for (i = 0; i < 512; i++)
		{
			ret = SPI_WriteByteData(SDCARD_PORT, *W_Buf++);

			if (ret == -1)
			{
				/* Chip select disable */
				SD_CS_HIGH();

				return (SDCARD_WRITEDISK_ERROR);
			}
		}
		
		/* Send 2 Bytes dummy CRC */
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
		
		/* Receive SD Card response data */
		(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
		
		if ((r1 & 0x1F) != 0x05)
		{
			/* Chip select disable */
			SD_CS_HIGH();
			
			return (SDCARD_WRITEDISK_ERROR);
		}
		
		/* Wait all the data program finished */
		retry = 0;
		
		do
		{
			(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
			
			if (retry++ == 0x40000)
			{
				/* Chip select disable */
				SD_CS_HIGH();
				
				return (SDCARD_WRITEDISK_ERROR);
			}
		}while (r1 == 0x00);
		
		/* Chip select disable */
		SD_CS_HIGH();

		/* Generate another 8 clocks to enable SD card finish the transfer and be stable */
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
	}
	else								/* Write Multiple sector data */
	{
	    /* Send command ACMD23 before multi write if is not a MMC card */
		if (g_SDCard_Type != SD_TYPE_MMC)
		{
			(void)SDCard_SendCommand(ACMD23, count, 0x01);   
		}
		
		/* Write Multi block data command */
		r1 = SDCard_SendCommand(CMD25, sector, 0x01);
			
		if (r1 != 0x00)return (SDCARD_WRITEDISK_ERROR);
		
		/* Chip select enable */
		SD_CS_LOW();
		
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
		
		for (n = 0; n < count; n++)
		{
			/* Start multi block write token: 0xFC */
			(void)SPI_WriteByteData(SDCARD_PORT, 0xFC);
			
			for (i = 0; i < 512; i++)
			{
				ret = SPI_WriteByteData(SDCARD_PORT, *W_Buf++);

				if (ret == -1)
				{
					/* Chip select disable */
					SD_CS_HIGH();

					return (SDCARD_WRITEDISK_ERROR);
				}
			}
			
			/* Send 2 Bytes dummy CRC */
			(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
			(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
			
			/* Receive SD Card response data */
			(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
			
			if ((r1 & 0x1F) != 0x05)
			{
				/* Chip select disable */
				SD_CS_HIGH();
				
				return (SDCARD_WRITEDISK_ERROR);
			}
			
			/* Wait all the data program finished */
			retry = 0;
			
			do
			{
				(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
				
				if (retry ++ == 0x40000)
				{
					/* Chip select disable */
					SD_CS_HIGH();
					
					return (SDCARD_WRITEDISK_ERROR);
				}
			}while (r1 != 0xFF);
		}
		
		/* Send end of transmit token: 0xFD */
		(void)SPI_ReadByteData(SDCARD_PORT, 0xFDu, &r1);
		
		if (r1 == 0x00)return (SDCARD_WRITEDISK_ERROR);
		
		/* Wait all the data program finished */
		retry = 0;
		
		do
		{
			(void)SPI_ReadByteData(SDCARD_PORT, DUMMY_BYTE, &r1);
			
			if (retry ++ == 0x40000)
			{
				/* Chip select disable */
				SD_CS_HIGH();
				
				return (SDCARD_WRITEDISK_ERROR);
			}
		}while (r1 != 0xFFu);
		
		/* Chip select disable */
		SD_CS_HIGH();
		
		/* Generate another 8 clocks to enable SD card finish the transfer and be stable */
		(void)SPI_WriteByteData(SDCARD_PORT, DUMMY_BYTE);
	}
	
	return (SDCARD_WRITEDISK_SUCCESS);
}

/*****************************END OF FILE**************************************/