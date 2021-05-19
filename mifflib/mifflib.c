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
#include "miffLib.h"
#include "local.h"

/******************************************************************************
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static MiffBool             _isStarted                = miffBoolFALSE;

/******************************************************************************
prototype:
******************************************************************************/

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, 
   MiffMode const mode, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, 
   void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure
   miff = _MemCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the structure
   if (!miffCreateWriterContent(
         miff, 
         isByteSwaping, 
         setBufferFunc, 
         mode, 
         subFormatName, 
         subFormatVersion, 
         dataRepo))
   {
      _MemDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateWriterContent
******************************************************************************/
MiffBool miffCreateWriterContent(Miff * const miff, MiffBool const isByteSwaping, 
   MiffSetBuffer setBufferFunc, MiffMode const mode, MiffC2 const * const subFormatName, 
   MiffN8 const subFormatVersion, void * const dataRepo)
{
   MiffN4 count;

   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !setBufferFunc  ||
      !subFormatName  ||
      !subFormatVersion);

   _MemClearType(Miff, miff);
   miff->method               = miffMethodWRITING;
   miff->mode                 = mode;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->setBuffer            = setBufferFunc;
   miff->subFormatVersion     = subFormatVersion;
   
   count = min(255, _C2GetCount(subFormatName));
   _MemCopyTypeArray(count, MiffC2, miff->subFormatNameC2, subFormatName);

   // Write the miff header.
   _WriteTxtC1(         miff, (MiffC1 *) "MIFF");
   _WriteTxtRecordEnder(miff);
   _WriteTxtC1(         miff, (MiffC1 *) "1");
   _WriteTxtRecordEnder(miff);
   if (mode == miffModeBINARY)
   {
      _WriteTxtC1(      miff, (MiffC1 *) "BIN");
   }
   else
   {
      _WriteTxtC1(      miff, (MiffC1 *) "TXT");
   }
   _WriteTxtRecordEnder(miff);

   _WriteTxtC2(         miff, miff->subFormatNameC2);
   _WriteTxtRecordEnder(miff);
   _WriteTxtValueN(     miff, miff->subFormatVersion);
   _WriteTxtRecordEnder(miff);

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

   //memDestroy(miff->mem);
   //memDestroy(miff->compressMem);
   _MemDestroy(miff);
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool miffStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc,
   MiffMemCompress const memCompressFunc, MiffMemDecompress const memDecompressFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes) 
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _MemStart(memCreateFunc, memDestroyFunc, memCompressFunc, memDecompressFunc);

   _isStarted     = miffBoolTRUE;

   _Base64Start();

   returnTrue;
}

/******************************************************************************
func: miffStop
******************************************************************************/
void miffStop(void)
{
   returnVoidIf(!_isStarted);

   _MemStop();

   _isStarted = miffBoolFALSE;
}

/******************************************************************************
func: miffSetRecordData
******************************************************************************/
MiffBool miffSetRecordData(Miff * const miff, MiffValueType const type, MiffC2 const * const key, 
   MiffN4 const count, MiffCompressFlag const compressFlag, MiffN4 const chunkByteCount)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !key        ||
      count == miffArrayCountUNKNOWN);

   // Copy the key.
   _MemClearTypeArray(256, MiffC2, miff->currentRecord.nameC2);
   _MemCopyTypeArray( 255, MiffC2, miff->currentRecord.nameC2, key);

   // Set the type and array.
   miff->currentRecord.type           = type;
   miff->currentRecord.arrayCount     = count;

   // Reset the compress flag.
   miff->currentRecord.compressFlag   = compressFlag;
   miff->currentRecord.chunkByteCount = chunkByteCount;

   // Set the current index of the next value.
   miff->currentIndex                 = 0;
   
   // Write out the record.
   returnFalseIf(!_WriteTxtRecordType(        miff, miff->currentRecord.type));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordKeyC2(       miff, miff->currentRecord.nameC2));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordArrayCount(  miff, miff->currentRecord.arrayCount));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordCompressFlag(miff, miff->currentRecord.compressFlag));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   if (miff->currentRecord.compressFlag == miffCompressFlagCHUNK_COMPRESS)
   {
      returnFalseIf(!_WriteTxtRecordChunkSize(miff, miff->currentRecord.chunkByteCount));
   }

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueBoolean
******************************************************************************/
MiffBool miffSetRecordValueBoolean(Miff * const miff, MiffBool const value)
{
   MiffC1 *c1;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeBOOLEAN);

   c1 = (MiffC1 *) (value ? "T" : "F");
   
   returnFalseIf(!_WriteTxtC1(miff, c1));
  
   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueI1
******************************************************************************/
MiffBool miffSetRecordValueI1(Miff * const miff, MiffI1 const value)
{
   Miff1 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeI1);

   vtemp.i = value;

   returnFalseIf(!_WriteTxtValue1(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueI2
******************************************************************************/
MiffBool miffSetRecordValueI2(Miff * const miff, MiffI2 const value)
{
   Miff2 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeI2);

   vtemp.i = value;

   returnFalseIf(!_WriteTxtValue2(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueI4
******************************************************************************/
MiffBool miffSetRecordValueI4(Miff * const miff, MiffI4 const value)
{
   Miff4 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeI4);

   vtemp.i = value;

   returnFalseIf(!_WriteTxtValue4(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueI8
******************************************************************************/
MiffBool miffSetRecordValueI8(Miff * const miff, MiffI8 const value)
{
   Miff8 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeI8);

   vtemp.i = value;

   returnFalseIf(!_WriteTxtValue8(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueN1
******************************************************************************/
MiffBool miffSetRecordValueN1(Miff * const miff, MiffN1 const value)
{
   Miff1 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeN1);

   vtemp.n = value;

   returnFalseIf(!_WriteTxtValue1(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueN2
******************************************************************************/
MiffBool miffSetRecordValueN2(Miff * const miff, MiffN2 const value)
{
   Miff2 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeN2);

   vtemp.n = value;

   returnFalseIf(!_WriteTxtValue2(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueN4
******************************************************************************/
MiffBool miffSetRecordValueN4(Miff * const miff, MiffN4 const value)
{
   Miff4 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeN4);

   vtemp.n = value;

   returnFalseIf(!_WriteTxtValue4(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueN8
******************************************************************************/
MiffBool miffSetRecordValueN8(Miff * const miff, MiffN8 const value)
{
   Miff8 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeN8);

   vtemp.n = value;

   returnFalseIf(!_WriteTxtValue8(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueR4
******************************************************************************/
MiffBool miffSetRecordValueR4(Miff * const miff, MiffR4 const value)
{
   Miff4 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeR4);

   vtemp.r = value;

   returnFalseIf(!_WriteTxtValue4(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueR8
******************************************************************************/
MiffBool miffSetRecordValueR8(Miff * const miff, MiffR8 const value)
{
   Miff8 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeR8);

   vtemp.r = value;

   returnFalseIf(!_WriteTxtValue8(miff, miff->currentRecord.type, vtemp));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueType
******************************************************************************/
MiffBool miffSetRecordValueType(Miff * const miff, MiffValueType const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type != miffValueTypeTYPE);

   returnFalseIf(!_WriteTxtValueType(miff, value));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}

/******************************************************************************
func: miffSetRecordValueC2
******************************************************************************/
MiffBool miffSetRecordValueC2(Miff * const miff, MiffC2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      miff->currentRecord.type != miffValueTypeSTRING);

   returnFalseIf(!_WriteTxtValueC2(miff, value));

   returnFalseIf(!_CurrentIndexInc(miff));

   returnTrue;
}


#if 0
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, 
   void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure.
   miff = memCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the miff structure.
   if (!miffCreateReaderContent(miff, isByteSwaping, getBufferFunc, dataRepo))
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
   MiffGetBuffer getBufferFunc, void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !getBufferFunc);

   memClearType(Miff, miff);
   miff->method         = miffMethodREADING;
   miff->isByteSwapping = isByteSwaping;
   miff->getBuffer      = getBufferFunc;
   miff->dataRepo       = dataRepo;

#pragma message("TODO Read the header.")

   returnTrue;
}

/******************************************************************************
func: miffGetFileFlagIsBinary
******************************************************************************/
MiffBool miffGetFileFlagIsBinary(Miff const * const miff, MiffBool * const mode)
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
MiffBool miffGetFileVersion(Miff const * const miff, MiffN8 * const version)
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
MiffBool miffGetFileVersionSubFormat(Miff const * const miff, MiffC2 const ** const subFormatName, 
   MiffN8 * const subFormatVersion)
{
   returnFalseIf(
      !_isStarted    ||
      !miff          ||
      !subFormatName ||
      !subFormatVersion);

   *subFormatName    = miff->subFormatNameC2;
   *subFormatVersion = miff->subFormatVersion;

   returnTrue;
}

/******************************************************************************
func: miffGetKey
******************************************************************************/
MiffBool miffGetKey(Miff const * const miff, MiffC2 const ** const key)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !key);

   *key = miff->keyC2;

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
MiffBool miffGetValueHeader(Miff const * const miff, MiffValueType * const type, 
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
MiffN4 miffGetArrayCount(Miff const * const miff)
{
   return0If(
      !_isStarted ||
      !miff);

   return miff->arrayCount;
}

/******************************************************************************
func: miffGetValue1
******************************************************************************/
Miff1 miffGetValue1(Miff const * const miff)
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
Miff2 miffGetValue2(Miff const * const miff)
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
Miff3 miffGetValue3(Miff const * const miff)
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
Miff4 miffGetValue4(Miff const * const miff)
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
Miff8 miffGetValue8(Miff const * const miff)
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
Miff16 miffGetValue16(Miff const * const miff)
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
Miff32 miffGetValue32(Miff const * const miff)
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
Miff64 miffGetValue64(Miff const * const miff)
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
Miff128 miffGetValue128(Miff const * const miff)
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
Miff256 miffGetValue256(Miff const * const miff)
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
func: miffSetValue1
******************************************************************************/
MiffBool miffSetValue1(Miff * const miff, Miff1 * const value)
{
   return _WriteValue1(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue2
******************************************************************************/
MiffBool miffSetValue2(Miff * const miff, Miff2 * const value)
{
   return _WriteValue2(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue3
******************************************************************************/
MiffBool miffSetValue3(Miff * const miff, Miff3 * const value)
{
   miff; value;
   returnFalse;
}

/******************************************************************************
func: miffSetValue4
******************************************************************************/
MiffBool miffSetValue4(Miff * const miff, Miff4 * const value)
{
   return _WriteValue4(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue8
******************************************************************************/
MiffBool miffSetValue8(Miff * const miff, Miff8 * const value)
{
   return _WriteValue8(miff, miff->mode, miff->valueType, value);
}

/******************************************************************************
func: miffSetValue16
******************************************************************************/
MiffBool miffSetValue16(Miff * const miff, Miff16 * const value)
{
   miff; value;
   returnFalse;
}

/******************************************************************************
func: miffSetValue32
******************************************************************************/
MiffBool miffSetValue32(Miff * const miff, Miff32 * const value)
{
   miff; value;
   returnFalse;
}

/******************************************************************************
func: miffSetValue64
******************************************************************************/
MiffBool miffSetValue64(Miff * const miff, Miff64 * const value)
{
   miff; value;
   returnFalse;
}

/******************************************************************************
func: miffSetValue128
******************************************************************************/
MiffBool miffSetValue128(Miff * const miff, Miff128 * const value)
{
   miff; value;
   returnFalse;
}

/******************************************************************************
func: miffSetValue256
******************************************************************************/
MiffBool miffSetValue256(Miff * const miff, Miff256 * const value)
{
   miff; value;
   returnFalse;
}

/******************************************************************************
func: miffUserTypeAddVar
******************************************************************************/
MiffBool miffUserTypeAddVar(Miff * const miff, MiffC2 const * const name, MiffValueType const type, MiffN4 const arrayCount)
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

   memCopyTypeArray(255, MiffC2, newType->nameC2, name);
   newType->nameC2Count = (MiffN1) wcsnlen_s(newType->nameC2, 255);
   _MiffC2ToC1Key(newType->nameC2Count, newType->nameC2, &newType->nameC1Count, newType->nameC1);

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
MiffValueType miffUserTypeStart(Miff * const miff, MiffC2 const * const name)
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
            miff->userTypeList[index + miffValueTypeFIRST_USER_TYPE].nameC2Count * sizeof(MiffC2),
            (MiffN1 *) miff->userTypeList[index + miffValueTypeFIRST_USER_TYPE].nameC2,
            (MiffN4) (wcsnlen_s(name, 255)                                       * sizeof(MiffC2)),
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
func: _ReadFirstLetter

Skip white space to get the first letter.
******************************************************************************/
static MiffN1 _ReadFirstLetter(Miff const * const miff)
{
   MiffN1 letter;

   letter = 0;
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Miff1 *) &letter));
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

   miff->keyC1Count = 0;
   miff->keyC2Count = 0;
   miff->keyC1[0]   = 0;
   miff->keyC2[0]   = 0;

   if (mode == miffModeTEXT)
   {
      returnFalseIf(!_ReadPart(miff, 255, &ntemp, miff->keyC1));
      miff->keyC1Count = (MiffN1) ntemp;
   }
   else
   {
      MiffN1 n1;

      // Get the key byte count.
      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, (Miff1 *) &n1));
      returnFalseIf(n1 == 0);

      // Get the key.
      miff->keyC1Count = n1;
      returnFalseIf(!miff->getBuffer(miff->dataRepo, miff->keyC1Count, (Miff1 *) miff->keyC1));
   }

   // Convert the key to MiffC2
   _MiffC1ToC2Key(miff->keyC1Count, miff->keyC1, &miff->keyC2Count, miff->keyC2);

   returnTrue;
}

/******************************************************************************
func: _ReadPart

Read a stream of non-space characters.
******************************************************************************/
static MiffBool _ReadPart(Miff const * const miff, MiffN4 const maxByteCount, MiffN4 * const partByteCount, MiffN1 * const part)
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

      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Miff1 *) &letter));
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
static MiffBool _ReadPartDynamic(Miff const * const miff, MiffN4 * const partByteCount, MiffN1 ** const part)
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

      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Miff1 *) &letter));
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
      if      (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "{"))      miff->valueType = miffValueTypeKEY_VALUE_BLOCK;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "*"))      miff->valueType = miffValueTypeBINARY_DATA_1;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "**"))     miff->valueType = miffValueTypeBINARY_DATA_2;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "***"))    miff->valueType = miffValueTypeBINARY_DATA_3;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "****"))   miff->valueType = miffValueTypeBINARY_DATA_4;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "[*]"))    miff->valueType = miffValueTypeEMBEDDED_FILE_1;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "[**]"))   miff->valueType = miffValueTypeEMBEDDED_FILE_2;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "[***]"))  miff->valueType = miffValueTypeEMBEDDED_FILE_3;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "[****]")) miff->valueType = miffValueTypeEMBEDDED_FILE_4;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "type"))   miff->valueType = miffValueTypeTYPE;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "\""))     miff->valueType = miffValueTypeSTRING;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "->"))     miff->valueType = miffValueTypePATH;
      else if (_MemIsEqual(typeByteCount, type, 8, (MiffN1 *) "define")) miff->valueType = miffValueTypeUSER_TYPE;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "bool"))   miff->valueType = miffValueTypeBOOLEAN;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i1"))     miff->valueType = miffValueTypeI1;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i2"))     miff->valueType = miffValueTypeI2;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i3"))     miff->valueType = miffValueTypeI3;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i4"))     miff->valueType = miffValueTypeI4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i8"))     miff->valueType = miffValueTypeI8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i16"))    miff->valueType = miffValueTypeI16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i32"))    miff->valueType = miffValueTypeI32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i64"))    miff->valueType = miffValueTypeI64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "i128"))   miff->valueType = miffValueTypeI128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "i256"))   miff->valueType = miffValueTypeI256;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n1"))     miff->valueType = miffValueTypeN1;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n2"))     miff->valueType = miffValueTypeN2;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n3"))     miff->valueType = miffValueTypeN3;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n4"))     miff->valueType = miffValueTypeN4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n8"))     miff->valueType = miffValueTypeN8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n16"))    miff->valueType = miffValueTypeN16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n32"))    miff->valueType = miffValueTypeN32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n64"))    miff->valueType = miffValueTypeN64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "n128"))   miff->valueType = miffValueTypeN128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "n256"))   miff->valueType = miffValueTypeN256;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "r4"))     miff->valueType = miffValueTypeR4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "r8"))     miff->valueType = miffValueTypeR8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r16"))    miff->valueType = miffValueTypeR16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r32"))    miff->valueType = miffValueTypeR32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r64"))    miff->valueType = miffValueTypeR64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "r128"))   miff->valueType = miffValueTypeR128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "r256"))   miff->valueType = miffValueTypeR256;
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

      returnFalseIf(!miff->getBuffer(miff->dataRepo, 2, (Miff1 *) &value2));
      miff->compressFlag = (value2.n >> 14) & 0x0003;
      miff->arrayFlag    = (value2.n >> 12) & 0x0003;
      miff->valueType    =  value2.n        & 0x0FFF;

      // Read in the array count
      if (miff->arrayFlag == miffArrayFlagIS_ARRAY)
      {
         returnFalseIf(!miff->getBuffer(miff->dataRepo, 4, (Miff1 *) &miff->arrayCount));
      }
   }

   returnTrue;
}

/******************************************************************************
func: _ReadValue1
******************************************************************************/
static MiffBool _ReadValue1(Miff const * const miff, MiffMode const mode, 
   MiffValueType const type, Miff1 * const value)
{
   MiffN4 partByteCount;
   MiffC1 part[64];
   
   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, value));
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

   if (type == miffValueTypeI1)
   {
      value->i = (MiffI1) _MiffC1ToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN1) _MiffC1ToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _ReadValue2
******************************************************************************/
static MiffBool _ReadValue2(Miff const * const miff, MiffMode const mode, 
   MiffValueType const type, Miff2 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];
   
   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer(miff->dataRepo, 2, (Miff1 *) value));
      _ByteSwap2(miff, value);
   }

   if (type == miffValueTypeR2)
   {
      if (miff->isByteSwapping)
      {
         returnFalseIf(
            !_Base64Get(miff, (Miff1 *) &value->byte[1]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_Base64Get(miff, (Miff1 *) &value->byte[0]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[1]));
      }

      returnTrue;
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

   if (type == miffValueTypeI2)
   {
      value->i = (MiffI2) _MiffC1ToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN2) _MiffC1ToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _ReadValue4
******************************************************************************/
static MiffBool _ReadValue4(Miff const * const miff, MiffMode const mode, 
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
            !_Base64Get(miff, (Miff1 *) &value->byte[3]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[2]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[1]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_Base64Get(miff, (Miff1 *) &value->byte[0]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[1]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[2]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[3]));
      }

      returnTrue;
   }

   // read the value part.
   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

   // Convert the value part to I or N.
   if (type == miffValueTypeI4)
   {
      value->i = (MiffI4) _MiffC1ToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN4) _MiffC1ToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _ReadValue8
******************************************************************************/
static MiffBool _ReadValue8(Miff const * const miff, MiffMode const mode, 
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
            !_Base64Get(miff, (Miff1 *) &value->byte[7]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[6]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[5]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[4]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[3]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[2]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[1]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[0]));
      }
      else
      {
         returnFalseIf(
            !_Base64Get(miff, (Miff1 *) &value->byte[0]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[1]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[2]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[3]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[4]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[5]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[6]) ||
            !_Base64Get(miff, (Miff1 *) &value->byte[7]));
      }

      returnTrue;
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

   if (type == miffValueTypeI8)
   {
      value->i = (MiffI8) _MiffC1ToI(partByteCount, part);
      memDestroy(part);

      returnTrue;
   }

   value->n = (MiffN8) _MiffC1ToN(partByteCount, part); 
   memDestroy(part);

   returnTrue;
}

/******************************************************************************
func: _WriteKey
******************************************************************************/
static MiffBool _WriteKey(Miff * const miff, MiffMode const mode)
{
   if (mode == miffModeTEXT)
   {
      returnFalseIf(!miff->setBuffer(miff->dataRepo, miff->keyByteCount, (Miff1 *) miff->key));
   }
   else
   {
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1,                  (Miff1 *) &miff->keyByteCount));
      returnFalseIf(!miff->setBuffer(miff->dataRepo, miff->keyByteCount, (Miff1 *) miff->key));
   }

   returnTrue;
}

/******************************************************************************
func: _WriteRecordSeparator
******************************************************************************/
static MiffBool _WriteRecordSeparator(Miff * const miff, MiffMode const mode)
{
   if (mode == miffModeTEXT)
   {
      return _WriteTxtRecordDivider(miff);
   }

   returnTrue;
}

/******************************************************************************
func: _WriteRecordStop
******************************************************************************/
static MiffBool _WriteRecordStop(Miff * const miff, MiffMode const mode)
{
   if (mode == miffModeTEXT)
   {
      return _WriteTxtRecordEnder(miff);
   }

   returnTrue;
}

/******************************************************************************
func: _WriteValue2
******************************************************************************/
static MiffBool _WriteValue2(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff2 * const value)
{
   // dump the 8 byte value.
   if (mode == miffModeBINARY)
   {
      // Swap if necessary
      _ByteSwap2(miff, value);

      return miff->setBuffer(miff->dataRepo, 2, value->byte);
   }

   return _WriteTxtValue2(miff, type, value);
}

/******************************************************************************
func: _WriteValue4
******************************************************************************/
static MiffBool _WriteValue4(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff4 * const value)
{
   // dump the 8 byte value.
   if (mode == miffModeBINARY)
   {
      // Swap if necessary
      _ByteSwap4(miff, value);

      return miff->setBuffer(miff->dataRepo, 4, value->byte);
   }

   return _WriteTxtValue4(miff, type, value);
}

/******************************************************************************
func: _WriteValue8
******************************************************************************/
static MiffBool _WriteValue8(Miff * const miff, MiffMode const mode, MiffValueType const type, Miff8 * const value)
{
   // dump the 8 byte value.
   if (mode == miffModeBINARY)
   {
      // Swap if necessary
      _ByteSwap8(miff, value);

      return miff->setBuffer(miff->dataRepo, 8, value->byte);
   }

   return _WriteTxtValue8(miff, type, value);
}
#endif
