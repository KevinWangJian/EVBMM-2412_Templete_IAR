/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: ISR.h
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
#ifndef  __ISR_H
#define  __ISR_H

#include <stdint.h>
#include "SKEAZ1284.h"


/* Exported types ------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

void SysTick_Handler(void);

void MSCAN_RX_Handler(void);

void PIT_CH0_Handler(void);


#ifdef __cplusplus
}
#endif

#endif
/*****************************END OF FILE**************************************/