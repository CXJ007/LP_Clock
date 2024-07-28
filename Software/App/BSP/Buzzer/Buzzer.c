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
#include "Buzzer.h"
/**********************************************************************************
*****************************Local variable Definitions****************************
**********************************************************************************/
BuzzerInfo_Type iBuzzerInfo = {BUZZER_MODE_FLICKER,
                               BUZZER_STATU_STOP,
                               BUZZER_CYCLE_MS,
                               0x0U};
/**********************************************************************************
*****************************Local Function Definitions****************************
**********************************************************************************/
static void Buzzer_MainFunc(void);
/**********************************************************************************
 * name        :
 * input       :
 * input\output:
 * return      :
 * description ：
 * limit       :
 *********************************************************************************/
static void Buzzer_MainFunc(void)
{
    if ((BUZZER_STATU_RUN == iBuzzerInfo.Statu) && (iBuzzerInfo.LoopCount > 0x0U))
    {
        if (BUZZER_MODE_FLICKER == iBuzzerInfo.Mode)
        {
            Buzzer_Toggle();
        }
        else if (BUZZER_MODE_KEEP == iBuzzerInfo.Mode)
        {
            Buzzer_On();
        }
        else
        {
            /* nothing */
        }
        iBuzzerInfo.LoopCount--;
    }
    else
    {
        Buzzer_Off();
        iBuzzerInfo.Statu = BUZZER_STATU_STOP;
        iBuzzerInfo.LoopCount = 0x0U;
        (void)Buzzer_DeactivateTimer();
    }
}
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
 *********************************************************************************/
Std_ReturnType Buzzer_Init(void)
{
    Std_ReturnType RetVal;

    RetVal = Buzzer_CreatTimer(Buzzer_MainFunc, iBuzzerInfo.Cycle);

    return RetVal;
}

/**********************************************************************************
 * name        :
 * input       :
 * input\output:
 * return      :
 * description ：
 * limit       :
 *********************************************************************************/
Std_ReturnType Buzzer_ActiveOnce(void)
{
    iBuzzerInfo.Statu = BUZZER_STATU_RUN;
    iBuzzerInfo.LoopCount = 1;

    return Buzzer_ActivteTimer();
}

/**********************************************************************************
 * name        :
 * input       :
 * input\output:
 * return      :
 * description ：
 * limit       :
 *********************************************************************************/
Std_ReturnType Buzzer_ActiveForever(void)
{
    iBuzzerInfo.Statu = BUZZER_STATU_RUN;
    iBuzzerInfo.LoopCount = 0xFFFFFFFFU;
    
    return Buzzer_ActivteTimer();
}

/**********************************************************************************
 * name        :
 * input       :
 * input\output:
 * return      :
 * description ：
 * limit       :
 *********************************************************************************/
Std_ReturnType Buzzer_Active(uint32 LoopCount)
{
    iBuzzerInfo.Statu = BUZZER_STATU_RUN;
    iBuzzerInfo.LoopCount = LoopCount;

    return Buzzer_ActivteTimer();
}

/**********************************************************************************
 * name        :
 * input       :
 * input\output:
 * return      :
 * description ：
 * limit       :
 *********************************************************************************/
void Buzzer_Abort(void)
{
    iBuzzerInfo.Statu = BUZZER_STATU_STOP;
}
