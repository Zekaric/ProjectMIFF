/**************************************************************************************************
file:       gmiff_get
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Get functions local to the library.
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
#include "gmiff_local.h"

/**************************************************************************************************
local:
prototype:
**************************************************************************************************/
static void  _GetNum(            Gmiff * const miff, GmiffValue * const value, Gcount const count, Gn1 const * const buffer);
static void  _GetNumInteger(     Gmiff * const miff, Gstr * const str);
static void  _GetNumReal(        Gmiff * const miff, Gstr * const str);

static Gn8   _NFromStr(          Gstr const * const str);

static Gn1   _ValueFromHexInt(   Gn1 const value);

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _MiffGetBin

Read in the binary to the internal buffer.
**************************************************************************************************/
Gb _MiffGetBin(Gmiff * const miff, Gcount const binCount, Gn1 * const binBuffer)
{
   Gindex index;
   Gb     result;

   // Reading a larger buffer directly from the repo.
   forCount(index, binount)
   {
      result = _MiffGetBinByte(miff, &binBuffer[index]);

      breakIf(!result);
   }

   miff->value.count = index;

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetBinByte
**************************************************************************************************/
Gb _MiffGetBinByte(Gmiff * const miff, Gn1 * const binByte)
{
   Gn1 byte[2];

   returnFalseIf(!miff->getBuffer(miff->dataRepo, 2, byte));

   *binByte = (_ValueFromHexInt(byte[0]) << 4) | _ValueFromHexInt(byte[1]);

   miff->bufferIndex++;

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetLineSkip
**************************************************************************************************/
Gb _MiffGetLineSkip(Gmiff * const miff)
{
   Gn1 byte;

   returnFalseIf(!miff);

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetNum
**************************************************************************************************/
void _MiffGetNum(Gmiff * const miff)
{
   _GetNum(miff, &miff->value, miff->readCount, miff->readData);
}

/**************************************************************************************************
func: _MiffGetPart
**************************************************************************************************/
Gb _MiffGetPart(Gmiff * const miff, Gb const trimLeadingTabs)
{
   Gn4   index;
   Gn1   byte;
   Gb    trimTabs;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->isRecordDone);

   trimTabs = trimLeadingTabs;
   index    = 0;
   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));

      // Eating tabs at the start of the line.
      if (trimTabs && byte == '\t')
      {
         continue;
      }
      else
      {
         trimTabs = gbFALSE;
      }

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      // Only read up to a key size.  Ignore everything else.
      if (index < gmiffCountDEFAULT)
      {
         miff->readData[index++] = byte;
      }
   }

   // Need NULL terminator.
   miff->readData[index] = 0;
   miff->readCount       = index;

   miff->isPartDone = gbTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = gbTRUE;
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetPartRest
**************************************************************************************************/
Gb _MiffGetPartRest(Gmiff * const miff, Gn1 const start)
{
   Gn4   index;
   Gn1   byte;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->isRecordDone);

   index = 0;

   // Start with the byte already read.
   miff->readData[index++] = start;

   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      // Only read up to a key size.  Ignore everything else.
      if (index < gmiffCountDEFAULT)
      {
         miff->readData[index++] = byte;
      }
   }

   // Need NULL terminator.
   miff->readData[index] = 0;
   miff->readCount       = index;

   miff->isPartDone = gbTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = gbTRUE;
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetPartEnd

Skip to the next tab or newline.
**************************************************************************************************/
Gb _MiffGetPartEnd(Gmiff * const miff)
{
   Gn4   index;
   Gn1   byte;

   // Nothing left to read for this record.
   returnFalseIf(!miff);

   returnTrueIf(
      miff->isPartDone ||
      miff->isRecordDone);

   index = 0;
   byte  = 0;
   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');
   }

   miff->isPartDone = gbTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = gbTRUE;
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetStrLetter
**************************************************************************************************/
GmiffData _MiffGetStrLetter(Gmiff * const miff, Gstr * const letter)
{
   returnIf(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) letter), gmiffDataERROR);

   // Escape single character slash, tab, and newline characters.
   switch (*letter)
   {
   case '\t':
      miff->isPartDone = gbTRUE;
      return gmiffDataIS_PART_DONE;

   case '\n':
      // An actual tab or cursor return character should never be inside a string.
      miff->isRecordDone = gbTRUE;
      return gmiffDataIS_RECORD_DONE;

   case '\\':
      returnIf(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) letter), gmiffDataERROR);
      switch (*letter)
      {
      case '\\':
         *letter = '\\';
         break;

      case 't':
         *letter = '\t';
         break;

      case 'n':
         *letter = '\n';
         break;
      }
      break;

   default:
      break;
   }

   return gmiffDataIS_DATA;
}

/**************************************************************************************************
func: _MiffGetValueCount
**************************************************************************************************/
Gcount _MiffGetValueCount(Gmiff * const miff)
{
   Gindex     index;
   Gstr       buffer[32];
   GmiffValue value;

   forCount(index, 32)
   {
      return0If(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &buffer[index]));

      breakIf(
         buffer[index] == ' '  ||
         buffer[index] == '\t' ||
         buffer[index] == '\n');
   }

   if (buffer[index] == '\t' ||
       buffer[index] == '\n')
   {
      miff->isPartDone   = gbTRUE;
      miff->isRecordDone = (buffer[index] == '\n');
   }

   buffer[index] = 0;

   if (buffer[0] == '*')
   {
      return gmiffCountUNKNOWN;
   }
   if (buffer[0] == 0)
   {
      return -gmiffCountDEFAULT;
   }

   _GetNum(miff, &value, (Gn4) index, (Gn1 *) buffer);

   return (Gcount) value.inr.n;
}

/**************************************************************************************************
func: _MiffGetValueHeader
**************************************************************************************************/
Gstr _MiffGetValueHeader(Gmiff * const miff)
{
   Gn1 byte;

   return0If(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));

   return (Gstr) byte;
}

/**************************************************************************************************
func: _MiffGetStr

Reading string into the internal buffer.
**************************************************************************************************/
Gb _MiffGetStr(Gmiff * const miff, Gcount const strCount, Gstr * const str)
{
   Gindex      index,
               strIndex;
   Gstr        letter;
   GmiffData   dataResult;

   strIndex = 0;
   forCount(index, strCount)
   {
      dataResult = gmiffGetValueStrData(miff, &letter);

      returnFalseIf(dataResult == gmiffDataERROR);

      if (dataResult == gmiffDataIS_PART_DONE ||
          dataResult == gmiffDataIS_RECORD_DONE)
      {
         miff->value.count  = strIndex;
         miff->isPartDone   = gbTRUE;
         miff->isRecordDone = (dataResult == gmiffDataIS_RECORD_DONE);
         break;
      }

      str[strIndex++] = letter;
   }

   // Ensure null termination.
   str[strIndex] = 0;

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _GetNum
**************************************************************************************************/
static void _GetNum(Gmiff * const miff, GmiffValue * const value, Gcount const count, Gn1 const * const buffer)
{
   Gb   isIntegerNumberFound,
        isFractionalNumberFound,
        isExponentNumberFound;
   // Current doulbe maxes out a 10^+/-308, 400 characters is large enough.
   Gstr numStr[400];
   int  numIndex;
   int  index;

   value->type  = gmiffValueTypeNONE;
   value->inr.n = 0;

   // Reset the number string.
   forCount(index, 400)
   {
      numStr[index] = 0;
   }

   isIntegerNumberFound = gbFALSE;

   // Read in the integer part.
   numIndex = 0;
   for (index = 0; index < count; index++)
   {
      breakIf(!buffer[index]);

      // Number is a Real
      if (buffer[index] == '.')
      {
         gotoIf(isIntegerNumberFound, GET_FRACTION);
         return;
      }
      // Number is Real with an exponent
      if (buffer[index] == 'e' ||
          buffer[index] == 'E')
      {
         gotoIf(isIntegerNumberFound, GET_EXPONENT);
         return;
      }

      // Add to the integer portion.
      if ('0' <= buffer[index] && buffer[index] <= '9')
      {
         numStr[numIndex++]     = (Gstr) buffer[index];
         isIntegerNumberFound = gbTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         break;
      }
   }

   if (isIntegerNumberFound)
   {
      value->type = gmiffValueTypeNUM;
      _GetNumInteger(miff, numStr);
   }
   return;

GET_FRACTION:
   // Decimal was found.
   isFractionalNumberFound = gbFALSE;
   numStr[numIndex++]      = '.';
   for (; index < count; index++)
   {
      breakIf(!buffer[index]);

      // Exponent defined
      if (buffer[index] == 'e' ||
          buffer[index] == 'E')
      {
         if (!isFractionalNumberFound)
         {
            numIndex--;
         }
         goto GET_EXPONENT;
      }

      // Add to the fractional portion.
      if ('0' <= buffer[index] && buffer[index] <= '9')
      {
         numStr[numIndex]        = (Gstr) buffer[index];
         isFractionalNumberFound = gbTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         break;
      }
   }

   if (isFractionalNumberFound)
   {
      value->type = gmiffValueTypeNUM;
      _GetNumReal(miff, numStr);
   }
   return;

GET_EXPONENT:
   // Exponent was found
   isExponentNumberFound = gbFALSE;
   numStr[numIndex++]    = 'e';

   // End of file in the middle of a number!
   returnVoidIf(
      index >= count ||
      !buffer[index]);

   // Get the sign.
   if      ('0' <= buffer[index] && buffer[index] <= '9')
   {
      numStr[numIndex++]    = (Gstr) buffer[index];
      isExponentNumberFound = gbTRUE;
   }
   // Negative exponent.
   else if (buffer[index] == '-')
   {
      numStr[numIndex++] = (Gstr) buffer[index];
   }
   // Wasn't a sign or a digit after the 'e'.
   else if (buffer[index] != '+')
   {
      return;
   }
   // If it was a "+" sign we just eat it.  Unnecessary.
   index++;

   // Get the exponent
   for (; index < count; index++)
   {
      breakIf(!buffer[index]);

      if ('0' <= buffer[index] && buffer[index] <= '9')
      {
         numStr[numIndex++]    = buffer[index];
         isExponentNumberFound = gbTRUE;
      }
      else
      {
         break;
      }
   }

   if (isExponentNumberFound)
   {
      _GetNumReal(miff, numStr);
   }
}

/**************************************************************************************************
func: _GetNumInteger
**************************************************************************************************/
static void _GetNumInteger(Gmiff * const miff, Gstr * const str)
{
   // Default number type.
   miff->value.type    = gmiffValueTypeNUM;
   miff->value.isI     =
      miff->value.isN  =
      miff->value.isR8 =
      miff->value.isR4 = gbFALSE;

   // This is a negative number.  Definitely not a natural number.
   if (str[0] == '-')
   {
      miff->value.inr.n = _NFromStr(&str[1]);
      if (miff->value.inr.n == ((Gn8) Gi8MAX) + 1)
      {
         miff->value.isI   = gbTRUE;
         miff->value.inr.i = Gi8MIN;

         return;
      }
      if (miff->value.inr.n <= Gi8MAX)
      {
         miff->value.isI   = gbTRUE;
         miff->value.inr.i = - ((Gi8) miff->value.inr.n);

         return;
      }

      // Not all values are representable if they are too large.
      miff->value.inr.n = 0;

      return;
   }

   miff->value.isN   = gbTRUE;
   miff->value.inr.n = _NFromStr(str);

   // This unsigned integer is small enough to be a signed integer.
   if (miff->value.inr.n <= Gi8MAX)
   {
      miff->value.isI   = gbTRUE;
      miff->value.inr.i = miff->value.inr.n;

      return;
   }
}

/**************************************************************************************************
func: _GetNumReal
**************************************************************************************************/
static void _GetNumReal(Gmiff * const miff, Gstr * const str)
{
   miff->value.inr.i  = 0;
   miff->value.inr.n  = 0;
   miff->value.inr.r  = _atof_l(str, _MiffLocaleGet());
   miff->value.inr4.r = (float) miff->value.inr.r;
   miff->value.type   = gmiffValueTypeNUM;
}

/**************************************************************************************************
func: _NFromStr
**************************************************************************************************/
static Gn8 _NFromStr(Gstr const * const str)
{
   Gi4 index;
   Gn8 value;

   value = 0;
   loopCount(index)
   {
      breakIf(str[index] == 0);

      value = value * 10 + str[index] - '0';
   }

   return value;
}

/**************************************************************************************************
func: _ValueFromHexInt
**************************************************************************************************/
static Gn1 _ValueFromHexInt(Gn1 const value)
{
   switch (value)
   {
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
      return value - '0';

   case 'A':
   case 'B':
   case 'C':
   case 'D':
   case 'E':
   case 'F':
      return value - 'A' + 0xA;

   case 'a':
   case 'b':
   case 'c':
   case 'd':
   case 'e':
   case 'f':
      return value - 'a' + 0xA;
   }

   return 0;
}
