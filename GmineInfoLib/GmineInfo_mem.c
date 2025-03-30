/**************************************************************************************************
file:       GmineInfo_mem
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
memory functions
**************************************************************************************************/

/**************************************************************************************************
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
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "GmineInfo_local.h"

/**************************************************************************************************
local:
variable:
**************************************************************************************************/
static GmemCloc   _memCloc = NULL;
static GmemDloc   _memDloc = NULL;

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _MiMemCloc
**************************************************************************************************/
void *_MiMemCloc(Gcount const memByteCount)
{
   returnNullIf(memByteCount < 0);

   return _memCloc(memByteCount);
}

/**************************************************************************************************
func: _MiMemDloc
**************************************************************************************************/
void _MiMemDloc(void * const mem)
{
   _memDloc(mem);
}

/**************************************************************************************************
func: _MiMemStart
**************************************************************************************************/
void _MiMemStart(GmemCloc const memClocFunc, GmemDloc const memDlocFunc)
{
   _memCloc = memClocFunc;
   _memDloc = memDlocFunc;
}

/**************************************************************************************************
func: _MiMemStop
**************************************************************************************************/
void _MiMemStop(void)
{
   _memCloc = NULL;
   _memDloc = NULL;
}

/**************************************************************************************************
func: _MiStrIsEmpty
**************************************************************************************************/
Gb _MiStrIsEmpty(Gstr const * const value)
{
   Gcount count;
   Gindex index;

   // Null means true.
   returnTrueIf(!value);

   // The string value is 0 length, means true.
   count = _MiStrGetCount(value, Gn4MAX);
   returnTrueIf(count == 0);

   // Hunt for a non space.
   forCount(index, count)
   {
      continueIf(isspace(value[index]));
   }

   // If index is not count then there is a non space character in the string.
   return (index != count);
}

/**************************************************************************************************
func: _MiStrClone
**************************************************************************************************/
Gstr *_MiStrClone(Gstr const * const value)
{
   Gcount count;
   Gindex index,
          indexStr;
   Gstr  *str;

   returnNullIf(!value);

   count = _MiStrGetCount(value, Gn4MAX);

   // Skip over leading space.
   forCount(index, count)
   {
      continueIf(isspace(value[index]));
   }
   returnNullIf(index == count);

   str = _MiMemClocTypeArray(count + 1, Gstr);
   returnNullIf(!str);

   // Copy over the string.
   indexStr = 0;
   for (; index < count; index++)
   {
      str[indexStr++] = value[index];
   }
   str[indexStr++] = 0;

   // Trim trailing space.
   forCountDown(index, indexStr)
   {
      if (isspace(str[index]))
      {
         str[index] = 0;
      }
   }

   return str;
}
