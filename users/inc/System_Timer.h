/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: System_Timer.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about Flash driver.
  * @Others: None
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __SYSTEM_TIMER_H
#define  __SYSTEM_TIMER_H

#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"
#include "PIT_Driver.h"


/* Exported types ------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */

	
void CANSendMessageTimer_Init(void);





#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/