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
static MiffBool    _isStarted = miffBoolFALSE;

/******************************************************************************
prototype:
******************************************************************************/

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc,
   MiffC2 * const subFormatName, MiffN8 * const subFormatVersion, void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure.
   miff = _MemCreateType(Miff);
   returnNullIf(!miff);

   // Initialize the miff structure.
   if (!miffCreateReaderContent(
         miff,
         isByteSwaping,
         getBufferFunc,
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
func: miffCreateReaderContent
******************************************************************************/
MiffBool miffCreateReaderContent(Miff * const miff, MiffBool const isByteSwaping,
   MiffGetBuffer getBufferFunc, MiffC2 * const subFormatName, MiffN8 * const subFormatVersion,
   void * const dataRepo)
{
   MiffN1 ntemp;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !getBufferFunc);

   _MemClearType(Miff, miff);
   miff->method               = miffMethodREADING;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->getBuffer            = getBufferFunc;
   miff->readByteCountActual  = 1024;
   miff->readByteCount        = 0;
   miff->readByteData         = _MemCreateTypeArray(1024, MiffN1);
   returnFalseIf(!miff->readByteData);

   // Read the header.
   returnFalseIf(!_ReadTxtLine(miff));
   returnFalseIf(!_MemIsEqual(miff->readByteCount, miff->readByteData, 4, (MiffN1 *) MIFF_HEADER_FILETYPE_STR));
   returnFalseIf(!_ReadTxtLine(miff));
   miff->version = _C1ToN(miff->readByteCount, (MiffC1 *) miff->readByteData);

   returnFalseIf(!_MemIsEqual(miff->readByteCount, miff->readByteData, 1, (MiffN1 *) MIFF_HEADER_VERSION_STR));
   returnFalseIf(!_ReadTxtLine(miff));
   if      (_MemIsEqual(miff->readByteCount, miff->readByteData, 3, (MiffN1 *) MIFF_HEADER_TXT_STR))
   {
      miff->mode = miffModeTEXT;
   }
   else if (_MemIsEqual(miff->readByteCount, miff->readByteData, 3, (MiffN1 *) MIFF_HEADER_BIN_STR))
   {
      miff->mode = miffModeBINARY;
   }
   else
   {
      returnFalse;
   }
   returnFalseIf(!_ReadTxtLine(miff));
   _C1ToC2Key(  miff->readByteCount, (MiffC1 *) miff->readByteData, &ntemp, miff->subFormatNameC2);
   _MemCopyTypeArray(miffKeyBYTE_COUNT, MiffC2, subFormatName, miff->subFormatNameC2);
   returnFalseIf(!_ReadTxtLine(miff));
   miff->subFormatVersion =
      *subFormatVersion   = _C1ToN(miff->readByteCount, miff->readByteData);

   returnTrue;
}

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
   miff->version              = 1;
   miff->method               = miffMethodWRITING;
   miff->mode                 = mode;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->setBuffer            = setBufferFunc;
   miff->subFormatVersion     = subFormatVersion;

   count = min(255, _C2GetCount(subFormatName));
   _MemCopyTypeArray(count, MiffC2, miff->subFormatNameC2, subFormatName);

   // Write the miff header.
   _WriteTxtC1(         miff, (MiffC1 *) MIFF_HEADER_FILETYPE_STR);
   _WriteTxtRecordEnder(miff);
   _WriteTxtC1(         miff, (MiffC1 *) MIFF_HEADER_VERSION_STR);
   _WriteTxtRecordEnder(miff);
   if (mode == miffModeBINARY)
   {
      _WriteTxtC1(      miff, (MiffC1 *) MIFF_HEADER_BIN_STR);
   }
   else
   {
      _WriteTxtC1(      miff, (MiffC1 *) MIFF_HEADER_TXT_STR);
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
func: miffRecordGetBegin

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffBool miffRecordGetBegin(Miff * const miff, MiffType * const type, MiffC2 * const key,
   MiffN4 * const count, MiffBool * const isCompressed, MiffN4 * const compressedChunkByteCount)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->readRecordIsDone = miffBoolFALSE;

   *type                     = miffTypeNONE;
   *count                    = 0;
   *isCompressed             = miffBoolFALSE;
   *compressedChunkByteCount = 0;

   // Clear the key.
   _MemClearTypeArray(miffKeySIZE, MiffC2, key);

   // Read in the type.
   returnFalseIf(_ReadTxtRecordType(miff, &miff->currentRecord.type));
   *type = miff->currentRecord.type;

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      return miff->readRecordIsDone;
   }

   // Read in the name of the record
   returnFalseIf(_ReadTxtRecordKeyC2(miff, miff->currentRecord.nameC2));
   _MemCopyTypeArray(miffKeyBYTE_COUNT, MiffC2, key, miff->currentRecord.nameC2);

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_START)
   {
      return miff->readRecordIsDone;
   }

   // Read in the array count
   returnFalseIf(_ReadTxtRecordArrayCount(miff, &miff->currentRecord.arrayCount));
   *count = miff->currentRecord.arrayCount;

   // Read in the compression flag
   returnFalseIf(_ReadTxtRecordCompressFlag(miff, &miff->currentRecord.isCompressed));
   *isCompressed = miff->currentRecord.isCompressed;

   // if Compressed...
   if (miff->currentRecord.isCompressed)
   {
      // Read in the compression chunk size.
      returnFalseIf(_ReadTxtRecordChunkSize(miff, &miff->currentRecord.compressedChunkByteCount));
      *compressedChunkByteCount = miff->currentRecord.compressedChunkByteCount;
   }

   returnTrue;
}

/******************************************************************************
func: miffRecordGetEnd
******************************************************************************/
MiffBool miffRecordGetEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnTrueIf(miff->readRecordIsDone);

   returnFalseIf(!_ReadTxtLineSkip(miff));

   miff->readRecordIsDone = miffBoolTRUE;

   returnTrue;
}

/******************************************************************************
func: miffRecordGetCount
******************************************************************************/
MiffN8 miffRecordGetCount(Miff * const miff)
{
   return0If(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type == miffTypeNONE);

   return miff->currentRecord.arrayCount;
}

/******************************************************************************
func: miffRecordGetName
******************************************************************************/
MiffC2 *miffRecordGetName(Miff * const miff)
{
   returnNullIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type == miffTypeNONE);

   return miff->currentRecord.nameC2;
}

/******************************************************************************
func: miffRecordGetType
******************************************************************************/
MiffType miffRecordGetType(Miff * const miff)
{
   returnIf(
         !_isStarted ||
         !miff,
      miffTypeNONE);

   return miff->currentRecord.type;
}

/******************************************************************************
func: miffRecordSetBegin
******************************************************************************/
MiffBool miffRecordSetBegin(Miff * const miff, MiffType const type, MiffC2 const * const key,
   MiffN4 const count, MiffBool const isCompressed, MiffN4 const compressedChunkByteCount)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Write the type for the record.  Common for all cases.
   returnFalseIf(!_WriteTxtRecordType(miff, type));

   // We are ending a key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      // TODO key value block stack pop.
      returnFalseIf(!_WriteTxtRecordEnder(miff));
      returnTrue;
   }

   returnFalseIf(!key);

   // Copy the key.
   miff->currentArrayIndex                      = 0;
   miff->currentRecord.type                     = type;
   miff->currentRecord.arrayCount               = count;
   miff->currentRecord.isCompressed             = isCompressed;
   miff->currentRecord.compressedChunkByteCount = compressedChunkByteCount;
   _MemClearTypeArray(miffKeySIZE,      MiffC2, miff->currentRecord.nameC2);
   _MemCopyTypeArray( _C2GetCount(key), MiffC2, miff->currentRecord.nameC2, key);

   // We are starting a new key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_START)
   {
      // TODO key value block stack push.
      returnFalseIf(!_WriteTxtRecordSeparator(miff));
      returnFalseIf(!_WriteTxtRecordKeyC2(    miff, miff->currentRecord.nameC2));

      return _WriteTxtRecordEnder(miff);
   }

   // Write out the record.
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordKeyC2(       miff, miff->currentRecord.nameC2));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordArrayCount(  miff, count));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordCompressFlag(miff, isCompressed));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   if (isCompressed)
   {
      returnFalseIf(!_WriteTxtRecordChunkSize(miff, compressedChunkByteCount));
      returnFalseIf(!_WriteTxtRecordSeparator(miff));
   }

   // If compression is used, prep the internal buffer
   if (isCompressed)
   {
      // Prepare the compression buffer.
      _CompressStart(miff, compressedChunkByteCount);
   }

   returnTrue;
}

/******************************************************************************
func: miffRecordSetEnd
******************************************************************************/
MiffBool miffRecordSetEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Finish the last chunk.
   if (miff->isCompressed)
   {
      returnFalseIf(!_CompressWrite(miff, miff->compressMemByteIndex, miff->compressMemByteData));
      _CompressStop(miff);
   }

   // Write the record ender
   returnFalseIf(!_WriteTxtRecordEnder(miff));

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->currentRecord.type = miffTypeNONE;

   returnTrue;
}

/******************************************************************************
func: miffRecordSetNextArrayItem
******************************************************************************/
MiffBool miffRecordSetNextArrayItem(Miff * const miff, MiffN8 const index, MiffN8 const count)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Type already ended
   returnFalseIf(miff->currentRecord.type == miffTypeNONE)

   // Move to the next array item
   miff->currentArrayIndex++;

   // Are we still writing out elements?
   if (index != count - 1)
   {
      if (!miff->isCompressed)
      {
         returnFalseIf(!_WriteTxtRecordSeparator(miff));
      }
   }

   returnTrue;
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool miffStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc,
   MiffMemCompressBound const memCompressBoundFunc, MiffMemCompress const memCompressFunc,
   MiffMemDecompress const memDecompressFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _MemStart(memCreateFunc, memDestroyFunc, memCompressBoundFunc, memCompressFunc, memDecompressFunc);

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
func: miffSetBlockStart
******************************************************************************/
MiffBool miffSetBlockStart(Miff * const miff, MiffC2 const * const key)
{
   return miffRecordSetBegin(miff, miffTypeKEY_VALUE_BLOCK_START, key, miffArrayCountUNKNOWN, miffBoolFALSE, 0);
}

/******************************************************************************
func: miffSetBlockStop
******************************************************************************/
MiffBool miffSetBlockStop(Miff * const miff)
{
   return miffRecordSetBegin(miff, miffTypeKEY_VALUE_BLOCK_STOP, NULL, miffArrayCountUNKNOWN, miffBoolFALSE, 0);
}

/******************************************************************************
func: miffSet1ABI1
******************************************************************************/
MiffBool miffSet1ABI1(Miff * const miff, MiffC2 const * const key, MiffABI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABI1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABI1(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABI2
******************************************************************************/
MiffBool miffSet1ABI2(Miff * const miff, MiffC2 const * const key, MiffABI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABI2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABI2(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABI4
******************************************************************************/
MiffBool miffSet1ABI4(Miff * const miff, MiffC2 const * const key, MiffABI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABI4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABI4(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN1
******************************************************************************/
MiffBool miffSet1ABN1(Miff * const miff, MiffC2 const * const key, MiffABN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABN1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABN1(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN2
******************************************************************************/
MiffBool miffSet1ABN2(Miff * const miff, MiffC2 const * const key, MiffABN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABN2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABN2(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN4
******************************************************************************/
MiffBool miffSet1ABN4(Miff * const miff, MiffC2 const * const key, MiffABN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABN4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABN4(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR4
******************************************************************************/
MiffBool miffSet1ABR4(Miff * const miff, MiffC2 const * const key, MiffABR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABR4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABR4(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR8
******************************************************************************/
MiffBool miffSet1ABR8(Miff * const miff, MiffC2 const * const key, MiffABR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABR8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABR8(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI1
******************************************************************************/
MiffBool miffSet1ABCI1(Miff * const miff, MiffC2 const * const key, MiffABCI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCI1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCI1(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI2
******************************************************************************/
MiffBool miffSet1ABCI2(Miff * const miff, MiffC2 const * const key, MiffABCI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCI2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCI2(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI4
******************************************************************************/
MiffBool miffSet1ABCI4(Miff * const miff, MiffC2 const * const key, MiffABCI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCI4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCI4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN1
******************************************************************************/
MiffBool miffSet1ABCN1(Miff * const miff, MiffC2 const * const key, MiffABCN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCN1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCN1(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN2
******************************************************************************/
MiffBool miffSet1ABCN2(Miff * const miff, MiffC2 const * const key, MiffABCN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCN2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCN2(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN4
******************************************************************************/
MiffBool miffSet1ABCN4(Miff * const miff, MiffC2 const * const key, MiffABCN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCN4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCN4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR4
******************************************************************************/
MiffBool miffSet1ABCR4(Miff * const miff, MiffC2 const * const key, MiffABCR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCR4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCR4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR8
******************************************************************************/
MiffBool miffSet1ABCR8(Miff * const miff, MiffC2 const * const key, MiffABCR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCR8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCR8(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI1
******************************************************************************/
MiffBool miffSet1ABCDI1(Miff * const miff, MiffC2 const * const key, MiffABCDI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDI1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDI1( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI2
******************************************************************************/
MiffBool miffSet1ABCDI2(Miff * const miff, MiffC2 const * const key, MiffABCDI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDI2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDI2( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI4
******************************************************************************/
MiffBool miffSet1ABCDI4(Miff * const miff, MiffC2 const * const key, MiffABCDI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDI4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDI4( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN1
******************************************************************************/
MiffBool miffSet1ABCDN1(Miff * const miff, MiffC2 const * const key, MiffABCDN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDN1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDN1( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN2
******************************************************************************/
MiffBool miffSet1ABCDN2(Miff * const miff, MiffC2 const * const key, MiffABCDN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDN2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDN2( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN4
******************************************************************************/
MiffBool miffSet1ABCDN4(Miff * const miff, MiffC2 const * const key, MiffABCDN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDN4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDN4( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR4
******************************************************************************/
MiffBool miffSet1ABCDR4(Miff * const miff, MiffC2 const * const key, MiffABCDR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDR4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDR4( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR8
******************************************************************************/
MiffBool miffSet1ABCDR8(Miff * const miff, MiffC2 const * const key, MiffABCDR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDR8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueABCDR8( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Boolean
******************************************************************************/
MiffBool miffSet1Boolean(Miff * const miff, MiffC2 const * const key, MiffBool const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeBOOLEAN, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueBoolean(miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I1
******************************************************************************/
MiffBool miffSet1I1(Miff * const miff, MiffC2 const * const key, MiffI1 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueI1(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I2
******************************************************************************/
MiffBool miffSet1I2(Miff * const miff, MiffC2 const * const key, MiffI2 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueI2(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I4
******************************************************************************/
MiffBool miffSet1I4(Miff * const miff, MiffC2 const * const key, MiffI4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueI4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I8
******************************************************************************/
MiffBool miffSet1I8(Miff * const miff, MiffC2 const * const key, MiffI8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueI8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R4
******************************************************************************/
MiffBool miffSet1Matrix2x2R4(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX2X2R4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueMatrix2x2R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R8
******************************************************************************/
MiffBool miffSet1Matrix2x2R8(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX2X2R8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueMatrix2x2R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R4
******************************************************************************/
MiffBool miffSet1Matrix3x3R4(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX3X3R4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueMatrix3x3R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R8
******************************************************************************/
MiffBool miffSet1Matrix3x3R8(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX3X3R8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueMatrix3x3R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R4
******************************************************************************/
MiffBool miffSet1Matrix4x4R4(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX4X4R4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueMatrix4x4R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R8
******************************************************************************/
MiffBool miffSet1Matrix4x4R8(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX4X4R8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueMatrix4x4R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N1
******************************************************************************/
MiffBool miffSet1N1(Miff * const miff, MiffC2 const * const key, MiffN1 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN1, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueN1(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N2
******************************************************************************/
MiffBool miffSet1N2(Miff * const miff, MiffC2 const * const key, MiffN2 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN2, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueN2(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N4
******************************************************************************/
MiffBool miffSet1N4(Miff * const miff, MiffC2 const * const key, MiffN4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueN4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N8
******************************************************************************/
MiffBool miffSet1N8(Miff * const miff, MiffC2 const * const key, MiffN8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueN8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R4
******************************************************************************/
MiffBool miffSet1R4(Miff * const miff, MiffC2 const * const key, MiffR4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueR4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R8
******************************************************************************/
MiffBool miffSet1R8(Miff * const miff, MiffC2 const * const key, MiffR8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueR8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Type
******************************************************************************/
MiffBool miffSet1Type(Miff * const miff, MiffC2 const * const key, MiffType const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeTYPE, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueType(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1StringC2
******************************************************************************/
MiffBool miffSet1StringC2(Miff * const miff, MiffC2 const * const key, MiffC2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(  miff, miffTypeSTRING, key, 1, miffBoolFALSE, 0));
   returnFalseIf(!miffSetValueStringC2(miff, value));
   returnFalseIf(!miffRecordSetEnd(    miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABI1
******************************************************************************/
MiffBool miffSetNABI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABI1(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABI2
******************************************************************************/
MiffBool miffSetNABI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABI2(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABI4
******************************************************************************/
MiffBool miffSetNABI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABI4(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABN1
******************************************************************************/
MiffBool miffSetNABN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABN1(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABN2
******************************************************************************/
MiffBool miffSetNABN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABN2(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABN4
******************************************************************************/
MiffBool miffSetNABN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABN4(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABR4
******************************************************************************/
MiffBool miffSetNABR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABR4(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABR8
******************************************************************************/
MiffBool miffSetNABR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABR8(          miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCI1
******************************************************************************/
MiffBool miffSetNABCI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCI1(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCI2
******************************************************************************/
MiffBool miffSetNABCI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCI2(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCI4
******************************************************************************/
MiffBool miffSetNABCI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCI4(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCN1
******************************************************************************/
MiffBool miffSetNABCN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCN1(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCN2
******************************************************************************/
MiffBool miffSetNABCN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCN2(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCN4
******************************************************************************/
MiffBool miffSetNABCN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCN4(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCR4
******************************************************************************/
MiffBool miffSetNABCR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCR4(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCR8
******************************************************************************/
MiffBool miffSetNABCR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCR8(         miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDI1
******************************************************************************/
MiffBool miffSetNABCDI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDI1(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDI2
******************************************************************************/
MiffBool miffSetNABCDI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDI2(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDI4
******************************************************************************/
MiffBool miffSetNABCDI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDI4(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDN1
******************************************************************************/
MiffBool miffSetNABCDN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDN1(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDN2
******************************************************************************/
MiffBool miffSetNABCDN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDN2(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDN4
******************************************************************************/
MiffBool miffSetNABCDN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDN4(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDR4
******************************************************************************/
MiffBool miffSetNABCDR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDR4(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNABCDR8
******************************************************************************/
MiffBool miffSetNABCDR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueABCDR8(        miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNBoolean
******************************************************************************/
MiffBool miffSetNBoolean(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffBool const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeBOOLEAN, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueBoolean(       miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI1
******************************************************************************/
MiffBool miffSetNI1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueI1(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI2
******************************************************************************/
MiffBool miffSetNI2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueI2(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI4
******************************************************************************/
MiffBool miffSetNI4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueI4(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNI8
******************************************************************************/
MiffBool miffSetNI8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffI8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueI8(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix2x2R4
******************************************************************************/
MiffBool miffSetNMatrix2x2R4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueMatrix2x2R4(   miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix2x2R8
******************************************************************************/
MiffBool miffSetNMatrix2x2R8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueMatrix2x2R8(   miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix3x3R4
******************************************************************************/
MiffBool miffSetNMatrix3x3R4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueMatrix3x3R4(   miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix3x3R8
******************************************************************************/
MiffBool miffSetNMatrix3x3R8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueMatrix3x3R8(   miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix4x4R4
******************************************************************************/
MiffBool miffSetNMatrix4x4R4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueMatrix4x4R4(   miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNMatrix4x4R8
******************************************************************************/
MiffBool miffSetNMatrix4x4R8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueMatrix4x4R8(   miff, &value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN1
******************************************************************************/
MiffBool miffSetNN1(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN1 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN1, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueN1(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN2
******************************************************************************/
MiffBool miffSetNN2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN2 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN2, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueN2(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN4
******************************************************************************/
MiffBool miffSetNN4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueN4(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNN8
******************************************************************************/
MiffBool miffSetNN8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffN8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueN8(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNR4
******************************************************************************/
MiffBool miffSetNR4(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffR4 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueR4(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNR8
******************************************************************************/
MiffBool miffSetNR8(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffR8 const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueR8(            miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNType
******************************************************************************/
MiffBool miffSetNType(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffType const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeTYPE, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueType(          miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetNStringC2
******************************************************************************/
MiffBool miffSetNStringC2(Miff * const miff, MiffC2 const * const key, MiffN4 const count, MiffC2 const * const * const value)
{
   MiffN4 index;

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeSTRING, key, count, miffBoolFALSE, 0));
   forCount(index, count)
   {
      returnFalseIf(!miffSetValueStringC2(      miff, value[index]));
      returnFalseIf(!miffRecordSetNextArrayItem(miff, index, count));
   }
   returnFalseIf(!miffRecordSetEnd(miff));
   returnTrue;
}

/******************************************************************************
func: miffSetValueABI1
******************************************************************************/
MiffBool miffSetValueABI1(Miff * const miff, MiffABI1 const * const value)
{
   Miff2  vtemp;
   Miff1  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABI1 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abi = *value;

   miff->currentRecord.type = miffTypeI1;
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abi.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abi.b, vtemps)));
   miff->currentRecord.type = miffTypeABI1;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABI2
******************************************************************************/
MiffBool miffSetValueABI2(Miff * const miff, MiffABI2 const * const value)
{
   Miff4  vtemp;
   Miff2  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABI2 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abi = *value;

   miff->currentRecord.type = miffTypeI2;
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abi.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abi.b, vtemps)));
   miff->currentRecord.type = miffTypeABI2;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABI4
******************************************************************************/
MiffBool miffSetValueABI4(Miff * const miff, MiffABI4 const * const value)
{
   Miff8  vtemp;
   Miff4  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABI4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abi = *value;

   miff->currentRecord.type = miffTypeI4;
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abi.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abi.b, vtemps)));
   miff->currentRecord.type = miffTypeABI4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABN1
******************************************************************************/
MiffBool miffSetValueABN1(Miff * const miff, MiffABN1 const * const value)
{
   Miff2  vtemp;
   Miff1  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABN1 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abn = *value;

   miff->currentRecord.type = miffTypeN1;
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abn.b, vtemps)));
   miff->currentRecord.type = miffTypeABN1;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABN2
******************************************************************************/
MiffBool miffSetValueABN2(Miff * const miff, MiffABN2 const * const value)
{
   Miff4  vtemp;
   Miff2  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABN2 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abn = *value;

   miff->currentRecord.type = miffTypeN2;
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abn.b, vtemps)));
   miff->currentRecord.type = miffTypeABN2;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABN4
******************************************************************************/
MiffBool miffSetValueABN4(Miff * const miff, MiffABN4 const * const value)
{
   Miff8  vtemp;
   Miff4  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABN4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abn = *value;

   miff->currentRecord.type = miffTypeN4;
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abn.b, vtemps)));
   miff->currentRecord.type = miffTypeABN4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABR4
******************************************************************************/
MiffBool miffSetValueABR4(Miff * const miff, MiffABR4 const * const value)
{
   Miff8  vtemp;
   Miff4  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABR4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abr = *value;

   miff->currentRecord.type = miffTypeR4;
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abr.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abr.b, vtemps)));
   miff->currentRecord.type = miffTypeABR4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueAB R8
******************************************************************************/
MiffBool miffSetValueABR8(Miff * const miff, MiffABR8 const * const value)
{
   Miff16  vtemp;
   Miff8   vtemp8;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABR8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abr = *value;

   miff->currentRecord.type = miffTypeR8;
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abr.a, vtemp8)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abr.b, vtemp8)));
   miff->currentRecord.type = miffTypeABR8;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCI1
******************************************************************************/
MiffBool miffSetValueABCI1(Miff * const miff, MiffABCI1 const * const value)
{
   Miff3 vtemp;
   Miff1 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCI1 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abci = *value;

   miff->currentRecord.type = miffTypeI1;
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abci.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abci.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abci.c, vtemps)));
   miff->currentRecord.type = miffTypeABCI1;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCI2
******************************************************************************/
MiffBool miffSetValueABCI2(Miff * const miff, MiffABCI2 const * const value)
{
   Miff6 vtemp;
   Miff2 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCI2 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abci = *value;

   miff->currentRecord.type = miffTypeI2;
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abci.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abci.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abci.c, vtemps)));
   miff->currentRecord.type = miffTypeABCI2;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCI4
******************************************************************************/
MiffBool miffSetValueABCI4(Miff * const miff, MiffABCI4 const * const value)
{
   Miff12  vtemp;
   Miff4   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCI4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abci = *value;

   miff->currentRecord.type = miffTypeI4;
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abci.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abci.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abci.c, vtemps)));
   miff->currentRecord.type = miffTypeABCI4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCN1
******************************************************************************/
MiffBool miffSetValueABCN1(Miff * const miff, MiffABCN1 const * const value)
{
   Miff3 vtemp;
   Miff1 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCN1 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcn = *value;

   miff->currentRecord.type = miffTypeN1;
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcn.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcn.c, vtemps)));
   miff->currentRecord.type = miffTypeABCN1;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCN2
******************************************************************************/
MiffBool miffSetValueABCN2(Miff * const miff, MiffABCN2 const * const value)
{
   Miff6 vtemp;
   Miff2 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCN2 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcn = *value;

   miff->currentRecord.type = miffTypeN2;
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcn.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcn.c, vtemps)));
   miff->currentRecord.type = miffTypeABCN2;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCN4
******************************************************************************/
MiffBool miffSetValueABCN4(Miff * const miff, MiffABCN4 const * const value)
{
   Miff12  vtemp;
   Miff4   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCN4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcn = *value;

   miff->currentRecord.type = miffTypeN4;
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcn.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcn.c, vtemps)));
   miff->currentRecord.type = miffTypeABCN4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCR4
******************************************************************************/
MiffBool miffSetValueABCR4(Miff * const miff, MiffABCR4 const * const value)
{
   Miff12  vtemp;
   Miff4   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCR4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcr = *value;

   miff->currentRecord.type = miffTypeR4;
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcr.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcr.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcr.c, vtemps)));
   miff->currentRecord.type = miffTypeABCR4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCR8
******************************************************************************/
MiffBool miffSetValueABCR8(Miff * const miff, MiffABCR8 const * const value)
{
   Miff24  vtemp;
   Miff8   vtemp8;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCR8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcr = *value;

   miff->currentRecord.type = miffTypeR8;
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcr.a, vtemp8)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcr.b, vtemp8)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcr.c, vtemp8)));
   miff->currentRecord.type = miffTypeABCR8;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDI1
******************************************************************************/
MiffBool miffSetValueABCDI1(Miff * const miff, MiffABCDI1 const * const value)
{
   Miff4 vtemp;
   Miff1 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDI1 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdi = *value;

   miff->currentRecord.type = miffTypeI1;
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abcdi.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abcdi.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abcdi.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeI1, (vtemps.i = vtemp.abcdi.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDI1;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDI2
******************************************************************************/
MiffBool miffSetValueABCDI2(Miff * const miff, MiffABCDI2 const * const value)
{
   Miff8 vtemp;
   Miff2 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDI2 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdi = *value;

   miff->currentRecord.type = miffTypeI2;
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abcdi.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abcdi.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abcdi.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeI2, (vtemps.i = vtemp.abcdi.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDI2;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDI4
******************************************************************************/
MiffBool miffSetValueABCDI4(Miff * const miff, MiffABCDI4 const * const value)
{
   Miff16  vtemp;
   Miff4   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDI4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdi = *value;

   miff->currentRecord.type = miffTypeI4;
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abcdi.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abcdi.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abcdi.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeI4, (vtemps.i = vtemp.abcdi.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDI4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDN1
******************************************************************************/
MiffBool miffSetValueABCDN1(Miff * const miff, MiffABCDN1 const * const value)
{
   Miff4 vtemp;
   Miff1 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDN1 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdn = *value;

   miff->currentRecord.type = miffTypeN1;
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcdn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcdn.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcdn.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue1(miff, miffTypeN1, (vtemps.n = vtemp.abcdn.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDN1;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDN2
******************************************************************************/
MiffBool miffSetValueABCDN2(Miff * const miff, MiffABCDN2 const * const value)
{
   Miff8 vtemp;
   Miff2 vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDN2 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdn = *value;

   miff->currentRecord.type = miffTypeN2;
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcdn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcdn.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcdn.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue2(miff, miffTypeN2, (vtemps.n = vtemp.abcdn.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDN2;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDN4
******************************************************************************/
MiffBool miffSetValueABCDN4(Miff * const miff, MiffABCDN4 const * const value)
{
   Miff16  vtemp;
   Miff4   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDN4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdn = *value;

   miff->currentRecord.type = miffTypeN4;
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcdn.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcdn.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcdn.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeN4, (vtemps.n = vtemp.abcdn.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDN4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDR4
******************************************************************************/
MiffBool miffSetValueABCDR4(Miff * const miff, MiffABCDR4 const * const value)
{
   Miff16  vtemp;
   Miff4   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDR4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdr = *value;

   miff->currentRecord.type = miffTypeR4;
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcdr.a, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcdr.b, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcdr.c, vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.abcdr.d, vtemps)));
   miff->currentRecord.type = miffTypeABCDR4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueABCDR8
******************************************************************************/
MiffBool miffSetValueABCDR8(Miff * const miff, MiffABCDR8 const * const value)
{
   Miff32  vtemp;
   Miff8   vtemp8;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeABCDR8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   vtemp.abcdr = *value;

   miff->currentRecord.type = miffTypeR8;
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcdr.a, vtemp8)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcdr.b, vtemp8)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcdr.c, vtemp8)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemp8.r = vtemp.abcdr.d, vtemp8)));
   miff->currentRecord.type = miffTypeABCDR8;

   returnTrue;
}

/******************************************************************************
func: miffSetValueBoolean
******************************************************************************/
MiffBool miffSetValueBoolean(Miff * const miff, MiffBool const value)
{
   MiffC1 *c1;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeBOOLEAN ||
        miff->currentRecord.type == miffTypeVARIABLE));

   c1 = (MiffC1 *) (value ? "T" : "F");

   returnFalseIf(!_WriteTxtC1(miff, c1));

   returnTrue;
}

/******************************************************************************
func: miffSetValueI1
******************************************************************************/
MiffBool miffSetValueI1(Miff * const miff, MiffI1 const value)
{
   Miff1 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.i = value;

   return _WriteValue1(miff, miffTypeI1, vtemp);
}

/******************************************************************************
func: miffSetValueI2
******************************************************************************/
MiffBool miffSetValueI2(Miff * const miff, MiffI2 const value)
{
   Miff2 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.i = value;

   return _WriteValue2(miff, miffTypeI2, vtemp);
}

/******************************************************************************
func: miffSetValueI4
******************************************************************************/
MiffBool miffSetValueI4(Miff * const miff, MiffI4 const value)
{
   Miff4 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.i = value;

   return _WriteValue4(miff, miffTypeI4, vtemp);
}

/******************************************************************************
func: miffSetValueI8
******************************************************************************/
MiffBool miffSetValueI8(Miff * const miff, MiffI8 const value)
{
   Miff8 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff)

   vtemp.i = value;

   return _WriteValue8(miff, miffTypeI8, vtemp);
}

/******************************************************************************
func: miffSetValueMatrix2x2R4
******************************************************************************/
MiffBool miffSetValueMatrix2x2R4(Miff * const miff, MiffMatrix2x2R4 const * const value)
{
   Miff16 vtemp;
   Miff4  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   _MemCopyTypeArray(1, MiffMatrix2x2R4, &vtemp.matrix2x2r, value);

   miff->currentRecord.type = miffTypeR4;
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix2x2r.cell[0][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix2x2r.cell[0][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix2x2r.cell[1][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix2x2r.cell[1][1], vtemps)));
   miff->currentRecord.type = miffTypeMATRIX2X2R4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix2x2R8
******************************************************************************/
MiffBool miffSetValueMatrix2x2R8(Miff * const miff, MiffMatrix2x2R8 const * const value)
{
   Miff32 vtemp;
   Miff8  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX2X2R8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   _MemCopyTypeArray(1, MiffMatrix2x2R4, &vtemp.matrix2x2r, value);

   miff->currentRecord.type = miffTypeR8;
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix2x2r.cell[0][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix2x2r.cell[0][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix2x2r.cell[1][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix2x2r.cell[1][1], vtemps)));
   miff->currentRecord.type = miffTypeMATRIX2X2R8;

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix3x3R4
******************************************************************************/
MiffBool miffSetValueMatrix3x3R4(Miff * const miff, MiffMatrix3x3R4 const * const value)
{
   Miff36 vtemp;
   Miff4  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   _MemCopyTypeArray(1, MiffMatrix3x3R4, &vtemp.matrix3x3r, value);

   miff->currentRecord.type = miffTypeR4;
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[0][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[0][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[0][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[1][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[1][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[1][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[2][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[2][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix3x3r.cell[2][2], vtemps)));
   miff->currentRecord.type = miffTypeMATRIX3X3R4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix3x3R8
******************************************************************************/
MiffBool miffSetValueMatrix3x3R8(Miff * const miff, MiffMatrix3x3R8 const * const value)
{
   Miff72 vtemp;
   Miff8  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX3X3R8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   _MemCopyTypeArray(1, MiffMatrix3x3R8, &vtemp.matrix3x3r, value);

   miff->currentRecord.type = miffTypeR8;
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[0][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[0][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[0][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[1][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[1][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[1][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[2][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[2][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix3x3r.cell[2][2], vtemps)));
   miff->currentRecord.type = miffTypeMATRIX3X3R8;

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix4x4R4
******************************************************************************/
MiffBool miffSetValueMatrix4x4R4(Miff * const miff, MiffMatrix4x4R4 const * const value)
{
   Miff64 vtemp;
   Miff4  vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   _MemCopyTypeArray(1, MiffMatrix4x4R4, &vtemp.matrix4x4r, value);

   miff->currentRecord.type = miffTypeR4;
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[0][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[0][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[0][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[0][3], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[1][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[1][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[1][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[1][3], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[2][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[2][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[2][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[2][3], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[3][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[3][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[3][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue4(miff, miffTypeR4, (vtemps.r = vtemp.matrix4x4r.cell[3][3], vtemps)));
   miff->currentRecord.type = miffTypeMATRIX4X4R4;

   returnTrue;
}

/******************************************************************************
func: miffSetValueMatrix4x4R8
******************************************************************************/
MiffBool miffSetValueMatrix4x4R8(Miff * const miff, MiffMatrix4x4R8 const * const value)
{
   Miff128 vtemp;
   Miff8   vtemps;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeMATRIX4X4R8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   _MemCopyTypeArray(1, MiffMatrix4x4R4, &vtemp.matrix4x4r, value);

   miff->currentRecord.type = miffTypeR8;
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[0][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[0][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[0][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[0][3], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[1][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[1][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[1][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[1][3], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[2][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[2][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[2][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[2][3], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[3][0], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[3][1], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[3][2], vtemps)));
   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteValue8(miff, miffTypeR8, (vtemps.r = vtemp.matrix4x4r.cell[3][3], vtemps)));
   miff->currentRecord.type = miffTypeMATRIX4X4R8;

   returnTrue;
}


/******************************************************************************
func: miffSetValueN1
******************************************************************************/
MiffBool miffSetValueN1(Miff * const miff, MiffN1 const value)
{
   Miff1 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.n = value;

   return _WriteValue1(miff, miffTypeN1, vtemp);
}

/******************************************************************************
func: miffSetValueN2
******************************************************************************/
MiffBool miffSetValueN2(Miff * const miff, MiffN2 const value)
{
   Miff2 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.n = value;

   return _WriteValue2(miff, miffTypeN2, vtemp);
}

/******************************************************************************
func: miffSetValueN4
******************************************************************************/
MiffBool miffSetValueN4(Miff * const miff, MiffN4 const value)
{
   Miff4 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.n = value;

   return _WriteValue4(miff, miffTypeN4, vtemp);
}

/******************************************************************************
func: miffSetValueN8
******************************************************************************/
MiffBool miffSetValueN8(Miff * const miff, MiffN8 const value)
{
   Miff8 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.n = value;

   return _WriteValue8(miff, miffTypeN8, vtemp);
}

/******************************************************************************
func: miffSetValueR4
******************************************************************************/
MiffBool miffSetValueR4(Miff * const miff, MiffR4 const value)
{
   Miff4 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.r = value;

   return _WriteValue4(miff, miffTypeR4, vtemp);
}

/******************************************************************************
func: miffSetValueR8
******************************************************************************/
MiffBool miffSetValueR8(Miff * const miff, MiffR8 const value)
{
   Miff8 vtemp;

   returnFalseIf(
      !_isStarted ||
      !miff);

   vtemp.r = value;

   return _WriteValue8(miff, miffTypeR8, vtemp);
}

/******************************************************************************
func: miffSetValueStringC2
******************************************************************************/
MiffBool miffSetValueStringC2(Miff * const miff, MiffC2 const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value      ||
      !(miff->currentRecord.type == miffTypeSTRING ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_WriteTxtValueC2(miff, value));

   returnTrue;
}

/******************************************************************************
func: miffSetValueType
******************************************************************************/
MiffBool miffSetValueType(Miff * const miff, MiffType const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeTYPE ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_WriteTxtValueType(miff, value));

   returnTrue;
}

#if 0
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
MiffBool miffGetValueHeader(Miff const * const miff, MiffType * const type,
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
MiffBool miffUserTypeAddVar(Miff * const miff, MiffC2 const * const name, MiffType const type, MiffN4 const arrayCount)
{
   MiffRecord *newType;

   returnFalseIf(
      !_isStarted                 ||
      !name                       ||
      !miff                       ||
      !miff->userTypeCurr == 0 ||
      type < 0 || miff->userTypeCount + miffTypeFIRST_USER_TYPE < type);

   newType = memCreateType(MiffRecord);
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
MiffType miffUserTypeStart(Miff * const miff, MiffC2 const * const name)
{
   int             index;
   MiffRecord *currType,
                  *nextType;

   returnIf(
         !_isStarted ||
         !miff       ||
         !name,
      (MiffType) 0);

   // Find if the user type already exists.
   forCount(index, miff->userTypeCount)
   {
      // Check if a user type exists.
      if (_MemIsEqual(
            miff->userTypeList[index + miffTypeFIRST_USER_TYPE].nameC2Count * sizeof(MiffC2),
            (MiffN1 *) miff->userTypeList[index + miffTypeFIRST_USER_TYPE].nameC2,
            (MiffN4) (wcsnlen_s(name, 255)                                       * sizeof(MiffC2)),
            (MiffN1 *) name))
      {
         // We are resetting that user type.
         miff->userTypeCurr = (MiffN2) (index + miffTypeFIRST_USER_TYPE);

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
      if      (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "{"))      miff->valueType = miffTypeKEY_VALUE_BLOCK;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "type"))   miff->valueType = miffTypeTYPE;
      else if (_MemIsEqual(typeByteCount, type, 8, (MiffN1 *) "define")) miff->valueType = miffTypeDEFINE;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "\""))     miff->valueType = miffTypeSTRING;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "bool"))   miff->valueType = miffTypeBOOLEAN;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i1"))     miff->valueType = miffTypeI1;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i2"))     miff->valueType = miffTypeI2;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i3"))     miff->valueType = miffTypeI3;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i4"))     miff->valueType = miffTypeI4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "i8"))     miff->valueType = miffTypeI8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i16"))    miff->valueType = miffTypeI16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i32"))    miff->valueType = miffTypeI32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "i64"))    miff->valueType = miffTypeI64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "i128"))   miff->valueType = miffTypeI128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "i256"))   miff->valueType = miffTypeI256;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n1"))     miff->valueType = miffTypeN1;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n2"))     miff->valueType = miffTypeN2;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n3"))     miff->valueType = miffTypeN3;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n4"))     miff->valueType = miffTypeN4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "n8"))     miff->valueType = miffTypeN8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n16"))    miff->valueType = miffTypeN16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n32"))    miff->valueType = miffTypeN32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "n64"))    miff->valueType = miffTypeN64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "n128"))   miff->valueType = miffTypeN128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "n256"))   miff->valueType = miffTypeN256;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "r4"))     miff->valueType = miffTypeR4;
      else if (_MemIsEqual(typeByteCount, type, 2, (MiffN1 *) "r8"))     miff->valueType = miffTypeR8;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r16"))    miff->valueType = miffTypeR16;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r32"))    miff->valueType = miffTypeR32;
      else if (_MemIsEqual(typeByteCount, type, 3, (MiffN1 *) "r64"))    miff->valueType = miffTypeR64;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "r128"))   miff->valueType = miffTypeR128;
      else if (_MemIsEqual(typeByteCount, type, 4, (MiffN1 *) "r256"))   miff->valueType = miffTypeR256;
      else
      {
         // User type
         //miff->valueType = _FindUserType(typeByteCount, type);
      }

      // Read in the array count
      if (miff->arrayFlag == miffArrayFlagIS_ARRAY)
      {
         returnFalseIf(!_ReadValue4(miff, mode, miffTypeN4, (Miff4 *) &miff->arrayCount));
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
   MiffType const type, Miff1 * const value)
{
   MiffN4 partByteCount;
   MiffC1 part[64];

   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer(miff->dataRepo, 1, value));
   }

   memClearTypeArray(64, MiffN1, part);
   returnFalseIf(_ReadPart(miff, 64, &partByteCount, part));

   if (type == miffTypeI1)
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
   MiffType const type, Miff2 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];

   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer(miff->dataRepo, 2, (Miff1 *) value));
      _ByteSwap2(miff, value);
   }

   if (type == miffTypeR2)
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

   if (type == miffTypeI2)
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
   MiffType const type, Miff4 * const value)
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
   if (type == miffTypeR4)
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
   if (type == miffTypeI4)
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
   MiffType const type, Miff8 * const value)
{
   MiffN4 partByteCount;
   MiffN1 part[64];

   if (mode == miffModeBINARY)
   {
      returnFalseIf(!miff->getBuffer8(miff->dataRepo, 1, value));
      _ByteSwap8(miff, value);
   }

   if (type == miffTypeR4)
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

   if (type == miffTypeI8)
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
static MiffBool _WriteValue2(Miff * const miff, MiffMode const mode, MiffType const type, Miff2 * const value)
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
static MiffBool _WriteValue4(Miff * const miff, MiffMode const mode, MiffType const type, Miff4 * const value)
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
static MiffBool _WriteValue8(Miff * const miff, MiffMode const mode, MiffType const type, Miff8 * const value)
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
