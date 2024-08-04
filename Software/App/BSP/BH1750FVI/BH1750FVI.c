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
#include "BH1750FVI.h"
/*******************************************************************************
****************************Global variable Definitions*************************
*******************************************************************************/
uint16 gBH1750FVI_LightMajor = 0x0U;
uint16 gBH1750FVI_LightMinor = 0x0U;
/*******************************************************************************
*****************************Local Function Definitions*************************
*******************************************************************************/

/*******************************************************************************
 * Function name :
 * Inputs        :
 * Return        :
 * description   :
 * Limitation    :
 ******************************************************************************/

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
Std_ReturnType BH1750FVI_Init(void)
{
    Std_ReturnType RetVal = E_OK;

    /* hard reset */
    BH1750FVI_WriteDVI(STD_LOW);
    BH1750FVI_DelayMs(1);
    BH1750FVI_WriteDVI(STD_HIGH);
    RetVal |= BH1750FVI_WRITE_CMD(BH1750FVI_POWER_ON_CMD);
    /* Clear data register value */
    RetVal |= BH1750FVI_WRITE_CMD(BH1750FVI_SOFT_RESET_CMD);
    RetVal |= BH1750FVI_WRITE_CMD(BH1750FVI_POWER_DOWN_CMD);

    return RetVal;
}
/*******************************************************************************
 * Function name :
 * Inputs        :
 * Return        :
 * description   :
 * Limitation    :
 ******************************************************************************/
Std_ReturnType BH1750FVI_MainFunc(uint32 CycleMs)
{
    Std_ReturnType RetVal;
    uint32         CurrTicks                      = 0x0U;
    uint8          RxBuffer[BH1750FVI_BUFFER_LEN] = { 0x0U };
    float32        Data                           = 0;
    static uint32  Ticks                          = 0x0U;
    static uint32  LastTicks                      = 0x0U;
    static uint8   UpdataStatu                    = BH1750FVI_UPDATA_STOP;
    static uint8   EnterFirst                     = STD_TRUE;

    CurrTicks = BH1750FVI_Get_TicksMs();
    if(CurrTicks > LastTicks)
    {
        Ticks += (CurrTicks - LastTicks);
        if(UpdataStatu == BH1750FVI_UPDATA_STOP)
        {
            if(EnterFirst == STD_TRUE)
            {
                Ticks      = 0x0U;
                EnterFirst = STD_FALSE;
            }
            else
            {
                if(CycleMs <= Ticks)
                {
                    UpdataStatu = BH1750FVI_UPDATA_RUN;
                    EnterFirst  = STD_TRUE;
                }
                else
                {
                    /* nothing */
                }
            }
        }
        else if(UpdataStatu == BH1750FVI_UPDATA_RUN)
        {
            if(EnterFirst == STD_TRUE)
            {
                RetVal = BH1750FVI_WRITE_CMD(BH1750FVI_ONCE_1P0_CMD);
                if(E_OK == RetVal)
                {
                    Ticks      = 0x0U;
                    EnterFirst = STD_FALSE;
                }
                else
                {
                    /* nothing */
                }
            }
            else
            {
                if(BH1750FVI_DELAY_MS <= Ticks)
                {
                    RetVal =
                        BH1750FVI_READ_DATA(RxBuffer, BH1750FVI_BUFFER_LEN);
                    Data = ((float32)((RxBuffer[0] << 8U) | RxBuffer[1]))
                           / BH1750FVI_ACCURACY;
                    gBH1750FVI_LightMajor = (uint8)Data;
                    gBH1750FVI_LightMinor =
                        (uint8)((Data - gBH1750FVI_LightMajor) * 100U);
                    UpdataStatu = BH1750FVI_UPDATA_STOP;
                    EnterFirst  = STD_TRUE;
                }
                else
                {
                    /* nothing */
                }
            }
        }
    }
    else
    {
        /* nothing */
    }

    return RetVal;
}
