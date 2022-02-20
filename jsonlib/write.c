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
func: _JsonWriteC1
******************************************************************************/
JsonBool _JsonWriteC1(Json const * const json, JsonC1 const * const value)
{
   return json->setBuffer(json->dataRepo, _JsonC1GetCount(value), value);
}

/******************************************************************************
func: _JsonWriteC2
******************************************************************************/
JsonBool _JsonWriteC2(Json const * const json, JsonC2 const * const c2)
{
   JsonN4  c1Count;
   JsonC1 *c1;

   if (!_JsonC2ToC1(_JsonC2GetCount(c2) + 1, c2, &c1Count, &c1))
   {
      returnFalse;
   }

   _JsonWriteC1(json, c1);

   _JsonMemDestroy(c1);

   returnTrue;
}

/******************************************************************************
func: _JsonWriteC2Key
******************************************************************************/
JsonBool _JsonWriteC2Key(Json const * const json, JsonC2 const * const key)
{
   JsonC1 c1Key[256];
   JsonN1 c1Count;

   _JsonMemClearTypeArray(256, JsonC1, c1Key);

   returnFalseIf(!_JsonC2ToC1Key(_JsonC2GetCount(key), key, &c1Count, c1Key));
   return _JsonWriteC1(json, c1Key);
}

/******************************************************************************
func: _JsonWriteI
******************************************************************************/
JsonBool _JsonWriteI(Json * const json, JsonI8 const value)
{
   JsonN8 ntemp;

   ntemp = 0;
   if (value < 0)
   {
      json->setBuffer(json->dataRepo, 1, (JsonN1 *) "-");
      ntemp = -value;
   }
   else
   {
      ntemp =  value;
   }

   return _JsonWriteN(json, ntemp);
}

/******************************************************************************
func: _JsonWriteIndent
******************************************************************************/
JsonBool _JsonWriteIndent(Json * const json)
{
   JsonI4 index;

   forCount (index, json->scope)
   {
      returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\t"));
   }
   returnTrue;
}

/******************************************************************************
func: _JsonWriteN
******************************************************************************/
JsonBool _JsonWriteN(Json * const json, JsonN8 const value)
{
   int    index,
          count,
          digit;
   JsonN8 temp;
   JsonN1 string[32];

   temp = value;
   for (index = 0; ; index++)
   {
      digit = temp % 10;
      temp  = temp / 10;

      string[index] = (JsonN1) ('0' + digit);

      breakIf(temp == 0);
   }

   count = index + 1;
   forCountDown (index, count)
   {
      returnFalseIf(!json->setBuffer(json->dataRepo, 1, &string[index]));
   }

   returnTrue;
}

/******************************************************************************
func: _JsonWriteR4S
******************************************************************************/
JsonBool _JsonWriteR4(Json * const json, JsonR4 const value)
{
   JsonC1 ctemp[80];

   sprintf_s((char *) ctemp, 80, "%.6g", value);
   
   return _JsonWriteC1(json, ctemp);
}

/******************************************************************************
func: _JsonWriteR8S
******************************************************************************/
JsonBool _JsonWriteR8(Json * const json, JsonR8 const value)
{
   JsonC1 ctemp[80];

   sprintf_s((char *) ctemp, 80, "%.15g", value);

   return _JsonWriteC1(json, ctemp);
}

/******************************************************************************
func: _JsonWriteStringC2
******************************************************************************/
JsonBool _JsonWriteStringC2(Json * const json, JsonC2 const * const value)
{
   JsonI4  index;
   JsonN4  valueC1Count;
   JsonC1 *valueC1,
           letter[2];
   
   letter[1] = 0;

   returnFalseIf(!_JsonC2ToC1(_JsonC2GetCount(value), value, &valueC1Count, &valueC1));

   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_QUOTE_STR));
   
   forCount(index, valueC1Count)
   {
      if      (valueC1[index] == '\"')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_QUOTE_STR));
      }
      else if (valueC1[index] == '\\')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_SLASH_STR));
      }
      else if (valueC1[index] == '/')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_BACKSLASH_STR));
      }
      else if (valueC1[index] == '\b')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_BACKSPACE_STR));
      }
      else if (valueC1[index] == '\f')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_FORMFEED_STR));
      }
      else if (valueC1[index] == '\n')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_LINEFEED_STR));
      }
      else if (valueC1[index] == '\r')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR));
      }
      else if (valueC1[index] == '\t')
      {
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_ESCAPE_TAB_STR));
      }
      else
      {
         letter[0] = valueC1[index];
         returnFalseIf(!_JsonWriteC1(json, letter));
      }
   }

   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSTRING_QUOTE_STR));

   returnTrue;
}
