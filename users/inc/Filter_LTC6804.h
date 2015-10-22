/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: SPI_Driver.c
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions to help users to use KEA serious 
  *                MCU SPI module.This functions include initializing SPI ports,
  *                reading and writing data by SPI ports.
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

/* Includes ------------------------------------------------------------------*/

#ifndef  __FILTER_LTC6804_H
#define  __FILTER_LTC6804_H



#define   VOL_BUF_LENGTH    10
#define   TEMP_BUF_LENGTH   10

#define   VOL_UPPER_LIMIT   5000
#define   VOL_LOWER_LIMIT   1500

#define   TEMP_UPPER_LIMIT   125
#define   TEMP_LOWER_LIMIT   20


extern uint8_t  Temp_Error_Count[8];

extern uint16_t Filter_Vol_First(uint8_t Num,uint16_t Vol);
extern uint16_t Filter_Vol_Second(uint8_t Num,uint16_t Vol);
extern uint8_t Filter_Temp_First(uint8_t Num,uint8_t Temp);
extern uint8_t Filter_Temp_Second(uint8_t Num,uint8_t Temp);
extern void Filter_Prevent_Vol_Jump(uint16_t * NewVol,uint16_t * LastVol);
extern void Filter_Prevent_Temp_Jump(uint16_t * NewTemp,uint16_t * LastTemp);


#endif