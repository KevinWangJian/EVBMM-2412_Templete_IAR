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
#include "Delay.h"
#include "MSCAN_Driver.h"
#include "GPIO_Driver.h"
#include "CAN_SoftBuffer.h"
#include "System_Init.h"
#include "LTC6804_Driver.h"
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
		(void)Fill_CANReceiveBuffer(&R_Message);
	}
}



/*
* @brief    PIT timer channel 0 interrupt service routine.
* @param    None.
* @returns  None.
*/
void PIT_CH0_Handler(void)
{
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		/* Before exit PIT0_IRQHandler,clear PIT_Channel0 interrupt flag TIF */
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;

        if( g_LTC6804_Time_Count <= 40)
        {
            g_LTC6804_Time_Count ++;
        }
        else
        {
            g_LTC6804_Conversion_Finish_Flag = 1;
        }
	}
}



/*
* @brief    PIT timer channel 1 interrupt service routine.
* @param    None.
* @returns  None.
*/
void PIT_CH1_Handler(void)
{
	static uint16_t Time_Count = 0;
	
	int32_t ret_val;
	
	MSCAN_Message_TypeDef S_Message;
	
	if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)
	{
		/* Before exit PIT1_IRQHandler,clear PIT_Channel0 interrupt flag TIF */
		PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;

		Time_Count++;
		
		if (Time_Count >= 1000)
		{
			Time_Count = 0;
			
			EVBMM_PreparePartialConfigTableData();
			
			LED_TOGGLE();
		}
				
		/* Checking whether MSCAN module have enough TX buffer to send CAN message. */
		if (MSCAN_HardTxBufferCheck() == 0)
		{
			/* Checking soft CAN TX buffer have valid CAN message. */
			ret_val = Check_CANSendBuffer(&S_Message);
			
			/* If yes,load the read CAN message and send it. */
			if (ret_val == 0)
			{
				(void)MSCAN_SendFrame(&S_Message);
			}
		}
	}
}



/*
* @brief    KBI0 interrupt service routine.
* @param    None.
* @returns  None.
*/
void KBI0_Handler(void)
{
#if 1
	uint32_t ret_val;

	ret_val = KBI0->SP;
	
	if ((ret_val & (1 << 24)))
	{
		GPIO_PinToggle(GPIO_PTC1);
	}
	
	if ((ret_val & (1 << 25)))
	{
		GPIO_PinToggle(GPIO_PTC1);
	}
	
	/* Attention: The following two statements must be called to clear KBI interrupt flag. */	
	KBI0->SC |= KBI_SC_KBACK_MASK;      
	KBI0->SC |= KBI_SC_RSTKBSP_MASK;
#endif
}



/*
* @brief    KBI1 interrupt service routine.
* @param    None.
* @returns  None.
*/
void KBI1_Handler(void)
{
	
}

/***************************** END OF FILE ************************************/