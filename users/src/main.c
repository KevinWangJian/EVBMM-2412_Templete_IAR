/*
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: main.c
  * @author: Wangjian
  * @Descriptiuon: System main loop function.
  * @Others:  None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    05-Nov-2015
  *
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
#include "System_Init.h"
#include "CAN_Message.h"
#include "LTC6804_Driver.h"




/**
 * @brief   System main loop function.
 * @attention  This function is executing after start up code has been executed. 
 * @param   None
 * @returns None
 */
int main(void)
{		
	System_Initialization();
	
	for (;;)
	{	
		Vol_Temp_Conversion_And_Read();
		
		EVBMM_CANReceiveBufferParsing();
	}
}
/***************************** END OF FILE ************************************/