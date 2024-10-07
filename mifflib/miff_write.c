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
static MiffB _Set4Base64(  Miff       * const miff, Miff4 const value);
static MiffB _Set8Base64(  Miff       * const miff, Miff8 const value);
static MiffB _SetBinBase64(Miff       * const miff, MiffN const bufferCount, MiffN1 const * const bufferData);
static MiffB _SetCBase64(  Miff       * const miff, MiffValue const * const value);
static MiffB _SetC4Base64( Miff       * const miff, MiffValue const * const value);
static MiffB _SetI(        Miff       * const miff, MiffI     const value);
static MiffB _SetNb(       Miff       * const miff, MiffN     const value);
static MiffB _SetNx(       Miff       * const miff, MiffN     const value);
static MiffB _SetR(        Miff       * const miff, MiffR     const value);
static MiffB _SetR4(       Miff       * const miff, MiffR4    const value);

/******************************************************************************
global:
function
******************************************************************************/
/******************************************************************************
func: _MiffSetBinByte
******************************************************************************/
MiffData _MiffSetBinByte(Miff * const miff, MiffN1 const binByte)
{
   BsfI  count;
   BsfN1 bsf[2];

   // Time to write out the buffer chunk
   if (miff->valueIndex == 0)
   {
      returnIf(!bsfPrep(&miff->bsfData), miffDataERROR);
   }

   // Add a new binByte.
   count = bsfToBsf(&miff->bsfData, binByte, &bsf[0], &bsf[1]);
   returnIf(count == 0, miffDataERROR);

   if (count == 1)
   {
      returnIf(!_MiffSetBuffer(miff, 1, bsf), miffDataERROR);
      miff->valueIndex++;
      return miffDataIS_DATA;
   }

   //if (count == 2)
   returnIf(!_MiffSetBuffer(miff, 2, bsf), miffDataERROR);
   miff->valueIndex++;
   return miffDataIS_DATA;
}

/******************************************************************************
func: _MiffSetBinByteEnd
******************************************************************************/
MiffB _MiffSetBinByteEnd(Miff * const miff)
{
   BsfI  count;
   BsfN1 bsf;

   // Finish the base64 chunk.
   count = bsfToBsfEnd(&miff->bsfData, &bsf);

   // If there is anything to write out then the first binByte will be non-0.
   if (count)
   {
      returnFalseIf(_MiffSetBuffer(miff, 1, &bsf));
   }

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
func: _MiffSetN
******************************************************************************/
MiffB _MiffSetN(Miff * const miff, MiffN const value)
{
   int    index,
          count,
          digit;
   MiffN  temp;
   MiffN1 string[32];

   temp = value;
   for (index = 0; ; index++)
   {
      digit = temp % 10;
      temp  = temp / 10;

      string[index] = (MiffN1) ('0' + digit);

      breakIf(temp == 0);
   }

   count = index + 1;
   forCountDown (index, count)
   {
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) &string[index]));
   }

   returnTrue;
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
MiffB _MiffSetValueHeader(Miff * const miff, MiffValue const * const value)
{
   miff->valueIndex = 0;

   switch (value->type)
   {
   case miffValueTypeB:
      if (value->b)
      {
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "T");
      }

      return _MiffSetBuffer(miff, 1, (MiffN1 *) "F");

   case miffValueTypeBIN:
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "*"));
      returnFalseIf(!_MiffSetN(     miff, value->bufferCount));
      return         _MiffSetBuffer(miff, 1, (MiffN1 *) " ");

   case miffValueTypeC:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         return _MiffSetBuffer(  miff, 1, (MiffN1 *) "C");

      case miffValueFormatCIR_TEXT:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "c");
      }
      break;

   case miffValueTypeI:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "I");

      case miffValueFormatCIR_TEXT:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "i");
      }
      break;

   case miffValueTypeN:
      switch (value->formatN)
      {
      case miffValueFormatN_B:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "b");

      case miffValueFormatN_BASE64:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "N");

      case miffValueFormatN_TEXT:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "n");

      case miffValueFormatN_X:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "x");
      }
      break;

   case miffValueTypeNULL:
      return _MiffSetBuffer(miff, 1, (MiffN1 *) "~");

   case miffValueTypeR:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "R");

      case miffValueFormatCIR_TEXT:
         return _MiffSetBuffer(miff, 1, (MiffN1 *) "r");
      }
      break;

   case miffValueTypeSTR:
      returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "\""));
      returnFalseIf(!_MiffSetN(     miff, value->bufferCount));
      return         _MiffSetBuffer(miff, 1, (MiffN1 *) " ");
   }

   returnTrue;
}

/******************************************************************************
func: _MiffSetValueData
******************************************************************************/
MiffB _MiffSetValueData(Miff * const miff, MiffValue const * const value)
{
   switch (value->type)
   {
   case miffValueTypeB:
      break;

   case miffValueTypeBIN:
      returnFalseIf(
         value->bufferCount == miffBufferCountUNKNOWN ||
         !value->bufferData.bin);
      return _SetBinBase64(miff, value->bufferCount, value->bufferData.bin);

   case miffValueTypeC:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         if (value->is4)
         {
            return _SetC4Base64(miff, value);
         }
         else
         {
            return _SetCBase64(miff, value);
         }

      case miffValueFormatCIR_TEXT:
         if (value->is4)
         {
            returnFalseIf(!_SetR4(miff, value->inr4.r));
            if (value->imaginary4.r != 0.0)
            {
               returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "+"));
               return         _SetR4(        miff, value->imaginary4.r);
            }
         }
         else 
         {
            returnFalseIf(!_SetR(miff, value->inr.r));
            if (value->imaginary.r != 0.0)
            {
               returnFalseIf(!_MiffSetBuffer(miff, 1, (MiffN1 *) "+"));
               return         _SetR(         miff, value->imaginary.r);
            }
         }
         break;
      }
      break;

   case miffValueTypeI:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         return _Set8Base64(miff, value->inr);

      case miffValueFormatCIR_TEXT:
         return _SetI(miff, (MiffN) value->inr.i);
      }
      break;

   case miffValueTypeN:
      switch (value->formatN)
      {
      case miffValueFormatN_B:
         return _SetNb(miff, value->inr.n);

      case miffValueFormatN_BASE64:
         return _Set8Base64(miff, value->inr);

      case miffValueFormatN_TEXT:
         return _MiffSetN(miff, value->inr.n);

      case miffValueFormatN_X:
         return _SetNx(miff, value->inr.n);
      }
      break;

   case miffValueTypeNULL:
      break;

   case miffValueTypeR:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         if (value->is4)
         {
            return _Set4Base64(miff, value->inr4);
         }
         else
         {
            return _Set8Base64(miff, value->inr);
         }

      case miffValueFormatCIR_TEXT:
         if (value->is4)
         {
            return _SetR4(miff, value->inr4.r);
         }
         else
         {
            return _SetR(miff, value->inr.r);
         }
      }
      break;

   case miffValueTypeSTR:
      returnFalseIf(
         value->bufferCount == miffBufferCountUNKNOWN ||
         !value->bufferData.str);
      return _MiffSetStr(miff, value->bufferCount, value->bufferData.str);
   }

   returnTrue;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _Set4Base64
******************************************************************************/
static MiffB _Set4Base64(Miff * const miff, Miff4 const value)
{
   Miff4 vtemp;
   BsfI  index;
   BsfN1 buffer[64];

   vtemp.n = value.n;
   _MiffByteSwap4(miff, &vtemp);

   returnFalseIf(bsfPrep(&miff->bsfData));

   index = 0;
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[0], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[1], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[2], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[3], &buffer[index], &buffer[index + 1]);
   index += bsfToBsfEnd(&miff->bsfData, &buffer[index]);
   
   return _MiffSetBuffer(miff, index, (MiffN1 *) buffer);
}

/******************************************************************************
func: _Set8Base64
******************************************************************************/
static MiffB _Set8Base64(Miff * const miff, Miff8 const value)
{
   Miff8 vtemp;
   BsfI  index;
   BsfN1 buffer[64];

   vtemp.n = value.n;
   _MiffByteSwap8(miff, &vtemp);

   returnFalseIf(bsfPrep(&miff->bsfData));

   index = 0;
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[0], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[1], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[2], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[3], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[4], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[5], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[6], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[7], &buffer[index], &buffer[index + 1]);
   index += bsfToBsfEnd(&miff->bsfData, &buffer[index]);

   return _MiffSetBuffer(miff, index, (MiffN1 *) buffer);
}

/******************************************************************************
func: _SetBinBase64
******************************************************************************/
static MiffB _SetBinBase64(Miff * const miff, MiffN const bufferCount, 
   MiffN1 const * const bufferData)
{
   MiffN        index;
#if 1
   // Testing the user way.
   forCount(index, bufferCount)
   {
      returnFalseIf(!miffSetValueBinData(miff, bufferData[index]));
   }
   return miffSetValueStop(miff);
#else
   BsfDataSet   data;
   MiffN1       buffer[5];

   data = bsfPrepSet(buffer);
   forCount(index, bufferCount)
   {

      returnFalseIf(!bsfSet(&data, bufferData[index]));
      index++;
      breakIf(index == bufferCount);

      returnFalseIf(!bsfSet(&data, bufferData[index]));
      index++;
      breakIf(index == bufferCount);

      returnFalseIf(!bsfSet(&data, bufferData[index]));

      returnFalseIf(!bsfSetEnd(&data));
      returnFalseIf(!_MiffSetBuffer(miff, 4, (MiffN1 *) buffer));

      returnTrueIf(index == bufferCount);

      data = bsfPrepSet(buffer);
   }

   returnFalseIf(!bsfSetEnd(&data));
   return         _MiffSetBuffer(miff, _MiffStrGetCount(buffer), (MiffN1 *) buffer);
#endif
}

/******************************************************************************
func: _SetCBase64
******************************************************************************/
static MiffB _SetCBase64(Miff * const miff, MiffValue const  * const value)
{
   Miff8 vtemp;
   BsfI  index;
   BsfN1 buffer[64];

   vtemp.n = value->inr.n;
   _MiffByteSwap8(miff, &vtemp);

   returnFalseIf(bsfPrep(&miff->bsfData));

   index = 0;
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[0], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[1], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[2], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[3], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[4], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[5], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[6], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[7], &buffer[index], &buffer[index + 1]);

   vtemp.n = value->imaginary.n;
   _MiffByteSwap8(miff, &vtemp);

   index += bsfToBsf(   &miff->bsfData, vtemp.byte[0], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[1], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[2], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[3], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[4], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[5], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[6], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[7], &buffer[index], &buffer[index + 1]);
   index += bsfToBsfEnd(&miff->bsfData, &buffer[index]);

   return _MiffSetBuffer(miff, index, (MiffN1 *) buffer);
}

/******************************************************************************
func: _SetC4Base64
******************************************************************************/
static MiffB _SetC4Base64(Miff * const miff, MiffValue const * const value)
{
   Miff4 vtemp;
   BsfI  index;
   BsfN1 buffer[64];

   vtemp.n = value->inr4.n;
   _MiffByteSwap4(miff, &vtemp);

   returnFalseIf(bsfPrep(&miff->bsfData));

   index = 0;
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[0], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[1], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[2], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[3], &buffer[index], &buffer[index + 1]);

   vtemp.n = value->imaginary4.n;
   _MiffByteSwap4(miff, &vtemp);

   index += bsfToBsf(   &miff->bsfData, vtemp.byte[0], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[1], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[2], &buffer[index], &buffer[index + 1]);
   index += bsfToBsf(   &miff->bsfData, vtemp.byte[3], &buffer[index], &buffer[index + 1]);
   index += bsfToBsfEnd(&miff->bsfData, &buffer[index]);

   return _MiffSetBuffer(miff, index, (MiffN1 *) buffer);
}

/******************************************************************************
func: _SetI
******************************************************************************/
static MiffB _SetI(Miff * const miff, MiffI const value)
{
   MiffN ntemp;

   ntemp = 0;
   if (value < 0)
   {
      _MiffSetBuffer(miff, 1, (MiffN1 *) "-");
      ntemp = -value;
   }
   else
   {
      ntemp =  value;
   }

   return _MiffSetN(miff, ntemp);
}

/******************************************************************************
func: _SetNb
******************************************************************************/
static MiffB _SetNb(Miff * const miff, MiffN const value)
{
   int      index,
            count,
            shift,
            stringIndex,
            ntemp;
   MiffN    mask;
   MiffStr  string[64];
   MiffStr  letters[] = "01";

   if (value == 0)
   {
      return _MiffSetBuffer(miff, 1, (MiffN1 *) "0");
   }

   stringIndex = 0;
   count       = 64;
   shift       = 63;
   mask        = ((MiffN) 1) << shift;

   // Skip 0s
   forCount(index, count)
   {
      if (value & mask)
      {
         break;
      }

      mask = mask >> 1;
      shift--;
   }

   // Fill in the buffer.
   for (; index < count; index++)
   {
      ntemp = (int) ((value & mask) >> shift);
      string[stringIndex++] = letters[ntemp];

      mask = mask >> 1;
      shift--;
   }

   return _MiffSetBuffer(miff, stringIndex, (MiffN1 *) string);
}

/******************************************************************************
func: _SetNx
******************************************************************************/
static MiffB _SetNx(Miff * const miff, MiffN const value)
{
   int      index,
            count,
            shift,
            stringIndex,
            ntemp;
   MiffN    mask;
   MiffStr  string[8];
   MiffStr  letters[] = "0123456789ABCDEF";

   if (value == 0)
   {
      return _MiffSetBuffer(miff, 1, (MiffN1 *) "0");
   }

   stringIndex = 0;
   count       = 8;
   shift       = 60;
   mask        = ((MiffN) 0xf) << shift;

   // Skip 0s
   forCount(index, count)
   {
      if (value & mask)
      {
         break;
      }

      mask   = mask >> 4;
      shift -= 4;
   }

   // Fill in the buffer.
   for (; index < count; index++)
   {
      ntemp                 = (int) ((value & mask) >> shift);
      string[stringIndex++] = letters[ntemp];

      mask   = mask >> 4;
      shift -= 4;
   }

   return _MiffSetBuffer(miff, stringIndex, (MiffN1 *) string);
}

/******************************************************************************
func: _SetR
******************************************************************************/
static MiffB _SetR(Miff * const miff, MiffR const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.15g", _MiffLocaleGet(), value);

   return _MiffSetBuffer(miff, _MiffStrGetCount(ctemp), (MiffN1 *) ctemp);
}

/******************************************************************************
func: _SetR4
******************************************************************************/
static MiffB _SetR4(Miff * const miff, MiffR4 const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.6g", _MiffLocaleGet(), value);
   
   return _MiffSetBuffer(miff, _MiffStrGetCount(ctemp), (MiffN1 *) ctemp);
}
