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
#include <stdint.h>
#include <math.h>

#include "SPI_Driver.h"
#include "GPIO_Driver.h"
#include "LTC6804_Driver.h"
#include "Delay.h"
#include "Filter_LTC6804.h"




/*  函数声明   */

static uint16_t LTC6804_Pec15_Calc(uint8_t len, uint8_t *data);
static void LTC6820_SPI_WriteNByteData(uint8_t len, uint8_t *data);
static void LTC6820_SPI_ReadNByteData(uint8_t len, uint8_t *data);
static int16_t LTC6804_ResToTempNTC(float ResCalValue);
static void LTC6804_Vol_Test(void);
static void LTC6804_Temp_Test(void);




CollectData  Collect_Data;
ParaStruct Para_Struct;         // 系统参数结构体
uint8_t       g_LTC6804_Vol_Read_Finish_Flag;            /* 电压读取完成完成标志 */
uint8_t       g_LTC6804_Vol_Conversion_Start_Flag;         /* 启动电压转换标志 */
uint8_t       g_LTC6804_Temp_Read_Finish_Flag;          /* 温度读取完成完成标志 */
uint8_t       g_LTC6804_Temp_Conversion_Start_Flag;           /* 启动温度转换标志 */
uint8_t       g_LTC6804_Time_Count;                                         /* 转换过程中定时器计数 */
uint8_t       g_LTC6804_Conversion_Finish_Flag;               /* LTC6804转换完成标志 */

uint8_t CellTemp_Buff_Zengbo[12];

static uint16_t g_LTC6804_STAR_Data[2][6];        /*状态寄存器B数据*/

float  R_NTC_zengbo;


static uint16_t     g_LTC6804_original_vol_buff[LTC6804NUM][LTC6804CELL1];                            /* LTC6804原始电压依据分配原则数组从新排列 */
static uint16_t     g_LTC6804_original_temp[LTC6804NUM][6];                            /* LTC6804原始温度 */

uint8_t     g_LTC6804_original_temp_buff[LTC6804NUM][6]; 
static uint16_t     g_LTC6804_original_vol[LTC6804NUM][LTC6804CELL1];                            /* LTC6804原始电压 */

static const uint16_t crc15Table[256] = 
{
0x0,0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  
0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9, 
0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
};


#if 0
/*NTC阻值温度转换表,100K*/
const float TableNTC_100K[191]=
{                 
4123.445, 3846.898, 3590.500, 3352.675, 3131.979, 2927.087, 2736.784, 2559.953, 2395.568, 2242.688,                 //-30~-21
2100.446, 1968.046, 1844.753, 1729.894, 1622.847, 1523.039, 1429.942, 1343.069, 1261.972, 1186.235,                 //-20~-11
1115.477, 1049.344, 987.508, 929.669, 875.547, 824.883, 777.438, 732.992, 691.338, 652.287,                 //-10~-1
615.662, 581.301, 549.051, 518.771, 490.332, 463.611, 438.497, 414.883, 392.673, 371.776,                 //0~9
352.107, 333.588, 316.146, 299.713, 284.225, 269.624, 255.852, 242.861, 230.600, 219.026,                 //10~19
208.096, 197.771, 188.016, 178.794, 170.076, 161.829, 154.028, 146.645, 139.655, 133.037,                 //20~29
126.768, 120.828, 115.199, 109.862, 104.800, 100.000, 95.444, 91.121, 87.016, 83.118,                 //30~39
79.415, 75.897, 72.553, 69.375, 66.352, 63.477, 60.741, 58.138, 55.660, 53.300,                 //40~49
51.053, 48.912, 46.872, 44.928, 43.074, 41.307, 39.621, 38.012, 36.477, 35.012,                 //50~59
33.614, 32.278, 31.002, 29.783, 28.618, 27.505, 26.441, 25.423, 24.449, 23.518,                 //60~69
22.627, 21.774, 20.957, 20.175, 19.426, 18.709, 18.022, 17.363, 16.732, 16.127,                //70~79
15.546, 14.990, 14.456, 13.943, 13.452, 12.980, 12.527, 12.092, 11.674, 11.272,                 //80~89
10.887, 10.516, 10.160, 9.817, 9.488, 9.172, 8.867, 8.574, 8.292, 8.021,                //90~99
7.760, 7.508, 7.266, 7.033, 6.808, 6.592, 6.384, 6.183, 5.989, 5.803,       //100~109
5.623, 5.449, 5.282, 5.120, 4.964, 4.814, 4.669, 4.529, 4.394, 4.263,       //110~119
4.137, 4.015, 3.897, 3.784, 3.674, 3.568, 3.465, 3.366, 3.270, 3.177,                                         //120~125
3.088, 3.002, 2.918, 2.836, 2.758, 2.682, 2.608, 2.537, 2.468, 2.401,
2.336, 2.274, 2.213, 2.154, 2.097, 2.042, 1.988, 1.936, 1.886, 1.837,
1.789, 1.744, 1.699, 1.656, 1.614, 1.573, 1.534, 1.495, 1.458, 1.422,1.387
};
#endif

/*NTC阻值温度转换表,10K*/
const float TableNTC_10K[161]={                 
118.8, 112.5, 106.6, 101.1, 96.02, 91.21, 86.70, 82.44, 78.41, 74.61,                 //-30~-21
71.01, 67.60, 64.36, 61.29, 58.39, 55.63, 53.01, 50.52, 48.16, 45.92,                 //-20~-11
43.80, 41.78, 39.87, 38.06, 36.33, 34.70, 33.15, 31.67, 30.28, 28.95,                 //-10~-1
27.69, 26.49, 25.35, 24.27, 23.24, 22.26, 21.33, 20.44, 19.60, 18.80,                 //0~9
18.03, 17.31, 16.61, 15.95, 15.32, 14.72, 14.14, 13.60, 13.07, 12.57,                 //10~19
12.09, 11.64, 11.20, 10.78, 10.38, 10.00, 9.633, 9.282, 8.946, 8.623,                 //20~29
8.314, 8.018, 7.734, 7.461, 7.199, 6.948, 6.707, 6.475, 6.253, 6.039,                 //30~39
5.834, 5.636, 5.447, 5.264, 5.089, 4.920, 4.757, 4.601, 4.451, 4.306,                 //40~49
4.167, 4.033, 3.903, 3.779, 3.659, 3.543, 3.432, 3.325, 3.221, 3.121,                 //50~59
3.025, 2.932, 2.843, 2.757, 2.673, 2.593, 2.515, 2.440, 2.368, 2.298,                 //60~69
2.231, 2.166, 2.103, 2.042, 1.983, 1.926, 1.871, 1.818, 1.767, 1.717,                 //70~79
1.669, 1.623, 1.578, 1.534, 1.492, 1.451, 1.412, 1.374, 1.336, 1.301,                 //80~89
1.266, 1.232, 1.199, 1.167, 1.137, 1.107, 1.078, 1.050, 1.023, 0.9961,                //90~99
0.9704, 0.9454, 0.9212, 0.8976, 0.8747, 0.8525, 0.8309, 0.8099, 0.7894, 0.7695,       //100~109
0.7502, 0.7314, 0.7131, 0.6953, 0.6780, 0.6611, 0.6447, 0.6286, 0.6131, 0.5979,       //110~119
0.5831, 0.5686, 0.5546, 0.5409,0.5275,0.5145                                          //120~125
};



uint8_t g_Write_ConfigRegister[2][6] =
{
	{0xfc, 0x19, 0xb6, 0x88, 0x00, 0x00},
	{0xfc, 0x19, 0xb6, 0x88, 0x00, 0x00},
};

/*uint8_t g_Write_ConfigRegister[2][6] =
{
	{0x04, 0x19, 0xb6, 0x88, 0x00, 0x00},
	{0x04, 0x19, 0xb6, 0x88, 0x00, 0x00},
};*/

uint8_t g_Write_ConfigRegister_buff[2][6];


/*!****************************************************
 * @brief: Wake Up serial interface.
 * @param   None
 * @returns None 
 *****************************************************/

//----------------------------------------------------------------------------
// Function      : void LTC6804_WakeUp(void)                               |
// Description   : 唤醒总线                                                |
// Input         : 无                                                      |
// Output        : 无                                                      |
// Others        : 每次通讯前需要唤醒总线                                   |
//----------------------------------------------------------------------------

void LTC6804_WakeUp(void)
{
    uint8_t i;

    LTC6820_CS_Clear(GPIO_PTG7);
    
    (void)SPI_WriteByteData(SPI1, 0x00);
    
    LTC6820_CS_Set(GPIO_PTG7);
    
    for (i = 0;i < 2; i++) 
    {
        Delay10us(100);
    }
}

//----------------------------------------------------------------------------
// Function      : void LTC6820_EN_Set(GPIO_PinType GPIO_Pin)                              |
// Description   : LTC6820使能拉高                                               |
// Input         : 使能脚                                                     |
// Output        : 无                                                      |
// Others        : 上电使能脚拉高                                  |
//----------------------------------------------------------------------------

void LTC6820_EN_Set(GPIO_PinType GPIO_Pin)
{
      GPIO_PinInit(GPIO_Pin, GPIO_PinOutput);
      GPIO_PinSet(GPIO_Pin);
}

//----------------------------------------------------------------------------
// Function      : void LTC6820_EN_Clear(GPIO_PinType GPIO_Pin)                             |
// Description   : LTC6820使能拉低                                             |
// Input         : 使能脚                                                     |
// Output        : 无                                                      |
// Others        : 断电前拉低                                 |
//----------------------------------------------------------------------------

void LTC6820_EN_Clear(GPIO_PinType GPIO_Pin)
{
      GPIO_PinInit(GPIO_Pin, GPIO_PinOutput);
      GPIO_PinClear(GPIO_Pin);
}


//----------------------------------------------------------------------------
// Function      : void LTC6820_EN_Clear(GPIO_PinType GPIO_Pin)                             |
// Description   : LTC6820CS拉低                                             |
// Input         : CS脚                                                     |
// Output        : 无                                                      |
// Others        : 断电前拉低                                 |
//----------------------------------------------------------------------------

void LTC6820_CS_Clear(GPIO_PinType GPIO_Pin)
{
      GPIO_PinInit(GPIO_Pin, GPIO_PinOutput);
      GPIO_PinClear(GPIO_Pin);
}

//----------------------------------------------------------------------------
// Function      : void LTC6820_EN_Clear(GPIO_PinType GPIO_Pin)              |
// Description   : LTC6820 CS拉高                                            |
// Input         : CS脚                                                     |
// Output        : 无                                                      |
// Others        : 断电前拉低                                 |
//----------------------------------------------------------------------------

void LTC6820_CS_Set(GPIO_PinType GPIO_Pin)
{
      GPIO_PinInit(GPIO_Pin, GPIO_PinOutput);
      GPIO_PinSet(GPIO_Pin);
}


//----------------------------------------------------------------------------
// Function      : void SPI0_Config_Initial(void)             |
// Description   : 初始化与LTC6820通讯的接口                               |
// Input         : 无                                                    |
// Output        : 无                                                      |
// Others        :                                 |
//----------------------------------------------------------------------------
void LTC6820_SPIx_Config_Initial(SPIx_PinsRemap  SPIx_Type)
{
      SPIx_ConfigType spi0_init;
      spi0_init.SPRF_MODF_INT_Enable = 0;
      spi0_init.Transmit_INT_Enable = 0;
      spi0_init.Match_INT_Enable = 0;
      spi0_init.MasterOrSlaveMode = 1;
      spi0_init.CPOL = 1;
      spi0_init.CPHA = 1;
      spi0_init.LSBFE = 0;
      spi0_init.Signal_Pins = SPIx_Type;    
      
      SPI_Init(SPI1,&spi0_init,High_Speed);
      
      LTC6820_CS_Set(GPIO_PTG7);
 
      LTC6820_EN_Set(GPIO_PTE5);
      
      LTC6804_WRCFG(2,g_Write_ConfigRegister);
      LTC6804_RDCFG(2,g_Write_ConfigRegister_buff);
      
      Para_Struct.TempNum = 8;
      
      Para_Struct.Device_Address = 0x80;
      
      Para_Struct.CellNum = 24;
      
      Para_Struct.LTC6804_CellNum[0] = 12;
      
      Para_Struct.LTC6804_CellNum[1] = 12;
        
      LTC6804_Vol_First_Conversion();
}

//----------------------------------------------------------------------------
// Function      : void SPI_WriteNByteData(uint8_t len, uint8_t *data)       |
// Description   : 写N字节数据                                               |
// Input         : 长度 N字节数据                                            |
// Output        : 无                                                       |
// Others        :                                                          |
//----------------------------------------------------------------------------

static void LTC6820_SPI_WriteNByteData(uint8_t len, uint8_t *data)
{
    uint8_t i;
    
    for (i = 0; i < len; i++)
    {
        SPI_WriteByteData(SPI1, *data++);
    }  
}

//----------------------------------------------------------------------------
// Function      : void SPI_ReadByteData(uint8_t len, uint8_t *data)       |
// Description   : 读N字节数据                                               |
// Input         : 长度 N字节数据                                            |
// Output        : 无                                                       |
// Others        :                                                          |
//----------------------------------------------------------------------------

static void LTC6820_SPI_ReadNByteData(uint8_t len, uint8_t *data)
{
    uint8_t i;
      
    for (i = 0; i < len; i++)
    {
        SPI_ReadByteData(SPI1, 0xaa, data);
        
        data++;
    }
}

//----------------------------------------------------------------------------
// Function      : uint16_t LTC6804_Pec15_Calc(uint8_t len, uint8_t *data)           |
// Description   : pec 校验                                                  |
// Input         : 长度 数据                                                 |
// Output        : 校验数据                                                  |
// Others        :                                                          |
//----------------------------------------------------------------------------

static uint16_t LTC6804_Pec15_Calc(uint8_t len, uint8_t *data) 
{
    uint16_t remainder, addr;
    
    uint8_t i;
    
    /* PEC seed */
    remainder = 16;
    
    for (i = 0; i < len; i++)
    {
        addr = ((remainder >> 7) ^ data[i]) & 0xff;    //calculate PEC table address
        
        remainder = (remainder << 8) ^ crc15Table[addr]; 
    }
    
    /* The CRC15 has a 0 in the LSB so the final remainder must be multiplied by 2 */
    return(remainder*2);
}


//----------------------------------------------------------------------------
// Function      : uint32_t LTC6804_ADCV(LTC6804_ADC_Mode md, LTC6804_DischargeEnable dcp, LTC6804_CellSelection_CH ch)          |
// Description   : 启动电压转换                                                 |
// Input         : ADC模式  均衡放电开关   通道号                                                 |
// Output        :                                                  |
// return        : 0  成功    -1  失败                                                         |
//----------------------------------------------------------------------------

uint32_t LTC6804_ADCV(LTC6804_ADC_Mode md, LTC6804_DischargeEnable dcp, LTC6804_CellSelection_CH ch) 
{
    uint8_t cmd[4];
    uint16_t cmd_pec;
    uint8_t bit;
    
    if ((md <= MD_FILTERED)&&(md >= MD_FAST)) 
    {
        if (dcp <= DCP_ENABLED)
        {
            if (ch <= CELL_CH_6and12)
            {
                bit = (md & 0x02) >> 1;
                cmd[0] = bit + 0x02;
                bit = (md & 0x01) << 7;
                cmd[1] = bit + 0x60 + (dcp << 4) + ch;
                
                cmd_pec = LTC6804_Pec15_Calc(2, cmd);
                
                cmd[2] = (uint8_t)(cmd_pec >> 8);
                cmd[3] = (uint8_t)(cmd_pec);
                
                LTC6804_WakeUp();
            
                LTC6820_CS_Clear(GPIO_PTG7);
                
                LTC6820_SPI_WriteNByteData(4, cmd);
                
                LTC6820_CS_Set(GPIO_PTG7);
                
                return 0;
            }
        }
    }
    
    return -1;
}



//----------------------------------------------------------------------------
// Function      : uint32_t LTC6804_ADAX(LTC6804_ADC_Mode md, LTC6804_GPIOSelection_CH chg)          |
// Description   : 启动温度转换                                                 |
// Input         : ADC模式    通道号                                                 |
// Output        :                                                  |
// return        : 0  成功    -1  失败                                                         |
//----------------------------------------------------------------------------

uint32_t LTC6804_ADAX(LTC6804_ADC_Mode md, LTC6804_GPIOSelection_CH chg) 
{
    uint8_t cmd[4];
    uint16_t cmd_pec;
    uint8_t bit;
    
    if ((md <= MD_FILTERED) && (md >= MD_FAST)) 
    {
        if (chg <= AUX_CH_VREF2) 
        {
            bit = (md & 0x02) >> 1;
            cmd[0] = bit + 0x04;
            
            bit = (md & 0x01) << 7;
            cmd[1] = bit + 0x60 + chg;
            
            cmd_pec = LTC6804_Pec15_Calc(2, cmd);
            
            cmd[2] = (uint8_t)(cmd_pec >> 8);
            cmd[3] = (uint8_t)(cmd_pec);
            
            LTC6804_WakeUp();
            
            LTC6820_CS_Clear(GPIO_PTG7);
    
            LTC6820_SPI_WriteNByteData(4, cmd);
            
            LTC6820_CS_Set(GPIO_PTG7);
            
            return 0;
        }
    }
    
    return -1;
}


//----------------------------------------------------------------------------
// Function      : uint32_t LTC6804_ReadRawData_Reg(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint8_t *r_data)      |
// Description   : 读电压温度等数据                                               |
// Input         : 寄存器组名称   读出数据                                                |
// Output        :                                                  |
// return        : 0  成功    -1  失败                                                         |
//----------------------------------------------------------------------------
uint32_t LTC6804_ReadRawData_Reg(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint8_t *r_data)
{
    //const uint8_t REG_LEN = 8;
    uint8_t cmd[4];
    uint16_t cmd_pec;
    
    if ((rd_cmd <= LTC6804_RDSTATB)&&(rd_cmd >= LTC6804_RDCVA) && (total_ic <= TOTAL_IC)&&(total_ic >= 1) && (r_data != NULL))
    {
        switch (rd_cmd) 
        {
            case LTC6804_RDCVA:{cmd[0] = 0x00;cmd[1] = 0x04;
            }break;
            
            case LTC6804_RDCVB:{cmd[0] = 0x00;cmd[1] = 0x06;
            }break;
            
            case LTC6804_RDCVC:{cmd[0] = 0x00;cmd[1] = 0x08;
            }break;
            
            case LTC6804_RDCVD:{cmd[0] = 0x00;cmd[1] = 0x0A;
            }break;
            
            case LTC6804_RDAUXA:{cmd[0] = 0x00;cmd[1] = 0x0C;
            }break;
            
            case LTC6804_RDAUXB:{cmd[0] = 0x00;cmd[1] = 0x0E;
            }break;
            
            case LTC6804_RDSTATA:{cmd[0] = 0x00;cmd[1] = 0x10;
            }break;
            
            case LTC6804_RDSTATB:{cmd[0] = 0x00;cmd[1] = 0x12;
            }break;
            
            default:break;
        }
        
        cmd_pec = LTC6804_Pec15_Calc(2, cmd);
        
        cmd[2] = (uint8_t)(cmd_pec >> 8);
        cmd[3] = (uint8_t)(cmd_pec);
        
        LTC6804_WakeUp();
  
        LTC6820_CS_Clear(GPIO_PTG7);
                
        LTC6820_SPI_WriteNByteData(4, cmd);
        LTC6820_SPI_ReadNByteData((8*total_ic),r_data);
        
        LTC6820_CS_Set(GPIO_PTG7);
        
        return 0;
    }
    
    return -1;
}

//----------------------------------------------------------------------------
// Function      : int LTC6804_RDCV(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t cell_codes[][12])     |
// Description   : 读电压                                            |
// Input         : 寄存器组名称   LTC6804个数  电压数据                                                |
// Output        :                                                  |
// return        : 0  成功    -1  失败                                                         |
//----------------------------------------------------------------------------
# if 0
int LTC6804_RDCV(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t cell_codes[LTC6804NUM][LTC6804CELL1])
{
	const uint8_t NUM_RX_BYTE = 8;
	const uint8_t BYTE_IN_GRPREG = 6;         /* byte numbers in each cell voltage register groups. */
	const uint8_t CELL_IN_GRPREG = 3;         /* cell numbers in each cell voltage register groups. */

	uint8_t cell_data[8 * TOTAL_IC];
      
      uint16_t Vol_data[LTC6804NUM][LTC6804CELL1];
	int pec_error = -1;
	uint16_t parsed_cell;
	uint16_t received_pec;
	uint16_t data_pec;
	uint8_t data_counter = 0;
	uint8_t cell_reg,current_ic,current_cell;

	if (rd_cmd <= LTC6804_RDCVD)
	{
		if ((total_ic >= 1) && (total_ic <= TOTAL_IC))
		{
			if (rd_cmd == LTC6804_RDCVALL)
			{
				for (cell_reg = 1; cell_reg < 5; cell_reg++)  // Each of the LTC6804 has 4 cell voltage register groups.
				{
					data_counter = 0;

					if (cell_reg == 1)LTC6804_ReadRawData_Reg(LTC6804_RDCVA, total_ic, cell_data);
					else if (cell_reg == 2)LTC6804_ReadRawData_Reg(LTC6804_RDCVB, total_ic, cell_data);
					else if (cell_reg == 3)LTC6804_ReadRawData_Reg(LTC6804_RDCVC, total_ic, cell_data);
					else LTC6804_ReadRawData_Reg(LTC6804_RDCVD, total_ic, cell_data);

					for (current_ic = 0 ; current_ic < total_ic; current_ic++)
					{
						for (current_cell = 0; current_cell < CELL_IN_GRPREG; current_cell++)
						{
							parsed_cell = cell_data[data_counter] + ((uint16_t)cell_data[data_counter + 1] << 8);

							cell_codes[current_ic][current_cell + ((cell_reg - 1) * CELL_IN_GRPREG)] = parsed_cell;

							data_counter += 2;
						}

						received_pec = ((uint16_t)cell_data[data_counter] << 8) + cell_data[data_counter + 1];

						data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &cell_data[current_ic * NUM_RX_BYTE]);

						if (received_pec != data_pec)return -1;
                                    
                                    /*if(received_pec == data_pec)
                                    {
                                          
                                    }
                                    else
                                    {
                                    }*/

						data_counter += 2;
					}
				}
				
				pec_error = 0;
			}
			else
			{
				pec_error = LTC6804_ReadRawData_Reg(rd_cmd, total_ic, cell_data);

				for (current_ic = 0; current_ic < total_ic; current_ic++)
				{
					for (current_cell = 0; current_cell < CELL_IN_GRPREG; current_cell++)
					{
						parsed_cell = cell_data[data_counter] + ((uint16_t)cell_data[data_counter+1] << 8);

						if (rd_cmd == LTC6804_RDCVA)cell_codes[current_ic][current_cell] = 0x0000FFFF & parsed_cell;
						else if (rd_cmd == LTC6804_RDCVB)cell_codes[current_ic][current_cell + CELL_IN_GRPREG] = 0x0000FFFF & parsed_cell;
						else if (rd_cmd == LTC6804_RDCVC)cell_codes[current_ic][current_cell + 2*CELL_IN_GRPREG] = 0x0000FFFF & parsed_cell;
						else cell_codes[current_ic][current_cell + 3*CELL_IN_GRPREG] = 0x0000FFFF & parsed_cell;

						data_counter += 2;
					}

					received_pec = ((uint16_t)cell_data[data_counter] << 8) + cell_data[data_counter + 1];

					data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &cell_data[current_ic * NUM_RX_BYTE]);

					if(received_pec != data_pec)return -1;

					data_counter += 2;
				}
				
				pec_error = 0;
			}
		}
	}

	return (pec_error);
}
#endif



int LTC6804_RDCV(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t cell_codes[LTC6804NUM][LTC6804CELL1])
{
	const uint8_t NUM_RX_BYTE = 8;
	const uint8_t BYTE_IN_GRPREG = 6;         /* byte numbers in each cell voltage register groups. */
	const uint8_t CELL_IN_GRPREG = 3;         /* cell numbers in each cell voltage register groups. */

	uint8_t cell_data[8 * TOTAL_IC];
      
      uint16_t Vol_data[LTC6804NUM][LTC6804CELL1];
	int pec_error = -1;
	uint16_t parsed_cell;
	uint16_t received_pec;
	uint16_t data_pec;
	uint8_t data_counter = 0;
	uint8_t cell_reg,current_ic,current_cell;

	if (rd_cmd <= LTC6804_RDCVD)
	{
		if ((total_ic >= 1) && (total_ic <= TOTAL_IC))
		{
			if (rd_cmd == LTC6804_RDCVALL)
			{
				for (cell_reg = 1; cell_reg < 5; cell_reg++)  // Each of the LTC6804 has 4 cell voltage register groups.
				{
					data_counter = 0;

					if (cell_reg == 1)LTC6804_ReadRawData_Reg(LTC6804_RDCVA, total_ic, cell_data);
					else if (cell_reg == 2)LTC6804_ReadRawData_Reg(LTC6804_RDCVB, total_ic, cell_data);
					else if (cell_reg == 3)LTC6804_ReadRawData_Reg(LTC6804_RDCVC, total_ic, cell_data);
					else LTC6804_ReadRawData_Reg(LTC6804_RDCVD, total_ic, cell_data);

					for (current_ic = 0 ; current_ic < total_ic; current_ic++)
					{
						for (current_cell = 0; current_cell < CELL_IN_GRPREG; current_cell++)
						{
							parsed_cell = cell_data[data_counter] + ((uint16_t)cell_data[data_counter + 1] << 8);

							Vol_data[current_ic][current_cell + ((cell_reg - 1) * CELL_IN_GRPREG)] = parsed_cell;

							data_counter += 2;
						}

						received_pec = ((uint16_t)cell_data[data_counter] << 8) + cell_data[data_counter + 1];

						data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &cell_data[current_ic * NUM_RX_BYTE]);

						//if (received_pec != data_pec)return -1;
                                    
                                    if(received_pec == data_pec)
                                    {
                                        for (current_cell = 0; current_cell < CELL_IN_GRPREG; current_cell++)
                                        {
                                              cell_codes[current_ic][current_cell + ((cell_reg - 1) * CELL_IN_GRPREG)] = Vol_data[current_ic][current_cell + ((cell_reg - 1) * CELL_IN_GRPREG)];
                                        }
                                    }
                                    else
                                    {
                                          /* 寄存器A B C D 四个同时校验不通过，认为通讯失败*/
                                    }

						data_counter += 2;
					}
				}
				
				pec_error = 0;
			}
			else
			{
				pec_error = LTC6804_ReadRawData_Reg(rd_cmd, total_ic, cell_data);

				for (current_ic = 0; current_ic < total_ic; current_ic++)
				{
					for (current_cell = 0; current_cell < CELL_IN_GRPREG; current_cell++)
					{
						parsed_cell = cell_data[data_counter] + ((uint16_t)cell_data[data_counter+1] << 8);

						if (rd_cmd == LTC6804_RDCVA)Vol_data[current_ic][current_cell] = 0x0000FFFF & parsed_cell;
						else if (rd_cmd == LTC6804_RDCVB)Vol_data[current_ic][current_cell + CELL_IN_GRPREG] = 0x0000FFFF & parsed_cell;
						else if (rd_cmd == LTC6804_RDCVC)Vol_data[current_ic][current_cell + 2*CELL_IN_GRPREG] = 0x0000FFFF & parsed_cell;
						else Vol_data[current_ic][current_cell + 3*CELL_IN_GRPREG] = 0x0000FFFF & parsed_cell;

						data_counter += 2;
					}

					received_pec = ((uint16_t)cell_data[data_counter] << 8) + cell_data[data_counter + 1];

					data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &cell_data[current_ic * NUM_RX_BYTE]);

					//if(received_pec != data_pec)return -1;
                              
                              if(received_pec == data_pec)
                              {
                                   for (current_cell = 0; current_cell < CELL_IN_GRPREG; current_cell++)
                                   {
                                         if (rd_cmd == LTC6804_RDCVA)
                                         {
                                            cell_codes[current_ic][current_cell] = Vol_data[current_ic][current_cell];
                                         }
                                         else if (rd_cmd == LTC6804_RDCVB)
                                         {
                                              cell_codes[current_ic][current_cell + CELL_IN_GRPREG] = Vol_data[current_ic][current_cell + CELL_IN_GRPREG];
                                         }
                                         else if (rd_cmd == LTC6804_RDCVC)
                                         {
                                              cell_codes[current_ic][current_cell + 2*CELL_IN_GRPREG] = Vol_data[current_ic][current_cell + 2*CELL_IN_GRPREG];
                                         }
                                         else 
                                         {
                                              cell_codes[current_ic][current_cell + 3*CELL_IN_GRPREG] = Vol_data[current_ic][current_cell + 3*CELL_IN_GRPREG];
                                         }
                                   }
                               }
                               else
                               {
                                          /*记状态*/
                               }

					data_counter += 2;
				}
				
				pec_error = 0;
			}
		}
	}

	return (pec_error);
}

//----------------------------------------------------------------------------
// Function      : uint32_t LTC6804_RDAUX(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t aux_codes[][6])            |
// Description   : 读温度                                                 |
// Input         : 寄存器组名称 芯片个数  数据缓存                                                |
// Output        :                                                 |
// Others        :  0  有效   -1  无效                                                        |
//----------------------------------------------------------------------------

uint32_t LTC6804_RDAUX(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t aux_codes[][6]) 
{
      const uint8_t NUM_RX_BYTE = 8;
	const uint8_t BYTE_IN_GRPREG = 6;         /* byte numbers in each Auxiliary Register Group. */
	const uint8_t GPIO_IN_REG = 3; 
	
	uint8_t data[8 * TOTAL_IC]; 
      uint16_t Temp_data[LTC6804NUM][6];
	uint8_t data_counter = 0;
	uint16_t parsed_aux;
	uint16_t received_pec;
	uint16_t data_pec;
	int pec_error = -1;
	
	uint8_t gpio_reg,current_ic,current_gpio;
	
	if (rd_cmd == LTC6804_RDAUXALL) 
	{
	    for (gpio_reg = 1; gpio_reg < GPIO_IN_REG; gpio_reg++) 
	    {
	        data_counter = 0;
	        
	        if (gpio_reg == 1)LTC6804_ReadRawData_Reg(LTC6804_RDAUXA, total_ic, data);
	        else LTC6804_ReadRawData_Reg(LTC6804_RDAUXB, total_ic, data);
	        
	        for (current_ic = 0; current_ic < total_ic; current_ic++) 
	        {
	            for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++) 
	            {
	                parsed_aux = data[data_counter] + ((uint16_t)data[data_counter+1] << 8);
	                
	                Temp_data[current_ic][current_gpio + ((gpio_reg-1)*GPIO_IN_REG)] = parsed_aux;
	                
	                data_counter += 2;
	            }
	            
	            received_pec = ((uint16_t)data[data_counter] << 8) + data[data_counter + 1];
	            
	            data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &data[current_ic * NUM_RX_BYTE]);
	            
	            //if (received_pec != data_pec)return -1;
                  if (received_pec == data_pec)
                  {
                         for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++)
                         {
                                 aux_codes[current_ic][current_gpio + ((gpio_reg-1)*GPIO_IN_REG)] = Temp_data[current_ic][current_gpio + ((gpio_reg-1)*GPIO_IN_REG)];
                         }              
                  }
                  else
                  {
                       /* 寄存器A B C D 四个同时校验不通过，认为通讯失败*/
                  }
	            
	            data_counter += 2;
	        }
	    }
	    
	    pec_error = 0;
	} 
	else 
	{
	    pec_error = LTC6804_ReadRawData_Reg(rd_cmd, total_ic, data);
	    
	    for (current_ic = 0; current_ic < total_ic; current_ic++) 
	    {
	        for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++) 
	        {
	            parsed_aux = data[data_counter] + ((uint16_t)data[data_counter+1] << 8);
                  
                  
                  if (rd_cmd == LTC6804_RDAUXA)
                  {
                        Temp_data[current_ic][current_gpio] = parsed_aux;
                  }
			else
                  {
                        Temp_data[current_ic][current_gpio + GPIO_IN_REG] =  parsed_aux;
                  }
	            data_counter += 2;
	        }
	        
	        received_pec = ((uint16_t)data[data_counter] << 8) + data[data_counter + 1];
	        
	        data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &data[current_ic * NUM_RX_BYTE]);
	        
	        //if (received_pec != data_pec)return -1;
	        if (received_pec == data_pec)
              {
                    if (rd_cmd == LTC6804_RDAUXA)
                    {
                         for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++)
                         {
                              aux_codes[current_ic][current_gpio] = Temp_data[current_ic][current_gpio];
                         } 
                    }
                    else
                    {
                        for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++)
                        {
                            aux_codes[current_ic][current_gpio + GPIO_IN_REG] =  Temp_data[current_ic][current_gpio + GPIO_IN_REG];
                        }
                    }             
              }
              else
              {
                    /* 寄存器A B 两个同时校验不通过，认为通讯失败*/
              }
	        data_counter += 2;
	    }
          
	    pec_error = 0;
	}
	
	return (pec_error);
}

//----------------------------------------------------------------------------
// Function      : uint16_t LTC6804_Pec15_Calc(uint8_t len, uint8_t *data)           |
// Description   : 写配置寄存器组                                                 |
// Input         : 芯片个数  数据                                                 |
// Output        : 无                                                 |
// Others        :                                                          |
//----------------------------------------------------------------------------
void LTC6804_WRCFG(uint8_t total_ic, uint8_t w_config[][6])
{
    const uint8_t BYTES_IN_REG = 6; 
    uint8_t CMD_LEN;
    uint8_t cmd_index; 
    uint8_t current_byte,current_ic;
    uint8_t cmd[8*TOTAL_IC + 4];
    uint16_t cfg_pec;
    
    if (total_ic == 1) 
    {
        CMD_LEN = 12;     
    } 
    else if (total_ic == 2) 
    {
        CMD_LEN = 20; 
    } 
    else if (total_ic == 3)
    {
        CMD_LEN = 28;
    } 
    else 
    {
        CMD_LEN = 36;
    }
  
    cmd[0] = 0x00;
    cmd[1] = 0x01;
    cmd[2] = 0x3d;
    cmd[3] = 0x6e;

    cmd_index = 4;

    for (current_ic = total_ic; current_ic > 0; current_ic--) 
    {
        for (current_byte = 0; current_byte < BYTES_IN_REG; current_byte++)
        {                                                                           
            cmd[cmd_index] = w_config[current_ic - 1][current_byte];                      
            cmd_index = cmd_index + 1;
        }
        
        /* calculating the PEC for each ICs configuration register data */
        cfg_pec = LTC6804_Pec15_Calc(BYTES_IN_REG, &w_config[current_ic-1][0]);  
        
        cmd[cmd_index] = (uint8_t)(cfg_pec >> 8);
        cmd[cmd_index + 1] = (uint8_t)cfg_pec;
        
        cmd_index = cmd_index + 2;
    }
    LTC6804_WakeUp();
            
    LTC6820_CS_Clear(GPIO_PTG7); 
    
    LTC6820_SPI_WriteNByteData(CMD_LEN, cmd);
    
    LTC6820_CS_Set(GPIO_PTG7); 

}

//----------------------------------------------------------------------------
// Function      : uint32_t LTC6804_RDCFG(uint8_t total_ic, uint8_t r_config[][6])            |
// Description   : 读配置寄存器                                                |
// Input         : 芯片个数  数据缓存                                                |
// Output        :                                                 |
// Others        :  0  有效   -1  无效                                                        |
//----------------------------------------------------------------------------

uint32_t LTC6804_RDCFG(uint8_t total_ic, uint8_t r_config[][6])
{
    const uint8_t BYTES_IN_REG = 6;
    int pec_error = 0;
    uint16_t cmd_pec;
    
    uint8_t cmd[4];
    uint8_t rx_data[8 * TOTAL_IC];
    uint8_t current_ic,current_byte;
    
    uint8_t data_pec;
    uint8_t received_pec;
  
    /* Read Configuration Register Group Command is 0x0002 */
    cmd[0] = 0x00;
    cmd[1] = 0x02;
    
    cmd_pec = LTC6804_Pec15_Calc(2, cmd);
        
    cmd[2] = (uint8_t)(cmd_pec >> 8);
    cmd[3] = (uint8_t)(cmd_pec);
    
    /* Read the configuration data of all ICs on the daisy chain into rx_data[] array */
    LTC6804_WakeUp();
            
    LTC6820_CS_Clear(GPIO_PTG7);
    
    LTC6820_SPI_WriteNByteData(4, cmd);
    LTC6820_SPI_ReadNByteData(((BYTES_IN_REG+2) * 2),rx_data);
    
    LTC6820_CS_Set(GPIO_PTG7);
        
    for (current_ic = 0; current_ic < total_ic; current_ic++)             
    {
        for (current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) 
        {
            r_config[current_ic][current_byte] = rx_data[current_byte + (current_ic * (BYTES_IN_REG+2))];
        }
        
        received_pec = ((uint16_t)(rx_data[current_ic * (BYTES_IN_REG+2)+6]) << 8) + rx_data[current_ic * (BYTES_IN_REG+2)+7];
        
        data_pec = LTC6804_Pec15_Calc(6, &r_config[current_ic][0]);
        
        if(received_pec != data_pec)
        {
            pec_error = -1;
            
            break;
        }
    }

    return(pec_error);
}

//----------------------------------------------------------------------------
// Function      : int LTC6804_ClearRawData_Reg(LTC6804_ClearRegister_Command clr_cmd)    |
// Description   : 清寄存器数据                                                |
// Input         : 命令码                                              |
// Output        :                                                 |
// Others        :  0  有效   -1  无效                                                        |
//----------------------------------------------------------------------------

int LTC6804_ClearRawData_Reg(LTC6804_ClearRegister_Command clr_cmd) 
{
    uint8_t cmd[4];
    uint16_t cmd_pec;
    
    if ((clr_cmd <= LTC6804_CLRSTAT)&&(clr_cmd >= LTC6804_CLRCELL)) 
    {
        switch (clr_cmd) 
        {
            case LTC6804_CLRCELL:{cmd[0] = 0x07;cmd[1] = 0x11;
            }break;
            
            case LTC6804_CLRAUX:{cmd[0] = 0x07;cmd[1] = 0x12;
            }break;
            
            case LTC6804_CLRSTAT:{cmd[0] = 0x07;cmd[1] = 0x13;
            }break;
            
            default:break;
        }
        
        cmd_pec = LTC6804_Pec15_Calc(2, cmd);
        
        cmd[2] = (uint8_t)(cmd_pec >> 8);
        cmd[3] = (uint8_t)(cmd_pec);
    
        LTC6820_SPI_WriteNByteData(4, cmd);                                            
        
        return 0;
    }
    
    return -1;
}

//----------------------------------------------------------------------------
// Function      : int LTC6804_ADSTAT(LTC6804_ADC_Mode md, LTC6804_StatusGroupSelection chst)    |
// Description   : 启动ADSTAT 转换                                               |
// Input         : ADC模式  命令码                                             |
// Output        :                                                 |
// Others        :  0  有效   -1  无效                                                        |
//----------------------------------------------------------------------------

int LTC6804_ADSTAT(LTC6804_ADC_Mode md, LTC6804_StatusGroupSelection chst)
{
     uint8_t cmd[4];
     uint16_t cmd_pec;
     uint8_t bit;
     
     if ((md <= MD_FILTERED)&&(md >= MD_FAST)) 
     {
        if (chst <= CHST_VD)
        {
            bit = (md & 0x02) >> 1;
            cmd[0] = bit + 0x04;
            bit = (md & 0x01) << 7;
            cmd[1] = bit + 0x60 + 0x08 + chst;
            
            cmd_pec = LTC6804_Pec15_Calc(2, cmd);
            
            cmd[2] = (uint8_t)(cmd_pec >> 8);
            cmd[3] = (uint8_t)cmd_pec;

            LTC6820_SPI_WriteNByteData(4, cmd);
            
            return 0;
        }
     }
     
     return -1;
}

//----------------------------------------------------------------------------
// Function      : uint32_t LTC6804_RDAUX(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t aux_codes[][6])            |
// Description   : 读温度                                                 |
// Input         : 寄存器组名称 芯片个数  数据缓存                                                |
// Output        :                                                 |
// Others        :  0  有效   -1  无效                                                        |
//----------------------------------------------------------------------------

uint32_t LTC6804_RDSTAT(LTC6804_ReadRegister_Command rd_cmd, uint8_t total_ic, uint16_t aux_codes[][6]) 
{
      const uint8_t NUM_RX_BYTE = 8;
	const uint8_t BYTE_IN_GRPREG = 6;         /* byte numbers in each Auxiliary Register Group. */
	const uint8_t GPIO_IN_REG = 3; 
	
	uint8_t data[8 * TOTAL_IC]; 
	uint8_t data_counter = 0;
	uint16_t parsed_aux;
	uint16_t received_pec;
	uint16_t data_pec;
	int pec_error = -1;
	
	uint8_t gpio_reg,current_ic,current_gpio;
	
	if (rd_cmd == LTC6804_RDSTATALL) 
	{
	    for (gpio_reg = 1; gpio_reg < GPIO_IN_REG; gpio_reg++) 
	    {
	        data_counter = 0;
	        
	        if (gpio_reg == 1)LTC6804_ReadRawData_Reg(LTC6804_RDSTATA, total_ic, data);
	        else LTC6804_ReadRawData_Reg(LTC6804_RDSTATB, total_ic, data);
	        
	        for (current_ic = 0; current_ic < total_ic; current_ic++) 
	        {
	            for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++) 
	            {
	                parsed_aux = data[data_counter] + ((uint16_t)data[data_counter+1] << 8);
	                
	                aux_codes[current_ic][current_gpio + ((gpio_reg-1)*GPIO_IN_REG)] = parsed_aux;
	                
	                data_counter += 2;
	            }
	            
	            received_pec = ((uint16_t)data[data_counter] << 8) + data[data_counter + 1];
	            
	            data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &data[current_ic * NUM_RX_BYTE]);
	            
	            if (received_pec != data_pec)return -1;
	            
	            data_counter += 2;
	        }
	    }
	    
	    pec_error = 0;
	} 
	else 
	{
	    pec_error = LTC6804_ReadRawData_Reg(rd_cmd, total_ic, data);
	    
	    for (current_ic = 0; current_ic < total_ic; current_ic++) 
	    {
	        for (current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++) 
	        {
	            parsed_aux = data[data_counter] + ((uint16_t)data[data_counter+1] << 8);
                  
                  
                  if (rd_cmd == LTC6804_RDSTATA)
                  {
                        aux_codes[current_ic][current_gpio] = parsed_aux;
                  }
			else
                  {
                        aux_codes[current_ic][current_gpio + GPIO_IN_REG] =  parsed_aux;
                  }
	            data_counter += 2;
	        }
	        
	        received_pec = ((uint16_t)data[data_counter] << 8) + data[data_counter + 1];
	        
	        data_pec = LTC6804_Pec15_Calc(BYTE_IN_GRPREG, &data[current_ic * NUM_RX_BYTE]);
	        
	        if (received_pec != data_pec)return -1;
	            
	        data_counter += 2;
	    }
	    
	    pec_error = 0;
	}
	
	return (pec_error);
}






//----------------------------------------------------------------------------
// Function      : void Vol_Temp_Conversion_And_Read(void)    |
// Description   : 电压温度启动转换和读取                           |
// Input         :   无                                         |
// Output        :   无                                             |
// Others        :                                                        |
//----------------------------------------------------------------------------
void  LTC6804_Read_Original_Vol(void)
{
    if((g_LTC6804_Vol_Conversion_Start_Flag)&&(g_LTC6804_Conversion_Finish_Flag))
    {
           LTC6804_WakeUp(); 
           
           (void)LTC6804_RDCV(LTC6804_RDCVALL, 2, g_LTC6804_original_vol);
           
            LTC6804_RDSTAT(LTC6804_RDSTATB,2, g_LTC6804_STAR_Data);
            
            /* 每次电压读取完成后，清除电压寄存器数据*/
           
            LTC6804_ClearRawData_Reg(LTC6804_CLRCELL); 
            
            /* 每次电压读取完成后，清除状态寄存器数据*/
            
            LTC6804_ClearRawData_Reg(LTC6804_CLRSTAT);
            
            /* 启动温度转换*/
            
           (void)LTC6804_ADAX(MD_FILTERED, AUX_CH_GPIOxVREF2);
           
      
            g_LTC6804_Temp_Conversion_Start_Flag = 1;
            g_LTC6804_Conversion_Finish_Flag = 0;
            g_LTC6804_Vol_Conversion_Start_Flag = 0;
            g_LTC6804_Time_Count = 0;
            g_LTC6804_Vol_Read_Finish_Flag = 1;
      }
}
void LTC6804_Read_Temp(void)
{
    if((g_LTC6804_Temp_Conversion_Start_Flag)&&(g_LTC6804_Conversion_Finish_Flag))
    {   
           LTC6804_WakeUp(); 
              
           (void)LTC6804_RDAUX(LTC6804_RDAUXALL, 2, g_LTC6804_original_temp); 
           
           /* 每次温度读取完成后，清除温度寄存器数据*/
           
            LTC6804_ClearRawData_Reg(LTC6804_CLRAUX); 
            
            /* 每次温度读取完成后，写配置寄存器数据*/
            
            LTC6804_WRCFG(2,g_Write_ConfigRegister);
            
            /* 启动电压转换*/
            
           (void)LTC6804_ADCV(MD_FILTERED, DCP_DISABLED, CELL_CH_ALL);
        
          g_LTC6804_Temp_Conversion_Start_Flag = 0;
          g_LTC6804_Vol_Conversion_Start_Flag = 1;
          g_LTC6804_Conversion_Finish_Flag = 0;
          g_LTC6804_Time_Count = 0;
          g_LTC6804_Temp_Read_Finish_Flag = 1;
    }
}
void Vol_Temp_Conversion_And_Read(void)
{

    if((g_LTC6804_Vol_Conversion_Start_Flag)&&(g_LTC6804_Conversion_Finish_Flag))
    {
           LTC6804_WakeUp(); 
           
           (void)LTC6804_RDCV(LTC6804_RDCVALL, 2, g_LTC6804_original_vol);
           
            LTC6804_RDSTAT(LTC6804_RDSTATB,2, g_LTC6804_STAR_Data);
            
            /* 每次电压读取完成后，清除电压寄存器数据*/
           
            LTC6804_ClearRawData_Reg(LTC6804_CLRCELL); 
            
            /* 每次电压读取完成后，清除状态寄存器数据*/
            
            LTC6804_ClearRawData_Reg(LTC6804_CLRSTAT);
            
            /* 启动温度转换*/
            
           (void)LTC6804_ADAX(MD_FILTERED, AUX_CH_GPIOxVREF2);
           
           LTC6804_Vol_Test();
      
            g_LTC6804_Temp_Conversion_Start_Flag = 1;
            g_LTC6804_Conversion_Finish_Flag = 0;
            g_LTC6804_Vol_Conversion_Start_Flag = 0;
            g_LTC6804_Time_Count = 0;
            g_LTC6804_Vol_Read_Finish_Flag = 1;
      }

      if((g_LTC6804_Temp_Conversion_Start_Flag)&&(g_LTC6804_Conversion_Finish_Flag))
      {   
           LTC6804_WakeUp(); 
              
           (void)LTC6804_RDAUX(LTC6804_RDAUXALL, 2, g_LTC6804_original_temp); 
           
           /* 每次温度读取完成后，清除温度寄存器数据*/
           
            LTC6804_ClearRawData_Reg(LTC6804_CLRAUX); 
            
            /* 每次温度读取完成后，写配置寄存器数据*/
            
            LTC6804_WRCFG(2,g_Write_ConfigRegister);
            
            /* 启动电压转换*/
            
           (void)LTC6804_ADCV(MD_FILTERED, DCP_DISABLED, CELL_CH_ALL);
           
           LTC6804_Temp_Test();
        
          g_LTC6804_Temp_Conversion_Start_Flag = 0;
          g_LTC6804_Vol_Conversion_Start_Flag = 1;
          g_LTC6804_Conversion_Finish_Flag = 0;
          g_LTC6804_Time_Count = 0;
          g_LTC6804_Temp_Read_Finish_Flag = 1;
      }
}


//----------------------------------------------------------------------------
// Function      : void Vol_Temp_Process(void)                           |
// Description   : 电压温度数据处理                                 |
// Input         :   无                                                  |
// Output        :   无                                                  |
// Others        :                                                       |
//----------------------------------------------------------------------------

void Vol_Temp_Process(void)
{ 
      uint8_t i,j;
      int16_t Temp;
      uint8_t CellNum_count;
      uint8_t LTC6804_temp_buff;
      uint16_t Over_Vol_Flag[LTC6804NUM];
      uint16_t Under_Vol_Flag[LTC6804NUM];
      float  R_NTC;
      static uint8_t Vol_First_Collect[DCELLNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
      if(g_LTC6804_Vol_Read_Finish_Flag)
      {
          g_LTC6804_Vol_Read_Finish_Flag = 0;
          
          LTC6804_Vol_Allocation(); 
          CellNum_count = 0;
          for(i=0;i<Para_Struct.LTC6804Num;i++)
          {
                for(j=0;j<Para_Struct.LTC6804_CellNum[i];j++)
                {
                     g_LTC6804_original_vol_buff[i][j] /= 10;
                     if( g_LTC6804_original_vol_buff[i][j] < Para_Struct.Over_Vol_Threshold)
                     {
                          //if(Under_Vol_Flag[i])
                     }
                     //  一级滤波    Over_Vol_Threshold
                     //g_LTC6804_original_vol_buff[i][j] = Filter_Vol_First(CellNum_count+j,g_LTC6804_original_vol_buff[i][j]); 
                     /// 二级滤波
                    // Collect_Data.Vol_Buff[CellNum_count+j] = Filter_Vol_Second(CellNum_count+j,g_LTC6804_original_vol_buff[i][j]);
                     /*if(!Vol_First_Collect[CellNum_count+j]) 
                     {
                          Collect_Data.Vol[CellNum_count+j] = Collect_Data.Vol_Buff[CellNum_count+j];
                          Vol_First_Collect[CellNum_count+j] = 1;
                     }*/
                }
               // CellNum_count += Para_Struct.LTC6804_CellNum[i];
           }
          // Filter_Prevent_Vol_Jump(Collect_Data.Vol,Collect_Data.Vol_Buff); 
          LTC6804_Vol_Test();
     }  
     
     if(g_LTC6804_Temp_Read_Finish_Flag)
     {
          g_LTC6804_Temp_Read_Finish_Flag = 1;
          if(Para_Struct.TempNum <= 3 )
          {
              for(i=0;i< Para_Struct.TempNum;i++)
              {
                    LTC6804_temp_buff = g_LTC6804_original_temp[0][i+1];
                    LTC6804_temp_buff /= 10000;
                    R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);
                    
                    Temp = LTC6804_ResToTempNTC(R_NTC);
                    if(Temp != 0xff)
                    {
                          Temp = Temp + 40;
      				      
                          /*温度有效范围 -20~85 */
                          if((Temp<20) || (Temp > 125)) 
                          { 
      				 Temp = 0xff;
      			  }
      		   } 
                     g_LTC6804_original_temp_buff[0][i] = Temp;
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_First(i,Temp);
                     //CellTemp_Buff_Zengbo[i] = Collect_Data.CellTemp_Buff[i];
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_Second(i,Collect_Data.CellTemp_Buff[i]);
      		  // Cal_Struct.CellNTCTemp[i] = Temp_Two_Filter(i,Temp);
      		  // Cal_Struct.CellD_ALL_Temp[i] = Cal_Struct.CellNTCTemp[i];
            }
        }
        else if(Para_Struct.TempNum <= 6)
        {
            for(i=0;i<3;i++)
            {
                    LTC6804_temp_buff = g_LTC6804_original_temp_buff[0][i+1];
                    LTC6804_temp_buff /= 10000;
                    R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);
                    
                    Temp = LTC6804_ResToTempNTC(R_NTC);
                    if(Temp != 0xff)
                    {
                          Temp = Temp + 40;
      				      
                          /*温度有效范围 -20~85 */
                          if((Temp<20) || (Temp > 125)) 
                          { 
      				 Temp = 0xff;
      			  }
      		   } 
                    g_LTC6804_original_temp_buff[0][i] = Temp;
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_First(i,Temp);
                     //CellTemp_Buff_Zengbo[i] = Collect_Data.CellTemp_Buff[i];
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_Second(i,Collect_Data.CellTemp_Buff[i]);
      		  // Cal_Struct.CellNTCTemp[i] = Temp_Two_Filter(i,Temp);
      		  // Cal_Struct.CellD_ALL_Temp[i] = Cal_Struct.CellNTCTemp[i];
            }
            
            for(i=0;i<(Para_Struct.TempNum-3);i++)
            {
                    LTC6804_temp_buff = g_LTC6804_original_temp[1][i+1];
                    LTC6804_temp_buff /= 10000;
                    R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);
                    
                    Temp = LTC6804_ResToTempNTC(R_NTC);
                    if(Temp != 0xff)
                    {
                          Temp = Temp + 40;
      				      
                          /*温度有效范围 -20~85 */
                          if((Temp<20) || (Temp > 125)) 
                          { 
      				 Temp = 0xff;
      			  }
      		   } 
                    g_LTC6804_original_temp_buff[1][i] = Temp;
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_First(i,Temp);
                     //CellTemp_Buff_Zengbo[i] = Collect_Data.CellTemp_Buff[i];
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_Second(i,Collect_Data.CellTemp_Buff[i]);
      		  // Cal_Struct.CellNTCTemp[i] = Temp_Two_Filter(i,Temp);
      		  // Cal_Struct.CellD_ALL_Temp[i] = Cal_Struct.CellNTCTemp[i];
            }
            
        }
        else
        {
              for(i=0;i<4;i++)
              {
                    LTC6804_temp_buff = g_LTC6804_original_temp_buff[0][i+1];
                    LTC6804_temp_buff /= 10000;
                    R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);
                    R_NTC_zengbo = R_NTC;
                    Temp = LTC6804_ResToTempNTC(R_NTC);
                    if(Temp != 0xff)
                    {
                          Temp = Temp + 40;
      				      
                          /*温度有效范围 -20~85 */
                          if((Temp<20) || (Temp > 125)) 
                          { 
      				 Temp = 0xff;
      			  }
      		   }
                    if(i == 3)
                    {
                         g_LTC6804_original_temp_buff[1][i] = Temp; 
                    }
                    else
                    {
                          g_LTC6804_original_temp_buff[0][i] = Temp;
                    }
                     
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_First(i,Temp);
                     //CellTemp_Buff_Zengbo[i] = Collect_Data.CellTemp_Buff[i];
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_Second(i,Collect_Data.CellTemp_Buff[i]);
      		  // Cal_Struct.CellNTCTemp[i] = Temp_Two_Filter(i,Temp);
      		  // Cal_Struct.CellD_ALL_Temp[i] = Cal_Struct.CellNTCTemp[i];
            }
            
            for(i=0;i<(Para_Struct.TempNum-4);i++)
            {
                    LTC6804_temp_buff = g_LTC6804_original_temp[1][i+1];
                    LTC6804_temp_buff /= 10000;
                    R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);
                    
                    Temp = LTC6804_ResToTempNTC(R_NTC);
                    if(Temp != 0xff)
                    {
                          Temp = Temp + 40;
      				      
                          /*温度有效范围 -20~85 */
                          if((Temp<20) || (Temp > 125)) 
                          { 
      				 Temp = 0xff;
      			  }
      		   }
                    if(i == 3)
                    {
                           g_LTC6804_original_temp_buff[0][i] = Temp;
                     }
                    else
                    {
                          g_LTC6804_original_temp_buff[1][i] = Temp;
                     }
                    
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_First(i,Temp);
                     //CellTemp_Buff_Zengbo[i] = Collect_Data.CellTemp_Buff[i];
                     Collect_Data.CellTemp_Buff[i] = Filter_Temp_Second(i,Collect_Data.CellTemp_Buff[i]);
      		  // Cal_Struct.CellNTCTemp[i] = Temp_Two_Filter(i,Temp);
      		  // Cal_Struct.CellD_ALL_Temp[i] = Cal_Struct.CellNTCTemp[i];
            }
            
        }
     }
}


//----------------------------------------------------------------------------
// Function      : void LTC6804_Vol_Allocation(void)                          |
// Description   : 根据LTC6804电压分配原则，对数据进行排列                                |
// Input         :   无                                                  |
// Output        :   无                                                  |
// Others        :                                                       |
//----------------------------------------------------------------------------
void LTC6804_Vol_Allocation(void)
{
    uint8_t i,j,x,y;
    for(i=0;i< Para_Struct.LTC6804Num;i++)
    {
        x = Para_Struct.LTC6804_CellNum[i]/2;
        y = Para_Struct.LTC6804_CellNum[i]%2;
        for(j=0;j<x;j++)
        {
            if(y)
            {
                 g_LTC6804_original_vol_buff[i][x] =  g_LTC6804_original_vol[i][x];
                 g_LTC6804_original_vol_buff[i][j] =  g_LTC6804_original_vol[i][j];
                 g_LTC6804_original_vol_buff[i][j+x+1] =  g_LTC6804_original_vol[i][j+6];
            } 
            else
            {
                 g_LTC6804_original_vol_buff[i][j] =  g_LTC6804_original_vol[i][j];
                 g_LTC6804_original_vol_buff[i][j+x] =  g_LTC6804_original_vol[i][j+6];
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------
// Function      : INT16S ResToTempNTC(FP32 ResCalValue)                                          |
// Description   : 536 NTC阻值温度转换函数                                                        |
// Input         : ResCalValue-AD转换值                                                           |
// Output        : 返回查表计算的温度值                                                           |                                       |
// Others        : 无                                                                             |
//-------------------------------------------------------------------------------------------------
//536 NTC阻值温度转换函数,2013-11-06
static int16_t LTC6804_ResToTempNTC(float ResCalValue)
{
	uint8_t i=0;
	int16_t return_value;
      
      float  ResCalValue_Buff; 
        
	float DifValueWithTableFormerElement, DifValueWithTableLatterElement;
		
	for(i=0; i<155; i++)
	{	
		if((ResCalValue <= TableNTC_10K[i]) && (ResCalValue >= TableNTC_10K[i+1]))
		{
			break;
		}
	}

	if(i>= 155)
	{
		return_value = 0xff;
		return return_value;
	}
	
	//作差,取差值小的那一端作为温度值
      ResCalValue_Buff = ResCalValue - TableNTC_10K[i];
        
	DifValueWithTableFormerElement = fabs(ResCalValue_Buff);    //ABS(ArrayNTC[i], TempADData);
      
      ResCalValue_Buff = ResCalValue - TableNTC_10K[i+1];
	DifValueWithTableLatterElement = fabs(ResCalValue_Buff);
	if(DifValueWithTableFormerElement < DifValueWithTableLatterElement)
	{
		return_value = (int16_t)i - 30;    //NTC电阻表格中i为0时对应的温度为-30度,最后一个元素对应的温度为125度
	}
	else if(DifValueWithTableFormerElement > DifValueWithTableLatterElement)
	{
		return_value = (int16_t)i + 1 - 30;
	}
	
	return return_value;

}

void LTC6804_Vol_First_Conversion(void)
{
      (void)LTC6804_ADAX(MD_FILTERED, AUX_CH_GPIOxVREF2);
       g_LTC6804_Temp_Conversion_Start_Flag = 0;
       g_LTC6804_Vol_Conversion_Start_Flag = 1;
       g_LTC6804_Time_Count = 0;
}



static void LTC6804_Vol_Test(void)
{
      uint8_t i,j,n;
      
      LTC6804_Vol_Allocation();
      
      for(i=0;i<2;i++)
      {
        for(j=0;j<12;j++)
        {
             Collect_Data.Vol[n] = g_LTC6804_original_vol[i][j]/10;
             Collect_Data.Vol_Flag[n] = 0;
             n++;
        }
      }
}

static void LTC6804_Temp_Test(void)
{
      uint8_t i,j,n,x,y;
      uint16_t Temp;
      float LTC6804_temp_buff;
      float  R_NTC;
      
      for(i=0;i<4;i++)
      {
            LTC6804_temp_buff = g_LTC6804_original_temp[0][i+1];
            LTC6804_temp_buff /= 10000;
            R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);        
            Temp = LTC6804_ResToTempNTC(R_NTC);
            if(Temp != 0xff)
            {
                   Temp = Temp + 40;
                                      
                   /*温度有效范围 -20~85 */
                   if((Temp<20) || (Temp > 125)) 
                   { 
                        Temp = 0xff;
                   }
             }
            
            g_LTC6804_original_temp_buff[0][i] = Temp;
             /*if(i == 3)
             {
                   g_LTC6804_original_temp_buff[1][2] = Temp; 
             }
            else
            {
                   g_LTC6804_original_temp_buff[0][i] = Temp;
             }*/
      }
      
      for(j=0;j<(Para_Struct.TempNum-4);j++)
      {
            LTC6804_temp_buff = g_LTC6804_original_temp[1][j+1];
            LTC6804_temp_buff /= 10000;
            R_NTC = 10*LTC6804_temp_buff/(3- LTC6804_temp_buff);
            R_NTC_zengbo = R_NTC;         
            Temp = LTC6804_ResToTempNTC(R_NTC);
            if(Temp != 0xff)
            {
                   Temp = Temp + 40;
                                      
                   /*温度有效范围 -20~85 */
                   if((Temp<20) || (Temp > 125)) 
                   { 
                        Temp = 0xff;
                   }
             }
            
            g_LTC6804_original_temp_buff[1][j] = Temp;
             /*if(j == 0)
             {
                   g_LTC6804_original_temp_buff[0][3] = Temp; 
             }
            else
            {
                   g_LTC6804_original_temp_buff[1][j-1] = Temp;
             }*/
      }
      
      Collect_Data.CellTemp[0] = g_LTC6804_original_temp_buff[0][0];
      Collect_Data.CellTemp[1] = g_LTC6804_original_temp_buff[0][1];
      Collect_Data.CellTemp[2] = g_LTC6804_original_temp_buff[0][2];
      Collect_Data.CellTemp[3] = g_LTC6804_original_temp_buff[1][0];
      Collect_Data.CellTemp[4] = g_LTC6804_original_temp_buff[1][1];
      Collect_Data.CellTemp[5] = g_LTC6804_original_temp_buff[1][2];
      Collect_Data.CellTemp[6] = g_LTC6804_original_temp_buff[0][3];
      Collect_Data.CellTemp[7] = g_LTC6804_original_temp_buff[1][3];
      
      /*for(x=0;x<2;x++)
      {
        for(y=0;y<4;y++)
        {
             Collect_Data.CellTemp[n] = g_LTC6804_original_temp_buff[x][y];
             Collect_Data.CellTemp_Flag[n] = 0;
             n++;
        }
      }*/
      
}



