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
#define loop                                       for (;;)
#define forCount(INDEX, COUNT)                     for ((INDEX) = 0; (INDEX) < (COUNT); (INDEX)++)

#define breakIf(EXP)                               if (EXP) { break;    }
#define continueIf(EXP)                            if (EXP) { continue; }

#define returnFalse                                return miffBoolFALSE
#define returnNull                                 return NULL
#define returnTrue                                 return miffBoolTRUE
#define return0If(    EXP)                         if (EXP) { return 0;    }
#define returnFalseIf(EXP)                         if (EXP) { returnFalse; }
#define returnNullIf( EXP)                         if (EXP) { returnNull;  }
#define returnTrueIf( EXP)                         if (EXP) { returnTrue;  }
#define returnVoidIf( EXP)                         if (EXP) { return;      }

#define memClearType(             TYPE, MEM)                memset((MEM), 0,               sizeof(TYPE))
#define memClearTypeArray( COUNT, TYPE, MEM)                memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define memCopyTypeArray(  COUNT, TYPE, DST, SRC)           memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define memCreateType(            TYPE)            (TYPE *) _memCreate(                    sizeof(TYPE))
#define memCreateTypeArray(COUNT, TYPE)            (TYPE *) _memCreate(          (COUNT) * sizeof(TYPE))
#define memDestroy(                     MEM)                _memDestroy(MEM)

#define _strGetLength(STR)                         wcslen(STR)

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static MiffBool             _isStarted                = miffBoolFALSE;
static MiffMemCreate        _memCreate                = NULL;
static MiffMemDestroy       _memDestroy               = NULL;
static MiffMemCompress      _memCompress              = NULL;
static MiffMemDecompress    _memDecompress            = NULL;
static MiffN4               _base64State              = 0;
static MiffN1               _base64Byte               = 0;
static MiffN1               _base64LetterToValue[128];

/******************************************************************************
prototype:
******************************************************************************/
static void     _ByteSwap2(               Miff const * const miff, Miff2 * const value);
static void     _ByteSwap4(               Miff const * const miff, Miff4 * const value);
static void     _ByteSwap8(               Miff const * const miff, Miff8 * const value);

static MiffBool _MemIsEqual(              MiffN4 const countA, MiffN1 const * const strA, MiffN4 const countB, MiffN1 const * const strB);
                                          
static void     _ReadBase64Restart(       void);
static MiffBool _ReadBase64Byte(          Miff * const miff, MiffN1 * const byte);
static MiffN1   _ReadFirstLetter(         Miff * const miff);
static MiffBool _ReadKey(                 Miff * const miff, MiffBool const isRecordText);
static MiffBool _ReadPart(                Miff * const miff, MiffN4 * const partByteCount, MiffN1 *  const part);
static MiffBool _ReadPartDynamic(         Miff * const miff, MiffN4 * const partByteCount, MiffN1 ** const part);
static MiffBool _ReadValueHeader(         Miff * const miff, MiffBool const isRecordText);
static MiffBool _ReadValue1(              Miff * const miff, MiffBool const isRecordText, Miff1 *value);
static MiffBool _ReadValue2(              Miff * const miff, MiffBool const isRecordText, Miff2 *value);
static MiffBool _ReadValue4(              Miff * const miff, MiffBool const isRecordText, Miff4 *value);
static MiffBool _ReadValue8(              Miff * const miff, MiffBool const isRecordText, Miff8 *value);
                                          
static MiffC   *_MiffCClone(              MiffN4 const strCount, MiffC const * const str);
static MiffI8   _UTFDecimalToI(           MiffN4 const utfByteCount, MiffN1 const * const utf);
static MiffN8   _UTFDecimalToN(           MiffN4 const utfByteCount, MiffN1 const * const utf);
static void     _UTFToMiffC(              MiffN4 const utfByteCount, MiffN1 const * const utf, MiffN4 * const strCount, MiffC * const str);
static void     _UTFToMiffCKey(           MiffN4 const utfByteCount, MiffN1 const * const utf, MiffN1 * const strCount, MiffC * const str);
                                          
static MiffBool _MiffCToUTF(              MiffN4 const strCount, MiffC const * const str, MiffN4 * const utfByteCount, MiffN1 ** const utf);
static MiffBool _MiffCToUTFKey(           MiffN4 const strCount, MiffC const * const str, MiffN1 * const utfByteCount, MiffN1 * const utf);
static MiffN4   _MiffCLetterToUTFLetter(  MiffC const letter, MiffN1 * const a, MiffN1 * const b, MiffN1 * const c, MiffN1 * const d);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer1 getBuffer1Func, 
   MiffGetBuffer2 getBuffer2Func, MiffGetBuffer4 getBuffer4Func, MiffGetBuffer8 getBuffer8Func)
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
         getBuffer8Func))
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
   MiffGetBuffer1 getBuffer1Func, MiffGetBuffer2 getBuffer2Func, MiffGetBuffer4 getBuffer4Func, 
   MiffGetBuffer8 getBuffer8Func)
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

   returnFalseIf(!_ReadKey(miff, miffBoolFALSE));
   if      (_MemIsEqual(miff->keyByteCount, miff->key, 8, (MiffN1 *) "MIFF_BIN"))
   {
      miff->isBinary = miffBoolTRUE;
   }
   else if (_MemIsEqual(miff->keyByteCount, miff->key, 8, (MiffN1 *) "MIFF_TXT"))
   {
      miff->isBinary = miffBoolFALSE;
   }
   else
   {
      // Header is wrong.
      returnFalse;
   }

   returnFalseIf(!_ReadValueHeader(miff, miffBoolFALSE))
   returnFalseIf(!
      (miff->valueType    == miffValueTypeN8        &&
       miff->arrayFlag    == miffArrayFlagIS_SINGLE &&
       miff->compressFlag == miffCompressFlagIS_UNCOMPRESSED));
   returnFalseIf(!_ReadValue8(miff, miffBoolFALSE, (Miff8 *) &miff->version));
   returnFalseIf(miff->version != 1);

   returnFalseIf(!_ReadKey(miff, miffBoolFALSE));
   _UTFToMiffCKey(miff->keyByteCount, miff->key, &miff->subFormatNameCount, miff->subFormatName);
   returnFalseIf(!_ReadValueHeader(miff, miffBoolFALSE));
   returnFalseIf(!
      (miff->valueType    == miffValueTypeN8        &&
       miff->arrayFlag    == miffArrayFlagIS_SINGLE &&
       miff->compressFlag == miffCompressFlagIS_UNCOMPRESSED));
   returnFalseIf(!_ReadValue8(miff, miffBoolFALSE, (Miff8 *) &miff->subFormatVersion));

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer1 setBuffer1Func, 
   MiffSetBuffer2 setBuffer2Func, MiffSetBuffer4 setBuffer4Func, MiffSetBuffer8 setBuffer8Func, 
   MiffBool const isBinary, MiffN4 const subFormatNameLen, MiffC const * const subFormatName, 
   MiffN8 const subFormatVersion)
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
   MiffSetBuffer1 setBuffer1Func, MiffSetBuffer2 setBuffer2Func, MiffSetBuffer4 setBuffer4Func, 
   MiffSetBuffer8 setBuffer8Func, MiffBool const isBinary, MiffN1 const subFormatNameLen, 
   MiffC const * const subFormatName, MiffN8 const subFormatVersion)
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
   miff->method             = miffMethodWRITING;
   miff->isBinary           = isBinary;
   miff->isByteSwapping     = isByteSwaping;
   miff->setBuffer1         = setBuffer1Func;
   miff->setBuffer2         = setBuffer2Func;
   miff->setBuffer4         = setBuffer4Func;
   miff->setBuffer8         = setBuffer8Func;
   miff->subFormatVersion   = subFormatVersion;
   miff->subFormatNameCount = subFormatNameLen;
   memCopyTypeArray(subFormatNameLen, MiffC, miff->subFormatName, subFormatName);

   // Write the miff header.
   returnFalseIf(!(
      _writeKey(            miff, miffBoolFALSE, (isBinary) ? L"MIFF_BIN" : L"MIFF_TXT")                                      &&
      _writeValueHeaderText(miff, miffBoolFALSE, miffValueTypeN8, miffArrayFlagIS_SINGLE, miffCompressFlagIS_UNCOMPRESSED, 0) &&
      _writeValueN8(        miff, miffBoolFALSE, 1)                                                                           &&
      
      _writeKey(            miff, miffBoolFALSE, miff->subFormatName)                                                         &&
      _writeValueHeaderText(miff, miffBoolFALSE, miffValueTypeN8, miffArrayFlagIS_SINGLE, miffCompressFlagIS_UNCOMPRESSED, 0) &&
      _writeValueN8Text(    miff, miff->subFormatVersion)));

   return miff;
}

/******************************************************************************
func: miffDestroy
******************************************************************************/
void miffDestroy(Miff * const miff)
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

   memDestroy(miff->mem);
   memDestroy(miff->compressMem);
   memDestroy(miff->subFormatName);
   
   memClearType(Miff, miff);
   memDestroy(miff);
}

/******************************************************************************
func: miffGetFileFlagIsBinary
******************************************************************************/
MiffBool miffGetFileFlagIsBinary(Miff * const miff, MiffBool * const isBinary)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !isBinary);

   *isBinary = miff->isBinary;

   returnTrue;
}

/******************************************************************************
func: miffGetFileVersion
******************************************************************************/
MiffBool miffGetFileVersion(Miff * const miff, MiffN4 * const version)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !version);

   *version = miff->version;

   returnTrue;
}

/******************************************************************************
func: miffGetFileVersionSubFormat
******************************************************************************/
MiffBool miffGetFileVersionSubFormat(Miff * const miff, MiffC ** const subFormatName, 
   MiffN4 * const subFormatVersion)
{
   returnFalseIf(
      !_isStarted    ||
      !miff          ||
      !subFormatName ||
      !subFormatVersion);

   *subFormatName    = miff->subFormatName;
   *subFormatVersion = miff->subFormatVersion;

   returnTrue;
}

/******************************************************************************
func: miffGetKey
******************************************************************************/
MiffBool miffGetKey(Miff * const miff, MiffC ** const key)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !key);

   *key = miff->key;

   returnTrue;
}

/******************************************************************************
func: miffGetNextRecord
******************************************************************************/
MiffBool miffGetNextRecord(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnFalseIf(!_ReadKey(        miff, miff->isBinary));
   returnFalseIf(!_ReadValueHeader(miff, miff->isBinary));
}

/******************************************************************************
func: miffGetValueHeader
******************************************************************************/
MiffBool miffGetValueHeader(Miff * const miff, MiffValueType * const valueType, 
   MiffArrayFlag * const arrayFlag, MiffCompressFlag * const compressFlag)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !valueType  ||
      !arrayFlag);

   *valueType    = miff->valueType;
   *arrayFlag    = miff->arrayFlag;
   *compressFlag = miff->compressFlag;
}

/******************************************************************************
func: miffGetArrayCount
******************************************************************************/
MiffN4 miffGetArrayCount(Miff * const miff)
{
   return0If(
      !_isStarted ||
      !miff);

   return miff->arrayCount;
}

/******************************************************************************
func: miffGetValue1
******************************************************************************/
Miff1 miffGetValue1(Miff * const miff)
{
   Miff1 value;

   value.n = 0;

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, miff->valueType, &value);
      
   return value;
}

/******************************************************************************
func: miffGetValue2
******************************************************************************/
Miff2 miffGetValue2(Miff * const miff)
{
   Miff2 value;

   value.n = 0;

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue2(miff, miff->isBinary, miff->valueType, &value);

   return value;
}

/******************************************************************************
func: miffGetValue3
******************************************************************************/
Miff3 miffGetValue3(Miff * const miff)
{
   Miff3 value;

   memClearType(Miff3, value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, 3, &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue4
******************************************************************************/
Miff4 miffGetValue4(Miff * const miff)
{
   Miff4 value;

   value.n = 0;

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue4(miff, miff->isBinary, miff->valueType, &value);

   return value;
}

/******************************************************************************
func: miffGetValue8
******************************************************************************/
Miff8 miffGetValue8(Miff * const miff)
{
   Miff8 value;

   value.n = 0;

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue8(miff, miff->isBinary, miff->valueType, &value);

   return value;
}

/******************************************************************************
func: miffGetValue16
******************************************************************************/
Miff16 miffGetValue16(Miff * const miff)
{
   Miff16 value;

   memClearType(Miff16, value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, 16, &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue32
******************************************************************************/
Miff32 miffGetValue32(Miff * const miff)
{
   Miff32 value;

   memClearType(Miff32, value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, 32, &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue64
******************************************************************************/
Miff64 miffGetValue64(Miff * const miff)
{
   Miff16 value;

   memClearType(Miff64, value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, 64, &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue128
******************************************************************************/
Miff128 miffGetValue128(Miff * const miff)
{
   Miff128 value;

   memClearType(Miff128, value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, 128, &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue256
******************************************************************************/
Miff256 miffGetValue256(Miff * const miff)
{
   Miff256 value;

   memClearType(Miff256, value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->isBinary, 256, &value.byte);

   return value;
}

/******************************************************************************
func: miffSetKey
******************************************************************************/
MiffBool miffSetKey(Miff * const miff, MiffC const * const key, MiffValueType const type, 
   MiffArrayFlag const arrayFlag, MiffCompressFlag const compressFlag, 
   MiffN4 const chunkByteCount)
{
}

/******************************************************************************
func: miffSetValue1
******************************************************************************/
MiffBool miffSetValue1(Miff * const miff, Miff1 const value)
{
}

/******************************************************************************
func: miffSetValue2
******************************************************************************/
MiffBool miffSetValue2(Miff * const miff, Miff2 const value)
{
}

/******************************************************************************
func: miffSetValue3
******************************************************************************/
MiffBool miffSetValue3(Miff * const miff, Miff3 const value)
{
}

/******************************************************************************
func: miffSetValue4
******************************************************************************/
MiffBool miffSetValue4(Miff * const miff, Miff4 const value)
{
}

/******************************************************************************
func: miffSetValue8
******************************************************************************/
MiffBool miffSetValue8(Miff * const miff, Miff8 const value)
{
}

/******************************************************************************
func: miffSetValue16
******************************************************************************/
MiffBool miffSetValue16(Miff * const miff, Miff16 const value)
{
}

/******************************************************************************
func: miffSetValue32
******************************************************************************/
MiffBool miffSetValue32(Miff * const miff, Miff32 const value)
{
}

/******************************************************************************
func: miffSetValue64
******************************************************************************/
MiffBool miffSetValue64(Miff * const miff, Miff64 const value)
{
}

/******************************************************************************
func: miffSetValue128
******************************************************************************/
MiffBool miffSetValue128(Miff * const miff, Miff128 const value)
{
}

/******************************************************************************
func: miffSetValue256
******************************************************************************/
MiffBool miffSetValue256(Miff * const miff, Miff256 const value)
{
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool miffStart(MiffMemCreate const memCreate, MiffMemDestroy const memDestroy, 
   MiffMemCompress const compressBuffer, MiffMemDecompress const decompressBuffer);
{
   MiffN1 index;

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

   memClearTypeArray(128, MiffN1, _base64LetterToValue);
   index = 0;
   _base64LetterToValue['A'] = index++;
   _base64LetterToValue['B'] = index++;
   _base64LetterToValue['C'] = index++;
   _base64LetterToValue['D'] = index++;
   _base64LetterToValue['E'] = index++;
   _base64LetterToValue['F'] = index++;
   _base64LetterToValue['G'] = index++;
   _base64LetterToValue['H'] = index++;
   _base64LetterToValue['I'] = index++;
   _base64LetterToValue['J'] = index++;
   //10
   _base64LetterToValue['K'] = index++;
   _base64LetterToValue['L'] = index++;
   _base64LetterToValue['M'] = index++;
   _base64LetterToValue['N'] = index++;
   _base64LetterToValue['O'] = index++;
   _base64LetterToValue['P'] = index++;
   _base64LetterToValue['Q'] = index++;
   _base64LetterToValue['R'] = index++;
   _base64LetterToValue['S'] = index++;
   _base64LetterToValue['T'] = index++;
   //20
   _base64LetterToValue['U'] = index++;
   _base64LetterToValue['V'] = index++;
   _base64LetterToValue['W'] = index++;
   _base64LetterToValue['X'] = index++;
   _base64LetterToValue['Y'] = index++;
   _base64LetterToValue['Z'] = index++;
   _base64LetterToValue['a'] = index++;
   _base64LetterToValue['b'] = index++;
   _base64LetterToValue['c'] = index++;
   _base64LetterToValue['d'] = index++;
   //30
   _base64LetterToValue['e'] = index++;
   _base64LetterToValue['f'] = index++;
   _base64LetterToValue['g'] = index++;
   _base64LetterToValue['h'] = index++;
   _base64LetterToValue['i'] = index++;
   _base64LetterToValue['j'] = index++;
   _base64LetterToValue['k'] = index++;
   _base64LetterToValue['l'] = index++;
   _base64LetterToValue['m'] = index++;
   _base64LetterToValue['n'] = index++;
   //40
   _base64LetterToValue['o'] = index++;
   _base64LetterToValue['p'] = index++;
   _base64LetterToValue['q'] = index++;
   _base64LetterToValue['r'] = index++;
   _base64LetterToValue['s'] = index++;
   _base64LetterToValue['t'] = index++;
   _base64LetterToValue['u'] = index++;
   _base64LetterToValue['v'] = index++;
   _base64LetterToValue['w'] = index++;
   _base64LetterToValue['x'] = index++;
   //50
   _base64LetterToValue['y'] = index++;
   _base64LetterToValue['z'] = index++;
   _base64LetterToValue['0'] = index++;
   _base64LetterToValue['1'] = index++;
   _base64LetterToValue['2'] = index++;
   _base64LetterToValue['3'] = index++;
   _base64LetterToValue['4'] = index++;
   _base64LetterToValue['5'] = index++;
   _base64LetterToValue['6'] = index++;
   _base64LetterToValue['7'] = index++;
   //60
   _base64LetterToValue['8'] = index++;
   _base64LetterToValue['9'] = index++;
   _base64LetterToValue['+'] = index++;
   _base64LetterToValue['/'] = index++;

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
func: _ByteSwap2
******************************************************************************/
static void _ByteSwap2(Miff const * const miff, Miff2 * const value)
{
   if (miff->isByteSwapping)
   {
      value->byte[0] ^= value->byte[3];
      value->byte[3] ^= value->byte[0];
      value->byte[0] ^= value->byte[3];

      value->byte[1] ^= value->byte[2];
      value->byte[2] ^= value->byte[1];
      value->byte[1] ^= value->byte[2];
   }
}

/******************************************************************************
func: _ByteSwap4
******************************************************************************/
static void _ByteSwap4(Miff const * const miff, Miff4 * const value)
{
   if (miff->isByteSwapping)
   {
      value->byte[0] ^= value->byte[3];
      value->byte[3] ^= value->byte[0];
      value->byte[0] ^= value->byte[3];

      value->byte[1] ^= value->byte[2];
      value->byte[2] ^= value->byte[1];
      value->byte[1] ^= value->byte[2];
   }
}

/******************************************************************************
func: _ByteSwap8
******************************************************************************/
static void _ByteSwap8(Miff const * const miff, Miff8 * const value)
{
   if (miff->isByteSwapping)
   {
      value->byte[0] ^= value->byte[7];
      value->byte[7] ^= value->byte[0];
      value->byte[0] ^= value->byte[7];

      value->byte[1] ^= value->byte[6];
      value->byte[6] ^= value->byte[1];
      value->byte[1] ^= value->byte[6];

      value->byte[2] ^= value->byte[5];
      value->byte[5] ^= value->byte[2];
      value->byte[2] ^= value->byte[5];

      value->byte[3] ^= value->byte[4];
      value->byte[4] ^= value->byte[3];
      value->byte[3] ^= value->byte[4];
   }
}

/******************************************************************************
func: _MemIsEqual

Compare two binary buffers for equality.
******************************************************************************/
static MiffBool _MemIsEqual(MiffN4 const countA, MiffN1 const * const memA, MiffN4 const countB, 
   MiffN1 const * const memB)
{
   returnFalseIf(countA != countB);
   returnFalseIf(memcmp(memA, memB, countA) != 0);
   returnTrue;
}

/******************************************************************************
func: _ReadBase64Restart
******************************************************************************/
static void _ReadBase64Restart(void)
{
   _base64State = 0;
   _base64Byte  = 0;
}

/******************************************************************************
func: _ReadBase64Byte
******************************************************************************/
static MiffBool _ReadBase64Byte(Miff * const miff, Miff1 * const byte)
{
   byte->n = 0;


   switch (_base64State)
   {
   case 0:
      // byte to fill      Incoming base64 byte
      // [........]        [......]

      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, _base64Byte));

      // [111111..]        [111111]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 2);

      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, _base64Byte));

      // [11111122]        [22....]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] >> 4);

      // Remainder [..2222]
      _base64State = 1;
      break;

   case 1:
      // byte to fill      Incoming base64 byte
      // [........]        [..xxxx]

      // [xxxx....]        [..xxxx]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 4);

      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, _base64Byte));

      // [xxxx1111]        [1111..]
      byte->n != (MiffN1) (_base64LetterToValue[_base64Byte] >> 2);

      // Remainder [....22]
      _base64State = 2;
      break;

   case 2:
      // byte to fill      Incoming base64 byte
      // [........]        [....xx]

      // [xx......]        [....xx]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 6);

      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, _base64Byte));

      // [xx111111]        [111111]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte]);

      // Remainder [......]
      _base64State = 0;
      break;
   }

   returnTrue;
}

/******************************************************************************
func: _ReadFirstLetter

Skip white space to get the first letter.
******************************************************************************/
static MiffN1 _ReadFirstLetter(Miff * const miff)
{
   MiffN1 letter;

   letter = 0;
   loop
   {
      breakIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
      breakIf(
         !(letter == ' '  ||
           letter == '\t' || 
           letter == '\n');
   }

   return letter;
}

/******************************************************************************
func: _ReadKey
******************************************************************************/
static MiffBool _ReadKey(Miff * const miff, MiffBool const isRecordText)
{
   miff->keyByteCount = 0;
   miff->keyCCount    = 0;
   miff->key[0]       = 0;
   miff->keyC[0]      = 0;

   if (isRecordText)
   {
      _ReadPart(miff, 255, miff->keyByteCount, miff->key);
   }
   else
   {
      MiffN1 n1;

      // Get the key byte count.
      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, &n1));
      returnFalseIf(n1 == 0);

      // Get the key.
      miff->keyByteCount = n1;
      returnFalseIf(!miff->getBuffer1(miff->dataRepository, miff->keyByteCount, miff->key));
   }

   // Convert the key to MiffC
   _UTFToMiffCKey(miff->keyByteCount, miff->key, &miff->keyCCount, miff->keyC);

   returnTrue;
}

/******************************************************************************
func: _ReadPart

Read a stream of non-space characters.
******************************************************************************/
static MiffBool _ReadPart(Miff * const miff, MiffN4 const maxByteCount, MiffN4 * const partByteCount, MiffN1 * const part)
{
   MiffN4    byteCount;
   MiffN1   *btemp,
             letter;

   *partByteCount = 0;
   part[0]        = 0;

   // Skip over space.
   letter = _ReadFirstLetter(miff);

   // End of file and no letters anymore.
   returnFalseIf(
      letter == ' '  ||
      letter == '\t' ||
      letter == '\n');

   // Read in the stream of letters to the next space.
   byteCount = 0;
   loop
   {
      // If the buffer isn't larger enough for the part we read up to the maxByteCount and then 
      // ignore the rest.  This should never happen.  If it does we have a mal-formed MIFF.
      if (byteCount != maxByteCount)
      {
         part[byteCount++] = letter;
      }
      else
      {
         assert(0 && "This should not happen.  If you are here, dar be dragons!");
      }

      breakIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
      breakIf(
         letter == ' '  ||
         letter == '\t' ||
         letter == '\n');
   }

   *partByteCount = byteCount;

   returnTrue;
}

/******************************************************************************
func: _ReadPartDynamic

Read a stream of non-space characters.
******************************************************************************/
static MiffBool _ReadPartDynamic(Miff * const miff, MiffN4 * const partByteCount, MiffN1 ** const part)
{
   MiffN4  byteCount,
      byteCountMax;
   MiffN1 *buffer,
      *btemp,
      letter;

   *partByteCount = 0;
   *part          = NULL;

   byteCountMax = 16;
   buffer       = memCreateTypeArray(byteCountMax, MiffN1);
   returnFalseIf(!buffer);

   // Skip over space.
   letter = _ReadFirstLetter(miff);

   // End of file and no letters anymore.
   returnFalseIf(
      letter == ' '  ||
      letter == '\t' ||
      letter == '\n');

   // Read in the stream of letters to the next space.
   byteCount = 0;
   loop
   {
      buffer[byteCount++] = letter;

      breakIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
      breakIf(
         letter == ' '  ||
         letter == '\t' ||
         letter == '\n');

      // Resize the buffer when we hit the limit.
      if (byteCount == byteCountMax)
      {
         byteCountMax += byteCountMax / 2;
         btemp = memCreateTypeArray(byteCountmax, MiffN1);
         if (!btemp)
         {
            // Clean up on failure.
            memDestroy(buffer);
            returnFalse;
         }

         memCopyTypeArray(byteCount, buffer, btemp);
         memDestroy(buffer);
         buffer = btemp;
      }
   }

   *partByteCount = byteCount;
   *part          = buffer;

   returnTrue;
}

/******************************************************************************
func: _ReadValueHeader
******************************************************************************/
static MiffBool _ReadValueHeader(Miff * const miff, MiffBool const isRecordText)
{
   if (isRecordText)
   {
      MiffN1 letter;
      MiffN4 typeByteCount;
      MiffN1 type[264];

      memClearTypeArray(264, MiffN1, type);
      returnFalseIf(!_ReadPart(miff, 255, &typeByteCount, type));

      typeByteCount--;
      switch (type[typeByteCount])
      {
      default:
      case '-':
      case 'z':
      case 'c':
         miff->arrayFlag = miffArrayFlagIS_SINGLE;
         break;

      case '=':
      case 'Z':
      case 'C':
         miff->arrayFlag = miffArrayFlagIS_ARRAY;
         break;
      }

      switch (type[typeByteCount])
      {
      default:
      case '-':
      case '=':
         miff->compressFlag = miffCompressFlagIS_UNCOMPRESSED;
         break;

      case 'z':
      case 'Z':
         miff->compressFlag = miffCompressFlagIS_COMPRESSED;
         break;

      case 'c':
      case 'C':
         miff->compressFlag = miffCompressFlagIS_CHUNKED_AND_COMPRESSED;
         break;
      }

      type[typeByteCount] = 0;
      if      (_MemIsEqual(typeByteCount, type, 2, "[]"))         miff->valueType = miffValueTypeKEY_VALUE_BLOCK;
      else if (_MemIsEqual(typeByteCount, type, 5, "[...]"))      miff->valueType = miffValueTypeVALUE_STREAM_BLOCK;
      else if (_MemIsEqual(typeByteCount, type, 3, "..."))        miff->valueType = miffValueTypeBINARY_DATA;
      else if (_MemIsEqual(typeByteCount, type, 4, "file"))       miff->valueType = miffValueTypeEMBEDDED_FILE;
      else if (_MemIsEqual(typeByteCount, type, 4, "type"))       miff->valueType = miffValueTypeTYPE;
      else if (_MemIsEqual(typeByteCount, type, 2, "\"\""))       miff->valueType = miffValueTypeSTRING;
      else if (_MemIsEqual(typeByteCount, type, 4, "path"))       miff->valueType = miffValueTypePATH;
      else if (_MemIsEqual(typeByteCount, type, 8, "userType"))   miff->valueType = miffValueTypeUSER_TYPE;
      else if (_MemIsEqual(typeByteCount, type, 2, "i1"))         miff->valueType = miffValueTypeI1;
      else if (_MemIsEqual(typeByteCount, type, 2, "i2"))         miff->valueType = miffValueTypeI2;
      else if (_MemIsEqual(typeByteCount, type, 2, "i3"))         miff->valueType = miffValueTypeI3;
      else if (_MemIsEqual(typeByteCount, type, 2, "i4"))         miff->valueType = miffValueTypeI4;
      else if (_MemIsEqual(typeByteCount, type, 2, "i8"))         miff->valueType = miffValueTypeI8;
      else if (_MemIsEqual(typeByteCount, type, 3, "i16"))        miff->valueType = miffValueTypeI16;
      else if (_MemIsEqual(typeByteCount, type, 3, "i32"))        miff->valueType = miffValueTypeI32;
      else if (_MemIsEqual(typeByteCount, type, 3, "i64"))        miff->valueType = miffValueTypeI64;
      else if (_MemIsEqual(typeByteCount, type, 4, "i128"))       miff->valueType = miffValueTypeI128;
      else if (_MemIsEqual(typeByteCount, type, 4, "i256"))       miff->valueType = miffValueTypeI256;
      else if (_MemIsEqual(typeByteCount, type, 2, "n1"))         miff->valueType = miffValueTypeN1;
      else if (_MemIsEqual(typeByteCount, type, 2, "n2"))         miff->valueType = miffValueTypeN2;
      else if (_MemIsEqual(typeByteCount, type, 2, "n3"))         miff->valueType = miffValueTypeN3;
      else if (_MemIsEqual(typeByteCount, type, 2, "n4"))         miff->valueType = miffValueTypeN4;
      else if (_MemIsEqual(typeByteCount, type, 2, "n8"))         miff->valueType = miffValueTypeN8;
      else if (_MemIsEqual(typeByteCount, type, 3, "n16"))        miff->valueType = miffValueTypeN16;
      else if (_MemIsEqual(typeByteCount, type, 3, "n32"))        miff->valueType = miffValueTypeN32;
      else if (_MemIsEqual(typeByteCount, type, 3, "n64"))        miff->valueType = miffValueTypeN64;
      else if (_MemIsEqual(typeByteCount, type, 4, "n128"))       miff->valueType = miffValueTypeN128;
      else if (_MemIsEqual(typeByteCount, type, 4, "n256"))       miff->valueType = miffValueTypeN256;
      else if (_MemIsEqual(typeByteCount, type, 2, "r4"))         miff->valueType = miffValueTypeR4;
      else if (_MemIsEqual(typeByteCount, type, 2, "r8"))         miff->valueType = miffValueTypeR8;
      else if (_MemIsEqual(typeByteCount, type, 3, "r16"))        miff->valueType = miffValueTypeR16;
      else if (_MemIsEqual(typeByteCount, type, 3, "r32"))        miff->valueType = miffValueTypeR32;
      else if (_MemIsEqual(typeByteCount, type, 3, "r64"))        miff->valueType = miffValueTypeR64;
      else if (_MemIsEqual(typeByteCount, type, 4, "r128"))       miff->valueType = miffValueTypeR128;
      else if (_MemIsEqual(typeByteCount, type, 4, "r256"))       miff->valueType = miffValueTypeR256;
      else 
      {
         // User type
         //miff->valueType = _FindUserType(typeByteCount, type);
      }

      // Read in the array count
      if (miff->arrayFlag == miffArrayFlagIS_ARRAY)
      {
         returnFalseIf(!_ReadValue4(miff, isRecordText, miffValueTypeN4, &miff->arrayCount));
      }
   }
   else
   {
      MiffN2 n2;

      returnFalseIf(!miff->getBuffer2(miff->dataRepository, 2, &n2));
      miff->compressFlag = (n2 >> 14) & 0x0003;
      miff->arrayFlag    = (n2 >> 12) & 0x0003;
      miff->valueType    =  n2        & 0x0FFF;

      // Read in the array count
      if (miff->arrayFlag == miffArrayFlagIS_ARRAY)
      {
         returnFalseIf(!miff->getBuffer4(miff->dataRepository, 4, &miff->arrayCount));
      }
   }

   returnTrue;
}

/******************************************************************************
func: _ReadValue1
******************************************************************************/
static MiffBool _ReadValue1(Miff * const miff, MiffBool const isRecordText, 
   MiffValueType const type, Miff1 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (!isRecordText)
   {
      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, value));
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

   if (type == miffValueTypeI1)
   {
      value->i = (MiffI1) _UTFDecimalToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN1) _UTFDecimalToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _ReadValue2
******************************************************************************/
static MiffBool _ReadValue2(Miff * const miff, MiffBool const isRecordText, 
   MiffValueType const type, Miff2 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (!isRecordText)
   {
      returnFalseIf(!miff->getBuffer2(miff->dataRepository, 1, value));
      _ByteSwap2(miff, value);
   }

   if (type == miffValueTypeR4)
   {
      if (miff->isByteSwapping)
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, &value->byte[1]) ||
            !_ReadBase64Byte(miff, &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, &value->byte[0]) ||
            !_ReadBase64Byte(miff, &value->byte[1]));
      }

      returnTrue;
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, &part));

   if (type == miffValueTypeI2)
   {
      value->i = (MiffI2) _UTFDecimalToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN2) _UTFDecimalToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _ReadValue4
******************************************************************************/
static MiffBool _ReadValue4(Miff * const miff, MiffBool const isRecordText, 
   MiffValueType const type, Miff4 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   // Binary read value.
   if (!isRecordText)
   {
      returnFalseIf(!miff->getBuffer4(miff->dataRepository, 1, value));
      _ByteSwap4(miff, value);

      returnTrue;
   }

   // Text read.

   // Type requires a base64 representation.
   if (type == miffValueTypeR4)
   {
      if (miff->isByteSwapping)
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, &value->byte[3]) ||
            !_ReadBase64Byte(miff, &value->byte[2]) ||
            !_ReadBase64Byte(miff, &value->byte[1]) ||
            !_ReadBase64Byte(miff, &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, &value->byte[0]) ||
            !_ReadBase64Byte(miff, &value->byte[1]) ||
            !_ReadBase64Byte(miff, &value->byte[2]) ||
            !_ReadBase64Byte(miff, &value->byte[3]));
      }

      returnTrue;
   }

   // read the value part.
   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, &part));

   // Convert the value part to I or N.
   if (type == miffValueTypeI4)
   {
      value->i = (MiffI4) _UTFDecimalToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN4) _UTFDecimalToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _ReadValue8
******************************************************************************/
static MiffBool _ReadValue8(Miff * const miff, MiffBool const isRecordText, 
   MiffValueType const type, Miff8 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (!isRecordText)
   {
      returnFalseIf(!miff->getBuffer8(miff->dataRepository, 1, value));
      _ByteSwap8(miff, value);
   }

   if (type == miffValueTypeR4)
   {
      if (miff->isByteSwapping)
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, &value->byte[7]) ||
            !_ReadBase64Byte(miff, &value->byte[6]) ||
            !_ReadBase64Byte(miff, &value->byte[5]) ||
            !_ReadBase64Byte(miff, &value->byte[4]) ||
            !_ReadBase64Byte(miff, &value->byte[3]) ||
            !_ReadBase64Byte(miff, &value->byte[2]) ||
            !_ReadBase64Byte(miff, &value->byte[1]) ||
            !_ReadBase64Byte(miff, &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, &value->byte[0]) ||
            !_ReadBase64Byte(miff, &value->byte[1]) ||
            !_ReadBase64Byte(miff, &value->byte[2]) ||
            !_ReadBase64Byte(miff, &value->byte[3]) ||
            !_ReadBase64Byte(miff, &value->byte[4]) ||
            !_ReadBase64Byte(miff, &value->byte[5]) ||
            !_ReadBase64Byte(miff, &value->byte[6]) ||
            !_ReadBase64Byte(miff, &value->byte[7]));
      }

      returnTrue;
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, &part));

   if (type == miffValueTypeI8)
   {
      value->i = (MiffI8) _UTFDecimalToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN8) _UTFDecimalToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _MiffCClone
******************************************************************************/
static MiffC *_MiffCClone(MiffN4 const strCount, MiffC const * const str)
{
   MiffC *result;

   result = memCreateTypeArray(strCount, MiffC);
   returnNullIf(!result);

   memCopyTypeArray(strCount, MiffC, result, src);

   return result;
}

/******************************************************************************
func: _UTFDecimalToI
******************************************************************************/
static MiffI8 _UTFDecimalToI(MiffN4 const utfByteCount, MiffN1 const * const utf)
{
   MiffBool isNegative;
   MiffN4   index;
   MiffN8   value;

   isNegative = miffBoolFALSE;
   value      = 0;
   forCount(index, utfByteCount)
   {
      if (index == 0)
      {
         if (utf[index] == '-')
         {
            isNegative = miffBoolTRUE;
            continue;
         }
      }

      value = value * 10 + utf[index] - '0';
   }

   if (isNegative)
   {
      return -value;
   }

   return value;
}

/******************************************************************************
func: _UTFDecimalToN
******************************************************************************/
static MiffN8 _UTFDecimalToN(MiffN4 const utfByteCount, MiffN1 const * const utf)
{
   MiffN4   index;
   MiffN8   value;

   value = 0;
   forCount(index, utfByteCount)
   {
      value = value * 10 + utf[index] - '0';
   }

   return value;
}

/******************************************************************************
func: _UTFToMiffC
******************************************************************************/
static void _UTFToMiffC(MiffN4 const count, MiffN1 const * const utf8, MiffN4 * const strCount,
   MiffC * const str)
{
   MiffN4 index,
          rindex;
   MiffN1 a, 
          b,
          c,
          d;

   rindex = 0;
   forCount(index, count)
   {      
      a = utf8[index];
      if      ((a & 0x80) == 0)
      {
         str[rindex++] = a;
         index += 0;
      }
      else if ((a & 0xe0) == 0xc0)
      {
         b = utf8[index + 1];
         str[rindex++] =
            ((a & 0x1f) << 6) |
             (b & 0x3f);
         index += 1;
      }
      else if ((a & 0xf0) == 0xe0)
      {
         b = utf8[index + 1];
         c = utf8[index + 2];
         str[rindex++] =
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
         str[rindex++] =
            ((a & 0x07) << 18) |
            ((b & 0x3f) << 12) |
            ((c & 0x3f) <<  6) |
             (d & 0x3f);
         index += 3;
      }
   }
   *strCount = rindex;
}

/******************************************************************************
func: _UTFToMiffCKey
******************************************************************************/
static void _UTFToMiffCKey(MiffN4 const count, MiffN1 const * const utf8, MiffN1 * const strCount,
   MiffC * const str)
{
   MiffN4 index,
          rindex;
   MiffN1 a, 
          b,
          c,
          d;

   rindex = 0;
   forCount(index, count)
   {      
      a = utf8[index];
      if      ((a & 0x80) == 0)
      {
         str[rindex++] = a;
         index += 0;
      }
      else if ((a & 0xe0) == 0xc0)
      {
         b = utf8[index + 1];
         str[rindex++] =
            ((a & 0x1f) << 6) |
             (b & 0x3f);
         index += 1;
      }
      else if ((a & 0xf0) == 0xe0)
      {
         b = utf8[index + 1];
         c = utf8[index + 2];
         str[rindex++] =
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
         str[rindex++] =
            ((a & 0x07) << 18) |
            ((b & 0x3f) << 12) |
            ((c & 0x3f) <<  6) |
             (d & 0x3f);
         index += 3;
      }
   }
   *strCount = rindex;
}

/******************************************************************************
func: _MiffCToUTF
******************************************************************************/
static MiffBool _MiffCToUTF(MiffN4 const count, MiffC const * const str, MiffN4 * const utfByteCount, 
   MiffN1 ** const utf)
{
   MiffN4 utfIndex,
          strIndex,
          utfByteCountMax,
          letterCount;
   MiffN1 a,
          b,
          c,
          d,
         *result;

   utfByteCountMax = count * 4 + 1;
   result          = memCreateTypeArray(utfByteCountMax, MiffN1);
   returnNullIf(!result);

   utfIndex = 0;
   forCount(strIndex, count)
   {
      returnFalseIf(utfIndex == utfByteCountMax);

      letterCount = _MiffCLetterToUTFLetter(str[strIndex], &a, &b, &c, &d);
      returnFalseIf(!letterCount);

      result[utfIndex++] = a;
      continueIf(letterCount == 1);
      returnFalseIf(utfIndex == utfByteCountMax);
      
      result[utfIndex++] = b;
      continueIf(letterCount == 2);
      returnFalseIf(utfIndex == utfByteCountMax);

      result[utfIndex++] = c;
      continueIf(letterCount == 3);
      returnFalseIf(utfIndex == utfByteCountMax);
      
      key[keyIndex++] = d;
   }

   *utfByteCount = utfIndex;
   *utf          = result;

   returnTrue;
}

/******************************************************************************
func: _MiffCToUTFKey
******************************************************************************/
static MiffBool _MiffCToUTFKey(MiffN4 const count, MiffC const * const str, 
   MiffN1 * const utfByteCount, MiffN1 * const utf)
{
   MiffN4 utfIndex,
          strIndex,
          letterCount;
   MiffN1 a,
          b,
          c,
          d;

   utfIndex = 0;
   forCount(strIndex, count)
   {
      returnFalseIf(utfIndex == 256);

      letterCount = _MiffCLetterToUTFLetter(str[strIndex], &a, &b, &c, &d);
      returnFalseIf(!letterCount);

      utf[utfIndex++] = a;
      continueIf(letterCount == 1);
      returnFalseIf(utfIndex == 256);
      
      utf[utfIndex++] = b;
      continueIf(letterCount == 2);
      returnFalseIf(utfIndex == 256);

      utf[utfIndex++] = c;
      continueIf(letterCount == 3);
      returnFalseIf(utfIndex == 256);
      
      utf[utfIndex++] = d;
   }

   *utfByteCount = utfIndex;

   returnTrue;
}

/******************************************************************************
func: _MiffCLetterToUTFLetter
******************************************************************************/
static MiffN4 _MiffCLetterToUTFLetter(MiffC const letter, MiffN1 * const a, MiffN1 * const b, 
   MiffN1 * const c, MiffN1 * const d)
{
   return0If(!a || !b || !c || !d);

   if (letter < 0x0000007f)
   {
      *d = 0;
      *c = 0;
      *b = 0;
      *a = (Gc1) letter;

      return 1;
   }
   
   if (letter < 0x000007ff)
   {
      *d = 0;
      *c = 0;
      *b = (Gc1) (0x80 |  (letter       & 0x3f));
      *a = (Gc1) (0xc0 | ((letter >> 6) & 0x1f));

      return 2;
   }
   
   if (letter < 0x0000ffff)
   {
      *d = 0;
      *c = (Gc1) (0x80 |  (letter        & 0x3f));
      *b = (Gc1) (0x80 | ((letter >>  6) & 0x3f));
      *a = (Gc1) (0xe0 | ((letter >> 12) & 0x0f));

      return 3;
   }

   // letter < 0x001fffff
   *d = (Gc1) (0x80 |  (letter        & 0x3f));
   *c = (Gc1) (0x80 | ((letter >>  6) & 0x3f));
   *b = (Gc1) (0x80 | ((letter >> 12) & 0x3f));
   *c = (Gc1) (0xf0 | ((letter >> 18) & 0x07));

   return 4;
}
