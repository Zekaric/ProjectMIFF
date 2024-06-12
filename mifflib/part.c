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
   MiffN index;
   MiffN value;

   value = 0;
   forCount(index, miff->readByteCount)
   {
      value = value * 10 + miff->readByteData[index] - '0';
   }

   return value;
}

/******************************************************************************
func: _MiffPartToValue
******************************************************************************/
MiffValue _MiffPartToValue(Miff const * const miff)
{
   MiffValue value;

   // The header defines the data.
   switch (miff->readByteData[0])
   {
   case '~':
      value.type = miffValueTypeNULL;
      break;

   case 'T':
      value.type = miffValueTypeB;
      value.b    = miffTRUE;
      break;

   case 'F':
      value.type = miffValueTypeB;
      value.b    = miffFALSE;
      break;

   case 'x':
      value.type  = miffValueTypeI;
      value.inr.n = _MiffHexToN(miff);
      break;

   case 'o':
      value.type  = miffValueTypeI;
      value.inr.n = _MiffOctToN(miff);
      break;

   case 'b':
      value.type  = miffValueTypeI;
      value.inr.n = _MiffBinToN(miff);
      break;

   case 'I':
      value.type  = miffValueTypeI;
      value.inr.n = _MiffBase64ToN(miff);
      break;

   case 'R':
      value.type  = miffValueTypeR;
      value.isR4  = (miff->readByteCount < 8);
      value.inr.n = _MiffBase64ToN(miff);
      break;

   case '+':
   case '-':
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
      value = _PartToValueINR(miff);
      break;

   case '\"':
      value = _PartToValueStrBuffer(miff);
      break;

   case '*':
      value = _PartToValueByteBuffer(miff);
      break;
   }
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _PartToValueINR
******************************************************************************/
MiffValue _PartToValueINR(Miff const * const miff)
{
   MiffValue value;
   char     *ctemp;

   // Complex number
   ctemp = strstr(&miff->readByteData[1], "+");
   if      (ctemp != NULL)
   {
      value.inr.r     = atof_l(miff->readButeData, _MiffLocaleGet());
      value.imaginary = atof_l(ctemp + 1,          _MiffLocaleGet());
   }
   // Real number
   else if (strstr(miff->readByteData, ".") != NULL ||
            strstr(miff->readByteData, "e") != NULL)
   {
      value.inr.r = atof_l(miff->readByteData, _MiffLocalGet());
   }
   else if (miff->readByteData[0] == '-')
   {
      value.inr.i = -_MiffPartToN(miff);
   }
   else
   {
      value.inr.n = _MiffPartToN(miff);
   }

   return value;
}

/******************************************************************************
func: _PartToValueStrBuffer
******************************************************************************/
MiffValue _PartToValueStrBuffer(Miff const * const miff)
{
   MiffValue value;

   return value;
}

/******************************************************************************
func: _PartToValueByteBuffer
******************************************************************************/
MiffValue _PartToValueByteBuffer(Miff const * const miff)
{
   MiffValue value;

   return value;
}
