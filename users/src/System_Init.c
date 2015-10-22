/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: System_Init.c
  * @author: Wangjian
  * @Descriptiuon: System devices and peripherals initialization.
  * @Others:  None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    21-Nov-2015

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

#include "System_Init.h"
#include "arm_cm0.h"
#include "ISR.h"
#include "GPIO_Driver.h"
#include "MSCAN_Driver.h"
#include "System_Timer.h"
#include "Systick_Driver.h"
#include "M95160_Driver.h"
#include "CAN_Message.h"
#include "Flash_Driver.h"
#include "PIT_Driver.h"
#include "LTC6804_Driver.h"



/**
 * @brief   Initialize cell information sampling device. 
 * @attention  Actually,this function is initializing SPI1 peripheral which 
 *             communicate with LTC6820 chip.And initialize PIT_CH0 timer 
 *             which generate interrupt to start cell voltage and cell temperature
 *             conversion.
 * @param   None
 * @returns None
 */
static void CellInfoSampling_Init(void)
{
	PIT_TimebaseInitTypeDef PIT_Config;
	
	LTC6820_SPIx_Config_Initial(SPI1_PortGPins);
	
	PIT_Config.channel   = PIT_CH0;
	PIT_Config.INT_Func  = PIT_INT_ENABLE;
	PIT_Config.freeze    = PIT_FreezeDisable;
	PIT_Config.loadvalue = 100000 - 1;           /* generate interrupt every 5ms. (5ms * 20000(BUS clock) = 100000) */
	
	PIT_Init(&PIT_Config);
	
	PIT_EnableModule(PIT_CHANNEL0);
	
	Set_Vector_Handler(PIT_CH0_VECTORn, PIT_CH0_Handler);
	
	NVIC_EnableIRQ(PIT_CH0_IRQn);
	
	NVIC_SetPriority(PIT_CH0_IRQn, 2);
}



/**
 * @brief   System device and driver initialization before enter main loop.
 * @attention  This function must be executed before enter main loop function.
 * @param   None
 * @returns None
 */
void System_Initialization(void)
{
	uint8_t j;
	
	Systick_Init(_10us_perticks);

	Flash_Init();
	
	M95160_Init();
	
	for (j = 0; j < 2; j++)
	{
		EVBMM_ReadPartialConfigTableData();
	}
	
	CAN_Transmission_Init();
	
	CellInfoSampling_Init();

	LED_OUTPUT();
	LED_OFF();
	
	DI_INPUT();
	
	HEATING_IO_OUTPUT();
	HEATING_IO_LOW();
	
	COOL_IO_OUTPUT();
	COOL_IO_LOW();
}

/*****************************END OF FILE**************************************/