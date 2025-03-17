/**************************************************************************************************
file:       gjson_get
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Get functions local to the library.
**************************************************************************************************/

/**************************************************************************************************
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
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "gjson_local.h"

/**************************************************************************************************
local:
variable:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _JsonEatSpace
**************************************************************************************************/
Gb _JsonEatSpace(Gjson * const json)
{
   returnTrueIf(!_JsonIsSpace(json))

   loop
   {
      returnFalseIf(!_JsonGetChar(json));

      returnTrueIf(!_JsonIsSpace(json));
   }
}

/**************************************************************************************************
func: _JsonGetChar
**************************************************************************************************/
Gb _JsonGetChar(Gjson * const json)
{
   returnFalseIf(!json->getBuffer(json->dataRepo, 1, &json->lastByte));
   return gbTRUE;
}

/**************************************************************************************************
func: _JsonGetFalse
**************************************************************************************************/
GjsonType _JsonGetFalse(Gjson * const json)
{
   loop
   {
      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'a');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'l');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 's');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'e');

      json->lastByte = 0;

      return gjsonTypeVALUE_FALSE;
   }

   return gjsonTypeERROR_CONSTANT_FALSE_EXPECTED;
}

/**************************************************************************************************
func: _JsonGetNull
**************************************************************************************************/
GjsonType _JsonGetNull(Gjson * const json)
{
   loop
   {
      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'u');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'l');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'l');

      json->lastByte = 0;

      return gjsonTypeVALUE_NULL;
   }

   return gjsonTypeERROR_CONSTANT_NULL_EXPECTED;
}

/**************************************************************************************************
func: _JsonGetNumber
**************************************************************************************************/
GjsonType _JsonGetNumber(Gjson * const json)
{
   Gb   isIntegerNumberFound,
           isFractionalNumberFound,
           isExponentNumberFound;
   // Current doulbe maxes out a 10^+/-308, 400 characters is large enough.
   Gstr numberStr[400];
   int     index;

   // Reset the number string.
   forCount(index, 400)
   {
      numberStr[index] = 0;
   }

   // Read in the integer part.
   isIntegerNumberFound = gbFALSE;
   numberStr[0]        = json->lastByte;
   if ('0' <= json->lastByte && json->lastByte <= '9')
   {
      isIntegerNumberFound = gbTRUE;
   }

   for (index = 1; ; index++)
   {
      // End of file.
      if (!_JsonGetChar(json))
      {
         returnIf(isIntegerNumberFound, _JsonGetNumberInteger(json, numberStr));
         return gjsonTypeERROR_NUMBER_EXPECTED;
      }

      // Number is a Real
      if (json->lastByte == '.')
      {
         gotoIf(isIntegerNumberFound, GET_FRACTION);
         return gjsonTypeERROR_NUMBER_EXPECTED;
      }
      // Number is Real with an exponent
      if (json->lastByte == 'e' ||
          json->lastByte == 'E')
      {
         gotoIf(isIntegerNumberFound, GET_EXPONENT);
         return gjsonTypeERROR_NUMBER_EXPECTED;
      }

      // Add to the integer portion.
      if ('0' <= json->lastByte && json->lastByte <= '9')
      {
         numberStr[index]     = (Gstr) json->lastByte;
         isIntegerNumberFound = gbTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         returnIf(isIntegerNumberFound, _JsonGetNumberInteger(json, numberStr));
         return gjsonTypeERROR_NUMBER_EXPECTED;
      }
   }

GET_FRACTION:
   // Decimal was found.
   isFractionalNumberFound = gbFALSE;
   numberStr[index++]      = '.';
   for (;; index++)
   {
      // End of file
      if (!_JsonGetChar(json))
      {
         returnIf(isFractionalNumberFound, _JsonGetNumberReal(json, numberStr));
         return gjsonTypeERROR_NUMBER_REAL_EXPECTED;
      }

      // Exponent defined
      if (json->lastByte == 'e' ||
          json->lastByte == 'E')
      {
         gotoIf(isFractionalNumberFound, GET_EXPONENT);
         return gjsonTypeERROR_NUMBER_REAL_EXPECTED;
      }

      // Add to the fractional portion.
      if ('0' <= json->lastByte && json->lastByte <= '9')
      {
         numberStr[index]        = (Gstr) json->lastByte;
         isFractionalNumberFound = gbTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         returnIf(isFractionalNumberFound, _JsonGetNumberReal(json, numberStr));
         return gjsonTypeERROR_NUMBER_REAL_EXPECTED;
      }
   }

GET_EXPONENT:
   // Exponent was found
   isExponentNumberFound = gbFALSE;
   numberStr[index++]    = 'e';

   // End of file in the middle of a number!
   returnIf(!_JsonGetChar(json), gjsonTypeERROR_NUMBER_REAL_EXPECTED);

   // Get the sign.
   if      ('0' <= json->lastByte && json->lastByte <= '9')
   {
      numberStr[index++]    = (Gstr) json->lastByte;
      isExponentNumberFound = gbTRUE;
   }
   // Negative exponent.
   else if (json->lastByte == '-')
   {
      numberStr[index++] = (Gstr) json->lastByte;
   }
   // Wasn't a sign or a digit after the 'e'.
   else if (json->lastByte != '+')
   {
      return gjsonTypeERROR_NUMBER_REAL_EXPECTED;
   }
   // If it was a "+" sign we just eat it.  Unnecessary.

   // Get the exponent
   for (;; index++)
   {
      if (!_JsonGetChar(json))
      {
         returnIf(isExponentNumberFound, _JsonGetNumberReal(json, numberStr));
         return gjsonTypeERROR_NUMBER_REAL_EXPECTED;
      }

      if ('0' <= json->lastByte && json->lastByte <= '9')
      {
         numberStr[index]      = (Gstr) json->lastByte;
         isExponentNumberFound = gbTRUE;
      }
      else
      {
         break;
      }
   }

   returnIf(isExponentNumberFound, _JsonGetNumberReal(json, numberStr));
   return gjsonTypeERROR_NUMBER_REAL_EXPECTED;
}

/**************************************************************************************************
func: _JsonGetNumberInteger
**************************************************************************************************/
GjsonType _JsonGetNumberInteger(Gjson * const json, Gstr * const numberStr)
{
   GjsonType type;

   // Default number type.
   type = gjsonTypeVALUE_NUMBER_INTEGER;

   // This is a negative number.  Definitely not a natural number.
   if (numberStr[0] == '-')
   {
      json->value.n = _JsonStrToN(&numberStr[1]);
      if      (json->value.n == ((Gn8) Gi8MAX) + 1)
      {
         json->value.i = Gi8MIN;
      }
      else if (json->value.n <= Gi8MAX)
      {
         json->value.i = - (Gi8) json->value.n;
      }

      json->value.n  = 0;
      json->value.r  = (Gr8)  json->value.i;
      json->value.r4 = (Gr4) json->value.i;

      json->value.type = type;
      return type;
   }

   json->value.n = _JsonStrToN(numberStr);
   // This unsigned integer is small enough to be a signed integer.
   if (json->value.n <= Gi8MAX)
   {
      json->value.i = json->value.n;
   }
   // Special case.  Not all unsigned integers can be represented as a signed integer.
   else
   {
      json->value.i = 0;
      type          = gjsonTypeVALUE_NUMBER_NATURAL;
   }

   json->value.r  = (Gr8)  json->value.n;
   json->value.r4 = (Gr4) json->value.n;

   // Set the type of the value.
   json->value.type = type;
   return type;
}

/**************************************************************************************************
func: _JsonGetNumberReal
**************************************************************************************************/
GjsonType _JsonGetNumberReal(Gjson * const json, Gstr * const str)
{
   json->value.i    = 0;
   json->value.n    = 0;
   json->value.r    = _atof_l(str, _JsonLocaleGet());
   json->value.r4   = (float) json->value.r;
   json->value.type = gjsonTypeVALUE_NUMBER_REAL;

   return gjsonTypeVALUE_NUMBER_REAL;
}

/**************************************************************************************************
func: _JsonGetTrue
**************************************************************************************************/
GjsonType _JsonGetTrue(Gjson * const json)
{
   loop
   {
      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'r');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'u');

      breakIf(!_JsonGetChar(json));
      breakIf(json->lastByte != 'e');

      json->lastByte = 0;

      return gjsonTypeVALUE_TRUE;
   }

   return gjsonTypeERROR_CONSTANT_TRUE_EXPECTED;
}

/**************************************************************************************************
func: _JsonIsSpace
**************************************************************************************************/
Gb _JsonIsSpace(Gjson * const json)
{
   returnTrueIf(
      json->lastByte == 0    ||
      json->lastByte == 0x09 ||
      json->lastByte == 0x0A ||
      json->lastByte == 0x0D ||
      json->lastByte == 0x20);

   return gbFALSE;
}
