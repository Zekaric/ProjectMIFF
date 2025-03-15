/**************************************************************************************************
file:       GmineInfo_local
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Function local to the library
**************************************************************************************************/

/**************************************************************************************************
MIT License

Copyright (c) 2025, Robbert de Groot

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

#if !defined(GMINEINFO_LOCAL)
#define      GMINEINFO_LOCAL

/**************************************************************************************************
include:
**************************************************************************************************/
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "GmineInfoLib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/

/**************************************************************************************************
type:
**************************************************************************************************/

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
void              *_MiMemCloc(            Gcount const memByteCount);
void               _MiMemDloc(            void * const mem);
void               _MiMemStart(           GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void               _MiMemStop(            void);

#define            _MiMemClearType(       MEM, TYPE)                      memset((MEM), 0,               sizeof(TYPE))
#define            _MiMemClearTypeArray(  MEM, TYPE, COUNT)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define            _MiMemCopyTypeArray(   MEM, COUNT, TYPE, SRC)          memcpy((MEM), (SRC), (COUNT) * sizeof(TYPE))
#define            _MiMemClocType(               TYPE)           (TYPE *) _MiMemCloc(                    sizeof(TYPE))
#define            _MiMemClocTypeArray(   COUNT, TYPE)           (TYPE *) _MiMemCloc(          (COUNT) * sizeof(TYPE))

#endif
