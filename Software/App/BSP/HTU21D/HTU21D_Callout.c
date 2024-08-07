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
/*******************************************************************************
***********************************Includes*************************************
*******************************************************************************/
#include "HTU21D_Cbk.h"
#include "i2c.h"
#include "main.h"
#include "tx_api.h"
/*******************************************************************************
****************************Global Function Definitions*************************
*******************************************************************************/
/*******************************************************************************
 * Function name :
 * Inputs        :
 * Return        :
 * description   :
 * Limitation    :
 ******************************************************************************/
Std_ReturnType HTU21D_IIC_Write(const uint8 Address, const uint8 Register)
{
    Std_ReturnType RetVal;

    RetVal = (Std_ReturnType)HAL_I2C_Master_Transmit(
        &hi2c1, (uint16_t)Address, (uint8_t*)&Register, 0x1U, 0xFFFFU);

    return RetVal;
}
/*******************************************************************************
 * Function name :
 * Inputs        :
 * Return        :
 * description   :
 * Limitation    :
 ******************************************************************************/
Std_ReturnType HTU21D_IIC_Read
(
    const uint8 Address, 
    uint8* const pData,
    const uint8 Len
)
{
    Std_ReturnType RetVal;

    RetVal = (Std_ReturnType)HAL_I2C_Master_Receive(
        &hi2c1, (uint16_t)Address, (uint8_t*)pData, Len, 0xFFFFU);

    return RetVal;
}
/*******************************************************************************
 * Function name :
 * Inputs        :
 * Return        :
 * description   :
 * Limitation    :
 ******************************************************************************/
uint32 HTU21D_Get_TicksMs(void)
{
    return TICKS_TO_MS((uint32)tx_time_get());
}
