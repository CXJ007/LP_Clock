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
#ifndef RX8900_H
#define RX8900_H


#include "RX8900_Cbk.h"

#define RX8900_ADDRESS                                    (0x64U)
#define RX8900_WRITE_CMD                                  (0x0U)
#define RX8900_READ_CMD                                   (0x1U)


#define RX8900_READ_REGISTER(Register, pData, Len)        RX8900_IIC_Transmit               \
                                                          (                                 \
                                                            RX8900_ADDRESS,                 \
                                                            (Register),                     \
                                                            0U,                             \
                                                            (pData),                        \
                                                            (Len),                          \
                                                            RX8900_READ_CMD                 \
                                                          );     

#define RX8900_WRITE_REGISTER(Register, Data, Len)        RX8900_IIC_Transmit               \
                                                          (                                 \
                                                            RX8900_ADDRESS,                 \
                                                            (Register),                     \
                                                            (Data),                         \
                                                            NULL_PTR,                       \
                                                            (Len),                          \
                                                            RX8900_WRITE_CMD                \
                                                          );     


extern Std_ReturnType RX8900_Init(void);

#if (RX8900_DEBUG == STD_ON)

extern void RX8900_Debug(void);

#endif

#endif