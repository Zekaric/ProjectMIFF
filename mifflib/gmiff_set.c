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
#include <stdio.h>

#include "gmiff_local.h"

/******************************************************************************
local:
prototype:
******************************************************************************/
static Gb _SetBinBuffer(Gmiff       * const miff, Gn8 const bufferCount, Gn1 const * const bufferData);
static Gb _SetBinByte(  Gmiff       * const miff, Gn1    const value);
static Gb _SetNumInt(   Gmiff       * const miff, GmiffValue const value);
static Gb _SetNumReal(  Gmiff       * const miff, GmiffValue const valueInput);

/******************************************************************************
global:
function
******************************************************************************/
/******************************************************************************
func: _MiffSetBinByte
******************************************************************************/
Gb _MiffSetBinByte(Gmiff * const miff, Gn1 const binByte)
{
   returnFalseIf(!_SetBinByte(miff, binByte));

   miff->valueIndex++;

   returnTrue;
}

/******************************************************************************
func: _MiffSetBuffer
******************************************************************************/
Gb _MiffSetBuffer(Gmiff const * const miff, Gcount const bufCount, Gn1 const * const buf)
{
   assert(bufCount < GcountMAX);
   return miff->setBuffer(miff->dataRepo, (Gn4) bufCount, buf);
}

/******************************************************************************
func: _MiffSetNumInt
******************************************************************************/
Gb _MiffSetNumInt(Gmiff * const miff, Gn8 const value)
{
   GmiffValue vtemp;

   _MiffMemClearType(GmiffValue, &vtemp);

   vtemp.inr.n = value;
   return _SetNumInt(miff, vtemp);
}

/******************************************************************************
func: _MiffSetStr
******************************************************************************/
Gb _MiffSetStr(Gmiff * const miff, Gcount const strLen, Gstr const * const str)
{
   Gindex index;
   Gindex bufferIndex;
   Gstr   bufferData[66];

   bufferIndex = 0;
   _MiffMemClearTypeArray(66, Gn1, bufferData);
   forCount(index, strLen)
   {
      // Escape single character slash, tab, and newline characters.
      switch (str[index])
      {
      case '\\':
      case '\t':
      case '\n':
         bufferData[bufferIndex++] = '\\';
         switch (str[index])
         {
         case '\\':
            // Slashes become two character "\\"
            bufferData[bufferIndex++] = '\\';
            break;

         case '\t':
            // Tabs become two character "\t"
            bufferData[bufferIndex++] = 't';
            break;

         case '\n':
            // New lines become two character "\n"
            bufferData[bufferIndex++] = 'n';
            break;
         }
         break;

      default:
         bufferData[bufferIndex++] = str[index];
         break;
      }

      // Filled the buffer, write out and restart.
      if (bufferIndex >= 64)
      {
         returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (Gn1 *) bufferData));

         bufferIndex = 0;
         _MiffMemClearTypeArray(66, Gn1, bufferData);
      }
   }

   // Write out the remainder.
   if (bufferIndex)
   {
      returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (Gn1 *) bufferData));
   }

   miff->valueIndex += strLen;

   returnTrue;
}

/******************************************************************************
func: _MiffSetValueHeader
******************************************************************************/
Gb _MiffSetValueHeader(Gmiff * const miff, GmiffValue const value)
{
   GmiffValue vtemp;

   _MiffMemClearType(GmiffValue, &vtemp);
   miff->valueIndex = 0;

   switch (value.type)
   {
   case gmiffValueTypeNULL:
      return         _MiffSetBuffer(miff, 1, (Gn1 *) "~");

   case gmiffValueTypeBIN:
      vtemp.inr.n = value.bufferCount;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "*"));
      returnFalseIf(!_SetNumInt( miff, vtemp));
      return         _MiffSetBuffer(miff, 1, (Gn1 *) " ");

   case gmiffValueTypeSTR:
      vtemp.inr.n = value.bufferCount;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "\""));
      returnFalseIf(!_SetNumInt( miff, vtemp));
      return         _MiffSetBuffer(miff, 1, (Gn1 *) " ");
   }

   returnTrue;
}

/******************************************************************************
func: _MiffSetValueData
******************************************************************************/
Gb _MiffSetValueData(Gmiff * const miff, GmiffValue const value)
{
   switch (value.type)
   {
   case gmiffValueTypeBIN:
      returnFalseIf(
         value.bufferCount == miffBufferCountUNKNOWN ||
         !value.bufferData.bin);
      return _SetBinBuffer(miff, value.bufferCount, value.bufferData.bin);

   case gmiffValueTypeNUM_INT:
      return _SetNumInt(miff, value);

   case gmiffValueTypeNUM_REAL:
      return _SetNumReal(miff, value);

   case gmiffValueTypeSTR:
      returnFalseIf(
         value.bufferCount == miffBufferCountUNKNOWN ||
         !value.bufferData.str);
      return _MiffSetStr(miff, value.bufferCount, value.bufferData.str);
   }

   returnTrue;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _SetBinBuffer
******************************************************************************/
static Gb _SetBinBuffer(Gmiff * const miff, Gn8 const bufferCount,
   Gn1 const * const bufferData)
{
   Gn8        index;

   // Testing the user way.
   forCount(index, bufferCount)
   {
      returnFalseIf(!_SetBinByte(miff, bufferData[index]));
   }

   returnTrue;
}

/****************************************************************************
func: _SetBinByte

The different between this and _SetNumInt is that this will not trim leading
0s and works only on Bytes.
******************************************************************************/
static Gb _SetBinByte(Gmiff * const miff, Gn1 const value)
{
   Gstr  string[2];
   Gstr  letters[] = "0123456789ABCDEF";

   string[0] = letters[(int) (value >> 4)];
   string[1] = letters[(int) (value & 0x0F)];

   return _MiffSetBuffer(miff, 2, (Gn1 *) string);
}

/****************************************************************************
func: _SetNumInt
******************************************************************************/
static Gb _SetNumInt(Gmiff * const miff, GmiffValue const valueInput)
{
   int       index,
             count,
             shift,
             stringIndex,
             ntemp;
   Gn8     mask;
   Gstr   string[16];
   Gstr   letters[] = "0123456789ABCDEF";
   GmiffValue value;

   value = valueInput;

   // If the number is 0, it's 0.
   if (value.inr.n == 0)
   {
      return _MiffSetBuffer(miff, 1, (Gn1 *) "0");
   }

   // Toss in the negative if the number is a negative integer.
   if (value.isI)
   {
      if (value.inr.i < 0)
      {
         _MiffSetBuffer(miff, 1, (Gn1 *) "-");

         // Make the number positive.
         value.inr.n = ((Gn8) -(value.inr.i + 1)) + 1;
      }
   }

   count       = 16;
   shift       = 60;
   mask        = 0xf000000000000000;
   stringIndex = 0;

   // Skip 0s
   forCount(index, count)
   {
      if (value.inr.n & mask)
      {
         break;
      }

      mask   = mask >> 4;
      shift -= 4;
   }

   // Fill in the buffer.
   for (; index < count; index++)
   {
      ntemp                 = (int) ((value.inr.n & mask) >> shift);
      string[stringIndex++] = letters[ntemp];

      mask   = mask >> 4;
      shift -= 4;
   }

   return _MiffSetBuffer(miff, stringIndex, (Gn1 *) string);
}

/****************************************************************************
func: _SetNumReal
******************************************************************************/
static Gb _SetNumReal(Gmiff * const miff, GmiffValue const valueInput)
{
   int       index,
             count,
             shift,
             stringIndex,
             ntemp;
   Gn8     mask;
   Gstr   string[16];
   Gstr   letters[]  = "GHIJKLMNOPQRSTUV";
   GmiffValue value;

   value = valueInput;

   if (value.isR4)
   {
      if      (value.inr4.r == 0)
      {
         return _MiffSetBuffer(miff, 3, (Gn1 *) "Z40");
      }
      else if (value.inr4.r == Gr4MAX)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z4+M");
      }
      else if (value.inr4.r == -Gr4MAX)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z4-M");
      }
      else if (value.inr4.r == HUGE_VALF)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z4+I");
      }
      else if (value.inr4.r == -HUGE_VALF)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z4-I");
      }
      else if (isnan(value.inr4.r))
      {
         return _MiffSetBuffer(miff, 3, (Gn1 *) "Z4?");
      }
   }
   else
   {
      if      (value.inr.r == 0)
      {
         return _MiffSetBuffer(miff, 3, (Gn1 *) "Z80");
      }
      else if (value.inr.r == Gr8MAX)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z8+M");
      }
      else if (value.inr.r == -Gr8MAX)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z8-M");
      }
      else if (value.inr.r == HUGE_VALF)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z8+I");
      }
      else if (value.inr.r == -HUGE_VALF)
      {
         return _MiffSetBuffer(miff, 4, (Gn1 *) "Z8-I");
      }
      else if (isnan(value.inr.r))
      {
         return _MiffSetBuffer(miff, 3, (Gn1 *) "Z8?");
      }
   }

   // Write out the number.  Real numbers are being written out as hex.
   stringIndex = 0;

   // We need to byte swap the value first.
   if (value.isR4)
   {
      count = 8;
      shift = 28;
      mask  = 0xf0000000;

      _MiffByteSwap4(miff, &value.inr4);

      // Fill in the buffer.
      for (index = 0; index < count; index++)
      {
         ntemp         = (int) ((value.inr4.n & mask) >> shift);
         string[index] = letters[ntemp];

         mask   = mask >> 4;
         shift -= 4;
      }
   }
   else
   {
      count = 16;
      shift = 60;
      mask  = 0xf000000000000000;

      _MiffByteSwap8(miff, &value.inr);

      // Fill in the buffer.
      for (index = 0; index < count; index++)
      {
         ntemp         = (int) ((value.inr.n & mask) >> shift);
         string[index] = letters[ntemp];

         mask   = mask >> 4;
         shift -= 4;
      }
   }

   return _MiffSetBuffer(miff, count, (Gn1 *) string);
}
