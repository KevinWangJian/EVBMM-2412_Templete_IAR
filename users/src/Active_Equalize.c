/**
  ******************************************************************************
  * @Copyright (C), 1997-2015, Hangzhou Gold Electronic Equipment Co., Ltd.
  * @file name: Active_Equalize.h
  * @author: Wangjian
  * @Descriptiuon: Provides a set of functions which parsing recived CAN messages
  *                and raplying with the specified CAN messages to master.
  * @Others: None
  * @History: 1. Created by Wangjian.
  * @version: V1.0.0
  * @date:    26-Sep-2015

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

  #include "Active_Equalize.h"


  /* 8.5V voltage module output control. */
  static void _8V5Voltage_ControlPin_Output(void)
  {
      GPIO_PinInit(GPIO_PTI1, GPIO_PinOutput);
  }



  static void _8V5Voltage_Enable(void)
  {
      GPIO_PinSet(GPIO_PTI1);
  }



  static void _8V5Voltage_Disable(void)
  {
      GPIO_PinClear(GPIO_PTI1);
  }




  static int EqualizationModule_DirectionPins_Init(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinInit(GPIO_PTI0, GPIO_PinOutput); break;

          case Module_2: GPIO_PinInit(GPIO_PTH0, GPIO_PinOutput_HighCurrent); break;

          case Module_3: GPIO_PinInit(GPIO_PTE5, GPIO_PinOutput); break;

          case Module_4: GPIO_PinInit(GPIO_PTB4, GPIO_PinOutput_HighCurrent); break;

          default: break;
      }

      return 0;
  }



  static int EqualizationModule_EnterChargeMode(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinClear(GPIO_PTI0); break;

          case Module_2: GPIO_PinClear(GPIO_PTH0); break;

          case Module_3: GPIO_PinClear(GPIO_PTE5); break;

          case Module_4: GPIO_PinClear(GPIO_PTB4); break;

          default: break;
      }

      return 0;
  }



  static int EqualizationModule_EnterDischargeMode(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinSet(GPIO_PTI0); break;

          case Module_2: GPIO_PinSet(GPIO_PTH0); break;

          case Module_3: GPIO_PinSet(GPIO_PTE5); break;

          case Module_4: GPIO_PinSet(GPIO_PTB4); break;

          default: break;
      }

      return 0;
  }



  static int EqualizationModule_FaultSignalPins_Init(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinInit(GPIO_PTH1, GPIO_PinInput); break;

          case Module_2: GPIO_PinInit(GPIO_PTE6, GPIO_PinInput); break;

          case Module_3: GPIO_PinInit(GPIO_PTB5, GPIO_PinInput); break;

          case Module_4: GPIO_PinInit(GPIO_PTC3, GPIO_PinInput); break;

          default: break;
      }

      return 0 ;
  }



  static uint8_t EqualizationModule_GetFaultSignalValue(ModuleNumber_TypeDef Module_Num)
  {
      uint8_t ret_val;

      if ((Module_Num < Module_1) || (Module_Num > Module_4))return 0xFFu;

      switch (Module_Num)
      {
          case Module_1: ret_val = GPIO_PinRead(GPIO_PTH1); break;

          case Module_2: ret_val = GPIO_PinRead(GPIO_PTE6); break;

          case Module_3: ret_val = GPIO_PinRead(GPIO_PTB5); break;

          case Module_4: ret_val = GPIO_PinRead(GPIO_PTC3); break;

          default: break;
      }

      return (ret_val);
  }





  static int _74HC138_EnablePins_Init(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinInit(GPIO_PTC2, GPIO_PinOutput); break;

          case Module_2: GPIO_PinInit(GPIO_PTI6, GPIO_PinOutput); break;

          case Module_3: GPIO_PinInit(GPIO_PTH4, GPIO_PinOutput); break;

          case Module_4: GPIO_PinInit(GPIO_PTF5, GPIO_PinOutput); break;

          default: break;
      }

      return 0;
  }



  static int _74HC138_Enable(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinSet(GPIO_PTC2); break;

          case Module_2: GPIO_PinSet(GPIO_PTI6); break;

          case Module_3: GPIO_PinSet(GPIO_PTH4); break;

          case Module_4: GPIO_PinSet(GPIO_PTF5); break;

          default: break;
      }

      return 0;
  }



  static int _74HC138_Disable(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinClear(GPIO_PTC2); break;

          case Module_2: GPIO_PinClear(GPIO_PTI6); break;

          case Module_3: GPIO_PinClear(GPIO_PTH4); break;

          case Module_4: GPIO_PinClear(GPIO_PTF5); break;

          default: break;
      }

      return 0;     
  }



  static int _74HC138_ABCInputPins_Init(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: 
          {
            GPIO_PinInit(GPIO_PTD7, GPIO_PinOutput); // A2
            GPIO_PinInit(GPIO_PTD6, GPIO_PinOutput); // A1
            GPIO_PinInit(GPIO_PTD5, GPIO_PinOutput); // A0
          } 
          break;

          case Module_2:
          {
            GPIO_PinInit(GPIO_PTI5, GPIO_PinOutput); // A2
            GPIO_PinInit(GPIO_PTC1, GPIO_PinOutput); // A1
            GPIO_PinInit(GPIO_PTC0, GPIO_PinOutput); // A0
          }
          break;

          case Module_3:
          {
            GPIO_PinInit(GPIO_PTH3, GPIO_PinOutput); // A2
            GPIO_PinInit(GPIO_PTF7, GPIO_PinOutput); // A1
            GPIO_PinInit(GPIO_PTF6, GPIO_PinOutput); // A0
          }
          break;

          case Module_4:
          {
            GPIO_PinInit(GPIO_PTF4, GPIO_PinOutput); // A2
            GPIO_PinInit(GPIO_PTB3, GPIO_PinOutput); // A1
            GPIO_PinInit(GPIO_PTB2, GPIO_PinOutput); // A0
          }
          break;

          default: break;
      }

      return 0;
  }



  static int _74HC138_ChannelSelect(ModuleNumber_TypeDef Module_Num, uint8_t channel)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      if (channel >= 6)return -1;

      switch (Module_Num)
      {
          case Module_1: 
          {
              switch (channel)
              {
                  case 0:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinClear(GPIO_PTD5);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinSet(GPIO_PTD5); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinSet(GPIO_PTD6);
                      GPIO_PinClear(GPIO_PTD5);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinSet(GPIO_PTD6);
                      GPIO_PinSet(GPIO_PTD5); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinSet(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinClear(GPIO_PTD5);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinSet(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinSet(GPIO_PTD5);
                  }
                  break;

                  default: break;
              }
          }
          break;

          case Module_2:
          {
              switch (channel)
              {
                  case 0:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinClear(GPIO_PTC0);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinSet(GPIO_PTC0); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinSet(GPIO_PTC1);
                      GPIO_PinClear(GPIO_PTC0);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinSet(GPIO_PTC1);
                      GPIO_PinSet(GPIO_PTC0); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinSet(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinClear(GPIO_PTC0);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinSet(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinSet(GPIO_PTC0);
                  }
                  break;

                  default: break;
              }
          }
          break;

          case Module_3:
          {
              switch (channel)
              {
                  case 0:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinClear(GPIO_PTF6);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinSet(GPIO_PTF6); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinSet(GPIO_PTF7);
                      GPIO_PinClear(GPIO_PTF6);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinSet(GPIO_PTF7);
                      GPIO_PinSet(GPIO_PTF6); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinSet(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinClear(GPIO_PTF6);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinSet(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinSet(GPIO_PTF6);
                  }
                  break;

                  default: break;
              }
          }
          break;

          case Module_4:
          {
              switch (channel)
              {
                  case 0:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinClear(GPIO_PTB2);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinSet(GPIO_PTB2); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinSet(GPIO_PTB3);
                      GPIO_PinClear(GPIO_PTB2);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinSet(GPIO_PTB3);
                      GPIO_PinSet(GPIO_PTB2); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinSet(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinClear(GPIO_PTB2);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinSet(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinSet(GPIO_PTB2);
                  }
                  break;

                  default: break;
              }
          }
          break;

          default: break;
      }

      return 0;
  }






  /**
    * @brief   Initialize all corresponding control pins about equalization modules
    *          and configure them in default value.
    * @param   None
    * @returns 0, Calling successfully.
    *          -1, Calling failed.
  */
  int EqualizationModulePins_Initial(void)
  {
      uint8_t i;
      int ret;

      /* Configure 8.5V voltage output control pin as GPIO output mode. */
      _8V5Voltage_ControlPin_Output();

      /* Disable 8.5V voltage output first when power on reset. */
      _8V5Voltage_Disable();



      /* Configure the all four equalization modules's direction pin as GPIO output mode. */
      for (i = 1; i <= 4; i++)
      {
          ret = EqualizationModule_DirectionPins_Init((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }

      /* Then pull down the direction control pin by default.Because the 8.5V voltage
         output is disabled,the following statement's operation do not work. */
      for (i = 1; i <= 4; i++)
      {
          ret = EqualizationModule_EnterChargeMode((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }



       /* Configure the all four equalization modules's fault signal pin as GPIO input mode by default. */
      for (i = 1; i <= 4; i++)
      {
          ret = EqualizationModule_FaultSignalPins_Init((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }



      /* Configure the all four 74HC138 chips's ENABLE pins as GPIO output mode by default. */
      for (i = 1; i <= 4; i++)
      {
          ret = _74HC138_EnablePins_Init((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }

      /* Then disable all the four 74HC138 chips's ENABLE pins by default. */
      for (i = 1; i <= 4; i++)
      {
          ret = _74HC138_Disable((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }



      /* Configure all the four 74HC138 chips's binary number input pins as GPIO input mode. */
      for (i = 1; i <= 4; i++)
      {
          ret = _74HC138_ABCInputPins_Init(ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }

      return 0;
  }