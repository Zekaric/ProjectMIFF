/******************************************************************************
file:       string
author:     Robbert de Groot
company:    
copyright:  2021, Robbert de Groot

description:
String functions
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
#include "json_local.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static JsonN1 _hex[128];

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _JsonStrStart
******************************************************************************/
JsonB _JsonStrStart(void)
{
   _hex['0']    = 0x0;
   _hex['1']    = 0x1;
   _hex['2']    = 0x2;
   _hex['3']    = 0x3;
   _hex['4']    = 0x4;
   _hex['5']    = 0x5;
   _hex['6']    = 0x6;
   _hex['7']    = 0x7;
   _hex['8']    = 0x8;
   _hex['9']    = 0x9;
   _hex['a']    = 
      _hex['A'] = 0xa;
   _hex['b']    =
      _hex['B'] = 0xb;
   _hex['c']    =
      _hex['C'] = 0xc;
   _hex['d']    =
      _hex['D'] = 0xd;
   _hex['e']    =
      _hex['E'] = 0xe;
   _hex['f']    =
      _hex['F'] = 0xf;

   returnTrue;
}

/******************************************************************************
func: _JsonStrStop
******************************************************************************/
void _JsonStrStop(void)
{
   return;
}

/******************************************************************************
func: _JsonStrToHex
******************************************************************************/
JsonN1 _JsonStrToHex(JsonN1 const value)
{
   return _hex[value];
}

/******************************************************************************
func: _JsonStrToN
******************************************************************************/
JsonN _JsonStrToN(JsonStr const * const str)
{
   JsonI4  index;
   JsonN   value;

   value = 0;
   for (index = 0;; index++)
   {
      breakIf(str[index] == 0);

      value = value * 10 + str[index] - '0';
   }

   return value;
}
