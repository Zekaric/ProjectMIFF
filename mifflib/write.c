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

#include "local.h"

/******************************************************************************
global:
function
******************************************************************************/
/******************************************************************************
func: _MiffWriteCBase64
******************************************************************************/
MiffB _MiffWriteCBase64(Miff * const miff, MiffValue const value)
{
   Miff8          vtemp;
   MiffBase64Data data;
   MiffN1         buffer[64];

   vtemp.n = value.inr.n;
   _MiffByteSwap8(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[4]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[5]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[6]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[7]));

   vtemp.n = value.imaginary.n;
   _MiffByteSwap8(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[4]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[5]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[6]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[7]));
   returnFalseIf(!_MiffBase64SetEnd(&data));

   return _MiffWriteStr(miff, _MiffStrGetCount(buffer), (MiffStr *) buffer);
}

/******************************************************************************
func: _MiffWriteC4Base64
******************************************************************************/
MiffB _MiffWriteC4Base64(Miff * const miff, MiffValue const value)
{
   Miff4          vtemp;
   MiffBase64Data data;
   MiffN1         buffer[64];

   vtemp.n = value.inr4.n;
   _MiffByteSwap4(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));

   vtemp.n = value.imaginary4.n;
   _MiffByteSwap4(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_MiffBase64SetEnd(&data));

   return _MiffWriteStr(miff, _MiffStrGetCount(buffer), (MiffStr *) buffer);
}

/******************************************************************************
func: _MiffWriteI
******************************************************************************/
MiffB _MiffWriteI(Miff * const miff, MiffI const value)
{
   MiffN ntemp;

   ntemp = 0;
   if (value < 0)
   {
      miff->setBuffer(miff->dataRepo, 1, "-");
      ntemp = -value;
   }
   else
   {
      ntemp =  value;
   }

   return _MiffWriteN(miff, ntemp);
}

/******************************************************************************
func: _MiffWriteN
******************************************************************************/
MiffB _MiffWriteN(Miff * const miff, MiffN const value)
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
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, (MiffStr *) &string[index]));
   }

   returnTrue;
}

/******************************************************************************
func: _MiffWriteNb
******************************************************************************/
MiffB _MiffWriteNb(Miff * const miff, MiffN const value)
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
      return miff->setBuffer(miff->dataRepo, 1, "0");
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

   return miff->setBuffer(miff->dataRepo, stringIndex, (MiffStr *) string);
}

/******************************************************************************
func: _MiffWriteNx
******************************************************************************/
MiffB _MiffWriteNx(Miff * const miff, MiffN const value)
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
      return miff->setBuffer(miff->dataRepo, 1, "0");
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

   return miff->setBuffer(miff->dataRepo, stringIndex, (MiffStr *) string);
}

/******************************************************************************
func: _MiffWrite4Base64
******************************************************************************/
MiffB _MiffWrite4Base64(Miff * const miff, Miff4 const value)
{
   Miff4          vtemp;
   MiffBase64Data data;
   MiffN1         buffer[64];

   vtemp.n = value.n;
   _MiffByteSwap4(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_MiffBase64SetEnd(&data));

   return _MiffWriteStr(miff, _MiffStrGetCount(buffer), (MiffStr *) buffer);
}

/******************************************************************************
func: _MiffWrite8Base64
******************************************************************************/
MiffB _MiffWrite8Base64(Miff * const miff, Miff8 const value)
{
   Miff8          vtemp;
   MiffBase64Data data;
   MiffN1         buffer[64];

   vtemp.n = value.n;
   _MiffByteSwap8(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[4]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[5]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[6]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[7]));
   returnFalseIf(!_MiffBase64SetEnd(&data));

   return _MiffWriteStr(miff, _MiffStrGetCount(buffer), (MiffStr *) buffer);
}

/******************************************************************************
func: _MiffWriteBufferBase64
******************************************************************************/
MiffB _MiffWriteBufferBase64(Miff * const miff, MiffN const bufferCount, MiffN1 const * const bufferData)
{
   MiffN          index;
   MiffBase64Data data;
   MiffN1         buffer[5];

   forCount(index, bufferCount)
   {
      data = _MiffBase64Restart(buffer);

      returnFalseIf(!_MiffBase64Set(&data, bufferData[index]));
      index++;
      breakIf(index == bufferCount);

      returnFalseIf(!_MiffBase64Set(&data, bufferData[index]));
      index++;
      breakIf(index == bufferCount);

      returnFalseIf(!_MiffBase64Set(&data, bufferData[index]));
      index++;

      returnFalseIf(_MiffBase64SetEnd(&data));
      returnFalseIf(!_MiffWriteStr(miff, 4, (MiffStr *) buffer));

      returnTrueIf(index == bufferCount);
   }

   returnFalseIf(_MiffBase64SetEnd(&data));
   return        _MiffWriteStr(miff, _MiffStrGetCount(buffer), (MiffStr *) buffer);
}

/******************************************************************************
func: _MiffWriteR
******************************************************************************/
MiffB _MiffWriteR(Miff * const miff, MiffR const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.15g", _MiffLocaleGet(), value);

   return _MiffWriteStr(miff, _MiffStrGetCount(ctemp), ctemp);
}

/******************************************************************************
func: _MiffWriteR4
******************************************************************************/
MiffB _MiffWriteR4(Miff * const miff, MiffR4 const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.6g", _MiffLocaleGet(), value);
   
   return _MiffWriteStr(miff, _MiffStrGetCount(ctemp), ctemp);
}

/******************************************************************************
func: _MiffWriteStr
******************************************************************************/
MiffB _MiffWriteStr(Miff const * const miff, MiffN const strLen, MiffStr const * const str)
{
   assert(strLen < MiffN4_MAX);
   return miff->setBuffer(miff->dataRepo, (MiffN4) strLen, str);
}

/******************************************************************************
func: _MiffWriteStrEscaped
******************************************************************************/
MiffB _MiffWriteStrEscaped(Miff const * const miff, MiffN const strLen, MiffStr const * const str)
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
         returnFalseIf(!_MiffWriteStr(miff, bufferIndex, bufferData));

         bufferIndex = 0;
         _MiffMemClearTypeArray(66, MiffN1, bufferData);
      }
   }

   // Write out the remainder.
   if (bufferIndex)
   {
      returnFalseIf(!_MiffWriteStr(miff, bufferIndex, bufferData));
   }

   returnTrue;
}

/******************************************************************************
func: _MiffWriteValue
******************************************************************************/
MiffB _MiffWriteValue(Miff * const miff, MiffValue const value)
{
   switch (value.type)
   {
   case miffValueTypeB:
      if (value.b)
      {
         return _MiffWriteStr(miff, 1, "T");
      }

      return _MiffWriteStr(miff, 1, "F");

   case miffValueTypeBIN:
      returnFalseIf(!_MiffWriteStr(miff, 1, "*"));
      returnFalseIf(!_MiffWriteN(  miff, value.bufferCount));
      returnFalseIf(!_MiffWriteStr(miff, 1, " "));
      return         _MiffWriteBufferBase64(miff, value.bufferCount, value.bufferData.bin);

   case miffValueTypeC:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         returnFalseIf(!_MiffWriteStr(    miff, 1, "C"));
         return         _MiffWriteCBase64(miff, value);

      case miffValueFormatCIR_TEXT:
         returnFalseIf(!_MiffWriteStr(miff, 1, "c"));
         returnFalseIf(!_MiffWriteR(  miff, value.inr.r));
         if (value.imaginary.r != 0.0)
         {
            returnFalseIf(!_MiffWriteStr(miff, 1, "+"));
            return         _MiffWriteR(  miff, value.imaginary.r);
         }
         break;
      }
      break;

   case miffValueTypeC4:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         returnFalseIf(!_MiffWriteStr(     miff, 1, "C"));
         return         _MiffWriteC4Base64(miff, value);

      case miffValueFormatCIR_TEXT:
         returnFalseIf(!_MiffWriteStr(miff, 1, "c"));
         returnFalseIf(!_MiffWriteR4( miff, value.inr4.r));
         if (value.imaginary4.r != 0.0)
         {
            returnFalseIf(!_MiffWriteStr(miff, 1, "+"));
            return         _MiffWriteR4( miff, value.imaginary4.r);
         }
         break;
      }
      break;

   case miffValueTypeI:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         returnFalseIf(!_MiffWriteStr(    miff, 1, "I"));
         return         _MiffWrite8Base64(miff, value.inr);

      case miffValueFormatCIR_TEXT:
         returnFalseIf(!_MiffWriteStr(miff, 1, "i"));
         return         _MiffWriteI(  miff, (MiffN) value.inr.i);
      }
      break;

   case miffValueTypeN:
      switch (value.formatN)
      {
      case miffValueFormatN_B:
         returnFalseIf(!_MiffWriteStr(miff, 1, "b"));
         return         _MiffWriteNb( miff, value.inr.n);

      case miffValueFormatN_BASE64:
         returnFalseIf(!_MiffWriteStr(    miff, 1, "N"));
         return         _MiffWrite8Base64(miff, value.inr);

      case miffValueFormatN_TEXT:
         returnFalseIf(!_MiffWriteStr(miff, 1, "n"));
         return         _MiffWriteN(  miff, value.inr.n);

      case miffValueFormatN_X:
         returnFalseIf(!_MiffWriteStr(miff, 1, "x"));
         return         _MiffWriteNx( miff, value.inr.n);
      }
      break;

   case miffValueTypeNULL:
      return _MiffWriteStr(miff, 1, "~");

   case miffValueTypeR:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         returnFalseIf(!_MiffWriteStr(    miff, 1, "R"));
         return         _MiffWrite8Base64(miff, value.inr);

      case miffValueFormatCIR_TEXT:
         returnFalseIf(!_MiffWriteStr(miff, 1, "r"));
         return         _MiffWriteR(  miff, value.inr.r);
      }
      break;

   case miffValueTypeR4:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         returnFalseIf(!_MiffWriteStr(    miff, 1, "R"));
         return         _MiffWrite4Base64(miff, value.inr4);

      case miffValueFormatCIR_TEXT:
         returnFalseIf(!_MiffWriteStr(miff, 1, "r"));
         return         _MiffWriteR4( miff, value.inr4.r);
      }
      break;

   case miffValueTypeSTR:
      returnFalseIf(!_MiffWriteStr(miff, 1, "\""));
      returnFalseIf(!_MiffWriteN(  miff, value.bufferCount));
      returnFalseIf(!_MiffWriteStr(miff, 1, " "));
      return         _MiffWriteStrEscaped(miff, value.bufferCount, (MiffStr *) value.bufferData.str);
   }

   returnTrue;
}
