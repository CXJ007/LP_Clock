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
#ifndef RX8900_CFG_H
#define RX8900_CFG_H

#include "RX8900_Types.h"

#define RX8900_DEBUG                  (STD_ON)


#if (RX8900_DEBUG == STD_ON)

#define RX8900_REG_OFFSET              (16U)
#define RX8900_DEBUG_UPDATA_CMD        (0x0U)
#define RX8900_DEBUG_WRITE_CMD         (0x1U)
#define RX8900_DEBUG_NONE_CMD          (0xFU)

typedef struct 
{
  RX8900SecRegType            SecReg;
  RX8900MinRegType            MinReg;
  RX8900HourRegType           HourReg;
  RX8900WeekRegType           WeekReg;
  RX8900DayRegType            DayReg;
  RX8900MonthRegType          MonthReg;
  RX8900YearRegType           YearReg;
  RX8900RamRegType            RamReg;
  RX8900MinAlarmRegType       MinAlarmReg;
  RX8900HourAlarmRegType      HourAlarmReg;
  RX8900WeekDayAlarmRegType   WeekDayAlarmReg;
  RX8900TimeCount0RegType     TimeCount0Reg;
  RX8900TimeCount1RegType     TimeCount1Reg;
  RX8900ExtRegType            ExtReg;
  RX8900FlagRegType           FlagReg;
  RX8900ControlRegType        ControlReg;
  RX8900TEMPRegType           TempReg;
  RX8900BackupRegType         BackupReg;
}RX8900AllRegType;

#endif


extern const RX8900CfgType gRX8900Cfg;

#endif