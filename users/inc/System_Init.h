/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: System_Init.h
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
#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H

#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"
#include "GPIO_Driver.h"



/* Exported types ------------------------------------------------------------*/



#define   LED_OUTPUT()          GPIO_PinInit(GPIO_PTD4, GPIO_PinOutput)
#define   LED_ON()              GPIO_PinSet(GPIO_PTD4)
#define   LED_OFF()             GPIO_PinClear(GPIO_PTD4)
#define   LED_TOGGLE()          GPIO_PinToggle(GPIO_PTD4)

#define   DI_INPUT()        	GPIO_PinInit(GPIO_PTF0, GPIO_PinInput)
#define   READ_DI_VALUE()   	GPIO_PinRead(GPIO_PTF0)

#define   HEATING_IO_OUTPUT()	GPIO_PinInit(GPIO_PTA3, GPIO_PinOutput)  
#define   HEATING_IO_HIGH()     GPIO_PinSet(GPIO_PTA3)
#define   HEATING_IO_LOW()      GPIO_PinClear(GPIO_PTA3)

#define   COOL_IO_OUTPUT()      GPIO_PinInit(GPIO_PTD2, GPIO_PinOutput)
#define   COOL_IO_HIGH()		GPIO_PinSet(GPIO_PTD2)
#define   COOL_IO_LOW()         GPIO_PinClear(GPIO_PTD2)



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */
	
void System_Initialization(void);

	
#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/