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
func: _MiffC1EncodedToC1

Unescape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
MiffBool _MiffC1EncodedToC1(MiffN4 * const c1Count, MiffC1 * const c1)
{
   MiffN4    index,
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
func: _MiffC1ToC1Encoded

Escape certain characters

\n - new line
\t - tab
\\ - \
******************************************************************************/
MiffBool _MiffC1ToC1Encoded(MiffN4 const c1Count, MiffC1 const * const c1, 
   MiffN4 * const c1eCount, MiffC1 ** const c1e)
{
   MiffN4    index,
             eindex,
             charCount,
             eCount;
   MiffC1   *etemp;

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
   etemp  = _MiffMemCreateTypeArray(eCount, MiffC1);
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
func: _MiffC1ToC2

Size of C2 should be the same count as C1 even though it may need less.
******************************************************************************/
void _MiffC1ToC2(MiffN4 const c1Count, MiffC1 const * const c1, MiffN4 * const c2Count,
   MiffC2 * const c2)
{
   MiffN4 c1Index,
          c2Index,
          c1Consumed,
          c2Required;
   MiffC4 c4;

   c2Index = 0;
   forCount(c1Index, c1Count)
   {
      c1Consumed = _MiffC1LetterToC4Letter(&c1[c1Index], &c4);
      c2Required = _MiffC4LetterToC2Letter(c4, &c2[c2Index], &c2[c2Index + 1]);

      c1Index += c1Consumed - 1;
      c2Index += c2Required;
   }
   *c2Count = c2Index;
}

/******************************************************************************
func: _MiffC1ToC2Key
******************************************************************************/
void _MiffC1ToC2Key(MiffN4 const c1Count, MiffC1 const * const c1, MiffN1 * const c2Count,
   MiffC2 * const c2)
{
   MiffN4 c1Index,
          c2Index,
          c1Consumed,
          c2Required;
   MiffC4 c4;

   c2Index = 0;
   forCount(c1Index, min(255, c1Count))
   {    
      c1Consumed = _MiffC1LetterToC4Letter(&c1[c1Index], &c4);
      c2Required = _MiffC4LetterToC2Letter(c4, &c2[c2Index], &c2[c2Index + 1]);

      c1Index += c1Consumed - 1;
      c2Index += c2Required;
   }
   *c2Count = (MiffN1) c2Index;
}

/******************************************************************************
func: _MiffC1ToI
******************************************************************************/
MiffI8 _MiffC1ToI(MiffN4 const c1Count, MiffC1 const * const c1)
{
   MiffBool isNegative;
   MiffN4   c1Index;
   MiffN8   value;

   isNegative = miffBoolFALSE;
   value      = 0;
   forCount(c1Index, c1Count)
   {
      if (c1Index == 0)
      {
         if (c1[c1Index] == '-')
         {
            isNegative = miffBoolTRUE;
            continue;
         }
      }

      value = value * 10 + c1[c1Index] - '0';
   }

   if (isNegative)
   {
      return -((MiffI8) value);
   }

   return value;
}

/******************************************************************************
func: _MiffC1ToN
******************************************************************************/
MiffN8 _MiffC1ToN(MiffN4 const c1Count, MiffC1 const * const c1)
{
   MiffN4   c1Index;
   MiffN8   value;

   value = 0;
   forCount(c1Index, c1Count)
   {
      value = value * 10 + c1[c1Index] - '0';
   }

   return value;
}

/******************************************************************************
func: _MiffC2Append
******************************************************************************/
MiffC2 *_MiffC2Append(MiffC2 const * const a, MiffC2 const * const b, MiffC2 const * const c)
{
   MiffN4    length,
             offset;
   MiffC2   *result;

   length = _MiffC2GetCount(a) + _MiffC2GetCount(b) + _MiffC2GetCount(c) + 1;

   result = _MiffMemCreateTypeArray(length, MiffC2);
   returnNullIf(!result);

   offset = 0;
   length = _MiffC2GetCount(a);
   _MiffMemCopyTypeArray(length, MiffC2, &result[offset], a);
   
   offset += length;
   length  = _MiffC2GetCount(b);
   _MiffMemCopyTypeArray(length, MiffC2, &result[offset], b);
   
   offset += length;
   length  = _MiffC2GetCount(c);
   _MiffMemCopyTypeArray(length, MiffC2, &result[offset], c);

   return result;
}

/******************************************************************************
func: _MiffC2Clone
******************************************************************************/
MiffC2 *_MiffC2Clone(MiffN4 const c2Count, MiffC2 const * const c2)
{
   MiffC2 *result;

   result = _MiffMemCreateTypeArray(c2Count, MiffC2);
   returnNullIf(!result);

   _MiffMemCopyTypeArray(c2Count, MiffC2, result, c2);

   return result;
}

/******************************************************************************
func: _MiffC2ToC1
******************************************************************************/
MiffBool _MiffC2ToC1(MiffN4 const c2Count, MiffC2 const * const c2, MiffN4 * const c1Count,
   MiffC1 ** const c1)
{
   MiffN4    c1Index,
             c2Index,
             c1CountMax,
             c1Required,
             c2Consumed;
   MiffC1   *c1Result;
   MiffC4    c4;

   *c1Count = 0;
   *c1      = NULL;

   c1CountMax = c2Count * 2 + 1;
   c1Result   = _MiffMemCreateTypeArray(c1CountMax, MiffC1);
   returnFalseIf(!c1Result);

   c1Index = 0;
   forCount(c2Index, c2Count)
   {
      c2Consumed = _MiffC2LetterToC4Letter(&c2[c2Index], &c4);
      c1Required = _MiffC4LetterToC1Letter(
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
func: _MiffC2ToC1Key
******************************************************************************/
MiffBool _MiffC2ToC1Key(MiffN4 const c2Count, MiffC2 const * const c2, MiffN1 * const c1Count,
   MiffC1 * const c1)
{
   MiffN4    c1Index,
             c2Index,
             c1Required,
             c2Consumed;
   MiffC1    a,
             b,
             c, 
             d;
   MiffC4    c4;

   *c1Count = 0;
   c1[0]    = 0;

   c1Index = 0;
   forCount(c2Index, c2Count)
   {
      c2Consumed = _MiffC2LetterToC4Letter(&c2[c2Index], &c4);
      c1Required = _MiffC4LetterToC1Letter(c4, &a, &b, &c, &d);

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

   *c1Count = (MiffN1) c1Index;

   returnTrue;
}

/******************************************************************************
func: _MiffC1LetterToC4Letter
******************************************************************************/
MiffN4 _MiffC1LetterToC4Letter(MiffC1 const * const c1, MiffC4 * const c4)
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
          (((MiffC4) c1[0]) & 0x3f)        +
         ((((MiffC4) c1[1]) & 0x1f) << 6);

      return 2;
   }

   // three byte.
   if ((c1[0] & 0xf0) == 0xe0)
   {
      *c4 =
          (((MiffC4) c1[0]) & 0x3f)        +
         ((((MiffC4) c1[1]) & 0x3f) <<  6) +
         ((((MiffC4) c1[2]) & 0x0f) << 12);

      return 3;
   }

   // four byte.
   *c4 =
       (((MiffC4) c1[0]) & 0x3f)        +
      ((((MiffC4) c1[1]) & 0x3f) <<  6) +
      ((((MiffC4) c1[2]) & 0x3f) << 12) +
      ((((MiffC4) c1[3]) & 0x07) << 18);

   return 4;
}

/******************************************************************************
func: _MiffC2LetterToC4Letter
******************************************************************************/
MiffN4 _MiffC2LetterToC4Letter(MiffC2 const * const c2, MiffC4 * const c4)
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
func: _MiffC4LetterToC1Letter
******************************************************************************/
MiffN4 _MiffC4LetterToC1Letter(MiffN4 const c4, MiffN1 * const a, MiffN1 * const b,
   MiffN1 * const c, MiffN1 * const d)
{
   return0If(!a || !b || !c || !d);

   if (c4 < 0x0000007f)
   {
      *a = (MiffN1) c4;

      return 1;
   }

   if (c4 < 0x000007ff)
   {
      *b = (MiffN1) (0x80 |  (c4       & 0x3f));
      *a = (MiffN1) (0xc0 | ((c4 >> 6) & 0x1f));

      return 2;
   }

   if (c4 < 0x0000ffff)
   {
      *c = (MiffN1) (0x80 |  (c4        & 0x3f));
      *b = (MiffN1) (0x80 | ((c4 >>  6) & 0x3f));
      *a = (MiffN1) (0xe0 | ((c4 >> 12) & 0x0f));

      return 3;
   }

   // c4 < 0x001fffff
   *d = (MiffN1) (0x80 |  (c4        & 0x3f));
   *c = (MiffN1) (0x80 | ((c4 >>  6) & 0x3f));
   *b = (MiffN1) (0x80 | ((c4 >> 12) & 0x3f));
   *c = (MiffN1) (0xf0 | ((c4 >> 18) & 0x07));

   return 4;
}

/******************************************************************************
func: _MiffC4LetterToC2Letter
******************************************************************************/
MiffN4 _MiffC4LetterToC2Letter(MiffC4 const c4, MiffC2 * const a, MiffC2 * const b)
{
   return0If(!a || !b);

   if (c4 < 0x0000FFFF)
   {
      *b = 0;
      *a = (MiffC2) c4;

      return 1;
   }

   *a = (MiffC2) (
      0xD800                                 +
      ((((c4 >> 16) & 0x1F) - 1) << 6) +
       (((c4 >> 10) & 0x3F)));
   *b = (MiffC2) (
      0xDC00 +  
      (c4 & 0x000003FF));

   return 2;
}
