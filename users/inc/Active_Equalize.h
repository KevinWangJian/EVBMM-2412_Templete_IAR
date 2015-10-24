/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Active_Equalize.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions which parsing recived CAN messages
  *                and raplying with the specified CAN messages to master.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    26-Sep-2015

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
#ifndef  __ACTIVE_EQUALIZE_H
#define  __ACTIVE_EQUALIZE_H

#include <stdint.h>
#include <stdlib.h>

#include "arm_cm0.h"
#include "SKEAZ1284.h"
#include "GPIO_Driver.h"


/* Exported types ------------------------------------------------------------*/


/*
#define     _8V5VOL_CONTROLPIN_OUTPUT()              GPIO_PinInit(GPIO_PTI1, GPIO_PinOutput)
#define     _8V5VOL_ENABLE()                         GPIO_PinSet(GPIO_PTI1)
#define     _8V5VOL_DISABLE()                        GPIO_PinClear(GPIO_PTI1)



#define     EQUALIZATION_MODULE1_DIRPIN_OUTPUT()     GPIO_PinInit(GPIO_PTI0, GPIO_PinOutput)
#define     EQUALIZATION_MODULE1_CHARGE_MODE()       GPIO_PinClear(GPIO_PTI0)
#define     EQUALIZATION_MODULE1_DISCHARGE_MODE()    GPIO_PinSet(GPIO_PTI0)

#define     EQUALIZATION_MODULE1_FAULTPIN_INPUT()    GPIO_PinInit(GPIO_PTH1, GPIO_PinInput)
#define     EQUALIZATION_MODULE1_GETFAULT_STATUS()   GPIO_PinRead(GPIO_PTH1)



#define     EQUALIZATION_MODULE2_DIRPIN_OUTPUT()     GPIO_PinInit(GPIO_PTH0, GPIO_PinOutput_HighCurrent)
#define     EQUALIZATION_MODULE2_CHARGE_MODE()       GPIO_PinClear(GPIO_PTH0)
#define     EQUALIZATION_MODULE2_DISCHARGE_MODE()    GPIO_PinSet(GPIO_PTH0) 

#define     EQUALIZATION_MODULE2_FAULTPIN_INPUT()    GPIO_PinInit(GPIO_PTE6, GPIO_PinInput)
#define     EQUALIZATION_MODULE2_GETFAULT_STATUS()   GPIO_PinRead(GPIO_PTE6)



#define     EQUALIZATION_MODULE3_DIRPIN_OUTPUT()     GPIO_PinInit(GPIO_PTE5, GPIO_PinOutput)
#define     EQUALIZATION_MODULE3_CHARGE_MODE()       GPIO_PinClear(GPIO_PTE5)
#define     EQUALIZATION_MODULE3_DISCHARGE_MODE()    GPIO_PinSet(GPIO_PTE5) 

#define     EQUALIZATION_MODULE3_FAULTPIN_INPUT()    GPIO_PinInit(GPIO_PTB5, GPIO_PinInput)
#define     EQUALIZATION_MODULE3_GETFAULT_STATUS()   GPIO_PinRead(GPIO_PTB5)



#define     EQUALIZATION_MODULE4_DIRPIN_OUTPUT()     GPIO_PinInit(GPIO_PTB4, GPIO_PinOutput_HighCurrent)
#define     EQUALIZATION_MODULE4_CHARGE_MODE()       GPIO_PinClear(GPIO_PTB4)
#define     EQUALIZATION_MODULE4_DISCHARGE_MODE()    GPIO_PinSet(GPIO_PTB4)

#define     EQUALIZATION_MODULE4_FAULTPIN_INPUT()    GPIO_PinInit(GPIO_PTC3, GPIO_PinInput)
#define     EQUALIZATION_MODULE4_GETFAULT_STATUS()   GPIO_PinRead(GPIO_PTC3)
*/


typedef enum 
{
    Module_1 = 1,
    Module_2,
    Module_3,
    Module_4,
}ModuleNumber_TypeDef;






#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */




#ifdef __cplusplus
}
#endif

#endif
/*****************************END OF FILE**************************************/