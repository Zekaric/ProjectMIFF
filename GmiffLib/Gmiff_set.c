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
static Gb _SetBinBuffer(Gmiff       * const miff, Gcount const count, Gn1 const * const data);
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
func: _MiffSetStr
**************************************************************************************************/
Gb _MiffSetStr(Gmiff * const miff, Gcount const strLen, Gstr const * const str)
{
   Gindex index;
   Gindex bufferIndex;
   Gstr   data[66];

   bufferIndex = 0;
   _MiffMemClearTypeArray(data, Gn1, 66);
   forCount(index, strLen)
   {
      // Escape single character slash, tab, and newline characters.
      switch (str[index])
      {
      case '\\':
      case '\t':
      case '\n':
         data[bufferIndex++] = '\\';
         switch (str[index])
         {
         case '\\':
            // Slashes become two character "\\"
            data[bufferIndex++] = '\\';
            break;

         case '\t':
            // Tabs become two character "\t"
            data[bufferIndex++] = 't';
            break;

         case '\n':
            // New lines become two character "\n"
            data[bufferIndex++] = 'n';
            break;
         }
         break;

      default:
         data[bufferIndex++] = str[index];
         break;
      }

      // Filled the buffer, write out and restart.
      if (bufferIndex >= 64)
      {
         returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (Gn1 *) data));

         bufferIndex = 0;
         _MiffMemClearTypeArray(data, Gn1, 66);
      }
   }

   // Write out the remainder.
   if (bufferIndex)
   {
      returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (Gn1 *) data));
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
   case gmiffValueTypeARRAY_COUNT:
      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "["));
      if (value.count == miffCountUNKNOWN)
      {
         return _MiffSetBuffer(miff, 1, (Gn1 *) "*");
      }
      vtemp.inr.n = value.count;
      return _SetNum(miff, vtemp);

   case gmiffValueTypeBLOCK_START:
      miff->scopeLevel++;
      return _MiffSetBuffer(miff, 1, (Gn1 *) "{");

   case gmiffValueTypeBLOCK_STOP:
      miff->scopeLevel--;
      // Called too many times.
      returnFalseIf(miff->scopeLevel < 0);
      return _MiffSetBuffer(miff, 1, (Gn1 *) "}");

   case gmiffValueTypeGROUP_COUNT:
      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "("));
      vtemp.inr.n = value.count;
      return _SetNum(miff, vtemp);


   case gmiffValueTypeNULL:
      return _MiffSetBuffer(miff, 1, (Gn1 *) "~");

   case gmiffValueTypeBIN:
      vtemp.inr.n = value.count;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "`"));
      // Only need a number if the count is larger than 4K
      if      (value.count == miffCountUNKNOWN)
      {
         returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "*"));
      }
      else if (value.count > 4096)
      {
         returnFalseIf(!_SetNum( miff, vtemp));
      }
      return _MiffSetBuffer(miff, 1, (Gn1 *) " ");

   case gmiffValueTypeSTR:
      vtemp.inr.n = value.count;

      // If the start of the string is any of the headers for the other types then we need to use
      // the string header.
      if (value.count == miffCountUNKNOWN)
      {
         return _MiffSetBuffer(miff, 3, (Gn1 *) "\"* ");
      }

      // If the string is larger than 4K
      // Or we are streaming the string and don't know how it startes
      // Or we know how it starts and it is one of the other header letters
      // then we need to use the string header.
      if (value.count       >  4096 ||
          value.data.str    == NULL ||
          value.data.str[0] == '0'  ||
          value.data.str[0] == '1'  ||
          value.data.str[0] == '2'  ||
          value.data.str[0] == '3'  ||
          value.data.str[0] == '4'  ||
          value.data.str[0] == '5'  ||
          value.data.str[0] == '6'  ||
          value.data.str[0] == '7'  ||
          value.data.str[0] == '8'  ||
          value.data.str[0] == '9'  ||
          value.data.str[0] == '+'  ||
          value.data.str[0] == '-'  ||
          value.data.str[0] == '{'  ||
          value.data.str[0] == '}'  ||
          value.data.str[0] == '['  ||
          value.data.str[0] == '('  ||
          value.data.str[0] == '`'  ||
          value.data.str[0] == '\"')
      {
         returnFalseIf(!_MiffSetBuffer(miff, 1, (Gn1 *) "\""));
         // Only write out the string count if larger than 4K
         if (value.count > 4096)
         {
            returnFalseIf(!_SetNum(miff, vtemp));
         }
         return _MiffSetBuffer(miff, 1, (Gn1 *) " ");
      }
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
         value.count == miffCountUNKNOWN ||
         !value.data.bin);

      return _SetBinBuffer(miff, value.count, value.data.bin);

   case gmiffValueTypeNUM:
      return _SetNum(miff, value);

   case gmiffValueTypeSTR:
      returnFalseIf(
         value.count == miffCountUNKNOWN ||
         !value.data.str);

      return _MiffSetStr(miff, value.count, value.data.str);
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
static Gb _SetBinBuffer(Gmiff * const miff, Gcount const count, Gn1 const * const data)
{
   Gindex index;

   // Testing the user way.
   forCount(index, count)
   {
      returnFalseIf(!_MiffSetBinByte(miff, data[index]));
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
         if (value.inr4.r == Gr4MAX)       return _MiffSetBuffer(miff, 2, (Gn1 *) "+R");
         if (value.inr4.r == -Gr4MAX)      return _MiffSetBuffer(miff, 2, (Gn1 *) "-R");
         if (value.inr4.r == HUGE_VALF)    return _MiffSetBuffer(miff, 2, (Gn1 *) "+C");
         if (value.inr4.r == -HUGE_VALF)   return _MiffSetBuffer(miff, 2, (Gn1 *) "-C");
         if (isnan(value.inr4.r))          return _MiffSetBuffer(miff, 1, (Gn1 *) "?");

         _sprintf_s_l((char *) string, 80, "%.*g", _MiffLocaleGet(), FLT_DECIMAL_DIG, value.inr4.r);
      }
      else
      {
         if (value.inr.r == Gr8MAX)        return _MiffSetBuffer(miff, 2, (Gn1 *) "+R");
         if (value.inr.r == -Gr8MAX)       return _MiffSetBuffer(miff, 2, (Gn1 *) "-R");
         if (value.inr.r == HUGE_VALF)     return _MiffSetBuffer(miff, 2, (Gn1 *) "+C");
         if (value.inr.r == -HUGE_VALF)    return _MiffSetBuffer(miff, 2, (Gn1 *) "-C");
         if (isnan(value.inr.r))           return _MiffSetBuffer(miff, 1, (Gn1 *) "?");

         _sprintf_s_l((char *) string, 80, "%.*g", _MiffLocaleGet(), DBL_DECIMAL_DIG, value.inr.r);
      }

      return _MiffSetBuffer(miff, gstrGetCount(string), (Gn1 *) string);
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
      if (value.inr.i == Gi8MIN) return _MiffSetBuffer(miff, 2, (Gn1 *) "-I");
      if (value.inr.i == Gi8MAX) return _MiffSetBuffer(miff, 2, (Gn1 *) "+I");

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
      if (value.inr.n == Gn8MAX) return _MiffSetBuffer(miff, 2, (Gn1 *) "+N");
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
