/**************************************************************************************************
file:       gmifflib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw MIFF files.
**************************************************************************************************/

/**************************************************************************************************
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
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "gmifflib.h"
#include "gmiff_local.h"

/**************************************************************************************************
local:
variable:
**************************************************************************************************/
static Gb          _isStarted      = gbFALSE;
static Gb          _isByteSwapping = gbFALSE;
#if 0
static _locale_t   _locale;
#endif

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmiffClocReader
**************************************************************************************************/
Gmiff *gmiffClocReader(GgetBuffer getBufferFunc, Gstr * const subFormatName,
   Gstr * const subFormatVersion, void * const dataRepo)
{
   Gmiff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure.
   miff = _MiffMemClocType(Gmiff);
   returnNullIf(!miff);

   // Initialize the miff structure.
   if (!gmiffClocReaderContent(miff, getBufferFunc, subFormatName, subFormatVersion, dataRepo))
   {
      _MiffMemDloc(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/**************************************************************************************************
func: gmiffClocReaderContent
**************************************************************************************************/
Gb gmiffClocReaderContent(Gmiff * const miff, GgetBuffer getBufferFunc, Gstr * const subFormatName,
   Gstr * const subFormatVersion, void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !getBufferFunc);

   _MiffMemClearType(miff, Gmiff);
   miff->method    = gmethodREAD;
   miff->dataRepo  = dataRepo;
   miff->getBuffer = getBufferFunc;

   // Read the header.
   miff->isRecordDone = gbFALSE;
   returnFalseIf(!_MiffGetPart(miff, gbFALSE));
   returnFalseIf(!strIsEqual(miff->readCount, (char *) miff->readData, MIFF_HEADER_FILETYPE_STR));

   miff->isRecordDone = gbFALSE;
   returnFalseIf(!_MiffGetPart(miff, gbFALSE));
   returnFalseIf(!_MiffGetNumInt(miff, miff->readCount, miff->readData));
   miff->version = miff->value.inr.n;
   returnFalseIf(!strIsEqual(miff->readCount, (char *) miff->readData, MIFF_HEADER_VERSION_STR));

   miff->isRecordDone = gbFALSE;
   returnFalseIf(!_MiffGetPart(miff, gbFALSE));
   _MiffGetKey(miff);
   _MiffMemCopyTypeArray(subFormatName,       Gstr, GkeyBYTE_COUNT, miff->currentName);
   _MiffMemCopyTypeArray(miff->subFormatName, Gstr, GkeyBYTE_COUNT, miff->currentName);

   miff->isRecordDone = gbFALSE;
   returnFalseIf(!_MiffGetPart(miff, gbFALSE));
   _MiffGetKey(miff);
   _MiffMemCopyTypeArray(subFormatVersion,       Gstr, GkeyBYTE_COUNT, miff->currentName);
   _MiffMemCopyTypeArray(miff->subFormatVersion, Gstr, GkeyBYTE_COUNT, miff->currentName);

   returnFalseIf(!miff->isRecordDone);

   returnTrue;
}

/**************************************************************************************************
func: gmiffClocWriter
**************************************************************************************************/
Gmiff *gmiffClocWriter(GsetBuffer setBufferFunc, Gstr const * const subFormatName,
   Gstr const * const subFormatVersion, void * const dataRepo)
{
   Gmiff *miff;

   returnNullIf(!_isStarted);

   // Create the miff structure
   miff = _MiffMemClocType(Gmiff);
   returnNullIf(!miff);

   // Initialize the structure
   if (!gmiffClocWriterContent(miff, setBufferFunc, subFormatName, subFormatVersion, dataRepo))
   {
      _MiffMemDloc(miff);
      returnNull;
   }

   // return the miff structure
   return miff;
}

/**************************************************************************************************
func: gmiffClocWriterContent
**************************************************************************************************/
Gb gmiffClocWriterContent(Gmiff * const miff, GsetBuffer setBufferFunc,
   Gstr const * const subFormatName, Gstr const * const subFormatVersion, void * const dataRepo)
{
   Gn4 count;

   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !setBufferFunc  ||
      !subFormatName  ||
      !subFormatVersion);

   _MiffMemClearType(miff, Gmiff);
   miff->version              = 1;
   miff->method               = gmethodWRITE;
   miff->dataRepo             = dataRepo;
   miff->setBuffer            = setBufferFunc;

   count = min(255, _MiffStrGetCount(subFormatName));
   _MiffMemCopyTypeArray(miff->subFormatName,    Gstr, count, subFormatName);
   count = min(255, _MiffStrGetCount(subFormatVersion));
   _MiffMemCopyTypeArray(miff->subFormatVersion, Gstr, count, subFormatVersion);

   // Write the miff header.
   _MiffSetBuffer(   miff, MIFF_HEADER_FILETYPE_SIZE, (Gn1 *) MIFF_HEADER_FILETYPE_STR);
   gmiffSetRecordStop(miff);
   _MiffSetBuffer(   miff, MIFF_HEADER_VERSION_SIZE,  (Gn1 *) MIFF_HEADER_VERSION_STR);
   gmiffSetRecordStop(miff);

   _MiffSetBuffer(   miff, _MiffStrGetCount(miff->subFormatName),    (Gn1 *) miff->subFormatName);
   gmiffSetRecordStop(miff);
   _MiffSetBuffer(   miff, _MiffStrGetCount(miff->subFormatVersion), (Gn1 *) miff->subFormatVersion);
   gmiffSetRecordStop(miff);

   returnTrue;
}

/**************************************************************************************************
func: gmiffDloc
**************************************************************************************************/
void gmiffDloc(Gmiff * const miff)
{
   returnVoidIf(
      !_isStarted ||
      !miff);

   gmiffDlocContent(miff);
}

/**************************************************************************************************
func: gmiffDlocContent
**************************************************************************************************/
void gmiffDlocContent(Gmiff * const miff)
{
   returnVoidIf(
      !_isStarted ||
      !miff);

   _MiffMemDloc(miff);
}

/**************************************************************************************************
func: gmiffGetRecordStart

key needs to be a buffer of size GkeySIZE.
**************************************************************************************************/
Gb gmiffGetRecordStart(Gmiff * const miff, GmiffRecType * const type, Gcount * const count,
   Gstr * const key)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->isRecordDone = gbFALSE;

   *type              = gmiffRecTypeNONE;
   *count             = 0;

   // Clear the key.
   _MiffMemClearTypeArray(key, Gstr, GkeySIZE);

   // Read in the type.
   returnFalseIf(!_MiffGetPart(miff, gbTRUE));

   switch (miff->readData[0])
   {
   case '{':
      *type                   = gmiffRecTypeBLOCK_START;
      miff->currentArrayCount = miffArrayCountBLOCK_START;
      break;

   case '}':
      *type                   = gmiffRecTypeBLOCK_STOP;
      miff->currentArrayCount = miffArrayCountBLOCK_STOP;
      break;

   case '*':
      *type                   = gmiffRecTypeVALUE;
      miff->currentArrayCount =
         *count               =  miffArrayCountUNKNOWN;
      break;

   default:
      *type                   = gmiffRecTypeVALUE;
      returnFalseIf(!_MiffGetNumInt(miff, miff->readCount, miff->readData));
      miff->currentArrayCount =
         *count               = (Gcount) miff->value.inr.n;
      break;
   }

   returnTrueIf(miff->currentArrayCount == miffArrayCountBLOCK_STOP);

   // Read in the name of the record
   returnFalseIf(!_MiffGetPart(miff, gbFALSE));
   returnFalseIf(!_MiffGetKey( miff));

   _MiffMemCopyTypeArray(key, Gstr, miff->currentNameCount, miff->currentName);

   returnTrueIf(miff->currentArrayCount == miffArrayCountBLOCK_START);

   returnTrue;
}

/**************************************************************************************************
func: gmiffGetRecordEnd

Skip to the end of the record if not at the end already.
**************************************************************************************************/
Gb gmiffGetRecordEnd(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnTrueIf(miff->isRecordDone);

   returnFalseIf(!_MiffGetLineSkip(miff));

   miff->isRecordDone = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmiffGetValueBin

bin should be large enough for the bin data.  I.E. it should be of size from
gmiffGetValue() value.bufferCount
**************************************************************************************************/
Gb gmiffGetValueBin(Gmiff * const miff, Gcount const binCount, Gn1 * const binBuffer)
{
   Gn8 index;

   returnFalseIf(
      !_isStarted                          ||
      !miff                                ||
      miff->value.type != gmiffValueTypeBIN ||
      binCount != miff->value.bufferCount  ||
      !binBuffer);

   forCount(index, binCount)
   {
      returnFalseIf(!gmiffGetValueBinData(miff, &binBuffer[index]));
   }

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/**************************************************************************************************
func: miffValueBinGetData
**************************************************************************************************/
Gb gmiffGetValueBinData(Gmiff * const miff, Gn1 * const binByte)
{
   returnFalseIf(
      !_isStarted                          ||
      !miff                                ||
      miff->value.type != gmiffValueTypeBIN ||
      !binByte                             ||
      miff->valueIndex >= miff->value.bufferCount);

   return _MiffGetBinByte(miff, binByte);
}

/**************************************************************************************************
func: gmiffGetValue
**************************************************************************************************/
GmiffValue gmiffGetValue(Gmiff * const miff)
{
   Gstr   header;
   GmiffValue vtemp;

   _MiffMemClearType(&vtemp, GmiffValue);

   returnIf(
         !_isStarted ||
         !miff,
      vtemp);

   miff->isPartDone = gbFALSE;
   _MiffMemClearType(&miff->value, GmiffValue);

   miff->value.type = gmiffValueTypeNONE;
   miff->valueIndex = 0;

   header = _MiffGetValueHeader(miff);
   switch (header)
   {
   default:
      miff->value.type        = gmiffValueTypeOTHER;
      miff->value.header      = header;
      // Reading the rest of the value is up to the caller.
      break;

   case 0:
      break;

   case '~':
      miff->value.type        = gmiffValueTypeNULL;
      _MiffGetPartEnd(miff);
      break;

   case '"':
      miff->value.type        = gmiffValueTypeSTR;
      miff->value.bufferCount = _MiffGetValueBufferCount(miff);
      // Reading the string is done with gmiffGetValueStr() or gmiffGetValueStrData();
      break;

   case '-':
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case 'A':
   case 'B':
   case 'C':
   case 'D':
   case 'E':
   case 'F':
      miff->value.type        = gmiffValueTypeNUM_INT;
      _MiffGetPartRest(miff, header);
      _MiffGetNumInt(  miff, miff->readCount, miff->readData);
      break;

   case 'G': // 0
   case 'H': // 1
   case 'I': // 2
   case 'J': // 3
   case 'K': // 4
   case 'L': // 5
   case 'M': // 6
   case 'N': // 7
   case 'O': // 8
   case 'P': // 9
   case 'Q': // A
   case 'R': // B
   case 'S': // C
   case 'T': // D
   case 'U': // E
   case 'V': // F
   case 'g': // 0
   case 'h': // 1
   case 'i': // 2
   case 'j': // 3
   case 'k': // 4
   case 'l': // 5
   case 'm': // 6
   case 'n': // 7
   case 'o': // 8
   case 'p': // 9
   case 'q': // A
   case 'r': // B
   case 's': // C
   case 't': // D
   case 'u': // E
   case 'v': // F
      miff->value.type        = gmiffValueTypeNUM_REAL;
      _MiffGetPartRest(miff, header);
      _MiffGetNumReal( miff, miff->readCount, miff->readData);
      break;

   case 'Z': // constant
   case 'z': // constant
      _MiffGetPartRest(miff, header);
      _MiffGetConstant(miff, miff->readCount, miff->readData);
      break;

   case '*':
      miff->value.type        = gmiffValueTypeBIN;
      miff->value.bufferCount = _MiffGetValueBufferCount(miff);
      // Reading the buffer is done with miffValueBinGet();
      break;
   }

   return miff->value;
}

/**************************************************************************************************
func: gmiffGetValueStr

Convenience function for getting a whole string if memory requirements are
not that large.  Str should be large enough for the string being read in
including null terminator.
**************************************************************************************************/
Gb gmiffGetValueStr(Gmiff * const miff, Gcount const strCount, Gstr * const str)
{
   Gn8      index;
   Gn8      strIndex;
   Gstr    letter;
   GmiffData   dataResult;

   returnFalseIf(
      !_isStarted                         ||
      !miff                               ||
      strCount != miff->value.bufferCount ||
      !str)

   strIndex = 0;
   forCount(index, strCount)
   {
      dataResult = gmiffGetValueStrData(miff, &letter);

      // Ensure null termination.
      str[strIndex] = 0;

      returnFalseIf(
         dataResult == gmiffDataERROR        ||
         dataResult == gmiffDataIS_PART_DONE ||
         dataResult == gmiffDataIS_RECORD_DONE);

      str[strIndex++] = letter;
   }

   // Ensure null termination.
   str[strIndex]    = 0;
   miff->valueIndex = strCount;

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/**************************************************************************************************
func: gmiffGetValueStrData
**************************************************************************************************/
GmiffData gmiffGetValueStrData(Gmiff * const miff, Gstr * const strLetter)
{
   GmiffData miffData;

   returnIf(
         !_isStarted ||
         !miff       ||
         !strLetter  ||
         miff->valueIndex >= miff->value.bufferCount,
      gmiffDataERROR);

   miffData = _MiffGetStrLetter(miff, strLetter);
   miff->valueIndex++;

   return miffData;
}

/**************************************************************************************************
func: gmiffSetRecordStart
**************************************************************************************************/
Gb gmiffSetRecordStart(Gmiff * const miff, GmiffRecType const type, Gcount const count,
   Gstr const * const key)
{
   Gn8 index;

   returnFalseIf(
      !_isStarted ||
      !miff);

   miff->isRecordDone = gbFALSE;

   // Adjust the scope level before we indent.
   if (type == gmiffRecTypeBLOCK_STOP)
   {
      if (miff->currentScopeLevel != 0)
      {
         miff->currentScopeLevel--;
      }
   }

   // Add indents to the current scope level.
   forCount(index, miff->currentScopeLevel)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) " "));
   }

   // Write the type for the record.  Common for all cases.
   miff->currentArrayCount = count;
   miff->currentArrayIndex = 0;

   if      (type == gmiffRecTypeBLOCK_START)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "{"));
      miff->isRecordDone = gbTRUE;
   }
   else if (type == gmiffRecTypeBLOCK_STOP)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "}"));
      miff->isRecordDone = gbTRUE;
   }
   else if (count == miffArrayCountUNKNOWN)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "*"));
   }
   else
   {
      returnFalseIf(!_MiffSetNumInt(miff, count));
   }

   // We are ending a key value block.
   if (type == gmiffRecTypeBLOCK_STOP)
   {
      // TODO key value block stack pop.
      returnTrue;
   }

   returnFalseIf(!key);

   // Copy the key.
   miff->currentNameCount = min(256, _MiffStrGetCount(key));
   _MiffMemClearTypeArray(miff->currentName, Gstr, GkeySIZE);

   _MiffMemCopyTypeArray(miff->currentName, Gstr, miff->currentNameCount, key);

   returnFalseIf(!gmiffSetSeparator(miff));
   returnFalseIf(!_MiffSetBuffer(miff, miff->currentNameCount, (Gn1 *) miff->currentName));

   // We are starting a new key value block.
   if (type == gmiffRecTypeBLOCK_START)
   {
      miff->currentScopeLevel++;

      // TODO key value block stack push.
      returnTrue;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmiffSetRecordStop
**************************************************************************************************/
Gb gmiffSetRecordStop(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // The current type should end up being NONE.  It if isn't then the caller didn't
   // fullfill the record requirements.
   miff->isRecordDone = gbTRUE;

   return _MiffSetBuffer(miff, 1, (Gn1 *) "\n");
}

/**************************************************************************************************
func: gmiffSetSeparator
**************************************************************************************************/
Gb gmiffSetSeparator(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   return _MiffSetBuffer(miff, 1, (Gn1 *) "\t");
}

/**************************************************************************************************
func: gmiffSetValue

Only to be used if the value holds everything including the data.  String and
Binary data may only just have the header set.  In which case ou need to call
This sequence.

gmiffSetValueStart()
miffsetValueBinData() or gmiffSetValue_StrData() called multiple times.
gmiffSetValueStop()
**************************************************************************************************/
Gb gmiffSetValue(Gmiff * const miff, GmiffValue const value)
{
   // If the buffer data isn't provided with these types then we cannot used
   // this function to export them.
   returnFalseIf(
      (value.type == gmiffValueTypeBIN ||
       value.type == gmiffValueTypeSTR)   &&
      (value.bufferData.bin == NULL   ||
       value.bufferData.str == NULL));

   // Write out the header.
   returnFalseIf(!gmiffSetValueStart(miff, value));

   // Write out the value.
   returnFalseIf(!_MiffSetValueData(miff, value));

   // Write whatever is left to writ out and final check.
   return gmiffSetValueStop(miff);
}

/**************************************************************************************************
func: gmiffSetValue_BinData
**************************************************************************************************/
Gb gmiffSetValue_BinData(Gmiff * const miff, Gn1 const binByte)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnFalseIf(
      miff->value.type != gmiffValueTypeBIN ||
      miff->valueIndex >= miff->value.bufferCount);

   return _MiffSetBinByte(miff, binByte);
}

/**************************************************************************************************
func: gmiffSetValue_StrData
**************************************************************************************************/
Gb gmiffSetValue_StrData(Gmiff * const miff, Gstr const strLetter)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !strLetter);

   // Not a string
   // this character is beyond the length of the buffer we are expecting.
   returnFalseIf(
      miff->value.type != gmiffValueTypeSTR ||
      miff->valueIndex >= miff->value.bufferCount);

   return _MiffSetStr(miff, 1, &strLetter);
}

/**************************************************************************************************
func: gmiffSetValueStart
**************************************************************************************************/
Gb gmiffSetValueStart(Gmiff * const miff, GmiffValue const value)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   miff->value = value;

   gmiffSetSeparator(miff);

   return _MiffSetValueHeader(miff, value);
}

/**************************************************************************************************
func: gmiffSetValueStop
**************************************************************************************************/
Gb gmiffSetValueStop(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Binary or String buffer was not fully written out.
   returnFalseIf(
       (miff->value.type == gmiffValueTypeBIN ||
        miff->value.type == gmiffValueTypeSTR)            &&
       miff->value.bufferCount != miffBufferCountUNKNOWN &&
       miff->value.bufferCount != miff->valueIndex);

   miff->currentArrayIndex++;

   returnTrue;
}

/**************************************************************************************************
func: gmiffStart
**************************************************************************************************/
Gb gmiffStart(GmemCloc const memClocFunc, GmemDloc const memDlocFunc)
{
   Gmiff4 value;

   returnTrueIf(_isStarted);

   returnFalseIf(
      !memClocFunc ||
      !memDlocFunc)

   _MiffMemStart(memClocFunc, memDlocFunc);

   // Check if we need to byte swap.  Little Endian vs Big Endian.
   value.n = 1;
   if (value.byte[0] == 1)
   {
      _isByteSwapping = gbTRUE;
   }

#if 0
   _locale = _create_locale(LC_ALL, "C");
#endif

   _isStarted = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmiffStop
**************************************************************************************************/
void gmiffStop(void)
{
   returnVoidIf(!_isStarted);

   _MiffMemStop();

   _isStarted = gbFALSE;
}

/**************************************************************************************************
miffValue
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmiffValueGetB
**************************************************************************************************/
Gb gmiffValueGetB(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM_INT);

   returnTrueIf(value.inr.n > 0);

   returnFalse;
}

/**************************************************************************************************
func: gmiffValueGetBinCount
**************************************************************************************************/
Gcount gmiffValueGetBinCount(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeBIN);

   return value.bufferCount;
}

/**************************************************************************************************
func: gmiffValueGetI
**************************************************************************************************/
Gi8 gmiffValueGetI(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeNUM_INT);

   return value.inr.i;
}

/**************************************************************************************************
func: gmiffValueGetN
**************************************************************************************************/
Gn8 gmiffValueGetN(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeNUM_INT);

   return value.inr.n;
}

/**************************************************************************************************
func: gmiffValueGetR
**************************************************************************************************/
Gr8 gmiffValueGetR(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeNUM_REAL);

   if (value.isR8)
   {
      return value.inr.r;
   }

   return (Gr8) value.inr4.r;
}

/**************************************************************************************************
func: gmiffValueGetR4
**************************************************************************************************/
Gr4 gmiffValueGetR4(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeNUM_REAL);

   if (value.isR4)
   {
      return value.inr4.r;
   }

   return (Gr4) value.inr.r;
}

/**************************************************************************************************
func: gmiffValueGetStrLen
**************************************************************************************************/
Gcount gmiffValueGetStrCount(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeSTR);

   return value.bufferCount;
}

/**************************************************************************************************
func: gmiffValueGetType
**************************************************************************************************/
GmiffValueType gmiffValueGetType(GmiffValue const value)
{
   return value.type;
}

/**************************************************************************************************
func: gmiffValueIsR4

Return the Real value byte count.
True if Gr4
**************************************************************************************************/
Gb gmiffValueIsR4(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM_REAL);

   return value.isR4;
}

/**************************************************************************************************
func: gmiffValueIsR8

Return the Real value byte count.
True if Gr8
**************************************************************************************************/
Gb gmiffValueIsR8(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM_REAL);

   return value.isR8;
}

/**************************************************************************************************
func: gmiffValueSetB
**************************************************************************************************/
GmiffValue gmiffValueSetB(Gb const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM_INT;
   value.inr.n = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetBinBuffer
**************************************************************************************************/
GmiffValue gmiffValueSetBinBuffer(Gcount const binCount, Gn1 const * const binBuffer)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type           = gmiffValueTypeBIN;
   value.bufferCount    = binCount;
   value.bufferData.bin = (Gn1 *) binBuffer;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetBinCount
**************************************************************************************************/
GmiffValue gmiffValueSetBinCount(Gcount const binCount)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type        = gmiffValueTypeBIN;
   value.bufferCount = binCount;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetI
**************************************************************************************************/
GmiffValue gmiffValueSetI(Gi8 const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM_INT;
   value.isI   = gbTRUE;
   value.inr.i = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetN
**************************************************************************************************/
GmiffValue gmiffValueSetN(Gn8 const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM_INT;
   value.inr.n = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetNull
**************************************************************************************************/
GmiffValue gmiffValueSetNull(void)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type = gmiffValueTypeNULL;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetR
**************************************************************************************************/
GmiffValue gmiffValueSetR(Gr8 const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM_REAL;
   value.isR8  = gbTRUE;
   value.inr.r = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetR4
**************************************************************************************************/
GmiffValue gmiffValueSetR4(Gr4 const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type   = gmiffValueTypeNUM_REAL;
   value.isR4   = gbTRUE;
   value.inr4.r = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetStrBuffer
**************************************************************************************************/
GmiffValue gmiffValueSetStrBuffer(Gcount const strCount, Gstr const * const strBuffer)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type           = gmiffValueTypeSTR;
   value.bufferCount    = strCount;
   value.bufferData.str = (Gstr *) strBuffer;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetStrCount
**************************************************************************************************/
GmiffValue gmiffValueSetStrCount(Gcount const strCount)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type        = gmiffValueTypeSTR;
   value.bufferCount = strCount;

   return value;
}

/**************************************************************************************************
lib local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _MiffIsByteSwapping
**************************************************************************************************/
Gb _MiffIsByteSwapping(void)
{
   return _isByteSwapping;
}
