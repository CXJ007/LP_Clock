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
************************************include****************************************
**********************************************************************************/
#include "RX8900_Cbk.h"
#include "main.h"
#include "i2c.h" 
/**********************************************************************************
*********************************global function***********************************
**********************************************************************************/

/**********************************************************************************
* name        :
* input       :
* input\output:
* return      :
* description ：
* limit       :
**********************************************************************************/

Std_ReturnType RX8900_IIC_Transmit
(
  const uint8 Address, 
  const uint8 Register, 
  const uint8 WriteData, 
  uint8 * const pReadData, 
  const uint8 Len, 
  const uint8 Cmd
)
{
  Std_ReturnType RetVal;

  if(Cmd == 0U)
  {
    RetVal = (Std_ReturnType)HAL_I2C_Mem_Write(&hi2c1, (uint16_t)Address, (uint16_t)Register,           \
                                                                  1U, (uint8_t*)&WriteData, Len, 0XFFU);
  }
  else if(Cmd == 1U)
  {
    RetVal = (Std_ReturnType)HAL_I2C_Mem_Read(&hi2c1, (uint16_t)Address, (uint16_t)Register,             \
                                  1U, (uint8_t*)pReadData, Len, 0XFFU);
  }
  else
  {
    /* Nothing */
  }

  return RetVal;
}
 
