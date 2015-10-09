/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: KBI_Driver.c
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

/* Includes ------------------------------------------------------------------*/

#include "arm_cm0.h"
#include "KBI_Driver.h"



/**
 * @brief   Initialze KBI module by a given properties.
 * @param   *KBIx, specified KBI module number.
 *          *KBIx_Config, data buffer which store KBI module properties parameter.
 * @returns 0: Calling succeeded.
 * 			-1: Calling failed.
 */
int32_t KBI_Init(KBI_Type* KBIx, KBI_Configuration_TypeDef* KBIx_Config)
{
	if ((KBIx != KBI0) && (KBIx != KBI1))return -1;
	
	if (NULL == KBIx_Config)return -1;
	
	PORT->IOFLT0 |= PORT_IOFLT0_FLTKBI0_MASK;
	
	if (KBIx == KBI0)
	{
		SIM->SCGC |= SIM_SCGC_KBI0_MASK;
		
		/* KBI interrupt disabled first. */
		KBI0->SC &= ~KBI_SC_KBIE_MASK;
		
		if (KBIx_Config->Trigger_Mode == FallingEdge_LowLevel)
		{
			KBI0->ES &= ~KBI_ES_KBEDG_MASK;
		}
		else if (KBIx_Config->Trigger_Mode == RisingEdge_HighLevel)
		{
			KBI0->ES |= KBI_ES_KBEDG_MASK;
		}
		else 
		{
			return -1;
		}
		
		/* Enable the specified KBI pins. */
		KBI0->PE &= ~KBI_PE_KBIPE_MASK;
		KBI0->PE |= KBIx_Config->Pin_Num;
		
		/* Writing a 1 to KBACK is part of the flag clearing mechanism. */
		KBI0->SC |= KBI_SC_KBACK_MASK;
		
		/* Writing a 1 to RSTKBSP is to clear the KBIxSP Register. This bit always reads as 0. */
		KBI0->SC |= KBI_SC_RSTKBSP_MASK;
		
		/* The latched value in KBxSP register while interrupt flag occur to be read. */
		KBI0->SC |= KBI_SC_KBSPEN_MASK;
		
		if (KBIx_Config->Detection_Mode == 0)
		{
			/* Keyboard detects edges only. */
			KBI0->SC &= ~KBI_SC_KBMOD_MASK;
		}
		else
		{
			/* Keyboard detects both edges and levels. */
			KBI0->SC |= KBI_SC_KBMOD_MASK;
		}
		
		if (KBIx_Config->KBI_Interrupt_Enable != 0)
		{
			/* KBI interrupt enabled. */
			KBI0->SC |= KBI_SC_KBIE_MASK;		
		}
	}
	else if (KBIx == KBI1)
	{
		SIM->SCGC |= SIM_SCGC_KBI1_MASK;
		
		/* KBI interrupt disabled first. */
		KBI1->SC &= ~KBI_SC_KBIE_MASK;
		
		if (KBIx_Config->Trigger_Mode == FallingEdge_LowLevel)
		{
			KBI1->ES &= ~KBI_ES_KBEDG_MASK;
		}
		else if (KBIx_Config->Trigger_Mode == RisingEdge_HighLevel)
		{
			KBI1->ES |= KBI_ES_KBEDG_MASK;
		}
		else 
		{
			return -1;
		}
		
		/* Enable the specified KBI pins. */
		KBI1->PE &= ~KBI_PE_KBIPE_MASK;
		KBI1->PE |= KBIx_Config->Pin_Num;
		
		/* Writing a 1 to KBACK is part of the flag clearing mechanism. */
		KBI1->SC |= KBI_SC_KBACK_MASK;
		
		/* Writing a 1 to RSTKBSP is to clear the KBIxSP Register. This bit always reads as 0. */
		KBI1->SC |= KBI_SC_RSTKBSP_MASK;
		
		/* The latched value in KBxSP register while interrupt flag occur to be read. */
		KBI1->SC |= KBI_SC_KBSPEN_MASK;
		
		if (KBIx_Config->Detection_Mode == 0)
		{
			/* Keyboard detects edges only. */
			KBI1->SC &= ~KBI_SC_KBMOD_MASK;
		}
		else
		{
			/* Keyboard detects both edges and levels. */
			KBI1->SC |= KBI_SC_KBMOD_MASK;
		}
		
		if (KBIx_Config->KBI_Interrupt_Enable != 0)
		{
			/* KBI interrupt enabled. */
			KBI1->SC |= KBI_SC_KBIE_MASK;		
		}		
	}
	
	return 0;
}

/*****************************END OF FILE**************************************/