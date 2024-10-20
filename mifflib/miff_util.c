/******************************************************************************
file:       util
author:     Robbert de Groot
company:
copyright:  2021, Robbert de Groot

description:
General utils
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) 2021, Robbert de Groot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "miff_local.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static _locale_t  _locale;

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffUtilStart
******************************************************************************/
void _MiffUtilStart(void)
{
   _locale = _create_locale(LC_ALL, "C");
}

/******************************************************************************
func: _MiffUtilStop
******************************************************************************/
void _MiffUtilStop(void)
{
   // Nothing to do.
}

/******************************************************************************
func: _MiffStrToN
******************************************************************************/
MiffN _MiffStrToN(MiffN const count, MiffN1 const * const str)
{
   MiffN index;
   MiffN value;

   value = 0;

   // Skip spaces.
   forCount(index, count)
   {
      breakIf(str[index] != ' ');
   }

   for (; index < count; index++)
   {
      value = value * 10 + str[index] - '0';
   }

   return value;
}

/******************************************************************************
func: _MiffLocaleGet
******************************************************************************/
_locale_t _MiffLocaleGet(void)
{
   return _locale;
}

/******************************************************************************
func: _MiffMemIsEqual

Compare two binary buffers for equality.
******************************************************************************/
MiffB _MiffMemIsEqual(MiffN const countA, MiffN1 const * const memA, MiffN const countB,
   MiffN1 const * const memB)
{
   returnFalseIf(countA != countB);
   returnFalseIf(memcmp(memA, memB, countA) != 0);
   returnTrue;
}
