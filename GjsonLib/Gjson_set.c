/**************************************************************************************************
file:       gjson_set
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Set functions local to the library.
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
#include <stdio.h>
#include <math.h>

#include "gjson_local.h"

/**************************************************************************************************
global:
function
**************************************************************************************************/
/**************************************************************************************************
func: _JsonSetBuffer
**************************************************************************************************/
Gb _JsonSetBuffer(Gjson const * const json, Gcount const bufCount, Gn1 const * const buf)
{
   assert(bufCount < GcountMAX);
   return json->setBuffer(json->dataRepo, (Gn4) bufCount, buf);
}

/**************************************************************************************************
func: _JsonSetI
**************************************************************************************************/
Gb _JsonSetI(Gjson * const json, Gi8 const value)
{
   Gn8 ntemp;

   ntemp = 0;
   if (value < 0)
   {
      json->setBuffer(json->dataRepo, 1, (Gn1 *) "-");
      ntemp = -value;
   }
   else
   {
      ntemp =  value;
   }

   return _JsonSetN(json, ntemp);
}

/**************************************************************************************************
func: _JsonSetIndent
**************************************************************************************************/
Gb _JsonSetIndent(Gjson * const json)
{
   Gi4 index;

   // No writing indents when in compact mode.
   returnTrueIf(json->method == gmethodWRITE);

   forCount (index, json->scope)
   {
      returnFalseIf(!_JsonSetBuffer(json, 1, (Gn1 *) "\t"));
   }
   returnTrue;
}

/**************************************************************************************************
func: _JsonSetN
**************************************************************************************************/
Gb _JsonSetN(Gjson * const json, Gn8 const value)
{
   int    index,
          count,
          digit;
   Gn8  temp;
   Gn1 string[32];

   temp = value;
   loopCount(index)
   {
      digit = temp % 10;
      temp  = temp / 10;

      string[index] = (Gn1) ('0' + digit);

      breakIf(temp == 0);
   }

   count = index + 1;
   forCountDown (index, count)
   {
      returnFalseIf(!json->setBuffer(json->dataRepo, 1, (Gn1 *) &string[index]));
   }

   returnTrue;
}

/**************************************************************************************************
func: _JsonSetNewLine
**************************************************************************************************/
Gb _JsonSetNewLine(Gjson * const json)
{
   // No writing new lines when in compact mode.
   returnTrueIf(json->method == gmethodWRITE);

   returnFalseIf(!_JsonSetBuffer(json, 1, (Gn1 *) "\n"));

   returnTrue;
}

/**************************************************************************************************
func: _JsonSetR
**************************************************************************************************/
Gb _JsonSetR(Gjson * const json, Gr8 const value)
{
   Gstr ctemp[80];

   if      (value == HUGE_VAL)
   {
      return _JsonSetBuffer(json, 10, (Gn1 *) "\"Infinity\"");
   }
   else if (value == -HUGE_VAL)
   {
      return _JsonSetBuffer(json, 11, (Gn1 *) "\"-Infinity\"");
   }
   else if (isnan(value))
   {
      return _JsonSetBuffer(json, 5, (Gn1 *) "\"NaN\"");
   }

   _sprintf_s_l((char *) ctemp, 80, "%.17g", _JsonLocaleGet(), value);

   return _JsonSetBuffer(json, (Gcount) strlen(ctemp), (Gn1 *) ctemp);
}

/**************************************************************************************************
func: _JsonSetR4
**************************************************************************************************/
Gb _JsonSetR4(Gjson * const json, Gr4 const value)
{
   Gstr ctemp[80];

   if      (value == HUGE_VALF)
   {
      return _JsonSetBuffer(json, 10, (Gn1 *) "\"Infinity\"");
   }
   else if (value == -HUGE_VALF)
   {
      return _JsonSetBuffer(json, 11, (Gn1 *) "\"-Infinity\"");
   }
   else if (isnan(value))
   {
      return _JsonSetBuffer(json, 5, (Gn1 *) "\"NaN\"");
   }

   _sprintf_s_l((char *) ctemp, 80, "%.8g", _JsonLocaleGet(), value);

   return _JsonSetBuffer(json, (Gcount) strlen(ctemp), (Gn1 *) ctemp);
}
