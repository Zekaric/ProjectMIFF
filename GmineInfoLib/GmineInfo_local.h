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
#define STRINGIFY(x) #x

#define HEADER_TYPE_STR       "Mining Information"
#define HEADER_TYPE_LEN       18
#define HEADER_VERSION_NUM    1
#define HEADER_VERSION_STR    STRINGIFY(HEADER_VERSION_NUMBER)
#define HEADER_VERSION_LEN    1
#define KEY_FORMAT_STR        "format"
#define KEY_FORMAT_LEN        6
#define KEY_NAME_STR          "name"
#define KEY_NAME_LEN          4
#define KEY_VERSION_STR       "version"
#define KEY_VERSION_LEN       7

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
Gb                 _MiIoClocReader(       GmineInfo       * const mineInfo);
Gb                 _MiIoClocWriter(       GmineInfo       * const mineInfo);

void               _MiIoDloc(             GmineInfo       * const mineInfo);

void              *_MiMemCloc(            Gcount const memByteCount);
void               _MiMemDloc(            void * const mem);
void               _MiMemStart(           GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void               _MiMemStop(            void);

#define            _MiMemClearType(       MEM, TYPE)                      memset((MEM), 0,               sizeof(TYPE))
#define            _MiMemClearTypeArray(  MEM, TYPE, COUNT)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define            _MiMemClocType(               TYPE)           (TYPE *) _MiMemCloc(                    sizeof(TYPE))
#define            _MiMemClocTypeArray(   COUNT, TYPE)           (TYPE *) _MiMemCloc(          (COUNT) * sizeof(TYPE))
#define            _MiMemCopyTypeArray(   MEM, COUNT, TYPE, SRC)          memcpy((MEM), (SRC), (COUNT) * sizeof(TYPE))
#define            _MiMemIsEqual(         COUNT, MEM, COUNT_ALT, MEM_ALT) (memcmp(MEM, MEM_ALT, COUNT) == 0)
#define            _MiStrGetCount(        STR, MAX_LEN)                   strnlen(STR, MAX_LEN)
#define            _MiStrIsEqual(         STR, STR_ALT)                   _MiMemIsEqual(_MiStrGetCount(STR, GkeyBYTE_COUNT), STR, _MiStrGetCount(STR_ALT, GkeyBYTE_COUNT), STR_ALT)
#endif
