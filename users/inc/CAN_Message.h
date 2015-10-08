/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: CAN_Message.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of reading and writting soft CAN send and receive
  *                buffers functions.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    07-Nov-2015

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


#ifndef  __CAN_MESSAGE_H
#define  __CAN_MESSAGE_H

#include <stdint.h>
#include <stdlib.h>

#include "arm_cm0.h"
#include "SKEAZ1284.h"
#include "MSCAN_Driver.h"



/* Exported types ------------------------------------------------------------*/

#define   CAN_MESSAGE_VERSION         (100)   /* Version1.0.0 */

/* Global intranet receive buffer size definition which will be used by CPU core */
#define   INTRANET_RECEIVEBUF_SIZE    (40)

/* Global intranet receive buffer size definition which will just be used by CPU core */
#define   INTRANET_SENDBUF_SIZE       (30)





typedef struct 
{
    uint8_t Intranet_SendBuff_WPointer;
	
    uint8_t Intranet_SendBuff_RPointer;
    
    MSCAN_Message_TypeDef Intranet_SendBuff[INTRANET_SENDBUF_SIZE]; 
	
}CANSendMessagebuffer_TypeDef;




typedef struct 
{
    uint8_t Intranet_RecBuff_WPointer;
	
    uint8_t Intranet_RecBuff_RPointer;
    
    MSCAN_Message_TypeDef Intranet_RecBuf[INTRANET_RECEIVEBUF_SIZE];
    
}CANReceiveMessageBuffer_TypeDef;







#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */
	
int32_t Fill_CANReceiveBuffer(MSCAN_Message_TypeDef* CAN_WMessage);


int32_t Check_CANReceiveBuffer(MSCAN_Message_TypeDef* CAN_RMessage);


int32_t Fill_CANSendBuffer(MSCAN_Message_TypeDef* CAN_WMessage);


int32_t Check_CANSendBuffer(MSCAN_Message_TypeDef* CAN_RMessage);




#ifdef __cplusplus
}
#endif

#endif

/*****************************END OF FILE**************************************/