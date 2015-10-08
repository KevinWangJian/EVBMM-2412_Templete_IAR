/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: main.c
  * @author: Wangjian
  * @Descriptiuon: System main loop function.
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

#include <stdint.h>
#include "arm_cm0.h"
#include "ISR.h"
#include "Systick_Driver.h"
#include "GPIO_Driver.h"
#include "MSCAN_Driver.h"
#include "CAN_Message.h"
#include "Flash_Driver.h"
#include "PIT_Driver.h"




int main(void)
{
#if 0
	MSCAN_ParametersConfig CAN_Property;
	
	MSCAN_FilterConfigure CAN_FILTER;
	
	MSCAN_Message_TypeDef W_Message, Rd_Message;
#endif
	
	PIT_TimebaseInitTypeDef PIT_Config;
	
	
	Systick_Init(_1ms_perticks);

	GPIO_PinInit(GPIO_PTC1, GPIO_PinOutput);
	GPIO_PinInit(GPIO_PTG0, GPIO_PinOutput);
	
	GPIO_PinClear(GPIO_PTC1);
	GPIO_PinClear(GPIO_PTG0);

	PIT_Config.channel   = PIT_CH0;
	PIT_Config.INT_Func  = PIT_INT_ENABLE;
	PIT_Config.freeze    = PIT_FreezeEnable;
	PIT_Config.loadvalue = 120000 - 1;           // generate interrupt every 5ms. (5ms * 24000(BUS clock) = 120000)
	
	PIT_Init(&PIT_Config);
	
	PIT_EnableModule(PIT_CHANNEL0);
	
	Set_Vector_Handler(PIT_CH0_VECTORn, PIT_CH0_Handler);
	
	NVIC_EnableIRQ(PIT_CH0_IRQn);	
	
#if 0
	CAN_Property.baudrate                   = MSCAN_Baudrate_250K;
	CAN_Property.MSCAN_SignalPinsRemap      = 0;
	CAN_Property.MSCAN_StopInWaitMode       = 1;
	CAN_Property.MSCAN_TimeStampEnable      = 0;
	CAN_Property.MSCAN_WakeUpEnable         = 0;
	CAN_Property.MSCAN_ClockSource          = 1;
	CAN_Property.MSCAN_LoopbackMode         = 0;
	CAN_Property.MSCAN_ListenOnlyMode       = 0;
	CAN_Property.MSCAN_BusoffRecoveryMode   = 0;
	CAN_Property.MSCAN_WakeUpMode           = 0;
	CAN_Property.MSCAN_WakeUpINTEnable      = 0;
	CAN_Property.MSCAN_OverrunINTEnable     = 0;
	CAN_Property.MSCAN_ReceiveFullINTEnable = 1;
	CAN_Property.MSCAN_TransEmptyINTEnable  = 0;
	
	W_Message.frame_type  = DataFrameWithExtendedId;
	W_Message.frame_id    = 0x12344444u;
	W_Message.data_length = 8;
	W_Message.data[0]     = 0x86u;
	W_Message.data[1]     = 0x86u;
	W_Message.data[2]     = 0x86u;
	W_Message.data[3]     = 0x86u;
	W_Message.data[4]     = 0x86u;
	W_Message.data[5]     = 0x86u;
	W_Message.data[6]     = 0x86u;
	W_Message.data[7]     = 0x86u;
	
	CAN_FILTER.Filter_Enable  = 1;			 /* If this value is cleared,the following parameters does not work */
	CAN_FILTER.Filter_Channel = 0;
	CAN_FILTER.frame_type     = AcceptBothFrame;
	CAN_FILTER.id_format      = OnlyAcceptExtendedID;
	CAN_FILTER.id_mask        = 0xFFFFFFFFu; /* If this value is 0xFFFFFFFF,the id_accept value does not work */
	CAN_FILTER.id_accept      = 0x18901212u;
	
	MSCAN_Init(&CAN_Property, &CAN_FILTER);
	
	Set_Vector_Handler(MSCAN_RX_VECTORn, MSCAN_RX_Handler);
	
	NVIC_EnableIRQ(MSCAN_RX_IRQn);
#endif
	
	while (1)
	{	
#if 0
		if (Check_CANReceiveBuffer(&Rd_Message) == 0)
		{
			if (Rd_Message.frame_id == 0x18901212u)
			{
				Rd_Message.frame_id = 0;
				
				GPIO_PinToggle(GPIO_PTC1);
			}
		}
#endif
		;
	}
}

/***********************************  End Of File  *****************************/