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
func: _JsonSetBuffer
******************************************************************************/
JsonB _JsonSetBuffer(Json const * const json, JsonN const bufCount, JsonN1 const * const buf)
{
   assert(bufCount < JsonN4_MAX);
   return json->setBuffer(json->dataRepo, (JsonN4) bufCount, buf);
}

/******************************************************************************
func: _JsonSetI
******************************************************************************/
JsonB _JsonSetI(Json * const json, JsonI const value)
{
   JsonN ntemp;

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

   return _JsonSetN(json, ntemp);
}

/******************************************************************************
func: _JsonSetIndent
******************************************************************************/
JsonB _JsonSetIndent(Json * const json)
{
   JsonI4 index;

   // No writing indents when in compact mode.
   returnTrueIf(json->method == jsonMethodWRITING_COMPACT);

   forCount (index, json->scope)
   {
      returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) "\t"));
   }
   returnTrue;
}

/******************************************************************************
func: _JsonSetN
******************************************************************************/
JsonB _JsonSetN(Json * const json, JsonN const value)
{
   int    index,
          count,
          digit;
   JsonN  temp;
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
      returnFalseIf(!json->setBuffer(json->dataRepo, 1, (JsonN1 *) &string[index]));
   }

   returnTrue;
}

/******************************************************************************
func: _JsonSetNewLine
******************************************************************************/
JsonB _JsonSetNewLine(Json * const json)
{
   // No writing new lines when in compact mode.
   returnTrueIf(json->method == jsonMethodWRITING_COMPACT);

   returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) "\n"));

   returnTrue;
}

/******************************************************************************
func: _JsonSetR
******************************************************************************/
JsonB _JsonSetR(Json * const json, JsonR const value)
{
   JsonStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.17g", _JsonLocaleGet(), value);

   return _JsonSetBuffer(json, strlen(ctemp), (JsonN1 *) ctemp);
}

/******************************************************************************
func: _JsonSetR4
******************************************************************************/
JsonB _JsonSetR4(Json * const json, JsonR4 const value)
{
   JsonStr ctemp[80];

   _sprintf_s_l((char *) ctemp, 80, "%.8g", _JsonLocaleGet(), value);
   
   return _JsonSetBuffer(json, strlen(ctemp), (JsonN1 *) ctemp);
}

/******************************************************************************
func: _JsonSetStr
******************************************************************************/
JsonB _JsonSetStr(Json * const json, JsonN const strLen, JsonStr const * const str)
{
   JsonN    index;
   JsonN1   letter[2];
   
   letter[1] = 0;

   returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) jsonSTRING_QUOTE_STR));
   
   forCount(index, strLen)
   {
      if      (str[index] == '\"')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_QUOTE_STR));
      }
      else if (str[index] == '\\')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_BACK_SLASH_STR));
      }
      else if (str[index] == '/')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_FORWARD_SLASH_STR));
      }
      else if (str[index] == '\b')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_BACKSPACE_STR));
      }
      else if (str[index] == '\f')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_FORMFEED_STR));
      }
      else if (str[index] == '\n')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_LINEFEED_STR));
      }
      else if (str[index] == '\r')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR));
      }
      else if (str[index] == '\t')
      {
         returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_TAB_STR));
      }
      else
      {
         letter[0] = str[index];
         returnFalseIf(!_JsonSetBuffer(json, 1, letter));
      }
   }

   returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) jsonSTRING_QUOTE_STR));

   returnTrue;
}
