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
#include "local.h"
#include "jsonlib.h"

/******************************************************************************
local:
macro:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/
static JsonB _isStarted = jsonFALSE;

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: jsonCreateReader
******************************************************************************/
Json *jsonCreateReader(JsonGetBuffer getBufferFunc, void * const dataRepo)
{
   Json *json;

   returnNullIf(!_isStarted);

   // Create the json structure.
   json = _JsonMemCreateType(Json);
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
JsonB jsonCreateReaderContent(Json * const json, JsonGetBuffer getBufferFunc, 
   void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !getBufferFunc);

   _JsonMemClearType(Json, json);
   json->method               = jsonMethodREADING;
   json->dataRepo             = dataRepo;
   json->getBuffer            = getBufferFunc;

   return jsonTRUE;
}

/******************************************************************************
func: jsonCreateWriter
******************************************************************************/
Json *jsonCreateWriter(JsonSetBuffer setBufferFunc, void * const dataRepo, JsonB const isFormatted)
{
   Json *json;

   returnNullIf(!_isStarted);

   // Create the json structure
   json = _JsonMemCreateType(Json);
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
JsonB jsonCreateWriterContent(Json * const json, JsonSetBuffer setBufferFunc, 
   void * const dataRepo, JsonB const isFormatted)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !setBufferFunc);

   _JsonMemClearType(Json, json);
   if (isFormatted)
   {
      json->method   = jsonMethodWRITING_INDENTED;
   }
   else
   {
      json->method   = jsonMethodWRITING_COMPACT;
   }

   json->dataRepo    = dataRepo;
   json->setBuffer   = setBufferFunc;
   json->isFirstItem = jsonTRUE;

   return jsonTRUE;
}

/******************************************************************************
func: jsonDestroy
******************************************************************************/
void jsonDestroy(Json * const json)
{
   returnVoidIf(
      !_isStarted ||
      !json);

   jsonDestroyContent(json);
}

/******************************************************************************
func: jsonDestroyContent
******************************************************************************/
void jsonDestroyContent(Json * const json)
{
   returnVoidIf(
      !_isStarted ||
      !json);

   _JsonMemDestroy(json);
}

/******************************************************************************
func: jsonGetTypeFile

The first function you should be calling to obtain the value initial start of
the JSON file.
******************************************************************************/
JsonType jsonGetTypeFile(Json * const json)
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
      json->scopeType[json->scope++] = jsonScopeOBJECT;
      json->lastByte = 0;
      return jsonTypeFileOBJECT_START;

   case jsonARRAY_START_CHAR:
      json->scopeType[json->scope++] = jsonScopeARRAY;
      json->lastByte = 0;
      return jsonTypeFileARRAY_START;

   case jsonSTRING_QUOTE_CHAR:
      json->lastByte = 0;
      return jsonTypeFileSTRING_START;

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
JsonType jsonGetTypeObj(Json * const json)
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
      json->lastByte = 0;
      return jsonTypeObj_OBJECT_STOP;

   case jsonKEY_VALUE_SEPARATOR_CHAR:
      json->lastByte = 0;
      return jsonTypeObj_KEY_VALUE_SEPARATOR;

   case jsonSEPARATOR_CHAR:
      json->lastByte = 0;
      return jsonTypeObj_SEPARATOR;

   case jsonOBJECT_START_CHAR:
      json->scopeType[json->scope++] = jsonScopeOBJECT;
      json->lastByte = 0;
      return jsonTypeObj_OBJECT_START;

   case jsonARRAY_START_CHAR:
      json->scopeType[json->scope++] = jsonScopeARRAY;
      json->lastByte = 0;
      return jsonTypeObj_ARRAY_START;

   case jsonSTRING_QUOTE_CHAR:
      json->lastByte = 0;
      return jsonTypeObj_STRING_START;

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
JsonB jsonGetKey(Json * const json, JsonStr ** const key)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !key);

   *key = json->key;

   return jsonTRUE;
}

/******************************************************************************
func: jsonGetI
******************************************************************************/
JsonB jsonGetI(Json * const json, JsonI * const value)
{
   *value = 0;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      json->value.type != jsonTypeNUMBER_INTEGER);

   *value = json->value.i;

   return jsonTRUE;
}

/******************************************************************************
func: jsonGetN
******************************************************************************/
JsonB jsonGetN(Json * const json, JsonN *  const value)
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
         json->value.i    == (JsonI) json->value.n)));

   return jsonTRUE;
}

/******************************************************************************
func: jsonGetR

All numbers can be real.  However not all integers or naturals can be propely
or accurately be represented in real.  Up to the caller to determine if they
are doing something right in that situation.
******************************************************************************/
JsonB jsonGetR(Json * const json, JsonR *  const value)
{
   *value = json->value.r;

   return jsonTRUE;
}

/******************************************************************************
func: jsonGetR4
******************************************************************************/
JsonB jsonGetR4(Json * const json, JsonR4 *  const value)
{
   *value = json->value.r4;

   return jsonTRUE;
}

/******************************************************************************
func: jsonGetStr
******************************************************************************/
JsonB jsonGetStr(Json * const json, JsonI4 const maxCount, JsonStr *value)
{
   JsonI4        index;
   JsonStrLetter letterType;

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

   return jsonTRUE;
}

/******************************************************************************
func: jsonGetStrLetter

Returns...
jsonStrLetterNORMAL when is it just a regular UTF8 letter or letter part.
jsonStrLetterHEX    when it is a \u[hex][hex][hex][hex] esscaped sequence.
jsonStrLetterDONE   when the string was fully read.
jsonStrLetterERROR  when there was a problem reading the string.
******************************************************************************/
JsonStrLetter jsonGetStrLetter(Json * const json, JsonStr * const value)
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

      *value = (JsonStr) json->lastByte;
   }

   return jsonStrLetterNORMAL;
}

/******************************************************************************
func: jsonGetStrHex
******************************************************************************/
JsonB jsonGetStrHex(Json * const json, JsonStr * const h1, JsonStr * const h2, JsonStr * const h3, JsonStr * const h4)
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

   return jsonTRUE;
}

/******************************************************************************
func: jsonSetArrayStart
******************************************************************************/
JsonB jsonSetArrayStart(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }

   returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) jsonARRAY_START_STR));

   json->scopeType[json->scope] = jsonScopeARRAY;
   json->scope++;
   json->isFirstItem            = jsonTRUE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));

   return jsonTRUE;
}

/******************************************************************************
func: jsonSetArrayStop
******************************************************************************/
JsonB jsonSetArrayStop(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      json->scope == 0);

   json->scope--;
   json->scopeType[json->scope] = jsonScopeNONE;
   json->isFirstItem            = jsonFALSE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));
   returnFalseIf(!_JsonSetBuffer( json, 1, (JsonN1 *) jsonARRAY_STOP_STR));
   return jsonTRUE;
}

/******************************************************************************
func: jsonSetKey
******************************************************************************/
JsonB jsonSetKey(Json * const json, JsonStr const * const key)
{
   JsonI4 index;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      !key);

   returnFalseIf(!jsonSetSeparator(json));
   
   json->isFirstItem = jsonFALSE;

   returnFalseIf(!jsonSetValueStrStart(json));
   for (index = 0; ; index++)
   {
      breakIf(key[index] == 0);

      returnFalseIf(!jsonSetValueStrLetter(json, key[index]));
   }
   returnFalseIf(!jsonSetValueStrStop(json));

   returnFalseIf(!_JsonSetBuffer( json, 1, (JsonN1 *) jsonKEY_VALUE_SEPARATOR_STR));
   return jsonTRUE;
}

/******************************************************************************
func: jsonSetObjectStart
******************************************************************************/
JsonB jsonSetObjectStart(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }

   returnFalseIf(!_JsonSetBuffer( json, 1, (JsonN1 *) jsonOBJECT_START_STR));

   json->scopeType[json->scope] = jsonScopeOBJECT;
   json->scope++;
   json->isFirstItem            = jsonTRUE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));

   return jsonTRUE;
}

/******************************************************************************
func: jsonSetObjectStop
******************************************************************************/
JsonB jsonSetObjectStop(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      json->scope == 0);

   json->scope--;
   json->scopeType[json->scope] = jsonScopeNONE;
   json->isFirstItem            = jsonFALSE;

   returnFalseIf(!_JsonSetNewLine(json));
   returnFalseIf(!_JsonSetIndent( json));
   returnFalseIf(!_JsonSetBuffer( json, 1, (JsonN1 *) jsonOBJECT_STOP_STR));
   return jsonTRUE;
}

/******************************************************************************
func: jsonSetSeparator
******************************************************************************/
JsonB jsonSetSeparator(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (!json->isFirstItem)
   {
      returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) jsonSEPARATOR_STR));
      returnFalseIf(!_JsonSetNewLine( json));
      returnFalseIf(!_JsonSetIndent(  json));
   }
   json->isFirstItem = jsonFALSE;
   return jsonTRUE;
}

/******************************************************************************
func: jsonSetValueBool
******************************************************************************/
JsonB jsonSetValueBool(Json * const json, JsonB const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   if (value == jsonTRUE)
   {
      return _JsonSetBuffer(json, 4, (JsonN1 *) "true");
   }

   return _JsonSetBuffer(json, 5, (JsonN1 *) "false");
}

/******************************************************************************
func: jsonSetValueI
******************************************************************************/
JsonB jsonSetValueI(Json * const json, JsonI const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   return _JsonSetI(json, value);
}

/******************************************************************************
func: jsonSetValueN
******************************************************************************/
JsonB jsonSetValueN(Json * const json, JsonN const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   return _JsonSetN(json, value);
}

/******************************************************************************
func: jsonSetValueNull
******************************************************************************/
JsonB jsonSetValueNull(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   json->isFirstItem = jsonFALSE;

   return _JsonSetBuffer(json, 4, (JsonN1 *) "null");
}

/******************************************************************************
func: jsonSetValueR
******************************************************************************/
JsonB jsonSetValueR(Json * const json, JsonR const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   return _JsonSetR(json, value);
}

/******************************************************************************
func: jsonSetValueR4
******************************************************************************/
JsonB jsonSetValueR4(Json * const json, JsonR4 const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   return _JsonSetR4(json, value);
}

/******************************************************************************
func: jsonSetValueStr
******************************************************************************/
JsonB jsonSetValueStr(Json * const json, JsonStr const * const str)
{
   JsonI4 index;

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

   return jsonTRUE;
}

/******************************************************************************
func: jsonSetValueStrStart
******************************************************************************/
JsonB jsonSetValueStrStart(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   return _JsonSetBuffer(json, 1, (JsonN1 *) jsonSTRING_QUOTE_STR);
}

/******************************************************************************
func: jsonSetValueStrLetter
******************************************************************************/
JsonB jsonSetValueStrLetter(Json * const json, JsonStr const value)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   switch (value)
   {
   case jsonSTRING_QUOTE_CHAR:
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_QUOTE_STR));
      break;
      
   case jsonBACK_SLASH_CHAR:
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_BACK_SLASH_STR));
      break;

   case jsonFOREWARD_SLASH_CHAR:
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_FORWARD_SLASH_STR));
      break;

   case '\b':
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_BACKSPACE_STR));
      break;

   case '\f':
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_FORMFEED_STR));
      break;

   case '\n':
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_LINEFEED_STR));
      break;

   case '\r':
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_CARRIAGE_RETURN_STR));
      break;

   case '\t':
      returnFalseIf(!_JsonSetBuffer(json, 2, (JsonN1 *) jsonSTRING_ESCAPE_TAB_STR));
      break;

   default:
      returnFalseIf(!_JsonSetBuffer(json, 1, (JsonN1 *) &value));
   }

   return jsonTRUE;
}

/******************************************************************************
func: jsonSetValueStrStop
******************************************************************************/
JsonB jsonSetValueStrStop(Json * const json)
{
   returnFalseIf(
      !_isStarted ||
      !json);

   return _JsonSetBuffer(json, 1, (JsonN1 *) "\"");
}

/******************************************************************************
func: jsonStart
******************************************************************************/
JsonB jsonStart(JsonMemCreate const memCreateFunc, JsonMemDestroy const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress. (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _JsonMemStart(memCreateFunc, memDestroyFunc);
   _JsonStrStart();
   _JsonUtilStart();

   _isStarted = jsonTRUE;

   return jsonTRUE;
}

/******************************************************************************
func: jsonStop
******************************************************************************/
void jsonStop(void)
{
   returnVoidIf(!_isStarted);

   _JsonUtilStop();
   _JsonStrStop();
   _JsonMemStop();

   _isStarted = jsonFALSE;
}
