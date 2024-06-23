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


#if (RX8900_DEBUG == STD_ON)

RX8900AllRegType gRX8900AllReg = { 0U };
uint8 gRX8900DebugCtrCmd = RX8900_DEBUG_UPDATA_CMD;
RX8900_RegisterType gRX8900DebugRegAddr = RX8900_RAM_REG;
uint8 gRX8900DebugData = 0U;

#endif

#if (RX8900_DEBUG == STD_ON)

void RX8900_Debug(void)
{
  if(RX8900_DEBUG_UPDATA_CMD == gRX8900DebugCtrCmd)
  {
		/* Updata all registers */
    (void)RX8900_READ_REGISTER((uint8)RX8900_SEC_REG, (uint8*)&gRX8900AllReg,                            \
																		((uint8)RX8900_CONTROL_REG - (uint8)RX8900_SEC_REG + 1U));
    (void)RX8900_READ_REGISTER((uint8)RX8900_TEMP_REG, (((uint8*)&gRX8900AllReg) + RX8900_REG_OFFSET),   \
																		((uint8)RX8900_BACKUP_FUNCTION_REG - (uint8)RX8900_TEMP_REG + 1U));
		gRX8900DebugCtrCmd = RX8900_DEBUG_NONE_CMD;
	}
	else if(RX8900_DEBUG_WRITE_CMD == gRX8900DebugCtrCmd)
	{
		/* Write registers */
		RX8900_WRITE_REGISTER((uint8)gRX8900DebugRegAddr, gRX8900DebugData, 1U);
		/* Updata all registers the next time */
		gRX8900DebugCtrCmd = RX8900_DEBUG_UPDATA_CMD;
	}
	else
	{
		/* nothing */
	}
}

#endif

RX8900ControlRegType ControlReg0;
RX8900ControlRegType ControlReg1;

Std_ReturnType RX8900_Init(void)
{
  Std_ReturnType RetVal;





  uint8 data = 0xff;
	uint8 data1 = 0xFF;
	
	ControlReg0.U = 0x0;
	ControlReg1.U = 0;
	
	ControlReg0.B.CSEL1 = 1;
	ControlReg0.B.CSEL0 = 1;

//  

//	RetVal = RX8900_READ_REGISTER(0x0D, &data, 1);
  RX8900_WRITE_REGISTER(RX8900_CONTROL_REG, ControlReg0.U, 1);
//	ControlReg1.U = 0;
	RetVal = RX8900_READ_REGISTER(0x0F, (uint8*)&ControlReg1.U, 1);
  RetVal = RX8900_READ_REGISTER(0x0F, &data, 1);

	

  return RetVal;
}