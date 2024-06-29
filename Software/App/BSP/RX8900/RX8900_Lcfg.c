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
#include "RX8900_Cfg.h"

const RX8900CfgType gRX8900Cfg =
{
  /* Control register configer */
  {
    .B.RESET = 0x1U,//Software reset
    .B.RW0_0 = 0x0U,
    .B.AIE = STD_OFF,
    .B.TIE = STD_OFF,
    .B.UIE = STD_ON,
    .B.CSEL = RX8900_CSEL_0X5S
  },
  /* Extension register configer */
  {
    .B.TSEL = RX8900_TSEL_4096HZ,
    .B.FSEL = RX8900_FSEL_32768HZ,
    .B.TE = STD_OFF,
    .B.USEL = RX8900_USEL_SEC,
    .B.WADA = RX8900_WADA_DAY,
    .B.TEST = 0x0U
  },
  /* Timer Counter 0 */
  {
    .U = 0x0U
  },
  /* Timer Counter 1 */
  {
    .U = 0x0U
  }
};