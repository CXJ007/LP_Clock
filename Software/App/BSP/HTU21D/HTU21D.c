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
/**********************************************************************************
***********************************Includes****************************************
**********************************************************************************/
#include "HTU21D.h"
/**********************************************************************************
****************************Global variable Definitions****************************
**********************************************************************************/
uint8 gHTU21D_TempMajor = 0x0U;
uint8 gHTU21D_TempMinor = 0x0U;
uint8 gHTU21D_HumMajor = 0x0U;
uint8 gHTU21D_HumMinor = 0x0U;
/**********************************************************************************
*****************************Local Function Definitions****************************
**********************************************************************************/
static Std_ReturnType HTU21D_Updata(void);
static uint8 HTU21D_Crc8(uint8* pData, uint8 Len);
/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
static uint8 HTU21D_Crc8(uint8* pData, uint8 Len)
{
	uint8 index;
	uint8 indexBit;
	uint8 Crc8 = HTU21D_CRC8_INIT;

	for (index = 0; index < Len; index++) 
	{
			Crc8 ^= pData[index];
			for (indexBit = 0; indexBit < 8U; indexBit++) {
					if (Crc8 & 0x80U) 
					{ 
							Crc8 = (Crc8 << 1) ^ HTU21D_CRC8_POLY;
					} else 
					{
							Crc8 <<= 1;
					}
			}
	}

	return Crc8;
}
/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
static Std_ReturnType HTU21D_Updata(void)
{
  Std_ReturnType RetVal;
	uint8 RxBuffer[HTU21D_BUFFER_LEN] = {0x0U};
  uint16 RxData = 0x0U;
	float Data = 0;

	RetVal = HTU21D_READ_DATA(RxBuffer, HTU21D_BUFFER_LEN);
	
	if((E_OK == RetVal) && (RxBuffer[2] == HTU21D_Crc8(RxBuffer, 2U)))
	{
		RxData = RxBuffer[0] << 8U;
		RxData |= RxBuffer[1];
		if((RxBuffer[1] & HTU21D_HUM_STATUS) == HTU21D_HUM_STATUS)
		{
			/* Clear Status bit */
			RxData &= (~HTU21D_HUM_STATUS);
			Data = HTU21D_DATA_TO_HUM(RxData);
			gHTU21D_HumMajor = (uint8)Data;
			gHTU21D_HumMinor = (uint8)((Data - gHTU21D_HumMajor) * 100U);
		}
		else
		{
			Data = (float32)HTU21D_DATA_TO_TEMP(RxData);
			gHTU21D_TempMajor = (uint8)Data;
			gHTU21D_TempMinor = (uint8)((Data - gHTU21D_TempMajor) * 100U);
		}
	}
	else
	{
		RetVal = E_NOT_OK;
	}
	
	
	
	
  return RetVal;
}
/**********************************************************************************
****************************Global Function Definitions****************************
**********************************************************************************/
/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
Std_ReturnType HTU21D_Init(void)
{
	Std_ReturnType RetVal;

	RetVal = HTU21D_WRITE_CMD(HTU21D_SOFT_REST_CMD); 
	
	return RetVal;
}
/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/
Std_ReturnType HTU21D_MainFunc(uint32 CycleMs)
{
	uint32 CurrTicks = 0x0U;
	Std_ReturnType RetVal = E_OK;
	static uint8 EnterFirst = STD_TRUE;
	static uint32 Ticks = 0x0U;
	static uint32 LastTicks = 0x0U;
	static uint8 UpdataStatu = HTU21D_UPDATA_NONE;
	
	CurrTicks = HTU21D_Get_TicksMs();
	if(CurrTicks > LastTicks)
	{
		Ticks += (CurrTicks - LastTicks);
		switch(UpdataStatu)
		{
			case HTU21D_UPDATA_TEMP:
			{
				if(EnterFirst == STD_TRUE)
				{
					RetVal = HTU21D_WRITE_CMD(HTU21D_TEMP_NOHOLD_CMD);
					if(E_OK == RetVal)
					{
						Ticks = 0x0U;
						EnterFirst = STD_FALSE;
					}
					else
					{
						/* nothing */
					}
				}
				else if(HTU21D_TEMP_DELAY_MS <= Ticks)
				{
					/* Delay 50ms */
					RetVal = HTU21D_Updata();
					UpdataStatu = HTU21D_UPDATA_HUM;
					EnterFirst = STD_TRUE;
				}
				else
				{
					/* nothing */
				}
				break;
			}
			case HTU21D_UPDATA_HUM:
			{
				if(EnterFirst == STD_TRUE)
				{
					RetVal = HTU21D_WRITE_CMD(HTU21D_HUM_NOHOLD_CMD);
					if(E_OK == RetVal)
					{
						Ticks = 0x0U;
						EnterFirst = STD_FALSE;
					}
					else
					{
						/* nothing */
					}
				}
				else if(HTU21D_HUM_DELAY_MS <= Ticks)
				{
					/* Delay 16ms */
					RetVal = HTU21D_Updata();
					UpdataStatu = HTU21D_UPDATA_NONE;
					EnterFirst = STD_TRUE;
				}
				else
				{
					/* nothing */
				}
				break;
			}
			case HTU21D_UPDATA_NONE:
			{
				if(EnterFirst == STD_TRUE)
				{
					Ticks = 0x0U;
					EnterFirst = STD_FALSE;
				}
				else if(Ticks >= CycleMs)
				{
					UpdataStatu = HTU21D_UPDATA_TEMP;
					EnterFirst = STD_TRUE;
				}
				else
				{
					/* nothing */
				}
				break;
			}
			default:
			{
				RetVal = E_NOT_OK;
			}
		}
	}
	else
	{
		/* nothing */
	}
	LastTicks = CurrTicks;
	
	return RetVal;
}
