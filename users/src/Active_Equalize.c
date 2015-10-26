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
  #include "Delay.h"



  static EqualizeModuleStatus_TypeDef Starting_Equalizing;



  /**
    * @brief   Initialize 8.5V voltage output control pin as GPIO output mode.
    * @param   None
    * @returns None
  */
  static void _8V5Voltage_ControlPin_Output(void)
  {
      GPIO_PinInit(GPIO_PTI1, GPIO_PinOutput);
  }



  /**
    * @brief   Enable 8.5V voltage output.
    * @param   None
    * @returns None
  */
  void _8V5Voltage_Enable(void)
  {
      GPIO_PinSet(GPIO_PTI1);
  }



  /**
    * @brief   Disable 8.5V voltage output.
    * @param   None
    * @returns None
  */
  void _8V5Voltage_Disable(void)
  {
      GPIO_PinClear(GPIO_PTI1);
  }



  /**
    * @brief   Initialize the specified equalization module direction control pins as 
    *          GPIO output mode.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
  static int EqualizationModule_DirectionPins_Init(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinInit(GPIO_PTI0, GPIO_PinOutput); break;

          case Module_2: GPIO_PinInit(GPIO_PTH0, GPIO_PinOutput_HighCurrent); break;

          case Module_3: GPIO_PinInit(GPIO_PTE5, GPIO_PinOutput); break;

          case Module_4: GPIO_PinInit(GPIO_PTB1, GPIO_PinOutput); break;

          default: break;
      }

      return 0;
  }



  /**
    * @brief   Pull down equalization module direction control pin and make it 
    *          entering charge mode.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
  static int EqualizationModule_EnterChargeMode(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinClear(GPIO_PTI0); break;

          case Module_2: GPIO_PinClear(GPIO_PTH0); break;

          case Module_3: GPIO_PinClear(GPIO_PTE5); break;

          case Module_4: GPIO_PinClear(GPIO_PTB1); break;

          default: break;
      }

      return 0;
  }



  /**
    * @brief   Pull up equalization module direction control pin and make it 
    *          entering charge mode.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
  static int EqualizationModule_EnterDischargeMode(ModuleNumber_TypeDef Module_Num)
  {
      if ((Module_Num < Module_1) || (Module_Num > Module_4))return -1;

      switch (Module_Num)
      {
          case Module_1: GPIO_PinSet(GPIO_PTI0); break;

          case Module_2: GPIO_PinSet(GPIO_PTH0); break;

          case Module_3: GPIO_PinSet(GPIO_PTE5); break;

          case Module_4: GPIO_PinSet(GPIO_PTB1); break;

          default: break;
      }

      return 0;
  }



  /**
    * @brief   Initialize equalization modules's fault signal pins as GPIO input mode.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
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



  /**
    * @brief   Initializa 74HC138 enable pins as GPIO output mode.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
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



  /**
    * @brief   Enable the specified 74HC138 chip.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
  static int _74HC138_Disable(ModuleNumber_TypeDef Module_Num)
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



  /**
    * @brief   Disable the specified 74HC138 chip.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
  static int _74HC138_Enable(ModuleNumber_TypeDef Module_Num)
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



  /**
    * @brief   Initialize all 74HC138 chips input signals as GPIO input mode.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
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



  /**
    * @brief   Select the specified cell channel number and equalization module's number
    *          and assign the corresponding input value to 74HC138 chip.
    * @param   Module_Num, The specified equalize module number.
    *          channel, The selected cell channel number.
    * @returns 0, Calling successfully.
    *         -1, Calling failed.The given parameter is error!
  */
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
                      GPIO_PinSet(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinSet(GPIO_PTD5);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinSet(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinClear(GPIO_PTD5); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinSet(GPIO_PTD6);
                      GPIO_PinSet(GPIO_PTD5);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinSet(GPIO_PTD6);
                      GPIO_PinClear(GPIO_PTD5); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinSet(GPIO_PTD5);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinClear(GPIO_PTD7);
                      GPIO_PinClear(GPIO_PTD6);
                      GPIO_PinClear(GPIO_PTD5);
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
                      GPIO_PinSet(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinSet(GPIO_PTC0);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinSet(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinClear(GPIO_PTC0); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinSet(GPIO_PTC1);
                      GPIO_PinSet(GPIO_PTC0);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinSet(GPIO_PTC1);
                      GPIO_PinClear(GPIO_PTC0); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinSet(GPIO_PTC0);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinClear(GPIO_PTI5);
                      GPIO_PinClear(GPIO_PTC1);
                      GPIO_PinClear(GPIO_PTC0);
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
                      GPIO_PinSet(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinSet(GPIO_PTF6);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinSet(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinClear(GPIO_PTF6); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinSet(GPIO_PTF7);
                      GPIO_PinSet(GPIO_PTF6);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinSet(GPIO_PTF7);
                      GPIO_PinClear(GPIO_PTF6); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinSet(GPIO_PTF6);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinClear(GPIO_PTH3);
                      GPIO_PinClear(GPIO_PTF7);
                      GPIO_PinClear(GPIO_PTF6);
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
                      GPIO_PinSet(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinSet(GPIO_PTB2);
                  }
                  break;

                  case 1:
                  {
                      GPIO_PinSet(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinClear(GPIO_PTB2); 
                  }
                  break;

                  case 2:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinSet(GPIO_PTB3);
                      GPIO_PinSet(GPIO_PTB2);  
                  }
                  break;

                  case 3:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinSet(GPIO_PTB3);
                      GPIO_PinClear(GPIO_PTB2); 
                  }
                  break;

                  case 4:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinSet(GPIO_PTB2);
                  }
                  break;

                  case 5:
                  {
                      GPIO_PinClear(GPIO_PTF4);
                      GPIO_PinClear(GPIO_PTB3);
                      GPIO_PinClear(GPIO_PTB2);
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
    * @brief   Read and get equalization modules's fault signal pins status value.
    * @param   Module_Num, The specified equalize module number.
    * @returns 0, equalization module status is normal.
    *          1, equalization module status is error.
    *         0xFF, the given parameter is error.
  */
  uint8_t EqualizationModule_GetFaultSignalValue(ModuleNumber_TypeDef Module_Num)
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



  /**
    * @brief   Initialize all corresponding control pins which communicate with equalization modules
    *          and assign them with default value.
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


      /* Configure all the four equalization modules's direction pin as GPIO output mode. */
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


       /* Configure all the four equalization modules's fault signal pin as GPIO input mode by default. */
      for (i = 1; i <= 4; i++)
      {
          ret = EqualizationModule_FaultSignalPins_Init((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }


      /* Configure all the four 74HC138 chips's ENABLE pins as GPIO output mode by default. */
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
          ret = _74HC138_ABCInputPins_Init((ModuleNumber_TypeDef)i);

          if (ret != 0)return -1;
      }

      return 0;
  }



  /**
    * @brief   Open the specified equalization module number with specified cell channel number
    *          and start equalizing.
    * @param   Eq_Mode, User specified equalize mode.Which can be set with Charge_Equalize or Discharge_Equalize.
    *          Eq_Ch, User specified equalize cell channel number.
    * @returns 0, Calling successfully.
    *          -1, Calling failed.
  */
  int EqualizationModule_Open(EqualizeMode_TypeDef Eq_Mode, EqualizeChannel_TypeDef Eq_Ch)
  {
      uint8_t temp_ch;
      int ret_val;

      if ((Eq_Mode < Charge_Equalize) || (Eq_Mode > Discharge_Equalize))return -1;

      if ((Eq_Ch < Cell_Channel_1) || (Eq_Ch > Cell_Channel_24))return -1;

      /* Before configure the equalization modules and initialize the parameters,user should
         disable 8.5V voltage output first. */
      _8V5Voltage_Disable();

      /* If the equalization mode is charging. */
      if (Eq_Mode == Charge_Equalize)
      {
          if ((Eq_Ch >= Cell_Channel_1) && (Eq_Ch <= Cell_Channel_6))
          {
              /* First, disable 74HC138 enable pin and disable output. */
              ret_val = _74HC138_Disable(Module_1);

              if (ret_val != 0)return -1;

              /* Then, pull down equalization module's direction control pin
                 to make it enter charge mode. */
              ret_val = EqualizationModule_EnterChargeMode(Module_1);

              if (ret_val != 0)return -1;

              /* Calculate the selected channel number for getting ready to assign it. */
              temp_ch = (uint8_t)Eq_Ch - 1;

              ret_val = _74HC138_ChannelSelect(Module_1, temp_ch);

              if (ret_val != 0)return -1;

              /* Last, enable 74HC138 output function. */
              ret_val = _74HC138_Enable(Module_1);

              if (ret_val != 0)return -1;

              /* Set the module's equalization status flag with 0xCC. */
              Starting_Equalizing.Module1_Status = 0xCCu;
          }
          else if ((Eq_Ch >= Cell_Channel_7) && (Eq_Ch <= Cell_Channel_12))
          {
              ret_val = _74HC138_Disable(Module_2);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterChargeMode(Module_2);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 6 - 1;

              ret_val = _74HC138_ChannelSelect(Module_2, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_2);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module2_Status = 0xCCu;
          }
          else if ((Eq_Ch >= Cell_Channel_13) && (Eq_Ch <= Cell_Channel_18))
          {
              ret_val = _74HC138_Disable(Module_3);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterChargeMode(Module_3);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 12 - 1;

              ret_val = _74HC138_ChannelSelect(Module_3, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_3);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module3_Status = 0xCCu;
          }
          else if ((Eq_Ch >= Cell_Channel_19) && (Eq_Ch <= Cell_Channel_24))
          {
              ret_val = _74HC138_Disable(Module_4);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterChargeMode(Module_4);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 18 - 1;

              ret_val = _74HC138_ChannelSelect(Module_4, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_4);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module4_Status = 0xCCu;
          }
          else
          {
              return -1;
          }
      }
      /* Or the equalization mode is discharging. */
      else
      {
          if ((Eq_Ch >= Cell_Channel_1) && (Eq_Ch <= Cell_Channel_6))
          {
              ret_val = _74HC138_Disable(Module_1);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterDischargeMode(Module_1);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 1;

              ret_val = _74HC138_ChannelSelect(Module_1, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_1);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module1_Status = 0xCCu;
          }
          else if ((Eq_Ch >= Cell_Channel_7) && (Eq_Ch <= Cell_Channel_12))
          {
              ret_val = _74HC138_Disable(Module_2);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterDischargeMode(Module_2);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 6 - 1;

              ret_val = _74HC138_ChannelSelect(Module_2, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_2);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module2_Status = 0xCCu;
          }
          else if ((Eq_Ch >= Cell_Channel_13) && (Eq_Ch <= Cell_Channel_18))
          {
              ret_val = _74HC138_Disable(Module_3);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterDischargeMode(Module_3);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 12 - 1;

              ret_val = _74HC138_ChannelSelect(Module_3, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_3);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module3_Status = 0xCCu;
          }
          else if ((Eq_Ch >= Cell_Channel_19) && (Eq_Ch <= Cell_Channel_24))
          {
              ret_val = _74HC138_Disable(Module_4);

              if (ret_val != 0)return -1;

              ret_val = EqualizationModule_EnterDischargeMode(Module_4);

              if (ret_val != 0)return -1;

              temp_ch = (uint8_t)Eq_Ch - 18 - 1;

              ret_val = _74HC138_ChannelSelect(Module_4, temp_ch);

              if (ret_val != 0)return -1;

              ret_val = _74HC138_Enable(Module_4);

              if (ret_val != 0)return -1;

              Starting_Equalizing.Module4_Status = 0xCCu;
          }
          else
          {
              return -1;
          }
      }

      /* After all the configuration parameter is assigned, enable the 8.5V voltage output. */
      _8V5Voltage_Enable();

      return 0;
  }



  /**
    * @brief   Close the specified equalization module number with specified cell channel number
    *          and stop equalizing.
    * @param   Eq_Mode, User specified equalize mode.Which can be set with Charge_Equalize or Discharge_Equalize.
    *          Eq_Ch, User specified equalize cell channel number.
    * @returns 0, Calling successfully.
    *          -1, Calling failed.
  */
  int EqualizationModule_Close(EqualizeChannel_TypeDef Eq_Ch)
  {
      int ret_val;

      if ((Eq_Ch < Cell_Channel_1) || (Eq_Ch > Cell_Channel_24))return -1;

      /* Before configure the equalization modules and initialize the parameters,user should
         disable 8.5V voltage output first. */
      _8V5Voltage_Disable();

      Delay10us(500);

      if ((Eq_Ch >= Cell_Channel_1) && (Eq_Ch <= Cell_Channel_6))
      {
          ret_val = _74HC138_Disable(Module_1);

          if (ret_val != 0)return -1;        

          ret_val = EqualizationModule_EnterChargeMode(Module_1);

          if (ret_val != 0)return -1;

          Starting_Equalizing.Module1_Status = 0;
      }
      else if ((Eq_Ch >= Cell_Channel_7) && (Eq_Ch <= Cell_Channel_12))
      {
          ret_val = _74HC138_Disable(Module_2);

          if (ret_val != 0)return -1;        

          ret_val = EqualizationModule_EnterChargeMode(Module_2);

          if (ret_val != 0)return -1;

          Starting_Equalizing.Module2_Status = 0;
      }
      else if ((Eq_Ch >= Cell_Channel_13) && (Eq_Ch <= Cell_Channel_18))
      {
          ret_val = _74HC138_Disable(Module_3);

          if (ret_val != 0)return -1;        

          ret_val = EqualizationModule_EnterChargeMode(Module_3);

          if (ret_val != 0)return -1;

          Starting_Equalizing.Module3_Status = 0;
      }
      else if ((Eq_Ch >= Cell_Channel_19) && (Eq_Ch <= Cell_Channel_24))
      {
          ret_val = _74HC138_Disable(Module_4);

          if (ret_val != 0)return -1;        

          ret_val = EqualizationModule_EnterChargeMode(Module_4);

          if (ret_val != 0)return -1;

          Starting_Equalizing.Module4_Status = 0;
      }
      else
      {
          return -1;
      }

      return 0;
  }
  


  /**
    * @brief   Read and get the specified equalization module's status.
    * @param   Eq_Num, The specified equalization module number.
    * @returns The specified equalization module equalizing status value.
  */
  uint8_t Getting_EqualizingStatusValue(ModuleNumber_TypeDef Eq_Num)
  {
      uint8_t ret_val;

      switch (Eq_Num)
      {
          case Module_1: ret_val = Starting_Equalizing.Module1_Status; break;

          case Module_2: ret_val = Starting_Equalizing.Module2_Status; break;

          case Module_3: ret_val = Starting_Equalizing.Module3_Status; break;

          case Module_4: ret_val = Starting_Equalizing.Module4_Status; break;

          default: break;
      }

      return (ret_val);
  }



  /**
    * @brief   Assign the specified equalization module's status with a given value.
    * @param   Eq_Num, The specified equalization module number.
    *          val, The given value which will be assigned with.
    * @returns None.
  */
  void Settiing_EqualizingStatusValue(ModuleNumber_TypeDef Eq_Num, uint8_t val)
  {
      switch (Eq_Num)
      {
          case Module_1: Starting_Equalizing.Module1_Status = val; break;

          case Module_2: Starting_Equalizing.Module2_Status = val; break;

          case Module_3: Starting_Equalizing.Module3_Status = val; break;

          case Module_4: Starting_Equalizing.Module4_Status = val; break;

          default: break;
      }
  }

  /*****************************END OF FILE**************************************/