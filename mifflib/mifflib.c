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
#include <assert.h>
#include <memory.h>
#include <string.h>

#include "miffLib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
#define loop                                       for (;;)
#define forCount(    INDEX, COUNT)                 for ((INDEX) = 0;         (INDEX) < (COUNT); (INDEX)++)
#define forCountDown(INDEX, COUNT)                 for ((INDEX) = COUNT - 1; (INDEX) >= 0;      (INDEX)--)

#define breakIf(EXP)                               if (EXP) { break;    }
#define continueIf(EXP)                            if (EXP) { continue; }

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
static MiffC   *_MiffCClone(              MiffN4 const strCount, MiffC const * const str);
static MiffN4   _MiffCLetterToUTFLetter(  MiffN4 const letter, MiffN1 * const a, MiffN1 * const b, MiffN1 * const c, MiffN1 * const d);
static MiffBool _MiffCToUTF(              MiffN4 const strCount, MiffC const * const str, MiffN4 * const utfByteCount, MiffN1 ** const utf);
static MiffBool _MiffCToUTFKey(           MiffN4 const strCount, MiffC const * const str, MiffN1 * const utfByteCount, MiffN1 * const utf);
                                          
static void     _ReadBase64Restart(       void);
static MiffBool _ReadBase64Byte(          Miff * const miff, Miff1 * const byte);
static MiffN1   _ReadFirstLetter(         Miff * const miff);
static MiffBool _ReadKey(                 Miff * const miff, MiffMode const mode);
static MiffBool _ReadPart(                Miff * const miff, MiffN4 const maxByteCount, MiffN4 * const partByteCount, MiffN1 *  const part);
static MiffBool _ReadPartDynamic(         Miff * const miff,                            MiffN4 * const partByteCount, MiffN1 ** const part);
static MiffBool _ReadValueHeader(         Miff * const miff, MiffMode const mode);
static MiffBool _ReadValue1(              Miff * const miff, MiffMode const mode, MiffValueType const type, Miff1 * const value);
static MiffBool _ReadValue2(              Miff * const miff, MiffMode const mode, MiffValueType const type, Miff2 * const value);
static MiffBool _ReadValue4(              Miff * const miff, MiffMode const mode, MiffValueType const type, Miff4 * const value);
static MiffBool _ReadValue8(              Miff * const miff, MiffMode const mode, MiffValueType const type, Miff8 * const value);
                                          
static MiffI8   _UTFDecimalToI(           MiffN4 const utfByteCount, MiffN1 const * const utf);
static MiffN8   _UTFDecimalToN(           MiffN4 const utfByteCount, MiffN1 const * const utf);
static void     _UTFToMiffC(              MiffN4 const utfByteCount, MiffN1 const * const utf, MiffN4 * const strCount, MiffC * const str);
static void     _UTFToMiffCKey(           MiffN4 const utfByteCount, MiffN1 const * const utf, MiffN1 * const strCount, MiffC * const str);
                                          
static void     _WriteEndRecord(          Miff * const miff, MiffMode const mode);
static void     _WriteKey(                Miff * const miff, MiffMode const mode);
static void     _WriteValueHeader(        Miff * const miff, MiffMode const mode, MiffValueType const type, MiffArrayFlag const arrayFlag, MiffN4 const arrayCount, MiffCompressFlag const compressFlag, MiffN4 const compressChunkByteCount);
static void     _WriteValue1(             Miff * const miff, MiffMode const mode, MiffValueType const type, Miff1 * const value);
static void     _WriteValue2(             Miff * const miff, MiffMode const mode, MiffValueType const type, Miff2 * const value);
static void     _WriteValue4(             Miff * const miff, MiffMode const mode, MiffValueType const type, Miff4 * const value);
static void     _WriteValue8(             Miff * const miff, MiffMode const mode, MiffValueType const type, Miff8 * const value);
static void     _WriteValueNText(         Miff * const miff, MiffN8 const value);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer1 getBuffer1Func, 
   MiffGetBuffer2 getBuffer2Func, MiffGetBuffer4 getBuffer4Func, MiffGetBuffer8 getBuffer8Func, 
   void * const dataRepo)
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
         getBuffer8Func, 
         dataRepo))
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
   MiffGetBuffer8 getBuffer8Func, void * const dataRepo)
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
   miff->dataRepo = dataRepo;

   returnFalseIf(!_ReadKey(miff, miffModeTEXT));
   if      (_MemIsEqual(miff->keyByteCount, miff->key, 8, (MiffN1 *) "MIFF_BIN"))
   {
      miff->mode = miffModeBINARY;
   }
   else if (_MemIsEqual(miff->keyByteCount, miff->key, 8, (MiffN1 *) "MIFF_TXT"))
   {
      miff->mode = miffModeTEXT;
   }
   else
   {
      // Header is wrong.
      returnFalse;
   }

   returnFalseIf(!_ReadValueHeader(miff, miffModeTEXT))
   returnFalseIf(!
      (miff->valueType    == miffValueTypeN8        &&
       miff->arrayFlag    == miffArrayFlagIS_SINGLE &&
       miff->compressFlag == miffCompressFlagIS_UNCOMPRESSED));
   returnFalseIf(!_ReadValue8(miff, miffModeTEXT, miffValueTypeN8, (Miff8 *) &miff->version));
   returnFalseIf(miff->version != 1);

   returnFalseIf(!_ReadKey(miff, miffModeTEXT));
   _UTFToMiffCKey(miff->keyByteCount, miff->key, &miff->subFormatNameCount, miff->subFormatName);
   returnFalseIf(!_ReadValueHeader(miff, miffModeTEXT));
   returnFalseIf(!
      (miff->valueType    == miffValueTypeN8        &&
       miff->arrayFlag    == miffArrayFlagIS_SINGLE &&
       miff->compressFlag == miffCompressFlagIS_UNCOMPRESSED));
   returnFalseIf(!_ReadValue8(miff, miffModeTEXT, miffValueTypeN8, (Miff8 *) &miff->subFormatVersion));

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer1 setBuffer1Func, 
   MiffSetBuffer2 setBuffer2Func, MiffSetBuffer4 setBuffer4Func, MiffSetBuffer8 setBuffer8Func, 
   MiffMode const mode, MiffN1 const subFormatNameLen, MiffC const * const subFormatName, 
   MiffN8 const subFormatVersion, void * const dataRepo)
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
         mode, 
         subFormatNameLen,
         subFormatName, 
         subFormatVersion, 
         dataRepo))
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
   MiffSetBuffer8 setBuffer8Func, MiffMode const mode, MiffN1 const subFormatNameLen, 
   MiffC const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo)
{
   Miff8 value8;

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
   miff->mode               = mode;
   miff->isByteSwapping     = isByteSwaping;
   miff->setBuffer1         = setBuffer1Func;
   miff->setBuffer2         = setBuffer2Func;
   miff->setBuffer4         = setBuffer4Func;
   miff->setBuffer8         = setBuffer8Func;
   miff->subFormatVersion   = subFormatVersion;
   miff->subFormatNameCount = subFormatNameLen;
   memCopyTypeArray(subFormatNameLen, MiffC, miff->subFormatName, subFormatName);
   miff->dataRepo           = dataRepo;

   // Write the miff header.
   value8.n = 1;
   _MiffCToUTFKey(8, (mode == miffModeBINARY) ? L"MIFF_BIN" : L"MIFF_TXT", &miff->keyByteCount, miff->key);
   _WriteKey(        miff, miffModeTEXT);
   _WriteValueHeader(miff, miffModeTEXT, miffValueTypeN8, miffArrayFlagIS_SINGLE, 0, miffCompressFlagIS_UNCOMPRESSED, 0);
   _WriteValue8(     miff, miffModeTEXT, miffValueTypeN8, (Miff8 *) &value8);
   _WriteEndRecord(  miff, miffModeTEXT);
   
   _MiffCToUTFKey(miff->subFormatNameCount, miff->subFormatName, &miff->keyByteCount, miff->key);
   _WriteKey(        miff, miffModeTEXT);
   _WriteValueHeader(miff, miffModeTEXT, miffValueTypeN8, miffArrayFlagIS_SINGLE, 0, miffCompressFlagIS_UNCOMPRESSED, 0);
   _WriteValue8(     miff, miffModeTEXT, miffValueTypeN8, (Miff8 *) &miff->subFormatVersion);
   _WriteEndRecord(  miff, miffModeTEXT);

   returnTrue;
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
   returnVoidIf(
      !_isStarted ||
      !miff);

   memDestroy(miff->mem);
   memDestroy(miff->compressMem);
   
   memClearType(Miff, miff);
   memDestroy(miff);
}

/******************************************************************************
func: miffGetFileFlagIsBinary
******************************************************************************/
MiffBool miffGetFileFlagIsBinary(Miff * const miff, MiffBool * const mode)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !mode);

   *mode = (miff->mode == miffModeBINARY);

   returnTrue;
}

/******************************************************************************
func: miffGetFileVersion
******************************************************************************/
MiffBool miffGetFileVersion(Miff * const miff, MiffN8 * const version)
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
   MiffN8 * const subFormatVersion)
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

   *key = miff->keyC;

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

   returnFalseIf(!_ReadKey(        miff, miff->mode));
   returnFalseIf(!_ReadValueHeader(miff, miff->mode));

   returnTrue;
}

/******************************************************************************
func: miffGetValueHeader
******************************************************************************/
MiffBool miffGetValueHeader(Miff * const miff, MiffValueType * const type, 
   MiffArrayFlag * const arrayFlag, MiffN4 * const arrayCount, 
   MiffCompressFlag * const compressFlag, MiffN4 * const chunkByteCount)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !type       ||
      !arrayFlag);

   *type             = miff->valueType;
   *arrayFlag        = miff->arrayFlag;
   *arrayCount       = miff->arrayCount;
   *compressFlag     = miff->compressFlag;
   *chunkByteCount   = miff->compressChunkByteCount;

   returnTrue;
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

   _ReadValue1(miff, miff->mode, miff->valueType, &value);
      
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

   _ReadValue2(miff, miff->mode, miff->valueType, &value);

   return value;
}

/******************************************************************************
func: miffGetValue3
******************************************************************************/
Miff3 miffGetValue3(Miff * const miff)
{
   Miff3 value;

   memClearType(Miff3, &value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->mode, 3, (Miff1 *) &value.byte);

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

   _ReadValue4(miff, miff->mode, miff->valueType, &value);

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

   _ReadValue8(miff, miff->mode, miff->valueType, &value);

   return value;
}

/******************************************************************************
func: miffGetValue16
******************************************************************************/
Miff16 miffGetValue16(Miff * const miff)
{
   Miff16 value;

   memClearType(Miff16, &value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->mode, 16, (Miff1 *) &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue32
******************************************************************************/
Miff32 miffGetValue32(Miff * const miff)
{
   Miff32 value;

   memClearType(Miff32, &value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->mode, 32, (Miff1 *) &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue64
******************************************************************************/
Miff64 miffGetValue64(Miff * const miff)
{
   Miff64 value;

   memClearType(Miff64, &value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->mode, 64, (Miff1 *) &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue128
******************************************************************************/
Miff128 miffGetValue128(Miff * const miff)
{
   Miff128 value;

   memClearType(Miff128, &value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->mode, 128, (Miff1 *) &value.byte);

   return value;
}

/******************************************************************************
func: miffGetValue256
******************************************************************************/
Miff256 miffGetValue256(Miff * const miff)
{
   Miff256 value;

   memClearType(Miff256, &value);

   returnIf(
         !_isStarted ||
         !miff,
      value);

   _ReadValue1(miff, miff->mode, 256, (Miff1 *) &value.byte);

   return value;
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetI1(Miff * const miff, MiffC const * const key, MiffI1 const value)
{
   Miff1 vtemp;

   vtemp.i = value;

   miffSetNextRecord(miff, key, miffValueTypeI1, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue1(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetI2(Miff * const miff, MiffC const * const key, MiffI2 const value)
{
   Miff2 vtemp;

   vtemp.i = value;

   miffSetNextRecord(miff, key, miffValueTypeI2, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue2(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetI4(Miff * const miff, MiffC const * const key, MiffI4 const value)
{
   Miff4 vtemp;

   vtemp.i = value;

   miffSetNextRecord(miff, key, miffValueTypeI4, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue4(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetI8(Miff * const miff, MiffC const * const key, MiffI8 const value)
{
   Miff8 vtemp;

   vtemp.i = value;

   miffSetNextRecord(miff, key, miffValueTypeI8, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue8(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetN1(Miff * const miff, MiffC const * const key, MiffN1 const value)
{
   Miff1 vtemp;

   vtemp.n = value;

   miffSetNextRecord(miff, key, miffValueTypeN1, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue1(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetN2(Miff * const miff, MiffC const * const key, MiffN2 const value)
{
   Miff2 vtemp;

   vtemp.n = value;

   miffSetNextRecord(miff, key, miffValueTypeN2, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue2(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetN4(Miff * const miff, MiffC const * const key, MiffN4 const value)
{
   Miff4 vtemp;

   vtemp.n = value;

   miffSetNextRecord(miff, key, miffValueTypeN4, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue4(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetN8(Miff * const miff, MiffC const * const key, MiffN8 const value)
{
   Miff8 vtemp;

   vtemp.n = value;

   miffSetNextRecord(miff, key, miffValueTypeN8, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue8(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetR4(Miff * const miff, MiffC const * const key, MiffR4 const value)
{
   Miff4 vtemp;

   vtemp.r = value;

   miffSetNextRecord(miff, key, miffValueTypeR4, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue4(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSet
******************************************************************************/
void miffSetR8(Miff * const miff, MiffC const * const key, MiffR8 const value)
{
   Miff8 vtemp;

   vtemp.r = value;

   miffSetNextRecord(miff, key, miffValueTypeR8, miffArrayFlagIS_SINGLE, 1, miffCompressFlagIS_UNCOMPRESSED, 0);
   miffSetValue8(miff, &vtemp);
   _WriteEndRecord(miff, miff->mode);
}

/******************************************************************************
func: miffSetKey
******************************************************************************/
void miffSetNextRecord(Miff * const miff, MiffC const * const key, MiffValueType const type,
   MiffArrayFlag const arrayFlag, MiffN4 const arrayCount, MiffCompressFlag const compressFlag, 
   MiffN4 const chunkByteCount)
{
   memClearTypeArray(256, MiffC, miff->keyC);
   memCopyTypeArray( 255, MiffC, miff->keyC, key);
   miff->keyCCount = (MiffN1) wcsnlen_s(key, 255);

   _MiffCToUTFKey(miff->keyCCount, miff->keyC, &miff->keyByteCount, miff->key);
   miff->valueType               = type;
   miff->arrayFlag               = arrayFlag;
   miff->arrayCount              = arrayCount;
   miff->compressFlag            = compressFlag;
   miff->compressChunkByteCount  = chunkByteCount;

   _WriteKey(        miff, miff->mode);
   _WriteValueHeader(miff, miff->mode, type, arrayFlag, arrayCount, compressFlag, chunkByteCount);
}

/******************************************************************************
func: miffSetValue1
******************************************************************************/
void miffSetValue1(Miff * const miff, Miff1 * const value)
{
   _WriteValue1(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue2
******************************************************************************/
void miffSetValue2(Miff * const miff, Miff2 * const value)
{
   _WriteValue2(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue3
******************************************************************************/
void miffSetValue3(Miff * const miff, Miff3 * const value)
{
   miff; value;
}

/******************************************************************************
func: miffSetValue4
******************************************************************************/
void miffSetValue4(Miff * const miff, Miff4 * const value)
{
   _WriteValue4(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue8
******************************************************************************/
void miffSetValue8(Miff * const miff, Miff8 * const value)
{
   _WriteValue8(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue16
******************************************************************************/
void miffSetValue16(Miff * const miff, Miff16 * const value)
{
   miff; value;
}

/******************************************************************************
func: miffSetValue32
******************************************************************************/
void miffSetValue32(Miff * const miff, Miff32 * const value)
{
   miff; value;
}

/******************************************************************************
func: miffSetValue64
******************************************************************************/
void miffSetValue64(Miff * const miff, Miff64 * const value)
{
   miff; value;
}

/******************************************************************************
func: miffSetValue128
******************************************************************************/
void miffSetValue128(Miff * const miff, Miff128 * const value)
{
   miff; value;
}

/******************************************************************************
func: miffSetValue256
******************************************************************************/
void miffSetValue256(Miff * const miff, Miff256 * const value)
{
   miff; value;
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool miffStart(MiffMemCreate const memCreate, MiffMemDestroy const memDestroy,
   MiffMemCompress const memCompress, MiffMemDecompress const memDecompress)
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
func: miffUserTypeAddVar
******************************************************************************/
MiffBool miffUserTypeAddVar(Miff * const miff, MiffC const * const name, MiffValueType const type, MiffN4 const arrayCount)
{
   MiffTypeRecord *newType;

   returnFalseIf(
      !_isStarted                 ||
      !name                       ||
      !miff                       ||
      !miff->userTypeCurr == 0 ||
      type < 0 || miff->userTypeCount + miffValueTypeFIRST_USER_TYPE < type);

   newType = memCreateType(MiffTypeRecord);
   returnFalseIf(!newType);

   memCopyTypeArray(255, MiffC, newType->nameC, name);
   newType->nameCCount = (MiffN1) wcsnlen_s(newType->nameC, 255);
   _MiffCToUTFKey(newType->nameCCount, newType->nameC, &newType->nameByteCount, newType->name);

   newType->type       = type;
   newType->arrayCount = arrayCount;

   miff->userTypeList[miff->userTypeCurr].varCount++;

   // Add the new type to the user type.
   if (miff->userTypeList[miff->userTypeCurr].varStart == NULL)
   {
      miff->userTypeList[miff->userTypeCurr].varStart  =
         miff->userTypeList[miff->userTypeCurr].varEnd = newType;
   }
   else
   {
      miff->userTypeList[miff->userTypeCurr].varEnd->next = newType;
      miff->userTypeList[miff->userTypeCurr].varEnd       = newType;
   }

   returnTrue;
}

/******************************************************************************
func: miffUserTypeStart
******************************************************************************/
MiffValueType miffUserTypeStart(Miff * const miff, MiffC const * const name)
{
   int             index;
   MiffTypeRecord *currType,
                  *nextType;

   returnIf(
         !_isStarted ||
         !miff       ||
         !name,
      (MiffValueType) 0);

   // Find if the user type already exists.
   forCount(index, miff->userTypeCount)
   {
      // Check if a user type exists.
      if (_MemIsEqual(
            miff->userTypeList[index + miffValueTypeFIRST_USER_TYPE].nameCCount * sizeof(MiffC),
            (MiffN1 *) miff->userTypeList[index + miffValueTypeFIRST_USER_TYPE].nameC,
            (MiffN4) (wcsnlen_s(name, 255) * sizeof(MiffC)),
            (MiffN1 *) name))
      {
         // We are resetting that user type.
         miff->userTypeCurr = (MiffN2) (index + miffValueTypeFIRST_USER_TYPE);

         // Clean out the current type values.
         currType = miff->userTypeList[miff->userTypeCurr].varStart;
         nextType = currType->next;
         while (currType)
         {
            _memDestroy(currType);
            currType = nextType;
            nextType = currType->next;
         }

         miff->userTypeList[miff->userTypeCurr].varCount  = 0;
         miff->userTypeList[miff->userTypeCurr].varStart  = 
            miff->userTypeList[miff->userTypeCurr].varEnd = NULL;

         return miff->userTypeCurr;
      }
   }

   // Add a new user type to the user types.
   miff->userTypeCurr = miff->userTypeCount++;
   return miff->userTypeCurr;
}

/******************************************************************************
func: miffUserTypeStop
******************************************************************************/
void miffUserTypeStop(Miff * const miff)
{
   returnVoidIf(
      !_isStarted ||
      !miff);

   miff->userTypeCurr = 0;
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
func: _MiffCClone
******************************************************************************/
static MiffC *_MiffCClone(MiffN4 const strCount, MiffC const * const str)
{
   MiffC *result;

   result = memCreateTypeArray(strCount, MiffC);
   returnNullIf(!result);

   memCopyTypeArray(strCount, MiffC, result, str);

   return result;
}

/******************************************************************************
func: _MiffCLetterToUTFLetter
******************************************************************************/
static MiffN4 _MiffCLetterToUTFLetter(MiffN4 const letter, MiffN1 * const a, MiffN1 * const b,
   MiffN1 * const c, MiffN1 * const d)
{
   return0If(!a || !b || !c || !d);

   if (letter < 0x0000007f)
   {
      *d = 0;
      *c = 0;
      *b = 0;
      *a = (MiffN1) letter;

      return 1;
   }

   if (letter < 0x000007ff)
   {
      *d = 0;
      *c = 0;
      *b = (MiffN1) (0x80 |  (letter       & 0x3f));
      *a = (MiffN1) (0xc0 | ((letter >> 6) & 0x1f));

      return 2;
   }

   if (letter < 0x0000ffff)
   {
      *d = 0;
      *c = (MiffN1) (0x80 |  (letter        & 0x3f));
      *b = (MiffN1) (0x80 | ((letter >>  6) & 0x3f));
      *a = (MiffN1) (0xe0 | ((letter >> 12) & 0x0f));

      return 3;
   }

   // letter < 0x001fffff
   *d = (MiffN1) (0x80 |  (letter        & 0x3f));
   *c = (MiffN1) (0x80 | ((letter >>  6) & 0x3f));
   *b = (MiffN1) (0x80 | ((letter >> 12) & 0x3f));
   *c = (MiffN1) (0xf0 | ((letter >> 18) & 0x07));

   return 4;
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
   returnFalseIf(!result);

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

      result[utfIndex++] = d;
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

   *utfByteCount = (MiffN1) utfIndex;

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

      returnFalseIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &_base64Byte));

      // [111111..]        [111111]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 2);

      returnFalseIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &_base64Byte));

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

      returnFalseIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &_base64Byte));

      // [xxxx1111]        [1111..]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] >> 2);

      // Remainder [....22]
      _base64State = 2;
      break;

   case 2:
      // byte to fill      Incoming base64 byte
      // [........]        [....xx]

      // [xx......]        [....xx]
      byte->n |= (MiffN1) (_base64LetterToValue[_base64Byte] << 6);

      returnFalseIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &_base64Byte));

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
      breakIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &letter));
      breakIf(
         !(letter == ' '  ||
           letter == '\t' || 
           letter == '\n'));
   }

   return letter;
}

/******************************************************************************
func: _ReadKey
******************************************************************************/
static MiffBool _ReadKey(Miff * const miff, MiffMode const mode)
{
   MiffN4 ntemp;

   miff->keyByteCount = 0;
   miff->keyCCount    = 0;
   miff->key[0]       = 0;
   miff->keyC[0]      = 0;

   if (mode == miffModeTEXT)
   {
      returnFalseIf(!_ReadPart(miff, 255, &ntemp, miff->key));
      miff->keyByteCount = (MiffN1) ntemp;
   }
   else
   {
      MiffN1 n1;

      // Get the key byte count.
      returnFalseIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &n1));
      returnFalseIf(n1 == 0);

      // Get the key.
      miff->keyByteCount = n1;
      returnFalseIf(!miff->getBuffer1(miff->dataRepo, miff->keyByteCount, (Miff1 *) miff->key));
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
   MiffN1    letter;

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

      breakIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &letter));
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
   MiffN4    byteCount,
             byteCountMax;
   MiffN1   *buffer,
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

      breakIf(!miff->getBuffer1(miff->dataRepo, 1, (Miff1 *) &letter));
      breakIf(
         letter == ' '  ||
         letter == '\t' ||
         letter == '\n');

      // Resize the buffer when we hit the limit.
      if (byteCount == byteCountMax)
      {
         byteCountMax += byteCountMax / 2;
         btemp = memCreateTypeArray(byteCountMax, MiffN1);
         if (!btemp)
         {
            // Clean up on failure.
            memDestroy(buffer);
            returnFalse;
         }

         memCopyTypeArray(byteCount, MiffN1, buffer, btemp);
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
static MiffBool _ReadValueHeader(Miff * const miff, MiffMode const mode)
{
   if (mode == miffModeTEXT)
   {
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
      if      (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "[]"))         miff->valueType = miffValueTypeKEY_VALUE_BLOCK;
      else if (_MemIsEqual(typeByteCount, type, 5, (MiffN1 *) "[...]"))      miff->valueType = miffValueTypeVALUE_STREAM_BLOCK;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "..."))        miff->valueType = miffValueTypeBINARY_DATA;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "file"))       miff->valueType = miffValueTypeEMBEDDED_FILE;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "type"))       miff->valueType = miffValueTypeTYPE;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "\"\""))       miff->valueType = miffValueTypeSTRING;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "path"))       miff->valueType = miffValueTypePATH;
      else if (_MemIsEqual(typeByteCount, type, 8, (MiffN1 *) "userType"))   miff->valueType = miffValueTypeUSER_TYPE;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i1"))         miff->valueType = miffValueTypeI1;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i2"))         miff->valueType = miffValueTypeI2;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i3"))         miff->valueType = miffValueTypeI3;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i4"))         miff->valueType = miffValueTypeI4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i8"))         miff->valueType = miffValueTypeI8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i16"))        miff->valueType = miffValueTypeI16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i32"))        miff->valueType = miffValueTypeI32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i64"))        miff->valueType = miffValueTypeI64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "i128"))       miff->valueType = miffValueTypeI128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "i256"))       miff->valueType = miffValueTypeI256;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n1"))         miff->valueType = miffValueTypeN1;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n2"))         miff->valueType = miffValueTypeN2;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n3"))         miff->valueType = miffValueTypeN3;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n4"))         miff->valueType = miffValueTypeN4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n8"))         miff->valueType = miffValueTypeN8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n16"))        miff->valueType = miffValueTypeN16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n32"))        miff->valueType = miffValueTypeN32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n64"))        miff->valueType = miffValueTypeN64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "n128"))       miff->valueType = miffValueTypeN128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "n256"))       miff->valueType = miffValueTypeN256;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "r4"))         miff->valueType = miffValueTypeR4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "r8"))         miff->valueType = miffValueTypeR8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r16"))        miff->valueType = miffValueTypeR16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r32"))        miff->valueType = miffValueTypeR32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r64"))        miff->valueType = miffValueTypeR64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "r128"))       miff->valueType = miffValueTypeR128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "r256"))       miff->valueType = miffValueTypeR256;
      else 
      {
         // User type
         //miff->valueType = _FindUserType(typeByteCount, type);
      }

      // Read in the array count
      if (miff->arrayFlag == miffArrayFlagIS_ARRAY)
      {
         returnFalseIf(!_ReadValue4(miff, mode, miffValueTypeN4, (Miff4 *) &miff->arrayCount));
      }
   }
   else
   {
      Miff2 value2;

      returnFalseIf(!miff->getBuffer2(miff->dataRepo, 2, &value2));
      miff->compressFlag = (value2.n >> 14) & 0x0003;
      miff->arrayFlag    = (value2.n >> 12) & 0x0003;
      miff->valueType    =  value2.n        & 0x0FFF;

      // Read in the array count
      if (miff->arrayFlag == miffArrayFlagIS_ARRAY)
      {
         returnFalseIf(!miff->getBuffer4(miff->dataRepo, 4, (Miff4 *) &miff->arrayCount));
      }
   }

   returnTrue;
}

/******************************************************************************
func: _ReadValue1
******************************************************************************/
static MiffBool _ReadValue1(Miff * const miff, MiffMode const mode, 
   MiffValueType const type, Miff1 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer1(miff->dataRepo, 1, value));
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
static MiffBool _ReadValue2(Miff * const miff, MiffMode const mode, 
   MiffValueType const type, Miff2 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer2(miff->dataRepo, 1, value));
      _ByteSwap2(miff, value);
   }

   if (type == miffValueTypeR2)
   {
      if (miff->isByteSwapping)
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[1]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[0]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[1]));
      }

      returnTrue;
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

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
static MiffBool _ReadValue4(Miff * const miff, MiffMode const mode, 
   MiffValueType const type, Miff4 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   // Binary read value.
   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer4(miff->dataRepo, 1, value));
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
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[3]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[2]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[1]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[0]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[1]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[2]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[3]));
      }

      returnTrue;
   }

   // read the value part.
   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

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
static MiffBool _ReadValue8(Miff * const miff, MiffMode const mode, 
   MiffValueType const type, Miff8 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer8(miff->dataRepo, 1, value));
      _ByteSwap8(miff, value);
   }

   if (type == miffValueTypeR4)
   {
      if (miff->isByteSwapping)
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[7]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[6]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[5]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[4]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[3]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[2]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[1]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[0]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[1]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[2]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[3]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[4]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[5]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[6]) ||
            !_ReadBase64Byte(miff, (Miff1 *) &value->byte[7]));
      }

      returnTrue;
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

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
      return -((MiffI8) value);
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
   *strCount = (MiffN1) rindex;
}

/******************************************************************************
func: _WriteEndRecord
******************************************************************************/
static void _WriteEndRecord(Miff * const miff, MiffMode const mode)
{
   if (mode == miffModeTEXT)
   {
      miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "\n");
   }
}

/******************************************************************************
func: _WriteKey
******************************************************************************/
static void _WriteKey(Miff * const miff, MiffMode const mode)
{
   if (mode == miffModeTEXT)
   {
      miff->setBuffer1(miff->dataRepo, miff->keyByteCount, (Miff1 *) miff->key);
      miff->setBuffer1(miff->dataRepo, 1,                  (Miff1 *) " ");
   }
   else
   {
      miff->setBuffer1(miff->dataRepo, 1,                  (Miff1 *) &miff->keyByteCount);
      miff->setBuffer1(miff->dataRepo, miff->keyByteCount, (Miff1 *) miff->key);
   }
}

/******************************************************************************
func: _WriteValueHeader
******************************************************************************/
static void _WriteValueHeader(Miff * const miff, MiffMode const mode, MiffValueType const type,
   MiffArrayFlag const arrayFlag, MiffN4 const arrayCount, MiffCompressFlag const compressFlag,
   MiffN4 const compressChunkByteCount)
{
   if (mode == miffModeTEXT)
   {
      switch (type)
      {
      case miffValueTypeKEY_VALUE_BLOCK:     miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "[]");         break;
      case miffValueTypeVALUE_STREAM_BLOCK:  miff->setBuffer1(miff->dataRepo, 5, (Miff1 *) "[...]");      break;
      case miffValueTypeBINARY_DATA:         miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "...");        break;
      case miffValueTypeEMBEDDED_FILE:       miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "file");       break;
      case miffValueTypeTYPE:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "type");       break;
      case miffValueTypeSTRING:              miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "\"\"");       break;
      case miffValueTypePATH:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "path");       break;
      case miffValueTypeUSER_TYPE:           miff->setBuffer1(miff->dataRepo, 8, (Miff1 *) "userType");   break;
      case miffValueTypeI1:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "i1");         break;
      case miffValueTypeI2:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "i2");         break;
      case miffValueTypeI3:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "i3");         break;
      case miffValueTypeI4:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "i4");         break;
      case miffValueTypeI8:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "i8");         break;
      case miffValueTypeI16:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "i16");        break;
      case miffValueTypeI32:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "i32");        break;
      case miffValueTypeI64:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "i64");        break;
      case miffValueTypeI128:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "i128");       break;
      case miffValueTypeI256:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "i256");       break;
      case miffValueTypeN1:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "n1");         break;
      case miffValueTypeN2:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "n2");         break;
      case miffValueTypeN3:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "n3");         break;
      case miffValueTypeN4:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "n4");         break;
      case miffValueTypeN8:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "n8");         break;
      case miffValueTypeN16:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "n16");        break;
      case miffValueTypeN32:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "n32");        break;
      case miffValueTypeN64:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "n64");        break;
      case miffValueTypeN128:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "n128");       break;
      case miffValueTypeN256:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "n256");       break;
      case miffValueTypeR4:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "r4");         break;
      case miffValueTypeR8:                  miff->setBuffer1(miff->dataRepo, 2, (Miff1 *) "r8");         break;
      case miffValueTypeR16:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "r16");        break;
      case miffValueTypeR32:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "r32");        break;
      case miffValueTypeR64:                 miff->setBuffer1(miff->dataRepo, 3, (Miff1 *) "r64");        break;
      case miffValueTypeR128:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "r128");       break;
      case miffValueTypeR256:                miff->setBuffer1(miff->dataRepo, 4, (Miff1 *) "r256");       break;

      default:
         // user type.
         miff->setBuffer1(
            miff->dataRepo, 
            miff->userTypeList[type - miffValueTypeFIRST_USER_TYPE].nameByteCount, 
            (Miff1 *) miff->userTypeList[type - miffValueTypeFIRST_USER_TYPE].name);
      }

      if (arrayFlag == miffArrayFlagIS_SINGLE)
      {
         if      (compressFlag == miffCompressFlagIS_UNCOMPRESSED)
         {
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "-");
         }
         else if (compressFlag == miffCompressFlagIS_COMPRESSED)
         {
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "z");
         }
         else if (compressFlag == miffCompressFlagIS_CHUNKED_AND_COMPRESSED)
         {
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "c");
         }
         miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) " ");

         if (compressFlag == miffCompressFlagIS_CHUNKED_AND_COMPRESSED)
         {
            _WriteValue4(miff, mode, miffValueTypeN4, (Miff4 *) &compressChunkByteCount);
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) " ");
         }
      }
      else 
      {
         if      (compressFlag == miffCompressFlagIS_UNCOMPRESSED)
         {
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "=");
         }
         else if (compressFlag == miffCompressFlagIS_COMPRESSED)
         {
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "Z");
         }
         else if (compressFlag == miffCompressFlagIS_CHUNKED_AND_COMPRESSED)
         {
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "C");
         }
         miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) " ");

         if (arrayFlag == miffArrayFlagIS_ARRAY)
         {
            _WriteValue4(miff, mode, miffValueTypeN4, (Miff4 *) &arrayCount);
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) " ");
         }

         if (compressFlag == miffCompressFlagIS_CHUNKED_AND_COMPRESSED)
         {
            _WriteValue4(miff, mode, miffValueTypeN4, (Miff4 *) &compressChunkByteCount);
            miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) " ");
         }
      }
   }
   else
   {
      Miff2 value2;
      Miff4 value4;

      value2.n = (MiffN2) (
         (compressFlag << 14) |
         (arrayFlag    << 12) |
          type);
      _ByteSwap2(miff, &value2);
      miff->setBuffer2(miff->dataRepo, 1, &value2);

      if (arrayFlag == miffArrayFlagIS_ARRAY)
      {
         value4.n = arrayCount;
         _ByteSwap4(miff, &value4);
         miff->setBuffer4(miff->dataRepo, 1, &value4);
      }

      if (compressFlag == miffCompressFlagIS_CHUNKED_AND_COMPRESSED)
      {
         value4.n = arrayCount;
         _ByteSwap4(miff, &value4);
         miff->setBuffer4(miff->dataRepo, 1, &value4);
      }
   }
}


/******************************************************************************
func: _WriteValue1
******************************************************************************/
static void _WriteValue1(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff1 * const value)
{
   if (mode == miffModeBINARY)
   {
      miff->setBuffer1(miff->dataRepo, 1, value);
      return;
   }

   if (type == miffValueTypeI1)
   {
      if (value->i < 0)
      {
         miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "-");
         value->n = -value->i;
      }
   }
   
   _WriteValueNText(miff, value->n);
}

/******************************************************************************
func: _WriteValue2
******************************************************************************/
static void _WriteValue2(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff2 * const value)
{
   // dump the 8 byte value.
   if (mode == miffModeBINARY)
   {
      // Swap if necessary
      _ByteSwap2(miff, value);

      miff->setBuffer2(miff->dataRepo, 1, value);
      return;
   }

   if (type == miffValueTypeR2)
   {
      //TODO
   }

   if (type == miffValueTypeI2)
   {
      if (value->i < 0)
      {
         miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "-");
         value->n = -value->i;
      }
   }
   
   _WriteValueNText(miff, value->n);
}

/******************************************************************************
func: _WriteValue4
******************************************************************************/
static void _WriteValue4(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff4 * const value)
{
   // dump the 8 byte value.
   if (mode == miffModeBINARY)
   {
      // Swap if necessary
      _ByteSwap4(miff, value);

      miff->setBuffer4(miff->dataRepo, 1, value);
      return;
   }

   if (type == miffValueTypeR4)
   {
      //TODO
   }

   if (type == miffValueTypeI4)
   {
      if (value->i < 0)
      {
         miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "-");
         value->n = -value->i;
      }
   }
   
   _WriteValueNText(miff, value->n);
}

/******************************************************************************
func: _WriteValue8
******************************************************************************/
static void _WriteValue8(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff8 * const value)
{
   // dump the 8 byte value.
   if (mode == miffModeBINARY)
   {
      // Swap if necessary
      _ByteSwap8(miff, value);

      miff->setBuffer8(miff->dataRepo, 1, value);
      return;
   }

   if (type == miffValueTypeR8)
   {
      //TODO
   }

   if (type == miffValueTypeI8)
   {
      if (value->i < 0)
      {
         miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) "-");
         value->n = -value->i;
      }
   }
   
   _WriteValueNText(miff, value->n);
}

/******************************************************************************
func: _WriteValueNText
******************************************************************************/
static void _WriteValueNText(Miff * const miff, MiffN8 const value)
{
   int    index,
          count,
          digit;
   MiffN8 temp;
   MiffN1 string[32];

   temp = value;
   for (index = 0; ; index++)
   {
      digit = temp % 10;
      temp  = temp / 10;

      string[index] = (MiffN1) ('0' + digit);

      breakIf(temp == 0);
   }

   count = index + 1;
   forCountDown (index, count)
   {
      miff->setBuffer1(miff->dataRepo, 1, (Miff1 *) &string[index]);
   }
}
