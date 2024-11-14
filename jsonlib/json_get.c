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
#include "json_local.h"

/******************************************************************************
local:
variable:
******************************************************************************/

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _JsonEatSpace
******************************************************************************/
JsonB _JsonEatSpace(Json * const json)
{
   returnTrueIf(!_JsonIsSpace(json))

   loop
   {
      returnFalseIf(!_JsonGetChar(json));

      returnTrueIf(!_JsonIsSpace(json));
   }
}

/******************************************************************************
func: _JsonGetChar
******************************************************************************/
JsonB _JsonGetChar(Json * const json)
{
   returnFalseIf(!json->getBuffer(json->dataRepo, 1, &json->lastByte));
   return jsonTRUE;
}

/******************************************************************************
func: _JsonGetFalse
******************************************************************************/
JsonType _JsonGetFalse(Json * const json)
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

      return jsonTypeCONSTANT_FALSE;
   }

   return jsonTypeERROR_CONSTANT_FALSE_EXPECTED;
}

/******************************************************************************
func: _JsonGetNull
******************************************************************************/
JsonType _JsonGetNull(Json * const json)
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

      return jsonTypeCONSTANT_NULL;
   }

   return jsonTypeERROR_CONSTANT_NULL_EXPECTED;
}

/******************************************************************************
func: _JsonGetNumber
******************************************************************************/
JsonType _JsonGetNumber(Json * const json)
{
   JsonB   isIntegerNumberFound,
           isFractionalNumberFound,
           isExponentNumberFound;
   // Current doulbe maxes out a 10^+/-308, 400 characters is large enough.
   JsonStr numberStr[400];
   int     index;

   // Reset the number string.
   forCount(index, 400)
   {
      numberStr[index] = 0;
   }

   // Read in the integer part.
   isIntegerNumberFound = jsonFALSE;
   numberStr[0]        = json->lastByte;
   if ('0' <= json->lastByte && json->lastByte <= '9')
   {
      isIntegerNumberFound = jsonTRUE;
   }

   for (index = 1; ; index++)
   {
      // End of file.
      if (!_JsonGetChar(json))
      {
         returnIf(isIntegerNumberFound, _JsonGetNumberInteger(json, numberStr));
         return jsonTypeERROR_NUMBER_EXPECTED;
      }

      // Number is a Real
      if (json->lastByte == '.')
      {
         gotoIf(isIntegerNumberFound, GET_FRACTION);
         return jsonTypeERROR_NUMBER_EXPECTED;
      }
      // Number is Real with an exponent
      if (json->lastByte == 'e' ||
          json->lastByte == 'E')
      {
         gotoIf(isIntegerNumberFound, GET_EXPONENT);
         return jsonTypeERROR_NUMBER_EXPECTED;
      }
         
      // Add to the integer portion.
      if ('0' <= json->lastByte && json->lastByte <= '9')
      {
         numberStr[index]     = (JsonStr) json->lastByte;
         isIntegerNumberFound = jsonTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         returnIf(isIntegerNumberFound, _JsonGetNumberInteger(json, numberStr));
         return jsonTypeERROR_NUMBER_EXPECTED;
      }
   }

GET_FRACTION:
   // Decimal was found.
   isFractionalNumberFound = jsonFALSE;
   numberStr[index++]      = '.';
   for (;; index++)
   {
      // End of file
      if (!_JsonGetChar(json))
      {
         returnIf(isFractionalNumberFound, _JsonGetNumberReal(json, numberStr));
         return jsonTypeERROR_NUMBER_REAL_EXPECTED;
      }

      // Exponent defined
      if (json->lastByte == 'e' ||
          json->lastByte == 'E')
      {
         gotoIf(isFractionalNumberFound, GET_EXPONENT);
         return jsonTypeERROR_NUMBER_REAL_EXPECTED;
      }

      // Add to the fractional portion.
      if ('0' <= json->lastByte && json->lastByte <= '9')
      {
         numberStr[index]        = (JsonStr) json->lastByte;
         isFractionalNumberFound = jsonTRUE;
      }
      // Assuming this is the end of the number.
      else
      {
         returnIf(isFractionalNumberFound, _JsonGetNumberReal(json, numberStr));
         return jsonTypeERROR_NUMBER_REAL_EXPECTED;
      }
   }

GET_EXPONENT:
   // Exponent was found
   isExponentNumberFound = jsonFALSE;
   numberStr[index++]    = 'e';

   // End of file in the middle of a number!
   returnIf(!_JsonGetChar(json), jsonTypeERROR_NUMBER_REAL_EXPECTED);

   // Get the sign.
   if      ('0' <= json->lastByte && json->lastByte <= '9')
   {
      numberStr[index++]    = (JsonStr) json->lastByte;
      isExponentNumberFound = jsonTRUE;
   }
   // Negative exponent.
   else if (json->lastByte == '-')
   {
      numberStr[index++] = (JsonStr) json->lastByte;
   }
   // Wasn't a sign or a digit after the 'e'.
   else if (json->lastByte != '+')
   {
      return jsonTypeERROR_NUMBER_REAL_EXPECTED;
   }
   // If it was a "+" sign we just eat it.  Unnecessary.

   // Get the exponent
   for (;; index++)
   {
      if (!_JsonGetChar(json))
      {
         returnIf(isExponentNumberFound, _JsonGetNumberReal(json, numberStr));
         return jsonTypeERROR_NUMBER_REAL_EXPECTED;
      }

      if ('0' <= json->lastByte && json->lastByte <= '9')
      {
         numberStr[index]      = (JsonStr) json->lastByte;
         isExponentNumberFound = jsonTRUE;
      }
      else
      {
         break;
      }
   }

   returnIf(isExponentNumberFound, _JsonGetNumberReal(json, numberStr));
   return jsonTypeERROR_NUMBER_REAL_EXPECTED;
}

/******************************************************************************
func: _JsonGetNumberInteger
******************************************************************************/
JsonType _JsonGetNumberInteger(Json * const json, JsonStr * const numberStr)
{
   JsonType type;

   // Default number type.
   type = jsonTypeNUMBER_INTEGER;

   // This is a negative number.  Definitely not a natural number.
   if (numberStr[0] == '-')
   {
      json->value.n = _JsonStrToN(&numberStr[1]);
      if      (json->value.n == ((JsonN) JsonI_MAX) + 1)
      {
         json->value.i = JsonI_MIN;
      }
      else if (json->value.n <= JsonI_MAX)
      {
         json->value.i = - (JsonI) json->value.n;
      }

      json->value.n  = 0;
      json->value.r  = (JsonR)  json->value.i;
      json->value.r4 = (JsonR4) json->value.i;

      json->value.type = type;
      return type;
   }

   json->value.n = _JsonStrToN(numberStr);
   // This unsigned integer is small enough to be a signed integer.
   if (json->value.n <= JsonI_MAX)
   {
      json->value.i = json->value.n;
   }
   // Special case.  Not all unsigned integers can be represented as a signed integer.
   else
   {
      json->value.i = 0;
      type          = jsonTypeNUMBER_NATURAL;
   }

   json->value.r  = (JsonR)  json->value.n;
   json->value.r4 = (JsonR4) json->value.n;

   // Set the type of the value.
   json->value.type = type;
   return type;
}

/******************************************************************************
func: _JsonGetNumberReal
******************************************************************************/
JsonType _JsonGetNumberReal(Json * const json, JsonStr * const str)
{
   json->value.i    = 0;
   json->value.n    = 0;
   json->value.r    = _atof_l(str, _JsonLocaleGet());
   json->value.r4   = (float) json->value.r;
   json->value.type = jsonTypeNUMBER_REAL;

   return jsonTypeNUMBER_REAL;
}

/******************************************************************************
func: _JsonGetTrue
******************************************************************************/
JsonType _JsonGetTrue(Json * const json)
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

      return jsonTypeCONSTANT_TRUE;
   }

   return jsonTypeERROR_CONSTANT_TRUE_EXPECTED;
}

/******************************************************************************
func: _JsonIsSpace
******************************************************************************/
JsonB _JsonIsSpace(Json * const json)
{
   returnTrueIf(
      json->lastByte == 0    ||
      json->lastByte == 0x09 ||
      json->lastByte == 0x0A ||
      json->lastByte == 0x0D ||
      json->lastByte == 0x20);

   return jsonFALSE;
}
