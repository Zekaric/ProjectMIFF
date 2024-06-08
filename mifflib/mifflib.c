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
   MiffStr * const subFormatName, MiffN8 * const subFormatVersion, void * const dataRepo)
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
   MiffGetBuffer getBufferFunc, MiffStr * const subFormatName, MiffN8 * const subFormatVersion,
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
   returnFalseIf(!_MiffReadPart(miff));
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      4, 
      (MiffN1 *) MIFF_HEADER_FILETYPE_STR));

   returnFalseIf(!_MiffReadPart(miff));
   miff->version = _MiffStrToN(miff->readByteCount, (MiffStr *) miff->readByteData);
   returnFalseIf(!_MiffMemIsEqual(
      miff->readByteCount, 
      miff->readByteData, 
      1, 
      (MiffN1 *) MIFF_HEADER_VERSION_STR));

   returnFalseIf(!_MiffReadPart(miff));
   _MiffStrToKey(miff->readByteCount, (MiffStr *) miff->readByteData, &ntemp, miff->subFormatName);
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, subFormatName, miff->subFormatName);
   returnFalseIf(!_MiffReadPart(miff));
   miff->subFormatVersion =
      *subFormatVersion   = _MiffStrToN(miff->readByteCount, miff->readByteData);

   returnFalseIf(!miff->readRecordIsDone);

   returnTrue;
}

/******************************************************************************
func: miffCreateWriter
******************************************************************************/
Miff *miffCreateWriter(MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc,
   MiffStr const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo)
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
   MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN8 const subFormatVersion,
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
   _MiffWriteStr(   miff, MIFF_HEADER_FILETYPE_SIZE, (MiffStr *) MIFF_HEADER_FILETYPE_STR);
   miffRecordSetEnd(miff);
   _MiffWriteStr(   miff, MIFF_HEADER_VERSION_SIZE,  (MiffStr *) MIFF_HEADER_VERSION_STR);
   miffRecordSetEnd(miff);

   _MiffWriteStr(   miff, _MiffStrGetCount(miff->subFormatName),    miff->subFormatName);
   miffRecordSetEnd(miff);
   _MiffWriteN(     miff, miff->subFormatVersion);
   miffRecordSetEnd(miff);

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
func: miffRecordGetValueBoolean
******************************************************************************/
MiffValue miffRecordGetValueBoolean(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeBOOLEAN  ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadPart(miff), value);

   value.type = miffTypeBOOLEAN;
   value.b    = miffBoolFALSE;
   if (miff->readByteData[0] == 'T')
   {
      value.b = miffBoolTRUE;
   }

   return value;
}

/******************************************************************************
func: miffRecordGetValueI
******************************************************************************/
MiffValue miffRecordGetValueI(Miff * const miff)
{
   MiffValue value;
   char     *endPtr;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeI1       ||
           miff->currentRecord.type == miffTypeI2       ||
           miff->currentRecord.type == miffTypeI4       ||
           miff->currentRecord.type == miffTypeI8       ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadPart(miff), value);

   switch (miff->currentRecord.type)
   {
   case miffTypeI1:
   case miffTypeI2:
   case miffTypeI4:
   case miffTypeI8:
      value.type = miff->currentRecord.type;
      break;

   default:
      value.type = miffTypeI;
      break;
   }

   value.i = (MiffI8) _strtoi64((char *) miff->readByteData, &endPtr, 10);
   
   return value;
}

/******************************************************************************
func: miffRecordGetValueN
******************************************************************************/
MiffValue miffRecordGetValueN(Miff * const miff)
{
   MiffValue value;
   char     *endPtr;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeN1       ||
           miff->currentRecord.type == miffTypeN2       ||
           miff->currentRecord.type == miffTypeN4       ||
           miff->currentRecord.type == miffTypeN8       ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadPart(miff), value);

   switch (miff->currentRecord.type)
   {
   case miffTypeN1:
   case miffTypeN2:
   case miffTypeN4:
   case miffTypeN8:
      value.type = miff->currentRecord.type;
      break;

   default:
      value.type = miffTypeN;
      break;
   }

   value.n = (MiffN8) _strtoui64((char *) miff->readByteData, &endPtr, 10);
   
   return value;
}

/******************************************************************************
func: miffRecordGetValueR4
******************************************************************************/
MiffValue miffRecordGetValueR4(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeR4       ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadR4(miff, &value), value);

   value.type = miffTypeR4;

   return value;
}

/******************************************************************************
func: miffRecordGetValueR4S
******************************************************************************/
MiffValue miffRecordGetValueR4S(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeR4S      ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadR4S(miff, &value), value);

   value.type = miffTypeR4;

   return value;
}

/******************************************************************************
func: miffRecordGetValueR8
******************************************************************************/
MiffValue miffRecordGetValueR8(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeR8       ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadR8(miff, &value), value);

   value.type = miffTypeR8;

   return value;
}

/******************************************************************************
func: miffRecordGetValueR8S
******************************************************************************/
MiffValue miffRecordGetValueR8S(Miff * const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeR8S      ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadR8S(miff, &value), value);

   value.type = miffTypeR8;

   return value;
}

/******************************************************************************
func: miffRecordGetValueStr

value is not dynamic.  Caller must clone the string for persistence.
******************************************************************************/
MiffValue miffRecordGetValueStr(Miff *const miff)
{
   MiffValue value;

   value.type = miffTypeNONE;
   value.str  = NULL;

   returnIf(
         !_isStarted ||
         !miff       ||
         !(miff->currentRecord.type == miffTypeSTR      ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE),
      value);

   returnIf(!_MiffReadPart(miff), value);

   _MiffStrEncodedToStr(&miff->readByteCount, miff->readByteData);

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
func: miffRecordGetInfo

key needs to be a buffer of size miffKeySIZE.
******************************************************************************/
MiffBool miffRecordGetInfo(Miff * const miff, MiffType * const type, MiffStr * const typeName,
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
   if (*type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      return miff->readRecordIsDone;
   }

   // Read in the name of the record
   returnFalseIf(!_MiffReadKey(miff, miff->currentRecord.name));
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffStr, key, miff->currentRecord.name);

   // Special case,
   if (*type == miffTypeKEY_VALUE_BLOCK_START)
   {
      return miff->readRecordIsDone;
   }

   // Read in the array count
   returnFalseIf(!_MiffReadArrayCount(miff, &miff->currentRecord.arrayCount));
   *count = miff->currentRecord.arrayCount;

   returnTrue;
}

/******************************************************************************
func: miffRecordGetEnd

Skip to the end of the record if not at the end already.
******************************************************************************/
MiffBool miffRecordGetEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnTrueIf(miff->readRecordIsDone);

   returnFalseIf(!_MiffReadLineSkip(miff));

   miff->readRecordIsDone = miffBoolTRUE;

   returnTrue;
}

#if 0
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
MiffStr *miffRecordGetName(Miff * const miff)
{
   returnNullIf(
      !_isStarted ||
      !miff       ||
      miff->currentRecord.type == miffTypeNONE);

   return miff->currentRecord.name;
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
#endif

/******************************************************************************
func: miffRecordSetBlockStart
******************************************************************************/
MiffBool miffRecordSetBlockStart(Miff * const miff, MiffStr const * const key)
{
   return miffRecordSetInfo(miff, miffTypeKEY_VALUE_BLOCK_START, NULL, key, miffArrayCountUNKNOWN);
}

/******************************************************************************
func: miffRecordSetBlockStop
******************************************************************************/
MiffBool miffRecordSetBlockStop(Miff * const miff)
{
   return miffRecordSetInfo(miff, miffTypeKEY_VALUE_BLOCK_STOP, NULL, NULL, miffArrayCountUNKNOWN);
}

/******************************************************************************
func: miffRecordSetEnd
******************************************************************************/
MiffBool miffRecordSetEnd(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->currentRecord.type = miffTypeNONE;

   return _MiffWriteStr(miff, 1, (MiffStr *) "\n");
}

/******************************************************************************
func: miffRecordSetInfo
******************************************************************************/
MiffBool miffRecordSetInfo(Miff * const miff, MiffType const type, MiffStr const * const typeName, 
   MiffStr const * const key, MiffN const count)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Write the type for the record.  Common for all cases.
   if (type == miffTypeUSER_TYPE)
   {
      returnFalseIf(!_MiffWriteStr(miff, _MiffStrGetCount(typeName), typeName));
   }
   else
   {
      returnFalseIf(!_MiffWriteStr(miff, _MiffTypeGetNameSize(type), _MiffTypeGetName(type)));
   }

   // We are ending a key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_STOP)
   {
      // TODO key value block stack pop.
      returnFalseIf(!miffRecordSetEnd(miff));
      returnTrue;
   }

   returnFalseIf(!key);

   // Copy the key.
   miff->currentArrayIndex        = 0;
   miff->currentRecord.type       = type;
   miff->currentRecord.arrayCount = count;
   _MiffMemClearTypeArray(miffKeySIZE,           MiffStr, miff->currentRecord.name);
   _MiffMemCopyTypeArray( _MiffStrGetCount(key), MiffStr, miff->currentRecord.name, key);

   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteStr(         miff, _MiffStrGetCount(miff->currentRecord.name), miff->currentRecord.name));
   // We are starting a new key value block.
   if (type == miffTypeKEY_VALUE_BLOCK_START)
   {
      // TODO key value block stack push.
      return miffRecordSetEnd(miff);
   }

   // Write out the record.
   returnFalseIf(!miffRecordSetSeparator(miff));
   returnFalseIf(!_MiffWriteN(           miff, count));

   returnTrue;
}

/******************************************************************************
func: miffRecordSetSeparator
******************************************************************************/
MiffBool miffRecordSetSeparator(Miff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffWriteStr(miff, 1, (MiffStr *) "\t");
}

/******************************************************************************
func: miffRecordSetValue
******************************************************************************/
MiffBool miffRecordSetValue(Miff * const miff, MiffValue const value)
{
   MiffStr *str;

   returnFalseIf(
      !_isStarted ||
      !miff);

   miffRecordSetSeparator(miff);

   switch (value.type)
   {
   case miffTypeBOOLEAN:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeBOOLEAN  ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      str = (MiffStr *) ((value.b) ? "T" : "F");

      returnFalseIf(!_MiffWriteStr(miff, 1, str));
      break;

   case miffTypeI1:
   case miffTypeI2:
   case miffTypeI4:
   case miffTypeI8:
   case miffTypeI:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeI1       ||
           miff->currentRecord.type == miffTypeI2       ||
           miff->currentRecord.type == miffTypeI4       ||
           miff->currentRecord.type == miffTypeI8       ||
           miff->currentRecord.type == miffTypeI        ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_MiffWriteI(miff, value.i));
      break;

   case miffTypeN1:
   case miffTypeN2:
   case miffTypeN4:
   case miffTypeN8:
   case miffTypeN:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeN1       ||
           miff->currentRecord.type == miffTypeN2       ||
           miff->currentRecord.type == miffTypeN4       ||
           miff->currentRecord.type == miffTypeN8       ||
           miff->currentRecord.type == miffTypeN        ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_MiffWriteN(miff, value.n));
      break;

   case miffTypeR4:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeR4       ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_MiffWriteR4(miff, value.r4));
      break;

   case miffTypeR4S:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeR4S      ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_MiffWriteR4S(miff, value.r4));
      break;

   case miffTypeR8:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeR8       ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_MiffWriteR8(miff, value.r8));
      break;

   case miffTypeR8S:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeR8S      ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_MiffWriteR8S(miff, value.r8));
      break;

   case miffTypeSTR:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeSTR      ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

      returnFalseIf(!_WriteStr(miff, value.strLen, value.str));
      break;

   case miffTypeTYPE:
      returnFalseIf(
         !(miff->currentRecord.type == miffTypeTYPE     ||
           miff->currentRecord.type == miffTypeVARIABLE ||
           miff->currentRecord.type == miffTypeUSER_TYPE));

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
   returnFalseIf(value.type != miffTypeBOOLEAN);
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
func: miffValueGetR4
******************************************************************************/
MiffR4 miffValueGetR4(MiffValue value)
{
   return0If(value.type != miffTypeR4);
   return value.r4;
}

/******************************************************************************
func: miffValueGetR8
******************************************************************************/
MiffR8 miffValueGetR8(MiffValue value)
{
   return0If(value.type != miffTypeR8);
   return value.r8;
}

/******************************************************************************
func: miffValueGetStr
******************************************************************************/
MiffStr *miffValueGetStr(MiffValue value)
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

   value.type = miffTypeBOOLEAN;
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

   value.type = miffTypeR4;
   value.r4   = ivalue;

   return value;
}

/******************************************************************************
func: miffValueSetR8
******************************************************************************/
MiffValue miffValueSetR8(MiffR8 const ivalue)
{
   MiffValue value;

   value.type = miffTypeR8;
   value.r8   = ivalue;

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
   value.str    = (MiffStr *) str;

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
