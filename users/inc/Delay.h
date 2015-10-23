/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Delay.c
  * @author: Wangjian
  * @Descriptiuon: Provides a set of delay functions which is 1ms, 100us, 10us 
  *                and 1us base delay time.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    23-Nov-2015

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

#ifndef  __DELAY_H
#define  __DELAY_H

#include <stdint.h>


/* Exported types ------------------------------------------------------------*/

/* Declaration delay driver version */
#define   DELAY_DRIVER_VERSION     (100)		/* Rev1.0.0 */


/* Time ticks macro which can chose different delay function */
//#define   _1MS_PERTICKS
//#define   _100US_PERTICKS
#define   _10US_PERTICKS
//define    _1US_PERTICKS




#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

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