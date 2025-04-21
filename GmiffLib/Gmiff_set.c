/**************************************************************************************************
file:       gmiff_set
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Set functions local to the library.
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
#include <stdio.h>

#include "gmiff_local.h"

/**************************************************************************************************
local:
prototype:
**************************************************************************************************/
static Gb _SetBinBuffer(Gmiff       * const miff, Gcount const bufferCount, Gn1 const * const bufferData);
static Gb _SetBinByte(  Gmiff       * const miff, Gn1    const value);
static Gb _SetNum(      Gmiff       * const miff, GmiffValue const value);

/**************************************************************************************************
global:
function
**************************************************************************************************/
/**************************************************************************************************
func: _MiffSetBinByte
**************************************************************************************************/
Gb _MiffSetBinByte(Gmiff * const miff, Gn1 const binByte)
{
   returnFalseIf(!_SetBinByte(miff, binByte));

   miff->bufferIndex++;

   returnTrue;
}

/**************************************************************************************************
func: _MiffSetBuffer
**************************************************************************************************/
Gb _MiffSetBuffer(Gmiff const * const miff, Gcount const bufCount, Gn1 const * const buf)
{
   assert(bufCount < GcountMAX);
   return miff->setBuffer(miff->dataRepo, (Gn4) bufCount, buf);
}

/**************************************************************************************************
func: _MiffSetNumInt
**************************************************************************************************/
Gb _MiffSetNumInt(Gmiff * const miff, Gn8 const value)
{
   GmiffValue vtemp;

   _MiffMemClearType(&vtemp, GmiffValue);

   vtemp.inr.n = value;
   return _SetNum(miff, vtemp);
}

/**************************************************************************************************
func: _MiffSetStr
**************************************************************************************************/
Gb _MiffSetStr(Gmiff * const miff, Gcount const strLen, Gstr const * const str)
{
   Gindex index;
   Gindex bufferIndex;
   Gstr   bufferData[66];

   bufferIndex = 0;
   _MiffMemClearTypeArray(bufferData, Gn1, 66);
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
         _MiffMemClearTypeArray(bufferData, Gn1, 66);
      }
   }

   // Write out the remainder.
   if (bufferIndex)
   {
      returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (Gn1 *) bufferData));
   }

   miff->bufferIndex += strLen;

   returnTrue;
}

/**************************************************************************************************
func: _MiffSetValueHeader
**************************************************************************************************/
Gb _MiffSetValueHeader(Gmiff * const miff, GmiffValue const value)
{
   GmiffValue vtemp;

   _MiffMemClearType(&vtemp, GmiffValue);
   miff->bufferIndex = 0;

   switch (value.type)
   {
   case gmiffValueTypeARRAY_DEF:
      // This needs to be set before any actual values.
      returnFalseIf(miff->valueIndex != 0);

      miff->arrayCount = value.inr.n;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "["));
      if (value.inr.n == miffCountUNKNOWN)
      {
         return _MiffSetBuffer(miff, 1, (Gn1 *) "*");
      }
      return _SetNum(miff, value);

   case gmiffValueTypeBLOCK_DEF:
      // This needs to be set before any actual values.
      returnFalseIf(miff->valueIndex != 0);

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "{"));
      miff->scopeLevel++;
      returnTrue;

   case gmiffValueTypeGROUP_DEF:
      // This needs to be set before any actual values.
      returnFalseIf(miff->valueIndex != 0);

      miff->groupCount = value.inr.n;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "("));
      return         _SetNum(    miff, value);


   case gmiffValueTypeNULL:
      miff->valueIndex++;
      return _MiffSetBuffer(miff, 1, (Gn1 *) "~");

   case gmiffValueTypeBIN:
      miff->valueIndex++;
      vtemp.inr.n = value.bufferCount;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "."));
      // Only need a number if the count is larger than 4K
      if (value.bufferCount > 4096)
      {
         returnFalseIf(!_SetNum( miff, vtemp));
      }
      return _MiffSetBuffer(miff, 1, (Gn1 *) " ");

   case gmiffValueTypeSTR:
      miff->valueIndex++;
      vtemp.inr.n = value.bufferCount;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "\""));
      // Only need a number if the count is larger than 4K
      if (value.bufferCount > 4096)
      {
         returnFalseIf(!_SetNum( miff, vtemp));
      }
      return _MiffSetBuffer(miff, 1, (Gn1 *) " ");
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiffSetValueData
**************************************************************************************************/
Gb _MiffSetValueData(Gmiff * const miff, GmiffValue const value)
{
   switch (value.type)
   {
   case gmiffValueTypeBIN:
      returnFalseIf(
         value.bufferCount == miffBufferCountUNKNOWN ||
         !value.bufferData.bin);

      return _SetBinBuffer(miff, value.bufferCount, value.bufferData.bin);

   case gmiffValueTypeNUM:
      return _SetNum(miff, value);

   case gmiffValueTypeSTR:
      returnFalseIf(
         value.bufferCount == miffBufferCountUNKNOWN ||
         !value.bufferData.str);

      return _MiffSetStr(miff, value.bufferCount, value.bufferData.str);
   }

   returnTrue;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _SetBinBuffer
**************************************************************************************************/
static Gb _SetBinBuffer(Gmiff * const miff, Gcount const bufferCount, Gn1 const * const bufferData)
{
   Gindex index;

   // Testing the user way.
   forCount(index, bufferCount)
   {
      returnFalseIf(!_MiffSetBinByte(miff, bufferData[index]));
   }

   returnTrue;
}

/**************************************************************************************************
func: _SetBinByte
**************************************************************************************************/
static Gb _SetBinByte(Gmiff * const miff, Gn1 const value)
{
   Gstr  string[2];
   Gstr  letters[] = "0123456789ABCDEF";

   string[0] = letters[(int) (value >> 4)];
   string[1] = letters[(int) (value & 0x0F)];

   return _MiffSetBuffer(miff, 2, (Gn1 *) string);
}

/**************************************************************************************************
func: _SetNum
**************************************************************************************************/
static Gb _SetNum(Gmiff * const miff, GmiffValue const valueInput)
{
   int        index,
              count,
              digit;
   Gn8        nval;
   Gstr       string[80];
   GmiffValue value;

   value = valueInput;

   if (value.isR4 ||
       value.isR8)
   {
      if (value.isR4)
      {
         if (value.inr4.r == 0)            return _MiffSetBuffer(miff, 1, (Gn1 *) "0");
         if (value.inr4.r == Gr4MAX)       return _MiffSetBuffer(miff, 1, (Gn1 *) "R");
         if (value.inr4.r == -Gr4MAX)      return _MiffSetBuffer(miff, 1, (Gn1 *) "r");
         if (value.inr4.r == HUGE_VALF)    return _MiffSetBuffer(miff, 1, (Gn1 *) "C");
         if (value.inr4.r == -HUGE_VALF)   return _MiffSetBuffer(miff, 1, (Gn1 *) "c");
         if (isnan(value.inr4.r))          return _MiffSetBuffer(miff, 1, (Gn1 *) "?");

         _sprintf_s_l((char *) string, 80, "%.*g", _MiffLocaleGet(), FLT_DECIMAL_DIG, value.inr4.r);
      }
      else
      {
         if (value.inr.r == 0)             return _MiffSetBuffer(miff, 1, (Gn1 *) "0");
         if (value.inr.r == Gr8MAX)        return _MiffSetBuffer(miff, 1, (Gn1 *) "R");
         if (value.inr.r == -Gr8MAX)       return _MiffSetBuffer(miff, 1, (Gn1 *) "r");
         if (value.inr.r == HUGE_VALF)     return _MiffSetBuffer(miff, 1, (Gn1 *) "C");
         if (value.inr.r == -HUGE_VALF)    return _MiffSetBuffer(miff, 1, (Gn1 *) "c");
         if (isnan(value.inr.r))           return _MiffSetBuffer(miff, 1, (Gn1 *) "?");

         _sprintf_s_l((char *) string, 80, "%.*g", _MiffLocaleGet(), DBL_DECIMAL_DIG, value.inr.r);
      }

      return _MiffSetBuffer(miff, _MiffStrGetCount(string), (Gn1 *) string);
   }

   // If the number is 0, it's 0.
   if (value.inr.n == 0)
   {
      return _MiffSetBuffer(miff, 1, (Gn1 *) "0");
   }

   // Toss in the negative if the number is a negative integer.
   if (value.isI)
   {
      // Send out a constant.
      if (value.inr.i == Gi8MIN) return _MiffSetBuffer(miff, 1, (Gn1 *) "i");
      if (value.inr.i == Gi8MAX) return _MiffSetBuffer(miff, 1, (Gn1 *) "I");

      // Negative number.
      if (value.inr.i < 0)
      {
         _MiffSetBuffer(miff, 1, (Gn1 *) "-");

         // Make the number positive.
         value.inr.n = ((Gn8) -(value.inr.i + 1)) + 1;
      }
   }
   else
   {
      // Send out a constant.
      if (value.inr.n == Gn8MAX) return _MiffSetBuffer(miff, 1, (Gn1 *) "N");
   }

   nval = value.inr.n;
   loopCount(index)
   {
      digit = nval % 10;
      nval  = nval / 10;

      string[index] = (Gn1) ('0' + digit);

      breakIf(nval == 0);
   }

   count = index + 1;
   forCountDown(index, count)
   {
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, (Gn1 *) &string[index]));
   }

   returnTrue;
}
