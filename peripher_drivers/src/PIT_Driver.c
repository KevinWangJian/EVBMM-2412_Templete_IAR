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
#include "arm_cm0.h"
#include "PIT_Driver.h"



/**
 * @brief   Initialize PIT timer with specified channel and period time.
 * @param   *TimConfig, PIT timer configuration parameters.
 * @returns  0: Calling succeeded.
 * 			-1: Calling failed.
 */
int PIT_Init(PIT_TimebaseInitTypeDef* TimConfig)
{
	if (TimConfig == NULL)return -1;

	/* PIT peripheral module clock is enabled. */
	SIM->SCGC |= SIM_SCGC_PIT_MASK;

	/* First,clock for standard PIT timers is disabled.This field must be set before any other setup is done */
	PIT->MCR |= PIT_MCR_MDIS_MASK;

	if (TimConfig->freeze == PIT_FreezeEnable)
	{
		PIT->MCR |= PIT_MCR_FRZ_MASK;
	}
	else
	{
		PIT->MCR &= ~PIT_MCR_FRZ_MASK;
	}

	if (TimConfig->channel == PIT_CH0)
	{
		/* Disable Timer0 chain mode. Attention: Timer0 can not be chained. */
		PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_CHN_MASK;

		/* Load the time period value */
		PIT->CHANNEL[0].LDVAL = TimConfig->loadvalue;

		/* Sets to 1 at the end of the timer period. Writing 1 to this flag clears it. */
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
		
		if (TimConfig->INT_Func == PIT_INT_ENABLE)
		{
			/* Enable PIT timer interrupt */
			PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;		
		}
		else
		{
			/* Disable PIT timer interrupt */
			PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TIE_MASK;
		}
	}
	else if (TimConfig->channel == PIT_CH1)
	{
		/* Disable Timer0 chain mode. Attention: Timer0 can not be chained. Because KEAZ128 chip
		   just have two channels about PIT timer, so PIT channel1 can not configure as chain mode.
		*/
		PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_CHN_MASK;

		/* Load the time period value */
		PIT->CHANNEL[1].LDVAL = TimConfig->loadvalue;

		/* Sets to 1 at the end of the timer period. Writing 1 to this flag clears it. */
		PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
		
		if (TimConfig->INT_Func == PIT_INT_ENABLE)
		{
			/* Enable PIT timer interrupt */
			PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;		
		}
		else
		{
			/* Disable PIT timer interrupt */
			PIT->CHANNEL[1].TCTRL &= ~PIT_TCTRL_TIE_MASK;
		}
	}

	/* All setup is done,then enable clock for PIT timers. */
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;

	return 0;
}



/**
 * @brief   Enable specified PIT timer channel.
 * @param   ch, The specified channel number.
 * @returns  None.
 */
void PIT_EnableModule(uint8_t ch)
{
	PIT->CHANNEL[ch].TCTRL |= PIT_TCTRL_TEN_MASK;
}



/**
 * @brief   Disable specified PIT timer channel.
 * @param   ch, The specified channel number.
 * @returns  None.
 */
void PIT_DisableModule(uint8_t ch)
{
	PIT->CHANNEL[ch].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

/*****************************END OF FILE**************************************/