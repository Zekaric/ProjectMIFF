/**************************************************************************************************
file:       gmiff_local
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
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

#if !defined(GLIBMIFF_LOCAL)
#define      GLIBMIFF_LOCAL

/**************************************************************************************************
include:
**************************************************************************************************/
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "gmifflib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/

#define MIFF_HEADER_FILETYPE_STR                   "MIFF"
#define MIFF_HEADER_VERSION                        1

#define miffTypeBLOCK_START_STR                    "{"
#define miffTypeBLOCK_START_STR_SIZE               1

#define miffTypeBLOCK_STOP_STR                     "}"
#define miffTypeBLOCK_STOP_STR_SIZE                1

#define miffTypeTYPE_STR                           "t"
#define miffTypeTYPE_STR_SIZE                      1

#define miffTypeTYPE_DEFINITION_STR                "type"
#define miffTypeTYPE_DEFINITION_STR_SIZE           4

#define miffTypeSTR_STR                            "\""
#define miffTypeSTR_STR_SIZE                       1

#define miffTypeBOOLEAN_STR                        "b"
#define miffTypeBOOLEAN_STR_SIZE                   1

#define miffTypeI_STR                              "i"
#define miffTypeI_STR_SIZE                         1

#define miffTypeN_STR                              "n"
#define miffTypeN_STR_SIZE                         1

#define miffTypeR_STR                              "r"
#define miffTypeR_STR_SIZE                         1

#if 0
#define miffTypeR_FIXED_STR                        "R"
#define miffTypeR_FIXED_STR_SIZE                   1
#endif

#define miffTypeBINARY_STR                         "*"
#define miffTypeBINARY_STR_SIZE                    1

#define miffTypeSTR_SIZE_MAX                       16

/**************************************************************************************************
type:
**************************************************************************************************/

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/
Gb                 _MiffGetBin(              Gmiff       * const miff, Gcount const binCount, Gn1 * const binBuffer);
GmiffData          _MiffGetBinByte(          Gmiff       * const miff, Gn1 * const binByte);
Gb                 _MiffGetLineSkip(         Gmiff       * const miff);
Gb                 _MiffGetNum(              Gmiff       * const miff);
Gb                 _MiffGetPartRest(         Gmiff       * const miff, Gn1 const start);
Gb                 _MiffGetPart(             Gmiff       * const miff, Gb const trimLeadingTabs);
Gb                 _MiffGetPartEnd(          Gmiff       * const miff);
Gb                 _MiffGetStr(              Gmiff       * const miff, Gcount const strCount, Gstr * const str);
GmiffData          _MiffGetStrLetter(        Gmiff       * const miff, Gstr * const letter);
Gcount             _MiffGetValueCount(       Gmiff       * const miff);
Gstr               _MiffGetValueHeader(      Gmiff       * const miff);

_locale_t          _MiffLocaleGet(           void);

void              *_MiffMemCloc(             Gcount const memByteCount);
void               _MiffMemDloc(             void * const mem);
void               _MiffMemStart(            GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void               _MiffMemStop(             void);

#define            _MiffMemClearType(        MEM, TYPE)                      memset((MEM), 0,               sizeof(TYPE))
#define            _MiffMemClearTypeArray(   MEM, TYPE, COUNT)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define            _MiffMemCopyTypeArray(    MEM, TYPE, COUNT, SRC)          memcpy((MEM), (SRC), (COUNT) * sizeof(TYPE))
#define            _MiffMemClocType(                TYPE)           (TYPE *) _MiffMemCloc(                    sizeof(TYPE))
#define            _MiffMemClocTypeArray(    COUNT, TYPE)           (TYPE *) _MiffMemCloc(          (COUNT) * sizeof(TYPE))

Gb                 _MiffSetBinByte(          Gmiff       * const miff, Gn1 const binByte);
Gb                 _MiffSetBuffer(           Gmiff const * const miff, Gcount const bufCount, Gn1 const * const buf);
Gb                 _MiffSetStr(              Gmiff       * const miff, Gcount const strCount, Gstr const * const strBuffer);
Gb                 _MiffSetValueHeader(      Gmiff       * const miff, GmiffValue const value);
Gb                 _MiffSetValueData(        Gmiff       * const miff, GmiffValue const value);

#endif
