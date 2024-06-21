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
   miffValueFormatN_HUMAN_READABLE,
   miffValueFormatN_B,
   miffValueFormatN_BASE64,
   miffValueFormatN_O,
   miffValueFormatN_X
} MiffValueFormatN;

typedef enum
{
   miffValueFormatCIR_HUMAN_READABLE,
   miffValueFormatCIR_BASE64
} MiffValueFormatCIR;

typedef enum
{
   miffValueTypeNONE,

   miffValueTypeNULL,   // ~
   miffValueTypeSTR,    // "
   miffValueTypeC,      // c, C
   miffValueTypeC4,     // c, C
   miffValueTypeB,      // b
   miffValueTypeI,      // i, I
   miffValueTypeN,      // x, o, b
   miffValueTypeR,      // r, R
   miffValueTypeR4,     // r, R
   miffValueTypeBIN,    // ~
   miffValueTypeOTHER   // Any other character.
} MiffValueType;

#define miffArrayCountBLOCK_START      (UINT64_MAX - 2)
#define miffArrayCountBLOCK_STOP       (UINT64_MAX - 1)
#define miffArrayCountUNKNOWN           UINT64_MAX

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

#define MiffN4_MAX                     UINT32_MAX
#define MiffN8_MAX                     UINT64_MAX
#define MiffI4_MAX                     INT32_MAX
#define MiffI8_MAX                     INT64_MAX

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
   char              *str;
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
   MiffValueFormatN   formatN;
   MiffValueFormatCIR formatCIR;
   MiffStr            header;
   MiffB              is4;
   MiffB              b;
   Miff4              inr4;
   Miff8              inr;
   Miff8              imaginary;
   Miff4              imaginary4;
   MiffN              bufferCount;
   MiffP              bufferData;
} MiffValue;

typedef void *(*MiffMemCreate)(  MiffN4 const memByteCount);
typedef void  (*MiffMemDestroy)( void * const mem);

typedef MiffB (*MiffGetBuffer)(  void * const dataSource,      MiffN4 const byteCount, MiffStr       * const data);

typedef MiffB (*MiffSetBuffer)(  void * const dataDestination, MiffN4 const byteCount, MiffStr const * const data);

typedef struct
{
   // MIFF file information and configuration.
   MiffN                       version;
   MiffMethod                  method;
   MiffB                       isByteSwapping;
   MiffB                       isRecordDone;

   MiffStr                     subFormatName[miffKeySIZE];
   MiffN                       subFormatVersion;

   // The list of types, all types even the predefined base types.
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
   MiffN                       memByteCount;
   void                       *memByteData;

   // Temporary storage for reading
   MiffN                       readByteCountActual;
   MiffN                       readByteCount;
   MiffN1                     *readByteData;

   MiffN                       readStrCountActual;
   MiffN                       readStrCount;
   MiffStr                    *readStrData;
} Miff;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Miff           *miffCreateReader(                                           MiffB const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffN * const subFormatVersion, void * const dataRepo);
MiffB           miffCreateReaderContent(           Miff       * const miff, MiffB const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffN * const subFormatVersion, void * const dataRepo);
Miff           *miffCreateWriter(                                           MiffB const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN   const subFormatVersion, void * const dataRepo);
MiffB           miffCreateWriterContent(           Miff       * const miff, MiffB const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN   const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Miff       * const miff);
void            miffDestroyContent(                Miff       * const miff);

MiffB           miffGetInfo(                       Miff       * const miff, MiffRecType * const type, MiffN * const count, MiffStr       * const key);
MiffB           miffGetRecordEnd(                  Miff       * const miff);
MiffValue       miffGetValueHeader(                Miff       * const miff);
MiffStr         miffGetValueData(                  Miff const * const miff);

MiffB           miffSetInfo(                       Miff       * const miff, MiffRecType   const type, MiffN   const count, MiffStr const * const key);
MiffB           miffSetSeparator(                  Miff       * const miff);
MiffB           miffSetRecordEnd(                  Miff       * const miff);
MiffB           miffSetValue(                      Miff       * const miff, MiffValue value);
MiffB           miffSetValueData(                  Miff       * const miff, MiffStr const * const data);

MiffN1 const   *miffValueGetBinBuffer(             MiffValue const value);
MiffN           miffValueGetByteCount(             MiffValue const value);
MiffB           miffValueGetB(                     MiffValue const value);
MiffI           miffValueGetI(                     MiffValue const value);
MiffN           miffValueGetIs4(                   MiffValue const value);
MiffN           miffValueGetN(                     MiffValue const value);
MiffR           miffValueGetR(                     MiffValue const value);
MiffR4          miffValueGetR4(                    MiffValue const value);
MiffStr const  *miffValueGetStrBuffer(             MiffValue const value);
MiffN           miffValueGetStrCount(              MiffValue const value);
MiffValueType   miffValueGetType(                  MiffValue const value);

MiffValue       miffValueSetB(                     MiffB  const value);
MiffValue       miffValueSetBinBuffer(             MiffN  const binCount, MiffN1  * const binBuffer);
MiffValue       miffValueSetI(                     MiffI  const value, MiffValueFormatCIR const format);
MiffValue       miffValueSetN(                     MiffN  const value, MiffValueFormatN   const format);
MiffValue       miffValueSetR(                     MiffR  const value, MiffValueFormatCIR const format);
MiffValue       miffValueSetR4(                    MiffR4 const value, MiffValueFormatCIR const format);
MiffValue       miffValueSetStrBuffer(             MiffN  const strCount, MiffStr * const strBuffer);

MiffB           miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy);
void            miffStop(                          void);

// Convenience macroes
#define miffValueSetString(         VALUE)                       miffValueSetStr(strlen(VALUE), VALUE)

#define miffSetBlockStart(          MIFF, NAME)                  miffSetInfo(MIFF, miffTypeBLOCK_START, NULL, NAME, miffArrayCountUNKNOWN)
#define miffSetBlockStop(           MIFF)                        miffSetInfo(MIFF, miffTypeBLOCK_STOP,  NULL, NULL, miffArrayCountUNKNOWN)

#define miffSetBool(                MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeBOOL,        NULL, NAME,     1); miffSetValue(MIFF, miffValueSetBool(       VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetI(                   MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeI,           NULL, NAME,     1); miffSetValue(MIFF, miffValueSetI( (MiffI)  VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetN(                   MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeN,           NULL, NAME,     1); miffSetValue(MIFF, miffValueSetN( (MiffN)  VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetR4(                  MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeR,           NULL, NAME,     1); miffSetValue(MIFF, miffValueSetR4((MiffR4) VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetR8(                  MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeR,           NULL, NAME,     1); miffSetValue(MIFF, miffValueSetR8((MiffR8) VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetType(                MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeTYPE,        NULL, NAME,     1); miffSetValue(MIFF, miffValueSetType(       VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetStr(                 MIFF,       NAME, VALUE)   { miffSetInfo(MIFF, miffTypeSTR,         NULL, NAME,     1); miffSetValue(MIFF, miffValueSetString(     VALUE)); miffSetRecordEnd(MIFF); }

#define miffSetBoolArrayStart(      MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeBOOL,        NULL, NAME, COUNT)
#define miffSetIArrayStart(         MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeI,           NULL, NAME, COUNT)
#define miffSetNArrayStart(         MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeN,           NULL, NAME, COUNT)
#define miffSetR4ArrayStart(        MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeR,           NULL, NAME, COUNT)
#define miffSetR8ArrayStart(        MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeR,           NULL, NAME, COUNT)
#define miffSetTypeArrayStart(      MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeTYPE,        NULL, NAME, COUNT)
#define miffSetStrArrayStart(       MIFF,       NAME, COUNT)     miffSetInfo(MIFF, miffTypeSTR,         NULL, NAME, COUNT)

#define miffSetBoolArray(           MIFF, NAME, COUNT, ARRAY)  { miffSetBoolArrayStart(MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetBool(       ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetIArray(              MIFF, NAME, COUNT, ARRAY)  { miffSetIArrayStart(   MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetI( (MiffI)  ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetNArray(              MIFF, NAME, COUNT, ARRAY)  { miffSetNArrayStart(   MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetN( (MiffN)  ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetR4Array(             MIFF, NAME, COUNT, ARRAY)  { miffSetR4ArrayStart(  MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetR4((MiffR4) ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetR8Array(             MIFF, NAME, COUNT, ARRAY)  { miffSetR8ArrayStart(  MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetR8((MiffR8) ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetTypeArray(           MIFF, NAME, COUNT, ARRAY)  { miffSetTypeArrayStart(MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetType(       ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetStrArray(            MIFF, NAME, COUNT, ARRAY)  { miffSetStrArrayStart( MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetString(     ARRAY[__index__])); } miffSetRecordEnd(MIFF); }

#endif
