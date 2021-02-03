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
#define loop                                    for (;;)
#define forCount(INDEX, COUNT)                  for ((INDEX) = 0; (INDEX) < (COUNT); (INDEX)++)

#define breakIf(EXP)                            if (EXP) { break; }

#define returnFalse                             return miffBoolFALSE
#define returnNull                              return NULL
#define returnTrue                              return miffBoolTRUE
#define returnFalseIf(EXP)                      if (EXP) { returnFalse; }
#define returnNullIf( EXP)                      if (EXP) { returnNull;  }
#define returnTrueIf( EXP)                      if (EXP) { returnTrue;  }
#define returnVoidIf( EXP)                      if (EXP) { return;      }

#define memClearType(TYPE, MEM)                 memset((MEM), 0, sizeof(TYPE))
#define memCopyTypeArray(COUNT, TYPE, DST, SRC) memcpy((DST), (SRC), (COUNT) * sizeof(TYPE))
#define memCreateType(TYPE)                     (TYPE *) _memCreate(           sizeof(TYPE))
#define memCreateTypeArray(COUNT, TYPE)         (TYPE *) _memCreate( (COUNT) * sizeof(TYPE))
#define memDestroy(MEM)                                  _memDestroy(MEM)

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
static MiffBool _MemIsEqual(     MiffN4 const countA, MiffN1 const * const strA, MiffN4 const countB, MiffN1 const * const strB);

static MiffBool _ReadKey(        Miff * const miff, MiffBool const isRecordText);
static MiffBool _ReadValueHeader(Miff * const miff, MiffBool const isRecordText);
static MiffBool _ReadValue1(     Miff * const miff, MiffBool const isRecordText, Miff1 *value);
static MiffBool _ReadValue2(     Miff * const miff, MiffBool const isRecordText, Miff2 *value);
static MiffBool _ReadValue4(     Miff * const miff, MiffBool const isRecordText, Miff4 *value);
static MiffBool _ReadValue8(     Miff * const miff, MiffBool const isRecordText, Miff8 *value);

static MiffC   *_MiffCClone(     MiffN4 const count,  MiffC const * const str);
static MiffC   *_UTF8ToMiffC(    MiffN4 const count,  MiffN1 const * const utf8, MiffC * const str);

static void     _MiffCToUTF8Key( MiffN4 const count,  MiffC const * const str, MiffN1 * const key);
static MiffN1  *_MiffCToUTF8(    MiffN4 const count,  MiffC const * const str);

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
   _UTF8ToMiffC(miff->keyByteCount, miff->key, miff->subFormatName);
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
   MiffSetBuffer8 setBuffer8Func, MiffBool const isBinary, MiffN4 const subFormatNameLen, 
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
   miff->method            = miffMethodWRITING;
   miff->isBinary          = isBinary;
   miff->isByteSwapping    = isByteSwaping;
   miff->setBuffer1        = setBuffer1Func;
   miff->setBuffer2        = setBuffer2Func;
   miff->setBuffer4        = setBuffer4Func;
   miff->setBuffer8        = setBuffer8Func;
   miff->subFormatVersion  = subFormatVersion;
   _MiffCToUTF8Key(subFormatNameLen, subFormatName, miff->subFormatName);

   // Write the miff header.
   returnFalseIf(!(
      _writeKey(            miff, miffBoolFALSE, (isBinary) ? L"MIFF_BIN" : L"MIFF_TXT")                                      &&
      _writeValueHeaderText(miff, miffBoolFALSE, miffValueTypeN8, miffArrayFlagIS_SINGLE, miffCompressFlagIS_UNCOMPRESSED, 0) &&
      _writeValueN8(        miff, miffBoolFALSE, 1)                                                                           &&
      
      _writeKeyText(        miff, miffBoolFALSE, miff->subFormatName)                                                         &&
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
}

/******************************************************************************
func: miffGetValue1
******************************************************************************/
Miff1 miffGetValue1(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue2
******************************************************************************/
Miff2 miffGetValue2(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue3
******************************************************************************/
Miff3 miffGetValue3(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue4
******************************************************************************/
Miff4 miffGetValue4(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue8
******************************************************************************/
Miff8 miffGetValue8(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue16
******************************************************************************/
Miff16 miffGetValue16(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue32
******************************************************************************/
Miff32 miffGetValue32(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue64
******************************************************************************/
Miff64 miffGetValue64(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue128
******************************************************************************/
Miff128 miffGetValue128(Miff * const miff)
{
}

/******************************************************************************
func: miffGetValue256
******************************************************************************/
Miff256 miffGetValue256(Miff * const miff)
{
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
func: _ReadKey
******************************************************************************/
static MiffBool _ReadKey(Miff * const miff, MiffBool const isRecordText)
{
   MiffN4 keyByteCount;

   if (isRecordText)
   {
      MiffN1 letter;

      // Skip whitespace.
      loop
      {
         returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
         breakIf(
            !(letter == ' ' ||
              letter == '\t');
      }

      // Get the key
      keyByteCount = 0;
      miff->key[keyByteCount++] = letter;

      loop
      {
         breakIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
         breakIf(
            letter == ' ' ||
            letter == '\t');
         miff->key[keyByteCount++] = letter;
      }
   }
   else
   {
      MiffN1 n1;

      // Get the key byte count
      returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, &n1));
      keyByteCount = n1;
      returnFalseIf(keyByteCount == 0);
      returnFalseIf(!miff->getBuffer1(miff->dataRepository, keyByteCount, miff->key));
   }

   // Convert the key to MiffC
   memDestroy(miff->keyC);
   _UTF8ToMiffC(keyByteCount, miff->key, miff->keyC);

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
      MiffN1 type[256];

      // Skip whitespace.
      loop
      {
         returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
         breakIf(
            !(letter == ' ' ||
              letter == '\t');
      }

      // Get the key
      typeByteCount = 0;
      type[typeByteCount++] = letter;

      loop
      {
         breakIf(!miff->getBuffer1(miff->dataRepository, 1, &letter));
         breakIf(
            letter == ' ' ||
            letter == '\t');
         type[typeByteCount++] = letter;
      }

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
   }
   else
   {
      MiffN2 n2;

      returnFalseIf(!miff->getBuffer2(miff->dataRepository, 2, &n2));
      miff->arrayFlag    = (n2 & 0x3000) >> 12;
      miff->compressFlag = (n2 & 0xC000) >> 12;
      miff->valueType    =  n2 & 0x0FFF;
   }

   returnTrue;
}

/******************************************************************************
func: _ReadValue1
******************************************************************************/
static MiffBool _ReadValue1(Miff * const miff, MiffBool const isRecordText, Miff1 * const value)
{
   returnFalseIf(!miff->getBuffer1(miff->dataRepository, 1, value));

   returnTrue;
}

/******************************************************************************
func: _ReadValue2
******************************************************************************/
static MiffBool _ReadValue2(Miff * const miff, MiffBool const isRecordText, Miff2 * const value)
{
   returnFalseIf(!miff->getBuffer2(miff->dataRepository, 1, value));

   returnTrue;
}

/******************************************************************************
func: _ReadValue4
******************************************************************************/
static MiffBool _ReadValue4(Miff * const miff, MiffBool const isRecordText, Miff4 * const value)
{
   returnFalseIf(!miff->getBuffer4(miff->dataRepository, 1, value));

   returnTrue;
}

/******************************************************************************
func: _ReadValue8
******************************************************************************/
static MiffBool _ReadValue8(Miff * const miff, MiffBool const isRecordText, Miff8 * const value)
{
   returnFalseIf(!miff->getBuffer8(miff->dataRepository, 1, value));

   returnTrue;
}

/******************************************************************************
func: _MiffCClone
******************************************************************************/
static MiffC *_MiffCClone(MiffN4 const count, MiffC const * const str)
{
   MiffC *result;

   result = memCreateTypeArray(count, MiffC);
   returnNullIf(!result);

   memCopyTypeArray(count - 1, MiffC, result, src);

   return result;
}

/******************************************************************************
func: _UTF8ToMiffC
******************************************************************************/
static void _UTF8ToMiffC(MiffN4 const count, MiffN1 const * const utf8, MiffC * const str)
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
   str[rindex] = 0;
}

/******************************************************************************
func: _MiffCToUTF8Key
******************************************************************************/
static void _MiffCToUTF8Key(MiffN4 const count, MiffC const * const str, MiffN1 * const key)
{

}

/******************************************************************************
func: _MiffCToUTF8
******************************************************************************/
static MiffN1 *_MiffCToUTF8(MiffN4 const count, MiffC const * const str)
{
}
