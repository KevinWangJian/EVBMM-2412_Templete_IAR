/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Flash_Driver.c
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about Flash driver.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    05-Nov-2015

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

#include "Flash_Driver.h"
#include "system_SKEAZ1284.h"




/**
 * @brief   Initializes the flash controller's clock and flash Data Speculation
 * @param   none
 * @returns none
 */
void Flash_Init(void) 
{
	/* Set the Flash clock divider */
	FTMRE->FCLKDIV &= ~FTMRE_FCLKDIV_FDIV_MASK;

	if ((SystemBusClock >= 1000000u) && (SystemBusClock <= 1600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x00u);
	else if ((SystemBusClock > 1600000u) && (SystemBusClock <= 2600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x01u);
	else if ((SystemBusClock > 2600000u) && (SystemBusClock <= 3600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x02u);
	else if ((SystemBusClock > 3600000u) && (SystemBusClock <= 4600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x03u);
	else if ((SystemBusClock > 4600000u) && (SystemBusClock <= 5600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x04u);
	else if ((SystemBusClock > 5600000u) && (SystemBusClock <= 6600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x05u);
	else if ((SystemBusClock > 6600000u) && (SystemBusClock <= 7600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x06u);
	else if ((SystemBusClock > 7600000u) && (SystemBusClock <= 8600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x07u);
	else if ((SystemBusClock > 8600000u) && (SystemBusClock <= 9600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x08u);
	else if ((SystemBusClock > 9600000u) && (SystemBusClock <= 10600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x09u);
	else if ((SystemBusClock > 10600000u) && (SystemBusClock <= 11600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x0Au);
	else if ((SystemBusClock > 11600000u) && (SystemBusClock <= 12600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x0Bu);
	else if ((SystemBusClock > 12600000u) && (SystemBusClock <= 13600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x0Cu);
	else if ((SystemBusClock > 13600000u) && (SystemBusClock <= 14600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x0Du);
	else if ((SystemBusClock > 14600000u) && (SystemBusClock <= 15600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x0Eu);
	else if ((SystemBusClock > 15600000u) && (SystemBusClock <= 16600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x0Fu);
	else if ((SystemBusClock > 16600000u) && (SystemBusClock <= 17600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x10u);
	else if ((SystemBusClock > 17600000u) && (SystemBusClock <= 18600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x11u);
	else if ((SystemBusClock > 18600000u) && (SystemBusClock <= 19600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x12u);
	else if ((SystemBusClock > 19600000u) && (SystemBusClock <= 20600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x13u);
	else if ((SystemBusClock > 20600000u) && (SystemBusClock <= 21600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x14u);
	else if ((SystemBusClock > 21600000u) && (SystemBusClock <= 22600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x15u);
	else if ((SystemBusClock > 22600000u) && (SystemBusClock <= 23600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x16u);
	else if ((SystemBusClock > 23600000u) && (SystemBusClock <= 24600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x17u);
	else if ((SystemBusClock > 24600000u) && (SystemBusClock <= 25600000u))
		FTMRE->FCLKDIV |= FTMRE_FCLKDIV_FDIV(0x18u);
	else 
		return;

	/* Enables the Stalling Flash Controller and the Flash Data Speculation */
	MCM->PLACR = MCM_PLACR_ESFC_MASK | MCM_PLACR_EFDS_MASK;
}



/**
 * @brief   Erase a sector size of flash memory.
 * @param   StartAddrOfSector: the flash address user specified.
 * @attention One sector size is 512 bytes in KEA serious MCU.
 * @returns  0: Calling succeeded.
 *          -1: Calling failed.
 */
int Flash_EarseSector(uint32_t StartAddrOfSector) 
{
	int ret_val = -1;

	/* Ensures the given address of the sector which will be erased is within the writable area. */
	if (StartAddrOfSector <= __EndOfFlash) 
	{
		/* According to the RM of the KEA128, ensures the 2 LSB is */
		/* clear, otherwise the operation will cause error. */
		if (!(StartAddrOfSector & 0x03)) 
		{
			/* Ensures the Flash Controller is not busy, that is, the */
			/* bit FTMRE.FSTAT.CCIF is clear */
			if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
			{
				/* Clears the bits FTMRE.FSTAT.ACCERR and FTMRE.FSTAT.FPVIOL */
				FTMRE->FSTAT = FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK;

				/* Erases the sector. */
				FTMRE->FCCOBIX = 0;
				FTMRE->FCCOBHI = 0x0A;
				FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector >> 16);

				FTMRE->FCCOBIX = 1;
				FTMRE->FCCOBHI = (uint8_t)(StartAddrOfSector >> 8);
				FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector & 0xFF);

				/* Do the actual erasing operation */
				FTMRE->FSTAT |= FTMRE_FSTAT_CCIF_MASK;

				/* Read FTMRE_FSTAT register to check whether CCIF has been set */
				while (1) 
				{
					if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
					{
						break;
					}
				}

				ret_val = (FTMRE->FSTAT == FTMRE_FSTAT_CCIF_MASK) ? 0 : -1;
			}
		}
	}

	return ret_val;
}



/**
 * @brief   Writes a unit of data (8 Bytes) to the given area.
 * @param   StartAddrOfSector: the flash address user specified.
 *          data：the pointer which is point to the write data buffer,
 *                and the data length should be 8 bytes.
 * @returns  0：Calling succeeded.
 *          -1：Calling failed.
 */
int Flash_ProgramUnit(uint32_t StartAddrOfSector, uint8_t *data) 
{
	int ret_val = -1;

	/* Ensures the given address of the sector which will be erased is within the writable area. */
	if (StartAddrOfSector <= __EndOfFlash) 
	{
		/* According to the RM of the KEA128, ensures the 2 LSB is */
		/* clear, otherwise the operation will cause error. */
		if (!(StartAddrOfSector & 0x03))
		{
			/* Ensures the sector of being to be programmed has been */
			/* well erased already. */
			if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
			{
				/* Clears the bits FTMRE.FSTAT.ACCERR and FTMRE.FSTAT.FPVIOL */
				FTMRE->FSTAT = FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK;

				FTMRE->FCCOBIX = 0;
				FTMRE->FCCOBHI = 0x03;
				FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector >> 16);

				FTMRE->FCCOBIX = 1;
				FTMRE->FCCOBHI = (uint8_t)(StartAddrOfSector >> 8);
				FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector & 0xFF);

				FTMRE->FCCOBIX = 2;
				FTMRE->FCCOBHI = 0;
				FTMRE->FCCOBLO = 2; /*2 longwords, a unit*/

				/* Does actual test. */
				FTMRE->FSTAT |= FTMRE_FSTAT_CCIF_MASK;

				while (1) 
				{
					if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
					{
						break;
					}
				}

				/* check whether CCIF is set and do the actual program process */
				if (FTMRE->FSTAT == FTMRE_FSTAT_CCIF_MASK)
				{
					/* When the above test is passed,  */
					FTMRE->FCCOBIX = 0;
					/* assign the program flash command 0x06 */
					FTMRE->FCCOBHI = 0x06;
					FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector >> 16);

					FTMRE->FCCOBIX = 1;
					FTMRE->FCCOBHI = (uint8_t)(StartAddrOfSector >> 8);
					FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector & 0xFF);

					/* here,assign the actual data which want to write to the flash memory */
					FTMRE->FCCOBIX = 2;
					FTMRE->FCCOBLO = *data++;
					FTMRE->FCCOBHI = *data++;

					FTMRE->FCCOBIX = 3;
					FTMRE->FCCOBLO = *data++;
					FTMRE->FCCOBHI = *data++;

					FTMRE->FCCOBIX = 4;
					FTMRE->FCCOBLO = *data++;
					FTMRE->FCCOBHI = *data++;

					FTMRE->FCCOBIX = 5;
					FTMRE->FCCOBLO = *data++;
					FTMRE->FCCOBHI = *data++;

					/* Does the actual erasing operation. */
					FTMRE->FSTAT |= FTMRE_FSTAT_CCIF_MASK;

					while (1)
					{
						if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
						{
							break;
						}
					}

					/* read FTMRE_FSTAT register to check whether error has occurred. */
					ret_val = (FTMRE->FSTAT == FTMRE_FSTAT_CCIF_MASK) ? 0 : -1;
				}
			}
		}
	}

	return ret_val;
}



/**
 * @brief   Writes a sector of data to the flash memory
 * @param   StartAddrOfSector: the flash address user specified.
 *          data：the pointer which is point to the write data buffer,
 *                and the data length should be one sector size(512 bytes).
 * @returns  0：Calling succeeded.
 *          -1：Calling failed.
 */
int Flash_ProgramSector(uint32_t StartAddrOfSector, uint8_t *data) 
{
	int ret_val = -1;
	int i;

	/* Ensures the given address of the sector which will be erased is within the writable area. */
	if (StartAddrOfSector <= __EndOfFlash) 
	{
		/* According to the RM of the KEA128, ensures the 2 LSB is */
		/* clear, otherwise the operation will cause error. */
		if (!(StartAddrOfSector & 0x03)) 
		{
			/* Ensures the sector of being to be programmed has been */
			/* well erased already. */
			if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
			{
				/* Clears the bits FTMRE.FSTAT.ACCERR and FTMRE.FSTAT.FPVIOL */
				FTMRE->FSTAT = FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK;

				FTMRE->FCCOBIX = 0;
				/* assign the verify flash command 0x03 */
				FTMRE->FCCOBHI = 0x03;
				FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector >> 16);

				FTMRE->FCCOBIX = 1;
				FTMRE->FCCOBHI = (uint8_t)(StartAddrOfSector >> 8);
				FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector & 0xFF);

				FTMRE->FCCOBIX = 2;
				FTMRE->FCCOBHI = 0;
				FTMRE->FCCOBLO = 128; /*128 longwords, a sector*/

				/* Does actual test. */
				FTMRE->FSTAT |= FTMRE_FSTAT_CCIF_MASK;

				while (1) 
				{
					if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK)
					{
						break;
					}
				}

				if (FTMRE->FSTAT == FTMRE_FSTAT_CCIF_MASK) 
				{
					for (i = 0; i < 64; i++) 
					{
						FTMRE->FCCOBIX = 0;
						FTMRE->FCCOBHI = 0x06;
						FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector >> 16);

						FTMRE->FCCOBIX = 1;
						FTMRE->FCCOBHI = (uint8_t)(StartAddrOfSector >> 8);
						FTMRE->FCCOBLO = (uint8_t)(StartAddrOfSector & 0xFF);

						FTMRE->FCCOBIX = 2;
						FTMRE->FCCOBLO = *data++;
						FTMRE->FCCOBHI = *data++;

						FTMRE->FCCOBIX = 3;
						FTMRE->FCCOBLO = *data++;
						FTMRE->FCCOBHI = *data++;

						FTMRE->FCCOBIX = 4;
						FTMRE->FCCOBLO = *data++;
						FTMRE->FCCOBHI = *data++;

						FTMRE->FCCOBIX = 5;
						FTMRE->FCCOBLO = *data++;
						FTMRE->FCCOBHI = *data++;

						/* Does the actual erasing operation. */
						FTMRE->FSTAT |= FTMRE_FSTAT_CCIF_MASK;

						while (1) 
						{
							if (FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK) 
							{
								break;
							}
						}

						if (FTMRE->FSTAT != FTMRE_FSTAT_CCIF_MASK) 
						{
							break;
						}

						/* the address increase a unit(8 bytes) */
						StartAddrOfSector += 8;
					}

					/* read FTMRE_FSTAT register to check whether error has occurred. */
					ret_val = (FTMRE->FSTAT == FTMRE_FSTAT_CCIF_MASK) ? 0 : -1;
				}
			}
		}
	}

	return ret_val;
}

/*****************************END OF FILE**************************************/