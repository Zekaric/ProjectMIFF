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
   Gb              isR8;
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
Gmiff          *gmiffClocReader(                                       GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gstr       * const subFormatVersion, void * const dataRepo);
Gb              gmiffClocReaderContent(      Gmiff       * const miff, GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gstr       * const subFormatVersion, void * const dataRepo);
Gmiff          *gmiffClocWriter(                                       GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo);
Gb              gmiffClocWriterContent(      Gmiff       * const miff, GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo);

void            gmiffDloc(                   Gmiff       * const miff);
void            gmiffDlocContent(            Gmiff       * const miff);

Gb              gmiffGetRecordStart(         Gmiff       * const miff, GmiffRecType * const type, Gcount * const count, Gstr       * const key);
Gb              gmiffGetRecordEnd(           Gmiff       * const miff);
GmiffValue      gmiffGetValue(               Gmiff       * const miff);
Gb              gmiffGetValueBin(            Gmiff       * const miff, Gcount const binCount, Gn1  * const binBuffer);
GmiffData       gmiffGetValueBinData(        Gmiff       * const miff, Gn1 * const binByte);
Gb              gmiffGetValueStr(            Gmiff       * const miff, Gcount const strCount, Gstr * const str);
GmiffData       gmiffGetValueStrData(        Gmiff       * const miff, Gstr * const strLetter);

Gb              gmiffSetRecordB(             Gmiff       * const miff, Gstr const * const name, Gb const value);
Gb              gmiffSetRecordBArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gb const * const array);
Gb              gmiffSetRecordBinBuffer(     Gmiff       * const miff, Gstr const * const name, Gcount const count, Gn1 * const value);
Gb              gmiffSetRecordBinBufferStart(Gmiff       * const miff, Gstr const * const name, Gcount const count);
Gb              gmiffSetRecordBinBufferStop( Gmiff       * const miff);
Gb              gmiffSetRecordBlockStart(    Gmiff       * const miff, Gstr const * const name);
Gb              gmiffSetRecordBlockStop(     Gmiff       * const miff);
Gb              gmiffSetRecordI(             Gmiff       * const miff, Gstr const * const name, Gi8 const value);
Gb              gmiffSetRecordIArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gi8 const * const array);
Gb              gmiffSetRecordN(             Gmiff       * const miff, Gstr const * const name, Gn8 const value);
Gb              gmiffSetRecordNArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gn8 const * const array);
Gb              gmiffSetRecordNull(          Gmiff       * const miff, Gstr const * const name);
Gb              gmiffSetRecordR(             Gmiff       * const miff, Gstr const * const name, Gr8 const value);
Gb              gmiffSetRecordRArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gr8 const * const array);
Gb              gmiffSetRecordR4(            Gmiff       * const miff, Gstr const * const name, Gr4 const value);
Gb              gmiffSetRecordR4Array(       Gmiff       * const miff, Gstr const * const name, Gcount const count, Gr4 const * const array);
Gb              gmiffSetRecordStr(           Gmiff       * const miff, Gstr const * const name, Gstr * const value);
Gb              gmiffSetRecordStrArray(      Gmiff       * const miff, Gstr const * const name, Gcount const count, Gstr ** const array);
Gb              gmiffSetRecordStrStart(      Gmiff       * const miff, Gstr const * const name, Gcount const count);
Gb              gmiffSetRecordStrStop(       Gmiff       * const miff);
Gb              gmiffSetRecordStart(         Gmiff       * const miff, GmiffRecType   const type, Gcount   const count, Gstr const * const key);
Gb              gmiffSetRecordStop(          Gmiff       * const miff);
Gb              gmiffSetSeparator(           Gmiff       * const miff);
Gb              gmiffSetValue(               Gmiff       * const miff, GmiffValue const value);
Gb              gmiffSetValue_BinData(       Gmiff       * const miff, Gn1 const binByte);
Gb              gmiffSetValue_StrData(       Gmiff       * const miff, Gstr const strLetter);
Gb              gmiffSetValueStart(          Gmiff       * const miff, GmiffValue const value);
Gb              gmiffSetValueStop(           Gmiff       * const miff);

Gb              gmiffStart(                  GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void            gmiffStop(                   void);

/**************************************************************************************************
miffValue
prototype:
**************************************************************************************************/
Gb              gmiffValueGetB(              GmiffValue const value);
Gcount          gmiffValueGetBinCount(       GmiffValue const value);
Gi8             gmiffValueGetI(              GmiffValue const value);
Gn8             gmiffValueGetN(              GmiffValue const value);
Gr8             gmiffValueGetR(              GmiffValue const value);
Gr4             gmiffValueGetR4(             GmiffValue const value);
Gcount          gmiffValueGetStrCount(       GmiffValue const value);
GmiffValueType  gmiffValueGetType(           GmiffValue const value);

Gb              gmiffValueIsR4(              GmiffValue const value);
Gb              gmiffValueIsR8(              GmiffValue const value);

GmiffValue      gmiffValueSetB(              Gb  const bvalue);
GmiffValue      gmiffValueSetBinBuffer(      Gcount const binCount, Gn1 const * const binBuffer);
GmiffValue      gmiffValueSetBinCount(       Gcount const binCount);
GmiffValue      gmiffValueSetI(              Gi8 const ivalue);
GmiffValue      gmiffValueSetN(              Gn8 const nvalue);
GmiffValue      gmiffValueSetNull(           void);
GmiffValue      gmiffValueSetR(              Gr8 const rvalue);
GmiffValue      gmiffValueSetR4(             Gr4 const rvalue);
GmiffValue      gmiffValueSetStrBuffer(      Gcount const strCount, Gstr const * const strBuffer);
GmiffValue      gmiffValueSetStrCount(       Gcount const strCount);

// Convenience utils and macroes
#define gmiffValueSetStr(             VALUE)                   gmiffValueSetStrBuffer((Gcount) strlen(VALUE), (VALUE))

#define gmiffSetValue_Null(           MIFF)                    gmiffSetValue(     (MIFF), gmiffValueSetNull(                     ))
#define gmiffSetValue_B(              MIFF,        VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetB(                 (VALUE)))
#define gmiffSetValue_I(              MIFF,        VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetI(                 (VALUE)))
#define gmiffSetValue_N(              MIFF,        VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetN(                 (VALUE)))
#define gmiffSetValue_R(              MIFF,        VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetR(                 (VALUE)))
#define gmiffSetValue_R4(             MIFF,        VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetR4(                (VALUE)))
#define gmiffSetValue_Str(            MIFF,        VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetStr(               (VALUE)))
#define gmiffSetValue_StrStart(       MIFF, COUNT)             gmiffSetValueStart((MIFF), gmiffValueSetStrCount( (COUNT)         ))
#define gmiffSetValue_StrStop(        MIFF)                    gmiffSetValueStop( (MIFF))
#define gmiffSetValue_BinBuffer(      MIFF, COUNT, VALUE)      gmiffSetValue(     (MIFF), gmiffValueSetBinBuffer((COUNT), (VALUE)))
#define gmiffSetValue_BinBufferStart( MIFF, COUNT)             gmiffSetValueStart((MIFF), gmiffValueSetBinCount( (COUNT)         ))
#define gmiffSetValue_BinBufferStop(  MIFF)                    gmiffSetValueStop( (MIFF))

#define gmiffSetRecordArrayStart(     MIFF, NAME, COUNT)       gmiffSetRecordStart((MIFF), gmiffRecTypeVALUE,    (COUNT), (NAME))
#define gmiffSetRecordArrayStop(      MIFF)                    gmiffSetRecordStop( (MIFF))
#define gmiffSetRecordValueStart(     MIFF, NAME)              gmiffSetRecordStart((MIFF), gmiffRecTypeVALUE,    1,       (NAME))
#define gmiffSetRecordValueStop(      MIFF)                    gmiffSetRecordStop( (MIFF))
#define gmiffSetRecordValueArrayStart(MIFF, NAME, COUNT)       gmiffSetRecordStart((MIFF), gmiffRecTypeVALUE,    (COUNT), (NAME))
#define gmiffSetRecordValueArrayStop( MIFF)                    gmiffSetRecordStop( (MIFF))

#endif
