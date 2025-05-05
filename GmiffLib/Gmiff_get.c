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
static Gb    _GetNum(            Gmiff * const miff, Gcount const count, Gn1 const * const buffer);
static Gcount _GetNumCount(      Gstr * const str);
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
   Gindex    index;
   GmiffData result;

   // Reading a larger buffer directly from the repo.
   forCount(index, binCount)
   {
      result = _MiffGetBinByte(miff, &binBuffer[index]);

      if (result == gmiffDataIS_PART_DONE ||
          result == gmiffDataIS_RECORD_DONE)
      {
         miff->value.count  = index;
         miff->isPartDone   = gbTRUE;
         miff->isRecordDone = (result == gmiffDataIS_RECORD_DONE);
         break;
      }
   }

   miff->value.count = index;

   returnFalseIf(!_MiffGetPartEnd(miff));

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetBinByte
**************************************************************************************************/
GmiffData _MiffGetBinByte(Gmiff * const miff, Gn1 * const binByte)
{
   Gn1 byte[2];

   returnIf(!miff->getBuffer(miff->dataRepo, 1, &byte[0]), gmiffDataERROR);

   if (byte[0] == '\t')
   {
      miff->isPartDone = gbTRUE;
      return gmiffDataIS_PART_DONE;
   }

   if (byte[0] == '\n')
   {
      miff->isPartDone   = gbTRUE;
      miff->isRecordDone = gbTRUE;
      return gmiffDataIS_RECORD_DONE;
   }

   returnIf(!miff->getBuffer(miff->dataRepo, 1, &byte[1]), gmiffDataERROR);

   returnIf(
         byte[1] == '\t' ||
         byte[0] == '\n',
      gmiffDataERROR);

   *binByte = (_ValueFromHexInt(byte[0]) << 4) | _ValueFromHexInt(byte[1]);

   miff->bufferIndex++;

   return gmiffDataIS_DATA;
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
Gb _MiffGetNum(Gmiff * const miff)
{
   return _GetNum(miff, miff->readCount, miff->readData);
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
      dataResult = _MiffGetStrLetter(miff, &letter);

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

   return _GetNumCount(buffer);
}

/**************************************************************************************************
func: _MiffGetValueHeader
**************************************************************************************************/
Gstr _MiffGetValueHeader(Gmiff * const miff)
{
   Gn1 byte;

   // Eat leading spaces.
   loop
   {
      return0If(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));
      breakIf(byte != ' ');
   }

   return (Gstr) byte;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _GetNum
**************************************************************************************************/
static Gb _GetNum(Gmiff * const miff, Gcount const count, Gn1 const * const buffer)
{
   Gb   isIntegerSignFound,
        isIntegerNumberFound,
        isFractionalNumberFound,
        isExponentSignFound,
        isExponentNumberFound;
   // Current doulbe maxes out a 10^+/-308, 400 characters is large enough.
   Gstr numStr[400];
   int  numIndex;
   int  index;

   isIntegerSignFound         =
      isIntegerNumberFound    =
      isFractionalNumberFound =
      isExponentSignFound     =
      isExponentNumberFound   = gbFALSE;

   miff->value.type  = gmiffValueTypeNONE;
   miff->value.inr.n = 0;

   // Reset the number string.
   _MiffMemClearTypeArray(numStr, Gstr, 400);

   // Is the number a constant?
   if (buffer[0] == '?')
   {
      miff->value.type   = gmiffValueTypeNUM;
      miff->value.isR8   = gbTRUE;
      miff->value.isR4   = gbTRUE;
      miff->value.inr.r  = NAN;
      miff->value.inr4.r = NAN;
      returnTrue;
   }

   if      (buffer[0] == '+')
   {
      if      (buffer[1] == 'I')
      {
         miff->value.type  = gmiffValueTypeNUM;
         miff->value.isI   = gbTRUE;
         miff->value.isN   = gbTRUE;
         miff->value.inr.i = Gi8MAX;
         returnTrue;
      }
      else if (buffer[1] == 'N')
      {
         miff->value.type  = gmiffValueTypeNUM;
         miff->value.isN   = gbTRUE;
         miff->value.inr.i = Gn8MAX;
         returnTrue;
      }
      else if (buffer[1] == 'R')
      {
         miff->value.type   = gmiffValueTypeNUM;
         miff->value.isR8   = gbTRUE;
         miff->value.isR4   = gbTRUE;
         miff->value.inr.r  = Gr8MAX;
         miff->value.inr4.r = Gr4MAX;
         returnTrue;
      }
      else if (buffer[1] == 'C')
      {
         miff->value.type   = gmiffValueTypeNUM;
         miff->value.isR8   = gbTRUE;
         miff->value.isR4   = gbTRUE;
         miff->value.inr.r  = HUGE_VAL;
         miff->value.inr4.r = HUGE_VALF;
         returnTrue;
      }
   }
   else if (buffer[0] == '-')
   {
      if      (buffer[1] == 'I')
      {
         miff->value.type  = gmiffValueTypeNUM;
         miff->value.isI   = gbTRUE;
         miff->value.isN   = gbTRUE;
         miff->value.inr.i = Gi8MIN;
         returnTrue;
      }
      else if (buffer[1] == 'R')
      {
         miff->value.type   = gmiffValueTypeNUM;
         miff->value.isR8   = gbTRUE;
         miff->value.isR4   = gbTRUE;
         miff->value.inr.r  = -Gr8MAX;
         miff->value.inr4.r = -Gr4MAX;
         returnTrue;
      }
      else if (buffer[1] == 'C')
      {
         miff->value.type   = gmiffValueTypeNUM;
         miff->value.isR8   = gbTRUE;
         miff->value.isR4   = gbTRUE;
         miff->value.inr.r  = -HUGE_VAL;
         miff->value.inr4.r = -HUGE_VALF;
         returnTrue;
      }
   }

   // Number is not a constant. Convert it to an integer or real number.

   // Read in the integer part.
   numIndex = 0;
   for (index = 0; index < count; index++)
   {
      breakIf(!buffer[index]);

      // Number is a Real.
      if (buffer[index] == '.')
      {
         // Weird number,  '+' '.'... or '-' '.'... with no integer starter.  Add 0.
         if (!isIntegerNumberFound)
         {
            numStr[index++] = '0';
         }

         // Let's go fetch the fraction portion of the number.
         goto GET_FRACTION;
      }

      // Number is Real with an exponent.
      if (buffer[index] == 'e' ||
          buffer[index] == 'E')
      {
         // Weird number,  +e... or -e... with no integer or fractional starter.  Result is 0
         if (!isIntegerNumberFound)
         {
            miff->value.type  = gmiffValueTypeNUM;
            miff->value.isI   = gbTRUE;
            miff->value.isN   = gbTRUE;
            miff->value.inr.i = 0;
            returnTrue;
         }

         goto GET_EXPONENT;
      }

      // Add to the integer portion.
      if      ('0' <= buffer[index] && buffer[index] <= '9')
      {
         numStr[numIndex++]   = (Gstr) buffer[index];
         isIntegerNumberFound = gbTRUE;
      }
      // Copy over the sign.
      else if (buffer[index] == '-')
      {
         // We already saw a sign or number.  Assume we have read the number.
         breakIf(isIntegerSignFound);

         numStr[numIndex++] = (Gstr) buffer[index];
         isIntegerSignFound = gbTRUE;
      }
      else if (buffer[index] == '+')
      {
         // We already saw a sign or number.  Assume we have read the number.
         breakIf(isIntegerSignFound);

         // Don't bother adding the + sign to the string.
         isIntegerSignFound = gbTRUE;
      }
      // Assuming this is the end of the number if we see any other letter.
      else
      {
         break;
      }
   }

   if (isIntegerNumberFound)
   {
      _GetNumInteger(miff, numStr);
      returnTrue;
   }

   // Just a + or -?  Weird.  Don't know what this means.
   returnFalse;

GET_FRACTION:
   // Decimal was found.
   numStr[numIndex++]      = '.';
   index++;

   for (; index < count; index++)
   {
      breakIf(!buffer[index]);

      // Exponent defined
      if (buffer[index] == 'e' ||
          buffer[index] == 'E')
      {
         // Weird number "-.e" or "+.e".  In either case, 0 to an exponent is still 0.
         if (!isIntegerNumberFound &&
             !isFractionalNumberFound)
         {
            miff->value.type  = gmiffValueTypeNUM;
            miff->value.isI   = gbTRUE;
            miff->value.isN   = gbTRUE;
            miff->value.inr.i = 0;
            returnTrue;
         }

         // There was at least an integer portion but no numbers after the decimal.
         // Remove the decimal since is adds nothing to the number.
         if (!isFractionalNumberFound)
         {
            numIndex--;
         }

         // Lets go fetch the exponent.
         goto GET_EXPONENT;
      }

      // Add to the fractional portion.
      if ('0' <= buffer[index] && buffer[index] <= '9')
      {
         numStr[numIndex++]      = (Gstr) buffer[index];
         isFractionalNumberFound = gbTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         break;
      }
   }

   // We have both integer portion and fractional portion.
   if (isFractionalNumberFound)
   {
      _GetNumReal(miff, numStr);
      returnTrue;
   }

   // No fractional digits found.  Treat it like an integer.
   _GetNumInteger(miff, numStr);
   returnTrue;

GET_EXPONENT:
   // Exponent was found
   numStr[numIndex++]    = 'e';
   index++;

   // Get the exponent
   for (; index < count; index++)
   {
      // End of the string.
      breakIf(!buffer[index]);

      // Get the digits.
      if      ('0' <= buffer[index] && buffer[index] <= '9')
      {
         numStr[numIndex++]    = (Gstr) buffer[index];
         isExponentNumberFound = gbTRUE;
      }
      // Negative sign.
      else if (buffer[index] == '-')
      {
         // We have already seen a number.  Seeing a sign would be wrong.  Assume this letter
         // belongs to something else.
         breakIf(isExponentNumberFound);

         // Two signs in a row and no number?  Something wrong with the number.  Assume everything
         // after the e doesn't belong to the number.
         breakIf(isExponentSignFound);

         numStr[numIndex++]  = (Gstr) buffer[index];
         isExponentSignFound = gbTRUE;
      }
      // Positive sign
      else if (buffer[index] == '+')
      {
         // We have already seen a number.  Seeing a sign would be wrong.  Assume this letter
         // belongs to something else.
         breakIf(isExponentNumberFound);

         // Two signs in a row and no number?  Something wrong with the number.  Assume everything
         // after the e doesn't belong to the number.
         breakIf(isExponentSignFound);

         // No need to add to the string.  Just eat the sign.
         isExponentSignFound = gbTRUE;
      }
      // Wasn't a sign or a digit.
      else
      {
         // No number found after the e.  Assume everything from the e onward number.
         if (!isFractionalNumberFound)
         {
            // Trim back to the 'e'
            loop
            {
               breakIf(numStr[numIndex - 1] == 'e');
               numStr[--numIndex] = 0;
            }
         }

         // We hit something that is not part of the number.
         break;
      }
   }

   // We actually have a real number.
   if (isExponentNumberFound ||
       isFractionalNumberFound)
   {
      _GetNumReal(miff, numStr);
      returnTrue;
   }

   // We just have an integer number.
   if (isIntegerNumberFound)
   {
      _GetNumInteger(miff, numStr);
      returnTrue;
   }

   // I don't know what we have, it sure isn't a number.
   returnFalse;
}

/**************************************************************************************************
func: _GetNumCount
**************************************************************************************************/
static Gcount _GetNumCount(Gstr * const str)
{
   Gn8 value;

   // This is a negative number.  Definitely not a natural number.
   if (str[0] == '-')
   {
      value = _NFromStr(&str[1]);
      if (value >= ((Gn8) GcountMAX) + 1)
      {
         return GcountMIN;
      }

      return - ((Gi4) value);
   }

   value = _NFromStr(str);

   // This unsigned integer is small enough to be a signed integer.
   if (value > GcountMAX)
   {
      return GcountMAX;
   }

   return (Gcount) value;
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
   }
}

/**************************************************************************************************
func: _GetNumReal
**************************************************************************************************/
static void _GetNumReal(Gmiff * const miff, Gstr * const str)
{
   miff->value.type   = gmiffValueTypeNUM;
   miff->value.isR8   = gbTRUE;
   miff->value.inr.i  = 0;
   miff->value.inr.n  = 0;
   miff->value.inr.r  = _atof_l(str, _MiffLocaleGet());

   // Check if it is representable in a single precision real value.
   if (-Gr4MAX <= miff->value.inr.r && miff->value.inr.r <= Gr4MAX)
   {
      miff->value.isR4   = gbTRUE;
      miff->value.inr4.r = (float) miff->value.inr.r;
   }
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
