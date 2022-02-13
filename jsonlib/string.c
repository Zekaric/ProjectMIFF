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
func: _JsonC1EncodedToC1

Unescape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
JsonBool _JsonC1EncodedToC1(JsonN4 * const c1Count, JsonC1 * const c1)
{
   JsonN4    index,
             eindex,
             charCount;

   // Count the number of characters that need escaping.
   charCount = 0;
   index     = 0;
   forCount (eindex, *c1Count)
   {
      if (c1[eindex] == '\\' &&
          eindex + 1 <  *c1Count)
      {
         // Unescaping a \ character
         if      (c1[eindex + 1] == '\\')
         {
            eindex++;
         }
         // Unescaping a tab character
         else if (c1[eindex + 1] == 't')
         {
            c1[eindex + 1] = '\t';
            eindex++;
         }
         // Unescaping a new line character
         else if (c1[eindex + 1] == 'n')
         {
            c1[eindex + 1] = '\n';
            eindex++;
         }
      }

      // adjust the string
      if (index != eindex)
      {
         c1[index] = c1[eindex];
      }
      index++;
   }

   // Update the string length.
   *c1Count = index;

   returnTrue;
}

/******************************************************************************
func: _JsonC1ToC1Encoded

Escape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
JsonBool _JsonC1ToC1Encoded(JsonN4 const c1Count, JsonC1 const * const c1, 
   JsonN4 * const c1eCount, JsonC1 ** const c1e)
{
   JsonN4    index,
             eindex,
             charCount,
             eCount;
   JsonC1   *etemp;

   *c1eCount = 0;
   *c1e      = NULL;

   // Count the number of characters that need escaping.
   charCount = 0;
   forCount (index, c1Count)
   {
      if (c1[index] == 0x09 || // tab
          c1[index] == 0x0a || // new line
          c1[index] == '\\')
      {
         charCount++;
      }
   }

   eCount = c1Count + charCount + 1;
   etemp  = _JsonMemCreateTypeArray(eCount, JsonC1);
   returnFalseIf(!etemp);

   eindex = c1Count + charCount - 1;
   forCountDown (index, c1Count)
   {
      switch (c1[index])
      {
      case 0x09: etemp[eindex--] = 't';  etemp[eindex--] = '\\'; break;
      case 0x0a: etemp[eindex--] = 'n';  etemp[eindex--] = '\\'; break;
      case '\\': etemp[eindex--] = '\\'; etemp[eindex--] = '\\'; break;
      default:   etemp[eindex--] = c1[index];                    break;
      }

      breakIf(index == 0);
   }

   *c1eCount = c1Count + charCount;
   *c1e      = etemp;

   returnTrue;
}

/******************************************************************************
func: _JsonC1ToC2

Size of C2 should be the same count as C1 even though it may need less.
******************************************************************************/
void _JsonC1ToC2(JsonN4 const c1Count, JsonC1 const * const c1, JsonN4 * const c2Count,
   JsonC2 * const c2)
{
   JsonN4 c1Index,
          c2Index,
          c1Consumed,
          c2Required;
   JsonC4 c4;

   c2Index = 0;
   forCount(c1Index, c1Count)
   {
      c1Consumed = _JsonC1LetterToC4Letter(&c1[c1Index], &c4);
      c2Required = _JsonC4LetterToC2Letter(c4, &c2[c2Index], &c2[c2Index + 1]);

      c1Index += c1Consumed - 1;
      c2Index += c2Required;
   }
   *c2Count = c2Index;
}

/******************************************************************************
func: _JsonC1ToC2Key
******************************************************************************/
void _JsonC1ToC2Key(JsonN4 const c1Count, JsonC1 const * const c1, JsonN1 * const c2Count,
   JsonC2 * const c2)
{
   JsonN4 c1Index,
          c2Index,
          c1Consumed,
          c2Required;
   JsonC4 c4;

   c2Index = 0;
   forCount(c1Index, min(255, c1Count))
   {    
      c1Consumed = _JsonC1LetterToC4Letter(&c1[c1Index], &c4);
      c2Required = _JsonC4LetterToC2Letter(c4, &c2[c2Index], &c2[c2Index + 1]);

      c1Index += c1Consumed - 1;
      c2Index += c2Required;
   }
   *c2Count = (JsonN1) c2Index;
}

/******************************************************************************
func: _JsonC1ToI
******************************************************************************/
JsonI8 _JsonC1ToI(JsonN4 const c1Count, JsonC1 const * const c1)
{
   JsonBool isNegative;
   JsonN4   c1Index;
   JsonN8   value;

   isNegative = jsonBoolFALSE;
   value      = 0;
   forCount(c1Index, c1Count)
   {
      if (c1Index == 0)
      {
         if (c1[c1Index] == '-')
         {
            isNegative = jsonBoolTRUE;
            continue;
         }
      }

      value = value * 10 + c1[c1Index] - '0';
   }

   if (isNegative)
   {
      return -((JsonI8) value);
   }

   return value;
}

/******************************************************************************
func: _JsonC1ToN
******************************************************************************/
JsonN8 _JsonC1ToN(JsonN4 const c1Count, JsonC1 const * const c1)
{
   JsonN4   c1Index;
   JsonN8   value;

   value = 0;
   forCount(c1Index, c1Count)
   {
      value = value * 10 + c1[c1Index] - '0';
   }

   return value;
}

/******************************************************************************
func: _JsonC2Append
******************************************************************************/
JsonC2 *_JsonC2Append(JsonC2 const * const a, JsonC2 const * const b, JsonC2 const * const c)
{
   JsonN4    length,
             offset;
   JsonC2   *result;

   length = _JsonC2GetCount(a) + _JsonC2GetCount(b) + _JsonC2GetCount(c) + 1;

   result = _JsonMemCreateTypeArray(length, JsonC2);
   returnNullIf(!result);

   offset = 0;
   length = _JsonC2GetCount(a);
   _JsonMemCopyTypeArray(length, JsonC2, &result[offset], a);
   
   offset += length;
   length  = _JsonC2GetCount(b);
   _JsonMemCopyTypeArray(length, JsonC2, &result[offset], b);
   
   offset += length;
   length  = _JsonC2GetCount(c);
   _JsonMemCopyTypeArray(length, JsonC2, &result[offset], c);

   return result;
}

/******************************************************************************
func: _JsonC2Clone
******************************************************************************/
JsonC2 *_JsonC2Clone(JsonN4 const c2Count, JsonC2 const * const c2)
{
   JsonC2 *result;

   result = _JsonMemCreateTypeArray(c2Count, JsonC2);
   returnNullIf(!result);

   _JsonMemCopyTypeArray(c2Count, JsonC2, result, c2);

   return result;
}

/******************************************************************************
func: _JsonC2ToC1
******************************************************************************/
JsonBool _JsonC2ToC1(JsonN4 const c2Count, JsonC2 const * const c2, JsonN4 * const c1Count,
   JsonC1 ** const c1)
{
   JsonN4    c1Index,
             c2Index,
             c1CountMax,
             c1Required,
             c2Consumed;
   JsonC1   *c1Result;
   JsonC4    c4;

   *c1Count = 0;
   *c1      = NULL;

   c1CountMax = c2Count * 2 + 1;
   c1Result   = _JsonMemCreateTypeArray(c1CountMax, JsonC1);
   returnFalseIf(!c1Result);

   c1Index = 0;
   forCount(c2Index, c2Count)
   {
      c2Consumed = _JsonC2LetterToC4Letter(&c2[c2Index], &c4);
      c1Required = _JsonC4LetterToC1Letter(
         c4, 
         &c1Result[c1Index + 0], 
         &c1Result[c1Index + 1],
         &c1Result[c1Index + 2],
         &c1Result[c1Index + 3]);

      c2Index += c2Consumed - 1;
      c1Index += c1Required;
   }

   *c1Count = c1Index;
   *c1      = c1Result;

   returnTrue;
}

/******************************************************************************
func: _JsonC2ToC1Key
******************************************************************************/
JsonBool _JsonC2ToC1Key(JsonN4 const c2Count, JsonC2 const * const c2, JsonN1 * const c1Count,
   JsonC1 * const c1)
{
   JsonN4    c1Index,
             c2Index,
             c1Required,
             c2Consumed;
   JsonC1    a,
             b,
             c, 
             d;
   JsonC4    c4;

   *c1Count = 0;
   c1[0]    = 0;

   c1Index = 0;
   forCount(c2Index, c2Count)
   {
      c2Consumed = _JsonC2LetterToC4Letter(&c2[c2Index], &c4);
      c1Required = _JsonC4LetterToC1Letter(c4, &a, &b, &c, &d);

      if (c1Index + c1Required >= 256)
      {
         returnFalse;
      }

      switch(c1Required)
      {
      case 4:
         c1[c1Index + 3] = d;
         // fall through
      case 3:
         c1[c1Index + 2] = c;
         // fall through
      case 2:
         c1[c1Index + 1] = b;
         // fall through
      case 1:
         c1[c1Index + 0] = a;
         break;
      }

      c2Index += c2Consumed - 1;
      c1Index += c1Required;
   }

   *c1Count = (JsonN1) c1Index;

   returnTrue;
}

/******************************************************************************
func: _JsonC1LetterToC4Letter
******************************************************************************/
JsonN4 _JsonC1LetterToC4Letter(JsonC1 const * const c1, JsonC4 * const c4)
{
   returnIf(!c1, 0);

   // one byte.
   if ((c1[0] & 0x80) == 0)
   {
      *c4 = c1[0];
      return 1;
   }
   
   // two byte.
   if ((c1[0] & 0xe0) == 0xc0)
   {
      *c4 =
          (((JsonC4) c1[0]) & 0x3f)        +
         ((((JsonC4) c1[1]) & 0x1f) << 6);

      return 2;
   }

   // three byte.
   if ((c1[0] & 0xf0) == 0xe0)
   {
      *c4 =
          (((JsonC4) c1[0]) & 0x3f)        +
         ((((JsonC4) c1[1]) & 0x3f) <<  6) +
         ((((JsonC4) c1[2]) & 0x0f) << 12);

      return 3;
   }

   // four byte.
   *c4 =
       (((JsonC4) c1[0]) & 0x3f)        +
      ((((JsonC4) c1[1]) & 0x3f) <<  6) +
      ((((JsonC4) c1[2]) & 0x3f) << 12) +
      ((((JsonC4) c1[3]) & 0x07) << 18);

   return 4;
}

/******************************************************************************
func: _JsonC2LetterToC4Letter
******************************************************************************/
JsonN4 _JsonC2LetterToC4Letter(JsonC2 const * const c2, JsonC4 * const c4)
{
   return0If(!c2);

   // 2 utf16 to form the code point.
   if ((c2[0] & 0xFC00) == 0xD800 &&
       (c2[1] & 0xFC00) == 0xDC00)
   {
      *c4 =
         ((((c2[0] >> 6) & 0x000F) + 1) << 16) +
         ((((c2[0]     ) & 0x003F)    ) << 10) +
           ((c2[1]     ) & 0x03FF);

      return 2;
   }

   *c4 = c2[0];

   return 1;
}

/******************************************************************************
func: _JsonC4LetterToC1Letter
******************************************************************************/
JsonN4 _JsonC4LetterToC1Letter(JsonN4 const c4, JsonN1 * const a, JsonN1 * const b,
   JsonN1 * const c, JsonN1 * const d)
{
   return0If(!a || !b || !c || !d);

   if (c4 < 0x0000007f)
   {
      *a = (JsonN1) c4;

      return 1;
   }

   if (c4 < 0x000007ff)
   {
      *b = (JsonN1) (0x80 |  (c4       & 0x3f));
      *a = (JsonN1) (0xc0 | ((c4 >> 6) & 0x1f));

      return 2;
   }

   if (c4 < 0x0000ffff)
   {
      *c = (JsonN1) (0x80 |  (c4        & 0x3f));
      *b = (JsonN1) (0x80 | ((c4 >>  6) & 0x3f));
      *a = (JsonN1) (0xe0 | ((c4 >> 12) & 0x0f));

      return 3;
   }

   // c4 < 0x001fffff
   *d = (JsonN1) (0x80 |  (c4        & 0x3f));
   *c = (JsonN1) (0x80 | ((c4 >>  6) & 0x3f));
   *b = (JsonN1) (0x80 | ((c4 >> 12) & 0x3f));
   *c = (JsonN1) (0xf0 | ((c4 >> 18) & 0x07));

   return 4;
}

/******************************************************************************
func: _JsonC4LetterToC2Letter
******************************************************************************/
JsonN4 _JsonC4LetterToC2Letter(JsonC4 const c4, JsonC2 * const a, JsonC2 * const b)
{
   return0If(!a || !b);

   if (c4 < 0x0000FFFF)
   {
      *b = 0;
      *a = (JsonC2) c4;

      return 1;
   }

   *a = (JsonC2) (
      0xD800                                 +
      ((((c4 >> 16) & 0x1F) - 1) << 6) +
       (((c4 >> 10) & 0x3F)));
   *b = (JsonC2) (
      0xDC00 +  
      (c4 & 0x000003FF));

   return 2;
}
