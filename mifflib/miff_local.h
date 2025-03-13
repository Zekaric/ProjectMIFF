/******************************************************************************
file:       local
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
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

#if !defined(LIBMIFF_LOCAL)
#define      LIBMIFF_LOCAL

/******************************************************************************
include:
******************************************************************************/
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "b64lib.h"
#include "mifflib.h"

/******************************************************************************
local:
constant:
******************************************************************************/

#define strIsEqual(COUNT, A, B)                    (strncmp((char *) A, (char *) B, COUNT) == 0)

#define MIFF_HEADER_FILETYPE_STR                   "MIFF"
#define MIFF_HEADER_FILETYPE_SIZE                  4

#define MIFF_HEADER_VERSION_STR                    "1"
#define MIFF_HEADER_VERSION_SIZE                   1

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

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
void               _MiffByteSwap4(           Gmiff const * const miff, Gmiff4 * const value);
void               _MiffByteSwap8(           Gmiff const * const miff, Gmiff8 * const value);

Gb              _MiffGetBinByte(          Gmiff       * const miff, Gn1 * const binByte);
Gb              _MiffGetKey(              Gmiff       * const miff);
Gb              _MiffGetLineSkip(         Gmiff       * const miff);
Gb              _MiffGetNumInt(           Gmiff       * const miff, Gn4 const count, Gn1 const * const buffer);
Gb              _MiffGetNumReal(          Gmiff       * const miff, Gn4 const count, Gn1 const * const buffer);
Gb              _MiffGetPartRest(         Gmiff       * const miff, Gn1 const start);
Gb              _MiffGetPart(             Gmiff       * const miff, Gb const trimLeadingTabs);
Gb              _MiffGetPartEnd(          Gmiff       * const miff);
GmiffData           _MiffGetStrLetter(        Gmiff       * const miff, Gstr * const letter);
Gstr            _MiffGetValueHeader(      Gmiff       * const miff);
Gn8              _MiffGetValueBufferCount( Gmiff       * const miff);

void              *_MiffMemCreate(           Gn8 const memByteCount);
void               _MiffMemDestroy(          void * const mem);
void               _MiffMemStart(            MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc);
void               _MiffMemStop(             void);

#define            _MiffMemClearType(             TYPE, MEM)               memset((MEM), 0,               sizeof(TYPE))
#define            _MiffMemClearTypeArray( COUNT, TYPE, MEM)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define            _MiffMemCopyTypeArray(  COUNT, TYPE, DST, SRC)          memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define            _MiffMemCreateType(            TYPE)           (TYPE *) _MiffMemCreate(                    sizeof(TYPE))
#define            _MiffMemCreateTypeArray(COUNT, TYPE)           (TYPE *) _MiffMemCreate(          (COUNT) * sizeof(TYPE))

Gb              _MiffSetBinByte(          Gmiff       * const miff, Gn1 const binByte);
Gb              _MiffSetBuffer(           Gmiff const * const miff, Gn8     const bufCount, Gn1 const * const buf);
Gb              _MiffSetNumInt(           Gmiff       * const miff, Gn8     const value);
Gb              _MiffSetStr(              Gmiff       * const miff, Gn8     const strCount, Gstr const * const strBuffer);
Gb              _MiffSetValueHeader(      Gmiff       * const miff, GmiffValue const value);
Gb              _MiffSetValueData(        Gmiff       * const miff, GmiffValue const value);

#define            _MiffStrGetCount(STR)                          ((Gn4) strlen((char const *)    STR))

#endif
