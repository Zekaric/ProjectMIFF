/******************************************************************************
file:       write
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
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
#include "miff_local.h"

/******************************************************************************
local:
prototype:
******************************************************************************/
static void   _GetNumInt(        Miff * const miff, MiffValue * const value, MiffN4 const count, MiffN1 const * const buffer);
static MiffB  _GetNumIntNegative(Miff * const miff, MiffValue * const value, MiffN4 const count, MiffN1 const * const buffer);

static MiffN1 _ValueFromHexInt(  MiffN1 const value);
static MiffN1 _ValueFromHexReal( MiffN1 const value);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffGetBinByte
******************************************************************************/
MiffB _MiffGetBinByte(Miff * const miff, MiffN1 * const binByte)
{
   MiffN1 byte[2];

   returnFalseIf(!miff->getBuffer(miff->dataRepo, 2, byte));

   *binByte = (_ValueFromHexInt(byte[0]) << 4) | _ValueFromHexInt(byte[1]);

   miff->valueIndex++;

   returnTrue;
}

/******************************************************************************
func: _MiffGetKey
******************************************************************************/
MiffB _MiffGetKey(Miff * const miff)
{
   _MiffMemCopyTypeArray(miffKeyBYTE_COUNT, MiffN1, miff->currentName, miff->readData);

   miff->currentNameCount = miff->readCount;

   returnTrue;
}

/******************************************************************************
func: _MiffGetLineSkip
******************************************************************************/
MiffB _MiffGetLineSkip(Miff * const miff)
{
   MiffN1 byte;

   returnFalseIf(!miff);

   // Ignoring everything till the next record or eof.
   loop
   {
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));
      breakIf(byte == '\n');
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetNumInt
******************************************************************************/
MiffB _MiffGetNumInt(Miff * const miff, MiffN4 const count, MiffN1 const * const buffer)
{
   // Negative number.
   if (buffer[0] == '-')
   {
      return _GetNumIntNegative(miff, &miff->value, count, buffer);
   }

   _GetNumInt(miff, &miff->value, count, buffer);

   returnTrue;
}

/******************************************************************************
func: _MiffGetNumReal
******************************************************************************/
MiffB _MiffGetNumReal(Miff * const miff, MiffN4 const count, MiffN1 const * const buffer)
{
   MiffN index;
   MiffN value;
   MiffN letterValue;

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

      // Here just in case.  Should never be using lower case letters.
      case 'g':
      case 'h':
      case 'i':
      case 'j':
      case 'k':
      case 'l':
      case 'm':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'r':
      case 's':
      case 't':
      case 'u':
      case 'v':
         letterValue = letterValue - 'g';
         break;

      default:
         letterValue = 0;
         break;
      }

      value = (value << 4) + letterValue;
   }

   _MiffMemClearType(MiffValue, &miff->value);

   if      (count == 1 && (buffer[0] == 'G' || buffer[0] == 'g'))
   {
      miff->value.inr.r  = 0;
      miff->value.inr4.r = 0;
   }
   // double values always start with G
   else if (count == 16 || (buffer[0] == 'G' || buffer[0] == 'g'))
   {
      miff->value.inr.n = value;
      _MiffByteSwap8(miff, &miff->value.inr);
   }
   // float
   else
   {
      miff->value.inr4.n = (MiffN4) value;
      miff->value.isR4   = miffTRUE;
      _MiffByteSwap4(miff, &miff->value.inr4);
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetPart
******************************************************************************/
MiffB _MiffGetPart(Miff * const miff, MiffB const trimLeadingTabs)
{
   MiffN4   index;
   MiffN1   byte;
   MiffB    trimTabs;

   // Nothing left to read for this record.
   returnFalseIf(
      !miff ||
      miff->isRecordDone);

   trimTabs = trimLeadingTabs;
   index    = 0;
   loop
   {
      // End of file?
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

      // Eating tabs at the start of the line.
      if (trimTabs && byte == '\t')
      {
         continue;
      }
      else
      {
         trimTabs = miffFALSE;
      }

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      // Only read up to a key size.  Ignore everything else.
      if (index < miffKeyBYTE_COUNT)
      {
         miff->readData[index++] = byte;
      }
   }

   // Need NULL terminator.
   miff->readData[index] = 0;
   miff->readCount       = index;

   miff->isPartDone = miffTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetPartRest
******************************************************************************/
MiffB _MiffGetPartRest(Miff * const miff, MiffN1 const start)
{
   MiffN4   index;
   MiffN1   byte;

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
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      // Only read up to a key size.  Ignore everything else.
      if (index < miffKeyBYTE_COUNT)
      {
         miff->readData[index++] = byte;
      }
   }

   // Need NULL terminator.
   miff->readData[index] = 0;
   miff->readCount       = index;

   miff->isPartDone = miffTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetPartEnd

Skip to the next tab or newline.
******************************************************************************/
MiffB _MiffGetPartEnd(Miff * const miff)
{
   MiffN4   index;
   MiffN1   byte;

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
      breakIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

      // End of line or part, we are done reading.
      breakIf(byte == '\n' ||
              byte == '\t');
   }

   miff->isPartDone = miffTRUE;
   if (byte == '\n')
   {
      miff->isRecordDone = miffTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffGetStrLetter
******************************************************************************/
MiffData _MiffGetStrLetter(Miff * const miff, MiffStr * const letter)
{
   returnIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) letter), miffDataERROR); 

   // Escape single character slash, tab, and newline characters.
   switch (*letter)
   {
   case '\t':
      miff->isPartDone = miffTRUE;
      return miffDataIS_PART_DONE;

   case '\n':
      // An actual tab or cursor return character should never be inside a string.
      miff->isRecordDone = miffTRUE;
      return miffDataIS_RECORD_DONE;

   case '\\':
      returnIf(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) letter), miffDataERROR);
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

   return miffDataIS_DATA;
}

/******************************************************************************
func: _MiffGetValueHeader
******************************************************************************/
MiffStr _MiffGetValueHeader(Miff * const miff)
{
   MiffN1 byte;

   return0If(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &byte));

   return (MiffStr) byte;
}

/******************************************************************************
func: _MiffGetValueBufferCount
******************************************************************************/
MiffN _MiffGetValueBufferCount(Miff * const miff)
{
   MiffN     index;
   MiffStr   buffer[32];
   MiffValue value;

   forCount(index, 32)
   {
      return0If(!miff->getBuffer(miff->dataRepo, 1, (MiffN1 *) &buffer[index]));
      if (buffer[index] == ' ')
      {
         buffer[index] = 0;
         break;
      }
   }

   _GetNumInt(miff, &value, (MiffN4) index, (MiffN1 *) buffer);

   return value.inr.n;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _GetNumIntNegative
******************************************************************************/
static MiffB _GetNumIntNegative(Miff * const miff, MiffValue * const value, MiffN4 const count,
   MiffN1 const * const buffer)
{
   _GetNumInt(miff, value, count, buffer);

   // Out of range.  
   // If positive, nTemp can't be larger than UINT64_MAX.
   // If negative, nTemp can't be larger than UINT64_MAX + 1.  UINT64_MIN = -UINT64_MAX - 1.
   returnFalseIf(value->inr.n > ((MiffN) MiffI_MAX) + 1);
   
   value->inr.i = -((MiffI) value->inr.n - 1) - 1;
   
   returnTrue;
}

/******************************************************************************
func: _GetNumInt
******************************************************************************/
static void _GetNumInt(Miff * const miff, MiffValue * const value, MiffN4 const count, MiffN1 const * const buffer)
{
   MiffN index;
   MiffN ntemp;
   MiffN letterValue;

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

/******************************************************************************
func: _ValueFromHexInt
******************************************************************************/
static MiffN1 _ValueFromHexInt(MiffN1 const value)
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

/******************************************************************************
func: _ValueFromHexReal
******************************************************************************/
static MiffN1 _ValueFromHexReal(MiffN1 const value)
{
   switch (value)
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
      return value - 'G';

   case 'g':
   case 'h':
   case 'i':
   case 'j':
   case 'k':
   case 'l':
   case 'm':
   case 'n':
   case 'o':
   case 'p':
   case 'q':
   case 'r':
   case 's':
   case 't':
   case 'u':
   case 'v':
      return value - 'g';
   }

   return 0;
}