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
#include <ctype.h>

#include "GmineInfoLib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/
#define STRINGIFY(x) #x

#define HEADER_TYPE_STR                   "Mining Information"
#define HEADER_TYPE_LEN                   18
#define HEADER_VERSION_NUM                1
#define HEADER_VERSION_STR                STRINGIFY(HEADER_VERSION_NUMBER)
#define HEADER_VERSION_LEN                1

#define KEY_FORMAT_STR                    "format"
#define KEY_FORMAT_LEN                    6
#define KEY_NAME_STR                      "name"
#define KEY_NAME_LEN                      4
#define KEY_VERSION_STR                   "version"
#define KEY_VERSION_LEN                   7

#define KEY_BLOCK_DATA                    "data"
#define KEY_BLOCK_DATA_AUTHOR_NAME        "author name"
#define KEY_BLOCK_DATA_COMMENT            "comment"
#define KEY_BLOCK_DATA_COMPANY_NAME       "company name"
#define KEY_BLOCK_DATA_COPYRIGHT          "copyright"
#define KEY_BLOCK_DATA_PROJECT_MAX        "project max"
#define KEY_BLOCK_DATA_PROJECT_MIN        "project min"
#define KEY_BLOCK_DATA_PROJECT_NAME       "project name"
#define KEY_BLOCK_DATA_PROJECT_SYSTEM     "project system"
#define KEY_BLOCK_DATA_SOFTWARE_NAME      "software name"
#define KEY_BLOCK_DATA_SOFTWARE_VERSION   "software version"

#define KEY_BLOCK_ITEM                    "item"
#define KEY_BLOCK_GEOMETRY                "geometry"
#define KEY_BLOCK_DRILL_HOLE              "drillhole"
#define KEY_BLOCK_MODEL                   "model"

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

Gb                 _MiIoWriteBlockStart(  GmineInfo       * const gmineInfo, Gstr       * const key);
Gb                 _MiIoWriteBlockStop(   GmineInfo       * const gmineInfo);
Gb                 _MiIoWritePoint(       GmineInfo       * const gmineInfo, Gstr const * const key, GmineInfoPoint const * const value);
Gb                 _MiIoWriteString(      GmineInfo       * const gmineInfo, Gstr const * const key, Gstr * const value);

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

#define            _MiStrGetCount(        STR, MAX_LEN)                   (Gcount) (strnlen(STR, (size_t) (MAX_LEN)))
Gb                 _MiStrIsEmpty(         Gstr const * const value);
#define            _MiStrIsEqual(         STR, STR_ALT)                   _MiMemIsEqual(_MiStrGetCount(STR, GkeyBYTE_COUNT), STR, _MiStrGetCount(STR_ALT, GkeyBYTE_COUNT), STR_ALT)
Gstr              *_MiStrClone(           Gstr const * const value);

#endif
