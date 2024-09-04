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
func: _JsonStrEncodedToStr

Unescape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
JsonB _JsonStrEncodedToStr(JsonN4 * const strLen, JsonStr * const str)
{
   JsonN4    index,
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
func: _JsonStrToStrEncoded

Escape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
JsonB _JsonStrToStrEncoded(JsonN4 const strLen, JsonStr const * const str, 
   JsonN4 * const strEncodedLen, JsonStr ** const strEncoded)
{
   JsonN4    index,
             eindex,
             charCount,
             eCount;
   JsonStr   *etemp;

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
   etemp  = _JsonMemCreateTypeArray(eCount, JsonStr);
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
func: _JsonSTrToKey
******************************************************************************/
void _JsonSTrToKey(JsonN4 const strLen, JsonStr const * const str, JsonN1 * const keyLen,
   JsonStr * const key)
{
   *keyLen = (JsonN1) min(255, strLen);
   _JsonMemCopyTypeArray(*keyLen, JsonN1, key, str);
   key[*keyLen] = 0;
}

/******************************************************************************
func: _JsonStrToI
******************************************************************************/
JsonI _JsonStrToI(JsonN4 const strLen, JsonStr const * const str)
{
   JsonB  isNegative;
   JsonN4 c1Index;
   JsonN  value;

   isNegative = jsonFALSE;
   value      = 0;
   forCount(c1Index, strLen)
   {
      if (c1Index == 0)
      {
         if (str[c1Index] == '-')
         {
            isNegative = jsonTRUE;
            continue;
         }
      }

      value = value * 10 + str[c1Index] - '0';
   }

   if (isNegative)
   {
      return -((JsonI) value);
   }

   return value;
}

/******************************************************************************
func: _JsonStrToN
******************************************************************************/
JsonN _JsonStrToN(JsonN4 const strLen, JsonStr const * const str)
{
   JsonN4   c1Index;
   JsonN   value;

   value = 0;
   forCount(c1Index, strLen)
   {
      value = value * 10 + str[c1Index] - '0';
   }

   return value;
}

/******************************************************************************
func: _JsonC2Append
******************************************************************************/
JsonStr *_JsonC2Append(JsonStr const * const a, JsonStr const * const b, JsonStr const * const c)
{
   JsonN4    length,
             offset;
   JsonStr   *result;

   length = _JsonStrGetCount(a) + _JsonStrGetCount(b) + _JsonStrGetCount(c) + 1;

   result = _JsonMemCreateTypeArray(length, JsonStr);
   returnNullIf(!result);

   offset = 0;
   length = _JsonStrGetCount(a);
   _JsonMemCopyTypeArray(length, JsonStr, &result[offset], a);
   
   offset += length;
   length  = _JsonStrGetCount(b);
   _JsonMemCopyTypeArray(length, JsonStr, &result[offset], b);
   
   offset += length;
   length  = _JsonStrGetCount(c);
   _JsonMemCopyTypeArray(length, JsonStr, &result[offset], c);

   return result;
}

/******************************************************************************
func: _JsonC2Clone
******************************************************************************/
JsonStr *_JsonC2Clone(JsonN4 const c2Count, JsonStr const * const c2)
{
   JsonStr *result;

   result = _JsonMemCreateTypeArray(c2Count, JsonStr);
   returnNullIf(!result);

   _JsonMemCopyTypeArray(c2Count, JsonStr, result, c2);

   return result;
}
