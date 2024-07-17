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
#ifndef APPLOWPOWERMGR_H
#define APPLOWPOWERMGR_H
/**********************************************************************************
************************************Includes***************************************
**********************************************************************************/
#include "Std_Types.h"
/**********************************************************************************
*******************************Macro Definitions***********************************
**********************************************************************************/
#define LP_TIMTEOUT_VALUE                 (0xFFFFU)
/* Wakeup source */
#define LP_WAKEUP_SOURCE_NONE             (0x00U)
#define LP_WAKEUP_SOURCE_LPTIM            (0x01U)
#define LP_WAKEUP_SOURCE_RTC              (0x02U)
/* Wakeup mode */
#define LP_WAKEUP_MODE_LPTIM              (0x01U)
#define LP_WAKEUP_MODE_RTC                (0x02U)
#define LP_WAKEUP_MODE_NORMAL             (LP_WAKEUP_MODE_LPTIM                   \
                                          | LP_WAKEUP_MODE_RTC)

/**********************************************************************************
****************************Global variable Definitions****************************
**********************************************************************************/
extern uint32 gLP_Ticks;
extern uint8 gLP_WakeupSource;
/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
extern void LPower_Enter(void);
extern void LPower_Exit(void);
#endif