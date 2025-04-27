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
static _locale_t   _locale;

/**************************************************************************************************
prototype:
**************************************************************************************************/
static Gb _RecordSetSeparator(Gmiff * const miff);
static Gb _ValuePrep(         Gmiff * const miff);

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmiffClocReader
**************************************************************************************************/
Gmiff *gmiffClocReader(GgetBuffer getBufferFunc, Gstr * const subFormatName,
   Gn8 * const subFormatVersion, void * const dataRepo)
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
   Gn8 * const subFormatVersion, void * const dataRepo)
{
   Gstr         key[GkeySIZE];
   GmiffValue   value;

   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !getBufferFunc);

   _MiffMemClearType(miff, Gmiff);
   miff->method    = gmethodREAD;
   miff->dataRepo  = dataRepo;
   miff->getBuffer = getBufferFunc;

   // Read the header.
   returnFalseIf(!gmiffGetRecordStart(miff, key));
   returnFalseIf(!strIsEqual(GkeyBYTE_COUNT, key, MIFF_HEADER_FILETYPE_STR));

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeNUM &&
      gmiffValueGetN(   value) != MIFF_HEADER_VERSION)
   returnFalseIf(!gmiffGetRecordStop(miff));

   returnFalseIf(!gmiffGetRecordStart(miff, key));
   _MiffMemCopyTypeArray(subFormatName,       Gstr, GkeyBYTE_COUNT, key);
   _MiffMemCopyTypeArray(miff->subFormatName, Gstr, GkeyBYTE_COUNT, key);

   value = gmiffGetValue(miff);
   returnFalseIf(gmiffValueGetType(value) != gmiffValueTypeNUM)
   *subFormatVersion          =
      miff->subFormatVersion  = gmiffValueGetN(value);
   returnFalseIf(!gmiffGetRecordStop(miff));

   returnTrue;
}

/**************************************************************************************************
func: gmiffClocWriter
**************************************************************************************************/
Gmiff *gmiffClocWriter(GsetBuffer setBufferFunc, Gstr const * const subFormatName,
   Gn8 const subFormatVersion, void * const dataRepo)
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
   Gstr const * const subFormatName, Gn8 const subFormatVersion, void * const dataRepo)
{
   returnFalseIf(
      !_isStarted     ||
      !miff           ||
      !setBufferFunc  ||
      !subFormatName);

   _MiffMemClearType(miff, Gmiff);
   miff->version              = MIFF_HEADER_VERSION;
   miff->method               = gmethodWRITE;
   miff->dataRepo             = dataRepo;
   miff->setBuffer            = setBufferFunc;

   // Write the miff header.
   gmiffRecordSetN(miff, MIFF_HEADER_FILETYPE_STR, MIFF_HEADER_VERSION);
   gmiffRecordSetN(miff, subFormatName,            subFormatVersion);

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
Gb gmiffGetRecordStart(Gmiff * const miff, Gstr * const key)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Reset the record reading.
   miff->isRecordDone = gbFALSE;

   // Clear the key.
   _MiffMemClearTypeArray(key, Gstr, GkeySIZE);

   // Read in the name of the record
   returnFalseIf(!_MiffGetPart(miff, gbFALSE));
   returnFalseIf(!_MiffGetKey( miff, key));

   returnTrue;
}

/**************************************************************************************************
func: gmiffGetRecordStop

Skip to the end of the record if not at the end already.
**************************************************************************************************/
Gb gmiffGetRecordStop(Gmiff * const miff)
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
gmiffGetValue() value.count
**************************************************************************************************/
Gb gmiffGetValueBin(Gmiff * const miff, Gcount const binCount, Gn1 * const binBuffer)
{
   Gn8 index;

   returnFalseIf(
      !_isStarted                           ||
      !miff                                 ||
      miff->value.type != gmiffValueTypeBIN ||
      binCount         != miff->value.count ||
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
      !_isStarted                            ||
      !miff                                  ||
      miff->value.type  != gmiffValueTypeBIN ||
      !binByte                               ||
      miff->bufferIndex >= miff->value.count);

   return _MiffGetBinByte(miff, binByte);
}

/**************************************************************************************************
func: gmiffGetValue
**************************************************************************************************/
GmiffValue gmiffGetValue(Gmiff * const miff)
{
   Gstr        header;
   GmiffValue  vtemp;

   _MiffMemClearType(&vtemp, GmiffValue);

   returnIf(
         !_isStarted ||
         !miff,
      vtemp);

   miff->isPartDone = gbFALSE;
   _MiffMemClearType(&miff->value, GmiffValue);

   miff->value.type  = gmiffValueTypeNONE;
   miff->valueCount  = 0;
   miff->bufferIndex = 0;

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

   case '[':
      miff->value.type  = gmiffValueTypeARRAY_COUNT;
      miff->value.inr.n = _MiffGetValueCount(miff);
      break;

   case '{':
      miff->value.type = gmiffValueTypeBLOCK_START;
      miff->scopeLevel++;
      break;

   case '}':
      miff->value.type = gmiffValueTypeBLOCK_STOP;
      miff->scopeLevel--;
      break;

   case '(':
      miff->value.type  = gmiffValueTypeGROUP_COUNT;
      miff->value.inr.n = _MiffGetValueCount(miff);
      break;


   case '~':
      miff->value.type  = gmiffValueTypeNULL;
      _MiffGetPartEnd(miff);
      break;

   case '"':
      miff->value.type  = gmiffValueTypeSTR;
      miff->value.count = _MiffGetValueCount(miff);
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
   case 'I':
   case 'i':
   case 'N':
   case 'R':
   case 'r':
   case 'C':
   case 'c':
   case '?':
      miff->value.type  = gmiffValueTypeNUM;
      _MiffGetPartRest(miff, header);
      _MiffGetNum(     miff);
      break;

   case '.':
      miff->value.type  = gmiffValueTypeBIN;
      miff->value.count = _MiffGetValueCount(miff);
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
   Gn8         index;
   Gn8         strIndex;
   Gstr        letter;
   GmiffData   dataResult;

   returnFalseIf(
      !_isStarted                   ||
      !miff                         ||
      strCount != miff->value.count ||
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
   str[strIndex]     = 0;
   miff->bufferIndex = strCount;

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
         miff->bufferIndex >= miff->value.count,
      gmiffDataERROR);

   miffData = _MiffGetStrLetter(miff, strLetter);
   miff->bufferIndex++;

   return miffData;
}

/**************************************************************************************************
func: gmiffRecordSetB
**************************************************************************************************/
Gb gmiffRecordSetB(Gmiff * const miff, Gstr const * const name, Gb const value)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetB(value)));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetBArray
**************************************************************************************************/
Gb gmiffRecordSetBArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gb const * const array)
{
   Gindex index;

   returnFalseIf(
      count != 0 &&
      !array);

   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetArrayCount(count)));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetValue(miff, gmiffValueSetB(array[index])));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetBin
**************************************************************************************************/
Gb gmiffRecordSetBin(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gn1 * const value)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetBin(count, value)));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetBlockStart
**************************************************************************************************/
Gb gmiffRecordSetBlockStart(Gmiff * const miff, Gstr const * const name)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetBlockStart()));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetBlockStop
**************************************************************************************************/
Gb gmiffRecordSetBlockStop(Gmiff * const miff)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetBlockStop()));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetI
**************************************************************************************************/
Gb gmiffRecordSetI(Gmiff * const miff, Gstr const * const name, Gi8 const value)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetI(value)));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetIArray
**************************************************************************************************/
Gb gmiffRecordSetIArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gi8 const * const array)
{
   Gindex index;

   returnFalseIf(
      count != 0 &&
      !array);

   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetArrayCount(count)));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetValue(miff, gmiffValueSetI(array[index])));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetN
**************************************************************************************************/
Gb gmiffRecordSetN(Gmiff * const miff, Gstr const * const name, Gn8 const value)
{
   returnFalseIf(!gmiffSetValue(      miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(      miff, gmiffValueSetN(value)));
   return         gmiffSetRecordStop( miff);
}

/**************************************************************************************************
func: gmiffRecordSetNArray
**************************************************************************************************/
Gb gmiffRecordSetNArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gn8 const * const array)
{
   Gindex index;

   returnFalseIf(
      count != 0 &&
      !array);

   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetArrayCount(count)));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetValue(miff, gmiffValueSetN(array[index])));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetNull
**************************************************************************************************/
Gb gmiffRecordSetNull(Gmiff * const miff, Gstr const * const name)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetNull()));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetR
**************************************************************************************************/
Gb gmiffRecordSetR(Gmiff * const miff, Gstr const * const name, Gr8 const value)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetR(value)));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetRArray
**************************************************************************************************/
Gb gmiffRecordSetRArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gr8 const * const array)
{
   Gindex index;

   returnFalseIf(
      count != 0 &&
      !array);

   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetArrayCount(count)));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetValue(miff, gmiffValueSetR(array[index])));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetR4
**************************************************************************************************/
Gb gmiffRecordSetR4(Gmiff * const miff, Gstr const * const name, Gr4 const value)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetR4(value)));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetR4Array
**************************************************************************************************/
Gb gmiffRecordSetR4Array(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gr4 const * const array)
{
   Gindex index;

   returnFalseIf(
      count != 0 &&
      !array);

   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetArrayCount(count)));

   forCount(index, count)
   {
      returnFalseIf(!gmiffSetValue(miff, gmiffValueSetR4(array[index])));
   }

   return gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffRecordSetStr
**************************************************************************************************/
Gb gmiffRecordSetStr(Gmiff * const miff, Gstr const * const name, Gstr const * const value)
{
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(name),  name)));
   returnFalseIf(!gmiffSetValue(     miff, gmiffValueSetStr(gstrGetCount(value), value)));
   return         gmiffSetRecordStop(miff);
}

/**************************************************************************************************
func: gmiffSetStrArray
**************************************************************************************************/
Gb gmiffSetStrArray(Gmiff * const miff, Gstr const * const name, Gcount const count,
   Gstr ** const array)
{
   Gindex index;

   returnFalseIf(
      count != 0 &&
      !array);

   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(name), name)));
   returnFalseIf(!gmiffSetValue(miff, gmiffValueSetArrayCount(count)));

   forCount(index, count)
   {
      returnFalseIf(
         !gmiffSetValue(miff, gmiffValueSetStr(gstrGetCount(array[index]), array[index])));
   }

   return gmiffSetRecordStop(miff);
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

   _locale = _create_locale(LC_ALL, "C");

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
miffRecord
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmiffSetRecordStop
**************************************************************************************************/
Gb gmiffSetRecordStop(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !miff->valueCount);

   // Reset the value index.
   miff->valueCount = 0;

   return _MiffSetBuffer(miff, 1, (Gn1 *) "\n");
}

/**************************************************************************************************
func: gmiffSetValue
**************************************************************************************************/
Gb gmiffSetValue(Gmiff * const miff, GmiffValue const value)
{
   // If the buffer data isn't provided with these types then we cannot used
   // this function to export them.
   returnFalseIf(
      !_isStarted                            ||
      !miff                                  ||
      ((value.type == gmiffValueTypeBIN ||
        value.type == gmiffValueTypeSTR)   &&
       (value.data.bin == NULL          ||
        value.data.str == NULL)));

   returnFalseIf(!_ValuePrep(miff));

   // Write out the value header.
   returnFalseIf(!_MiffSetValueHeader(miff, value));

   // Write out the value data.
   returnFalseIf(!_MiffSetValueData(  miff, value));

   miff->valueCount++;
   returnTrue;
}

/**************************************************************************************************
func: gmiffSetValueArrayCount
**************************************************************************************************/
Gb gmiffSetValueArrayCount(Gmiff * const miff, Gcount const count)
{
   return gmiffSetValue(miff, gmiffValueSetArrayCount(count));
}

/**************************************************************************************************
func: gmiffSetValueB
**************************************************************************************************/
Gb gmiffSetValueB(Gmiff * const miff, Gb const value)
{
   return gmiffSetValue(miff, gmiffValueSetB(value));
}

/**************************************************************************************************
func: gmiffSetValueBin
**************************************************************************************************/
Gb gmiffSetValueBin(Gmiff * const miff, Gcount const binCount, Gn1 const * const binBuffer)
{
   return gmiffSetValue(miff, gmiffValueSetBin(binCount, binBuffer));
}

/**************************************************************************************************
func: gmiffSetValueBinData
**************************************************************************************************/
Gb gmiffSetValueBinData(Gmiff * const miff, Gn1 const binByte)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnFalseIf(
      miff->value.type  != gmiffValueTypeBIN ||
      miff->bufferIndex >= miff->value.count);

   return _MiffSetBinByte(miff, binByte);
}

/**************************************************************************************************
func: gmiffSetValueBinStart
**************************************************************************************************/
Gb gmiffSetValueBinStart(Gmiff * const miff, Gcount const binCount)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnFalseIf(!_ValuePrep(miff));

   miff->value = gmiffValueSetBin(binCount, NULL);

   return _MiffSetValueHeader(miff, gmiffValueSetBin(binCount, NULL));
}

/**************************************************************************************************
func: gmiffSetValueBinStop
**************************************************************************************************/
Gb gmiffSetValueBinStop(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Binary or String buffer was not fully written out.
   returnFalseIf(
       miff->value.type != gmiffValueTypeBIN          ||
       (miff->value.count != miffCountUNKNOWN &&
        miff->value.count != miff->bufferIndex));

   returnTrue;
}

/**************************************************************************************************
func: gmiffSetValueBlockStart
**************************************************************************************************/
Gb gmiffSetValueBlockStart(Gmiff * const miff)
{
   return gmiffSetValue(miff, gmiffValueSetBlockStart());
}

/**************************************************************************************************
func: gmiffSetValueBlockStop
**************************************************************************************************/
Gb gmiffSetValueBlockStop(Gmiff * const miff)
{
   return gmiffSetValue(miff, gmiffValueSetBlockStop());
}

/**************************************************************************************************
func: gmiffSetValueGroupCount
**************************************************************************************************/
Gb gmiffSetValueGroupCount(Gmiff * const miff, Gcount const count)
{
   return gmiffSetValue(miff, gmiffValueSetGroupCount(count));
}

/**************************************************************************************************
func: gmiffSetValueI
**************************************************************************************************/
Gb gmiffSetValueI(Gmiff * const miff, Gi8 const value)
{
   return gmiffSetValue(miff, gmiffValueSetI(value));
}

/**************************************************************************************************
func: gmiffSetValueN
**************************************************************************************************/
Gb gmiffSetValueN(Gmiff * const miff, Gn8 const value)
{
   return gmiffSetValue(miff, gmiffValueSetN(value));
}

/**************************************************************************************************
func: gmiffSetValueNull
**************************************************************************************************/
Gb gmiffSetValueNull(Gmiff * const miff)
{
   return gmiffSetValue(miff, gmiffValueSetNull());
}

/**************************************************************************************************
func: gmiffSetValueR
**************************************************************************************************/
Gb gmiffSetValueR(Gmiff * const miff, Gr8 const value)
{
   return gmiffSetValue(miff, gmiffValueSetR(value));
}

/**************************************************************************************************
func: gmiffSetValueR4
**************************************************************************************************/
Gb gmiffSetValueR4(Gmiff * const miff, Gr4 const value)
{
   return gmiffSetValue(miff, gmiffValueSetR4(value));
}

/**************************************************************************************************
func: gmiffSetValueStr
**************************************************************************************************/
Gb gmiffSetValueStr(Gmiff * const miff, Gcount const strCount, Gstr const * const strBuffer)
{
   return gmiffSetValue(miff, gmiffValueSetStr(strCount, strBuffer));
}

/**************************************************************************************************
func: gmiffSetValueStrData
**************************************************************************************************/
Gb gmiffSetValueStrData(Gmiff * const miff, Gstr const strLetter)
{
   returnFalseIf(
      !_isStarted ||
      !miff       ||
      !strLetter);

   // Not a string
   // this character is beyond the length of the buffer we are expecting.
   returnFalseIf(
      miff->value.type  != gmiffValueTypeSTR ||
      miff->bufferIndex >= miff->value.count);

   return _MiffSetStr(miff, 1, &strLetter);
}

/**************************************************************************************************
func: gmiffSetValueStrStart
**************************************************************************************************/
Gb gmiffSetValueStrStart(Gmiff * const miff, Gcount const strCount)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   returnFalseIf(!_ValuePrep(miff));

   miff->value = gmiffValueSetStr(strCount, NULL);

   return _MiffSetValueHeader(miff, miff->value);
}

/**************************************************************************************************
func: gmiffSetValueStrStop
**************************************************************************************************/
Gb gmiffSetValueStrStop(Gmiff * const miff)
{
   returnFalseIf(
      !_isStarted ||
      !miff);

   // Binary or String buffer was not fully written out.
   returnFalseIf(
       miff->value.type != gmiffValueTypeSTR          ||
       (miff->value.count != miffCountUNKNOWN &&
        miff->value.count != miff->bufferIndex));

   returnTrue;
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
   returnFalseIf(value.type != gmiffValueTypeNUM);

   returnTrueIf(value.inr.n > 0);

   returnFalse;
}

/**************************************************************************************************
func: gmiffValueGetBinCount
**************************************************************************************************/
Gcount gmiffValueGetBinCount(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeBIN);

   return value.count;
}

/**************************************************************************************************
func: gmiffValueGetI
**************************************************************************************************/
Gi8 gmiffValueGetI(GmiffValue const value)
{
   return0If(
      value.type != gmiffValueTypeNUM ||
      !value.isI);

   return value.inr.i;
}

/**************************************************************************************************
func: gmiffValueGetN
**************************************************************************************************/
Gn8 gmiffValueGetN(GmiffValue const value)
{
   return0If(
      value.type != gmiffValueTypeNUM||
      !value.isN);

   return value.inr.n;
}

/**************************************************************************************************
func: gmiffValueGetR
**************************************************************************************************/
Gr8 gmiffValueGetR(GmiffValue const value)
{
   return0If(
      value.type != gmiffValueTypeNUM ||
      !(value.isR8 ||
        value.isR4 ||
        value.isI  ||
        value.isN));

   if (value.isR8)
   {
      return value.inr.r;
   }
   if (value.isR4)
   {
      return (Gr8) value.inr4.r;
   }
   if (value.isI)
   {
      return (Gr8) value.inr.i;
   }
   if (value.isN)
   {
      return (Gr8) value.inr.n;
   }

   return 0;
}

/**************************************************************************************************
func: gmiffValueGetR4
**************************************************************************************************/
Gr4 gmiffValueGetR4(GmiffValue const value)
{
   return0If(
      value.type != gmiffValueTypeNUM ||
      !(value.isR8 ||
        value.isR4 ||
        value.isI  ||
        value.isN));

   if (value.isR4)
   {
      return value.inr4.r;
   }
   if (value.isR8)
   {
      return (Gr4) value.inr.r;
   }
   if (value.isI)
   {
      return (Gr4) value.inr.i;
   }
   if (value.isN)
   {
      return (Gr4) value.inr.n;
   }

   return 0;
}

/**************************************************************************************************
func: gmiffValueGetStrLen
**************************************************************************************************/
Gcount gmiffValueGetStrCount(GmiffValue const value)
{
   return0If(value.type != gmiffValueTypeSTR);

   return value.count;
}

/**************************************************************************************************
func: gmiffValueGetType
**************************************************************************************************/
GmiffValueType gmiffValueGetType(GmiffValue const value)
{
   return value.type;
}

/**************************************************************************************************
func: gmiffValueIsI

Return the Real value byte count.
True if Gr4
**************************************************************************************************/
Gb gmiffValueIsI(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM);

   return value.isI;
}

/**************************************************************************************************
func: gmiffValueIsN

Return the Real value byte count.
True if Gr4
**************************************************************************************************/
Gb gmiffValueIsN(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM);

   return value.isN;
}

/**************************************************************************************************
func: gmiffValueIsR4

Return the Real value byte count.
True if Gr4
**************************************************************************************************/
Gb gmiffValueIsR4(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM);

   return value.isR4;
}

/**************************************************************************************************
func: gmiffValueIsR8

Return the Real value byte count.
True if Gr8
**************************************************************************************************/
Gb gmiffValueIsR8(GmiffValue const value)
{
   returnFalseIf(value.type != gmiffValueTypeNUM);

   return value.isR8;
}

/**************************************************************************************************
func: gmiffValueSetArrayCount
**************************************************************************************************/
GmiffValue gmiffValueSetArrayCount(Gcount const count)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeARRAY_COUNT;
   value.count = count;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetB
**************************************************************************************************/
GmiffValue gmiffValueSetB(Gb const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM;
   value.isI   = gbTRUE;
   value.inr.i = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetBin
**************************************************************************************************/
GmiffValue gmiffValueSetBin(Gcount const binCount, Gn1 const * const binBuffer)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type     = gmiffValueTypeBIN;
   value.count    = binCount;
   value.data.bin = (Gn1 *) binBuffer;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetBinCount
**************************************************************************************************/
GmiffValue gmiffValueSetBinCount(Gcount const binCount)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeBIN;
   value.count = binCount;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetBlockStart
**************************************************************************************************/
GmiffValue gmiffValueSetBlockStart(void)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type = gmiffValueTypeBLOCK_START;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetBlockStop
**************************************************************************************************/
GmiffValue gmiffValueSetBlockStop(void)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type = gmiffValueTypeBLOCK_STOP;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetGroupCount
**************************************************************************************************/
GmiffValue gmiffValueSetGroupCount(Gcount const count)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeGROUP_COUNT;
   value.count = count;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetI
**************************************************************************************************/
GmiffValue gmiffValueSetI(Gi8 const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM;
   value.isI   = gbTRUE;
   value.inr.i = ivalue;
   if (value.inr.i >= 0)
   {
      value.isN = gbTRUE;
   }

   return value;
}

/**************************************************************************************************
func: gmiffValueSetN
**************************************************************************************************/
GmiffValue gmiffValueSetN(Gn8 const ivalue)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeNUM;
   value.isN   = gbTRUE;
   value.inr.n = ivalue;
   if (value.inr.n < Gi8MAX)
   {
      value.isI = gbTRUE;
   }

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

   value.type  = gmiffValueTypeNUM;
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

   value.type   = gmiffValueTypeNUM;
   value.isR4   = gbTRUE;
   value.inr4.r = ivalue;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetStr
**************************************************************************************************/
GmiffValue gmiffValueSetStr(Gcount const strCount, Gstr const * const strBuffer)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type     = gmiffValueTypeSTR;
   value.count    = strCount;
   value.data.str = (Gstr *) strBuffer;

   return value;
}

/**************************************************************************************************
func: gmiffValueSetStrCount
**************************************************************************************************/
GmiffValue gmiffValueSetStrCount(Gcount const strCount)
{
   GmiffValue value;

   _MiffMemClearType(&value, GmiffValue);

   value.type  = gmiffValueTypeSTR;
   value.count = strCount;

   return value;
}

/**************************************************************************************************
lib local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _MiffLocaleGet
**************************************************************************************************/
_locale_t _MiffLocaleGet(void)
{
   return _locale;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _RecordSetSeparator
**************************************************************************************************/
static Gb _RecordSetSeparator(Gmiff * const miff)
{
   return _MiffSetBuffer(miff, 1, (Gn1 *) "\t");
}

/**************************************************************************************************
func: _ValuePrep
**************************************************************************************************/
static Gb _ValuePrep(Gmiff * const miff)
{
   Gindex index;

   // Only add indentation for the first value.
   if (miff->valueCount == 0)
   {
      forCount(index, miff->scopeLevel)
      {
         returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) " "));
      }
      returnTrue;
   }

   // Only add separators after the first value.
   return _RecordSetSeparator(miff);
}
