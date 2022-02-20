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
variable:
******************************************************************************/
static JsonBool _isStarted = jsonBoolFALSE;

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
   json->method = jsonMethodREADING;
   json->dataRepo = dataRepo;
   json->getBuffer = getBufferFunc;

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
   json->method = jsonMethodWRITING;
   json->dataRepo = dataRepo;
   json->setBuffer = setBufferFunc;

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
func: jsonStart
******************************************************************************/
JsonBool jsonStart(JsonMemCreate const memCreateFunc, JsonMemDestroy const memDestroyFunc)
{
   returnTrueIf(_isStarted);

   // We can live without compress. (sometimes)
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
func: jsonWriteArrayStart
******************************************************************************/
JsonBool jsonWriteArrayStart(Json * const json)
{
   json->isLastWriteValue = jsonBoolFALSE;
   json->isIndentMissing = jsonBoolTRUE;
   json->scope++;

   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonARRAY_START_STR));
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
   returnTrue;
}

/******************************************************************************
func: jsonWriteArrayStop
******************************************************************************/
JsonBool jsonWriteArrayStop(Json * const json)
{
   if      (json->isIndentMissing)
   {
      // Nothing to do. Empty array.
   }
   else if (json->isLastWriteValue)
   {
      returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
   }

   json->isLastWriteValue = jsonBoolFALSE;
   json->isIndentMissing = jsonBoolTRUE;
   json->scope--;

   returnFalseIf(!_JsonWriteIndent(json));
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonARRAY_STOP_STR));
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
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
   // Last item written was a value, this must be another key value in an object. Add a separator.
   else if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
   }

   returnFalseIf(!jsonWriteValueStringC2(json, key));
   returnFalseIf(!_JsonWriteC1(          json, (JsonC1 *) jsonKEY_VALUE_SEPARATOR_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObjectStart
******************************************************************************/
JsonBool jsonWriteObjectStart(Json * const json)
{
   json->isLastWriteValue = jsonBoolFALSE;

   json->scope++;
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonOBJECT_START_STR));
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
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
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonOBJECT_STOP_STR));
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteSeparator
******************************************************************************/
JsonBool jsonWriteSeparator(Json * const json)
{
   json->isLastWriteValue = jsonBoolFALSE;

   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) jsonSEPARATOR_STR));
   returnFalseIf(!_JsonWriteC1(json, (JsonC1 *) "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABI
******************************************************************************/
JsonBool jsonWriteValueABI(Json * const json, JsonI8 const va, JsonI8 const vb)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueI(    json, va));
   returnFalseIf(!jsonWriteValueI(    json, vb));
   returnFalseIf(!jsonWriteArrayStop( json));

   json->isLastWriteValue = jsonBoolTRUE;
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABN
******************************************************************************/
JsonBool jsonWriteValueABN(Json * const json, JsonN8 const va, JsonN8 const vb)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueN(    json, va));
   returnFalseIf(!jsonWriteValueN(    json, vb));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABR4
******************************************************************************/
JsonBool jsonWriteValueABR4(Json * const json, JsonABR4 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->a));
   returnFalseIf(!jsonWriteValueR4(   json, value->b));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABR8
******************************************************************************/
JsonBool jsonWriteValueABR8(Json * const json, JsonABR8 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->a));
   returnFalseIf(!jsonWriteValueR8(   json, value->b));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCI
******************************************************************************/
JsonBool jsonWriteValueABCI(Json * const json, JsonI8 const va, JsonI8 const vb, JsonI8 const vc)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueI(    json, va));
   returnFalseIf(!jsonWriteValueI(    json, vb));
   returnFalseIf(!jsonWriteValueI(    json, vc));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCN
******************************************************************************/
JsonBool jsonWriteValueABCN(Json * const json, JsonN8 const va, JsonN8 const vb, JsonN8 const vc)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueN(    json, va));
   returnFalseIf(!jsonWriteValueN(    json, vb));
   returnFalseIf(!jsonWriteValueN(    json, vc));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCR4
******************************************************************************/
JsonBool jsonWriteValueABCR4(Json * const json, JsonABCR4 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->a));
   returnFalseIf(!jsonWriteValueR4(   json, value->b));
   returnFalseIf(!jsonWriteValueR4(   json, value->c));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCR8
******************************************************************************/
JsonBool jsonWriteValueABCR8(Json * const json, JsonABCR8 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->a));
   returnFalseIf(!jsonWriteValueR8(   json, value->b));
   returnFalseIf(!jsonWriteValueR8(   json, value->c));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCDI
******************************************************************************/
JsonBool jsonWriteValueABCDI(Json * const json, JsonI8 const va, JsonI8 const vb, JsonI8 const vc, JsonI8 const vd)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueI(    json, va));
   returnFalseIf(!jsonWriteValueI(    json, vb));
   returnFalseIf(!jsonWriteValueI(    json, vc));
   returnFalseIf(!jsonWriteValueI(    json, vd));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCDN
******************************************************************************/
JsonBool jsonWriteValueABCDN(Json * const json, JsonN8 const va, JsonN8 const vb, JsonN8 const vc, JsonN8 const vd)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueN(    json, va));
   returnFalseIf(!jsonWriteValueN(    json, vb));
   returnFalseIf(!jsonWriteValueN(    json, vc));
   returnFalseIf(!jsonWriteValueN(    json, vd));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCDR4
******************************************************************************/
JsonBool jsonWriteValueABCDR4(Json * const json, JsonABCDR4 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->a));
   returnFalseIf(!jsonWriteValueR4(   json, value->b));
   returnFalseIf(!jsonWriteValueR4(   json, value->c));
   returnFalseIf(!jsonWriteValueR4(   json, value->d));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueABCDR8
******************************************************************************/
JsonBool jsonWriteValueABCDR8(Json * const json, JsonABCDR8 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->a));
   returnFalseIf(!jsonWriteValueR8(   json, value->b));
   returnFalseIf(!jsonWriteValueR8(   json, value->c));
   returnFalseIf(!jsonWriteValueR8(   json, value->d));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueBoolean
******************************************************************************/
JsonBool jsonWriteValueBoolean(Json * const json, JsonBool const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteValueStringC2(json, (value == jsonBoolTRUE) ? L"true" : L"false"));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueI
******************************************************************************/
JsonBool jsonWriteValueI(Json * const json, JsonI8 const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteI(json, value);
}

/******************************************************************************
func: jsonWriteValueN
******************************************************************************/
JsonBool jsonWriteValueN(Json * const json, JsonN8 const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteN(json, value);
}

/******************************************************************************
func: jsonWriteValueMatrix2x2R4
******************************************************************************/
JsonBool jsonWriteValueMatrix2x2R4(Json * const json, JsonMatrix2x2R4 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueMatrix2x2R8
******************************************************************************/
JsonBool jsonWriteValueMatrix2x2R8(Json * const json, JsonMatrix2x2R8 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueMatrix3x3R4
******************************************************************************/
JsonBool jsonWriteValueMatrix3x3R4(Json * const json, JsonMatrix3x3R4 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueMatrix3x3R8
******************************************************************************/
JsonBool jsonWriteValueMatrix3x3R8(Json * const json, JsonMatrix3x3R8 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueMatrix4x4R4
******************************************************************************/
JsonBool jsonWriteValueMatrix4x4R4(Json * const json, JsonMatrix4x4R4 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][3]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][3]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][3]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][3]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueMatrix4x4R8
******************************************************************************/
JsonBool jsonWriteValueMatrix4x4R8(Json * const json, JsonMatrix4x4R8 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][3]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][3]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][3]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][3]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueStringC2
******************************************************************************/
JsonBool jsonWriteValueStringC2(Json * const json, JsonC2 const * const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteStringC2(json, value);
}

/******************************************************************************
func: jsonWriteValueR4
******************************************************************************/
JsonBool jsonWriteValueR4(Json * const json, JsonR4 const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteR4(json, value);
}

/******************************************************************************
func: jsonWriteValueR8
******************************************************************************/
JsonBool jsonWriteValueR8(Json * const json, JsonR8 const value)
{
   // Last item written was a value, this must be another key value in an object. Add a separator.
   if (json->isLastWriteValue)
   {
      json->isLastWriteValue = jsonBoolFALSE;
      returnFalseIf(!jsonWriteSeparator(json));
      returnFalseIf(!_JsonWriteIndent(  json));
   }
   // Missing indent we are following a open or close object, open or close array.
   else if (json->isIndentMissing)
   {
      returnFalseIf(!_JsonWriteIndent(json));
      json->isIndentMissing = jsonBoolFALSE;
   }

   json->isLastWriteValue = jsonBoolTRUE;
   return _JsonWriteR8(json, value);
}

/******************************************************************************
helper functions:
******************************************************************************/
/******************************************************************************
func: jsonWriteKey1ABI1
******************************************************************************/
JsonBool jsonWriteKey1ABI1(Json * const json, JsonC2 const * const key, JsonABI1 const * const value)
{
   returnFalseIf(!jsonWriteKey(     json, key));
   returnFalseIf(!jsonWriteValueABI(json, value->a, value->b));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABI2
******************************************************************************/
JsonBool jsonWriteKey1ABI2(Json * const json, JsonC2 const * const key, JsonABI2 const * const value)
{
   returnFalseIf(!jsonWriteKey(     json, key));
   returnFalseIf(!jsonWriteValueABI(json, value->a, value->b));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABI4
******************************************************************************/
JsonBool jsonWriteKey1ABI4(Json * const json, JsonC2 const * const key, JsonABI4 const * const value)
{
   returnFalseIf(!jsonWriteKey(     json, key));
   returnFalseIf(!jsonWriteValueABI(json, value->a, value->b));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABN1
******************************************************************************/
JsonBool jsonWriteKey1ABN1(Json * const json, JsonC2 const * const key, JsonABN1 const * const value)
{
   returnFalseIf(!jsonWriteKey(     json, key));
   returnFalseIf(!jsonWriteValueABN(json, value->a, value->b));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABN2
******************************************************************************/
JsonBool jsonWriteKey1ABN2(Json * const json, JsonC2 const * const key, JsonABN2 const * const value)
{
   returnFalseIf(!jsonWriteKey(     json, key));
   returnFalseIf(!jsonWriteValueABN(json, value->a, value->b));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABN4
******************************************************************************/
JsonBool jsonWriteKey1ABN4(Json * const json, JsonC2 const * const key, JsonABN4 const * const value)
{
   returnFalseIf(!jsonWriteKey(     json, key));
   returnFalseIf(!jsonWriteValueABN(json, value->a, value->b));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABR4
******************************************************************************/
JsonBool jsonWriteKey1ABR4(Json * const json, JsonC2 const * const key, JsonABR4 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABR4(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABR8
******************************************************************************/
JsonBool jsonWriteKey1ABR8(Json * const json, JsonC2 const * const key, JsonABR8 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABR8(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCI1
******************************************************************************/
JsonBool jsonWriteKey1ABCI1(Json * const json, JsonC2 const * const key, JsonABCI1 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABCI(json, value->a, value->b, value->c));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCI2
******************************************************************************/
JsonBool jsonWriteKey1ABCI2(Json * const json, JsonC2 const * const key, JsonABCI2 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABCI(json, value->a, value->b, value->c));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCI4
******************************************************************************/
JsonBool jsonWriteKey1ABCI4(Json * const json, JsonC2 const * const key, JsonABCI4 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABCI(json, value->a, value->b, value->c));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCN1
******************************************************************************/
JsonBool jsonWriteKey1ABCN1(Json * const json, JsonC2 const * const key, JsonABCN1 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABCN(json, value->a, value->b, value->c));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCN2
******************************************************************************/
JsonBool jsonWriteKey1ABCN2(Json * const json, JsonC2 const * const key, JsonABCN2 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABCN(json, value->a, value->b, value->c));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCN4
******************************************************************************/
JsonBool jsonWriteKey1ABCN4(Json * const json, JsonC2 const * const key, JsonABCN4 const * const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!jsonWriteValueABCN(json, value->a, value->b, value->c));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCR4
******************************************************************************/
JsonBool jsonWriteKey1ABCR4(Json * const json, JsonC2 const * const key, JsonABCR4 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCR4(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCR8
******************************************************************************/
JsonBool jsonWriteKey1ABCR8(Json * const json, JsonC2 const * const key, JsonABCR8 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCR8(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDI1
******************************************************************************/
JsonBool jsonWriteKey1ABCDI1(Json * const json, JsonC2 const * const key, JsonABCDI1 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCDI(json, value->a, value->b, value->c, value->d));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDI2
******************************************************************************/
JsonBool jsonWriteKey1ABCDI2(Json * const json, JsonC2 const * const key, JsonABCDI2 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCDI(json, value->a, value->b, value->c, value->d));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDI4
******************************************************************************/
JsonBool jsonWriteKey1ABCDI4(Json * const json, JsonC2 const * const key, JsonABCDI4 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCDI(json, value->a, value->b, value->c, value->d));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDN1
******************************************************************************/
JsonBool jsonWriteKey1ABCDN1(Json * const json, JsonC2 const * const key, JsonABCDN1 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCDN(json, value->a, value->b, value->c, value->d));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDN2
******************************************************************************/
JsonBool jsonWriteKey1ABCDN2(Json * const json, JsonC2 const * const key, JsonABCDN2 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCDN(json, value->a, value->b, value->c, value->d));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDN4
******************************************************************************/
JsonBool jsonWriteKey1ABCDN4(Json * const json, JsonC2 const * const key, JsonABCDN4 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteValueABCDN(json, value->a, value->b, value->c, value->d));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDR4
******************************************************************************/
JsonBool jsonWriteKey1ABCDR4(Json * const json, JsonC2 const * const key, JsonABCDR4 const * const value)
{
   returnFalseIf(!jsonWriteKey(        json, key));
   returnFalseIf(!jsonWriteValueABCDR4(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1ABCDR8
******************************************************************************/
JsonBool jsonWriteKey1ABCDR8(Json * const json, JsonC2 const * const key, JsonABCDR8 const * const value)
{
   returnFalseIf(!jsonWriteKey(        json, key));
   returnFalseIf(!jsonWriteValueABCDR8(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Boolean
******************************************************************************/
JsonBool jsonWriteKey1Boolean(Json * const json, JsonC2 const * const key, JsonBool const value)
{
   returnFalseIf(!jsonWriteKey(      json, key));
   returnFalseIf(!_jsonWriteStringC2(json, (value == jsonBoolTRUE) ? L"true" : L"false"));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1I1
******************************************************************************/
JsonBool jsonWriteKey1I1(Json * const json, JsonC2 const * const key, JsonI1 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueI(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1I2
******************************************************************************/
JsonBool jsonWriteKey1I2(Json * const json, JsonC2 const * const key, JsonI2 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueI(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1I4
******************************************************************************/
JsonBool jsonWriteKey1I4(Json * const json, JsonC2 const * const key, JsonI4 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueI(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1I8
******************************************************************************/
JsonBool jsonWriteKey1I8(Json * const json, JsonC2 const * const key, JsonI8 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueI(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Matrix2x2R4
******************************************************************************/
JsonBool jsonWriteKey1Matrix2x2R4(Json * const json, JsonC2 const * const key, JsonMatrix2x2R4 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Matrix2x2R8
******************************************************************************/
JsonBool jsonWriteKey1Matrix2x2R8(Json * const json, JsonC2 const * const key, JsonMatrix2x2R8 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Matrix3x3R4
******************************************************************************/
JsonBool jsonWriteKey1Matrix3x3R4(Json * const json, JsonC2 const * const key, JsonMatrix3x3R4 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Matrix3x3R8
******************************************************************************/
JsonBool jsonWriteKey1Matrix3x3R8(Json * const json, JsonC2 const * const key, JsonMatrix3x3R8 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Matrix4x4R4
******************************************************************************/
JsonBool jsonWriteKey1Matrix4x4R4(Json * const json, JsonC2 const * const key, JsonMatrix4x4R4 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[0][3]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[1][3]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[2][3]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][0]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][1]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][2]));
   returnFalseIf(!jsonWriteValueR4(   json, value->cell[3][3]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1Matrix4x4R8
******************************************************************************/
JsonBool jsonWriteKey1Matrix4x4R8(Json * const json, JsonC2 const * const key, JsonMatrix4x4R8 const * const value)
{
   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[0][3]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[1][3]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[2][3]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][0]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][1]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][2]));
   returnFalseIf(!jsonWriteValueR8(   json, value->cell[3][3]));
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1N1
******************************************************************************/
JsonBool jsonWriteKey1N1(Json * const json, JsonC2 const * const key, JsonN1 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueN(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1N2
******************************************************************************/
JsonBool jsonWriteKey1N2(Json * const json, JsonC2 const * const key, JsonN2 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueN(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1N4
******************************************************************************/
JsonBool jsonWriteKey1N4(Json * const json, JsonC2 const * const key, JsonN4 const value)
{
   returnFalseIf(!jsonWriteKey(   json, key));
   returnFalseIf(!jsonWriteValueN(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey1N8
******************************************************************************/
JsonBool jsonWriteKey1N8(Json * const json, JsonC2 const * const key, JsonN8 const value)
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
func: jsonWriteKey1StringC2
******************************************************************************/
JsonBool jsonWriteKey1StringC2(Json * const json, JsonC2 const * const key, JsonC2 const * const value)
{
   returnFalseIf(!jsonWriteKey(          json, key));
   returnFalseIf(!jsonWriteValueStringC2(json, value));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABI1
******************************************************************************/
JsonBool jsonWriteKeyNABI1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABI1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABI(json, value[index].a, value[index].b));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABI2
******************************************************************************/
JsonBool jsonWriteKeyNABI2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABI2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABI(json, value[index].a, value[index].b));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABI4
******************************************************************************/
JsonBool jsonWriteKeyNABI4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABI4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABI(json, value[index].a, value[index].b));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABN1
******************************************************************************/
JsonBool jsonWriteKeyNABN1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABN1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABN(json, value[index].a, value[index].b));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABN2
******************************************************************************/
JsonBool jsonWriteKeyNABN2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABN2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABN(json, value[index].a, value[index].b));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABN4
******************************************************************************/
JsonBool jsonWriteKeyNABN4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABN4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABN(json, value[index].a, value[index].b));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABR4
******************************************************************************/
JsonBool jsonWriteKeyNABR4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABR4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABR4(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABR8
******************************************************************************/
JsonBool jsonWriteKeyNABR8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABR8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABR8(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCI1
******************************************************************************/
JsonBool jsonWriteKeyNABCI1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCI1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCI(json, value[index].a, value[index].b, value[index].c));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCI2
******************************************************************************/
JsonBool jsonWriteKeyNABCI2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCI2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCI(json, value[index].a, value[index].b, value[index].c));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCI4
******************************************************************************/
JsonBool jsonWriteKeyNABCI4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCI4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCI(json, value[index].a, value[index].b, value[index].c));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCN1
******************************************************************************/
JsonBool jsonWriteKeyNABCN1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCN1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCN(json, value[index].a, value[index].b, value[index].c));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCN2
******************************************************************************/
JsonBool jsonWriteKeyNABCN2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCN2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCN(json, value[index].a, value[index].b, value[index].c));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCN4
******************************************************************************/
JsonBool jsonWriteKeyNABCN4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCN4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCN(json, value[index].a, value[index].b, value[index].c));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCR4
******************************************************************************/
JsonBool jsonWriteKeyNABCR4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCR4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCR4(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCR8
******************************************************************************/
JsonBool jsonWriteKeyNABCR8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCR8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCR8(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDI1
******************************************************************************/
JsonBool jsonWriteKeyNABCDI1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDI1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDI(json, value[index].a, value[index].b, value[index].c, value[index].d));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDI2
******************************************************************************/
JsonBool jsonWriteKeyNABCDI2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDI2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDI(json, value[index].a, value[index].b, value[index].c, value[index].d));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDI4
******************************************************************************/
JsonBool jsonWriteKeyNABCDI4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDI4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDI(json, value[index].a, value[index].b, value[index].c, value[index].d));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDN1
******************************************************************************/
JsonBool jsonWriteKeyNABCDN1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDN1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDN(json, value[index].a, value[index].b, value[index].c, value[index].d));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDN2
******************************************************************************/
JsonBool jsonWriteKeyNABCDN2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDN2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDN(json, value[index].a, value[index].b, value[index].c, value[index].d));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDN4
******************************************************************************/
JsonBool jsonWriteKeyNABCDN4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDN4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDN(json, value[index].a, value[index].b, value[index].c, value[index].d));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDR4
******************************************************************************/
JsonBool jsonWriteKeyNABCDR4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDR4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDR4(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNABCDR8
******************************************************************************/
JsonBool jsonWriteKeyNABCDR8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDR8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueABCDR8(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNBoolean
******************************************************************************/
JsonBool jsonWriteKeyNBoolean(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonBool const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueBoolean(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNI1
******************************************************************************/
JsonBool jsonWriteKeyNI1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonI1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueI(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNI2
******************************************************************************/
JsonBool jsonWriteKeyNI2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonI2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueI(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNI4
******************************************************************************/
JsonBool jsonWriteKeyNI4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonI4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueI(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNI8
******************************************************************************/
JsonBool jsonWriteKeyNI8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonI8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueI(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNMatrix2x2R4
******************************************************************************/
JsonBool jsonWriteKeyNMatrix2x2R4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix2x2R4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueMatrix2x2R4(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNMatrix2x2R8
******************************************************************************/
JsonBool jsonWriteKeyNMatrix2x2R8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix2x2R8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueMatrix2x2R8(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNMatrix3x3R4
******************************************************************************/
JsonBool jsonWriteKeyNMatrix3x3R4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix3x3R4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueMatrix3x3R4(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNMatrix3x3R8
******************************************************************************/
JsonBool jsonWriteKeyNMatrix3x3R8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix3x3R8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueMatrix3x3R8(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNMatrix4x4R4
******************************************************************************/
JsonBool jsonWriteKeyNMatrix4x4R4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix4x4R4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueMatrix4x4R4(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNMatrix4x4R8
******************************************************************************/
JsonBool jsonWriteKeyNMatrix4x4R8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix4x4R8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueMatrix4x4R8(json, &value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNN1
******************************************************************************/
JsonBool jsonWriteKeyNN1(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonN1 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueN(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNN2
******************************************************************************/
JsonBool jsonWriteKeyNN2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonN2 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueN(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNN4
******************************************************************************/
JsonBool jsonWriteKeyNN4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonN4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueN(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNN8
******************************************************************************/
JsonBool jsonWriteKeyNN8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonN8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueN(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNR4
******************************************************************************/
JsonBool jsonWriteKeyNR4(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonR4 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueR4(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNR8
******************************************************************************/
JsonBool jsonWriteKeyNR8(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonR8 const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueR8(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKeyNStringC2
******************************************************************************/
JsonBool jsonWriteKeyNStringC2(Json * const json, JsonC2 const * const key, JsonN4 const count, JsonC2 const * const * const value)
{
   JsonN4 index;

   returnFalseIf(!jsonWriteKey(       json, key));
   returnFalseIf(!jsonWriteArrayStart(json));
   forCount (index, count)
   {
      returnFalseIf(!jsonWriteValueStringC2(json, value[index]));
   }
   returnFalseIf(!jsonWriteArrayStop( json));
   returnTrue;
}
