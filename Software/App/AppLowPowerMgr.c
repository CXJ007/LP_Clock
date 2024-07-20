/*
 * MIT License
 * Copyright (c) 2024 CXJ007
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**********************************************************************************
************************************Includes***************************************
**********************************************************************************/
#include "main.h"
#include "i2c.h"
#include "lptim.h"
#include "AppMain.h"
#include "AppLowPowerMgr.h"
#include "AppModeCtrl.h"
#include "RX8900.h"
/**********************************************************************************
****************************Global variable Definitions****************************
**********************************************************************************/
uint32 gLP_Ticks = 0x0U;
uint8 gLP_WakeupSource = LP_WAKEUP_SOURCE_NONE;
uint8 gLP_WakeupMode= LP_WAKEUP_MODE_NORMAL;
/**********************************************************************************
*****************************Local Function Definitions****************************
**********************************************************************************/
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
static Std_ReturnType LPower_Peripheral_Set(void)
{
  Std_ReturnType RetVal = E_OK;

  RetVal |= HAL_I2C_DeInit(&hi2c1);
  RetVal |= HAL_LPTIM_Init(&hlptim1);
  
  return RetVal;
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
static Std_ReturnType LPower_Peripheral_Restore(void)
{
  Std_ReturnType RetVal = E_OK;

  RetVal |= HAL_LPTIM_DeInit(&hlptim1);
  RetVal |= HAL_I2C_Init(&hi2c1);
  
  return RetVal;
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
static Std_ReturnType LPower_Sysclock_Set(void)
{
  uint32 Timeout;
  Std_ReturnType RetVal = E_OK;

  /* Enable MSI */
  SET_BIT(RCC->CR, RCC_CR_MSION);
  /* Wait till MSI is ready */
  Timeout = LP_TIMTEOUT_VALUE;
  while((READ_BIT(RCC->CR, RCC_CR_MSIRDY) == 0x0U) && (Timeout > 0x0U))
  {
    Timeout--;
  }
  if(Timeout != 0x0U)
  {
    /* MSIRANGE can be modified when MSI is OFF (MSION=0) 
		or when MSI is ready (MSIRDY=1) */
    if((READ_BIT(RCC->CR, RCC_CR_MSION) != RCC_CR_MSION)                           \
			|| (READ_BIT(RCC->CR, RCC_CR_MSIRDY) == RCC_CR_MSIRDY))
    {
        /* Set MSI clock 1Mhz*/
        WRITE_REG((RCC->CR), (((READ_REG(RCC->CR))                                 \
										& (~(RCC_CR_MSIRANGE | RCC_CR_MSIRGSEL)))                      \
                    | (RCC_CR_MSIRANGE_4 | RCC_CR_MSIRGSEL_Msk)));
        /* Selected MSI as system clock  */
        WRITE_REG((RCC->CFGR), (((READ_REG(RCC->CFGR))                             \
										& (~(RCC_CFGR_SW))) | (RCC_CFGR_SW_MSI)));
        /* Wait till system clock source is ready */
        Timeout = LP_TIMTEOUT_VALUE;
        while((READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_MSI)              \
                                                              && (Timeout > 0x0U))
        {
          Timeout--;
        }
        if(Timeout != 0x0U)
        {
          /* Disable other clock */
          CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_HSIKERON         \
                              | RCC_CR_HSIASFS | RCC_CR_PLLON | RCC_CR_PLLSAI1ON);
        }
        else
        {
          RetVal = E_NOT_OK;
        }
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
static Std_ReturnType LPower_Sysclock_Restore(void)
{
  uint32 Timeout;
  Std_ReturnType RetVal = E_OK;

  /* Enable HSI */
  SET_BIT(RCC->CR, RCC_CR_HSION);
  /* Wait till HSI is ready */
  Timeout = LP_TIMTEOUT_VALUE;
  while((READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0x0U) && (Timeout > 0x0U))
  {
    Timeout--;
  }
  if(Timeout != 0x0U)
  {
    /* PLLM = 1 PLLN = 10 PLLR = 2 */
    WRITE_REG((RCC->PLLCFGR), (((READ_REG(RCC->PLLCFGR))                            \
										& (~(RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN 
                    | RCC_PLLCFGR_PLLR)))                                           \
                    | (RCC_PLLSOURCE_HSI | (0x0U << RCC_PLLCFGR_PLLM_Pos) 
                    | (0xAU << RCC_PLLCFGR_PLLN_Pos) 
                    | (0x0U << RCC_PLLCFGR_PLLR_Pos))));
    /* Enable PLL */
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    /* Wait till PLL is ready */
    Timeout = LP_TIMTEOUT_VALUE;
    while((READ_BIT(RCC->CR, RCC_CR_PLLRDY)== 0x0U) && (Timeout > 0x0U))
    {
      Timeout--;
    }
    if(Timeout != 0x0U)
    {
      /* Enable PLL output */
      SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);
      /* Selected PLL as system clock  */
      WRITE_REG((RCC->CFGR), (((READ_REG(RCC->CFGR))                                \
                              & (~(RCC_CFGR_SW))) | (RCC_CFGR_SW_PLL)));
      /* Wait till system clock source is ready */
      Timeout = LP_TIMTEOUT_VALUE;
      while((READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)                  \
                                                          && (Timeout > 0x0U))
      {
				Timeout--;
      }
      if(Timeout != 0x0U)
      {
        /* Disable MSI */
        CLEAR_BIT(RCC->CR, RCC_CR_MSION);
      }
      else
      {
        RetVal = E_NOT_OK;
      }
    }
    else
    {
      RetVal = E_NOT_OK;
    }
  }
  else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
static void LPower_NextWakeTime_Set(uint32 NextTime)
{
	/* 62.5Khz */
  /* 中断两次暂时无影响 */
  HAL_LPTIM_Counter_Start_IT(&hlptim1, (uint32)(62.5F * (float32)NextTime));
	
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
static uint32 LPower_SleepTime_Get(void)
{
	uint32 Time;
  uint32 CNT;

  CNT = HAL_LPTIM_ReadCounter(&hlptim1);
	Time = (uint32)((float32)CNT / 62.5F);
  
	return Time;
}
/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
uint32 sysclock0,sysclock1;
extern uint32_t HAL_RCC_GetSysClockFreq(void);
void LPower_Enter(void)
{
  Std_ReturnType RetVal = E_OK;

  if(gLP_Ticks != 0x0U)
  {
    /* Disable SysTick */
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
    RetVal |= LPower_Peripheral_Set();
    RetVal |= LPower_Sysclock_Set();
    if(E_OK == RetVal)
    {
			if(LP_WAKEUP_MODE_RTC == gLP_WakeupMode)
			{
				/* 1s interrupt once, after 1040ms wakeup  */
				gLP_Ticks = 1040U;
			}
			else
			{
				/* nothing */
			}
      LPower_NextWakeTime_Set(gLP_Ticks);
			gApp_Moudle = APP_MODE_LP;
      gLP_WakeupSource = LP_WAKEUP_SOURCE_NONE;
      HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
      sysclock0 = HAL_RCC_GetSysClockFreq();
    }
    else
    {
      Error_Handler();
    } 
  }
  else
  {
    /* nothing */
  }
  
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/

void LPower_Exit(void)
{
  uint32 Time;
	Std_ReturnType RetVal;

  if(gLP_Ticks != 0x0U)
  {

		RetVal = (Std_ReturnType)HAL_PWREx_DisableLowPowerRunMode();
    if(E_OK == RetVal)
    {
			if((LP_WAKEUP_SOURCE_LPTIM & gLP_WakeupSource) != LP_WAKEUP_SOURCE_LPTIM)
      {
        gLP_WakeupSource &= (~LP_WAKEUP_SOURCE_LPTIM);
        gLP_Ticks = LPower_SleepTime_Get();
				HAL_LPTIM_Counter_Stop_IT(&hlptim1);
				
      }
			else
			{
				/* nothing */
			}
      RetVal |= LPower_Sysclock_Restore();
      RetVal |= LPower_Peripheral_Restore();
			RetVal |= App_ModeCtrl_SendCmd(0x00000001U);
      if(E_OK == RetVal)
			{
				/* Enable SysTick */
				SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
				if((LP_WAKEUP_SOURCE_RTC & gLP_WakeupSource) == LP_WAKEUP_SOURCE_RTC)
				{
					gLP_WakeupSource &= (~LP_WAKEUP_SOURCE_RTC);
					RX8900_Process_ISR();
				}
				else
				{
					/* nothing */
				}
				gApp_Moudle = APP_MODE_RUN;
				sysclock1 = HAL_RCC_GetSysClockFreq();
			}
			else
			{
				Error_Handler();
			}
    }
    else
    {
      Error_Handler();
    }
  }
  else
  {
    /* nothing */
  }
}