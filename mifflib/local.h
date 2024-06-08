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

#include "mifflib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
#define loop                                       for (;;)
#define forCount(    INDEX, COUNT)                 for ((INDEX) = 0;         (INDEX) < (COUNT); (INDEX)++)
#define forCountDown(INDEX, COUNT)                 for ((INDEX) = COUNT - 1; (INDEX) >= 0;      (INDEX)--)
#define once                                       for (int __count__ = 0; __count__ < 1; __count__++)

#define breakIf(   EXP)                            if (EXP) { break;    }
#define continueIf(EXP)                            if (EXP) { continue; }
#define gotoIf(    EXP, LABEL)                     if (EXP) { goto LABEL; }

#if defined(min)
#undef min
#undef max
#endif
#define min(A, B)                                  (((A) < (B)) ? (A) : (B))
#define max(A, B)                                  (((A) < (B)) ? (B) : (A))

#define returnFalse                                return miffBoolFALSE
#define returnNull                                 return NULL
#define returnTrue                                 return miffBoolTRUE
#define returnIf(     EXP, VALUE)                  if (EXP) { return (VALUE); }
#define return0If(    EXP)                         if (EXP) { return 0;       }
#define returnFalseIf(EXP)                         if (EXP) { returnFalse;    }
#define returnNullIf( EXP)                         if (EXP) { returnNull;     }
#define returnTrueIf( EXP)                         if (EXP) { returnTrue;     }
#define returnVoidIf( EXP)                         if (EXP) { return;         }

#define MIFF_HEADER_FILETYPE_STR                   "MIFF"
#define MIFF_HEADER_FILETYPE_SIZE                  4

#define MIFF_HEADER_VERSION_STR                    "1"
#define MIFF_HEADER_VERSION_SIZE                   1

#define miffTypeKEY_VALUE_BLOCK_START_STR          "{"
#define miffTypeKEY_VALUE_BLOCK_START_STR_SIZE     1

#define miffTypeKEY_VALUE_BLOCK_STOP_STR           "}"
#define miffTypeKEY_VALUE_BLOCK_STOP_STR_SIZE      1

#define miffTypeTYPE_STR                           "type"
#define miffTypeTYPE_STR_SIZE                      4

#define miffTypeSTR_STR                            "\""
#define miffTypeSTR_STR_SIZE                       1

#define miffTypeVARIABLE_STR                       "v"
#define miffTypeVARIABLE_STR_SIZE                  1

#define miffTypeUSER_TYPE_STR                      "u"
#define miffTypeUSER_TYPE_STR_SIZE                 1

#define miffTypeBOOLEAN_STR                        "b"
#define miffTypeBOOLEAN_STR_SIZE                   1

#define miffTypeI_STR                              "i"
#define miffTypeI_STR_SIZE                         1

#define miffTypeI1_STR                             "i1"
#define miffTypeI1_STR_SIZE                        2

#define miffTypeI2_STR                             "i2"
#define miffTypeI2_STR_SIZE                        2

#define miffTypeI4_STR                             "i4"
#define miffTypeI4_STR_SIZE                        2

#define miffTypeI8_STR                             "i8"
#define miffTypeI8_STR_SIZE                        2

#define miffTypeN_STR                              "n"
#define miffTypeN_STR_SIZE                         1

#define miffTypeN1_STR                             "n1"
#define miffTypeN1_STR_SIZE                        2

#define miffTypeN2_STR                             "n2"
#define miffTypeN2_STR_SIZE                        2

#define miffTypeN4_STR                             "n4"
#define miffTypeN4_STR_SIZE                        2

#define miffTypeN8_STR                             "n8"
#define miffTypeN8_STR_SIZE                        2

#define miffTypeR4_STR                             "r4"
#define miffTypeR4_STR_SIZE                        2

#define miffTypeR4S_STR                            "r4s"
#define miffTypeR4S_STR_SIZE                       3

#define miffTypeR8_STR                             "r8"
#define miffTypeR8_STR_SIZE                        2

#define miffTypeR8S_STR                            "r8s"
#define miffTypeR8S_STR_SIZE                       3

#define miffTypeSTR_SIZE_MAX                       16

/******************************************************************************
type:
******************************************************************************/
typedef struct
{
   MiffN4  state;
   MiffN1  byte;
   MiffN4  index;
   MiffN1 *buffer;
} MiffBase64Data;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
MiffBase64Data  _MiffBase64Restart(     MiffN1 * const buffer);
MiffBool        _MiffBase64Get(         MiffBase64Data * const buffer, MiffN1 * const byte);
MiffBool        _MiffBase64Set(         MiffBase64Data * const buffer, MiffN1   const byte);
MiffBool        _MiffBase64SetEnd(      MiffBase64Data * const buffer);
void            _MiffBase64Start(       void);

void            _MiffByteSwap4(         Miff const * const miff, Miff4 * const value);
void            _MiffByteSwap8(         Miff const * const miff, Miff8 * const value);

_locale_t       _MiffLocaleGet(         void);

MiffBool        _MiffStrEncodedToStr(   MiffN   * const strLen, MiffStr       * const str);
MiffBool        _MiffStrToStrEncoded(   MiffN     const strLen, MiffStr const * const str, MiffN * const strEncodedLen, MiffStr ** const strEncoded);
MiffI           _MiffStrToI(            MiffN     const strLen, MiffStr const * const str);
MiffN           _MiffStrToN(            MiffN     const strLen, MiffStr const * const str);
MiffStr        *_MiffStrAppend(         MiffStr const * const a, MiffStr const * const b, MiffStr const * const c);
MiffStr        *_MiffStrClone(          MiffN     const strLen, MiffStr const * const str);
MiffBool        _MiffStrToKey(          MiffN     const strLen, MiffStr const * const str, MiffN * const keyLen, MiffStr *  const key);

#define         _MiffStrGetCount(STR)                           ((MiffN4) strlen((char const *)    STR))

void           *_MiffMemCreate(         MiffN const memByteCount);
void            _MiffMemDestroy(        void * const mem);
MiffBool        _MiffMemIsEqual(        MiffN const countA, MiffN1 const * const memA, MiffN const countB, MiffN1 const * const memB);
void            _MiffMemStart(          MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc);
void            _MiffMemStop(           void);

#define         _MiffMemClearType(             TYPE, MEM)               memset((MEM), 0,               sizeof(TYPE))
#define         _MiffMemClearTypeArray( COUNT, TYPE, MEM)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define         _MiffMemCopyTypeArray(  COUNT, TYPE, DST, SRC)          memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define         _MiffMemCreateType(            TYPE)           (TYPE *) _MiffMemCreate(                    sizeof(TYPE))
#define         _MiffMemCreateTypeArray(COUNT, TYPE)           (TYPE *) _MiffMemCreate(          (COUNT) * sizeof(TYPE))

MiffBool        _MiffReadArrayCount(    Miff       * const miff, MiffN    * const count);
MiffBool        _MiffReadKey(           Miff       * const miff, MiffStr  * const key);
MiffBool        _MiffReadLineSkip(      Miff       * const miff);
MiffBool        _MiffReadPart(          Miff       * const miff, MiffBool const trimLeadingTabs);
MiffBool        _MiffReadR4(            Miff       * const miff, MiffValue * const value);
MiffBool        _MiffReadR4S(           Miff       * const miff, MiffValue * const value);
MiffBool        _MiffReadR8(            Miff       * const miff, MiffValue * const value);
MiffBool        _MiffReadR8S(           Miff       * const miff, MiffValue * const value);
MiffBool        _MiffReadType(          Miff       * const miff, MiffType * const type, MiffStr * const typeName);

MiffStr        *_MiffTypeGetName(       MiffType const type);
MiffN4          _MiffTypeGetNameSize(   MiffType const type);

void            _MiffUtilStart(         void);
void            _MiffUtilStop(          void);

MiffBool        _MiffWriteStr(          Miff const * const miff, MiffN const strLen, MiffStr const * const str);
MiffBool        _MiffWriteI(            Miff       * const miff, MiffI  const value);
MiffBool        _MiffWriteN(            Miff       * const miff, MiffN  const value);
MiffBool        _MiffWriteR4(           Miff       * const miff, MiffR4 const value);
MiffBool        _MiffWriteR4S(          Miff       * const miff, MiffR4 const value);
MiffBool        _MiffWriteR8(           Miff       * const miff, MiffR8 const value);
MiffBool        _MiffWriteR8S(          Miff       * const miff, MiffR8 const value);

#endif
