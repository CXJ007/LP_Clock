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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef BUZZER_CBK_H
#define BUZZER_CBK_H
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
extern void           Buzzer_On(void);
extern void           Buzzer_Off(void);
extern void           Buzzer_Toggle(void);
extern Std_ReturnType Buzzer_CreatTimer(void (*CalloutFunc)(void), uint32 Ms);
extern Std_ReturnType Buzzer_ActivteTimer(void);
extern Std_ReturnType Buzzer_DeactivateTimer(void);
#endif
