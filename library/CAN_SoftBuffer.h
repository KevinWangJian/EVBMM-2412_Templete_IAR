/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: CAN_SoftBuffer.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of reading and writting soft CAN send and receive
  *                buffers functions.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    22-Nov-2015

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


#ifndef  __CAN_SOFTBUFFER_H
#define  __CAN_SOFTBUFFER_H


#include "MSCAN_Driver.h"



/* Exported types ------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Exported functions ------------------------------------------------------- */
	
	
extern int Fill_CANReceiveBuffer(MSCAN_Message_TypeDef* CAN_WMessage);


extern int Check_CANReceiveBuffer(MSCAN_Message_TypeDef* CAN_RMessage);


extern int Fill_CANSendBuffer(MSCAN_Message_TypeDef* CAN_WMessage);


extern int Check_CANSendBuffer(MSCAN_Message_TypeDef* CAN_RMessage);


#ifdef __cplusplus
}
#endif

#endif