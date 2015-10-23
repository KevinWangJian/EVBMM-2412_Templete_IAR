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

#include <stdint.h>

#include "Delay.h"


/* Global variable which store the systick timer count value */
static volatile uint32_t g_TimingDelay = 0;




 /**
 * @brief   Global time delay variable decrement function.  
 * @param   None
 * @returns None
 */
void TimeDelay_Decrement(void)
{
	if (g_TimingDelay != 0)
	{
		g_TimingDelay--;
	}
}



/**
 * @brief   1ms base delay function.
 * @param   nTime: User specified delay time. Time = nTime * 1ms.
 * @returns None
 */
#ifdef _1MS_PERTICKS
void Delay1ms(volatile uint32_t nTime)
{
	g_TimingDelay = nTime;
	
	while (g_TimingDelay != 0);
}
#endif



/**
 * @brief   100us base delay function.
 * @param   nTime: User specified delay time. Time = nTime * 100us.
 * @returns None
 */
#ifdef  _100US_PERTICKS
void Delay100us(volatile uint32_t nTime)
{
	g_TimingDelay = nTime;
	
	while (g_TimingDelay != 0);
}
#endif



/**
 * @brief   10us base delay function.
 * @param   nTime: User specified delay time. Time = nTime * 10us.
 * @returns None
 */
#ifdef _10US_PERTICKS
void Delay10us(volatile uint32_t nTime)
{
	g_TimingDelay = nTime;
	
	while (g_TimingDelay != 0);
}
#endif



/**
 * @brief   1us base delay function.
 * @param   nTime: User specified delay time. Time = nTime * 1us.
 * @returns None
 */
#ifdef _1US_PERTICKS
void Delay1us(volatile uint32_t nTime)
{
	g_TimingDelay = nTime;
	
	while (g_TimingDelay != 0);
}
#endif

/*****************************END OF FILE**************************************/