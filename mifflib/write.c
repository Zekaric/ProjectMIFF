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
func: _WriteC1
******************************************************************************/
MiffBool _WriteC1(Miff const * const miff, MiffC1 const * const value)
{
   return miff->setBuffer(miff->dataRepo, _C1GetCount(value), value);
}

/******************************************************************************
func: _WriteC2
******************************************************************************/
MiffBool _WriteC2(Miff const * const miff, MiffC2 const * const c2)
{
   MiffN4  c1Count;
   MiffC1 *c1;

   if (!_C2ToC1(_C2GetCount(c2) + 1, c2, &c1Count, &c1))
   {
      returnFalse;
   }

   _WriteC1(miff, c1);

   _MemDestroy(c1);

   returnTrue;
}

/******************************************************************************
func: _WriteC2Key
******************************************************************************/
MiffBool _WriteC2Key(Miff const * const miff, MiffC2 const * const key)
{
   MiffC1 c1Key[256];
   MiffN1 c1Count;

   _MemClearTypeArray(256, MiffC1, c1Key);

   returnFalseIf(!_C2ToC1Key(_C2GetCount(key), key, &c1Count, c1Key));
   return _WriteC1(miff, c1Key);
}

/******************************************************************************
func: _WriteI
******************************************************************************/
MiffBool _WriteI(Miff * const miff, MiffI8 const value)
{
   MiffN8 ntemp;

   ntemp = 0;
   if (value < 0)
   {
      miff->setBuffer(miff->dataRepo, 1, (MiffN1 *) "-");
      ntemp = -value;
   }
   else
   {
      ntemp =  value;
   }

   return _WriteN(miff, ntemp);
}

/******************************************************************************
func: _WriteN
******************************************************************************/
MiffBool _WriteN(Miff * const miff, MiffN8 const value)
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

/******************************************************************************
func: _WriteR4
******************************************************************************/
MiffBool _WriteR4(Miff * const miff, MiffR4 const value)
{
   Miff4      vtemp;
   Base64Data data;
   MiffN1     buffer[16];

   vtemp.r = value;
   _ByteSwap4(miff, &vtemp);

   data = _Base64Restart(buffer);

   returnFalseIf(!_Base64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_Base64SetEnd(&data));

   return _WriteC1(miff, buffer);
}

/******************************************************************************
func: _WriteR4S
******************************************************************************/
MiffBool _WriteR4S(Miff * const miff, MiffR4 const value)
{
   MiffC1 ctemp[80];

   sprintf_s((char *) ctemp, 80, "%.6g", value);
   
   return _WriteC1(miff, ctemp);
}

/******************************************************************************
func: _WriteR8
******************************************************************************/
MiffBool _WriteR8(Miff * const miff, MiffR8 const value)
{
   Miff8      vtemp;
   Base64Data data;
   MiffN1     buffer[16];

   vtemp.r = value;
   _ByteSwap8(miff, &vtemp);

   data = _Base64Restart(buffer);

   returnFalseIf(!_Base64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[4]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[5]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[6]));
   returnFalseIf(!_Base64Set(   &data, vtemp.byte[7]));
   returnFalseIf(!_Base64SetEnd(&data));

   return _WriteC1(miff, buffer);
}

/******************************************************************************
func: _WriteR8S
******************************************************************************/
MiffBool _WriteR8S(Miff * const miff, MiffR8 const value)
{
   MiffC1 ctemp[80];

   sprintf_s((char *) ctemp, 80, "%.15g", value);

   return _WriteC1(miff, ctemp);
}
