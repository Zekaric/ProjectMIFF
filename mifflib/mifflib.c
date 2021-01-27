/******************************************************************************
file:       MiffLib
author:     Robbert de Groot
company:    
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw MIFF files.
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

/******************************************************************************
include:
******************************************************************************/
#include <memory.h>
#include <string.h>

#include "miffLib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
#define forCount(INDEX, COUNT)                  for ((INDEX) = 0; (INDEX) < (COUNT); (INDEX)++)

#define returnFalse                             return miffBoolFALSE
#define returnNull                              return NULL
#define returnTrue                              return miffBoolTRUE
#define returnFalseIf(EXP)                      if (EXP) { returnFalse; }
#define returnNullIf( EXP)                      if (EXP) { returnNull;  }
#define returnTrueIf( EXP)                      if (EXP) { returnTrue;  }
#define returnVoidIf( EXP)                      if (EXP) { return;      }

#define memClearType(TYPE, MEM)                 memset((MEM), 0, sizeof(TYPE))
#define memCopyTypeArray(COUNT, TYPE, DST, SRC) memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define memCreateType(TYPE)                     (TYPE *)  _memCreate(          sizeof(TYPE))
#define memCreateTypeArray(COUNT, TYPE)         (TYPE **) _memCreate((COUNT) * sizeof(TYPE))
#define memDestroy(MEM)                                   _memDestroy(MEM)

#define _strGetLength(STR)                      wcslen(STR)

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static MiffBool          _isStarted     = miffBoolFALSE;
static MiffMemCreate     _memCreate     = NULL;
static MiffMemDestroy    _memDestroy    = NULL;
static MiffMemCompress   _memCompress   = NULL;
static MiffMemDecompress _memDecompress = NULL;

/******************************************************************************
prototype:
******************************************************************************/
static MiffC   *_strClone(    MiffN4 const count, MiffC const * const str);
static MiffC   *_strFromUTF8( MiffN4 const count, MiffN1 const * const utf8);
static MiffN1  *_UTF8FromStr( MiffN4 const count, MiffC const * const str);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer1 const getBuffer1Func, 
   MiffGetBuffer2 const getBuffer2Func, MiffGetBuffer4 const getBuffer4Func, 
   MiffGetBuffer8 const getBuffer8Func);
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure.
   miff = memCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the miff structure.
   if (!miffCreateReaderContent(
         miff, 
         isByteSwaping, 
         getBuffer1Func, 
         getBuffer2Func, 
         getBuffer4Func, 
         getBuffer8Func)
   {
      memDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateReaderContent
******************************************************************************/
MiffBool miffCreateReaderContent(Miff * const miff, MiffBool const isByteSwaping, 
   MiffGetBuffer1 const getBuffer1Func, MiffGetBuffer2 const getBuffer2Func, 
   MiffGetBuffer4 const getBuffer4Func, MiffGetBuffer8 const getBuffer8Func)
{
   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !getBuffer1Func ||
      !getBuffer2Func ||
      !getBuffer4Func ||
      !getBuffer8Func);

   memClearType(Miff, miff);
   miff->method         = miffMethodREADING;
   miff->isByteSwapping = isByteSwaping;
   miff->getBuffer1     = getBuffer1Func;
   miff->getBuffer2     = getBuffer2Func;
   miff->getBuffer4     = getBuffer4Func;
   miff->getBuffer8     = getBuffer8Func;
   miff->key            = memCreateTypeArray(255, Mn1);

   returnFalseIf(!miff->key);

   _readKeyText(miff);
   if (_strIsEqual())

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer1 const setBuffer1Func, 
   MiffSetBuffer2 const setBuffer2Func, MiffSetBuffer4 const setBuffer4Func, 
   MiffSetBuffer8 const setBuffer8Func, MiffBool const isBinary, MiffN4 const subFormatNameLen,
   MiffC const * const subFormatName, MiffN4 const subFormatVersion)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure
   miff = memCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the structure
   if (!miffCreateWriterContent(
         miff, 
         isByteSwaping, 
         setBuffer1Func, 
         setBuffer2Func, 
         setBuffer4Func, 
         setBuffer8Func, 
         isBinary, 
         subFormatNameLen,
         subFormatName, 
         subFormatVersion))
   {
      memDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateWriterContent
******************************************************************************/
MiffBool miffCreateWriterContent(Miff * const miff, MiffBool const isByteSwaping, 
   MiffSetBuffer1 const setBuffer1Func, MiffSetBuffer2 const setBuffer2Func, 
   MiffSetBuffer4 const setBuffer4Func, MiffSetBuffer8 const setBuffer8Func, 
   MiffBool const isBinary, MiffN4 const subFormatNameLen, MiffC const * const subFormatName, 
   MiffN4 const subFormatVersion)
{
   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !setBuffer1Func ||
      !setBuffer2Func ||
      !setBuffer4Func ||
      !setBuffer8Func ||
      !subFormatName  ||
      !subFormatVersion);

   memClearType(Miff, miff);
   miff->method            = miffMethodWRITING;
   miff->isBinary          = isBinary;
   miff->isByteSwapping    = isByteSwaping;
   miff->setBuffer1        = setBuffer1Func;
   miff->setBuffer2        = setBuffer2Func;
   miff->setBuffer4        = setBuffer4Func;
   miff->setBuffer8        = setBuffer8Func;
   miff->subFormatName     = _strClone(subFormatNameLen, subFormatName);
   miff->subFormatVersion  = subFormatVersion;
   miff->key               = memCreateTypeArray(255, Mn1);

   returnFalseIf(!miff->key);

   // Write the miff header.
   if (isBinary)
   {
      returnFalseIf(!_writeKeyText(miff, L"MIFF_BIN"));
   }
   else
   {
      returnFalseIf(!_writeKeyText(miff, L"MIFF_TXT"));
   }
   returnFalseIf(
      !_writeValueHeaderText(
         miff, 
         miffValueTypeN8, 
         miffArrayFlagIS_SINGLE, 
         miffCompressionFlagIS_UNCOMPRESSED,
         0));
   returnFalseIf(!_writeValueN8Text(miff, 1));

   returnFalseIf(!_writeKeyText(miff, miff->subFormatName));
   returnFalseIf(
      !_writeValueHeaderText(
         miff, 
         miffValueTypeN8, 
         miffArrayFlagIS_SINGLE, 
         miffCompressionFlagIS_UNCOMPRESSED, 
         0));
   returnFalseIf(!_writeValueN8Text(miff, miff->subFormatVersion));

   return miff;
}

/******************************************************************************
func: miffDestroy
******************************************************************************/
void *miffDestroy(Miff * const miff)
{
   returnVoidIf(
      !_isStarted ||
      !miff);

   miffDestroyContent(miff);
}

/******************************************************************************
func: miffDestroyContent
******************************************************************************/
void miffDestroyContent(Miff * const miff)
{
   returnVoifIf(
      !_isStarted ||
      !miff);

   memDestroy(miff->compressedMem);
   memDestroy(miff->mem);
   memDestroy(miff->subFormatName);
   
   memClearType(Miff, miff);
   memDestroy(miff);
}

MiffBool  miffGetVersion(           Miff * const miff, MiffN4 * const version, MiffBool * const isBinary);
MiffBool  miffGetVersionSubFormat(  Miff * const miff, MiffC * const subFormatName, MiffN4 * const subFormatVersion);

MiffBool  miffGetKey(               Miff * const miff, MiffC * const key);
MiffBool  miffGetValueHeader(       Miff * const miff, MiffValueType * const type, MiffValueArrayFlag * const arrayFlag);
MiffN4    miffGetArrayCount(        Miff * const miff);
Miff1     miffGetValue1(            Miff * const miff);
Miff2     miffGetValue2(            Miff * const miff);
Miff3     miffGetValue3(            Miff * const miff);
Miff4     miffGetValue4(            Miff * const miff);
Miff8     miffGetValue8(            Miff * const miff);
Miff16    miffGetValue16(           Miff * const miff);
Miff32    miffGetValue32(           Miff * const miff);
Miff64    miffGetValue64(           Miff * const miff);
Miff128   miffGetValue128(          Miff * const miff);
Miff256   miffGetValue256(          Miff * const miff);

MiffBool  miffSetKey(               Miff * const miff, MiffC const * const key, MiffValueType const type, MiffValueArrayFlag const arrayFlag, MiffValueCompressionFlag const compressionFlag, MiffN4 const chunkByteCount);
MiffBool  miffSetValue1(            Miff * const miff, Miff1   const value);
MiffBool  miffSetValue2(            Miff * const miff, Miff2   const value);
MiffBool  miffSetValue3(            Miff * const miff, Miff3   const value);
MiffBool  miffSetValue4(            Miff * const miff, Miff4   const value);
MiffBool  miffSetValue8(            Miff * const miff, Miff8   const value);
MiffBool  miffSetValue16(           Miff * const miff, Miff16  const value);
MiffBool  miffSetValue32(           Miff * const miff, Miff32  const value);
MiffBool  miffSetValue64(           Miff * const miff, Miff64  const value);
MiffBool  miffSetValue128(          Miff * const miff, Miff128 const value);
MiffBool  miffSetValue256(          Miff * const miff, Miff256 const value);

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool  miffStart(MiffMemCreate const memCreate, MiffMemDestroy const memDestroy, 
   MiffMemCompress const compressBuffer, MiffMemDecompress const decompressBuffer);
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes) 
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreate ||
      !memDestroy)

   _memCreate     = memCreate;
   _memDestroy    = memDestroy;
   _memCompress   = memCompress;
   _memDecompress = memDecompress;

   _isStarted     = miffBoolTRUE;

   returnTrue;
}

/******************************************************************************
func: miffStop
******************************************************************************/
void miffStop(void)
{
   returnVoidIf(!_isStarted);

   _memCreate     = NULL;
   _memDestroy    = NULL;
   _memCompress   = NULL;
   _memDecompress = NULL;

   _isStarted = miffBoolFALSE;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _strClone
******************************************************************************/
static MiffC *_strClone(MiffN4 const count, MiffC const * const str)
{
   MiffC *result;

   result = memCreateTypeArray(count, MiffC);
   returnNullIf(!result);

   memCopyTypeArray(count - 1, MiffC, result, src);

   return result;
}

/******************************************************************************
func: _strFromUTF8
******************************************************************************/
static MiffC *_strFromUTF8(MiffN4 const count, MiffN1 const * const utf8)
{
   MiffN4 index,
          rindex;
   MiffC *result;
   MiffN1 a, 
          b,
          c,
          d;

   result = memCreateTypeArray(count + 1, MiffC);
   returnNullIf(!result);

   rindex = 0;
   forCount(index, count)
   {      
      a = utf8[index];
      if      ((a & 0x80) == 0)
      {
         result[rindex++] = a;
         index += 0;
      }
      else if ((a & 0xe0) == 0xc0)
      {
         b = utf8[index + 1];
         result[rindex++] =
            ((a & 0x1f) << 6) |
             (b & 0x3f);
         index += 1;
      }
      else if ((a & 0xf0) == 0xe0)
      {
         b = utf8[index + 1];
         c = utf8[index + 2];
         result[rindex++] =
            ((a & 0x0f) << 12) |
            ((b & 0x3f) <<  6) |
             (c & 0x3f);
         index += 2;
      }
      else
      {
         b = utf8[index + 1];
         c = utf8[index + 2];
         d = utf8[index + 1];
         result[rindex++] =
            ((a & 0x07) << 18) |
            ((b & 0x3f) << 12) |
            ((c & 0x3f) <<  6) |
             (d & 0x3f);
         index += 3;
      }
   }
   result[rindex] = 0;

   return result;
}

/******************************************************************************
func: _UTF8FromStr
******************************************************************************/
static MiffN1 *_UTF8FromStr(MiffN4 const count, MiffC const * const str)
{
}
