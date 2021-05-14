/******************************************************************************
file:       local
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Write functions local to the library.
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) !!!!YEAR!!!!, Robbert de Groot

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
global:
function
******************************************************************************/
/******************************************************************************
func: _WriteTxtRecordDivider
******************************************************************************/
MiffBool _WriteTxtRecordDivider(Miff const * const miff)
{
   return miff->setBuffer(miff->dataRepo, 1, "\t");
}

/******************************************************************************
func: _WriteTxtRecordEnder
******************************************************************************/
MiffBool _WriteTxtRecordEnder(Miff const * const miff)
{
   return miff->setBuffer(miff->dataRepo, 1, "\n");
}

/******************************************************************************
func: _WriteTxtStringC1
******************************************************************************/
MiffBool _WriteTxtStringC1(Miff const * const miff, MiffC1 const * const value)
{
   return miff->setBuffer(miff->dataRepo, miffC1GetLength(value), value);
}

/******************************************************************************
func: _WriteTxtStringC2
******************************************************************************/
MiffBool _WriteTxtStringC2(Miff const * const miff, MiffC2 const * const value)
{
   int index,
       count;

   count = miffC2GetLength(value);
   for (index = 0; index < count; index++)
   {
      index += _WriteTxtStringC2Letter(miff, &value[index]);

      returnFalseIf(index < 0);
   }

   returnTrue;
}

/******************************************************************************
func: _WriteTxtStringC2Letter

This will first determine if the current letter is a pair or a single.
Then convert the pair/single to UTF8.
Then write the UTF8 to the buffer.
******************************************************************************/
int _WriteTxtStringC2Letter(Miff const * const miff, MiffC2 const * const value)
{
   MiffN4 fullLetter;


}

/******************************************************************************
func: _WriteTxtValue1
******************************************************************************/
MiffBool _WriteTxtValue1(Miff const * const miff, MiffValueType const type, Miff1 const * const value)
{
   if (type == miffValueTypeBOOLEAN)
   {
      if (value.n)
      {
         return miff->setBuffer(miff->dataRepo, 1, "T");
      }
      
      return miff->setBuffer(miff->dataRepo, 1, "F");
   }

   if (type == miffValueTypeI1)
   {
      return _WriteTxtValueI(miff, value);
   }

   return _WriteTxtValueN(miff, value);
}

/******************************************************************************
func: _WriteTxtValue2
******************************************************************************/
MiffBool _WriteTxtValue2(Miff const * const miff, MiffValueType const type, Miff2 const * const value)
{
   if (type == miffValueTypeR2)
   {
      Miff8 vtemp;

      vtemp = value;
      _ByteSwap2(miff, vtemp);

      returnFalseIf(!_Base64Set(miff, value->byte[0]));
      returnFalseIf(!_Base64Set(miff, value->byte[1]));

      return _Base64SetEnd(miff);
   }

   if (type == miffValueTypeI2)
   {
      return _WriteTxtValueI(miff, value);
   }

   return _WriteTxtValueN(miff, value);
}

/******************************************************************************
func: _WriteTxtValue4
******************************************************************************/
MiffBool _WriteTxtValue4(Miff const * const miff, MiffValueType const type, Miff4 const * const value)
{
   if (type == miffValueTypeR4)
   {
      Miff8 vtemp;

      vtemp = value;
      _ByteSwap4(miff, vtemp);

      returnFalseIf(!_Base64Set(miff, value->byte[0]));
      returnFalseIf(!_Base64Set(miff, value->byte[1]));
      returnFalseIf(!_Base64Set(miff, value->byte[2]));
      returnFalseIf(!_Base64Set(miff, value->byte[3]));

      return _Base64SetEnd(miff);
   }

   if (type == miffValueTypeI4)
   {
      return _WriteTxtValueI(miff, value);
   }

   return _WriteTxtValueN(miff, value);
}

/******************************************************************************
func: _WriteTxtValue8
******************************************************************************/
MiffBool _WriteTxtValue8(Miff const * const miff, MiffValueType const type, Miff8 const * const value)
{
   if (type == miffValueTypeR8)
   {
      Miff8 vtemp;

      vtemp = value;
      _ByteSwap8(miff, vtemp);

      returnFalseIf(!_Base64Set(miff, value->byte[0]));
      returnFalseIf(!_Base64Set(miff, value->byte[1]));
      returnFalseIf(!_Base64Set(miff, value->byte[2]));
      returnFalseIf(!_Base64Set(miff, value->byte[3]));
      returnFalseIf(!_Base64Set(miff, value->byte[4]));
      returnFalseIf(!_Base64Set(miff, value->byte[5]));
      returnFalseIf(!_Base64Set(miff, value->byte[6]));
      returnFalseIf(!_Base64Set(miff, value->byte[7]));

      return _Base64SetEnd(miff);
   }

   if (type == miffValueTypeI8)
   {
      return _WriteTxtValueI(miff, value);
   }

   return _WriteTxtValueN(miff, value);
}

/******************************************************************************
func: _WriteTxtValueI
******************************************************************************/
MiffBool _WriteTxtValueI(Miff const * const miff, Miff8 const * const value)
{
   Miff8 vtemp;

   vtemp = value;
   if (vtemp->i < 0)
   {
      miff->setBuffer(miff->dataRepo, 1, "-");
      vtemp->n = -vtemp->i;
   }
   return _WriteTxtValueN(miff, &vtemp);
}

/******************************************************************************
func: _WriteTxtValueN
******************************************************************************/
MiffBool _WriteTxtValueN(Miff const * const miff, Miff8 const * const value)
{
   int    index,
          count,
          digit;
   MiffN8 temp;
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
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &string[index]));
   }

   returnTrue;
}
