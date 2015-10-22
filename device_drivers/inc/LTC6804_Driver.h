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

#ifndef  __LTC6804_DRIVER_H
#define  __LTC6804_DRIVER_H

#include "SPI_Driver.h"

/* The max number of LTC6804 IC. */
#define   TOTAL_IC   2

#define     LTC6804NUM            2                  //LTC6804芯片个数
#define     LTC6804CELL1          12                 //第一片LTC6804连接电池数
#define     LTC6804CELL2          12                  //第二片LTC6804连接电池数
#define     DCELLNUM              (LTC6804CELL1+LTC6804CELL2)  //连接电池总数

#define     LTC6804TEMP1          4                 //第一片LTC6804连接温度个数
#define     LTC6804TEMP2          4                  //第二片LTC6804连接温度个数
#define     DTEMPNUM              (LTC6804TEMP1+LTC6804TEMP2)  //连接温度总数


extern uint8_t       g_LTC6804_Vol_Read_Finish_Flag;            /* 电压读取完成完成标志 */
extern uint8_t       g_LTC6804_Vol_Conversion_Start_Flag;         /* 启动电压转换标志 */
extern uint8_t       g_LTC6804_Temp_Read_Finish_Flag;          /* 温度读取完成完成标志 */
extern uint8_t       g_LTC6804_Temp_Conversion_Start_Flag;           /* 启动温度转换标志 */
extern uint8_t       g_LTC6804_Time_Count;                                         /* 转换过程中定时器计数 */
extern uint8_t       g_LTC6804_Conversion_Finish_Flag;               /* LTC6804转换完成标志 */


extern uint8_t Write_ConfigRegister[2][6];
extern uint8_t g_Write_ConfigRegister_buff[2][6];

/* Read register raw data command */
typedef enum
{
    LTC6804_RDCVALL = 0,
    LTC6804_RDCVA = 1,
    LTC6804_RDCVB,
    LTC6804_RDCVC,
    LTC6804_RDCVD,
    LTC6804_RDAUXALL,
    LTC6804_RDAUXA,
    LTC6804_RDAUXB,
    LTC6804_RDSTATALL,
    LTC6804_RDSTATA,
    LTC6804_RDSTATB,
}LTC6804_ReadRegister_Command;


/* Clear register raw data command */
typedef enum{
    LTC6804_CLRCELL = 1,
    LTC6804_CLRAUX,
    LTC6804_CLRSTAT,
}LTC6804_ClearRegister_Command;

/*
  |MD| Dec  | ADC Conversion Model|
  |--|------|---------------------|
  |01| 1    | Fast                |
  |10| 2    | Normal              |
  |11| 3    | Filtered            |
*/
typedef enum {
    MD_FAST = 1,
    MD_NORMAL,
    MD_FILTERED,
}LTC6804_ADC_Mode;


/*
 |CH | Dec  | Channels to convert |
 |---|------|---------------------|
 |000| 0    | All Cells           |
 |001| 1    | Cell 1 and Cell 7   |
 |010| 2    | Cell 2 and Cell 8   |
 |011| 3    | Cell 3 and Cell 9   |
 |100| 4    | Cell 4 and Cell 10  |
 |101| 5    | Cell 5 and Cell 11  |
 |110| 6    | Cell 6 and Cell 12  |
*/
typedef enum {
    CELL_CH_ALL = 0,
    CELL_CH_1and7,
    CELL_CH_2and8,
    CELL_CH_3and9,
    CELL_CH_4and10,
    CELL_CH_5and11,
    CELL_CH_6and12,
}LTC6804_CellSelection_CH;



/*
  |CHG | Dec  |Channels to convert   |
  |----|------|----------------------|
  |000 | 0    | All GPIOS and 2nd Ref|
  |001 | 1    | GPIO 1               |
  |010 | 2    | GPIO 2               |
  |011 | 3    | GPIO 3               |
  |100 | 4    | GPIO 4               |
  |101 | 5    | GPIO 5               |
  |110 | 6    | Vref2                |
*/
typedef enum{
    AUX_CH_GPIOxVREF2 = 0,
    AUX_CH_GPIO1,
    AUX_CH_GPIO2,
    AUX_CH_GPIO3,
    AUX_CH_GPIO4,
    AUX_CH_GPIO5,
    AUX_CH_VREF2,
}LTC6804_GPIOSelection_CH;


// CHG = 0; //!< aux channels to be converted
 /*****************************************************
  \brief Controls if Discharging transitors are enabled
  or disabled during Cell conversions.

 |DCP | Discharge Permitted During conversion |
 |----|----------------------------------------|
 |0   | No - discharge is not permitted         |
 |1   | Yes - discharge is permitted           |
 
********************************************************/
typedef enum {
    DCP_DISABLED = 0,
    DCP_ENABLED,
}LTC6804_DischargeEnable;


/* 
Status Group Selection 
000   Status group ADC Conversion SOC,ITMP,VA,VD
001   Status group ADC Conversion SOC
010   Status group ADC Conversion ITMP
011   Status group ADC Conversion VA
100   Status group ADC Conversion VD
*/
typedef enum{
    CHST_ALL = 0,
    CHST_SOC,
    CHST_ITMP,
    CHST_VA,
    CHST_VD,
}LTC6804_StatusGroupSelection;



/*采集数据*/
typedef struct 
{
      uint16_t Vol[DCELLNUM];
      
      uint8_t Vol_Flag[DCELLNUM];
      
      uint16_t Vol_Buff[DCELLNUM];
      
      uint8_t CellTemp[DTEMPNUM];
      
      uint8_t CellTemp_Flag[DTEMPNUM];
      
     uint8_t CellTemp_Buff[DTEMPNUM]; 
}CollectData;


/*系统参数结构体*/
typedef struct 
{
    uint8_t   Device_Address;             /*设备地址*/
    uint8_t   CellNum;                    /*电池节数*/
    uint8_t   LTC6804Num;                 /*LTC6804片数*/
    uint8_t   LTC6804_CellNum[2];         /*2片LTC6804连接电池数*/
    uint8_t   TempNum;                    /*温度个数*/
    uint16_t  Over_Vol_Threshold;                   /*过压门限*/
    uint16_t  Under_Vol_Threshold;                  /*欠压门限*/
}ParaStruct;  


extern CollectData  Collect_Data;
extern ParaStruct Para_Struct;         // 系统参数结构体



extern void LTC6804_WakeUp(void);
extern void LTC6820_SPIx_Config_Initial(SPIx_PinsRemap  SPIx_Type);

extern uint32_t LTC6804_ADCV(LTC6804_ADC_Mode md, LTC6804_DischargeEnable dcp, LTC6804_CellSelection_CH ch);
extern uint32_t LTC6804_ADAX(LTC6804_ADC_Mode md, LTC6804_GPIOSelection_CH chg);
extern int LTC6804_RDCV(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t cell_codes[LTC6804NUM][LTC6804CELL1]);
extern uint32_t LTC6804_RDAUX(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t aux_codes[][6]);
extern int LTC6804_ClearRawData_Reg(LTC6804_ClearRegister_Command clr_cmd);
extern int LTC6804_ADSTAT(LTC6804_ADC_Mode md, LTC6804_StatusGroupSelection chst);
extern uint32_t LTC6804_RDCFG(uint8_t total_ic, uint8_t r_config[][6]);
extern void LTC6804_WRCFG(uint8_t total_ic, uint8_t w_config[][6]);
extern void LTC6820_EN_Clear(GPIO_PinType GPIO_Pin);
extern void LTC6820_EN_Set(GPIO_PinType GPIO_Pin);
extern void LTC6820_CS_Set(GPIO_PinType GPIO_Pin);
extern void LTC6820_CS_Clear(GPIO_PinType GPIO_Pin);
extern void Vol_Temp_Conversion_And_Read(void);
extern void LTC6804_Vol_Allocation(void);
extern int16_t ResToTempNTC(float ResCalValue);
extern void LTC6804_Vol_First_Conversion(void);
extern uint32_t LTC6804_RDSTAT(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t aux_codes[][6]);

#endif