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
#ifndef RX8900_TYPES_H
#define RX8900_TYPES_H

#include "Std_Types.h"

/* Temperature compensation selection */
#define RX8900_CSEL_0X5S            (0x0U)
#define RX8900_CSEL_2S              (0x1U)
#define RX8900_CSEL_10S             (0x2U)
#define RX8900_CSEL_30S             (0x3U)
/* Timer source clock selection */
#define RX8900_TSEL_4096HZ          (0x0U)
#define RX8900_TSEL_64HZ            (0x1U)
#define RX8900_TSEL_SEC             (0x2U)
#define RX8900_TSEL_MIN             (0x3U)
/* Output Frequency selection */
#define RX8900_FSEL_32768HZ         (0x0U)
#define RX8900_FSEL_1024HZ          (0x1U)
#define RX8900_FSEL_1HZ             (0x2U)
/* Update Interrupt Select */
#define RX8900_USEL_SEC             (0x0U)
#define RX8900_USEL_MIN             (0x1U)
/* Week Alarm/Day Alarm */
#define RX8900_WADA_WEEK            (0x0U)
#define RX8900_WADA_DAY             (0x1U)

typedef enum
{
  RX8900_SEC_REG                   = 0x0U,
  RX8900_MIN_REG,
  RX8900_HOUR_REG,
  RX8900_WEEK_REG,
  RX8900_DAY_REG,
  RX8900_MONTH_REG,
  RX8900_YEAR_REG,
  RX8900_RAM_REG,
  RX8900_MIN_ALARM_REG,
  RX8900_HOUR_ALARM_REG,
  RX8900_WEEKDAY_ALARM_REG,
  RX8900_TIMER_COUNTER0_REG,
  RX8900_TIMER_COUNTER1_REG,
  RX8900_EXTENSION_REG,
  RX8900_FLAG_REG,
  RX8900_CONTROL_REG,
  RX8900_SEC_MIRROR_REG,
  RX8900_MIN_MIRROR_REG,
  RX8900_HOUR_MIRROR_REG,
  RX8900_WEEK_MIRROR_REG,
  RX8900_DAY_MIRROR_REG,
  RX8900_MONTH_MIRROR_REG,
  RX8900_YEAR_MIRROR_REG,
  RX8900_TEMP_REG,
  RX8900_BACKUP_FUNCTION_REG,
  RX8900_NOT_USE0_REG,
  RX8900_NOT_USE1_REG,
  RX8900_TIMER_COUNTER0_MIRROR_REG,
  RX8900_TIMER_COUNTER1_MIRROR_REG,
  RX8900_EXTENSION_MIRROR_REG,
  RX8900_FLAG_MIRROR_REG,
  RX8900_CONTROL_MIRROR_REG,
  RX8900_MAX_REG
}RX8900_RegisterType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 SECBCD:7; 
    uint8 RW0_0:1;
  }B;
}RX8900SecRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 MINBCD:7; 
    uint8 RW0_0:1;
  }B;
}RX8900MinRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 HOURBCD:6; 
    uint8 RW0_0:2;
  }B;
}RX8900HourRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 SUN:1; 
    uint8 MON:1;
    uint8 TUE:1;
    uint8 WED:1;
    uint8 THU:1;
    uint8 FRI:1;
    uint8 SAT:1;
    uint8 RW0_0:1; 
  }B;
}RX8900WeekRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 DAYBCD:6;
    uint8 RW0_0:2;
  }B;
}RX8900DayRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 MONTHBCD:5;
    uint8 RW0_0:3;
  }B;
}RX8900MonthRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 YEARBCD:8;
  }B;
}RX8900YearRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 RAM_0:1;
		uint8 RAM_1:1;
		uint8 RAM_2:1;
		uint8 RAM_3:1;
		uint8 RAM_4:1;
		uint8 RAM_5:1;
		uint8 RAM_6:1;
		uint8 RAM_7:1;
  }B;
}RX8900RamRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 ALARMBCD:7;
    uint8 AE:1;
  }B;
}RX8900MinAlarmRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 ALARMBCD:6;
    uint8 RAM:1;
    uint8 AE:1;
  }B;
}RX8900HourAlarmRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 ALARMBCD:7;
    uint8 AE:1;
  }WEEKB;
  struct 
  {
    uint8 ALARMBCD:6;
    uint8 RAM_0:1;
    uint8 AE:1;
  }DAYB;
}RX8900WeekDayAlarmRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 TIMELOW:8;
  }B;
}RX8900TimeCount0RegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 TIMEHIGH:4;
    uint8 RAM_0:1;
		uint8 RAM_1:1;
		uint8 RAM_2:1;
		uint8 RAM_3:1;
  }B;
}RX8900TimeCount1RegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 TSEL:2;
    uint8 FSEL:2;
    uint8 TE:1;
    uint8 USEL:1;
    uint8 WADA:1;
    uint8 TEST:1;
  }B;
}RX8900ExtRegType;

typedef union
{
  uint8 U;
  struct 
  {
    uint8 VDET:1;
    uint8 VLF:1;
    uint8 RW0_0:1;
    uint8 AF:1;
    uint8 TF:1;
    uint8 UF:1;
    uint8 RW0_1:2;
  }B;
}RX8900FlagRegType;

typedef union
{
	uint8 U;
  struct 
  {
		uint8 RESET:1;
		uint8 RW0_0:2;
		uint8 AIE:1;
		uint8 TIE:1;
		uint8 UIE:1;
		uint8 CSEL:2; 
  }B;
}RX8900ControlRegType;

typedef union
{
	uint8 U;
  struct 
  {
		uint8 TEMP:8; 
  }B;
}RX8900TEMPRegType;

typedef union
{
	uint8 U;
  struct 
  {
		uint8 BKSMP0:1;
    uint8 BKSMP1:1;
    uint8 SWOFF:1;
    uint8 VDETOFF:1;
    uint8 RW0_0:4;		
  }B;
}RX8900BackupRegType;


typedef struct
{
  uint8 sec;
  uint8 min; 
  uint8 hour; 
  uint8 week;
  uint8 day; 
  uint8 mon;
  uint8 year;
}RX8900TimeType;

typedef struct 
{
  RX8900ControlRegType ControlRegCfg;
  RX8900ExtRegType ExtRegCfg;
  RX8900TimeCount0RegType TimeCount0RegCfg;
  RX8900TimeCount1RegType TimeCount1RegCfg;
}RX8900CfgType;


#endif