/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: PIT_Driver.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about GPIO driver.
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
#ifndef __PIT_DRIVER_H
#define __PIT_DRIVER_H

#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"



/* Exported types ------------------------------------------------------------*/

#define   PIT_DRIVER_VERSION    (100)



#define   PIT_CHANNEL0    0
#define   PIT_CHANNEL1    1



typedef enum
{
	PIT_CH0 = 1,
	PIT_CH1,
}PIT_Channel_TypeDef;



typedef enum
{
	PIT_FreezeEnable = 1,
	PIT_FreezeDisable,
}PIT_FreezeFunc_TypeDef;



typedef enum
{
	PIT_INT_ENABLE = 1,
	PIT_INT_DISABLE,
}PIT_InterruptFunc_TypeDef;



typedef struct
{
	PIT_Channel_TypeDef channel;
	
	PIT_InterruptFunc_TypeDef INT_Func;

	PIT_FreezeFunc_TypeDef freeze;

	uint32_t loadvalue;
	
}PIT_TimebaseInitTypeDef;




#ifdef __cplusplus
extern "C" {
#endif


/* Exported functions ------------------------------------------------------- */


int32_t PIT_Init(PIT_TimebaseInitTypeDef* TimConfig);


void PIT_EnableModule(uint8_t ch);


void PIT_DisableModule(uint8_t ch);



#ifdef __cplusplus
}
#endif

#endif /* __FLASHCI_H */

/*****************************END OF FILE**************************************/