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
static MiffBool   _isStarted = miffBoolFALSE;

/******************************************************************************
prototype:
******************************************************************************/
static MiffBool   _WriteStr(  Miff * const miff, MiffN const strLen, MiffStr const * const str);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: miffCreateReader
******************************************************************************/
Miff *miffCreateReader(MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc,
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
MiffBool miffCreateReaderContent(Miff * const miff, MiffBool const isByteSwaping,
   MiffGetBuffer getBufferFunc, MiffStr * const subFormatName, MiffN * const subFormatVersion,
   void * const dataRepo)
{
   MiffN ntemp;

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
   miff->readRecordIsDone = miffBoolFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffBoolFALSE));
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      4, 
      (MiffN1 *) MIFF_HEADER_FILETYPE_STR));

   miff->readRecordIsDone = miffBoolFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffBoolFALSE));
   miff->version = _MiffStrToN(miff->readByteCount, (MiffStr *) miff->readByteData);
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      1, 
      (MiffN1 *) MIFF_HEADER_VERSION_STR));

   miff->readRecordIsDone = miffBoolFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffBoolFALSE));
   _MiffStrToKey(miff->readByteCount, (MiffStr *) miff->readByteData, &ntemp, miff->subFormatName);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, subFormatName, miff->subFormatName);

   miff->readRecordIsDone = miffBoolFALSE;
   returnFalseIf(!_MiffReadPart(miff, miffBoolFALSE));
   miff->subFormatVersion =
      *subFormatVersion   = _MiffStrToN(miff->readByteCount, (MiffStr *) miff->readByteData);

   returnFalseIf(!miff->readRecordIsDone);

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc,
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
MiffBool miffCreateWriterContent(Miff * const miff, MiffBool const isByteSwaping,
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
func: miffGetValueBoolean
******************************************************************************/
MiffValue miffGetValueBoolean(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeBOOL ||
           miff->currentRecord.type == miffTypeOTHER),
      value);

   returnIf(!_MiffReadPart(miff, miffBoolFALSE), value);

   value.type = miffTypeBOOL;
   value.b    = miffBoolFALSE;
   if (miff->readByteData[0] == 'T')
   {
      value.b = miffBoolTRUE;
   }

   return value;
}

/******************************************************************************
func: miffGetValueI
******************************************************************************/
MiffValue miffGetValueI(Miff * const miff)
{
   MiffValue value;
   char     *endPtr;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeI ||
           miff->currentRecord.type == miffTypeOTHER),
      value);

   returnIf(!_MiffReadPart(miff, miffBoolFALSE), value);

   switch (miff->currentRecord.type)
   {
   case miffTypeI:
      value.type = miff->currentRecord.type;
      break;

   default:
      value.type = miffTypeI;
      break;
   }

   value.i = (MiffI) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   return value;
}

/******************************************************************************
func: miffGetValueN
******************************************************************************/
MiffValue miffGetValueN(Miff * const miff)
{
   MiffValue value;
   char     *endPtr;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeN ||
           miff->currentRecord.type == miffTypeOTHER),
      value);

   returnIf(!_MiffReadPart(miff, miffBoolFALSE), value);

   switch (miff->currentRecord.type)
   {
   case miffTypeN:
      value.type = miff->currentRecord.type;
      break;

   default:
      value.type = miffTypeN;
      break;
   }

   value.n = (MiffN) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   return value;
}

/******************************************************************************
func: miffGetValueR
******************************************************************************/
MiffValue miffGetValueR(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeR ||
           miff->currentRecord.type == miffTypeOTHER),
      value);

   returnIf(!_MiffReadR(miff, &value), value);

   value.type = miffTypeR;

   return value;
}

#if 0
/******************************************************************************
func: miffGetValueR_FIXED
******************************************************************************/
MiffValue miffGetValueR_FIXED(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeR ||
           miff->currentRecord.type == miffTypeOTHER),
      value);

   returnIf(!_MiffReadRFixed(miff, &value), value);

   value.type = miffTypeR_FIXED;

   return value;
}
#endif

/******************************************************************************
func: miffGetValueStr

value is not dynamic.  Caller must clone the string for persistence.
******************************************************************************/
MiffValue miffGetValueStr(Miff *const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;
   value.str  = NULL;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeSTR ||
           miff->currentRecord.type == miffTypeOTHER),
      value);

   returnIf(!_MiffReadPart(miff, miffBoolFALSE), value);

   _MiffStrEncodedToStr(&miff->readByteCount, (MiffStr *) miff->readByteData);

   // Adjust the str buffer.
   if (miff->readByteCount >= miff->readStrCountActual)
   {
      _MiffMemDestroy(miff->readStrData);

      // Get the new buffer size.
      miff->readStrCountActual = ((miff->readByteCount / 1024) + 2) * 1024;
      miff->readStrData        = _MiffMemCreateTypeArray(miff->readStrCountActual, MiffStr);

      // Ran out of memory.
      returnIf(!miff->readStrData, value);
   }

   // Clear the string.
   _MiffMemClearTypeArray(miff->readStrCountActual, MiffStr, miff->readStrData);
   
   // Copy the string over to the temporary string holder.
   _MiffMemCopyTypeArray(miff->readStrCount, MiffStr, miff->readStrData, miff->readByteData);
   miff->readStrCount = miff->readByteCount;

   // Return string.
   value.strLen = miff->readStrCount;
   value.str    = miff->readStrData;

   return value;
}

/******************************************************************************
func: miffGetInfo

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffBool miffGetInfo(Miff * const miff, MiffType * const type, MiffStr * const typeName,
   MiffStr * const key, MiffN * const count)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->readRecordIsDone = miffBoolFALSE;

   *type                  = miffTypeNONE;
   *count                 = 0;

   // Clear the key.
   _MiffMemClearTypeArray(miffKeySIZE, MiffStr, key);

   // Read in the type.
   returnFalseIf(!_MiffReadType(miff, &miff->currentRecord.type, typeName));
   *type = miff->currentRecord.type;

   // Special case block end
   if (*type == miffTypeBLOCK_STOP)
   {
      return miff->readRecordIsDone;
   }

   // Read in the name of the record
   returnFalseIf(!_MiffReadKey(miff, miff->currentRecord.name));
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, key, miff->currentRecord.name);

   // Special case,
   if (*type == miffTypeBLOCK_START)
   {
      return miff->readRecordIsDone;
   }

   // Read in the array count
   returnFalseIf(!_MiffReadArrayCount(miff, &miff->currentRecord.arrayCount));
   *count = miff->currentRecord.arrayCount;

   returnTrue;
}

/******************************************************************************
func: miffGetRecordEnd

Skip to the end of the record if not at the end already.
******************************************************************************/
MiffBool miffGetRecordEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnTrueIf(miff->readRecordIsDone);

   returnFalseIf(!_MiffReadLineSkip(miff));

   miff->readRecordIsDone = miffBoolTRUE;

   returnTrue;
}

/******************************************************************************
func: miffSetRecordEnd
******************************************************************************/
MiffBool miffSetRecordEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->currentRecord.type = miffTypeNONE;

   return _MiffWriteStr(miff, 1, "\n");
}

/******************************************************************************
func: miffSetInfo
******************************************************************************/
MiffBool miffSetInfo(Miff * const miff, MiffType const type, MiffStr const * const typeName, 
   MiffStr const * const key, MiffN const count)
{
   MiffN index;

   returnFalseIf(
      !_isStarted ||
      !miff);

   // Adjust the scope level before we indent.
   if (type == miffTypeBLOCK_STOP)
   {
      if (miff->currentScopeLevel != 0)
      {
         miff->currentScopeLevel--;
      }
   }

   // Add indents to the current scope level.
   forCount(index, miff->currentScopeLevel)
   {
      returnFalseIf(!_MiffWriteStr(miff, 1, "\t"));
   }

   // Write the type for the record.  Common for all cases.
   if      (type == miffTypeOTHER)
   {
      returnFalseIf(!_MiffWriteStr(miff, _MiffStrGetCount(typeName), typeName));
   }
   else if (type == miffTypeNONE)
   {
      returnFalse;
   }
   else
   {
      returnFalseIf(!_MiffWriteStr(miff, _MiffTypeGetNameSize(type), _MiffTypeGetName(type)));
   }

   // We are ending a key value block.
   if (type == miffTypeBLOCK_STOP)
   {
      // TODO key value block stack pop.
      returnFalseIf(!miffSetRecordEnd(miff));
      returnTrue;
   }

   returnFalseIf(!key);

   // Copy the key.
   miff->currentArrayIndex        = 0;
   miff->currentRecord.type       = type;
   miff->currentRecord.arrayCount = count;
   _MiffMemClearTypeArray(miffKeySIZE,           MiffStr, miff->currentRecord.name);
   _MiffMemCopyTypeArray( _MiffStrGetCount(key), MiffStr, miff->currentRecord.name, key);

   returnFalseIf(!miffSetSeparator(miff));
   returnFalseIf(!_MiffWriteStr(   miff, _MiffStrGetCount(miff->currentRecord.name), miff->currentRecord.name));
   // We are starting a new key value block.
   if (type == miffTypeBLOCK_START)
   {
      miff->currentScopeLevel++;

      // TODO key value block stack push.
      return miffSetRecordEnd(miff);
   }

   // Write out the record.
   returnFalseIf(!miffSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(     miff, count));

   returnTrue;
}

/******************************************************************************
func: miffSetSeparator
******************************************************************************/
MiffBool miffSetSeparator(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteStr(miff, 1, "\t");
}

/******************************************************************************
func: miffSetValue
******************************************************************************/
MiffBool miffSetValue(Miff * const miff, MiffValue const value)
{
   MiffStr *str;

   returnFalseIf(
      !_isStarted ||
      !miff);

   miffSetSeparator(miff);

   switch (value.type)
   {
   case miffTypeBOOL:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeBOOL ||
           miff->currentRecord.type == miffTypeOTHER));

      str = ((value.b) ? "T" : "F");

      returnFalseIf(!_MiffWriteStr(miff, 1, str));
      break;

   case miffTypeI:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeI ||
           miff->currentRecord.type == miffTypeOTHER));

      returnFalseIf(!_MiffWriteI(miff, value.i));
      break;

   case miffTypeN:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeN ||
           miff->currentRecord.type == miffTypeOTHER));

      returnFalseIf(!_MiffWriteN(miff, value.n));
      break;

   case miffTypeR:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeR ||
           miff->currentRecord.type == miffTypeOTHER));

      if (value.rType == 4)
      {
         returnFalseIf(!_MiffWriteR4(miff, value.r4));
      }
      else
      {
         returnFalseIf(!_MiffWriteR8(miff, value.r8));
      }
      break;

#if 0
   case miffTypeR_FIXED:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeR8 ||
           miff->currentRecord.type == miffTypeOTHER));

      returnFalseIf(!_MiffWriteI(  miff, value.rFixedWhole));
      if (value.rFixedDecimal != 0)
      {
         returnFalseIf(!_MiffWriteStr(miff, 1, "."));
         returnFalseIf(!_MiffWriteN(  miff, value.rFixedWhole));
      }
      break;
#endif

   case miffTypeSTR:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeSTR ||
           miff->currentRecord.type == miffTypeOTHER));

      returnFalseIf(!_WriteStr(miff, value.strLen, value.str));
      break;

   case miffTypeTYPE:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeTYPE ||
           miff->currentRecord.type == miffTypeOTHER));

      returnFalseIf(!_MiffWriteStr(miff, _MiffTypeGetNameSize(value.t), _MiffTypeGetName(value.t)));
      break;
   }

   returnTrue;
}

/******************************************************************************
func: miffValueGetValueType
******************************************************************************/
MiffType miffValueGetValueType(MiffValue value)
{
   return value.type;
}

/******************************************************************************
func: miffValueGetBool
******************************************************************************/
MiffBool miffValueGetBool(MiffValue value)
{
   returnFalseIf(value.type != miffTypeBOOL);
   return value.b;
}

/******************************************************************************
func: miffValueGetI
******************************************************************************/
MiffI miffValueGetI(MiffValue value)
{
   return0If(value.type != miffTypeI);
   return value.i;
}

/******************************************************************************
func: miffValueGetN
******************************************************************************/
MiffN miffValueGetN(MiffValue value)
{
   return0If(value.type != miffTypeN);
   return value.n;
}

/******************************************************************************
func: miffValueGetRType

Return the Real value byte count.
4 for MiffR4
8 for MiffR8
******************************************************************************/
MiffN miffValueGetRType(MiffValue value)
{
   return0If(value.type != miffTypeR);

   return value.rType;
}

/******************************************************************************
func: miffValueGetR4
******************************************************************************/
MiffR4 miffValueGetR4(MiffValue value)
{
   return0If(value.type != miffTypeR);

   if (value.rType == 0)
   {
      return value.r4;
   }
 
   return (MiffR4) value.r8;
}

/******************************************************************************
func: miffValueGetR8
******************************************************************************/
MiffR8 miffValueGetR8(MiffValue value)
{
   return0If(value.type != miffTypeR);

   if (value.rType == 8)
   {
      return value.r8;
   }

   return (MiffR8) value.r4;
}

/******************************************************************************
func: miffValueGetStr
******************************************************************************/
MiffStr const *miffValueGetStr(MiffValue value)
{
   returnNullIf(value.type != miffTypeSTR);
   return value.str;
}

/******************************************************************************
func: miffValueGetStrLen
******************************************************************************/
MiffN miffValueGetStrLen(MiffValue value)
{
   return0If(value.type != miffTypeSTR);
   return value.strLen;
}

/******************************************************************************
func: miffValueGetType
******************************************************************************/
MiffType MiffValueGetType(MiffValue value)
{
   returnIf(value.type != miffTypeTYPE, miffTypeNONE);
   return value.t;
}

/******************************************************************************
func: miffValueSetBoolean
******************************************************************************/
MiffValue miffValueSetBoolean(MiffBool const ivalue)
{
   MiffValue value;

   value.type = miffTypeBOOL;
   value.b    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetI
******************************************************************************/
MiffValue miffValueSetI(MiffI const ivalue)
{
   MiffValue value;

   value.type = miffTypeI;
   value.i    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetN
******************************************************************************/
MiffValue miffValueSetN(MiffN const ivalue)
{
   MiffValue value;

   value.type = miffTypeN;
   value.n    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetR4
******************************************************************************/
MiffValue miffValueSetR4(MiffR4 const ivalue)
{
   MiffValue value;

   value.type  = miffTypeR;
   value.rType = 4;
   value.r4    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetR8
******************************************************************************/
MiffValue miffValueSetR8(MiffR8 const ivalue)
{
   MiffValue value;

   value.type  = miffTypeR;
   value.rType = 8;
   value.r8    = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetStr
******************************************************************************/
MiffValue miffValueSetStr(MiffN const strLen, MiffStr const * const str)
{
   MiffValue value;

   value.type   = miffTypeSTR;
   value.strLen = strLen;
   value.str    = str;

   return value;
}

/******************************************************************************
func: miffValueSetType
******************************************************************************/
MiffValue miffValueSetType(MiffType const ivalue)
{
   MiffValue value;

   value.type = miffTypeTYPE;
   value.t    = ivalue;

   return value;
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

   _MiffMemStart(memCreateFunc, memDestroyFunc);

   _isStarted = miffBoolTRUE;

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

   _MiffMemStop();

   _isStarted = miffBoolFALSE;
}

/******************************************************************************
func: miffTypeGetStr
******************************************************************************/
MiffStr const *miffTypeGetStr(MiffType const type)
{
   return _MiffTypeGetName(type);
}

/******************************************************************************
func: miffTypeGetC2
******************************************************************************/
MiffStr const *miffTypeGetC2(MiffType const type)
{
   return _MiffTypeGetName(type);
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _WriteStr
******************************************************************************/
static MiffBool _WriteStr(Miff * const miff, MiffN const strLen, MiffStr const * const str)
{
   MiffStr  *strEncoded;
   MiffN     strEncodedLen;
   MiffBool  result;

   strEncoded = NULL;
   result     = miffBoolFALSE;
   once
   {
      breakIf(!_MiffStrToStrEncoded(strLen, str, &strEncodedLen, &strEncoded));

      breakIf(!_MiffWriteStr(miff, strEncodedLen, strEncoded));

      result = miffBoolTRUE;
   }

   _MiffMemDestroy(strEncoded);

   returnTrue;
}
