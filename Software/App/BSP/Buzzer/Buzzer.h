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
#ifndef BUZZER_H
#define BUZZER_H
/**********************************************************************************
***********************************Includes****************************************
**********************************************************************************/
#include "Buzzer_Cbk.h"
/**********************************************************************************
*******************************Macro Definitions***********************************
**********************************************************************************/
#define BUZZER_CYCLE_MS                       (500U)

#define BUZZER_STATU_STOP                     (0x00U)
#define BUZZER_STATU_RUN                      (0x01U)

#define BUZZER_MODE_KEEP                      (0x00U)
#define BUZZER_MODE_FLICKER                   (0x01U)
/**********************************************************************************
*******************************Struct Definitions**********************************
**********************************************************************************/
typedef struct
{
    uint8 Mode;
    uint8 Statu;
    uint32 Cycle;
    uint32 LoopCount;
} BuzzerInfo_Type;
/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
extern Std_ReturnType Buzzer_Init(void);
extern Std_ReturnType Buzzer_ActiveOnce(void);
extern Std_ReturnType Buzzer_ActiveForever(void);
extern Std_ReturnType Buzzer_Active(uint32 LoopCount);
extern void Buzzer_Abort(void);
#endif
