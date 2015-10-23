/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Systick_Driver.c
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about systick timer initialization.
  *                Users can use systick for RTOS and delay functions.
  * @Others:  None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    30-Sep-2015

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
#include <stdint.h>

#include "arm_cm0.h"
#include "Systick_Driver.h"
#include "system_SKEAZ1284.h"
#include "ISR.h"





 /**
 * @brief   Configure Systick timer module with a given over flow cycle time.
 * @param   Cycle: User specified over flow cycle time value.
 * @attention  User can configure the systick timer with 1us,10us,100us,1000us over flow cycle.
 * @returns 0: Calling succeeded.
 * 			-1: Calling failed.
 */
int Systick_Init(CountPeriodPerTicks Cycle)
{
	if ((Cycle < _1ms_perticks) || (Cycle > _1us_perticks))return -1;

	/* Get the System Core clock */
	SystemCoreClockUpdate();

	/* SystemCoreClock / 1000,the CPU generate an interrupt every 1 millisecond */
	/* SystemCoreClock / 10000,the CPU generate an interrupt every 100 microsecond */
	/* SystemCoreClock / 100000,the CPU generate an interrupt every 10 microsecond */
	/* SystemCoreClock / 1000000,the CPU generate an interrupt every 1 microsecond */
	if (_1ms_perticks == Cycle)
	{
		if (SysTick_Config(SystemCoreClock / 1000) != 0)
		{
			return -1;
		}
	}
	else if (_100us_perticks == Cycle)
	{
		if (SysTick_Config(SystemCoreClock / 10000) != 0)
		{
			return -1;
		}		
	}
	else if (_10us_perticks == Cycle)
	{
		if (SysTick_Config(SystemCoreClock / 100000) != 0)
		{
			return -1;
		}		
	}
	else if (_1us_perticks == Cycle)
	{
		if (SysTick_Config(SystemCoreClock / 1000000) != 0)
		{
			return -1;
		}			
	}
	else
	{
		return -1;
	}

	Set_Vector_Handler(SysTick_VECTORn, SysTick_Handler);
	
	NVIC_EnableIRQ(SysTick_IRQn);
	
	return 0;
}


/*****************************END OF FILE**************************************/