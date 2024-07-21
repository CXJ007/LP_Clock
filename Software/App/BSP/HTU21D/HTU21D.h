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
#ifndef HTU21D_H
#define HTU21D_H
/**********************************************************************************
***********************************Includes****************************************
**********************************************************************************/
#include "HTU21D.h"
#include "HTU21D_Cbk.h"
/**********************************************************************************
*******************************Macro Definitions***********************************
**********************************************************************************/
#define HTU21D_ADDRESS                                    (0x80U)

#define HTU21D_TEMP_HOLD_CMD                              (0xE3U)
#define HTU21D_HUM_HOLD_CMD                               (0xE5U)
#define HTU21D_TEMP_NOHOLD_CMD                            (0xF3U)
#define HTU21D_HUM_NOHOLD_CMD                             (0xF5U)
#define HTU21D_SOFT_REST_CMD                              (0xFEU)


#define HTU21D_UPDATA_NONE                                (0x00U)
#define HTU21D_UPDATA_TEMP                                (0x01U)
#define HTU21D_UPDATA_HUM                                 (0x02U)


#define HTU21D_BUFFER_LEN                                 (3U)






#define HTU21D_WRITE_CMD(Register)                        HTU21D_IIC_Write        \
                                                          (                       \
                                                            HTU21D_ADDRESS,       \
                                                            (Register)            \
                                                          ); 

#define HTU21D_READ_DATA(pData, Len)                      HTU21D_IIC_Read         \
                                                          (                       \
                                                            HTU21D_ADDRESS,       \
                                                            (pData),              \
                                                            (Len)                 \
                                                          ); 

#define HTU21D_DATA_TO_TEMP(data)                         ((175.72 * ((float32)data)\
																													/ 65536) - 46.85)

#define HTU21D_DATA_TO_HUM(data)                         ((125* ((float32)data)    \
																													/ 65536) - 6)

/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
Std_ReturnType HTU21D_Init(void);
Std_ReturnType HTU21D_MainFunc(uint32 CycleMs);

#endif