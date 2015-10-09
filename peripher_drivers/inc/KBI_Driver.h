/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: KBI_Driver.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions about KBI driver.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    08-Nov-2015

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
#ifndef __KBI_DRIVER_H
#define __KBI_DRIVER_H

#include <stdint.h>
#include <stdlib.h>

#include "SKEAZ1284.h"



/* Exported types ------------------------------------------------------------*/

/* Declaration KBI driver version */
#define   KBI_DRIVER_VERSION    (100)		/* Rev1.0.0 */



/* KBI module pins number enumeration */
typedef enum
{
	KBIx_P0  = (1 << 0),
	
	KBIx_P1  = (1 << 1),
	KBIx_P2  = (1 << 2),
	KBIx_P3  = (1 << 3),
	KBIx_P4  = (1 << 4),
	KBIx_P5  = (1 << 5),
	KBIx_P6  = (1 << 6),
	KBIx_P7  = (1 << 7),
	KBIx_P8  = (1 << 8),
	KBIx_P9  = (1 << 9),
	KBIx_P10 = (1 << 10),

	KBIx_P11 = (1 << 11),
	KBIx_P12 = (1 << 12),
	KBIx_P13 = (1 << 13),
	KBIx_P14 = (1 << 14),
	KBIx_P15 = (1 << 15),
	KBIx_P16 = (1 << 16),
	KBIx_P17 = (1 << 17),
	KBIx_P18 = (1 << 18),
	KBIx_P19 = (1 << 19),
	KBIx_P20 = (1 << 20),
	
	KBIx_P21 = (1 << 21),
	KBIx_P22 = (1 << 22),
	KBIx_P23 = (1 << 23),
	KBIx_P24 = (1 << 24),
	KBIx_P25 = (1 << 25),
	KBIx_P26 = (1 << 26),
	KBIx_P27 = (1 << 27),
	KBIx_P28 = (1 << 28),
	KBIx_P29 = (1 << 29),
	KBIx_P30 = (1 << 30),
	
	KBIx_P31 = (1 << 31),	
}KBIx_PinNumber_TypeDef;



/* KBI trigger mode selections enumeration */
typedef enum
{
	FallingEdge_LowLevel,
	RisingEdge_HighLevel,
}KBIx_EdgeSelects_TypeDef;



/* KBI properties parameter enumeration */
typedef struct
{	
	uint8_t Detection_Mode                  :1;
	
	uint8_t KBI_Interrupt_Enable            :1;
	
	KBIx_PinNumber_TypeDef Pin_Num;
	
	KBIx_EdgeSelects_TypeDef Trigger_Mode;
	
}KBI_Configuration_TypeDef;




#ifdef __cplusplus
extern "C" {
#endif


/* Exported functions ------------------------------------------------------- */


int32_t KBI_Init(KBI_Type* KBIx, KBI_Configuration_TypeDef* KBIx_Config);



#ifdef __cplusplus
}
#endif

#endif /* __FLASHCI_H */

/*****************************END OF FILE**************************************/