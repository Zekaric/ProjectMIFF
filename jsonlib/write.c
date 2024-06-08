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
func: _JsonWriteStr
******************************************************************************/
JsonBool _JsonWriteStr(Json const * const json, JsonStr const * const value)
{
   return json->setBuffer(json->dataRepo, _JsonStrGetCount(value), value);
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
      returnFalseIf(!_JsonWriteStr(json, (JsonStr *) "\t"));
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
   JsonStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.6g", _JsonLocaleGet(), value);
   
   return _JsonWriteStr(json, ctemp);
}

/******************************************************************************
func: _JsonWriteR8S
******************************************************************************/
JsonBool _JsonWriteR8(Json * const json, JsonR8 const value)
{
   JsonStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.15g", _JsonLocaleGet(), value);

   return _JsonWriteStr(json, ctemp);
}

/******************************************************************************
func: _JsonWriteString
******************************************************************************/
JsonBool _JsonWriteString(Json * const json, JsonN const strLen, JsonStr const * const str)
{
   JsonN    index;
   JsonStr  letter[2];
   
   letter[1] = 0;

   returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_QUOTE_STR));
   
   forCount(index, strLen)
   {
      if      (str[index] == '\"')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_QUOTE_STR));
      }
      else if (str[index] == '\\')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_SLASH_STR));
      }
      else if (str[index] == '/')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_BACKSLASH_STR));
      }
      else if (str[index] == '\b')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_BACKSPACE_STR));
      }
      else if (str[index] == '\f')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_FORMFEED_STR));
      }
      else if (str[index] == '\n')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_LINEFEED_STR));
      }
      else if (str[index] == '\r')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR));
      }
      else if (str[index] == '\t')
      {
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_STR));
         returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_ESCAPE_TAB_STR));
      }
      else
      {
         letter[0] = str[index];
         returnFalseIf(!_JsonWriteStr(json, letter));
      }
   }

   returnFalseIf(!_JsonWriteStr(json, (JsonStr *) jsonSTRING_QUOTE_STR));

   returnTrue;
}
