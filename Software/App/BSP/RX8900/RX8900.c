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
#include "RX8900.h"

uint8 gRX8900RuntimeNotify = 0U;
RX8900TimeType gRX8900TimeInfo = {0U};
#if (RX8900_DEBUG == STD_ON)
RX8900AllRegType gRX8900AllReg = {0U};
uint8 gRX8900DebugCtrCmd = RX8900_DEBUG_UPDATA_CMD;
RX8900_RegisterType gRX8900DebugRegAddr = RX8900_RAM_REG;
uint8 gRX8900DebugData = 0U;
#endif

#if(RX8900_DEBUG == STD_ON)
static void RX8900_Debug(void);
#endif
static Std_ReturnType RX8900_UpErrFlag_Polling(void);

#if(RX8900_DEBUG == STD_ON)

static void RX8900_Debug(void)
{
  if(RX8900_DEBUG_UPDATA_CMD == gRX8900DebugCtrCmd)
  {
		/* Updata all registers */
    (void)RX8900_READ_REGISTER_LIST((uint8)RX8900_SEC_REG, (uint8*)&gRX8900AllReg,                            \
																		((uint8)RX8900_CONTROL_REG - (uint8)RX8900_SEC_REG + 1U));
    (void)RX8900_READ_REGISTER_LIST((uint8)RX8900_TEMP_REG, (((uint8*)&gRX8900AllReg) + RX8900_REG_OFFSET),   \
																		((uint8)RX8900_BACKUP_FUNCTION_REG - (uint8)RX8900_TEMP_REG + 1U));
		gRX8900DebugCtrCmd = RX8900_DEBUG_NONE_CMD;
	}
	else if(RX8900_DEBUG_WRITE_CMD == gRX8900DebugCtrCmd)
	{
		/* Write registers */
		RX8900_WRITE_REGISTER((uint8)gRX8900DebugRegAddr, gRX8900DebugData);
		/* Updata all registers the next time */
		gRX8900DebugCtrCmd = RX8900_DEBUG_UPDATA_CMD;
	}
	else
	{
		/* nothing */
	}
}
#endif

static Std_ReturnType RX8900_UpErrFlag_Polling(void)
{
	Std_ReturnType RetVal;
	RX8900FlagRegType FlagReg;

	RetVal = RX8900_READ_REGISTER((uint8)RX8900_FLAG_REG, &FlagReg.U);
	if((E_OK == RetVal) && (FlagReg.U != 0x0U))
	{
		/* Voltage Detection Flag */
		if(FlagReg.B.VDET == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_VDET_ERR;
			FlagReg.B.VDET = 0x0U;
		}
		else
		{
			/* nothing */
		}
		/* Voltage Low Flag */
		if(FlagReg.B.VLF == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_VLF_ERR;
			FlagReg.B.VLF = 0x0U;
		}
		else
		{
			/* nothing */
		}
		RetVal = RX8900_WRITE_REGISTER((uint8)RX8900_FLAG_REG, FlagReg.U);
	}
	else
	{
		/* nothing */
	}

	return RetVal;
}

Std_ReturnType RX8900_Set_Time(RX8900TimeType Time)
{
	Std_ReturnType RetVal;

	if((Time.sec <= RX8900_SEC_MAX) && (Time.min <= RX8900_MIN_MAX)            \
																&& (Time.hour <= RX8900_HOUR_MAX)            \
																&& (Time.week <= RX8900_WEEK_MAX)            \
																&& (Time.day <= RX8900_DAY_MAX)	             \
																&& (Time.mon <= RX8900_MON_MAX)							 \
																&& (Time.year <= RX8900_YEAR_MAX))
	{
		RetVal = RX8900_WRITE_REGISTER((uint8)RX8900_SEC_REG, RX8900_DEC_TO_BCD(Time.sec));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_MIN_REG, RX8900_DEC_TO_BCD(Time.min));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_HOUR_REG, RX8900_DEC_TO_BCD(Time.hour));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_WEEK_REG, Time.week);
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_DAY_REG, RX8900_DEC_TO_BCD(Time.day));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_MONTH_REG, RX8900_DEC_TO_BCD(Time.mon));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_YEAR_REG, RX8900_DEC_TO_BCD(Time.year));
	}	
	else
	{
		RetVal = E_NOT_OK;
	}

	return RetVal;
}

Std_ReturnType RX8900_Updata_Time(RX8900TimeType *pTime)
{
	Std_ReturnType RetVal;
	uint8 TimeData[(uint8)RX8900_YEAR_REG - (uint8)RX8900_SEC_REG + 1U];
	
	if(NULL_PTR != pTime)
	{
		RetVal = RX8900_READ_REGISTER_LIST((uint8)RX8900_SEC_REG, TimeData,                             \
																		((uint8)RX8900_YEAR_REG - (uint8)RX8900_SEC_REG + 1U));
		if(E_OK == RetVal)
		{
			TimeData[(uint8)RX8900_SEC_REG] = RX8900_BCD_TO_DEC(TimeData[(uint8)RX8900_SEC_REG]);
			TimeData[(uint8)RX8900_MIN_REG] = RX8900_BCD_TO_DEC(TimeData[(uint8)RX8900_MIN_REG]);
			TimeData[(uint8)RX8900_HOUR_REG] = RX8900_BCD_TO_DEC(TimeData[(uint8)RX8900_HOUR_REG]);
			TimeData[(uint8)RX8900_DAY_REG] = RX8900_BCD_TO_DEC(TimeData[(uint8)RX8900_DAY_REG]);
			TimeData[(uint8)RX8900_MONTH_REG] = RX8900_BCD_TO_DEC(TimeData[(uint8)RX8900_MONTH_REG]);
			TimeData[(uint8)RX8900_YEAR_REG] = RX8900_BCD_TO_DEC(TimeData[(uint8)RX8900_YEAR_REG]);
			if((TimeData[(uint8)RX8900_SEC_REG] <= RX8900_SEC_MAX) 
													&& (TimeData[(uint8)RX8900_MIN_REG] <= RX8900_MIN_MAX)    \
													&& (TimeData[(uint8)RX8900_HOUR_REG] <= RX8900_HOUR_MAX)  \
													&& (TimeData[(uint8)RX8900_WEEK_REG] <= RX8900_WEEK_MAX)  \
													&& (TimeData[(uint8)RX8900_DAY_REG] <= RX8900_DAY_MAX)	  \
													&& (TimeData[(uint8)RX8900_MONTH_REG] <= RX8900_MON_MAX)	\
													&& (TimeData[(uint8)RX8900_YEAR_REG] <= RX8900_YEAR_MAX))
			{
				pTime->sec = TimeData[(uint8)RX8900_SEC_REG];
				pTime->min = TimeData[(uint8)RX8900_MIN_REG];
				pTime->hour = TimeData[(uint8)RX8900_HOUR_REG];
				pTime->week = TimeData[(uint8)RX8900_WEEK_REG];
				pTime->day = TimeData[(uint8)RX8900_DAY_REG];
				pTime->mon = TimeData[(uint8)RX8900_MONTH_REG];
				pTime->year = TimeData[(uint8)RX8900_YEAR_REG];
			}
			else
			{
				RetVal = E_NOT_OK;
			}
		}
		else
		{
			/* nothing */
		}
	}	
	else
	{
		RetVal = E_NOT_OK;
	}

	return RetVal;
}

Std_ReturnType RX8900_Set_Alarm
(
	uint8 MinAlarm,
	uint8 HourAlarm,
	uint8 WeekAlarm
)
{
	Std_ReturnType RetVal;
	RX8900ExtRegType ExtReg;
	RX8900ControlRegType ControlReg;

	if((MinAlarm <= RX8900_MIN_MAX) && (HourAlarm <= RX8900_HOUR_MAX))
	{
		RetVal = RX8900_WRITE_REGISTER((uint8)RX8900_MIN_ALARM_REG,                                      \
																				((~RX8900_ALARM_DISABLE) & RX8900_DEC_TO_BCD(MinAlarm)));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_HOUR_ALARM_REG,                                    \
																				((~RX8900_ALARM_DISABLE) & RX8900_DEC_TO_BCD(HourAlarm)));
		RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_WEEKDAY_ALARM_REG,                                 \
																				((~RX8900_ALARM_DISABLE) & WeekAlarm));
		RetVal |= RX8900_READ_REGISTER((uint8)RX8900_CONTROL_REG, &ControlReg.U);
		RetVal |= RX8900_READ_REGISTER((uint8)RX8900_EXTENSION_REG, &ExtReg.U);
		if(E_OK == RetVal)
		{
			/* Enable Alarm */
			ControlReg.B.AIE = STD_ENABLE;
			/* Set alarm mode */
			ExtReg.B.WADA = RX8900_WADA_WEEK;
			RetVal = RX8900_WRITE_REGISTER((uint8)RX8900_CONTROL_REG, gRX8900Cfg.ControlRegCfg.U);
			RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_EXTENSION_REG, gRX8900Cfg.ExtRegCfg.U);
		}
	}	
	else
	{
		RetVal = E_NOT_OK;
	}

	return RetVal;
}

Std_ReturnType RX8900_Get_Alarm
(
	uint8 *pMinAlarm,
	uint8 *pHourAlarm,
	uint8 *pWeekAlarm
)
{
	Std_ReturnType RetVal;
	RX8900MinAlarmRegType MinAlarmReg;
	RX8900HourAlarmRegType HourAlarmReg;
	RX8900WeekDayAlarmRegType WeekDayAlarmReg;

	RetVal = RX8900_READ_REGISTER((uint8)RX8900_MIN_ALARM_REG, &MinAlarmReg.U);
	RetVal |= RX8900_READ_REGISTER((uint8)RX8900_HOUR_ALARM_REG, &HourAlarmReg.U);
	RetVal |= RX8900_READ_REGISTER((uint8)RX8900_WEEKDAY_ALARM_REG, &WeekDayAlarmReg.U);
	if(E_OK == RetVal)
	{
		*pMinAlarm = RX8900_BCD_TO_DEC(MinAlarmReg.B.ALARMBCD);
		*pHourAlarm = RX8900_BCD_TO_DEC(HourAlarmReg.B.ALARMBCD);
		*pWeekAlarm = WeekDayAlarmReg.WEEKB.ALARMBCD;
	}
	else
	{
		/* nothing */
	}

	return RetVal;
}

Std_ReturnType RX8900_Init(void)
{
  Std_ReturnType RetVal = E_OK;

	/* Clear all flag */
	RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_FLAG_REG, 0U);
	RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_CONTROL_REG,                  \
																				gRX8900Cfg.ControlRegCfg.U);
	RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_EXTENSION_REG,                \
																				gRX8900Cfg.ExtRegCfg.U);
	RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_TIMER_COUNTER0_REG,           \
																				gRX8900Cfg.TimeCount0RegCfg.U);	
	RetVal |= RX8900_WRITE_REGISTER((uint8)RX8900_TIMER_COUNTER1_REG,           \
																				gRX8900Cfg.TimeCount1RegCfg.U);	
  return RetVal;
}

Std_ReturnType RX8900_Main_Fun(void)
{
	Std_ReturnType RetVal = E_OK;

	RetVal |= RX8900_UpErrFlag_Polling();
	if(RX8900_UPTATA_EVENT == (gRX8900RuntimeNotify & RX8900_UPTATA_EVENT))
	{
		RetVal |= RX8900_Updata_Time(&gRX8900TimeInfo);
		/* Clear Updata time event*/
		gRX8900RuntimeNotify &= (~RX8900_UPTATA_EVENT);
	}
	else
	{
		/* nothing */
	}
	
#if(RX8900_DEBUG == STD_ON)
	RX8900_Debug();
#endif

	return RetVal;
}

void RX8900_Process_ISR(void)
{
	uint8 index;
	Std_ReturnType RetVal;
	RX8900FlagRegType FlagReg;

	RetVal = RX8900_READ_REGISTER((uint8)RX8900_FLAG_REG, &FlagReg.U);
	if((E_OK == RetVal) && (FlagReg.U != 0U))
	{
		/* Voltage Detection Flag */
		/* Not an interrupt source */
		if(FlagReg.B.VDET == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_VDET_ERR;
		}
		else
		{
			/* nothing */
		}
		/* Voltage Low Flag */
		/* Not an interrupt source */
		if(FlagReg.B.VLF == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_VLF_ERR;
		}
		else
		{
			/* nothing */
		}
		/* Alarm Flag */
		if(FlagReg.B.AF == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_ALARM_EVENT;
		}
		else
		{
			/* nothing */
		}
		/* Wakeup Timer Flag */
		if(FlagReg.B.TF == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_WAKEUP_EVENT;
		}
		else
		{
			/* nothing */
		}
		/* Timer Updata Flag */
		if(FlagReg.B.UF == 0x1U)
		{
			gRX8900RuntimeNotify |= RX8900_UPTATA_EVENT;
		}
		else
		{
			/* nothing */
		}
		/* clear interrupt flag */
		(void)RX8900_WRITE_REGISTER((uint8)RX8900_FLAG_REG, 0U);
	}
	else
	{
		/* nothing */
	}
}