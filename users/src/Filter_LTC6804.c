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

#include <stdint.h>

#include "SPI_Driver.h"
#include "GPIO_Driver.h"
#include "LTC6804_Driver.h"
#include "Systick_Driver.h"
#include "Filter_LTC6804.h"


#define  TEMP_ERROR_COUNT_TOTAL      200

static uint16_t Vol_First_Filter_Buff[DCELLNUM][VOL_BUF_LENGTH];
static uint16_t Vol_Second_Filter_Buff[DCELLNUM][VOL_BUF_LENGTH];

static uint8_t Temp_First_Filter_Buff[DTEMPNUM][TEMP_BUF_LENGTH];
static uint8_t Temp_Second_Filter_Buff[DTEMPNUM][TEMP_BUF_LENGTH];


uint8_t  Temp_Error_Count[DTEMPNUM];

//---------------------------------------------------------------------------------
// Function      : INT16U Vol_Filter(INT8U Num,INT16U Cur)                        |
// Description   : 电压平均值一级滤波函数                                                   |
// Input         : Num-滤波的缓存区标号                                           |
//                 Vol-当前采集到的电压值                                        |
// Output        :                                                                |
// Return        : 返回滤波后的电压值                                             |
// Others        :                                                                |
//---------------------------------------------------------------------------------

uint16_t Filter_Vol_First(uint8_t Num,uint16_t Vol)  
{
    uint8_t i,j;
   
    uint32_t SumTemp = 0;               //累积电压值
    uint16_t Intermediate_Data = 0;
    uint16_t PrevVol = 0;
    static uint8_t  Vol_Effective_Flag = 0;
    static uint8_t Vol_First_Filter_Length_Flag[DCELLNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    static uint8_t Vol_Index[DCELLNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    //队列空位置指示
    uint16_t Vol_First_Filter_Buff_Temp[VOL_BUF_LENGTH];   
    
 
    /*  电压上限限判断，有效范围内，更新数据，否则为上次的值*/
    if((Vol>= VOL_LOWER_LIMIT) && (Vol<= VOL_UPPER_LIMIT))
    {                  
        Vol_First_Filter_Buff[Num][Vol_Index[Num]] = Vol;
        Vol_Effective_Flag = 1;
    } 
    else 
    {
        if(Vol_Index[Num] ==0)
        {
            Vol_First_Filter_Buff[Num][Vol_Index[Num]] = Vol_First_Filter_Buff[Num][VOL_BUF_LENGTH-1];
        } 
        else
        {
            Vol_First_Filter_Buff[Num][Vol_Index[Num]] = Vol_First_Filter_Buff[Num][Vol_Index[Num]-1];
        }    
    }
    
    if(Vol_Effective_Flag)
    {
         Vol_Index[Num]++;
    }
     
    if(Vol_First_Filter_Length_Flag[Num] != 1) 
    {
          for(SumTemp = 0, i=0;i<Vol_Index[Num];i++)
          {
              SumTemp += Vol_First_Filter_Buff[Num][i];
          }
          
          PrevVol = SumTemp / Vol_Index[Num];
          if(Vol_Index[Num] >= VOL_BUF_LENGTH)
          {
               Vol_First_Filter_Length_Flag[Num] = 1;
               Vol_Index[Num] = 0;
          }
          return PrevVol;
    }
    else 
    {
         if(Vol_Index[Num]>= VOL_BUF_LENGTH)
         {
             Vol_Index[Num] = 0;
         }
         for(i=0;i<VOL_BUF_LENGTH;i++) 
         { 
              Vol_First_Filter_Buff_Temp[i] = Vol_First_Filter_Buff[Num][i];
         }
         
         for(i=0;i<VOL_BUF_LENGTH;i++)
         {
              for(j=i+1;j<VOL_BUF_LENGTH;j++)
	        {
			   if(Vol_First_Filter_Buff_Temp[j] > Vol_First_Filter_Buff_Temp[i])
		         {
				    Intermediate_Data = Vol_First_Filter_Buff_Temp[i];
				    Vol_First_Filter_Buff_Temp[i] = Vol_First_Filter_Buff_Temp[j];
				    Vol_First_Filter_Buff_Temp[j] = Intermediate_Data;
			    }
	         }
	   }
			   
	   for(SumTemp=0,i=2;i<(VOL_BUF_LENGTH-2); i++)
         {
		   SumTemp += Vol_First_Filter_Buff_Temp[i];
	   }
			   
	   PrevVol = SumTemp/(VOL_BUF_LENGTH-4);
			   
	    return PrevVol;		                                                
	}
}


//---------------------------------------------------------------------------------
// Function      : INT16U Vol_Filter(INT8U Num,INT16U Cur)                        |
// Description   : 电压平均值一级滤波函数                                                   |
// Input         : Num-滤波的缓存区标号                                           |
//                 Vol-当前采集到的电压值                                        |
// Output        :                                                                |
// Return        : 返回滤波后的电压值                                             |
// Others        :                                                                |
//---------------------------------------------------------------------------------

uint16_t Filter_Vol_Second(uint8_t Num,uint16_t Vol)  
{
    uint8_t i,j;
    static uint8_t Vol_Index[DCELLNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    //队列空位置指示
    uint32_t SumTemp = 0;               //累积电压值
    uint16_t Intermediate_Data = 0;
    uint16_t PrevVol = 0;
    static uint8_t  Vol_Effective_Flag = 0;
    static uint8_t  Vol_Second_Filter_Length_Flag[DCELLNUM] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    
    uint16_t Vol_Second_Filter_Buff_Temp[VOL_BUF_LENGTH];
 
    /*  电压上限限判断，有效范围内，更新数据，否则为上次的值*/
    if((Vol>= VOL_LOWER_LIMIT) && (Vol<= VOL_UPPER_LIMIT))
    {                  
        Vol_First_Filter_Buff[Num][Vol_Index[Num]] = Vol;
        Vol_Effective_Flag = 1;
    } 
    else 
    {
        if(Vol_Index[Num] ==0)
        {
            Vol_Second_Filter_Buff[Num][Vol_Index[Num]] = Vol_Second_Filter_Buff[Num][VOL_BUF_LENGTH-1];
        } 
        else
        {
            Vol_Second_Filter_Buff[Num][Vol_Index[Num]] = Vol_Second_Filter_Buff[Num][Vol_Index[Num]-1];
        }    
    }
    
    if(Vol_Effective_Flag)
    {
         Vol_Index[Num]++;
    }
     
    if(Vol_Second_Filter_Length_Flag[Num] != 1) 
    {
          for(SumTemp = 0, i=0;i<Vol_Index[Num];i++)
          {
              SumTemp += Vol_Second_Filter_Buff[Num][i];
          }
          
          PrevVol = SumTemp / Vol_Index[Num];
          
          if(Vol_Index[Num] >= VOL_BUF_LENGTH)
          {
               Vol_Second_Filter_Length_Flag[Num] = 1;
               Vol_Index[Num] = 0;
          }
          return PrevVol;
    }
    else 
    {
         if(Vol_Index[Num]>= VOL_BUF_LENGTH)
         {
             Vol_Index[Num] = 0;
         }
         
         for(i=0;i<VOL_BUF_LENGTH;i++) 
         { 
              Vol_Second_Filter_Buff_Temp[i] = Vol_Second_Filter_Buff[Num][i];
         }
         
         for(i=0;i<VOL_BUF_LENGTH;i++)
         {
              for(j=i+1;j<VOL_BUF_LENGTH;j++)
	        {
			   if(Vol_Second_Filter_Buff_Temp[j] > Vol_Second_Filter_Buff_Temp[i])
		         {
				    Intermediate_Data = Vol_Second_Filter_Buff_Temp[i];
				    Vol_Second_Filter_Buff_Temp[i] = Vol_Second_Filter_Buff_Temp[j];
				    Vol_Second_Filter_Buff_Temp[j] = Intermediate_Data;
			    }
	         }
	   }
			   
	   for(SumTemp=0,i=2;i<(VOL_BUF_LENGTH-2); i++)
         {
		   SumTemp += Vol_Second_Filter_Buff_Temp[i];
	   }
			   
	    PrevVol = SumTemp/(VOL_BUF_LENGTH-4);
			   
	    return PrevVol;		                                                
	}
}


//---------------------------------------------------------------------------------
// Function      : INT8U Temp_Filter(INT8U Num,INT8U Temp)                        |
// Description   : 温度平均值一级滤波                                                 |
// Input         : Num-滤波的缓存区标号                                           |
//                 Temp-当前采集到的温度值                                        |
// Output        :                                                                |
// Return        : 返回滤波后的温度值                                             |
// Others        :                                                                |
//--------------------------------------------------------------------------------

uint8_t Filter_Temp_First(uint8_t Num,uint8_t Temp) 
{
    uint8_t i,j;
    static uint8_t Index[DTEMPNUM] = {0,0,0,0,0,0,0,0};                  //队列空位置指示
    uint16_t SumTemp = 0;               //累积电压值
    uint8_t Intermediate_Data = 0;
    
    static uint8_t Temp_First_Filter_Length_Flag[DTEMPNUM] = {0,0,0,0,0,0,0,0};
    
    uint8_t Temp_First_Filter_Buff_Temp[TEMP_BUF_LENGTH];
    uint8_t PrevTemp = 0;
    
    if(Temp==0xFF) 
    {
	    //增加连续故障计数，当连续故障大于6秒，显示故障结果 
        if(Temp_Error_Count[Num] < TEMP_ERROR_COUNT_TOTAL) 
        {
            Temp_Error_Count[Num]++;
        }
        
        if(Index[Num] == 0)
        {
             Temp_First_Filter_Buff[Num][Index[Num]] = Temp_First_Filter_Buff[Num][TEMP_BUF_LENGTH-1];
        } 
        else
        {
            Temp_First_Filter_Buff[Num][Index[Num]] = Temp_First_Filter_Buff[Num][Index[Num]-1];
        }
    }
    else if((Temp >= TEMP_LOWER_LIMIT) && (Temp <= TEMP_UPPER_LIMIT))
    {
         if(Index[Num] == 0)
         {
             Temp_First_Filter_Buff[Num][Index[Num]] = Temp_First_Filter_Buff[Num][TEMP_BUF_LENGTH-1];
         } 
         else
         { 
            Temp_First_Filter_Buff[Num][Index[Num]] = Temp_First_Filter_Buff[Num][Index[Num]-1];
         } 
    }
    else 
    {
        Temp_First_Filter_Buff[Num][Index[Num]] = Temp;
    }
    Index[Num]++;
    
    if(!Temp_First_Filter_Length_Flag[Num])
    {
        for(SumTemp = 0,i = 0;i < Index[Num];i ++)
        {
            SumTemp += Temp_First_Filter_Buff[Num][i];
        }
        PrevTemp = SumTemp/Index[Num];
        if(Index[Num]>= TEMP_BUF_LENGTH)
        {
             Index[Num] = 0;
             Temp_First_Filter_Length_Flag[Num] = 1;
        }
        return PrevTemp; 
    } 
    else 
    {
        if(Index[Num]>= TEMP_BUF_LENGTH)
        {
             Index[Num] = 0;
        }
         for(i = 0; i<TEMP_BUF_LENGTH; i++)
         {
            Temp_First_Filter_Buff_Temp[i] = Temp_First_Filter_Buff[Num][i];   
         }
         for(i=0;i<TEMP_BUF_LENGTH;i++)
         {
              for(j=i+1;j<TEMP_BUF_LENGTH;j++)
              {
			  if(Temp_First_Filter_Buff_Temp[j] > Temp_First_Filter_Buff_Temp[i])
                    {
				    Intermediate_Data = Temp_First_Filter_Buff_Temp[i];
                            Temp_First_Filter_Buff_Temp[i] = Temp_First_Filter_Buff_Temp[j];
				    Temp_First_Filter_Buff_Temp[j] = Intermediate_Data;
                    }
               }
          }
          for(SumTemp = 0,i=2;i<(TEMP_BUF_LENGTH-2); i++)
          {
              SumTemp += Temp_First_Filter_Buff_Temp[i];
          }
          PrevTemp = SumTemp/(TEMP_BUF_LENGTH-4);
          return PrevTemp;
    }
}


//---------------------------------------------------------------------------------
// Function      : uint8_t Filter_Temp_Second(uint8_t Num,uint8_t Temp)                        |
// Description   : 温度平均值二级滤波                                                 |
// Input         : Num-滤波的缓存区标号                                           |
//                 Temp-当前采集到的温度值                                        |
// Output        :                                                                |
// Return        : 返回滤波后的温度值                                             |
// Others        :                                                                |
//--------------------------------------------------------------------------------

uint8_t Filter_Temp_Second(uint8_t Num,uint8_t Temp) 
{
    uint8_t i,j;
    static uint8_t Index[DTEMPNUM] = {0,0,0,0,0,0,0,0};                  //队列空位置指示
    uint16_t SumTemp = 0;               //累积电压值
    uint8_t Intermediate_Data = 0;
    
    static uint8_t Temp_Second_Filter_Length_Flag[DTEMPNUM] = {0,0,0,0,0,0,0,0};
    
    uint8_t Temp_First_Second_Buff_Temp[TEMP_BUF_LENGTH];
    uint8_t PrevTemp = 0;
    
    
    Temp_Second_Filter_Buff[Num][Index[Num]] = Temp;
    Index[Num]++;
    
    if(!Temp_Second_Filter_Length_Flag[Num])
    {
        for(SumTemp = 0,i = 0;i < Index[Num];i ++)
        {
            SumTemp += Temp_Second_Filter_Buff[Num][i];
        }
        PrevTemp = SumTemp/Index[Num];
        if(Index[Num]>= TEMP_BUF_LENGTH)
        {
             Index[Num] = 0;
             Temp_Second_Filter_Length_Flag[Num] = 1;
        }
        return PrevTemp; 
    } 
    else 
    {
        if(Index[Num]>= TEMP_BUF_LENGTH)
        {
             Index[Num] = 0;
        }
         for(i = 0; i<TEMP_BUF_LENGTH; i++)
         {
            Temp_First_Second_Buff_Temp[i] = Temp_Second_Filter_Buff[Num][i];   
         }
         for(i=0;i<TEMP_BUF_LENGTH;i++)
         {
              for(j=i+1;j<TEMP_BUF_LENGTH;j++)
              {
			  if(Temp_First_Second_Buff_Temp[j] > Temp_First_Second_Buff_Temp[i])
                    {
				    Intermediate_Data = Temp_First_Second_Buff_Temp[i];
                            Temp_First_Second_Buff_Temp[i] = Temp_First_Second_Buff_Temp[j];
				    Temp_First_Second_Buff_Temp[j] = Intermediate_Data;
                    }
               }
          }
          for(SumTemp = 0,i=2;i<(TEMP_BUF_LENGTH-2); i++)
          {
              SumTemp += Temp_First_Second_Buff_Temp[i];
          }
          PrevTemp = SumTemp/(TEMP_BUF_LENGTH-4);
          return PrevTemp;
    }
}


//---------------------------------------------------------------------------------
// Function      : void Filter_Prevent_Vol_Jump(uint16_t * NewVol,uint16_t * LastVol)        |
// Description   : 电压突变滤波函数                                               |
// Input         : NewVol-最新采集滤波后电压值                                    |
//                 LastVol-上次发送电压值                                         |
// Output        : 本次最新上送的电压值                                           |
// Return        : 返回滤波后的温度值                                             |
// Others        : 当电压跳动幅度过大时，限制每次跳动的范围                       |
//--------------------------------------------------------------------------------
void Filter_Prevent_Vol_Jump(uint16_t * NewVol,uint16_t * LastVol) 
{
    uint8_t i;
    static  uint8_t ready;
    static  uint16_t PK[DCELLNUM];
    static  uint16_t MK[DCELLNUM];
    uint16_t  Can_T_Vol;
    //2015-04-01增加，上电自检阶段不进行123滤波
    /*if(SysTime_Struct.CurTime < VOL_TEST_TIME)
    {
        for(i=0;i<Para_Struct.CellNum;i++)
        {
            LastVol[i] = NewVol[i];
        }
        return;
    }*/
    if (!ready) 
    {
        for(i=0;i< DCELLNUM;i++) 
        {
            PK[i] = 100;
            MK[i] = 100;
        }
        
        ready = 1;
    } 
    else 
    {
        for(i=0;i<DCELLNUM;i++)
        {
             if(NewVol[i] > LastVol[i])
             {
                  MK[i] = 100;
                  
                  Can_T_Vol = NewVol[i] - LastVol[i];
                  if(Can_T_Vol > PK[i])
                  {

                       LastVol[i] = LastVol[i] + PK[i];
                       if(PK[i] < 400)
                       {
                           PK[i] += 100;
                       } 
                        
                  } 
                  else 
                  {
                       LastVol[i] = NewVol[i];
                  }
             } 
             else if(NewVol[i] < LastVol[i])
             {
                  PK[i] = 100;
                  Can_T_Vol = LastVol[i] - NewVol[i];
                  if(Can_T_Vol>= (MK[i]))
                  {
                      LastVol[i] = LastVol[i] - MK[i];
                      if(MK[i] < 400)
                       {
                           MK[i] += 100;
                       }
                  } 
                  else 
                  {
                       LastVol[i] = NewVol[i];
                  }
             } 
             else 
             {
                PK[i] = 100;
                MK[i] = 100;
                LastVol[i] = NewVol[i];
             }            
        } 
    }
}


//---------------------------------------------------------------------------------
// Function      : void Can_T_Temp_Filter(INT8U Num)                              |
// Description   : 温度跳变滤波函数                                               |
// Input         : Num-滤波的电池节数                                             |
// Output        :                                                                |
// Return        : 返回滤波后的温度值                                             |
// Others        : 限制温度每秒最大变化1度                                        |
//--------------------------------------------------------------------------------
void Filter_Prevent_Temp_Jump(uint16_t * NewTemp,uint16_t * LastTemp) 
{
    uint8_t i;
    
    uint8_t Temp_buff;
    
    for(i=0;i<8;i++)
    {
         if(NewTemp[i] > LastTemp[i])
         {                 
              Temp_buff = NewTemp[i] - LastTemp[i];
              if(Temp_buff > 1)
              {
                   LastTemp[i] = LastTemp[i] + 1;                      
              } 
              else 
              {
                   LastTemp[i] = NewTemp[i];
              }
          } 
          else if(NewTemp[i] < LastTemp[i])
          {
                  Temp_buff = LastTemp[i] - NewTemp[i];
                  if(Temp_buff > 1)
                  {
                      LastTemp[i] = LastTemp[i] - 1;
                  } 
                  else 
                  {
                       LastTemp[i] = NewTemp[i];
                  }
           } 
           else 
           {
                LastTemp[i] = NewTemp[i];
           }            
        } 
}