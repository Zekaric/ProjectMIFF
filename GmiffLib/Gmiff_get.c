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
static void   _GetNumInt(        Gmiff * const miff, GmiffValue * const value, Gn4 const count, Gn1 const * const buffer);
static Gb  _GetNumIntNegative(Gmiff * const miff, GmiffValue * const value, Gn4 const count, Gn1 const * const buffer);

static Gn1 _ValueFromHexInt(  Gn1 const value);

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _MiffGetBinByte
**************************************************************************************************/
Gb _MiffGetBinByte(Gmiff * const miff, Gn1 * const binByte)
{
   Gn1 byte[2];

   returnFalseIf(!miff->getBuffer(miff->dataRepo, 2, byte));

   *binByte = (_ValueFromHexInt(byte[0]) << 4) | _ValueFromHexInt(byte[1]);

   miff->valueIndex++;

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetKey
**************************************************************************************************/
Gb _MiffGetKey(Gmiff * const miff)
{
   _MiffMemCopyTypeArray(miff->currentName, Gn1, GkeyBYTE_COUNT, miff->readData);

   miff->currentNameCount = miff->readCount;

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
func: _MiffGetNumInt
**************************************************************************************************/
Gb _MiffGetNumInt(Gmiff * const miff, Gn4 const count, Gn1 const * const buffer)
{
   // Negative number.
   if (buffer[0] == '-')
   {
      return _GetNumIntNegative(miff, &miff->value, count, buffer);
   }

   _GetNumInt(miff, &miff->value, count, buffer);

   returnTrue;
}

/**************************************************************************************************
func: _MiffGetNumReal
**************************************************************************************************/
Gb _MiffGetNumReal(Gmiff * const miff, Gn4 const count, Gn1 const * const buffer)
{
   Gn8 index;
   Gn8 value;
   Gn8 letterValue;

   // Constants
   if      (buffer[0] == 'Z')
   {
      if      (strIsEqual(3, buffer, "Z80"))
      {
         miff->value.inr.r = 0;
      }
      if      (strIsEqual(4, buffer, "Z8+M"))
      {
         miff->value.inr.r = Gr8MAX;
         returnTrue;
      }
      else if (strIsEqual(4, buffer, "Z8-M"))
      {
         miff->value.inr.r = -Gr8MAX;
         returnTrue;
      }
      else if (strIsEqual(4, buffer, "Z8+I"))
      {
         miff->value.inr.r = HUGE_VAL;
         returnTrue;
      }
      else if (strIsEqual(4, buffer, "Z8-I"))
      {
         miff->value.inr.r = -HUGE_VAL;
         returnTrue;
      }
      else if (strIsEqual(3, buffer, "Z8?"))
      {
         miff->value.inr.r = GrNAN;
         returnTrue;
      }
      else if (strIsEqual(3, buffer, "Z40"))
      {
         miff->value.isR4 = gbTRUE;
         miff->value.inr4.r = 0;
      }
      else if (strIsEqual(4, buffer, "Z4+M"))
      {
         miff->value.isR4 = gbTRUE;
         miff->value.inr4.r = Gr4MAX;
         returnTrue;
      }
      else if (strIsEqual(4, buffer, "Z4-M"))
      {
         miff->value.isR4 = gbTRUE;
         miff->value.inr4.r = -Gr4MAX;
         returnTrue;
      }
      else if (strIsEqual(4, buffer, "Z4+I"))
      {
         miff->value.isR4 = gbTRUE;
         miff->value.inr4.r = HUGE_VALF;
         returnTrue;
      }
      else if (strIsEqual(4, buffer, "Z4-I"))
      {
         miff->value.isR4 = gbTRUE;
         miff->value.inr4.r = -HUGE_VALF;
         returnTrue;
      }
      else if (strIsEqual(3, buffer, "Z4?"))
      {
         miff->value.isR4 = gbTRUE;
         miff->value.inr4.r = GrNAN;
         returnTrue;
      }
      // unknown Z value
      returnFalse;
   }

   value = 0;
   forCount(index, count)
   {
      letterValue = buffer[index];
      switch (letterValue)
      {
      case 'G':
      case 'H':
      case 'I':
      case 'J':
      case 'K':
      case 'L':
      case 'M':
      case 'N':
      case 'O':
      case 'P':
      case 'Q':
      case 'R':
      case 'S':
      case 'T':
      case 'U':
      case 'V':
         letterValue = letterValue - 'G';
         break;

      default:
         letterValue = 0;
         break;
      }

      value = (value << 4) + letterValue;
   }

   // double values always user upper case letters.
   if      (count == 16)
   {
      miff->value.inr.n = value;
      _MiffByteSwap8(&miff->value.inr);
   }
   // float values always use lower case letters.
   else if (count == 8)
   {
      miff->value.isR4   = gbTRUE;
      miff->value.inr4.n = (Gn4) value;
      _MiffByteSwap4(&miff->value.inr4);
   }

   returnTrue;
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
      if (index < GkeyBYTE_COUNT)
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
      if (index < GkeyBYTE_COUNT)
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
func: _MiffGetValueHeader
**************************************************************************************************/
Gstr _MiffGetValueHeader(Gmiff * const miff)
{
   Gn1 byte;

   return0If(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &byte));

   return (Gstr) byte;
}

/**************************************************************************************************
func: _MiffGetValueBufferCount
**************************************************************************************************/
Gcount _MiffGetValueBufferCount(Gmiff * const miff)
{
   Gindex     index;
   Gstr       buffer[32];
   GmiffValue value;

   forCount(index, 32)
   {
      return0If(!miff->getBuffer(miff->dataRepo, 1, (Gn1 *) &buffer[index]));
      if (buffer[index] == ' ')
      {
         buffer[index] = 0;
         break;
      }
   }

   _GetNumInt(miff, &value, (Gn4) index, (Gn1 *) buffer);

   return (Gcount) value.inr.n;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _GetNumIntNegative
**************************************************************************************************/
static Gb _GetNumIntNegative(Gmiff * const miff, GmiffValue * const value, Gn4 const count,
   Gn1 const * const buffer)
{
   _GetNumInt(miff, value, count, buffer);

   // Out of range.
   // If positive, nTemp can't be larger than UINT64_MAX.
   // If negative, nTemp can't be larger than UINT64_MAX + 1.  UINT64_MIN = -UINT64_MAX - 1.
   returnFalseIf(value->inr.n > ((Gn8) Gi8MAX) + 1);

   value->inr.i = -((Gi8) value->inr.n - 1) - 1;

   returnTrue;
}

/**************************************************************************************************
func: _GetNumInt
**************************************************************************************************/
static void _GetNumInt(Gmiff * const miff, GmiffValue * const value, Gn4 const count, Gn1 const * const buffer)
{
   Gn8 index;
   Gn8 ntemp;
   Gn8 letterValue;

   miff;

   ntemp = 0;
   forCount(index, count)
   {
      letterValue = buffer[index];
      switch (letterValue)
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
         letterValue = letterValue - '0';
         break;

      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
         letterValue = 0xA + letterValue - 'A';
         break;

      // Here just in case.  Should never be using lower case letters.
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
         letterValue = 0xA + letterValue - 'a';
         break;

      default:
         letterValue = 0;
         break;
      }

      ntemp = (ntemp << 4) + letterValue;
   }

   value->inr.n = ntemp;
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
