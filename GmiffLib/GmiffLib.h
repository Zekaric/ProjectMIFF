/**************************************************************************************************
file:       gmifflib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw MIFF files.
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

#if !defined(GMIFFLIB_H)
#define      GMIFFLIB_H

/**************************************************************************************************
include:
**************************************************************************************************/
#include "GrlMiffLib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/
typedef enum
{
   gmiffDataIS_DATA,
   gmiffDataIS_PART_DONE,
   gmiffDataIS_RECORD_DONE,

   gmiffDataERROR
} GmiffData;

typedef enum
{
   gmiffRecTypeNONE,

   gmiffRecTypeBLOCK_START,
   gmiffRecTypeBLOCK_STOP,
   gmiffRecTypeVALUE
} GmiffRecType;

typedef enum
{
   gmiffValueTypeNONE,

   gmiffValueTypeNULL,     // ~
   gmiffValueTypeSTR,      // "
   gmiffValueTypeNUM_INT,  // - 0 1 2 3 4 5 6 7 8 9 A B C D E F
   gmiffValueTypeNUM_REAL, //   G H I J K L M N O P Q R S T U V
   gmiffValueTypeBIN,      // *
   gmiffValueTypeOTHER     // Any other character.
} GmiffValueType;

#define miffArrayCountBLOCK_START      (UINT64_MAX - 2)
#define miffArrayCountBLOCK_STOP       (UINT64_MAX - 1)
#define miffArrayCountUNKNOWN           UINT64_MAX
#define miffBufferCountUNKNOWN          UINT64_MAX

/**************************************************************************************************
type:
**************************************************************************************************/
typedef union
{
   Gstr           *str;
   Gn1            *bin;
} GmiffP;

typedef union
{
   Gi4             i;
   Gn4             n;
   Gr4             r;
   Gn1             byte[4];
} Gmiff4;

typedef union
{
   Gi8             i;
   Gn8             n;
   Gr8             r;
   Gn1             byte[8];
} Gmiff8;

typedef struct
{
   GmiffValueType  type;
   Gstr            header;
   Gb              isI;
   Gb              isR4;
   Gmiff4          inr4;
   Gmiff8          inr;
   Gcount          bufferCount;
   GmiffP          bufferData;
} GmiffValue;

typedef struct
{
   // MIFF file information and configuration.
   Gn8             version;
   Gmethod         method;
   Gb              isPartDone;
   Gb              isRecordDone;

   Gstr            subFormatName[   GkeySIZE];
   Gstr            subFormatVersion[GkeySIZE];

   // The current value information.
   Gcount          currentScopeLevel;
   Gstr            currentName[GkeySIZE];
   Gcount          currentNameCount;
   Gcount          currentArrayCount;
   Gcount          currentArrayIndex;

   // Data repo getters and setters.
   void           *dataRepo;
   GgetBuffer      getBuffer;
   GsetBuffer      setBuffer;

   // Temporary storage for uncompressed memory.
   GmiffValue      value;
   Gindex          valueIndex;

   Gcount          readCount;
   Gn1             readData[GkeySIZE];
} Gmiff;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/
Gmiff          *gmiffClocReader(                                 GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gstr       * const subFormatVersion, void * const dataRepo);
Gb              gmiffClocReaderContent(Gmiff       * const miff, GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gstr       * const subFormatVersion, void * const dataRepo);
Gmiff          *gmiffClocWriter(                                 GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo);
Gb              gmiffClocWriterContent(Gmiff       * const miff, GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo);

void            gmiffDloc(             Gmiff       * const miff);
void            gmiffDlocContent(      Gmiff       * const miff);

Gb              gmiffGetRecordStart(   Gmiff       * const miff, GmiffRecType * const type, Gcount * const count, Gstr       * const key);
Gb              gmiffGetRecordEnd(     Gmiff       * const miff);
GmiffValue      gmiffGetValue(         Gmiff       * const miff);
Gb              gmiffGetValueBin(      Gmiff       * const miff, Gcount const binCount, Gn1  * const binBuffer);
GmiffData       gmiffGetValueBinData(  Gmiff       * const miff, Gn1 * const binByte);
Gb              gmiffGetValueStr(      Gmiff       * const miff, Gcount const strCount, Gstr * const str);
GmiffData       gmiffGetValueStrData(  Gmiff       * const miff, Gstr * const strLetter);

Gb              gmiffSetRecordStart(   Gmiff       * const miff, GmiffRecType   const type, Gcount   const count, Gstr const * const key);
Gb              gmiffSetRecordStop(    Gmiff       * const miff);
Gb              gmiffSetSeparator(     Gmiff       * const miff);
Gb              gmiffSetValue(         Gmiff       * const miff, GmiffValue const value);
Gb              gmiffSetValueBinData(  Gmiff       * const miff, Gn1 const binByte);
Gb              gmiffSetValueStart(    Gmiff       * const miff, GmiffValue const value);
Gb              gmiffSetValueStop(     Gmiff       * const miff);
Gb              gmiffSetValueStrData(  Gmiff       * const miff, Gstr const strLetter);

Gb              gmiffStart(            GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void            gmiffStop(             void);

/**************************************************************************************************
miffValue
prototype:
**************************************************************************************************/
Gb              gmiffValueGetB(        GmiffValue const value);
Gcount          gmiffValueGetBinCount( GmiffValue const value);
Gi8             gmiffValueGetI(        GmiffValue const value);
Gn8             gmiffValueGetN(        GmiffValue const value);
Gr8             gmiffValueGetR(        GmiffValue const value);
Gr4             gmiffValueGetR4(       GmiffValue const value);
Gcount          gmiffValueGetStrCount( GmiffValue const value);
GmiffValueType  gmiffValueGetType(     GmiffValue const value);

Gb              gmiffValueIs4(         GmiffValue const value);

GmiffValue      gmiffValueSetB(        Gb  const bvalue);
GmiffValue      gmiffValueSetBinBuffer(Gcount const binCount, Gn1  * const binBuffer);
GmiffValue      gmiffValueSetBinCount( Gcount const binCount);
GmiffValue      gmiffValueSetI(        Gi8 const ivalue);
GmiffValue      gmiffValueSetN(        Gn8 const nvalue);
GmiffValue      gmiffValueSetNull(     void);
GmiffValue      gmiffValueSetR(        Gr8 const rvalue);
GmiffValue      gmiffValueSetR4(       Gr4 const rvalue);
GmiffValue      gmiffValueSetStrBuffer(Gcount const strCount, Gstr * const strBuffer);
GmiffValue      gmiffValueSetStrCount( Gcount const strCount);

// Convenience macroes
#define gmiffValueSetStr(                  VALUE)                              gmiffValueSetStrBuffer((Gcount) strlen(VALUE), VALUE)

#define gmiffSetRecordBlockStart(          MIFF, NAME)                       ( gmiffSetRecordStart(MIFF, gmiffRecTypeBLOCK_START, 0, NAME), gmiffSetRecordStop(MIFF) )
#define gmiffSetRecordBlockStop(           MIFF)                             ( gmiffSetRecordStart(MIFF, gmiffRecTypeBLOCK_STOP,  0, NULL), gmiffSetRecordStop(MIFF) )

#define gmiffSetRecordValueNull(           MIFF)                             { GmiffValue __value__; __value__ = gmiffValueSetNull(                     ); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueB(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetB(                 (VALUE)); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueI(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetI(                 (VALUE)); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueN(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetN(                 (VALUE)); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueR(              MIFF,        VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetR(                 (VALUE)); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueR4(             MIFF,        VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetR4(                (VALUE)); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueStr(            MIFF,        VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetStr(               (VALUE)); gmiffSetValue(MIFF, __value__); }
#define gmiffSetRecordValueBinBuffer(      MIFF, COUNT, VALUE)               { GmiffValue __value__; __value__ = gmiffValueSetBinBuffer((COUNT), (VALUE)); gmiffSetValue(MIFF, __value__); }

#define gmiffSetRecordNull(                MIFF,       NAME)                 { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueNull(     MIFF                  ); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordBool(                MIFF,       NAME,        VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueB(        MIFF,          (VALUE)); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordI(                   MIFF,       NAME,        VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueI(        MIFF,          (VALUE)); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordN(                   MIFF,       NAME,        VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueN(        MIFF,          (VALUE)); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordR(                   MIFF,       NAME,        VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueR(        MIFF,          (VALUE)); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordR4(                  MIFF,       NAME,        VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueR4(       MIFF,          (VALUE)); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordStr(                 MIFF,       NAME,        VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueStr(      MIFF,          (VALUE)); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordBinBuffer(           MIFF,       NAME, COUNT, VALUE)   { gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME); { gmiffSetRecordValueBinBuffer(MIFF, (COUNT), (VALUE)); } gmiffSetRecordStop(MIFF); }

#define gmiffSetRecordStartBinArray(       MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartBoolArray(      MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartIArray(         MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartNArray(         MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartRArray(         MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartR4Array(        MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartStrArray(       MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartValueArray(     MIFF,       NAME, COUNT)            gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, COUNT, NAME)
#define gmiffSetRecordStartValue(          MIFF,       NAME)                   gmiffSetRecordStart(MIFF, gmiffRecTypeVALUE, 1,     NAME);

#define gmiffSetRecordBoolArray(           MIFF, NAME, COUNT, ARRAY)         { gmiffSetRecordStartBoolArray(MIFF, NAME, COUNT); int __index__; forCount(__index__, (COUNT)) { gmiffSetRecordValueB(  MIFF, (ARRAY[__index__])); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordIArray(              MIFF, NAME, COUNT, ARRAY)         { gmiffSetRecordStartIArray(   MIFF, NAME, COUNT); int __index__; forCount(__index__, (COUNT)) { gmiffSetRecordValueI(  MIFF, (ARRAY[__index__])); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordNArray(              MIFF, NAME, COUNT, ARRAY)         { gmiffSetRecordStartNArray(   MIFF, NAME, COUNT); int __index__; forCount(__index__, (COUNT)) { gmiffSetRecordValueN(  MIFF, (ARRAY[__index__])); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordRArray(              MIFF, NAME, COUNT, ARRAY)         { gmiffSetRecordStartRArray(   MIFF, NAME, COUNT); int __index__; forCount(__index__, (COUNT)) { gmiffSetRecordValueR(  MIFF, (ARRAY[__index__])); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordR4Array(             MIFF, NAME, COUNT, ARRAY)         { gmiffSetRecordStartR4Array(  MIFF, NAME, COUNT); int __index__; forCount(__index__, (COUNT)) { gmiffSetRecordValueR4( MIFF, (ARRAY[__index__])); } gmiffSetRecordStop(MIFF); }
#define gmiffSetRecordStrArray(            MIFF, NAME, COUNT, ARRAY)         { gmiffSetRecordStartStrArray( MIFF, NAME, COUNT); int __index__; forCount(__index__, (COUNT)) { gmiffSetRecordValueStr(MIFF, (ARRAY[__index__])); } gmiffSetRecordStop(MIFF); }

#endif
