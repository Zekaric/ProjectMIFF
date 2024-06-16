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
func: _MiffWriteStr
******************************************************************************/
MiffB _MiffWriteStr(Miff const * const miff, MiffN const strLen, MiffStr const * const str)
{
   assert(strLen < MiffN4_MAX);
   return miff->setBuffer(miff->dataRepo, (MiffN4) strLen, str);
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

#if 0
/******************************************************************************
func: _MiffWriteR4S
******************************************************************************/
MiffB _MiffWriteR4S(Miff * const miff, MiffR4 const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.6g", _MiffLocaleGet(), value);
   
   return _MiffWriteStr(miff, _MiffStrGetCount(ctemp), ctemp);
}

/******************************************************************************
func: _MiffWriteR8S
******************************************************************************/
MiffB _MiffWriteR8S(Miff * const miff, MiffR8 const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.15g", _MiffLocaleGet(), value);

   return _MiffWriteStr(miff, _MiffStrGetCount(ctemp), ctemp);
}
#endif

/******************************************************************************
func: _MiffWriteValue
******************************************************************************/
MiffB _MiffWriteValue(Miff * const miff, MiffValue const value)
{
   switch (value.type)
   {
   case miffValueTypeB:
      break;

   case miffValueTypeBIN:
      break;

   case miffValueTypeC:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         break;

      case miffValueFormatCIR_HUMAN_READABLE:
         break;
      }
      break;

   case miffValueTypeC4:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         break;

      case miffValueFormatCIR_HUMAN_READABLE:
         break;
      }
      break;

   case miffValueTypeI:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         break;

      case miffValueFormatCIR_HUMAN_READABLE:
         break;
      }
      break;

   case miffValueTypeN:
      switch (value.formatN)
      {
      case miffValueFormatN_B:
         break;

      case miffValueFormatN_HUMAN_READABLE:
         break;

      case miffValueFormatN_O:
         break;

      case miffValueFormatN_X:
         break;
      }
      break;

   case miffValueTypeNULL:
      break;

   case miffValueTypeR:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         break;

      case miffValueFormatCIR_HUMAN_READABLE:
         break;
      }
      break;

   case miffValueTypeR4:
      switch (value.formatCIR)
      {
      case miffValueFormatCIR_BASE64:
         break;

      case miffValueFormatCIR_HUMAN_READABLE:
         break;
      }
      break;

   case miffValueTypeSTR:
      break;
   }

   returnTrue;
}

/******************************************************************************
func: _MiffWriteValueOther
******************************************************************************/
MiffB _MiffWriteValueOther(Miff * const miff, MiffN const count, MiffStr const * const)
{

   returnTrue;
}
