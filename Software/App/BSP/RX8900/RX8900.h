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

#define RX8900_WEEK_SUN                                   (0x00U)
#define RX8900_WEEK_MON                                   (0x01U)
#define RX8900_WEEK_TUE                                   (0x02U)
#define RX8900_WEEK_WED                                   (0x04U)
#define RX8900_WEEK_THU                                   (0x08U)
#define RX8900_WEEK_FRI                                   (0x10U)
#define RX8900_WEEK_SAT                                   (0x20U)

#define RX8900_SEC_MAX                                    (59U)
#define RX8900_MIN_MAX                                    (59U)
#define RX8900_HOUR_MAX                                   (23U)
#define RX8900_WEEK_MAX                                   (RX8900_WEEK_SAT)
#define RX8900_DAY_MAX                                    (31U)
#define RX8900_MON_MAX                                    (12U)
#define RX8900_YEAR_MAX                                   (99U)
/* Run_time error flag */
#define RX8900_VDET_ERR                                   (0x01U)
#define RX8900_VLF_ERR                                    (0x02U)
/* Run_time Event flag */
#define RX8900_WAKEUP_EVENT                               (0x04U)
#define RX8900_UPTATA_EVENT                               (0x08U)
#define RX8900_ALARM_EVENT                                (0x10U)

#define RX8900_READ_REGISTER_LIST(Register, pData, Len)   RX8900_IIC_Transmit               \
                                                          (                                 \
                                                            RX8900_ADDRESS,                 \
                                                            (Register),                     \
                                                            0U,                             \
                                                            (pData),                        \
                                                            (Len),                          \
                                                            RX8900_READ_CMD                 \
                                                          ); 

#define RX8900_READ_REGISTER(Register, pData)             RX8900_IIC_Transmit               \
                                                          (                                 \
                                                            RX8900_ADDRESS,                 \
                                                            (Register),                     \
                                                            0U,                             \
                                                            (pData),                        \
                                                            1U,                             \
                                                            RX8900_READ_CMD                 \
                                                          );       

#define RX8900_WRITE_REGISTER(Register, Data)             RX8900_IIC_Transmit               \
                                                          (                                 \
                                                            RX8900_ADDRESS,                 \
                                                            (Register),                     \
                                                            (Data),                         \
                                                            NULL_PTR,                       \
                                                            1U,                             \
                                                            RX8900_WRITE_CMD                \
                                                          );     

#define RX8900_BCD_TO_DEC(bcd) (((bcd) & 0xFU) + (((bcd) >> 4U) & 0xFU) * 10U)
#define RX8900_DEC_TO_BCD(dec) ((((dec) / 10U) << 4U) + ((dec) % 10U))


extern uint8 gRX8900RuntimeNotify;
extern RX8900TimeType gRX8900TimeInfo;

extern Std_ReturnType RX8900_Init(void);
extern Std_ReturnType RX8900_Main_Fun(void);
extern Std_ReturnType RX8900_Set_Time(RX8900TimeType Time);
extern Std_ReturnType RX8900_Updata_Time(RX8900TimeType *pTime);
extern void RX8900_Process_ISR(void);

#endif