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
   MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo)
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
   MiffSetBuffer setBufferFunc, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion,
   void * const dataRepo)
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
func: miffGetValueBoolean
******************************************************************************/
MiffBool miffGetValueBoolean(Miff * const miff, MiffBool * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeBOOLEAN ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_ReadTxtPart(miff));

   *value = miffBoolFALSE;
   if (miff->readByteData[0] == 'T')
   {
      *value = miffBoolTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: miffGetValueI
******************************************************************************/
MiffBool miffGetValueI(Miff * const miff, MiffI8 * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeI1 ||
        miff->currentRecord.type == miffTypeI2 ||
        miff->currentRecord.type == miffTypeI4 ||
        miff->currentRecord.type == miffTypeI8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_ReadTxtPart(miff));

   *value = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}


/******************************************************************************
func: miffGetValueN
******************************************************************************/
MiffBool miffGetValueN(Miff * const miff, MiffN8 * const value)
{
   char *endPtr;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeN1 ||
        miff->currentRecord.type == miffTypeN2 ||
        miff->currentRecord.type == miffTypeN4 ||
        miff->currentRecord.type == miffTypeN8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_ReadTxtPart(miff));

   *value = (MiffN8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   returnTrue;
}

/******************************************************************************
func: miffGetValueR4
******************************************************************************/
MiffBool miffGetValueR4(Miff * const miff, MiffR4 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeR4 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_ReadTxtValueR4(miff, value))

   returnTrue;
}

/******************************************************************************
func: miffGetValueR8
******************************************************************************/
MiffBool miffGetValueR8(Miff * const miff, MiffR8 * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !(miff->currentRecord.type == miffTypeR8 ||
        miff->currentRecord.type == miffTypeVARIABLE));

   returnFalseIf(!_ReadTxtValueR8(miff, value));

   returnTrue;
}

/******************************************************************************
func: miffRecordGetBegin

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffBool miffRecordGetBegin(Miff * const miff, MiffType * const type, MiffC2 * const key,
   MiffN4 * const count)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->readRecordIsDone = miffBoolFALSE;

   *type                     = miffTypeNONE;
   *count                    = 0;

   // Clear the key.
   _MemClearTypeArray(miffKeySIZE, MiffC2, key);

   // Read in the type.
   returnFalseIf(!_ReadTxtRecordType(miff, &miff->currentRecord.type));
   *type = miff->currentRecord.type;

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      return miff->readRecordIsDone;
   }

   // Read in the name of the record
   returnFalseIf(!_ReadTxtRecordKeyC2(miff, miff->currentRecord.nameC2));
   _MemCopyTypeArray(miffKeyBYTE_COUNT, MiffC2, key, miff->currentRecord.nameC2);

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_START)
   {
      return miff->readRecordIsDone;
   }

   // Read in the array count
   returnFalseIf(!_ReadTxtRecordArrayCount(miff, &miff->currentRecord.arrayCount));
   *count = miff->currentRecord.arrayCount;

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
   MiffN4 const count)
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
   _MemClearTypeArray(miffKeySIZE,      MiffC2, miff->currentRecord.nameC2);
   _MemCopyTypeArray( _C2GetCount(key), MiffC2, miff->currentRecord.nameC2, key);

   returnFalseIf(!_WriteTxtRecordSeparator(miff));
   returnFalseIf(!_WriteTxtRecordKeyC2(    miff, miff->currentRecord.nameC2));
   // We are starting a new key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_START)
   {
      // TODO key value block stack push.
      return _WriteTxtRecordEnder(miff);
   }

   // Write out the record.
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));
   returnFalseIf(!_WriteTxtRecordArrayCount(  miff, count));
   returnFalseIf(!_WriteTxtRecordSeparator(   miff));

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
      returnFalseIf(!_WriteTxtRecordSeparator(miff));
   }

   returnTrue;
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffBool miffStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _MemStart(memCreateFunc, memDestroyFunc);

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
   return miffRecordSetBegin(miff, miffTypeKEY_VALUE_BLOCK_START, key, miffArrayCountUNKNOWN);
}

/******************************************************************************
func: miffSetBlockStop
******************************************************************************/
MiffBool miffSetBlockStop(Miff * const miff)
{
   return miffRecordSetBegin(miff, miffTypeKEY_VALUE_BLOCK_STOP, NULL, miffArrayCountUNKNOWN);
}

/******************************************************************************
func: miffSet1ABI1
******************************************************************************/
MiffBool miffSet1ABI1(Miff * const miff, MiffC2 const * const key, MiffABI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABI1, key, 1));
   returnFalseIf(!miffSetValueABI1(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABI2
******************************************************************************/
MiffBool miffSet1ABI2(Miff * const miff, MiffC2 const * const key, MiffABI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABI2, key, 1));
   returnFalseIf(!miffSetValueABI2(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABI4
******************************************************************************/
MiffBool miffSet1ABI4(Miff * const miff, MiffC2 const * const key, MiffABI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABI4, key, 1));
   returnFalseIf(!miffSetValueABI4(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN1
******************************************************************************/
MiffBool miffSet1ABN1(Miff * const miff, MiffC2 const * const key, MiffABN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABN1, key, 1));
   returnFalseIf(!miffSetValueABN1(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN2
******************************************************************************/
MiffBool miffSet1ABN2(Miff * const miff, MiffC2 const * const key, MiffABN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABN2, key, 1));
   returnFalseIf(!miffSetValueABN2(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABN4
******************************************************************************/
MiffBool miffSet1ABN4(Miff * const miff, MiffC2 const * const key, MiffABN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABN4, key, 1));
   returnFalseIf(!miffSetValueABN4(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR4
******************************************************************************/
MiffBool miffSet1ABR4(Miff * const miff, MiffC2 const * const key, MiffABR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABR4, key, 1));
   returnFalseIf(!miffSetValueABR4(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABR8
******************************************************************************/
MiffBool miffSet1ABR8(Miff * const miff, MiffC2 const * const key, MiffABR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABR8, key, 1));
   returnFalseIf(!miffSetValueABR8(   miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI1
******************************************************************************/
MiffBool miffSet1ABCI1(Miff * const miff, MiffC2 const * const key, MiffABCI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCI1, key, 1));
   returnFalseIf(!miffSetValueABCI1(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI2
******************************************************************************/
MiffBool miffSet1ABCI2(Miff * const miff, MiffC2 const * const key, MiffABCI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCI2, key, 1));
   returnFalseIf(!miffSetValueABCI2(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCI4
******************************************************************************/
MiffBool miffSet1ABCI4(Miff * const miff, MiffC2 const * const key, MiffABCI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCI4, key, 1));
   returnFalseIf(!miffSetValueABCI4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN1
******************************************************************************/
MiffBool miffSet1ABCN1(Miff * const miff, MiffC2 const * const key, MiffABCN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCN1, key, 1));
   returnFalseIf(!miffSetValueABCN1(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN2
******************************************************************************/
MiffBool miffSet1ABCN2(Miff * const miff, MiffC2 const * const key, MiffABCN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCN2, key, 1));
   returnFalseIf(!miffSetValueABCN2(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCN4
******************************************************************************/
MiffBool miffSet1ABCN4(Miff * const miff, MiffC2 const * const key, MiffABCN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCN4, key, 1));
   returnFalseIf(!miffSetValueABCN4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR4
******************************************************************************/
MiffBool miffSet1ABCR4(Miff * const miff, MiffC2 const * const key, MiffABCR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCR4, key, 1));
   returnFalseIf(!miffSetValueABCR4(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCR8
******************************************************************************/
MiffBool miffSet1ABCR8(Miff * const miff, MiffC2 const * const key, MiffABCR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCR8, key, 1));
   returnFalseIf(!miffSetValueABCR8(  miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI1
******************************************************************************/
MiffBool miffSet1ABCDI1(Miff * const miff, MiffC2 const * const key, MiffABCDI1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDI1, key, 1));
   returnFalseIf(!miffSetValueABCDI1( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI2
******************************************************************************/
MiffBool miffSet1ABCDI2(Miff * const miff, MiffC2 const * const key, MiffABCDI2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDI2, key, 1));
   returnFalseIf(!miffSetValueABCDI2( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDI4
******************************************************************************/
MiffBool miffSet1ABCDI4(Miff * const miff, MiffC2 const * const key, MiffABCDI4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDI4, key, 1));
   returnFalseIf(!miffSetValueABCDI4( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN1
******************************************************************************/
MiffBool miffSet1ABCDN1(Miff * const miff, MiffC2 const * const key, MiffABCDN1 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDN1, key, 1));
   returnFalseIf(!miffSetValueABCDN1( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN2
******************************************************************************/
MiffBool miffSet1ABCDN2(Miff * const miff, MiffC2 const * const key, MiffABCDN2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDN2, key, 1));
   returnFalseIf(!miffSetValueABCDN2( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDN4
******************************************************************************/
MiffBool miffSet1ABCDN4(Miff * const miff, MiffC2 const * const key, MiffABCDN4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDN4, key, 1));
   returnFalseIf(!miffSetValueABCDN4( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR4
******************************************************************************/
MiffBool miffSet1ABCDR4(Miff * const miff, MiffC2 const * const key, MiffABCDR4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDR4, key, 1));
   returnFalseIf(!miffSetValueABCDR4( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1ABCDR8
******************************************************************************/
MiffBool miffSet1ABCDR8(Miff * const miff, MiffC2 const * const key, MiffABCDR8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeABCDR8, key, 1));
   returnFalseIf(!miffSetValueABCDR8( miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Boolean
******************************************************************************/
MiffBool miffSet1Boolean(Miff * const miff, MiffC2 const * const key, MiffBool const value)
{
   returnFalseIf(!miffRecordSetBegin( miff, miffTypeBOOLEAN, key, 1));
   returnFalseIf(!miffSetValueBoolean(miff, value));
   returnFalseIf(!miffRecordSetEnd(   miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I1
******************************************************************************/
MiffBool miffSet1I1(Miff * const miff, MiffC2 const * const key, MiffI1 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI1, key, 1));
   returnFalseIf(!miffSetValueI1(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I2
******************************************************************************/
MiffBool miffSet1I2(Miff * const miff, MiffC2 const * const key, MiffI2 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI2, key, 1));
   returnFalseIf(!miffSetValueI2(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I4
******************************************************************************/
MiffBool miffSet1I4(Miff * const miff, MiffC2 const * const key, MiffI4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI4, key, 1));
   returnFalseIf(!miffSetValueI4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1I8
******************************************************************************/
MiffBool miffSet1I8(Miff * const miff, MiffC2 const * const key, MiffI8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI8, key, 1));
   returnFalseIf(!miffSetValueI8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R4
******************************************************************************/
MiffBool miffSet1Matrix2x2R4(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX2X2R4, key, 1));
   returnFalseIf(!miffSetValueMatrix2x2R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix2x2R8
******************************************************************************/
MiffBool miffSet1Matrix2x2R8(Miff * const miff, MiffC2 const * const key, MiffMatrix2x2R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX2X2R8, key, 1));
   returnFalseIf(!miffSetValueMatrix2x2R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R4
******************************************************************************/
MiffBool miffSet1Matrix3x3R4(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX3X3R4, key, 1));
   returnFalseIf(!miffSetValueMatrix3x3R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix3x3R8
******************************************************************************/
MiffBool miffSet1Matrix3x3R8(Miff * const miff, MiffC2 const * const key, MiffMatrix3x3R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX3X3R8, key, 1));
   returnFalseIf(!miffSetValueMatrix3x3R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R4
******************************************************************************/
MiffBool miffSet1Matrix4x4R4(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R4 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX4X4R4, key, 1));
   returnFalseIf(!miffSetValueMatrix4x4R4(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Matrix4x4R8
******************************************************************************/
MiffBool miffSet1Matrix4x4R8(Miff * const miff, MiffC2 const * const key, MiffMatrix4x4R8 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(     miff, miffTypeMATRIX4X4R8, key, 1));
   returnFalseIf(!miffSetValueMatrix4x4R8(miff, value));
   returnFalseIf(!miffRecordSetEnd(       miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N1
******************************************************************************/
MiffBool miffSet1N1(Miff * const miff, MiffC2 const * const key, MiffN1 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN1, key, 1));
   returnFalseIf(!miffSetValueN1(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N2
******************************************************************************/
MiffBool miffSet1N2(Miff * const miff, MiffC2 const * const key, MiffN2 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN2, key, 1));
   returnFalseIf(!miffSetValueN2(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N4
******************************************************************************/
MiffBool miffSet1N4(Miff * const miff, MiffC2 const * const key, MiffN4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN4, key, 1));
   returnFalseIf(!miffSetValueN4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1N8
******************************************************************************/
MiffBool miffSet1N8(Miff * const miff, MiffC2 const * const key, MiffN8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN8, key, 1));
   returnFalseIf(!miffSetValueN8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R4
******************************************************************************/
MiffBool miffSet1R4(Miff * const miff, MiffC2 const * const key, MiffR4 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4, key, 1));
   returnFalseIf(!miffSetValueR4(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1R8
******************************************************************************/
MiffBool miffSet1R8(Miff * const miff, MiffC2 const * const key, MiffR8 const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8, key, 1));
   returnFalseIf(!miffSetValueR8(    miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1Type
******************************************************************************/
MiffBool miffSet1Type(Miff * const miff, MiffC2 const * const key, MiffType const value)
{
   returnFalseIf(!miffRecordSetBegin(miff, miffTypeTYPE, key, 1));
   returnFalseIf(!miffSetValueType(  miff, value));
   returnFalseIf(!miffRecordSetEnd(  miff));
   returnTrue;
}

/******************************************************************************
func: miffSet1StringC2
******************************************************************************/
MiffBool miffSet1StringC2(Miff * const miff, MiffC2 const * const key, MiffC2 const * const value)
{
   returnFalseIf(!miffRecordSetBegin(  miff, miffTypeSTRING, key, 1));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABI4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABN4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABR8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCI4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCN4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCR8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDI4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDN4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeABCDR8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeBOOLEAN, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeI8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX2X2R8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX3X3R8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeMATRIX4X4R8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN1, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN2, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeN8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR4, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeR8, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeTYPE, key, count));
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

   returnFalseIf(!miffRecordSetBegin(miff, miffTypeSTRING, key, count));
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

/******************************************************************************
func: miffTypeGetC1
******************************************************************************/
MiffC1 const *miffTypeGetC1(MiffType const type)
{
   return _TypeGetNameC1(type);
}

/******************************************************************************
func: miffTypeGetC2
******************************************************************************/
MiffC2 const *miffTypeGetC2(MiffType const type)
{
   return _TypeGetNameC2(type);
}
