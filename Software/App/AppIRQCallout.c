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
#include "gpio.h"
#include "lptim.h"
#include "RX8900.h"
#include "AppMain.h"
#include "AppLowPowerMgr.h"
/**********************************************************************************
*************************Interrupt Function Definitions****************************
**********************************************************************************/
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_PIN_1 == GPIO_Pin)
  {
		if(APP_MOUDLE_LP == gApp_Moudle)
		{
			gLP_WakeupSource |= LP_WAKEUP_RTC;
		}
		else
		{
			/* nothing */
		}
    RX8900_Process_ISR();
  }
  
}
/**********************************************************************************
* Function name :
* Inputs        :
* Return        :
* description   :
* Limitation    :
**********************************************************************************/
uint32 lpcount = 0;

void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
  lpcount++;
	gLP_WakeupSource |= LP_WAKEUP_LPTIM;
  HAL_LPTIM_Counter_Stop_IT(hlptim);
}
