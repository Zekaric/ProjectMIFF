/******************************************************************************
file:       local
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) !!!!YEAR!!!!, Robbert de Groot

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
#include "mifflib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
#define loop                                       for (;;)
#define forCount(    INDEX, COUNT)                 for ((INDEX) = 0;         (INDEX) < (COUNT); (INDEX)++)
#define forCountDown(INDEX, COUNT)                 for ((INDEX) = COUNT - 1; (INDEX) >= 0;      (INDEX)--)

#define breakIf(EXP)                               if (EXP) { break;    }
#define continueIf(EXP)                            if (EXP) { continue; }

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

#define memClearType(             TYPE, MEM)                memset((MEM), 0,               sizeof(TYPE))
#define memClearTypeArray( COUNT, TYPE, MEM)                memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define memCopyTypeArray(  COUNT, TYPE, DST, SRC)           memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define memCreateType(            TYPE)            (TYPE *) _memCreate(                    sizeof(TYPE))
#define memCreateTypeArray(COUNT, TYPE)            (TYPE *) _memCreate(          (COUNT) * sizeof(TYPE))
#define memDestroy(                     MEM)                _memDestroy(MEM)

#define _C1GetCount(STR)                       strlen(STR)
#define _C2GetCount(STR)                       wcslen(STR)

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
void      _Base64Restart(                    void);
MiffBool  _Base64Get(                        Miff * const miff, Miff1 * const byte);
MiffBool  _Base64Set(                        Miff * const miff, Miff1 * const byte);
MiffBool  _Base64SetEnd(                     Miff * const miff);

void      _ByteSwap2(                        Miff const * const miff, Miff2 * const value);
void      _ByteSwap4(                        Miff const * const miff, Miff4 * const value);
void      _ByteSwap8(                        Miff const * const miff, Miff8 * const value);

MiffBool  _C1ToC1Encoded(                    MiffN4 const c1Count, MiffC1 const * const c1, MiffN4 * const c1eCount, MiffC1 ** const c1e);
void      _C1ToC2(                           MiffN4 const c1Count, MiffC1 const * const c1, MiffN4 * const c2Count,  MiffC2 *  const c2);
void      _C1ToC2Key(                        MiffN4 const c1Count, MiffC1 const * const c1, MiffN1 * const c2Count,  MiffC2 *  const c2);
MiffI8    _C1ToI(                            MiffN4 const c1Count, MiffC1 const * const c1);
MiffN8    _C1ToN(                            MiffN4 const c1Count, MiffC1 const * const c1);
MiffC2   *_C2Clone(                          MiffN4 const c2Count, MiffC2 const * const c2);
MiffBool  _C2ToC1(                           MiffN4 const c2Count, MiffC2 const * const c2, MiffN4 * const c1Count, MiffC1 ** const c1);
MiffBool  _C2ToC1Key(                        MiffN4 const c2Count, MiffC2 const * const c2, MiffN1 * const c1Count, MiffC1 *  const c1);
MiffN4    _C1LetterToC4Letter(               MiffC1 const * const c1, MiffC4 * const c4);
MiffN4    _C2LetterToC4Letter(               MiffC2 const * const c2, MiffC4 * const c4);
MiffN4    _C4LetterToC1Letter(               MiffC4 const c4Letter, MiffC1 * const a, MiffC1 * const b, MiffC1 * const c, MiffC1 * const d);
MiffN4    _C4LetterToC2Letter(               MiffC4 const c4Letter, MiffC2 * const a, MiffC2 * const b);
MiffBool  _CurrentIndexInc(                  Miff       * const miff);

MiffBool  _MemIsEqual(                       MiffN4 const countA, MiffN1 const * const memA, MiffN4 const countB, MiffN1 const * const memB);

MiffBool  _WriteTxtRecordArrayCount(         Miff const * const miff, MiffN4 const count);
MiffBool  _WriteTxtRecordCompressChunkSize(  Miff const * const miff, MiffN4 const chunkSize);
MiffBool  _WriteTxtRecordCompressFlag(       Miff const * const miff, MiffCompressFlag const compressFlag);
MiffBool  _WriteTxtRecordEnder(              Miff const * const miff);
MiffBool  _WriteTxtRecordSeparator(          Miff const * const miff);
MiffBool  _WriteTxtRecordType(               Miff const * const miff, MiffValueType const type);
MiffBool  _WriteTxtC1(                       Miff const * const miff, MiffC1 const * const value);
MiffBool  _WriteTxtC2(                       Miff const * const miff, MiffC2 const * const value);
MiffBool  _WriteTxtValue1(                   Miff const * const miff, MiffValueType const type, Miff1  const value);
MiffBool  _WriteTxtValue2(                   Miff const * const miff, MiffValueType const type, Miff2  const value);
MiffBool  _WriteTxtValue4(                   Miff const * const miff, MiffValueType const type, Miff4  const value);
MiffBool  _WriteTxtValue8(                   Miff const * const miff, MiffValueType const type, Miff8  const value);
MiffBool  _WriteTxtValueI(                   Miff const * const miff,                           MiffI8 const value);
MiffBool  _WriteTxtValueN(                   Miff const * const miff,                           MiffN8 const value);

#endif
