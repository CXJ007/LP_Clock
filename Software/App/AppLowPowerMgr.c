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
#include "AppLowPowerMgr.h"
#include "main.h"
#include "i2c.h"
/**********************************************************************************
****************************Global variable Definitions****************************
**********************************************************************************/
uint32 gLPtick;
/**********************************************************************************
*****************************Local Function Definitions****************************
**********************************************************************************/
static Std_ReturnType LPower_Sysclock_Set(void)
{
  Std_ReturnType RetVal;
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	
  /* Configure the main internal regulator output voltage */
  if((HAL_OK == HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1))
      && (HAL_OK == HAL_RCC_DeInit()))
  {
    /* Open MSI clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
    RetVal = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
	}
  else
  {
    RetVal = E_NOT_OK;
  }

  return RetVal;
}

static void LPower_SysTickTick_Set(uint32 TickMs)
{
  if(TickMs!=0) 
  {
    SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
    /* set reload register */
    SysTick->LOAD = (uint32_t)(SystemCoreClock / 1000U * TickMs - 1U);                         
    SysTick->VAL = 0U;                                          
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 
  }
}

uint32 sysclock1;
extern void SystemClock_Config(void);
Std_ReturnType App_Sysclock_Restore(void)
{
  Std_ReturnType RetVal = E_OK;

  SystemClock_Config();
	/* Set systick 1khz  */
  SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
  SysTick->LOAD = (uint32_t)(SystemCoreClock / 1000U - 1U);                         /* set reload register */
  SysTick->VAL = 0U;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk    \
                  | SysTick_CTRL_TICKINT_Msk    \
                  | SysTick_CTRL_ENABLE_Msk;
  sysclock1 = HAL_RCC_GetSysClockFreq();

  return RetVal;
}

/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
uint32 sysclock0;
extern uint32_t HAL_RCC_GetSysClockFreq(void);
void LPower_Enter(void)
{
  Std_ReturnType RetVal = E_OK;

  RetVal |= HAL_I2C_DeInit(&hi2c1);
  RetVal |= LPower_Sysclock_Set();
  LPower_SysTickTick_Set(gLPtick);
  sysclock0 = HAL_RCC_GetSysClockFreq();

  HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
//	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

