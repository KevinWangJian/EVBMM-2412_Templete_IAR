/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Systick_Driver.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about systick timer initialization.
  *                Users can use systick for RTOS and delay functions.
  * @Others: None
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

#ifndef  __SYSTICK_DRIVER_H
#define  __SYSTICK_DRIVER_H

#include <stdint.h>
#include "SKEAZ1284.h"


/* Exported types ------------------------------------------------------------*/

/* Declaration Systick timer driver version */
#define   SYSTICK_DRIVER_VERSION     (100)		/* Rev1.0.0 */


/* Time ticks macro which can chose different delay function */
#define   _1MS_PERTICKS
//#define   _100US_PERTICKS
//#define   _10US_PERTICKS
//define    _1US_PERTICKS


typedef enum
{
	_1ms_perticks = 1,
	_100us_perticks,
	_10us_perticks,
	_1us_perticks,
}CountPeriodPerTicks;



#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

int32_t Systick_Init(CountPeriodPerTicks Cycle);


void TimeDelay_Decrement(void);


void Delay1ms(volatile uint32_t nTime);

void Delay100us(volatile uint32_t nTime);

void Delay10us(volatile uint32_t nTime);

void Delay1us(volatile uint32_t nTime);


#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/