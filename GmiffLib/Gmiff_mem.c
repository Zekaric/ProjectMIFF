/**************************************************************************************************
file:       gmiff_mem
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
#include "gmiff_local.h"

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
func: _MiffMemCloc
**************************************************************************************************/
void *_MiffMemCloc(Gcount const memByteCount)
{
   returnNullIf(memByteCount < 0);

   return _memCloc(memByteCount);
}

/**************************************************************************************************
func: _MiffMemDloc
**************************************************************************************************/
void _MiffMemDloc(void * const mem)
{
   _memDloc(mem);
}

/**************************************************************************************************
func: _MiffMemStart
**************************************************************************************************/
void _MiffMemStart(GmemCloc const memClocFunc, GmemDloc const memDlocFunc)
{
   _memCloc = memClocFunc;
   _memDloc = memDlocFunc;
}

/**************************************************************************************************
func: _MiffMemStop
**************************************************************************************************/
void _MiffMemStop(void)
{
   _memCloc = NULL;
   _memDloc = NULL;
}
