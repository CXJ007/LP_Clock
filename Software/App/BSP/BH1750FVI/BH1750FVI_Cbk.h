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
#ifndef BH1750FVI_CBK_H
#define BH1750FVI_CBK_H
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
extern Std_ReturnType BH1750FVI_IIC_Write
(
    const uint8 Address,
    const uint8 Register
);
extern Std_ReturnType BH1750FVI_IIC_Read
(
    const uint8 Address, 
    uint8* const pData,
    const uint8 Len
);
extern void           BH1750FVI_WriteDVI(uint8 PinStatu);
extern void           BH1750FVI_DelayMs(uint32 Ms);
extern uint32         BH1750FVI_Get_TicksMs(void);
#endif
