/******************************************************************************
file: JsonLib
author: Robbert de Groot
company:
copyright: 2021, Robbert de Groot

description:
Library for dealing with raw JSON files.
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
#include "jsonlib.h"

/******************************************************************************
local:
macro:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static Gb       _isStarted = gbFALSE;
static _locale_t   _locale;

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: jsonCreateReader
******************************************************************************/
Gjson *jsonCreateReader(GgetBuffer getBufferFunc, void * const dataRepo)
{
   Gjson *json;

   returnNullIf(!_isStarted);

   // Create the json structure.
   json = _JsonMemCreateType(Gjson);
   returnNullIf(!json);

   // Initialize the json structure.
   if (!jsonCreateReaderContent(json, getBufferFunc, dataRepo))
   {
      _JsonMemDestroy(json);
      returnNull;
   }

   // return the json structure
   return json;
}

/******************************************************************************
func: jsonCreateReaderContent
******************************************************************************/
Gb jsonCreateReaderContent(Gjson * const json, GgetBuffer getBufferFunc,
   void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !getBufferFunc);

   _JsonMemClearType(Gjson, json);
   json->method               = gmethodREAD;
   json->dataRepo             = dataRepo;
   json->getBuffer            = getBufferFunc;

   returnTrue;
}

/******************************************************************************
func: jsonCreateWriter
******************************************************************************/
Gjson *jsonCreateWriter(GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted)
{
   Gjson *json;

   returnNullIf(!_isStarted);

   // Create the json structure
   json = _JsonMemCreateType(Gjson);
   returnNullIf(!json);

   // Initialize the structure
   if (!jsonCreateWriterContent(json, setBufferFunc, dataRepo, isFormatted))
   {
      _JsonMemDestroy(json);
      returnNull;
   }

   // return the json structure
   return json;
}

/******************************************************************************
func: jsonCreateWriterContent
******************************************************************************/
Gb jsonCreateWriterContent(Gjson * const json, GsetBuffer setBufferFunc,
   void * const dataRepo, Gb const isFormatted)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !setBufferFunc);

   _JsonMemClearType(Gjson, json);
   if (isFormatted)
   {
      json->method   = gmethodWRITE_MINIMIZED;
   }
   else
   {
      json->method   = gmethodWRITE;
   }

   json->dataRepo    = dataRepo;
   json->setBuffer   = setBufferFunc;
   json->isFirstItem = gbTRUE;

   returnTrue;
}

/******************************************************************************
func: jsonDestroy
******************************************************************************/
void jsonDestroy(Gjson * const json)
{
   returnVoidIf(
      !_isStarted ||
      !json);

   jsonDestroyContent(json);
}

/******************************************************************************
func: jsonDestroyContent
******************************************************************************/
void jsonDestroyContent(Gjson * const json)
{
   returnVoidIf(
      !_isStarted ||
      !json);

   _JsonMemDestroy(json);
}

/******************************************************************************
func: jsonGetTypeElem
******************************************************************************/
GjsonType jsonGetTypeElem(Gjson * const json)
{
   returnIf(
         !_isStarted ||
         !json,
      jsonTypeNONE);

   // Reset the value type.
   json->value.type = jsonTypeNONE;

   // Eat space.
   _JsonEatSpace(json);

   // What do we have as a value.
   switch (json->lastByte)
   {
   case jsonARRAY_STOP_CHAR:
      json->scope--;
      json->method   =  json->scopeType[json->scope].method;
      json->lastByte = 0;
      return jsonTypeARRAY_STOP;

   case jsonSEPARATOR_CHAR:
      json->lastByte = 0;
      return jsonTypeSEPARATOR;

   case jsonOBJECT_START_CHAR:
      json->scopeType[json->scope++].type = jsonScopeTypeOBJECT;
      json->lastByte = 0;
      return jsonTypeOBJECT_START;

   case jsonARRAY_START_CHAR:
      json->scopeType[json->scope++].type = jsonScopeTypeARRAY;
      json->method   = gmethodWRITE;
      json->lastByte = 0;
      return jsonTypeARRAY_START;

   case jsonSTRING_QUOTE_CHAR:
      json->lastByte = 0;
      return jsonTypeSTRING_START;

   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case '-':
      return _JsonGetNumber(json);

   case 't':
      return _JsonGetTrue(json);

   case 'f':
      return _JsonGetFalse(json);

   case 'n':
      return _JsonGetNull(json);
   }

   return jsonTypeERROR_UNEXPECTED_CHAR;
}

/******************************************************************************
func: jsonGetTypeFile

The first function you should be calling to obtain the value initial start of
the JSON file.
******************************************************************************/
GjsonType jsonGetTypeFile(Gjson * const json)
{
   returnIf(
         !_isStarted ||
         !json,
      jsonTypeNONE);

   // Reset the value type.
   json->value.type = jsonTypeNONE;

   // Eat space.
   _JsonEatSpace(json);

   // What do we have as a value.
   switch (json->lastByte)
   {
   case jsonOBJECT_START_CHAR:
      json->scopeType[json->scope++].type = jsonScopeTypeOBJECT;
      json->lastByte = 0;
      return jsonTypeOBJECT_START;

   case jsonARRAY_START_CHAR:
      json->scopeType[json->scope++].type = jsonScopeTypeARRAY;
      json->lastByte = 0;
      return jsonTypeARRAY_START;

   case jsonSTRING_QUOTE_CHAR:
      json->lastByte = 0;
      return jsonTypeSTRING_START;

   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case '-':
      return _JsonGetNumber(json);

   case 't':
      return _JsonGetTrue(json);

   case 'f':
      return _JsonGetFalse(json);

   case 'n':
      return _JsonGetNull(json);
   }

   return jsonTypeERROR_UNEXPECTED_CHAR;
}

/******************************************************************************
func: jsonGetTypeObj
******************************************************************************/
GjsonType jsonGetTypeObj(Gjson * const json)
{
   returnIf(
         !_isStarted ||
         !json,
      jsonTypeNONE);

   // Reset the value type.
   json->value.type = jsonTypeNONE;

   // Eat space.
   _JsonEatSpace(json);

   // What do we have as a value.
   switch (json->lastByte)
   {
   case jsonOBJECT_STOP_CHAR:
      json->scope--;
      json->method   =  json->scopeType[json->scope].method;
      json->lastByte = 0;
      return jsonTypeOBJECT_STOP;

   case jsonKEY_VALUE_SEPARATOR_CHAR:
      json->lastByte = 0;
      return jsonTypeKEY_VALUE_SEPARATOR;

   case jsonSEPARATOR_CHAR:
      json->lastByte = 0;
      return jsonTypeSEPARATOR;

   case jsonOBJECT_START_CHAR:
      json->scopeType[json->scope++].type = jsonScopeTypeOBJECT;
      json->lastByte = 0;
      return jsonTypeOBJECT_START;

   case jsonARRAY_START_CHAR:
      json->scopeType[json->scope++].type = jsonScopeTypeARRAY;
      json->lastByte = 0;
      return jsonTypeARRAY_START;

   case jsonSTRING_QUOTE_CHAR:
      json->lastByte = 0;
      return jsonTypeSTRING_START;

   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
   case '-':
      return _JsonGetNumber(json);

   case 't':
      return _JsonGetTrue(json);

   case 'f':
      return _JsonGetFalse(json);

   case 'n':
      return _JsonGetNull(json);
   }

   return jsonTypeERROR_UNEXPECTED_CHAR;
}

/******************************************************************************
func: jsonGetKey
******************************************************************************/
Gb jsonGetKey(Gjson * const json, Gstr ** const key)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !key);

   *key = json->key;

   returnTrue;
}

/******************************************************************************
func: jsonGetI
******************************************************************************/
Gb jsonGetI(Gjson * const json, Gi8 * const value)
{
   *value = 0;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      json->value.type != jsonTypeNUMBER_INTEGER);

   *value = json->value.i;

   returnTrue;
}

/******************************************************************************
func: jsonGetN
******************************************************************************/
Gb jsonGetN(Gjson * const json, Gn8 *  const value)
{
   *value = 0;

   returnFalseIf(
      !_isStarted ||
      !json);

   *value = json->value.n;

   // Return is false if the type of the number is no a natural or if it is a negative integer.
   returnFalseIf(
      !(json->value.type == jsonTypeNUMBER_NATURAL    ||
        (json->value.type == jsonTypeNUMBER_INTEGER &&
         json->value.i    == (Gi8) json->value.n)));

   returnTrue;
}

/******************************************************************************
func: jsonGetR

All numbers can be real.  However not all integers or naturals can be propely
or accurately be represented in real.  Up to the caller to determine if they
are doing something right in that situation.
******************************************************************************/
Gb jsonGetR(Gjson * const json, Gr8 *  const value)
{
   *value = json->value.r;

   returnTrue;
}

/******************************************************************************
func: jsonGetR4
******************************************************************************/
Gb jsonGetR4(Gjson * const json, Gr4 *  const value)
{
   *value = json->value.r4;

   returnTrue;
}

/******************************************************************************
func: jsonGetStr
******************************************************************************/
Gb jsonGetStr(Gjson * const json, Gi4 const maxCount, Gstr *value)
{
   Gi4        index;
   GjsonStrLetter letterType;

   // Get the letters.
   for (index = 0; index < maxCount; index++)
   {
      letterType = jsonGetStrLetter(json, &value[index]);
      breakIf(letterType == jsonStrLetterDONE);

      // Convert Unicode letter to UTF8
      if (letterType == jsonStrLetterHEX)
      {
         // todo
      }
      returnFalseIf(letterType == jsonStrLetterERROR);
   }

   // Null terminate
   if (index != maxCount)
   {
      value[index] = 0;
   }

   returnTrue;
}

/******************************************************************************
func: jsonGetStrBinByte
******************************************************************************/
GjsonStrLetter jsonGetStrBinByte(Gjson * const json, Gn1 * const value)
{
   Gn1 vtemp;

   returnIf(!json, jsonStrLetterERROR);

   returnIf(!json->getBuffer(json->dataRepo, 1, &json->lastByte), jsonStrLetterERROR);

   returnIf(json->lastByte == '\"', jsonStrLetterDONE);

   switch (json->lastByte)
   {
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      vtemp = (json->lastByte - '0') << 4;
      break;

   case 'A':
   case 'B':
   case 'C':
   case 'D':
   case 'E':
   case 'F':
      vtemp = (json->lastByte - 'A' + 0xA) << 4;
      break;

   case 'a':
   case 'b':
   case 'c':
   case 'd':
   case 'e':
   case 'f':
      vtemp = (json->lastByte - 'a' + 0xA) << 4;
      break;
   }

   returnIf(!json->getBuffer(json->dataRepo, 1, &json->lastByte), jsonStrLetterERROR);

   returnIf(json->lastByte == '\"', jsonStrLetterERROR);

   switch (json->lastByte)
   {
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      vtemp |= json->lastByte - '0';
      break;

   case 'A':
   case 'B':
   case 'C':
   case 'D':
   case 'E':
   case 'F':
      vtemp |= json->lastByte - 'A' + 0xA;
      break;

   case 'a':
   case 'b':
   case 'c':
   case 'd':
   case 'e':
   case 'f':
      vtemp |= json->lastByte - 'a' + 0xA;
      break;
   }

   *value = vtemp;

   return jsonStrLetterNORMAL;
}

/******************************************************************************
func: jsonGetStrLetter

Returns...
jsonStrLetterNORMAL when is it just a regular UTF8 letter or letter part.
jsonStrLetterHEX    when it is a \u[hex][hex][hex][hex] esscaped sequence.
jsonStrLetterDONE   when the string was fully read.
jsonStrLetterERROR  when there was a problem reading the string.
******************************************************************************/
GjsonStrLetter jsonGetStrLetter(Gjson * const json, Gstr * const value)
{
   *value = 0;

   returnIf(!_JsonGetChar(json), jsonStrLetterERROR);

   // Escaped letter
   if (json->lastByte == '\\')
   {
      returnIf(!_JsonGetChar(json), jsonStrLetterERROR);

      switch (json->lastByte)
      {
      case jsonSTRING_QUOTE_CHAR:
         *value = jsonSTRING_QUOTE_CHAR;
         break;

      case jsonBACK_SLASH_CHAR:
         *value = jsonBACK_SLASH_CHAR;
         break;

      case jsonFOREWARD_SLASH_CHAR:
         *value = jsonFOREWARD_SLASH_CHAR;
         break;

      case 'b':
         *value = '\b';
         break;

      case 'f':
         *value = '\f';
         break;

      case 'n':
         *value = '\n';
         break;

      case 'r':
         *value = '\r';
         break;

      case 't':
         *value = '\t';
         break;

      case 'u':
         *value = 0;
         returnFalseIf(!_JsonGetChar(json));
         json->hex[0] = _JsonStrToHex(json->lastByte);

         returnFalseIf(!_JsonGetChar(json));
         json->hex[1] = _JsonStrToHex(json->lastByte);

         returnFalseIf(!_JsonGetChar(json));
         json->hex[1] = _JsonStrToHex(json->lastByte);

         returnFalseIf(!_JsonGetChar(json));
         json->hex[1] = _JsonStrToHex(json->lastByte);
         return jsonStrLetterHEX;

      default:
         // Unknow excape sequence.
         return jsonStrLetterERROR;
      }
   }
   else
   {
      if (json->lastByte == '\"')
      {
         json->lastByte = 0;

         return jsonStrLetterDONE;
      }

      *value = (Gstr) json->lastByte;
   }

   return jsonStrLetterNORMAL;
}

/******************************************************************************
func: jsonGetStrHex
******************************************************************************/
Gb jsonGetStrHex(Gjson * const json, Gstr * const h1, Gstr * const h2, Gstr * const h3, Gstr * const h4)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !h1         ||
      !h2         ||
      !h3         ||
      !h4);

   *h1 = json->hex[0];
   *h2 = json->hex[1];
   *h3 = json->hex[2];
   *h4 = json->hex[3];

   returnTrue;
}

/******************************************************************************
func: jsonSetArrayStart
******************************************************************************/
Gb jsonSetArrayStart(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }

   returnFalseIf(!_JsonSetBuffer(json, 1, (Gn1 *) jsonARRAY_START_STR));

   json->scopeType[json->scope].type   = jsonScopeTypeARRAY;
   json->scopeType[json->scope].method = json->method;
   json->method                        = gmethodWRITE;
   json->scope++;
   json->isFirstItem                   = gbTRUE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));

   returnTrue;
}

/******************************************************************************
func: jsonSetArrayStop
******************************************************************************/
Gb jsonSetArrayStop(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      json->scope == 0);

   json->scope--;
   json->scopeType[json->scope].type = jsonScopeTypeNONE;
   json->isFirstItem                 = gbFALSE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));
   returnFalseIf(!_JsonSetBuffer( json, 1, (Gn1 *) jsonARRAY_STOP_STR));

   json->method = json->scopeType[json->scope].method;

   returnTrue;
}

/******************************************************************************
func: jsonSetKey
******************************************************************************/
Gb jsonSetKey(Gjson * const json, Gstr const * const key)
{
   Gi4 index;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      !key);

   returnFalseIf(!jsonSetSeparator(json));

   json->isFirstItem = gbFALSE;

   returnFalseIf(!jsonSetValueStrStart(json));
   for (index = 0; ; index++)
   {
      breakIf(key[index] == 0);

      returnFalseIf(!jsonSetValueStrLetter(json, key[index]));
   }
   returnFalseIf(!jsonSetValueStrStop(json));

   returnFalseIf(!_JsonSetBuffer( json, 1, (Gn1 *) jsonKEY_VALUE_SEPARATOR_STR));
   returnTrue;
}

/******************************************************************************
func: jsonSetObjectStart
******************************************************************************/
Gb jsonSetObjectStart(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }

   returnFalseIf(!_JsonSetBuffer( json, 1, (Gn1 *) jsonOBJECT_START_STR));

   json->scopeType[json->scope].type   = jsonScopeTypeOBJECT;
   json->scopeType[json->scope].method = json->method;
   json->scope++;
   json->isFirstItem            = gbTRUE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));

   returnTrue;
}

/******************************************************************************
func: jsonSetObjectStop
******************************************************************************/
Gb jsonSetObjectStop(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      json->scope == 0);

   json->scope--;
   json->scopeType[json->scope].type = jsonScopeTypeNONE;
   json->method                      = json->scopeType[json->scope].method;
   json->isFirstItem                 = gbFALSE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));
   returnFalseIf(!_JsonSetBuffer( json, 1, (Gn1 *) jsonOBJECT_STOP_STR));
   returnTrue;
}

/******************************************************************************
func: jsonSetSeparator
******************************************************************************/
Gb jsonSetSeparator(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (!json->isFirstItem)
   {
      returnFalseIf(!_JsonSetBuffer(json, 1, (Gn1 *) jsonSEPARATOR_STR));
      returnFalseIf(!_JsonSetNewLine( json));
      returnFalseIf(!_JsonSetIndent(  json));
   }
   json->isFirstItem = gbFALSE;
   returnTrue;
}

/******************************************************************************
func: jsonSetValueBin
******************************************************************************/
Gb jsonSetValueBin(Gjson * const json, Gn8 const count, Gn1 const * const value)
{
   Gi4 index;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      !value);

   returnFalseIf(!jsonSetValueStrStart(json));
   for (index = 0; index < count; index++)
   {
      returnFalseIf(!jsonSetValueStrBinByte(json, value[index]));
   }
   returnFalseIf(!jsonSetValueStrStop(json));

   returnTrue;
}

/******************************************************************************
func: jsonSetValueBool
******************************************************************************/
Gb jsonSetValueBool(Gjson * const json, Gb const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = gbFALSE;

   if (value == gbTRUE)
   {
      return _JsonSetBuffer(json, 4, (Gn1 *) "true");
   }

   return _JsonSetBuffer(json, 5, (Gn1 *) "false");
}

/******************************************************************************
func: jsonSetValueI
******************************************************************************/
Gb jsonSetValueI(Gjson * const json, Gi8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = gbFALSE;

   return _JsonSetI(json, value);
}

/******************************************************************************
func: jsonSetValueN
******************************************************************************/
Gb jsonSetValueN(Gjson * const json, Gn8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = gbFALSE;

   return _JsonSetN(json, value);
}

/******************************************************************************
func: jsonSetValueNull
******************************************************************************/
Gb jsonSetValueNull(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   json->isFirstItem = gbFALSE;

   return _JsonSetBuffer(json, 4, (Gn1 *) "null");
}

/******************************************************************************
func: jsonSetValueR
******************************************************************************/
Gb jsonSetValueR(Gjson * const json, Gr8 const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = gbFALSE;

   return _JsonSetR(json, value);
}

/******************************************************************************
func: jsonSetValueR4
******************************************************************************/
Gb jsonSetValueR4(Gjson * const json, Gr4 const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = gbFALSE;

   return _JsonSetR4(json, value);
}

/******************************************************************************
func: jsonSetValueStr
******************************************************************************/
Gb jsonSetValueStr(Gjson * const json, Gstr const * const str)
{
   Gi4 index;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      !str);

   returnFalseIf(!jsonSetValueStrStart(json));
   for (index = 0; ; index++)
   {
      breakIf(str[index] == 0);

      returnFalseIf(!jsonSetValueStrLetter(json, str[index]));
   }
   returnFalseIf(!jsonSetValueStrStop(json));

   returnTrue;
}

/******************************************************************************
func: jsonSetValueStrBinByte
******************************************************************************/
Gb jsonSetValueStrBinByte(Gjson * const json, Gn1 const value)
{
   Gstr letters[] = "0123456789ABCDEF";
   Gn1  output[2];

   returnFalseIf(!json);

   output[0] = letters[value >> 4];
   output[1] = letters[value & 0x0F];

   returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) output));

   returnTrue;
}

/******************************************************************************
func: jsonSetValueStrLetter
******************************************************************************/
Gb jsonSetValueStrLetter(Gjson * const json, Gstr const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   switch (value)
   {
   case jsonSTRING_QUOTE_CHAR:
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_QUOTE_STR));
      break;

   case jsonBACK_SLASH_CHAR:
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_BACK_SLASH_STR));
      break;

   case jsonFOREWARD_SLASH_CHAR:
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_FORWARD_SLASH_STR));
      break;

   case '\b':
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_BACKSPACE_STR));
      break;

   case '\f':
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_FORMFEED_STR));
      break;

   case '\n':
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_LINEFEED_STR));
      break;

   case '\r':
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR));
      break;

   case '\t':
      returnFalseIf(!_JsonSetBuffer(json, 2, (Gn1 *) jsonSTRING_ESCAPE_TAB_STR));
      break;

   default:
      returnFalseIf(!_JsonSetBuffer(json, 1, (Gn1 *) &value));
   }

   returnTrue;
}

/******************************************************************************
func: jsonSetValueStrStart
******************************************************************************/
Gb jsonSetValueStrStart(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1].type == jsonScopeTypeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = gbFALSE;

   return _JsonSetBuffer(json, 1, (Gn1 *) jsonSTRING_QUOTE_STR);
}

/******************************************************************************
func: jsonSetValueStrStop
******************************************************************************/
Gb jsonSetValueStrStop(Gjson * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   return _JsonSetBuffer(json, 1, (Gn1 *) "\"");
}

/******************************************************************************
func: jsonStart
******************************************************************************/
Gb jsonStart(GmemCloc const memCreateFunc, GmemDloc const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress. (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _JsonMemStart(memCreateFunc, memDestroyFunc);
   _JsonStrStart();

   _locale = _create_locale(LC_ALL, "C");

   _isStarted = gbTRUE;

   returnTrue;
}

/******************************************************************************
func: jsonStop
******************************************************************************/
void jsonStop(void)
{
   returnVoidIf(!_isStarted);

   _JsonStrStop();
   _JsonMemStop();

   _isStarted = gbFALSE;
}

/******************************************************************************
library local:
function:
******************************************************************************/
/******************************************************************************
func: _JsonLocaleGet
******************************************************************************/
_locale_t _JsonLocaleGet(void)
{
   return _locale;
}
