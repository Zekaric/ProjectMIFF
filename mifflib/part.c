/******************************************************************************
file:       part
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Line part functions.
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
#include "local.h"

/******************************************************************************
lib local:
prototype:
******************************************************************************/
static MiffB       _PartAToC(       Miff const * const miff, MiffR * const real, MiffR * const imaginary);
static MiffI       _PartAToI(       Miff const * const miff);
static MiffN       _PartAToN(       Miff const * const miff);
static MiffR       _PartAToR(       Miff const * const miff);
static MiffB       _PartBase64ToC(  Miff const * const miff, MiffN const count, MiffN1 const * const buffer, Miff8 * const real, Miff8 * const imaginary);
static MiffB       _PartBase64ToC4( Miff const * const miff, MiffN const count, MiffN1 const * const buffer, Miff4 * const real, Miff4 * const imaginary);
static MiffB       _PartBase64ToN(  Miff const * const miff, MiffN const count, MiffN1 const * const buffer, Miff8 * const value);
static MiffN4      _PartBase64ToN4( Miff const * const miff, MiffN const count, MiffN1 const * const buffer, Miff4 * const value);
static MiffN       _PartBinToN(     Miff const * const miff);
static MiffN       _PartHexToN(     Miff const * const miff);
static MiffN       _PartOctToN(     Miff const * const miff);

/******************************************************************************
function:
******************************************************************************/
/******************************************************************************
func: _MiffPartToKey
******************************************************************************/
MiffB _MiffPartToKey(Miff * const miff)
{
   MiffN len;

   len = min(256, miff->readByteCount);
   _MiffMemCopyTypeArray(len, MiffN1, miff->currentName, miff->readByteData);
   miff->currentName[len] = 0;
   miff->currentNameCount = len - 1;

   returnTrue;
}

/******************************************************************************
func: _MiffPartToN
******************************************************************************/
MiffN _MiffPartToN(Miff const * const miff)
{
   return _MiffAToN(miff->readByteCount, miff->readByteData);
}

/******************************************************************************
func: _MiffPartToValue

Read the know values that are of fixed size.
******************************************************************************/
MiffB _MiffPartToValue(Miff const * const miff, MiffValue * const value)
{
   // The header defines the data.
   switch (value->type)
   {
   case miffValueTypeN:
      switch (value->formatN)
      {
      case miffValueFormatN_HUMAN_READABLE:
         value->inr.n = _PartAToN(miff);
         break;

      case miffValueFormatN_BASE64:
         value->is4 = (miff->readByteCount < 8);
         if (!value->is4)
         {
            _PartBase64ToN( miff, miff->readByteCount, miff->readByteData, &value->inr);
         }
         else
         {
            _PartBase64ToN4(miff, miff->readByteCount, miff->readByteData, &value->inr4);

            // Auto promotion yes.  Auto demotion no, caller needs to control that.
            value->inr.n = value->inr4.n;
         }
         break;

      case miffValueFormatN_X:
         value->inr.n = _PartHexToN(miff);
         break;

      case miffValueFormatN_O:
         value->inr.n = _PartOctToN(miff);
         break;

      case miffValueFormatN_B:
         value->inr.n = _PartBinToN(miff);
         break;
      }
      break;

   case miffValueTypeC:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_HUMAN_READABLE:
         _PartAToC(miff, &value->inr.r, &value->imaginary.r);
         break;

      case miffValueFormatCIR_BASE64:
         value->is4 = (miff->readByteCount < 16);
         if (!value->is4)
         {
            _PartBase64ToC( miff, miff->readByteCount, miff->readByteData, &value->inr,  &value->imaginary);
         }
         else
         {
            value->type = miffValueTypeC4;
            _PartBase64ToC4(miff, miff->readByteCount, miff->readByteData, &value->inr4, &value->imaginary4);

            // Auto promotion yes.  Auto demotion no, caller needs to control that.
            value->inr.r       = value->inr4.r;
            value->imaginary.r = value->imaginary4.r;
         }
         break;
      }
      break;

   case miffValueTypeI:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_HUMAN_READABLE:
         value->inr.i = _PartAToI(miff);
         break;

      case miffValueFormatCIR_BASE64:
         value->is4  = (miff->readByteCount < 8);
         if (!value->is4)
         {
            _PartBase64ToN( miff, miff->readByteCount, miff->readByteData, &value->inr);
         }
         else
         {
            _PartBase64ToN4(miff, miff->readByteCount, miff->readByteData, &value->inr4);

            // Auto promotion yes.  Auto demotion no, caller needs to control that.
            value->inr.i = value->inr4.i;
         }
         break;
      }
      break;

   case miffValueTypeR:
      switch (value->formatCIR)
      {
      case miffValueFormatCIR_HUMAN_READABLE:
         value->inr.r = _PartAToR(miff);
         break;

      case miffValueFormatCIR_BASE64:
         value->is4  = (miff->readByteCount < 8);
         if (!value->is4)
         {
            _PartBase64ToN( miff, miff->readByteCount, miff->readByteData, &value->inr);
         }
         else
         {
            value->type = miffValueTypeR4;
            _PartBase64ToN4(miff, miff->readByteCount, miff->readByteData, &value->inr4);

            // Auto promotion yes.  Auto demotion no, caller needs to control that.
            value->inr.r = value->inr4.r;
         }
         break;
      }
   }

   returnTrue;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _PartAToC
******************************************************************************/
static MiffB _PartAToC(Miff const * const miff, MiffR * const real, MiffR * const imaginary)
{
   MiffN1 *n1Temp;

   // Get the real value of the complex number.
   *real = _strtod_l((char *) miff->readByteData, (char **) &n1Temp, _MiffLocaleGet());

   // Check to see if the imaginary number exists.
   if ((MiffN) (n1Temp - miff->readByteData) < miff->readByteCount &&
       n1Temp[0] == '+')
   {
      // Get the imaginary value of the complex number.
      *imaginary = _strtod_l((char *) &(n1Temp[1]), (char **) &n1Temp, _MiffLocaleGet());
   }

   returnTrue;
}

/******************************************************************************
func: _PartAToI
******************************************************************************/
static MiffI _PartAToI(Miff const * const miff)
{
   MiffB     isPositive;
   MiffN     count;
   MiffN1   *npTemp;
   MiffN     nTemp;

   isPositive = miffTRUE;

   if (miff->readByteData[1] == '-')
   {
      isPositive = miffFALSE;
      count      = miff->readByteCount - 2;
      npTemp     = &(miff->readByteData[2]);
   }
   else
   {
      count  = miff->readByteCount - 1;
      npTemp = &(miff->readByteData[1]);
   }

   nTemp = _MiffAToN(count, npTemp);
   // Out of range.  
   // If positive, nTemp can't be larger than UINT64_MAX.
   // If negative, nTemp can't be larger than UINT64_MAX + 1.  UINT64_MIN = -UINT64_MAX - 1.
   if ((!isPositive && nTemp > ((MiffN) MiffI8_MAX) + 1) ||
       ( isPositive && nTemp > ((MiffN) MiffI8_MAX)))
   {
      return 0;
   }
   
   // Returning the negative.
   if (!isPositive)
   {
      // WTF?  2's complement.  UINT64_MIN != -UINT64_MAX, it is one less.
      return -((MiffI) nTemp - 1) - 1;
   }
   
   return (MiffI) nTemp;
}

/******************************************************************************
func: _PartAToN
******************************************************************************/
static MiffN _PartAToN(Miff const * const miff)
{
   return _MiffAToN(miff->readByteCount - 1, &(miff->readByteData[1]));
}

/******************************************************************************
func: _PartAToR
******************************************************************************/
static MiffR _PartAToR(Miff const * const miff)
{
   MiffN1 *n1Temp;

   return _strtod_l((char *) miff->readByteData, (char **) &n1Temp, _MiffLocaleGet());
}

/******************************************************************************
func: _PartBase64ToC
******************************************************************************/
static MiffB _PartBase64ToC(Miff const * const miff, MiffN const count, MiffN1 const * const buffer,
   Miff8 * const real, Miff8 * const imaginary)
{
   MiffBase64Data data;

   count;

   // cast safe.  We are not writing, just reading.
   data = _MiffBase64Restart((MiffN1 *) buffer);

   returnFalseIf(!_MiffBase64Get(&data, &real->byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[3]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[4]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[5]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[6]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[7]));

   _MiffByteSwap8(miff, real);

   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[3]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[4]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[5]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[6]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[7]));

   _MiffByteSwap8(miff, imaginary);

   returnTrue;
}

/******************************************************************************
func: _PartBase64ToC4
******************************************************************************/
static MiffB _PartBase64ToC4(Miff const * const miff, MiffN const count, MiffN1 const * const buffer,
   Miff4 * const real, Miff4 * const imaginary)
{
   MiffBase64Data data;

   count;

   // cast safe.  We are not writing, just reading.
   data = _MiffBase64Restart((MiffN1 *) buffer);

   returnFalseIf(!_MiffBase64Get(&data, &real->byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &real->byte[3]));

   _MiffByteSwap4(miff, real);

   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &imaginary->byte[3]));

   _MiffByteSwap4(miff, imaginary);

   returnTrue;
}

/******************************************************************************
func: _PartBase64ToN
******************************************************************************/
static MiffB _PartBase64ToN(Miff const * const miff, MiffN const count, MiffN1 const * const buffer,
   Miff8 * const value)
{
   MiffBase64Data data;

   count;

   // cast safe.  We are not writing, just reading.
   data = _MiffBase64Restart((MiffN1 *) buffer);

   returnFalseIf(!_MiffBase64Get(&data, &value->byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[3]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[4]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[5]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[6]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[7]));

   _MiffByteSwap8(miff, value);

   returnTrue;
}

/******************************************************************************
func: _PartBase64ToN4
******************************************************************************/
static MiffN4 _PartBase64ToN4(Miff const * const miff, MiffN const count, MiffN1 const * const buffer,
   Miff4 * const value)
{
   MiffBase64Data data;

   count;

   // cast safe.  We are not writing, just reading.
   data = _MiffBase64Restart((MiffN1 *) buffer);

   returnFalseIf(!_MiffBase64Get(&data, &value->byte[0]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[1]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[2]));
   returnFalseIf(!_MiffBase64Get(&data, &value->byte[3]));

   _MiffByteSwap4(miff, value);

   returnTrue;
}

/******************************************************************************
func: _PartBinToN
******************************************************************************/
static MiffN _PartBinToN(Miff const * const miff)
{
   MiffN index;
   MiffN value;
   MiffN letterValue;

   value = 0;
   forCount(index, miff->readByteCount)
   {
      letterValue = miff->readByteData[index + 1];
      switch (letterValue)
      {
      case '0':
      case '1':
         letterValue = letterValue - '0';
         break;

      default:
         letterValue = 0;
         break;
      }

      value += (value << 1) + letterValue;
   }

   return value;
}

/******************************************************************************
func: _PartHexToN
******************************************************************************/
static MiffN _PartHexToN(Miff const * const miff)
{
   MiffN index;
   MiffN value;
   MiffN letterValue;

   value = 0;
   forCount(index, miff->readByteCount)
   {
      letterValue = miff->readByteData[index + 1];
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
         letterValue = 10 + letterValue - 'A';
         break;

      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
         letterValue = 10 + letterValue - 'a';
         break;

      default:
         letterValue = 0;
         break;
      }

      value += (value << 4) + letterValue;
   }

   return value;
}

/******************************************************************************
func: _PartOctToN
******************************************************************************/
static MiffN _PartOctToN(Miff const * const miff)
{
   MiffN index;
   MiffN value;
   MiffN letterValue;

   value = 0;
   forCount(index, miff->readByteCount)
   {
      letterValue = miff->readByteData[index + 1];
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
         letterValue = letterValue - '0';
         break;

      default:
         letterValue = 0;
         break;
      }

      value += (value << 3) + letterValue;
   }

   return value;
}
