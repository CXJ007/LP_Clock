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
***********************************Includes****************************************
**********************************************************************************/
#include "Buzzer_Cbk.h"
#include "gpio.h"
#include "tx_api.h"
/**********************************************************************************
*****************************Local variable Definitions****************************
**********************************************************************************/
static TX_TIMER iBuzzerTimer;
/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
void Buzzer_On(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}

/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
void Buzzer_Off(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
}

/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
void Buzzer_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
}

/**********************************************************************************
 * name        :
 * input       :
 * input\output:
 * return      :
 * description ：
 * limit       :
 *********************************************************************************/
Std_ReturnType Buzzer_CreatTimer(void (*CalloutFunc)(void), uint32 Ms)
{
    return (Std_ReturnType)tx_timer_create(&iBuzzerTimer,                         \
                                           "BuzzerTimer",                         \
                                           (void (*)(ULONG))CalloutFunc,          \
                                           0U,                                    \
                                           1U,                                    \
                                           MS_TO_TICKS(Ms),                       \
                                           TX_NO_ACTIVATE);                     
}

/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
Std_ReturnType Buzzer_ActivteTimer(void)
{
	return (Std_ReturnType)tx_timer_activate(&iBuzzerTimer);
}

/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
Std_ReturnType Buzzer_DeactivateTimer(void)
{
	return (Std_ReturnType)tx_timer_deactivate(&iBuzzerTimer);
}