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
#include "local.h"

/******************************************************************************
local:
variable:
******************************************************************************/
static MiffB   _isStarted = miffFALSE;

/******************************************************************************
prototype:
******************************************************************************/
static MiffB   _WriteStr(  Miff * const miff, MiffN const strLen, MiffStr const * const str);

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
   miff->readByteCountActual  = 1024;
   miff->readStrCountActual   = 1024;
   miff->readByteCount        = 0;
   miff->readStrCount         = 0;
   miff->readByteData         = _MiffMemCreateTypeArray(1024, MiffN1);
   miff->readStrData          = _MiffMemCreateTypeArray(1024, MiffStr);
   returnFalseIf(!miff->readByteData);
   returnFalseIf(!miff->readStrData);

   // Read the header.
   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffFALSE));
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      4, 
      (MiffN1 *) MIFF_HEADER_FILETYPE_STR));

   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffFALSE));
   miff->version = _MiffPartToN(miff);
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      1, 
      (MiffN1 *) MIFF_HEADER_VERSION_STR));

   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffFALSE));
   _MiffPartToKey(miff);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, subFormatName,       miff->currentName);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, miff->subFormatName, miff->currentName);

   miff->isRecordDone = miffFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffFALSE));
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
   _MiffWriteStr(   miff, MIFF_HEADER_FILETYPE_SIZE, MIFF_HEADER_FILETYPE_STR);
   miffSetRecordEnd(miff);
   _MiffWriteStr(   miff, MIFF_HEADER_VERSION_SIZE,  MIFF_HEADER_VERSION_STR);
   miffSetRecordEnd(miff);

   _MiffWriteStr(   miff, _MiffStrGetCount(miff->subFormatName),    miff->subFormatName);
   miffSetRecordEnd(miff);
   _MiffWriteN(     miff, miff->subFormatVersion);
   miffSetRecordEnd(miff);

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

   _MiffMemDestroy(miff->readByteData);
   _MiffMemDestroy(miff->readStrData);
   _MiffMemDestroy(miff);
}

/******************************************************************************
func: miffGetInfo

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffB miffGetInfo(Miff * const miff, MiffRecType * const type, MiffN * const count,
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
   returnFalseIf(!_MiffReadPart(miff, miffTRUE));

   switch (miff->readByteData[0])
   {
   case '{':
      *type                   = miffRecTypeBLOCK_START;
      miff->currentArrayCount = miffArrayCountBLOCK_START;
      miff->isRecordDone      = miffTRUE;
      break;

   case '}':
      *type                   = miffRecTypeBLOCK_STOP;
      miff->currentArrayCount = miffArrayCountBLOCK_STOP;
      miff->isRecordDone      = miffTRUE;
      returnTrue;

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

   // Read in the name of the record
   returnFalseIf(!_MiffReadPart( miff, miffFALSE));
   returnFalseIf(!_MiffPartToKey(miff));

   _MiffMemCopyTypeArray(miff->currentNameCount, MiffStr, key, miff->currentName);

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

   returnFalseIf(!_MiffReadLineSkip(miff));

   miff->isRecordDone = miffTRUE;

   returnTrue;
}

/******************************************************************************
func: miffGetValueHeader
******************************************************************************/
MiffValue miffGetValueHeader(Miff * const miff)
{
   MiffStr   header;
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type = miffValueTypeNONE;

   returnIf(
         !_isStarted ||
         !miff,
      value);

   header = _MiffReadValueHeader(miff);
   switch (header)
   {
   default:
      value.type        = miffValueTypeOTHER;
      value.header      = header;
      // Reading the rest of the value is up to the caller.
      break;

   case 0:
      break;

   case '~':
      value.type        = miffValueTypeNULL;
      _MiffReadPartEnd(miff);
      break;

   case '"':
      value.type        = miffValueTypeSTR;
      value.bufferCount = _MiffReadValueBufferCount(miff);
      // Reading the string is done with miffGetValueStr();
      break;

   case 'c':
      value.type        = miffValueTypeC;
      value.formatCIR   = miffValueFormatCIR_TEXT;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'C':
      value.type        = miffValueTypeC;
      value.formatCIR   = miffValueFormatCIR_BASE64;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'T':
      value.type        = miffValueTypeB;
      value.b           = miffTRUE;
      _MiffReadPartEnd(miff);
      break;

   case 'F':
      value.type        = miffValueTypeB;
      _MiffReadPartEnd(miff);
      break;

   case 'i':
      value.type        = miffValueTypeI;
      value.formatCIR   = miffValueFormatCIR_TEXT;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'I':
      value.type        = miffValueTypeI;
      value.formatCIR   = miffValueFormatCIR_BASE64;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'n':
      value.type        = miffValueTypeN;
      value.formatN     = miffValueFormatN_TEXT;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'N':
      value.type        = miffValueTypeN;
      value.formatN     = miffValueFormatN_BASE64;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'x':
      value.type        = miffValueTypeN;
      value.formatN     = miffValueFormatN_X;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'o':
      value.type        = miffValueTypeN;
      value.formatN     = miffValueFormatN_O;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'b':
      value.type        = miffValueTypeN;
      value.formatN     = miffValueFormatN_B;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'r':
      value.type        = miffValueTypeR;
      value.formatCIR   = miffValueFormatCIR_TEXT;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case 'R':
      value.type        = miffValueTypeR;
      value.formatCIR   = miffValueFormatCIR_BASE64;
      _MiffReadPart(miff, miffFALSE);
      _MiffPartToValue(miff, &value);
      break;

   case '*':
      value.type        = miffValueTypeBIN;
      value.bufferCount = _MiffReadValueBufferCount(miff);
      // Reading the buffer is done with miffGetValueBin();
      break;
   }
   
   return value;
}

/******************************************************************************
func: miffGetValueStr
******************************************************************************/
MiffB miffGetValueStr(Miff * const miff, MiffStr * const str)
{
   returnFalse; // _MiffRead
}

/******************************************************************************
func: miffGetValueBin

bin should be large enough for the bin data.  I.E. it should be of size from
miffGetValueHeader() value.bufferCount
******************************************************************************/
MiffB miffGetValueBin(Miff * const miff, MiffN const binCount, MiffN1  * const binBuffer)
{
   returnFalse; // _MiffRead(miff, binCount, binBuffer);
}

/******************************************************************************
func: miffSetInfo
******************************************************************************/
MiffB miffSetInfo(Miff * const miff, MiffRecType const type, MiffN const count, 
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
      returnFalseIf(!_MiffWriteStr(miff, 1, " "));
   }

   // Write the type for the record.  Common for all cases.
   miff->currentArrayCount = count;
   miff->currentArrayIndex = 0;

   if      (type == miffRecTypeBLOCK_START)
   {
      returnFalseIf(!_MiffWriteStr(miff, 1, "{"));
      miff->isRecordDone = miffTRUE;
   }
   else if (type == miffRecTypeBLOCK_STOP)
   {
      returnFalseIf(!_MiffWriteStr(miff, 1, "}"));
      miff->isRecordDone = miffTRUE;
   }
   else if (count == miffArrayCountUNKNOWN)
   {
      returnFalseIf(!_MiffWriteStr(miff, 1, "*"));
   }
   else
   {
      returnFalseIf(!_MiffWriteN(miff, count));
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
   returnFalseIf(!_MiffWriteStr(
      miff, 
      miff->currentNameCount, 
      miff->currentName));
   
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
func: miffSetRecordEnd
******************************************************************************/
MiffB miffSetRecordEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->isRecordDone = miffTRUE;

   return _MiffWriteStr(miff, 1, "\n");
}

/******************************************************************************
func: miffSetSeparator
******************************************************************************/
MiffB miffSetSeparator(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteStr(miff, 1, "\t");
}

/******************************************************************************
func: miffSetValue
******************************************************************************/
MiffB miffSetValue(Miff * const miff, MiffValue const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   miffSetSeparator(miff);

   _MiffWriteValue(miff, value);
   
   miff->currentArrayIndex++;

   returnTrue;
}

/******************************************************************************
func: miffValueGetBool
******************************************************************************/
MiffB miffValueGetB(MiffValue value)
{
   returnFalseIf(value.type != miffValueTypeB);
   
   return value.b;
}

/******************************************************************************
func: miffValueGetI
******************************************************************************/
MiffI miffValueGetI(MiffValue value)
{
   return0If(value.type != miffValueTypeI);
   
   return value.inr.i;
}

/******************************************************************************
func: miffValueGetN
******************************************************************************/
MiffN miffValueGetN(MiffValue value)
{
   return0If(value.type != miffValueTypeN);
   
   return value.inr.n;
}

/******************************************************************************
func: miffValueIs4

Return the Real value byte count.
True if MiffR4
******************************************************************************/
MiffB miffValueIs4(MiffValue value)
{
   return0If(value.type != miffValueTypeR);

   return value.is4;
}

/******************************************************************************
func: miffValueGetR4
******************************************************************************/
MiffR4 miffValueGetR4(MiffValue value)
{
   return0If(value.type != miffValueTypeR);

   if (value.is4)
   {
      return value.inr4.r;
   }
 
   return (MiffR4) value.inr.r;
}

/******************************************************************************
func: miffValueGetR8
******************************************************************************/
MiffR miffValueGetR(MiffValue value)
{
   return0If(value.type != miffValueTypeR);

   if (value.is4)
   {
      return (MiffR) value.inr4.r;
   }

   return value.inr.r;
}

/******************************************************************************
func: miffValueGetStr
******************************************************************************/
MiffStr const *miffValueGetStrBuffer(MiffValue value)
{
   returnNullIf(value.type != miffValueTypeSTR);
   return value.bufferData.str;
}

/******************************************************************************
func: miffValueGetStrLen
******************************************************************************/
MiffN miffValueGetStrCount(MiffValue value)
{
   return0If(value.type != miffValueTypeSTR);
   return value.bufferCount;
}

/******************************************************************************
func: miffValueGetType
******************************************************************************/
MiffValueType miffValueGetType(MiffValue const value)
{
   return value.type;
}

/******************************************************************************
func: miffValueIs4
******************************************************************************/
MiffN miffValueGetIs4(MiffValue const value)
{
   return0If(
      value.type != miffValueTypeR ||
      value.type != miffValueTypeC);

   return value.is4;
}

/******************************************************************************
func: miffValueSetB
******************************************************************************/
MiffValue miffValueSetB(MiffB const ivalue)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type = miffValueTypeB;
   value.b    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetBinBuffer
******************************************************************************/
MiffValue miffValueSetBinBuffer(MiffN  const binCount, MiffN1  * const binBuffer)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type           = miffValueTypeBIN;
   value.bufferCount    = binCount;
   value.bufferData.bin = binBuffer;

   return value;
}

/******************************************************************************
func: miffValueSetI
******************************************************************************/
MiffValue miffValueSetI(MiffI const ivalue, MiffValueFormatCIR const format)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type      = miffValueTypeI;
   value.formatCIR = format;
   value.inr.i     = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetN
******************************************************************************/
MiffValue miffValueSetN(MiffN const ivalue, MiffValueFormatN const format)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type    = miffValueTypeN;
   value.formatN = format;
   value.inr.n   = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetNull
******************************************************************************/
MiffValue miffValueSetNull(void)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type = miffValueTypeNULL;

   return value;
}

/******************************************************************************
func: miffValueSetR4
******************************************************************************/
MiffValue miffValueSetR4(MiffR4 const ivalue, MiffValueFormatCIR const format)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type      = miffValueTypeR;
   value.formatCIR = format;
   value.is4       = miffTRUE;
   value.inr4.r    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetR
******************************************************************************/
MiffValue miffValueSetR(MiffR const ivalue, MiffValueFormatCIR const format)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type      = miffValueTypeR;
   value.formatCIR = format;
   value.inr.r     = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetStrBuffer
******************************************************************************/
MiffValue miffValueSetStrBuffer(MiffN const strCount, MiffStr * const strBuffer)
{
   MiffValue value;

   _MiffMemClearType(MiffValue, &value);

   value.type           = miffValueTypeSTR;
   value.bufferCount    = strCount;
   value.bufferData.str = strBuffer;

   return value;
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

   _MiffBase64Start();
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
   _MiffBase64Stop();

   _MiffMemStop();

   _isStarted = miffFALSE;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _WriteStr
******************************************************************************/
static MiffB _WriteStr(Miff * const miff, MiffN const strLen, MiffStr const * const str)
{
   MiffStr  *strEncoded;
   MiffN     strEncodedLen;
   MiffB  result;

   strEncoded = NULL;
   result     = miffFALSE;
   once
   {
      breakIf(!_MiffStrToStrEncoded(strLen, str, &strEncodedLen, &strEncoded));

      breakIf(!_MiffWriteStr(miff, strEncodedLen, strEncoded));

      result = miffTRUE;
   }

   _MiffMemDestroy(strEncoded);

   returnTrue;
}
