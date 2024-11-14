/******************************************************************************
file:       MiffLib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw MIFF files.
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

#if !defined(MIFFLIB_H)
#define      MIFFLIB_H

/******************************************************************************
include:
******************************************************************************/
#include <stdint.h>
#include <float.h>
#include <math.h>

/******************************************************************************
local:
constant:
******************************************************************************/
#define miffKeyBYTE_COUNT                 256
#define miffKeySIZE                       257

typedef enum
{
   miffFALSE,
   miffTRUE
} MiffB;

typedef enum
{
   miffDataIS_DATA,
   miffDataIS_PART_DONE,
   miffDataIS_RECORD_DONE,

   miffDataERROR
} MiffData;

typedef enum
{
   miffMethodREADING,
   miffMethodWRITING,
} MiffMethod;

typedef enum
{
   miffRecTypeNONE,

   miffRecTypeBLOCK_START,
   miffRecTypeBLOCK_STOP,
   miffRecTypeVALUE
} MiffRecType;

typedef enum
{
   miffValueTypeNONE,

   miffValueTypeNULL,     // ~
   miffValueTypeSTR,      // "
   miffValueTypeNUM_INT,  // - 0 1 2 3 4 5 6 7 8 9 A B C D E F
   miffValueTypeNUM_REAL, //   G H I J K L M N O P Q R S T U V
   miffValueTypeBIN,      // *
   miffValueTypeOTHER     // Any other character.
} MiffValueType;

#define miffArrayCountBLOCK_START      (UINT64_MAX - 2)
#define miffArrayCountBLOCK_STOP       (UINT64_MAX - 1)
#define miffArrayCountUNKNOWN           UINT64_MAX
#define miffBufferCountUNKNOWN          UINT64_MAX

/******************************************************************************
type:
******************************************************************************/
// Integer type -X to X
typedef int64_t                        MiffI;
typedef int32_t                        MiffI4;
// Natural type 0 to X
typedef uint64_t                       MiffN;
typedef uint32_t                       MiffN4;
typedef uint8_t                        MiffN1;
// Real types X.XXX
typedef double                         MiffR;
typedef float                          MiffR4;
// String (UTF8) type
typedef char                           MiffStr;

#define MiffR_MAX                      DBL_MAX
#define MiffR_EPSILON                  DBL_EPSILON
#define MiffR_NAN                      NAN
#define MiffR4_MAX                     FLT_MAX
#define MiffR4_EPSILON                 FLT_EPSILON
#define MiffN_MAX                      UINT64_MAX
#define MiffN4_MAX                     UINT32_MAX
#define MiffI_MAX                      INT64_MAX
#define MiffI_MIN                      INT64_MIN
#define MiffI4_MAX                     INT32_MAX

typedef struct
{
   MiffR4             real;
   MiffR4             imaginary;
} MiffComplex4;

typedef struct
{
   MiffR              real;
   MiffR              imaginary;
} MiffComplex;

typedef union
{
   MiffStr           *str;
   MiffN1            *bin;
} MiffP;

typedef union
{
   MiffI4             i;
   MiffN4             n;
   MiffR4             r;
   MiffN1             byte[4];
} Miff4;

typedef union
{
   MiffI              i;
   MiffN              n;
   MiffR              r;
   MiffN1             byte[8];
} Miff8;

typedef struct
{
   MiffValueType      type;
   MiffStr            header;
   MiffB              isI;
   MiffB              isR4;
   Miff4              inr4;
   Miff8              inr;
   MiffN              bufferCount;
   MiffP              bufferData;
} MiffValue;

typedef void *(*MiffMemCreate)(  MiffN4 const memByteCount);
typedef void  (*MiffMemDestroy)( void * const mem);

typedef MiffB (*MiffGetBuffer)(  void * const dataSource,      MiffN4 const byteCount, MiffN1       * const data);

typedef MiffB (*MiffSetBuffer)(  void * const dataDestination, MiffN4 const byteCount, MiffN1 const * const data);

typedef struct
{
   // MIFF file information and configuration.
   MiffN                       version;
   MiffMethod                  method;
   MiffB                       isByteSwapping;
   MiffB                       isPartDone;
   MiffB                       isRecordDone;

   MiffStr                     subFormatName[   miffKeySIZE];
   MiffStr                     subFormatVersion[miffKeySIZE];

   // The current value information.
   MiffN                       currentScopeLevel;
   MiffStr                     currentName[miffKeySIZE];
   MiffN                       currentNameCount;
   MiffN                       currentArrayCount;
   MiffN                       currentArrayIndex;

   // Data repo getters and setters.
   void                       *dataRepo;
   MiffGetBuffer               getBuffer;
   MiffSetBuffer               setBuffer;

   // Temporary storage for uncompressed memory.
   MiffValue                   value;
   MiffN                       valueIndex;

   MiffN4                      readCount;
   MiffN1                      readData[miffKeySIZE];
} Miff;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Miff           *miffCreateReader(                                           MiffB const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffStr       * const subFormatVersion, void * const dataRepo);
MiffB           miffCreateReaderContent(           Miff       * const miff, MiffB const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffStr       * const subFormatVersion, void * const dataRepo);
Miff           *miffCreateWriter(                                           MiffB const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffStr const * const subFormatVersion, void * const dataRepo);
MiffB           miffCreateWriterContent(           Miff       * const miff, MiffB const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffStr const * const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Miff       * const miff);
void            miffDestroyContent(                Miff       * const miff);

MiffB           miffGetRecordStart(                Miff       * const miff, MiffRecType * const type, MiffN * const count, MiffStr       * const key);
MiffB           miffGetRecordEnd(                  Miff       * const miff);
MiffValue       miffGetValue(                      Miff       * const miff);
MiffB           miffGetValueBin(                   Miff       * const miff, MiffN const binCount, MiffN1  * const binBuffer);
MiffData        miffGetValueBinData(               Miff       * const miff, MiffN1 * const binByte);
MiffB           miffGetValueStr(                   Miff       * const miff, MiffN const strCount, MiffStr * const str);
MiffData        miffGetValueStrData(               Miff       * const miff, MiffStr * const strLetter);

MiffB           miffSetRecordStart(                Miff       * const miff, MiffRecType   const type, MiffN   const count, MiffStr const * const key);
MiffB           miffSetRecordStop(                 Miff       * const miff);
MiffB           miffSetSeparator(                  Miff       * const miff);
MiffB           miffSetValue(                      Miff       * const miff, MiffValue const value);
MiffB           miffSetValueBinData(               Miff       * const miff, MiffN1 const binByte);
MiffB           miffSetValueStart(                 Miff       * const miff, MiffValue const value);
MiffB           miffSetValueStop(                  Miff       * const miff);
MiffB           miffSetValueStrData(               Miff       * const miff, MiffStr const strLetter);

MiffB           miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy);
void            miffStop(                          void);

/******************************************************************************
miffValue
prototype:
******************************************************************************/
MiffB           miffValueGetB(                     MiffValue const value);
MiffN           miffValueGetBinCount(              MiffValue const value);
MiffI           miffValueGetI(                     MiffValue const value);
MiffN           miffValueGetN(                     MiffValue const value);
MiffR           miffValueGetR(                     MiffValue const value);
MiffR4          miffValueGetR4(                    MiffValue const value);
MiffN           miffValueGetStrCount(              MiffValue const value);
MiffValueType   miffValueGetType(                  MiffValue const value);

MiffB           miffValueIs4(                      MiffValue const value);

MiffValue       miffValueSetB(                     MiffB  const bvalue);
MiffValue       miffValueSetBinBuffer(             MiffN  const binCount, MiffN1  * const binBuffer);
MiffValue       miffValueSetBinCount(              MiffN  const binCount);
MiffValue       miffValueSetI(                     MiffI  const ivalue);
MiffValue       miffValueSetN(                     MiffN  const nvalue);
MiffValue       miffValueSetNull(                  void);
MiffValue       miffValueSetR(                     MiffR  const rvalue);
MiffValue       miffValueSetR4(                    MiffR4 const rvalue);
MiffValue       miffValueSetStrBuffer(             MiffN  const strCount, MiffStr * const strBuffer);
MiffValue       miffValueSetStrCount(              MiffN  const strCount);

// Convenience macroes
#define miffValueSetStr(                  VALUE)                              miffValueSetStrBuffer(strlen(VALUE), VALUE)

#define miffSetRecordBlockStart(          MIFF, NAME)                       { miffSetRecordStart(MIFF, miffRecTypeBLOCK_START, 0, NAME); miffSetRecordStop(MIFF); }
#define miffSetRecordBlockStop(           MIFF)                             { miffSetRecordStart(MIFF, miffRecTypeBLOCK_STOP,  0, NULL); miffSetRecordStop(MIFF); }

#define miffSetRecordValueNull(           MIFF)                             { MiffValue __value__; __value__ = miffValueSetNull(                     ); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueB(              MIFF,        VALUE)               { MiffValue __value__; __value__ = miffValueSetB(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueI(              MIFF,        VALUE)               { MiffValue __value__; __value__ = miffValueSetI(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueN(              MIFF,        VALUE)               { MiffValue __value__; __value__ = miffValueSetN(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueR(              MIFF,        VALUE)               { MiffValue __value__; __value__ = miffValueSetR(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueR4(             MIFF,        VALUE)               { MiffValue __value__; __value__ = miffValueSetR4(                (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueStr(            MIFF,        VALUE)               { MiffValue __value__; __value__ = miffValueSetStr(               (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueBinBuffer(      MIFF, COUNT, VALUE)               { MiffValue __value__; __value__ = miffValueSetBinBuffer((COUNT), (VALUE)); miffSetValue(MIFF, __value__); }

#define miffSetRecordNull(                MIFF,       NAME)                 { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueNull(     MIFF                  ); } miffSetRecordStop(MIFF); }
#define miffSetRecordBool(                MIFF,       NAME,        VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueB(        MIFF,          (VALUE)); } miffSetRecordStop(MIFF); }
#define miffSetRecordI(                   MIFF,       NAME,        VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueI(        MIFF,          (VALUE)); } miffSetRecordStop(MIFF); }
#define miffSetRecordN(                   MIFF,       NAME,        VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueN(        MIFF,          (VALUE)); } miffSetRecordStop(MIFF); }
#define miffSetRecordR(                   MIFF,       NAME,        VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueR(        MIFF,          (VALUE)); } miffSetRecordStop(MIFF); }
#define miffSetRecordR4(                  MIFF,       NAME,        VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueR4(       MIFF,          (VALUE)); } miffSetRecordStop(MIFF); }
#define miffSetRecordStr(                 MIFF,       NAME,        VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueStr(      MIFF,          (VALUE)); } miffSetRecordStop(MIFF); }
#define miffSetRecordBinBuffer(           MIFF,       NAME, COUNT, VALUE)   { miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME); { miffSetRecordValueBinBuffer(MIFF, (COUNT), (VALUE)); } miffSetRecordStop(MIFF); }

#define miffSetRecordStartBinArray(       MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartBoolArray(      MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartIArray(         MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartNArray(         MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartRArray(         MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartR4Array(        MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartStrArray(       MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartValueArray(     MIFF,       NAME, COUNT)            miffSetRecordStart(MIFF, miffRecTypeVALUE, COUNT, NAME)
#define miffSetRecordStartValue(          MIFF,       NAME)                   miffSetRecordStart(MIFF, miffRecTypeVALUE, 1,     NAME);

#define miffSetRecordBoolArray(           MIFF, NAME, COUNT, ARRAY)         { miffSetRecordStartBoolArray(MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetRecordValueB(  MIFF, (ARRAY[__index__])); } miffSetRecordStop(MIFF); }
#define miffSetRecordIArray(              MIFF, NAME, COUNT, ARRAY)         { miffSetRecordStartIArray(   MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetRecordValueI(  MIFF, (ARRAY[__index__])); } miffSetRecordStop(MIFF); }
#define miffSetRecordNArray(              MIFF, NAME, COUNT, ARRAY)         { miffSetRecordStartNArray(   MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetRecordValueN(  MIFF, (ARRAY[__index__])); } miffSetRecordStop(MIFF); }
#define miffSetRecordRArray(              MIFF, NAME, COUNT, ARRAY)         { miffSetRecordStartRArray(   MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetRecordValueR(  MIFF, (ARRAY[__index__])); } miffSetRecordStop(MIFF); }
#define miffSetRecordR4Array(             MIFF, NAME, COUNT, ARRAY)         { miffSetRecordStartR4Array(  MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetRecordValueR4( MIFF, (ARRAY[__index__])); } miffSetRecordStop(MIFF); }
#define miffSetRecordStrArray(            MIFF, NAME, COUNT, ARRAY)         { miffSetRecordStartStrArray( MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetRecordValueStr(MIFF, (ARRAY[__index__])); } miffSetRecordStop(MIFF); }

#endif
