/******************************************************************************
file:       JsonLib
author:     Robbert de Groot
company:
copyright:  2021, Robbert de Groot

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
variable:
******************************************************************************/
static JsonBool    _isStarted = jsonBoolFALSE;

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
JsonBool jsonCreateReaderContent(Json * const json, JsonGetBuffer getBufferFunc, 
   void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !getBufferFunc);

   _JsonMemClearType(Json, json);
   json->method      = jsonMethodREADING;
   json->dataRepo    = dataRepo;
   json->getBuffer   = getBufferFunc;

   returnTrue;
}

/******************************************************************************
func: jsonCreateWriter
******************************************************************************/
Json *jsonCreateWriter(JsonSetBuffer setBufferFunc, void * const dataRepo)
{
   Json *json;

   returnNullIf(!_isStarted);

   // Create the json structure
   json = _JsonMemCreateType(Json);
   returnNullIf(!json);

   // Initialize the structure
   if (!jsonCreateWriterContent(json, setBufferFunc, dataRepo))
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
JsonBool jsonCreateWriterContent(Json * const json, JsonSetBuffer setBufferFunc, 
   void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !setBufferFunc);

   _JsonMemClearType(Json, json);
   json->method      = jsonMethodWRITING;
   json->dataRepo    = dataRepo;
   json->setBuffer   = setBufferFunc;

   returnTrue;
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
func: jsonWriteArrayStart
******************************************************************************/
JsonBool jsonWriteArrayStart(Json * const json)
{
   json->isLastWriteValue = jsonBoolFALSE;
   json->isIndentMissing  = jsonBoolTRUE;
   json->scope++;

   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) jsonARRAY_START_STR));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) "\n"));
   returnTrue;
}

/******************************************************************************
func: jsonWriteArrayStop
******************************************************************************/
JsonBool jsonWriteArrayStop(Json * const json)
{
   if      (json->isMissingIndent)
   {
      // Nothing to do.  Empty array.
   }
   else if (json->isLastWriteValue)
   {
      returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
   }

   json->isLastWriteValue = jsonBoolFALSE;
   json->isIndentMissing  = jsonBoolTRUE;
   json->scope--;
   
   returnFalseIf(!_JsonWriteIndent(json));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) jsonARRAY_STOP_STR));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) "\n"));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey
******************************************************************************/
JsonBool jsonWriteKey(Json * const json, JsonC2 const * const key)
{
   // Missing indent we are following a open or close object, open or close array.
   if      (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }
   // Last item written was a value, this must be another key value in an object.  Add a separator.
   else if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
   }

   returnFalseIf(!jsonWriteValueString(json, key));
   returnFalseIf(!_JsonWriteC1(            json, (JsonC1 *) jsonKEY_VALUE_SEPARATOR_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObjectStart
******************************************************************************/
JsonBool jsonWriteObjectStart(Json * const json)
{
   json->isLastWriteValue = jsonBoolFALSE;

   json->scope++;
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) jsonOBJECT_START_STR));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObjectStop
******************************************************************************/
JsonBool jsonWriteObjectStop(Json * const json)
{
   if (json->isLastWriteValue)
   {
      returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
   }
   json->isLastWriteValue = jsonBoolFALSE;
   
   json->scope--;
   returnFalseIf(!_JsonWriteIndent(json));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) jsonOBJECT_STOP_STR));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteSeparator
******************************************************************************/
JsonBool jsonWriteSeparator(Json * const json)
{
   json->isLastWriteValue = jsonBoolFALSE;

   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) jsonSEPARATOR_STR));
   returnFalseIf(!_JsonWriteC1(    json, (JsonC1 *) "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueI
******************************************************************************/
JsonBool jsonWriteValueI(Json * const json, JsonI8 const value)
{
   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteI(json, value);
}

/******************************************************************************
func: jsonWriteValueN
******************************************************************************/
JsonBool jsonWriteValueN(Json * const json, JsonN8 const value)
{
   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteN(json, value);
}

/******************************************************************************
func: jsonWriteValueString
******************************************************************************/
JsonBool jsonWriteValueString(Json * const json, JsonC2 const * const value)
{
   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteString(json, value);
}

/******************************************************************************
func: jsonWriteValueR4
******************************************************************************/
JsonBool jsonWriteValueR4(Json * const json, JsonR4 const value)
{
   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteR4(json, value);
}

/******************************************************************************
func: jsonWriteValueR8
******************************************************************************/
JsonBool jsonWriteValueR8(Json * const json, JsonR8 const value)
{
   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteR8(json, value);
}

/******************************************************************************
func: jsonStart
******************************************************************************/
JsonBool jsonStart(JsonMemCreate const memCreateFunc, JsonMemDestroy const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress.  (sometimes)
   // we can't live without dynamic memory.
   returnFalseIf(
      !memCreateFunc ||
      !memDestroyFunc)

   _JsonMemStart(memCreateFunc, memDestroyFunc);

   _isStarted = jsonBoolTRUE;

   returnTrue;
}

/******************************************************************************
func: jsonStop
******************************************************************************/
void jsonStop(void)
{
   returnVoidIf(!_isStarted);

   _JsonMemStop();

   _isStarted = jsonBoolFALSE;
}

/******************************************************************************
helper functions:
******************************************************************************/
/******************************************************************************
func: jsonWriteKey1Boolean
******************************************************************************/
JsonBool jsonWriteKey1Boolean(Json * const json, JsonC2 const * const key, JsonBool const value)
{
   returnFalseIf(!jsonWriteKey(json, key));
   returnFalseIf(!jsonWriteValueString(json, (value == jsonBoolTRUE) ? L"true" : L"false"));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1I
******************************************************************************/
JsonBool jsonWriteKey1I(Json * const json, JsonC2 const * const key, JsonI8 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueI(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1N
******************************************************************************/
JsonBool jsonWriteKey1N(Json * const json, JsonC2 const * const key, JsonN8 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueN(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1R4
******************************************************************************/
JsonBool jsonWriteKey1R4(Json * const json, JsonC2 const * const key, JsonR4 const value)
{
   returnFalseIf(!jsonWriteKey(    json, key));
   returnFalseIf(!jsonWriteValueR4(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1R8
******************************************************************************/
JsonBool jsonWriteKey1R8(Json * const json, JsonC2 const * const key, JsonR8 const value)
{
   returnFalseIf(!jsonWriteKey(    json, key));
   returnFalseIf(!jsonWriteValueR8(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1String
******************************************************************************/
JsonBool jsonWriteKey1String(Json * const json, JsonC2 const * const key, JsonC2 const * const value)
{
   returnFalseIf(!jsonWriteKey(    json, key));
   returnFalseIf(!jsonWriteValueString(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObject1Boolean
******************************************************************************/
JsonBool jsonWriteObject1Boolean(Json * const json, JsonC2 const * const key, JsonBool const value)
{
   returnFalseIf(!jsonWriteObjectStart(json));
   returnFalseIf(!jsonWriteKey1Boolean(json, key, value));
   returnFalseIf(!jsonWriteObjectStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObject1I
******************************************************************************/
JsonBool jsonWriteObject1I(Json * const json, JsonC2 const * const key, JsonI8 const value)
{
   returnFalseIf(!jsonWriteObjectStart(json));
   returnFalseIf(!jsonWriteKey1I(      json, key, value));
   returnFalseIf(!jsonWriteObjectStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObject1N
******************************************************************************/
JsonBool jsonWriteObject1N(Json * const json, JsonC2 const * const key, JsonN8 const value)
{
   returnFalseIf(!jsonWriteObjectStart(json));
   returnFalseIf(!jsonWriteKey1N(      json, key, value));
   returnFalseIf(!jsonWriteObjectStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObject1R4
******************************************************************************/
JsonBool jsonWriteObject1R4(Json * const json, JsonC2 const * const key, JsonR4 const value)
{
   returnFalseIf(!jsonWriteObjectStart(json));
   returnFalseIf(!jsonWriteKey1R4(     json, key, value));
   returnFalseIf(!jsonWriteObjectStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObject1R8
******************************************************************************/
JsonBool jsonWriteObject1R8(Json * const json, JsonC2 const * const key, JsonR8 const value)
{
   returnFalseIf(!jsonWriteObjectStart(json));
   returnFalseIf(!jsonWriteKey1R8(     json, key, value));
   returnFalseIf(!jsonWriteObjectStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObject1String
******************************************************************************/
JsonBool jsonWriteObject1String(Json * const json, JsonC2 const * const key, JsonC2 const * const value)
{
   returnFalseIf(!jsonWriteObjectStart(json));
   returnFalseIf(!jsonWriteKey1String( json, key, value));
   returnFalseIf(!jsonWriteObjectStop( json));
   returnTrue;
}