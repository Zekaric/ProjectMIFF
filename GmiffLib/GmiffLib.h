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
   gmiffValueTypeNONE,

   gmiffValueTypeARRAY_COUNT, // [###
   gmiffValueTypeBLOCK_START, // {
   gmiffValueTypeBLOCK_STOP,  // }
   gmiffValueTypeGROUP_COUNT, // (###

   gmiffValueTypeNULL,        // ~
   gmiffValueTypeSTR,         // "###
   gmiffValueTypeNUM,         // -0123456789e.
   gmiffValueTypeBIN,         // *###

   gmiffValueTypeOTHER        // Any other character.
} GmiffValueType;

#define miffArrayCountZERO        -2
#define miffCountUNKNOWN          -1

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
   Gb              isN;
   Gb              isR8;
   Gb              isR4;
   Gmiff4          inr4;
   Gmiff8          inr;
   Gcount          count;
   GmiffP          data;
} GmiffValue;

typedef struct
{
   // MIFF file information and configuration.
   Gn8             version;
   Gmethod         method;
   Gb              isPartDone;
   Gb              isRecordDone;

   Gstr            subFormatName[GkeySIZE];
   Gi8             subFormatVersion;

   // The current value information.
   Gcount          scopeLevel;

   // Data repo getters and setters.
   void           *dataRepo;
   GgetBuffer      getBuffer;
   GsetBuffer      setBuffer;

   // Temporary storage for uncompressed memory.
   GmiffValue      value;
   Gcount          valueCount;
   Gindex          bufferIndex;
   Gn1             buffer[4097];

   Gcount          readCount;
   Gn1             readData[GkeySIZE * 2];
} Gmiff;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/
Gmiff          *gmiffClocReader(                                       GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gn8 * const subFormatVersion, void * const dataRepo);
Gb              gmiffClocReaderContent(      Gmiff       * const miff, GgetBuffer getBufferFunc, Gstr       * const subFormatName, Gn8 * const subFormatVersion, void * const dataRepo);
Gmiff          *gmiffClocWriter(                                       GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gn8   const subFormatVersion, void * const dataRepo);
Gb              gmiffClocWriterContent(      Gmiff       * const miff, GsetBuffer setBufferFunc, Gstr const * const subFormatName, Gn8   const subFormatVersion, void * const dataRepo);

void            gmiffDloc(                   Gmiff       * const miff);
void            gmiffDlocContent(            Gmiff       * const miff);

Gb              gmiffGetRecordStart(         Gmiff       * const miff, Gstr       * const key);
Gb              gmiffGetRecordStop(          Gmiff       * const miff);
GmiffValue      gmiffGetValue(         Gmiff       * const miff);
Gb              gmiffGetValueBin(            Gmiff       * const miff, Gcount const binCount, Gn1  * const binBuffer);
GmiffData       gmiffGetValueBinData(        Gmiff       * const miff, Gn1 * const binByte);
Gb              gmiffGetValueStr(            Gmiff       * const miff, Gcount const strCount, Gstr * const str);
GmiffData       gmiffGetValueStrData(        Gmiff       * const miff, Gstr * const strLetter);

Gb              gmiffSetRecordStop(          Gmiff       * const miff);
Gb              gmiffSetValue(               Gmiff       * const miff, GmiffValue const value);
Gb              gmiffSetValueArrayCount(     Gmiff       * const miff, Gcount const count);
Gb              gmiffSetValueB(              Gmiff       * const miff, Gb const value);
Gb              gmiffSetValueBin(            Gmiff       * const miff, Gcount const binCount, Gn1 const * const binBuffer);
Gb              gmiffSetValueBinData(        Gmiff       * const miff, Gn1 const binByte);
Gb              gmiffSetValueBinStart(       Gmiff       * const miff, Gcount const binCount);
Gb              gmiffSetValueBinStop(        Gmiff       * const miff);
Gb              gmiffSetValueBlockStart(     Gmiff       * const miff);
Gb              gmiffSetValueBlockStop(      Gmiff       * const miff);
Gb              gmiffSetValueGroupCount(     Gmiff       * const miff, Gcount const count);
Gb              gmiffSetValueI(              Gmiff       * const miff, Gi8 const value);
Gb              gmiffSetValueN(              Gmiff       * const miff, Gn8 const value);
Gb              gmiffSetValueNull(           Gmiff       * const miff);
Gb              gmiffSetValueR(              Gmiff       * const miff, Gr8 const value);
Gb              gmiffSetValueR4(             Gmiff       * const miff, Gr4 const value);
Gb              gmiffSetValueStr(            Gmiff       * const miff, Gcount const strCount, Gstr const * const strBuffer);
Gb              gmiffSetValueStrData(        Gmiff       * const miff, Gstr const strByte);
Gb              gmiffSetValueStrStart(       Gmiff       * const miff, Gcount const strCount);
Gb              gmiffSetValueStrStop(        Gmiff       * const miff);

Gb              gmiffStart(                  GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void            gmiffStop(                   void);

#define gmiffSetValueStrFinite(MIFF, STR) gmiffSetValueStr(MIFF, gstrGetCount(STR), STR)

/**************************************************************************************************
miffRecord
Convenience functions for setting a complete MIFF record in one call.
**************************************************************************************************/
Gb              gmiffRecordSetB(             Gmiff       * const miff, Gstr const * const name, Gb const value);
Gb              gmiffRecordSetBArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gb const * const array);
Gb              gmiffRecordSetBin(           Gmiff       * const miff, Gstr const * const name, Gcount const count, Gn1 * const value);
Gb              gmiffRecordSetBlockStart(    Gmiff       * const miff, Gstr const * const key);
Gb              gmiffRecordSetBlockStop(     Gmiff       * const miff);
Gb              gmiffRecordSetI(             Gmiff       * const miff, Gstr const * const name, Gi8 const value);
Gb              gmiffRecordSetIArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gi8 const * const array);
Gb              gmiffRecordSetN(             Gmiff       * const miff, Gstr const * const name, Gn8 const value);
Gb              gmiffRecordSetNArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gn8 const * const array);
Gb              gmiffRecordSetNull(          Gmiff       * const miff, Gstr const * const name);
Gb              gmiffRecordSetR(             Gmiff       * const miff, Gstr const * const name, Gr8 const value);
Gb              gmiffRecordSetRArray(        Gmiff       * const miff, Gstr const * const name, Gcount const count, Gr8 const * const array);
Gb              gmiffRecordSetR4(            Gmiff       * const miff, Gstr const * const name, Gr4 const value);
Gb              gmiffRecordSetR4Array(       Gmiff       * const miff, Gstr const * const name, Gcount const count, Gr4 const * const array);
Gb              gmiffRecordSetStr(           Gmiff       * const miff, Gstr const * const name, Gstr const * const value);

/**************************************************************************************************
miffValue
**************************************************************************************************/
Gb              gmiffValueGetB(              GmiffValue const value);
Gcount          gmiffValueGetBinCount(       GmiffValue const value);
Gi8             gmiffValueGetI(              GmiffValue const value);
Gn8             gmiffValueGetN(              GmiffValue const value);
Gr8             gmiffValueGetR(              GmiffValue const value);
Gr4             gmiffValueGetR4(             GmiffValue const value);
Gcount          gmiffValueGetStrCount(       GmiffValue const value);
GmiffValueType  gmiffValueGetType(           GmiffValue const value);

Gb              gmiffValueIsI(               GmiffValue const value);
Gb              gmiffValueIsN(               GmiffValue const value);
Gb              gmiffValueIsR4(              GmiffValue const value);
Gb              gmiffValueIsR8(              GmiffValue const value);

GmiffValue      gmiffValueSetArrayCount(     Gcount const count);
GmiffValue      gmiffValueSetB(              Gb  const bvalue);
GmiffValue      gmiffValueSetBin(            Gcount const binCount, Gn1 const * const binBuffer);
GmiffValue      gmiffValueSetBlockStart(     void);
GmiffValue      gmiffValueSetBlockStop(      void);
GmiffValue      gmiffValueSetGroupCount(     Gcount const count);
GmiffValue      gmiffValueSetI(              Gi8 const ivalue);
GmiffValue      gmiffValueSetN(              Gn8 const nvalue);
GmiffValue      gmiffValueSetNull(           void);
GmiffValue      gmiffValueSetR(              Gr8 const rvalue);
GmiffValue      gmiffValueSetR4(             Gr4 const rvalue);
GmiffValue      gmiffValueSetStr(            Gcount const strCount, Gstr const * const strBuffer);

#define gmiffValueSetStrFinite(STR)    gmiffValueSetStr(gstrGetCount(STR), STR)

#endif
