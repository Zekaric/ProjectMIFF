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

#include "miff_local.h"

/******************************************************************************
local:
prototype:
******************************************************************************/
static MiffB _SetBinBuffer(Miff       * const miff, MiffN const bufferCount, MiffN1 const * const bufferData);
static MiffB _SetBinByte(  Miff       * const miff, MiffN1    const value);
static MiffB _SetNumInt(   Miff       * const miff, MiffValue const value);
static MiffB _SetNumReal(  Miff       * const miff, MiffValue const valueInput);

/******************************************************************************
global:
function
******************************************************************************/
/******************************************************************************
func: _MiffSetBinByte
******************************************************************************/
MiffB _MiffSetBinByte(Miff * const miff, MiffN1 const binByte)
{
   returnFalseIf(!_SetBinByte(miff, binByte));

   miff->valueIndex++;

   returnTrue;
}

/******************************************************************************
func: _MiffSetBuffer
******************************************************************************/
MiffB _MiffSetBuffer(Miff const * const miff, MiffN const bufCount, MiffN1 const * const buf)
{
   assert(bufCount < MiffN4_MAX);
   return miff->setBuffer(miff->dataRepo, (MiffN4) bufCount, buf);
}

/******************************************************************************
func: _MiffSetNumInt
******************************************************************************/
MiffB _MiffSetNumInt(Miff * const miff, MiffN const value)
{
   MiffValue vtemp;

   _MiffMemClearType(MiffValue, &vtemp);

   vtemp.inr.n = value;
   return _SetNumInt(miff, vtemp);
}

/******************************************************************************
func: _MiffSetStr
******************************************************************************/
MiffB _MiffSetStr(Miff * const miff, MiffN const strLen, MiffStr const * const str)
{
   MiffN   index;
   MiffN   bufferIndex;
   MiffStr bufferData[66];

   bufferIndex = 0;
   _MiffMemClearTypeArray(66, MiffN1, bufferData);
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
         returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (MiffN1 *) bufferData));

         bufferIndex = 0;
         _MiffMemClearTypeArray(66, MiffN1, bufferData);
      }
   }

   // Write out the remainder.
   if (bufferIndex)
   {
      returnFalseIf(!_MiffSetBuffer(miff, bufferIndex, (MiffN1 *) bufferData));
   }

   miff->valueIndex += strLen;

   returnTrue;
}

/******************************************************************************
func: _MiffSetValueHeader
******************************************************************************/
MiffB _MiffSetValueHeader(Miff * const miff, MiffValue const value)
{
   MiffValue vtemp;

   _MiffMemClearType(MiffValue, &vtemp);
   miff->valueIndex = 0;

   switch (value.type)
   {
   case miffValueTypeNULL:
      return         _MiffSetBuffer(miff, 1, (MiffN1 *) "~");

   case miffValueTypeBIN:
      vtemp.inr.n = value.bufferCount;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "*"));
      returnFalseIf(!_SetNumInt( miff, vtemp));
      return         _MiffSetBuffer(miff, 1, (MiffN1 *) " ");

   case miffValueTypeSTR:
      vtemp.inr.n = value.bufferCount;

      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "\""));
      returnFalseIf(!_SetNumInt( miff, vtemp));
      return         _MiffSetBuffer(miff, 1, (MiffN1 *) " ");
   }

   returnTrue;
}

/******************************************************************************
func: _MiffSetValueData
******************************************************************************/
MiffB _MiffSetValueData(Miff * const miff, MiffValue const value)
{
   switch (value.type)
   {
   case miffValueTypeBIN:
      returnFalseIf(
         value.bufferCount == miffBufferCountUNKNOWN ||
         !value.bufferData.bin);
      return _SetBinBuffer(miff, value.bufferCount, value.bufferData.bin);

   case miffValueTypeNUM_INT:
      return _SetNumInt(miff, value);

   case miffValueTypeNUM_REAL:
      return _SetNumReal(miff, value);

   case miffValueTypeSTR:
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
static MiffB _SetBinBuffer(Miff * const miff, MiffN const bufferCount, 
   MiffN1 const * const bufferData)
{
   MiffN        index;
   
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
static MiffB _SetBinByte(Miff * const miff, MiffN1 const value)
{
   MiffStr  string[2];
   MiffStr  letters[] = "0123456789ABCDEF";

   string[0] = letters[(int) (value >> 4)];
   string[1] = letters[(int) (value & 0x0F)];

   return _MiffSetBuffer(miff, 2, (MiffN1 *) string);
}

/****************************************************************************
func: _SetNumInt
******************************************************************************/
static MiffB _SetNumInt(Miff * const miff, MiffValue const valueInput)
{
   int       index,
             count,
             shift,
             stringIndex,
             ntemp;
   MiffN     mask;
   MiffStr   string[16];
   MiffStr   letters[] = "0123456789ABCDEF";
   MiffValue value;

   value = valueInput;

   // If the number is 0, it's 0.
   if (value.inr.n == 0)
   {
      return _MiffSetBuffer(miff, 1, (MiffN1 *) "0");
   }

   // Toss in the negative if the number is a negative integer.
   if (value.isI)
   {
      if (value.inr.i < 0)
      {
         _MiffSetBuffer(miff, 1, (MiffN1 *) "-");

         // Make the number positive.
         value.inr.n = ((MiffN) -(value.inr.i + 1)) + 1;
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

   return _MiffSetBuffer(miff, stringIndex, (MiffN1 *) string);
}

/****************************************************************************
func: _SetNumReal
******************************************************************************/
static MiffB _SetNumReal(Miff * const miff, MiffValue const valueInput)
{
   int       index,
             count,
             shift,
             stringIndex,
             ntemp;
   MiffN     mask;
   MiffStr   string[16];
   MiffStr   letters[] = "GHIJKLMNOPQRSTUV";
   MiffValue value;

   value = valueInput;

   if (( value.isR4 && value.inr4.r == 0.0f) ||
       (!value.isR4 && value.inr.r  == 0.0))
   {
      return _MiffSetBuffer(miff, 1, (MiffN1 *) "G");
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

      // Skip leading 0s
      for (index = 0; index < count; index++)
      {
         ntemp = (int) ((value.inr4.n & mask) >> shift);
         breakIf(ntemp);

         mask   = mask >> 4;
         shift -= 4;
      }

      // Fill in the buffer.
      for (         ; index < count; index++)
      {
         ntemp                 = (int) ((value.inr4.n & mask) >> shift);
         string[stringIndex++] = letters[ntemp];

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

      // Skip leading 0s
      for (index = 0; index < count; index++)
      {
         ntemp = (int) ((value.inr.n & mask) >> shift);
         breakIf(ntemp);

         mask   = mask >> 4;
         shift -= 4;
      }

      // doubles start with a G
      if (index)
      { 
         string[stringIndex++] = 'G';
      }

      // Fill in the buffer.
      for (         ; index < count; index++)
      {
         ntemp                 = (int) ((value.inr.n & mask) >> shift);
         string[stringIndex++] = letters[ntemp];

         mask   = mask >> 4;
         shift -= 4;
      }
   }

   return _MiffSetBuffer(miff, stringIndex, (MiffN1 *) string);
}
