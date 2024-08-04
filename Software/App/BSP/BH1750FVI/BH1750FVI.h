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
#ifndef BH1750FVI_H
#define BH1750FVI_H
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "BH1750FVI_Cbk.h"
/*******************************************************************************
*******************************Macro Definitions********************************
*******************************************************************************/

#define BH1750FVI_ADDRESS                                      (0x46U)

#define BH1750FVI_POWER_DOWN_CMD                               (0x00U)
#define BH1750FVI_POWER_ON_CMD                                 (0x01U)
#define BH1750FVI_SOFT_RESET_CMD                               (0x07U)
#define BH1750FVI_CONTIN_1P0_CMD                               (0x10U)
#define BH1750FVI_CONTIN_0P5_CMD                               (0x11U)
#define BH1750FVI_CONTIN_4P0_CMD                               (0x13U)
#define BH1750FVI_ONCE_1P0_CMD                                 (0x20U)
#define BH1750FVI_ONCE_0P5_CMD                                 (0x21U)
#define BH1750FVI_ONCE_4P0_CMD                                 (0x23U)

#define BH1750FVI_BUFFER_LEN                                   (2U)

#define BH1750FVI_ACCURACY                                     (1.2F)

#define BH1750FVI_DELAY_MS                                     (200U)

#define BH1750FVI_UPDATA_STOP                                  (0x00U)
#define BH1750FVI_UPDATA_RUN                                   (0x01U)

#define BH1750FVI_WRITE_CMD(Register)                                          \
                        BH1750FVI_IIC_Write(BH1750FVI_ADDRESS, (Register));

#define BH1750FVI_READ_DATA(pData, Len)                                        \
                        BH1750FVI_IIC_Read(BH1750FVI_ADDRESS, (pData), (Len));
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
extern Std_ReturnType BH1750FVI_Init(void);
extern Std_ReturnType BH1750FVI_MainFunc(uint32 CycleMs);
#endif