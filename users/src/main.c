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
#include "Systick_Driver.h"
#include "GPIO_Driver.h"



int main(void)
{
	Systick_Init(_1ms_perticks);

	GPIO_PinInit(GPIO_PTC1, GPIO_PinOutput);

	while (1)
	{
		GPIO_PinToggle(GPIO_PTC1);
	
		Delay1ms(1000);
	}
}

/***********************************  End Of File  *****************************/