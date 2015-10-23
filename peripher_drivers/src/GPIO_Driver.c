/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: GPIO_Driver.c
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

/* Includes ------------------------------------------------------------------*/

#include "GPIO_Driver.h"


/*****************************************************************************//*!
* @brief    Initialize GPIO pins which are specified by u32PinMask
*        
* @param[in] pGPIO      Pointer to GPIO module, can be GPIOA/GPIOB.
* @param[in] u32PinMask GPIO pin mask need to be set
* @param[in] sGpioType  pin attribute 
*
* @return   none
*
* @Note
*   . High-current drive function is disabled, if the pin is configured as an input.
*   . Internal pullup is disabled if the pin is configured as an output
*   . u32PinMask parameter must be matched with pGPIO parameter.Users should check 
*     over GPIO_Driver.h file to find enumeration.
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_Init(GPIO_Type *pGPIO, GPIO_PinMaskType u32PinMask, GPIO_PinConfigType sGpioType)
{
    /* Config GPIO for Input or Output */
    if ((sGpioType == GPIO_PinOutput) || (sGpioType == GPIO_PinOutput_HighCurrent))
    {
        pGPIO->PDDR |= u32PinMask;      /* Enable Port Data Direction Register */
        pGPIO->PIDR |= u32PinMask;      /* Set Port Input Disable Register */   
    }
    else if ((sGpioType == GPIO_PinInput) || (sGpioType == GPIO_PinInput_InternalPullup))
    {
        pGPIO->PDDR &= ~u32PinMask;   /* Disable Port Data Direction Register */
        pGPIO->PIDR &= ~u32PinMask;   /* Clear Port Input Disable Register */
    }
	
	/* Config PORT Pull select for GPIO */
    switch((uint32_t)pGPIO)
    {
        case GPIOA_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUE0 |= u32PinMask):(PORT->PUE0 &= ~u32PinMask);
            break;
        case GPIOB_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUE1 |= u32PinMask):(PORT->PUE1 &= ~u32PinMask);
            break;
        case GPIOC_BASE:
            (sGpioType == GPIO_PinInput_InternalPullup)?(PORT->PUE2 |= u32PinMask):(PORT->PUE2 &= ~u32PinMask);
            break;
        default:
            break;
     }
	
	/* Config PORT GPIO_PinOutput_HighCurrent for GPIO */
    if (pGPIO == GPIOA)
    {
        if (u32PinMask & GPIO_PTB4_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTB4_MASK;
        }   
        if (u32PinMask & GPIO_PTB5_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
        }   
        if (u32PinMask & GPIO_PTD0_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTD0_MASK;
        }   
        if (u32PinMask & GPIO_PTD1_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTD1_MASK;
        }   
    }
    else if (pGPIO == GPIOB)
    {
        if (u32PinMask & GPIO_PTE0_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTE0_MASK;
        }   
        if (u32PinMask & GPIO_PTE1_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTE1_MASK;
        }   
        if (u32PinMask & GPIO_PTH0_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTH0_MASK;
        }   
        if (u32PinMask & GPIO_PTH1_MASK)
        {   
            PORT->HDRVE |= PORT_HDRVE_PTH1_MASK;
        }   
    }
}



/*****************************************************************************//*!
* @brief    Toggle the pins which are specified by u32PinMask
*        
* @param[in] pGPIO       Pointer to GPIO module, can be GPIOA/GPIOB.
* @param[in] u32PinMask  Specify GPIO pin need to be toggled
*
* @return  None.
* @note
*          u32PinMask parameter must be matched with pGPIO parameter.Users should check 
*          over GPIO_Driver.h file to find enumeration.
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_Toggle(GPIO_Type *pGPIO, GPIO_PinMaskType u32PinMask)
{
	pGPIO->PTOR = u32PinMask;   /* Toggle the pins specified by u32PinMask */
}



/*****************************************************************************//*!
* @brief Read input data from GPIO which is specified by pGPIO
*        
* @param[in] pGPIO  Pointer to GPIO module, can be GPIOA/GPIOB.
*
* @return   GPIO input value unsigned int 32-bit
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
uint32_t GPIO_Read(GPIO_Type *pGPIO)
{
	return (pGPIO->PDIR);				/* Read Port Data Input Register */
}



/*****************************************************************************//*!
* @brief    Write output data to GPIO which is specified by pGPIO
*        
* @param[in] pGPIO       Pointer to GPIO module, can be GPIOA/GPIOB.
* @param[in] u32Value    value to output
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_Write(GPIO_Type *pGPIO, uint32_t u32Value)
{
	pGPIO->PDOR = u32Value;    /* Write Port Ouput Data Register */
}



/*****************************************************************************//*!
* @brief    Initialize GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin        GPIO pin name, can be GPIO_PTA0,1 ...
* @param[in] GPIO_PinConfig  Config output or input
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinInit(GPIO_PinType GPIO_Pin, GPIO_PinConfigType GPIO_PinConfig)
{
	if (GPIO_Pin < GPIO_PTE0)
	{
	  switch (GPIO_PinConfig)
	  {
	  case GPIO_PinOutput:
		  GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
		  GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
		  PORT->PUE0 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  case GPIO_PinInput:
		  GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
		  GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
		  PORT->PUE0 &= ~(1<<GPIO_Pin);    	 /* Disable Pullup */
		break;
	  case GPIO_PinInput_InternalPullup:
		  GPIOA->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
		  GPIOA->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
		  PORT->PUE0 |= (1<<GPIO_Pin);       /* Enable Pullup */
		break;
	  case GPIO_PinOutput_HighCurrent:
		  GPIOA->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
		  GPIOA->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
		  PORT->PUE0 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  }
	}
	else if (GPIO_Pin < GPIO_PTI0)
	{
	  GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 32);
	  switch (GPIO_PinConfig)
	  {
	  case GPIO_PinOutput:
		  GPIOB->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
		  GPIOB->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
		  PORT->PUE1 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  case GPIO_PinInput:
		  GPIOB->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
		  GPIOB->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
		  PORT->PUE1 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  case GPIO_PinInput_InternalPullup:
		  GPIOB->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
		  GPIOB->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
		  PORT->PUE1 |= (1<<GPIO_Pin);       /* Enable Pullup */
		break;
	  case GPIO_PinOutput_HighCurrent:
		  GPIOB->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
		  GPIOB->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
		  PORT->PUE1 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  }
	}
	else
	{
	  GPIO_Pin = (GPIO_PinType)(GPIO_Pin - 64);
	  switch (GPIO_PinConfig)
	  {
	  case GPIO_PinOutput:
		  GPIOC->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
		  GPIOC->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
		  PORT->PUE2 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  case GPIO_PinInput:
		  GPIOC->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
		  GPIOC->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
		  PORT->PUE2 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  case GPIO_PinInput_InternalPullup:
		  GPIOC->PDDR &= ~(1<<GPIO_Pin);     /* Disable Port Data Direction Register */
		  GPIOC->PIDR &= ~(1<<GPIO_Pin);     /* Clear Port Input Disable Register */
		  PORT->PUE2 |= (1<<GPIO_Pin);       /* Enable Pullup */
		break;
	  case GPIO_PinOutput_HighCurrent:
		  GPIOC->PDDR |= (1<<GPIO_Pin);      /* Enable Port Data Direction Register */
		  GPIOC->PIDR |= (1<<GPIO_Pin);      /* Set Port Input Disable Register */
		  PORT->PUE2 &= ~(1<<GPIO_Pin);      /* Disable Pullup */
		break;
	  }
	}
	
    /* Config GPIO HDRV */
    if(GPIO_PinConfig == GPIO_PinOutput_HighCurrent)
    {
        switch (GPIO_Pin)
        {
            case GPIO_PTB4:
                PORT->HDRVE |= PORT_HDRVE_PTB4_MASK;
                break;
            case GPIO_PTB5:
                PORT->HDRVE |= PORT_HDRVE_PTB5_MASK;
                break;
            case GPIO_PTD0:
                PORT->HDRVE |= PORT_HDRVE_PTD0_MASK;
                break;
            case GPIO_PTD1:
                PORT->HDRVE |= PORT_HDRVE_PTD1_MASK;
                break;
            case GPIO_PTE0:
                PORT->HDRVE |= PORT_HDRVE_PTE0_MASK;
                break;
            case GPIO_PTE1:
                PORT->HDRVE |= PORT_HDRVE_PTE1_MASK;
                break;
            case GPIO_PTH0:
                PORT->HDRVE |= PORT_HDRVE_PTH0_MASK;
                break;
            case GPIO_PTH1:
                PORT->HDRVE |= PORT_HDRVE_PTH1_MASK;
                break;
            default:
                break; 
        }
    }
}



/*****************************************************************************//*!
* @brief    Read GPIO single pin input value.
*        
* @param[in] GPIO_Pin,  GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
uint8_t GPIO_PinRead(GPIO_PinType GPIO_Pin)
{
	uint8_t input_val;
	
	if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */		
		input_val = (uint8_t)(GPIOA->PDIR >> GPIO_Pin) & 0x01u;
    }	

#if (defined(CPU_KEA8) | defined(CPU_KEA128))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);

		input_val = (uint8_t)(GPIOB->PDIR >> GPIO_Pin) & 0x01u;
    }
#endif

#if defined(CPU_KEA128)  
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
	
		input_val = (uint8_t)(GPIOC->PDIR >> GPIO_Pin) & 0x01u;
    }
#endif
	
	return (input_val);
}



/*****************************************************************************//*!
* @brief    Toggle GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin,  GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinToggle(GPIO_PinType GPIO_Pin)
{
    if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */
        GPIOA->PTOR = (1<<GPIO_Pin);
    }

#if (defined(CPU_KEA8) | defined(CPU_KEA128))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PTOR = (1<<GPIO_Pin);
    }
#endif

#if defined(CPU_KEA128)  
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
        GPIOC->PTOR = (1<<GPIO_Pin);
    }
#endif
}



/*****************************************************************************//*!
* @brief    Set GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin,  GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinSet(GPIO_PinType GPIO_Pin)
{
    if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */
        GPIOA->PSOR = (1<<GPIO_Pin);
    }

#if (defined(CPU_KEA8) | defined(CPU_KEA128))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PSOR = (1<<GPIO_Pin);
    }
#endif

#if defined(CPU_KEA128) 
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
        GPIOC->PSOR = (1<<GPIO_Pin);
    }
#endif
}



/*****************************************************************************//*!
* @brief    Clear GPIO single pin which is specified by GPIO_Pin
*        
* @param[in] GPIO_Pin,  GPIO pin name, can be GPIO_PTA0,1 ...
*
* @return   none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void GPIO_PinClear(GPIO_PinType GPIO_Pin)
{
    if (GPIO_Pin < GPIO_PTE0)
    {
        /* PTA0-7, PTB0-7, PTC0-7, PTD0-7 */
        GPIOA->PCOR = (1<<GPIO_Pin);
    }

#if (defined(CPU_KEA8) | defined(CPU_KEA128))

    else if (GPIO_Pin < GPIO_PTI0)
    {
        /* PTE0-7, PTF0-7, PTH0-7, PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTE0);
        GPIOB->PCOR = (1<<GPIO_Pin);
    }
#endif

#if defined(CPU_KEA128)
    else if(GPIO_Pin < GPIO_PIN_MAX)
    {
        /* PTI0-7 */
        GPIO_Pin = (GPIO_PinType)(GPIO_Pin - GPIO_PTI0);
        GPIOC->PCOR = (1<<GPIO_Pin);
    }
#endif
}

/*****************************END OF FILE**************************************/