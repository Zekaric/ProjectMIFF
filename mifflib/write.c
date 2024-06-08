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
MiffBool _MiffWriteStr(Miff const * const miff, MiffN const strLen, MiffStr const * const str)
{
   return miff->setBuffer(miff->dataRepo, strLen, str);
}

/******************************************************************************
func: _MiffWriteI
******************************************************************************/
MiffBool _MiffWriteI(Miff * const miff, MiffI const value)
{
   MiffN ntemp;

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

   return _MiffWriteN(miff, ntemp);
}

/******************************************************************************
func: _MiffWriteN
******************************************************************************/
MiffBool _MiffWriteN(Miff * const miff, MiffN const value)
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
      returnFalseIf(!miff->setBuffer(miff->dataRepo, 1, &string[index]));
   }

   returnTrue;
}

/******************************************************************************
func: _MiffWriteR4
******************************************************************************/
MiffBool _MiffWriteR4(Miff * const miff, MiffR4 const value)
{
   Miff4      vtemp;
   MiffBase64Data data;
   MiffN1     buffer[16];

   vtemp.r = value;
   _MiffByteSwap4(miff, &vtemp);

   data = _MiffBase64Restart(buffer);

   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[0]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[1]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[2]));
   returnFalseIf(!_MiffBase64Set(   &data, vtemp.byte[3]));
   returnFalseIf(!_MiffBase64SetEnd(&data));

   return _MiffWriteStr(miff, _MiffStrGetCount(buffer), buffer);
}

/******************************************************************************
func: _MiffWriteR4S
******************************************************************************/
MiffBool _MiffWriteR4S(Miff * const miff, MiffR4 const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.6g", _MiffLocaleGet(), value);
   
   return _MiffWriteStr(miff, _MiffStrGetCount(ctemp), ctemp);
}

/******************************************************************************
func: _MiffWriteR8
******************************************************************************/
MiffBool _MiffWriteR8(Miff * const miff, MiffR8 const value)
{
   Miff8      vtemp;
   MiffBase64Data data;
   MiffN1     buffer[16];

   vtemp.r = value;
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

   return _MiffWriteStr(miff, _MiffStrGetCount(buffer), buffer);
}

/******************************************************************************
func: _MiffWriteR8S
******************************************************************************/
MiffBool _MiffWriteR8S(Miff * const miff, MiffR8 const value)
{
   MiffStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.15g", _MiffLocaleGet(), value);

   return _MiffWriteStr(miff, _MiffStrGetCount(ctemp), ctemp);
}
