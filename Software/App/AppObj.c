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
************************************Includes************************************
*******************************************************************************/
#include "AppObj.h"
#include "Buzzer.h"
/*******************************************************************************
****************************Global variable Definitions*************************
*******************************************************************************/
TX_EVENT_FLAGS_GROUP gApp_ModeCtrlEventGroup;
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
Std_ReturnType App_ObjInit(void)
{
    Std_ReturnType RetVal = E_OK;

    RetVal |= ( Std_ReturnType )tx_event_flags_create(&gApp_ModeCtrlEventGroup,
                                                      "ModeCtrlEvent");

    return RetVal;
}
