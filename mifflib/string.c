/******************************************************************************
file:       string
author:     Robbert de Groot
company:    
copyright:  2021, Robbert de Groot

description:
String functions
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
global:
function:
******************************************************************************/
/******************************************************************************
func: _MiffStrEncodedToStr

Unescape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
MiffB _MiffStrEncodedToStr(MiffN * const strLen, MiffStr * const str)
{
   MiffN     index,
             eindex,
             charCount;

   // Count the number of characters that need escaping.
   charCount = 0;
   index     = 0;
   forCount (eindex, *strLen)
   {
      if (str[eindex] == '\\' &&
          eindex + 1 <  *strLen)
      {
         // Unescaping a \ character
         if      (str[eindex + 1] == '\\')
         {
            eindex++;
         }
         // Unescaping a tab character
         else if (str[eindex + 1] == 't')
         {
            str[eindex + 1] = '\t';
            eindex++;
         }
         // Unescaping a new line character
         else if (str[eindex + 1] == 'n')
         {
            str[eindex + 1] = '\n';
            eindex++;
         }
      }

      // adjust the string
      if (index != eindex)
      {
         str[index] = str[eindex];
      }
      index++;
   }

   // Update the string length.
   *strLen = index;

   returnTrue;
}

/******************************************************************************
func: _MiffStrToStrEncoded

Escape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
MiffB _MiffStrToStrEncoded(MiffN const strLen, MiffStr const * const str, 
   MiffN * const strEncodedLen, MiffStr ** const strEncoded)
{
   MiffN     index,
             eindex,
             charCount,
             eCount;
   MiffStr   *etemp;

   *strEncodedLen = 0;
   *strEncoded      = NULL;

   // Count the number of characters that need escaping.
   charCount = 0;
   forCount (index, strLen)
   {
      if (str[index] == 0x09 || // tab
          str[index] == 0x0a || // new line
          str[index] == '\\')
      {
         charCount++;
      }
   }

   eCount = strLen + charCount + 1;
   etemp  = _MiffMemCreateTypeArray(eCount, MiffStr);
   returnFalseIf(!etemp);

   eindex = strLen + charCount - 1;
   forCountDown (index, strLen)
   {
      switch (str[index])
      {
      case 0x09: etemp[eindex--] = 't';  etemp[eindex--] = '\\'; break;
      case 0x0a: etemp[eindex--] = 'n';  etemp[eindex--] = '\\'; break;
      case '\\': etemp[eindex--] = '\\'; etemp[eindex--] = '\\'; break;
      default:   etemp[eindex--] = str[index];                    break;
      }

      breakIf(index == 0);
   }

   *strEncodedLen = strLen + charCount;
   *strEncoded      = etemp;

   returnTrue;
}

/******************************************************************************
func: _MiffStrAppend
******************************************************************************/
MiffStr *_MiffStrAppend(MiffStr const * const a, MiffStr const * const b, MiffStr const * const c)
{
   MiffN4    length,
             offset;
   MiffStr   *result;

   length = _MiffStrGetCount(a) + _MiffStrGetCount(b) + _MiffStrGetCount(c) + 1;

   result = _MiffMemCreateTypeArray(length, MiffStr);
   returnNullIf(!result);

   offset = 0;
   length = _MiffStrGetCount(a);
   _MiffMemCopyTypeArray(length, MiffStr, &result[offset], a);
   
   offset += length;
   length  = _MiffStrGetCount(b);
   _MiffMemCopyTypeArray(length, MiffStr, &result[offset], b);
   
   offset += length;
   length  = _MiffStrGetCount(c);
   _MiffMemCopyTypeArray(length, MiffStr, &result[offset], c);

   return result;
}

/******************************************************************************
func: _MiffStrClone
******************************************************************************/
MiffStr *_MiffStrClone(MiffN const strLen, MiffStr const * const str)
{
   MiffStr *result;

   result = _MiffMemCreateTypeArray(strLen, MiffStr);
   returnNullIf(!result);

   _MiffMemCopyTypeArray(strLen, MiffStr, result, str);

   return result;
}
