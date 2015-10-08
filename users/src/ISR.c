/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: ISR.c
  * @author: Wangjian
  * @Descriptiuon: System interrupt service routines which provides peripheral
  *                modules interrupt program by users.
  * @Others:  None
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

#include "ISR.h"
#include "Systick_Driver.h"
#include "MSCAN_Driver.h"
#include "GPIO_Driver.h"
#include "CAN_Message.h"




/*
* @brief    Systick timer interrupt service routine.
* @param    None.
* @returns  None.
*/
void SysTick_Handler(void)
{
	TimeDelay_Decrement();
}



/*
* @brief    MSCAN module interrupt service routine.
* @param    None.
* @returns  None.
*/
void MSCAN_RX_Handler(void)
{
	MSCAN_Message_TypeDef R_Message;
	
	if (MSCAN_ReceiveFrame(&R_Message) == 0)
	{
		Fill_CANReceiveBuffer(&R_Message);
	}
}



/*
* @brief    PIT timer channel 0 interrupt service routine.
* @param    None.
* @returns  None.
*/
void PIT_CH0_Handler(void)
{
	static uint16_t Time_Count = 0;
	
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		/* Before exit PIT0_IRQHandler,clear PIT_Channel0 interrupt flag TIF */
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
		
		Time_Count++;
		
		if (Time_Count >= 200)
		{
			Time_Count = 0;
			
			GPIO_PinToggle(GPIO_PTC1);
		}
	}
}


/*****************************END OF FILE**************************************/