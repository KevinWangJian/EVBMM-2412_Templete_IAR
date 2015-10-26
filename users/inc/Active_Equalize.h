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



typedef enum 
{
    Module_1 = 1,
    Module_2,
    Module_3,
    Module_4,
}ModuleNumber_TypeDef;



typedef enum 
{
    Charge_Equalize = 1,
    Discharge_Equalize,
}EqualizeMode_TypeDef;



typedef enum 
{
    Cell_Channel_1 = 1,
    Cell_Channel_2,
    Cell_Channel_3,
    Cell_Channel_4,
    Cell_Channel_5,
    Cell_Channel_6,

    Cell_Channel_7,
    Cell_Channel_8,
    Cell_Channel_9,
    Cell_Channel_10,
    Cell_Channel_11,
    Cell_Channel_12,

    Cell_Channel_13,
    Cell_Channel_14,
    Cell_Channel_15,
    Cell_Channel_16,
    Cell_Channel_17,
    Cell_Channel_18,
    
    Cell_Channel_19,
    Cell_Channel_20,
    Cell_Channel_21,
    Cell_Channel_22,
    Cell_Channel_23,
    Cell_Channel_24,
}EqualizeChannel_TypeDef;



typedef struct 
{
    uint8_t Module1_Status;
    uint8_t Module2_Status;
    uint8_t Module3_Status;
    uint8_t Module4_Status; 
}EqualizeModuleStatus_TypeDef;



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

void _8V5Voltage_Enable(void);


void _8V5Voltage_Disable(void);


uint8_t EqualizationModule_GetFaultSignalValue(ModuleNumber_TypeDef Module_Num);


int EqualizationModulePins_Initial(void);


int EqualizationModule_Open(EqualizeMode_TypeDef Eq_Mode, EqualizeChannel_TypeDef Eq_Ch);


int EqualizationModule_Close(EqualizeChannel_TypeDef Eq_Ch);


uint8_t Getting_EqualizingStatusValue(ModuleNumber_TypeDef Eq_Num);


void Settiing_EqualizingStatusValue(ModuleNumber_TypeDef Eq_Num, uint8_t val);


#ifdef __cplusplus
}
#endif

#endif
/*****************************END OF FILE**************************************/