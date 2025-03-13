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

/******************************************************************************
local:
constant:
******************************************************************************/
typedef enum
{
   miffDataIS_DATA,
   miffDataIS_PART_DONE,
   miffDataIS_RECORD_DONE,

   miffDataERROR
} GmiffData;

typedef enum
{
   miffRecTypeNONE,

   miffRecTypeBLOCK_START,
   miffRecTypeBLOCK_STOP,
   miffRecTypeVALUE
} GmiffRecType;

typedef enum
{
   miffValueTypeNONE,

   miffValueTypeNULL,     // ~
   miffValueTypeSTR,      // "
   miffValueTypeNUM_INT,  // - 0 1 2 3 4 5 6 7 8 9 A B C D E F
   miffValueTypeNUM_REAL, //   G H I J K L M N O P Q R S T U V
   miffValueTypeBIN,      // *
   miffValueTypeOTHER     // Any other character.
} GmiffValueType;

#define miffArrayCountBLOCK_START      (UINT64_MAX - 2)
#define miffArrayCountBLOCK_STOP       (UINT64_MAX - 1)
#define miffArrayCountUNKNOWN           UINT64_MAX
#define miffBufferCountUNKNOWN          UINT64_MAX

/******************************************************************************
type:
******************************************************************************/
typedef union
{
   Gstr            *str;
   Gn1             *bin;
} GmiffP;

typedef union
{
   Gi4              i;
   Gn4              n;
   Gr4              r;
   Gn1              byte[4];
} Gmiff4;

typedef union
{
   Gi8               i;
   Gn8               n;
   Gr8               r;
   Gn1              byte[8];
} Gmiff8;

typedef struct
{
   GmiffValueType      type;
   Gstr             header;
   Gb               isI;
   Gb               isR4;
   Gmiff4              inr4;
   Gmiff8              inr;
   Gn8               bufferCount;
   GmiffP              bufferData;
} GmiffValue;

typedef struct
{
   // MIFF file information and configuration.
   Gn8                        version;
   Gmethod                  method;
   Gb                        isByteSwapping;
   Gb                        isPartDone;
   Gb                        isRecordDone;

   Gstr                      subFormatName[   GkeySIZE];
   Gstr                      subFormatVersion[GkeySIZE];

   // The current value information.
   Gn8                        currentScopeLevel;
   Gstr                      currentName[GkeySIZE];
   Gn8                        currentNameCount;
   Gn8                        currentArrayCount;
   Gn8                        currentArrayIndex;

   // Data repo getters and setters.
   void                       *dataRepo;
   GgetBuffer                getBuffer;
   GsetBuffer                setBuffer;

   // Temporary storage for uncompressed memory.
   GmiffValue                   value;
   Gn8                       valueIndex;

   Gn4                      readCount;
   Gn1                      readData[GkeySIZE];
} Gmiff;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Gmiff           *miffCreateReader(                                           Gb const isByteSwaping, GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gstr       * const subFormatVersion, void * const dataRepo);
Gb            miffCreateReaderContent(           Gmiff       * const miff, Gb const isByteSwaping, GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gstr       * const subFormatVersion, void * const dataRepo);
Gmiff           *miffCreateWriter(                                           Gb const isByteSwaping, GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo);
Gb            miffCreateWriterContent(           Gmiff       * const miff, Gb const isByteSwaping, GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Gmiff       * const miff);
void            miffDestroyContent(                Gmiff       * const miff);

Gb            miffGetRecordStart(                Gmiff       * const miff, GmiffRecType * const type, Gn8 * const count, Gstr       * const key);
Gb            miffGetRecordEnd(                  Gmiff       * const miff);
GmiffValue       miffGetValue(                      Gmiff       * const miff);
Gb            miffGetValueBin(                   Gmiff       * const miff, Gn8 const binCount, Gn1  * const binBuffer);
GmiffData        miffGetValueBinData(               Gmiff       * const miff, Gn1 * const binByte);
Gb            miffGetValueStr(                   Gmiff       * const miff, Gn8 const strCount, Gstr * const str);
GmiffData        miffGetValueStrData(               Gmiff       * const miff, Gstr * const strLetter);

Gb            miffSetRecordStart(                Gmiff       * const miff, GmiffRecType   const type, Gn8   const count, Gstr const * const key);
Gb            miffSetRecordStop(                 Gmiff       * const miff);
Gb            miffSetSeparator(                  Gmiff       * const miff);
Gb            miffSetValue(                      Gmiff       * const miff, GmiffValue const value);
Gb            miffSetValueBinData(               Gmiff       * const miff, Gn1 const binByte);
Gb            miffSetValueStart(                 Gmiff       * const miff, GmiffValue const value);
Gb            miffSetValueStop(                  Gmiff       * const miff);
Gb            miffSetValueStrData(               Gmiff       * const miff, Gstr const strLetter);

Gb            miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy);
void            miffStop(                          void);

/******************************************************************************
miffValue
prototype:
******************************************************************************/
Gb            miffValueGetB(                     GmiffValue const value);
Gn8            miffValueGetBinCount(              GmiffValue const value);
Gi8            miffValueGetI(                     GmiffValue const value);
Gn8            miffValueGetN(                     GmiffValue const value);
Gr8            miffValueGetR(                     GmiffValue const value);
Gr4           miffValueGetR4(                    GmiffValue const value);
Gn8            miffValueGetStrCount(              GmiffValue const value);
GmiffValueType   miffValueGetType(                  GmiffValue const value);

Gb            miffValueIs4(                      GmiffValue const value);

GmiffValue       miffValueSetB(                     Gb  const bvalue);
GmiffValue       miffValueSetBinBuffer(             Gn8  const binCount, Gn1  * const binBuffer);
GmiffValue       miffValueSetBinCount(              Gn8  const binCount);
GmiffValue       miffValueSetI(                     Gi8  const ivalue);
GmiffValue       miffValueSetN(                     Gn8  const nvalue);
GmiffValue       miffValueSetNull(                  void);
GmiffValue       miffValueSetR(                     Gr8  const rvalue);
GmiffValue       miffValueSetR4(                    Gr4 const rvalue);
GmiffValue       miffValueSetStrBuffer(             Gn8  const strCount, Gstr * const strBuffer);
GmiffValue       miffValueSetStrCount(              Gn8  const strCount);

// Convenience macroes
#define miffValueSetStr(                  VALUE)                              miffValueSetStrBuffer(strlen(VALUE), VALUE)

#define miffSetRecordBlockStart(          MIFF, NAME)                       { miffSetRecordStart(MIFF, miffRecTypeBLOCK_START, 0, NAME); miffSetRecordStop(MIFF); }
#define miffSetRecordBlockStop(           MIFF)                             { miffSetRecordStart(MIFF, miffRecTypeBLOCK_STOP,  0, NULL); miffSetRecordStop(MIFF); }

#define miffSetRecordValueNull(           MIFF)                             { GmiffValue __value__; __value__ = miffValueSetNull(                     ); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueB(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = miffValueSetB(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueI(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = miffValueSetI(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueN(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = miffValueSetN(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueR(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = miffValueSetR(                 (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueR4(             MIFF,        VALUE)               { GmiffValue __value__; __value__ = miffValueSetR4(                (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueStr(            MIFF,        VALUE)               { GmiffValue __value__; __value__ = miffValueSetStr(               (VALUE)); miffSetValue(MIFF, __value__); }
#define miffSetRecordValueBinBuffer(      MIFF, COUNT, VALUE)               { GmiffValue __value__; __value__ = miffValueSetBinBuffer((COUNT), (VALUE)); miffSetValue(MIFF, __value__); }

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
