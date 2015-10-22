/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: M95160_Driver.c
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

/* Includes ------------------------------------------------------------------*/

#include "M95160_Driver.h"
#include "SPI_Driver.h"



/**
 * @brief   Initialzie M95160 chip.
 * @attention Actually,this function is initialize SPI port which will conmunicate 
 *            with M95160 chip.
 * @param   None.
 * @returns None.
 */
void M95160_Init(void) 
{
    SPIx_ConfigType spi_config;
    
    spi_config.SPRF_MODF_INT_Enable      = 0;
    spi_config.Match_INT_Enable          = 0;
    spi_config.Transmit_INT_Enable       = 0;
    spi_config.CPHA                      = 0;
    spi_config.CPOL                      = 0;
    spi_config.LSBFE                     = 0;
    spi_config.MasterOrSlaveMode         = 1;
	spi_config.Signal_Pins               = SPI0_PortEPins;
    
    (void)SPI_Init(SPI0, &spi_config, High_Speed);
    
    /* Configure GPIO pin as SPI chip select signal. */
    M95160_CS_Output();
    
    /* Pull down SPI chip select signal. */
    M95160_CS_LOW();
}



/**
 * @brief   Writting status register with specified value.
 * @param   SPIx, The specified SPI port.
 *          W_Value, user specified value which will be written to M95160 status register.
 * @returns None.
 */
void M95160_WriteStatusRegister(uint8_t W_Value) 
{   
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, WREN);
    
    M95160_CS_HIGH(); 
    
    
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, WRSR);
    
    (void)SPI_WriteByteData(EEPROM_PORT, W_Value);
    
    M95160_CS_HIGH();    
}



/**
 * @brief   Reading status register.
 * @param   SPIx, The specified SPI port.
 *          *R_Value, Data buffer which store the status register value.
 * @returns 0, Called successfully.Which means writting process has completely finished.
           -1, Called failed.Which means writting process has not completely finished.It's busy!
 */
int M95160_ReadStatusRegister(uint8_t* R_Value) 
{
    uint16_t Time_Count = 0;
    
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, RDSR);
    
    do 
    {
        (void)SPI_ReadByteData(EEPROM_PORT, 0xFFu, R_Value);
        
        Time_Count++;
        
    }while (((*R_Value & 0x01) != 0) && (Time_Count < 0xFFFFu));
    
    if (Time_Count >= 0xFFFFu) 
    {
        M95160_CS_HIGH();
        
        return -1;
    }

    M95160_CS_HIGH();
    
    return 0;
}



/**
 * @brief   Writting one byte data to M95160 chip with specified memory address.
 * @param   SPIx, The specified SPI port.
 *          emory_Addr, User specified memory address.
 *          W_Data, User specified data which will be written to the M95160 chip.
 * @returns 0, Called successfully.Which means writting process has completely finished.
           -1, Called failed.Which means writting process has not completely finished.It's busy!
 */       
int M95160_WriteSingleByteData(uint16_t Memory_Addr, uint8_t W_Data) 
{
    uint8_t Address[2];
    uint8_t i,ret_val;
    
    if (Memory_Addr > MEMORY_ENDADDRESS)return -1;
    
    Address[0] = (uint8_t)((Memory_Addr >> 8) & 0x00FFu);
    Address[1] = (uint8_t)(Memory_Addr & 0x00FFu);
    
    if (M95160_ReadStatusRegister(&ret_val) != 0)return -1;
    
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, WREN);
    
    M95160_CS_HIGH(); 
    
    
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, WRITE);
    
    for (i = 0; i < 2; i++) 
    {
        (void)SPI_WriteByteData(EEPROM_PORT, Address[i]);
    }
    
    (void)SPI_WriteByteData(EEPROM_PORT, W_Data);
    
    M95160_CS_HIGH();
    
    if (M95160_ReadStatusRegister(&ret_val) != 0)return -1;
    
    return 0;
}



/**
 * @brief   Writting sequence bytes data to M95160 chip with specified memory address.
 * @attention  The writen data length can not be more than one page size,and the page size is
 *             32 bytes. 
 * @param   SPIx, The specified SPI port.
 *          *W_Data, Data buffer which store the written data.
 *          Memory_Addr, User specified memory address.
 *          W_Length, user specified data length.
 * @returns 0, Called successfully.Which means writting process has completely finished.
           -1, Called failed.Which means writting process has not completely finished.It's busy!
 */
int M95160_WriteSequenceBytesData(uint16_t Memory_Addr, uint8_t* W_Data, uint8_t W_Length)
{
    uint8_t Address[2];
    uint8_t i,ret_val;

    if (Memory_Addr > MEMORY_ENDADDRESS)return -1;
    
    if (W_Data == NULL)return -1;
    
    if (W_Length > PAGE_SIZE)return -1;
    
    Address[0] = (uint8_t)((Memory_Addr >> 8) & 0x00FFu);
    Address[1] = (uint8_t)(Memory_Addr & 0x00FFu);

    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, WREN);
    
    M95160_CS_HIGH();
    
    
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, WRITE);
    
    for (i = 0; i < 2; i++) 
    {
        (void)SPI_WriteByteData(EEPROM_PORT, Address[i]);
    }
    
    for (i = 0; i < W_Length; i++) 
    {
        (void)SPI_WriteByteData(EEPROM_PORT, *W_Data++);      
    }
        
    M95160_CS_HIGH();
    
    if (M95160_ReadStatusRegister(&ret_val) != 0)return -1;
	
    return 0;    
}



/**
 * @brief   Reading sequence bytes data from M95160 chip with specified memory address.
 * @param   SPIx, The specified SPI port.
 *          *R_Data, Data buffer which store the read data.
 *          Memory_Addr, User specified memory address.
 *          R_Length, user specified data length.
 * @returns 0, Called successfully.Which means writting process has completely finished.
           -1, Called failed.Which means writting process has not completely finished.It's busy!
 */
int M95160_ReadSequenceBytesData(uint16_t Memory_Addr, uint8_t* R_Data, uint16_t R_Length) 
{
    uint8_t Address[2];
    uint16_t i;
    
    if ((Memory_Addr + R_Length) > MEMORY_ENDADDRESS)return -1;
    
    if (R_Data == NULL)return -1;
    
    Address[0] = (uint8_t)((Memory_Addr >> 8) & 0x00FFu);
    Address[1] = (uint8_t)(Memory_Addr & 0x00FFu);
    
    M95160_CS_LOW();
    
    (void)SPI_WriteByteData(EEPROM_PORT, READ);
    
    for (i = 0; i < 2; i++) 
    {
        (void)SPI_WriteByteData(EEPROM_PORT, Address[i]);
    }
    
    for (i = 0; i < R_Length; i++) 
    {
        (void)SPI_ReadByteData(EEPROM_PORT, 0xFFu, R_Data);
        
        R_Data++;      
    }
        
    M95160_CS_HIGH();
    
    return 0;        
}

/*****************************END OF FILE**************************************/