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
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "mifflib.h"
#include "miff_local.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static MiffB   _isStarted = miffFALSE;

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
Miff *miffCreateReader(MiffB const isByteSwaping, MiffGetBuffer getBufferFunc,
   MiffStr * const subFormatName, MiffN * const subFormatVersion, void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure.
   miff = _MiffMemCreateType(Miff);
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
      _MiffMemDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateReaderContent
******************************************************************************/
MiffB miffCreateReaderContent(Miff * const miff, MiffB const isByteSwaping,
   MiffGetBuffer getBufferFunc, MiffStr * const subFormatName, MiffN * const subFormatVersion,
   void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !getBufferFunc);

   _MiffMemClearType(Miff, miff);
   miff->method               = miffMethodREADING;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->getBuffer            = getBufferFunc;

   // Read the header.
   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffGetPart(miff, miffFALSE));
   returnFalseIf(!_MiffMemIsEqual(
      miff->readCount, 
      miff->readData, 
      4, 
      (MiffN1 *) MIFF_HEADER_FILETYPE_STR));

   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffGetPart(miff, miffFALSE));
   miff->version = _MiffPartToN(miff);
   returnFalseIf(!_MiffMemIsEqual(
      miff->readCount, 
      miff->readData, 
      1, 
      (MiffN1 *) MIFF_HEADER_VERSION_STR));

   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffGetPart(miff, miffFALSE));
   _MiffPartToKey(miff);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, subFormatName,       miff->currentName);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, miff->subFormatName, miff->currentName);

   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffGetPart(miff, miffFALSE));
   miff->subFormatVersion =
      *subFormatVersion   = _MiffPartToN(miff);

   returnFalseIf(!miff->isRecordDone);

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffB const isByteSwaping, MiffSetBuffer setBufferFunc,
   MiffStr const * const subFormatName, MiffN const subFormatVersion, void * const dataRepo)
{
   Miff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure
   miff = _MiffMemCreateType(Miff);
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
      _MiffMemDestroy(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/******************************************************************************
func: miffCreateWriterContent
******************************************************************************/
MiffB miffCreateWriterContent(Miff * const miff, MiffB const isByteSwaping,
   MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN const subFormatVersion,
   void * const dataRepo)
{
   MiffN4 count;

   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !setBufferFunc  ||
      !subFormatName  ||
      !subFormatVersion);

   _MiffMemClearType(Miff, miff);
   miff->version              = 1;
   miff->method               = miffMethodWRITING;
   miff->dataRepo             = dataRepo;
   miff->isByteSwapping       = isByteSwaping;
   miff->setBuffer            = setBufferFunc;
   miff->subFormatVersion     = subFormatVersion;

   count = min(255, _MiffStrGetCount(subFormatName));
   _MiffMemCopyTypeArray(count, MiffStr, miff->subFormatName, subFormatName);

   // Write the miff header.
   _MiffSetBuffer(   miff, MIFF_HEADER_FILETYPE_SIZE, (MiffN1 *) MIFF_HEADER_FILETYPE_STR);
   miffSetRecordStop(miff);
   _MiffSetBuffer(   miff, MIFF_HEADER_VERSION_SIZE,  (MiffN1 *) MIFF_HEADER_VERSION_STR);
   miffSetRecordStop(miff);

   _MiffSetBuffer(   miff, _MiffStrGetCount(miff->subFormatName), (MiffN1 *) miff->subFormatName);
   miffSetRecordStop(miff);
   _MiffSetN(        miff, miff->subFormatVersion);
   miffSetRecordStop(miff);

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

   _MiffMemDestroy(miff);
}

/******************************************************************************
func: miffGetRecordStart

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffB miffGetRecordStart(Miff * const miff, MiffRecType * const type, MiffN * const count,
   MiffStr * const key)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->isRecordDone = miffFALSE;

   *type              = miffRecTypeNONE;
   *count             = 0;

   // Clear the key.
   _MiffMemClearTypeArray(miffKeySIZE, MiffStr, key);

   // Read in the type.
   returnFalseIf(!_MiffGetPart(miff, miffTRUE));

   switch (miff->readData[0])
   {
   case '{':
      *type                   = miffRecTypeBLOCK_START;
      miff->currentArrayCount = miffArrayCountBLOCK_START;
      break;

   case '}':
      *type                   = miffRecTypeBLOCK_STOP;
      miff->currentArrayCount = miffArrayCountBLOCK_STOP;
      break;

   case '*':
      *type                   = miffRecTypeVALUE;
      miff->currentArrayCount =
         *count               =  miffArrayCountUNKNOWN;
      break;

   default:
      *type                   = miffRecTypeVALUE;
      miff->currentArrayCount = 
         *count               = _MiffPartToN(miff);
      break;
   }

   returnTrueIf(miff->currentArrayCount == miffArrayCountBLOCK_STOP);

   // Read in the name of the record
   returnFalseIf(!_MiffGetPart( miff, miffFALSE));
   returnFalseIf(!_MiffPartToKey(miff));

   _MiffMemCopyTypeArray(miff->currentNameCount, MiffStr, key, miff->currentName);

   returnTrueIf(miff->currentArrayCount == miffArrayCountBLOCK_START);

   returnTrue;
}

/******************************************************************************
func: miffGetRecordEnd

Skip to the end of the record if not at the end already.
******************************************************************************/
MiffB miffGetRecordEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnTrueIf(miff->isRecordDone);

   returnFalseIf(!_MiffGetLineSkip(miff));

   miff->isRecordDone = miffTRUE;

   returnTrue;
}

/******************************************************************************
func: miffGetValueBin

bin should be large enough for the bin data.  I.E. it should be of size from
miffGetValue() value.bufferCount
******************************************************************************/
MiffB miffGetValueBin(Miff * const miff, MiffN const binCount, MiffN1 * const binBuffer)
{
   returnFalseIf(
      !_isStarted                          ||
      !miff                                ||
      miff->value.type != miffValueTypeBIN ||
      binCount != miff->value.bufferCount  ||
      !binBuffer);

   returnFalseIf(!_MiffGetBin(miff, binCount, binBuffer));

   miff->valueIndex = binCount;

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/******************************************************************************
func: miffValueBinGetData
******************************************************************************/
MiffData miffGetValueBinData(Miff * const miff, MiffN1 * const binByte)
{
   returnIf(
         !_isStarted                          ||
         !miff                                ||
         miff->value.type != miffValueTypeBIN ||
         !binByte                             ||
         miff->valueIndex >= miff->value.bufferCount,
      miffDataERROR);

   return _MiffGetBinByte(miff, binByte);
}

/******************************************************************************
func: miffGetValueBinDataEnd
******************************************************************************/
MiffData miffGetValueBinDataEnd(Miff * const miff)
{
   returnIf(
         !_isStarted ||
         !miff       ||
         !miff->value.type != miffValueTypeBIN,
      miffDataERROR);

   return _MiffGetBinByteEnd(miff);
}

/******************************************************************************
func: miffGetValue
******************************************************************************/
MiffValue *miffGetValue(Miff * const miff)
{
   MiffStr   header;

   miff->isPartDone = miffFALSE;
   _MiffMemClearType(MiffValue, &miff->value);

   miff->value.type = miffValueTypeNONE;
   miff->valueIndex = 0;

   returnNullIf(
      !_isStarted ||
      !miff);

   header = _MiffGetValueHeader(miff);
   switch (header)
   {
   default:
      miff->value.type        = miffValueTypeOTHER;
      miff->value.header      = header;
      // Reading the rest of the value is up to the caller.
      break;

   case 0:
      break;

   case '~':
      miff->value.type        = miffValueTypeNULL;
      _MiffGetPartEnd(miff);
      break;

   case '"':
      miff->value.type        = miffValueTypeSTR;
      miff->value.bufferCount = _MiffGetValueBufferCount(miff);
      // Reading the string is done with miffGetValueStr() or miffGetValueStrData();
      break;

   case 'c':
      miff->value.type        = miffValueTypeC;
      miff->value.formatCIR   = miffValueFormatCIR_TEXT;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'C':
      miff->value.type        = miffValueTypeC;
      miff->value.formatCIR   = miffValueFormatCIR_BASE64;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'T':
      miff->value.type        = miffValueTypeB;
      miff->value.b           = miffTRUE;
      _MiffGetPartEnd(miff);
      break;

   case 'F':
      miff->value.type        = miffValueTypeB;
      _MiffGetPartEnd(miff);
      break;

   case 'i':
      miff->value.type        = miffValueTypeI;
      miff->value.formatCIR   = miffValueFormatCIR_TEXT;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'I':
      miff->value.type        = miffValueTypeI;
      miff->value.formatCIR   = miffValueFormatCIR_BASE64;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'n':
      miff->value.type        = miffValueTypeN;
      miff->value.formatN     = miffValueFormatN_TEXT;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'N':
      miff->value.type        = miffValueTypeN;
      miff->value.formatN     = miffValueFormatN_BASE64;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'x':
      miff->value.type        = miffValueTypeN;
      miff->value.formatN     = miffValueFormatN_X;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'b':
      miff->value.type        = miffValueTypeN;
      miff->value.formatN     = miffValueFormatN_B;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'r':
      miff->value.type        = miffValueTypeR;
      miff->value.formatCIR   = miffValueFormatCIR_TEXT;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case 'R':
      miff->value.type        = miffValueTypeR;
      miff->value.formatCIR   = miffValueFormatCIR_BASE64;
      _MiffGetPart(miff, miffFALSE);
      _MiffPartToValue(miff, &miff->value);
      break;

   case '*':
      miff->value.type        = miffValueTypeBIN;
      miff->value.bufferCount = _MiffGetValueBufferCount(miff);
      // Reading the buffer is done with miffValueBinGet();
      break;
   }
   
   return &miff->value;
}

/******************************************************************************
func: miffGetValueStr

Convenience function for getting a whole string if memory requirements are 
not that large.  Str should be large enough for the string being read in 
including null terminator.
******************************************************************************/
MiffB miffGetValueStr(Miff * const miff, MiffN const strCount, MiffStr * const str)
{
   MiffN      index;
   MiffN      strIndex;
   MiffStr    letter;
   MiffData   dataResult;

   returnFalseIf(
      !_isStarted                         ||
      !miff                               ||
      strCount != miff->value.bufferCount ||
      !str)

   strIndex = 0;
   forCount(index, strCount)
   {
      dataResult = miffGetValueStrData(miff, &letter);

      // Ensure null termination.
      str[strIndex] = 0;

      returnFalseIf(
         dataResult == miffDataERROR        ||
         dataResult == miffDataIS_PART_DONE ||
         dataResult == miffDataIS_RECORD_DONE);

      str[strIndex++] = letter;
   }

   // Ensure null termination.
   str[strIndex]    = 0;
   miff->valueIndex = strCount;

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/******************************************************************************
func: miffGetValueStrData
******************************************************************************/
MiffData miffGetValueStrData(Miff * const miff, MiffStr * const strLetter)
{
   MiffData miffData;

   returnIf(
         !_isStarted ||
         !miff       ||
         !strLetter  ||
         miff->valueIndex >= miff->value.bufferCount,
      miffDataERROR);

   miffData = _MiffGetStrLetter(miff, strLetter);
   miff->valueIndex++;

   return miffData;
}

/******************************************************************************
func: miffSetRecordStart
******************************************************************************/
MiffB miffSetRecordStart(Miff * const miff, MiffRecType const type, MiffN const count, 
   MiffStr const * const key)
{
   MiffN index;

   returnFalseIf(
      !_isStarted ||
      !miff);

   miff->isRecordDone = miffFALSE;

   // Adjust the scope level before we indent.
   if (type == miffRecTypeBLOCK_STOP)
   {
      if (miff->currentScopeLevel != 0)
      {
         miff->currentScopeLevel--;
      }
   }

   // Add indents to the current scope level.
   forCount(index, miff->currentScopeLevel)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) " "));
   }

   // Write the type for the record.  Common for all cases.
   miff->currentArrayCount = count;
   miff->currentArrayIndex = 0;

   if      (type == miffRecTypeBLOCK_START)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "{"));
      miff->isRecordDone = miffTRUE;
   }
   else if (type == miffRecTypeBLOCK_STOP)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "}"));
      miff->isRecordDone = miffTRUE;
   }
   else if (count == miffArrayCountUNKNOWN)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "*"));
   }
   else
   {
      returnFalseIf(!_MiffSetN(miff, count));
   }

   // We are ending a key value block.
   if (type == miffRecTypeBLOCK_STOP)
   {
      // TODO key value block stack pop.
      returnTrue;
   }

   returnFalseIf(!key);

   // Copy the key.
   miff->currentNameCount = min(256, _MiffStrGetCount(key));
   _MiffMemClearTypeArray(miffKeySIZE,            MiffStr, miff->currentName);
   _MiffMemCopyTypeArray( miff->currentNameCount, MiffStr, miff->currentName, key);

   returnFalseIf(!miffSetSeparator(miff));
   returnFalseIf(!_MiffSetBuffer(miff, miff->currentNameCount, (MiffN1 *) miff->currentName));
   
   // We are starting a new key value block.
   if (type == miffRecTypeBLOCK_START)
   {
      miff->currentScopeLevel++;

      // TODO key value block stack push.
      returnTrue;
   }

   returnTrue;
}

/******************************************************************************
func: miffSetRecordStop
******************************************************************************/
MiffB miffSetRecordStop(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->isRecordDone = miffTRUE;

   return _MiffSetBuffer(miff, 1, (MiffN1 *) "\n");
}

/******************************************************************************
func: miffSetSeparator
******************************************************************************/
MiffB miffSetSeparator(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffSetBuffer(miff, 1, (MiffN1 *) "\t");
}

/******************************************************************************
func: miffSetValue

Only to be used if the value holds everything including the data.  String and
Binary data may only just have the header set.  In which case ou need to call
This sequence.

miffSetValueStart()
miffsetValueBinData() or miffSetValueStrData() called multiple times.
miffSetValueStop()
******************************************************************************/
MiffB miffSetValue(Miff * const miff, MiffValue const * const value)
{
   // If the buffer data isn't provided with these types then we cannot used
   // this function to export them.
   returnFalseIf(
      (value->type == miffValueTypeBIN ||
       value->type == miffValueTypeSTR)   &&
      (value->bufferData.bin == NULL   ||
       value->bufferData.str == NULL));

   // Write out the header.
   returnFalseIf(!miffSetValueStart(miff, value));

   // Write out the value.
   returnFalseIf(!_MiffSetValueData(miff, value));
   
   // Write whatever is left to writ out and final check.
   return miffSetValueStop(miff);
}

/******************************************************************************
func: miffSetValueStart
******************************************************************************/
MiffB miffSetValueStart(Miff * const miff, MiffValue const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !value);

   miffSetSeparator(miff);

   return _MiffSetValueHeader(miff, value);
}

/******************************************************************************
func: miffSetValueBinData
******************************************************************************/
MiffB miffSetValueBinData(Miff * const miff, MiffN1 const binByte)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !binByte);

   returnFalseIf(
      miff->value.type != miffValueTypeBIN ||
      miff->valueIndex >= miff->value.bufferCount);

   return _MiffSetBinByte(miff, binByte);
}

/******************************************************************************
func: miffSetValueStrData
******************************************************************************/
MiffB miffSetValueStrData(Miff * const miff, MiffStr const strLetter)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !strLetter);

   // Not a string
   // this character is beyond the length of the buffer we are expecting.
   returnFalseIf(
      miff->value.type != miffValueTypeSTR ||
      miff->valueIndex >= miff->value.bufferCount);
   
   return _MiffSetStr(miff, 1, &strLetter);
}

/******************************************************************************
func: miffSetValueStop
******************************************************************************/
MiffB miffSetValueStop(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Write out the remainder base64 bit if needed.
   if (miff->value.type == miffValueTypeBIN)
   {
      returnFalseIf(!_MiffSetBinByteEnd(miff));
   }

   // Binary or String buffer was not fully written out.
   returnFalseIf(
       (miff->value.type == miffValueTypeBIN ||
        miff->value.type == miffValueTypeSTR)            &&
       miff->value.bufferCount != miffBufferCountUNKNOWN &&
       miff->value.bufferCount != miff->valueIndex);

   miff->currentArrayIndex++;

   returnTrue;
}

/******************************************************************************
func: miffStart
******************************************************************************/
MiffB miffStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _MiffMemStart(memCreateFunc, memDestroyFunc);

   _isStarted = miffTRUE;

   bsfStart();
   _MiffUtilStart();

   returnTrue;
}

/******************************************************************************
func: miffStop
******************************************************************************/
void miffStop(void)
{
   returnVoidIf(!_isStarted);

   _MiffUtilStop();
   bsfStop();

   _MiffMemStop();

   _isStarted = miffFALSE;
}

/******************************************************************************
miffValue 
function:
******************************************************************************/
/******************************************************************************
func: miffValueGetBinCount
******************************************************************************/
MiffN miffValueGetBinCount(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeBIN);

   return value->bufferCount;
}

/******************************************************************************
func: miffValueGetB
******************************************************************************/
MiffB miffValueGetB(MiffValue const * const value)
{
   returnFalseIf(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeB);
   
   return value->b;
}

/******************************************************************************
func: miffValueGetI
******************************************************************************/
MiffI miffValueGetI(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeI);
   
   return value->inr.i;
}

/******************************************************************************
func: miffValueGetN
******************************************************************************/
MiffN miffValueGetN(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeN);
   
   return value->inr.n;
}

/******************************************************************************
func: miffValueGetR
******************************************************************************/
MiffR miffValueGetR(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeR);

   if (value->is4)
   {
      return (MiffR) value->inr4.r;
   }

   return value->inr.r;
}

/******************************************************************************
func: miffValueGetR4
******************************************************************************/
MiffR4 miffValueGetR4(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeR);

   if (value->is4)
   {
      return value->inr4.r;
   }
 
   return (MiffR4) value->inr.r;
}

/******************************************************************************
func: miffValueGetStrLen
******************************************************************************/
MiffN miffValueGetStrCount(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeSTR);

   return value->bufferCount;
}

/******************************************************************************
func: miffValueGetType
******************************************************************************/
MiffValueType miffValueGetType(MiffValue const * const value)
{
   returnIf(
         !_isStarted ||
         !value,
      miffValueTypeNONE);

   return value->type;
}

/******************************************************************************
func: miffValueIs4

Return the Real value byte count.
True if MiffR4
******************************************************************************/
MiffB miffValueIs4(MiffValue const * const value)
{
   return0If(
      !_isStarted ||
      !value      ||
      value->type != miffValueTypeR);

   return value->is4;
}

/******************************************************************************
func: miffValueSetB
******************************************************************************/
MiffB miffValueSetB(MiffValue * const value, MiffB const ivalue)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type = miffValueTypeB;
   value->b    = ivalue;

   returnTrue;
}

/******************************************************************************
func: miffValueSetBinBuffer
******************************************************************************/
MiffB miffValueSetBinBuffer(MiffValue * const value, MiffN  const binCount, MiffN1  * const binBuffer)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type           = miffValueTypeBIN;
   value->bufferCount    = binCount;
   value->bufferData.bin = binBuffer;

   returnTrue;
}

/******************************************************************************
func: miffValueSetBinCount
******************************************************************************/
MiffB miffValueSetBinCount(MiffValue * const value, MiffN  const binCount)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type           = miffValueTypeBIN;
   value->bufferCount    = binCount;

   returnTrue;
}

/******************************************************************************
func: miffValueSetI
******************************************************************************/
MiffB miffValueSetI(MiffValue * const value, MiffI const ivalue, MiffValueFormatCIR const format)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type      = miffValueTypeI;
   value->formatCIR = format;
   value->inr.i     = ivalue;

   returnTrue;
}

/******************************************************************************
func: miffValueSetN
******************************************************************************/
MiffB miffValueSetN(MiffValue * const value, MiffN const ivalue, MiffValueFormatN const format)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type    = miffValueTypeN;
   value->formatN = format;
   value->inr.n   = ivalue;

   returnTrue;
}

/******************************************************************************
func: miffValueSetNull
******************************************************************************/
MiffB miffValueSetNull(MiffValue * const value)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type = miffValueTypeNULL;

   returnTrue;
}

/******************************************************************************
func: miffValueSetR
******************************************************************************/
MiffB miffValueSetR(MiffValue * const value, MiffR const ivalue, MiffValueFormatCIR const format)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type      = miffValueTypeR;
   value->formatCIR = format;
   value->inr.r     = ivalue;

   returnTrue;
}

/******************************************************************************
func: miffValueSetR4
******************************************************************************/
MiffB miffValueSetR4(MiffValue * const value, MiffR4 const ivalue, MiffValueFormatCIR const format)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type      = miffValueTypeR;
   value->formatCIR = format;
   value->is4       = miffTRUE;
   value->inr4.r    = ivalue;

   returnTrue;
}

/******************************************************************************
func: miffValueSetStrBuffer
******************************************************************************/
MiffB miffValueSetStrBuffer(MiffValue * const value, MiffN const strCount, MiffStr * const strBuffer)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type           = miffValueTypeSTR;
   value->bufferCount    = strCount;
   value->bufferData.str = strBuffer;

   returnTrue;
}

/******************************************************************************
func: miffValueSetStrCount
******************************************************************************/
MiffB miffValueSetStrCount(MiffValue * const value, MiffN const strCount)
{
   returnFalseIf(
      !_isStarted ||
      !value);

   _MiffMemClearType(MiffValue, value);

   value->type           = miffValueTypeSTR;
   value->bufferCount    = strCount;

   returnTrue;
}
