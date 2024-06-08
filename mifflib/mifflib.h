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
   miffBoolFALSE,
   miffBoolTRUE
} MiffBool;

typedef enum
{
   miffMethodREADING,
   miffMethodWRITING,
} MiffMethod;

typedef enum
{
   miffTypeNONE,

   miffTypeBLOCK_STOP,
   miffTypeBLOCK_START,
   miffTypeTYPE,
   miffTypeSTR,
   miffTypeVAR,
   miffTypeUSER,
   miffTypeBOOL,
   miffTypeI,
   miffTypeN,
   miffTypeR4,
   miffTypeR8,

   miffTypeCOUNT
} MiffType;

#define miffArrayCountUNKNOWN          UINT64_MAX
#define miffArrayCountUNKNOWN_MiffStr  "*"

/******************************************************************************
type:
******************************************************************************/
// Integer type -X to X
typedef int64_t                        MiffI;
// Natural type 0 to X
typedef uint64_t                       MiffN;
typedef uint32_t                       MiffN4;
typedef uint8_t                        MiffN1;
// Real types X.XXX
typedef float                          MiffR4;
typedef double                         MiffR8;
// String (UTF8) type
typedef char                           MiffStr;

#define MiffN4_MAX                     UINT32_MAX

typedef struct
{
   MiffType           type;
   MiffType           t;
   MiffBool           b;
   MiffI              i;
   MiffN              n;
   MiffR4             r4;
   MiffR8             r8;
   MiffN              strLen;
   MiffStr const     *str;
} MiffValue;

typedef union
{
   MiffR4             r;
   MiffN1             byte[4];
} Miff4;

typedef union
{
   MiffR8             r;
   MiffN1             byte[8];
} Miff8;

typedef void    *(*MiffMemCreate)(  MiffN4 const memByteCount);
typedef void     (*MiffMemDestroy)( void * const mem);

typedef MiffBool (*MiffGetBuffer)(  void * const dataSource,      MiffN4 const byteCount, MiffStr       * const data);

typedef MiffBool (*MiffSetBuffer)(  void * const dataDestination, MiffN4 const byteCount, MiffStr const * const data);

typedef struct
{
   MiffStr                     name[miffKeySIZE];
   MiffType                    type;
   MiffN                       arrayCount;
} MiffRecord;

typedef struct
{
   // MIFF file information and configuration.
   MiffN                       version;
   MiffMethod                  method;
   MiffBool                    isByteSwapping;

   MiffStr                     subFormatName[miffKeySIZE];
   MiffN                       subFormatVersion;

   // The list of types, all types even the predefined base types.
   MiffN                       currentScopeLevel;
   MiffRecord                  currentRecord;
   MiffN                       currentArrayIndex;

   // Data repo getters and setters.
   void                       *dataRepo;
   MiffGetBuffer               getBuffer;
   MiffSetBuffer               setBuffer;

   // Temporary storage for uncompressed memory.
   MiffN                       memByteCount;
   void                       *memByteData;

   // Temporary storage for reading
   MiffBool                    readRecordIsDone;
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
Miff           *miffCreateReader(                                           MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffN * const subFormatVersion, void * const dataRepo);
MiffBool        miffCreateReaderContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffN * const subFormatVersion, void * const dataRepo);
Miff           *miffCreateWriter(                                           MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN   const subFormatVersion, void * const dataRepo);
MiffBool        miffCreateWriterContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN   const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Miff       * const miff);
void            miffDestroyContent(                Miff       * const miff);

MiffBool        miffGetInfo(                       Miff       * const miff, MiffType * const type, MiffStr       * const typeName, MiffStr       * const key, MiffN * const count);
MiffBool        miffGetRecordEnd(                  Miff       * const miff);
MiffValue       miffGetValueBoolean(               Miff       * const miff);
MiffValue       miffGetValueI(                     Miff       * const miff);
MiffValue       miffGetValueN(                     Miff       * const miff);
MiffValue       miffGetValueR4(                    Miff       * const miff);
MiffValue       miffGetValueR8(                    Miff       * const miff);
MiffValue       miffGetValueStr(                   Miff       * const miff);

MiffBool        miffSetInfo(                       Miff       * const miff, MiffType   const type, MiffStr const * const typeName, MiffStr const * const key, MiffN const count);
MiffBool        miffSetSeparator(                  Miff       * const miff);
MiffBool        miffSetRecordEnd(                  Miff       * const miff);
MiffBool        miffSetValue(                      Miff       * const miff, MiffValue value);

MiffType        miffValueGetValueType(             MiffValue value);
MiffBool        miffValueGetBool(                  MiffValue value);
MiffI           miffValueGetI(                     MiffValue value);
MiffN           miffValueGetN(                     MiffValue value);
MiffR4          miffValueGetR4(                    MiffValue value);
MiffR8          miffValueGetR8(                    MiffValue value);
MiffStr const  *miffValueGetStr(                   MiffValue value);
MiffN           miffValueGetStrLen(                MiffValue value);
MiffType        MiffValueGetType(                  MiffValue value);

MiffValue       miffValueSetBoolean(               MiffBool const value);
MiffValue       miffValueSetI(                     MiffI    const value);
MiffValue       miffValueSetN(                     MiffN    const value);
MiffValue       miffValueSetR4(                    MiffR4   const value);
MiffValue       miffValueSetR8(                    MiffR8   const value);
MiffValue       miffValueSetStr(                   MiffN const strLen, MiffStr const * const str);
MiffValue       miffValueSetType(                  MiffType const value);

MiffBool        miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy);
void            miffStop(                          void);

MiffStr const  *miffTypeGetStr(                    MiffType const type);

// Convenience macroes
#define miffValueSetString(         VALUE)                       miffValueSetStr(strlen(VALUE), VALUE)

#define miffSetBlockStart(          MIFF, NAME)                  miffSetInfo(MIFF, miffTypeBLOCK_START, NULL,     NAME, miffArrayCountUNKNOWN)
#define miffSetBlockStop(           MIFF)                        miffSetInfo(MIFF, miffTypeBLOCK_STOP,  NULL,     NULL, miffArrayCountUNKNOWN)

#define miffSetBoolean(             MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeBOOL   ,     NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetBoolean(    VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetI(                   MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeI,           NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetI( (MiffI)  VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetN(                   MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeN,           NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetN( (MiffN)  VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetR4(                  MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeR4,          NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetR4((MiffR4) VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetR8(                  MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeR8,          NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetR8((MiffR8) VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetType(                MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeTYPE,        NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetType(       VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetStr(                 MIFF, NAME, VALUE)         { miffSetInfo(MIFF, miffTypeSTR,         NULL,     NAME,     1); miffSetValue(MIFF, miffValueSetString(     VALUE)); miffSetRecordEnd(MIFF); }
#define miffSetVariableStart(       MIFF, NAME)                  miffSetInfo(MIFF, miffTypeVAR,         NULL,     NAME,     1)
#define miffSetUserTypeStart(       MIFF, TYPENAME, NAME)        miffSetInfo(MIFF, miffTypeUSER,        TYPENAME, NAME,     1)

#define miffSetBooleanArrayStart(   MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeBOOL,        NULL,     NAME, COUNT)
#define miffSetIArrayStart(         MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeI,           NULL,     NAME, COUNT)
#define miffSetNArrayStart(         MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeN,           NULL,     NAME, COUNT)
#define miffSetR4ArrayStart(        MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeR4,          NULL,     NAME, COUNT)
#define miffSetR8ArrayStart(        MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeR8,          NULL,     NAME, COUNT)
#define miffSetTypeArrayStart(      MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeTYPE,        NULL,     NAME, COUNT)
#define miffSetStrArrayStart(       MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeSTR,         NULL,     NAME, COUNT)
#define miffSetVariableArrayStart(  MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeVAR,         NULL,     NAME, COUNT)
#define miffSetUserTypeArrayStart(  MIFF, NAME, COUNT)           miffSetInfo(MIFF, miffTypeUSER,        NULL,     NAME, COUNT)

#define miffSetBooleanArray(        MIFF, NAME, COUNT, ARRAY)  { miffSetBooleanArrayStart(MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetBoolean(    ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetIArray(              MIFF, NAME, COUNT, ARRAY)  { miffSetIArrayStart(      MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetI( (MiffI)  ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetNArray(              MIFF, NAME, COUNT, ARRAY)  { miffSetNArrayStart(      MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetN( (MiffN)  ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetR4Array(             MIFF, NAME, COUNT, ARRAY)  { miffSetR4ArrayStart(     MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetR4((MiffR4) ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetR8Array(             MIFF, NAME, COUNT, ARRAY)  { miffSetR8ArrayStart(     MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetR8((MiffR8) ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetTypeArray(           MIFF, NAME, COUNT, ARRAY)  { miffSetTypeArrayStart(   MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetType(       ARRAY[__index__])); } miffSetRecordEnd(MIFF); }
#define miffSetStrArray(            MIFF, NAME, COUNT, ARRAY)  { miffSetStrArrayStart(    MIFF, NAME, COUNT); for (int __index__ = 0; __index__ < COUNT; __index__++) { miffSetValue(MIFF, miffValueSetString(     ARRAY[__index__])); } miffSetRecordEnd(MIFF); }

#endif
