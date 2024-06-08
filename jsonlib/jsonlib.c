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
   json->method              = jsonMethodREADING;
   json->dataRepo            = dataRepo;
   json->getBuffer           = getBufferFunc;
   json->readByteCount       = 1024;
   json->readByteCountActual = 1024;
   json->readByteData        = _JsonMemCreateTypeArray(json->readByteCount, JsonStr);

   returnFalseIf(!json->readByteData);

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
   json->method    = jsonMethodWRITING;
   json->dataRepo  = dataRepo;
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

   _JsonMemDestroy(json->readByteData);
   _JsonMemDestroy(json);
}

/******************************************************************************
func: jsonRead
******************************************************************************/
JsonBool jsonRead(Json * const json, JsonReadType * const type)
{
   JsonN4  index;
   JsonN1  byte;
   JsonN1 *bufTemp;

   returnFalseIf(
      !_isStarted ||
      !json       ||
      !type);

   // Start of a Json object.
   if (json->readState == jsonReadStateEXPECTING_OBJECT_START)
   {
      loop
      {
         breakIf(!json->getBuffer(json->dataRepo, 1, (JsonStr *) &byte));

         // Skip over white space.
         continueIf(jsonIS_SPACE(byte));
         
         // Found the object start.
         if (byte == jsonOBJECT_START_STR[0])
         {
            json->readState                = jsonReadStateEXPECTING_KEY_OR_OBJECT_STOP;
            json->scopeType[json->scope++] = jsonScopeOBJECT;
            json->scope++;

            *type = jsonReadTypeOBJECT_START;
            returnTrue;
         }

         // Anything else is an error.
         break;
      }

      *type = jsonReadTypeNONE;
      returnFalse;
   }

   // Inside an object scope.  Looking for a key or a close object scope.
   if (json->readState == jsonReadStateEXPECTING_KEY_OR_OBJECT_STOP)
   {
      loop
      {
         breakIf(!json->getBuffer(json->dataRepo, 1, (JsonStr *) &byte));

         // Skip over white space.
         continueIf(jsonIS_SPACE(byte));

         // Found the object stop
         if (byte == jsonOBJECT_STOP_STR[0])
         {
            json->scope--;
            json->scopeType[json->scope] = jsonScopeNONE;

            if (json->scope)
            {
               if      (json->scopeType[json->scope - 1] == jsonScopeARRAY)
               {
                  json->readState = jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY_OR_ARRAY_STOP;
               }
               else if (json->scopeType[json->scope - 1] == jsonScopeOBJECT)
               {
                  json->readState = jsonReadStateEXPECTING_KEY_OR_OBJECT_STOP;
               }
               else
               {
                  // Should never happening.  Butt flapping.
                  break;
               }
            }
            else
            {
               json->readState = jsonReadStateEXPECTING_OBJECT_START;
            }
            
            *type = jsonReadTypeOBJECT_STOP;
            returnTrue;
         }

         // Found a comma so there is another key value.
         if (byte == jsonSEPARATOR_STR[0])
         {
            // Read in the key value.
            //_JsonReadStr(json, &json->key);

            *type = jsonReadTypeKEY;
            returnTrue;
         }
      }

      *type = jsonReadTypeNONE;
      returnFalse;
   }

   // Getting a value inside an array.
   if (json->readState == jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY_OR_ARRAY_STOP)
   {
      loop
      {
         breakIf(!json->getBuffer(json->dataRepo, 1, (JsonStr *) &byte));

         // Skip over white space.
         continueIf(jsonIS_SPACE(byte));

         // Found the array stop
         if (byte == jsonARRAY_STOP_STR[0])
         {
            json->scope--;
            json->scopeType[json->scope] = jsonScopeNONE;

            if      (json->scopeType[json->scope - 1] == jsonScopeARRAY)
            {
               json->readState = jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY_OR_ARRAY_STOP;
            }
            else if (json->scopeType[json->scope - 1] == jsonScopeOBJECT)
            {
               json->readState = jsonReadStateEXPECTING_KEY_OR_OBJECT_STOP;
            }
            else
            {
               // Should never happening.  Butt flapping.
               break;
            }

            *type = jsonReadTypeARRAY_STOP;
            returnTrue;
         }

         // Found a comma so there is another value.
         if (byte == jsonSEPARATOR_STR[0])
         {
            *type = jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY_OR_ARRAY_STOP;
         }
      }

      *type = jsonReadTypeNONE;
      returnFalse;
   }


   index = 0;
   loop
   {
      // Resize the internal buffer when we have exhausted it.
      if (json->readByteCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _JsonMemCreateTypeArray(json->readByteCountActual * 2, JsonN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _JsonMemCopyTypeArray(json->readByteCountActual, JsonN1, bufTemp, json->readByteData);
         // Destroy the old buffer.
         _JsonMemDestroy(json->readByteData);

         // Reset the internal buffer to the new larger buffer.
         json->readByteCountActual = json->readByteCountActual * 2;
         json->readByteData        = (JsonStr *) bufTemp;
      }

      // End of file?
      breakIf(!json->getBuffer(json->dataRepo, 1, (JsonStr *) &byte));
      // End of line or part, we done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      json->readByteData[index++] = byte;
   }

   // Need NULL terminator.
   json->readByteCount       = index;
   json->readByteData[index] = 0;

   if (byte == '\n')
   {
      //json->readRecordIsDone = jsonBoolTRUE;
   }

   returnTrue;
}

/******************************************************************************
func: jsonReadKey
******************************************************************************/
JsonBool jsonReadKey(Json * const json, JsonStr ** const key)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !key);

   *key = json->key;

   returnTrue;
}

/******************************************************************************
func: jsonReadI
******************************************************************************/
JsonBool jsonReadI(Json * const json, JsonI8 * const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadN
******************************************************************************/
JsonBool jsonReadN(Json * const json, JsonN8 *  const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadR4
******************************************************************************/
JsonBool jsonReadR4(Json * const json, JsonR4 *  const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadR8
******************************************************************************/
JsonBool jsonReadR8(Json * const json, JsonR8 *  const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadStr
******************************************************************************/
JsonBool jsonReadStr(Json * const json, JsonStr ** const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadStringC2Letter
******************************************************************************/
JsonBool jsonReadStringC2Letter(Json * const json, JsonStr *  const value)
{
   returnTrue;
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

   returnFalseIf(!_JsonReadStart());

   _isStarted = jsonBoolTRUE;

   returnTrue;
}

/******************************************************************************
func: jsonStop
******************************************************************************/
void jsonStop(void)
{
   returnVoidIf(!_isStarted);

   _JsonReadStop();
   _JsonMemStop();

   _isStarted = jsonBoolFALSE;
}

/******************************************************************************
func: jsonWriteArrayStart
******************************************************************************/
JsonBool jsonWriteArrayStart(Json * const json)
{
   json->scopeType[json->scope] = jsonScopeARRAY;
   json->scope++;
   json->isFirstItem            = jsonBoolTRUE;

   returnFalseIf(!_JsonWriteStr(json, jsonARRAY_START_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteArrayStop
******************************************************************************/
JsonBool jsonWriteArrayStop(Json * const json)
{
   json->scope--;
   json->scopeType[json->scope] = jsonScopeNONE;
   json->isFirstItem            = jsonBoolFALSE;

   returnFalseIf(!_JsonWriteStr(    json, "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnFalseIf(!_JsonWriteStr(    json, jsonARRAY_STOP_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteKey
******************************************************************************/
JsonBool jsonWriteKey(Json * const json, JsonStr const * const key)
{
   returnFalseIf(!jsonWriteSeparator(     json));
   returnFalseIf(!_JsonWriteStr(          json, "\n"));
   returnFalseIf(!_JsonWriteIndent(       json));
   
   json->isFirstItem = jsonBoolFALSE;

   //returnFalseIf(!jsonWriteValueStr(      json, key));
   returnFalseIf(!_JsonWriteStr(          json, jsonKEY_VALUE_SEPARATOR_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObjectStart
******************************************************************************/
JsonBool jsonWriteObjectStart(Json * const json)
{
   json->scopeType[json->scope] = jsonScopeOBJECT;
   json->scope++;
   json->isFirstItem            = jsonBoolTRUE;

   returnFalseIf(!_JsonWriteStr(json, jsonOBJECT_START_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteObjectStop
******************************************************************************/
JsonBool jsonWriteObjectStop(Json * const json)
{
   json->scope--;
   json->scopeType[json->scope] = jsonScopeNONE;
   json->isFirstItem            = jsonBoolFALSE;

   returnFalseIf(!_JsonWriteStr(    json, "\n"));
   returnFalseIf(!_JsonWriteIndent(json));
   returnFalseIf(!_JsonWriteStr(    json, jsonOBJECT_STOP_STR));
   returnTrue;
}

/******************************************************************************
func: jsonWriteSeparator
******************************************************************************/
JsonBool jsonWriteSeparator(Json * const json)
{
   if (!json->isFirstItem)
   {
      returnFalseIf(!_JsonWriteStr(json, jsonSEPARATOR_STR));
   }
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueBoolean
******************************************************************************/
JsonBool jsonWriteValueBoolean(Json * const json, JsonBool const value)
{
   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonWriteSeparator(    json));
      returnFalseIf(!_JsonWriteStr(          json, "\n"));
      returnFalseIf(!_JsonWriteIndent(      json));
   }
   json->isFirstItem = jsonBoolFALSE;
   
   returnFalseIf(!_JsonWriteStr(json, ((value == jsonBoolTRUE) ? "\"true\"" : "\"false\"")));
   returnTrue;
}

/******************************************************************************
func: jsonWriteValueI
******************************************************************************/
JsonBool jsonWriteValueI(Json * const json, JsonI8 const value)
{
   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonWriteSeparator(    json));
      returnFalseIf(!_JsonWriteStr(          json, "\n"));
      returnFalseIf(!_JsonWriteIndent(      json));
   }
   json->isFirstItem = jsonBoolFALSE;
   
   return _JsonWriteI(json, value);
}

/******************************************************************************
func: jsonWriteValueN
******************************************************************************/
JsonBool jsonWriteValueN(Json * const json, JsonN8 const value)
{
   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonWriteSeparator(    json));
      returnFalseIf(!_JsonWriteStr(          json, "\n"));
      returnFalseIf(!_JsonWriteIndent(      json));
   }
   json->isFirstItem = jsonBoolFALSE;
   
   return _JsonWriteN(json, value);
}

/******************************************************************************
func: jsonWriteValueR4
******************************************************************************/
JsonBool jsonWriteValueR4(Json * const json, JsonR4 const value)
{
   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonWriteSeparator(    json));
      returnFalseIf(!_JsonWriteStr(          json, "\n"));
      returnFalseIf(!_JsonWriteIndent(      json));
   }
   json->isFirstItem = jsonBoolFALSE;
   
   return _JsonWriteR4(json, value);
}

/******************************************************************************
func: jsonWriteValueR8
******************************************************************************/
JsonBool jsonWriteValueR8(Json * const json, JsonR8 const value)
{
   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonWriteSeparator(    json));
      returnFalseIf(!_JsonWriteStr(          json, "\n"));
      returnFalseIf(!_JsonWriteIndent(      json));
   }
   json->isFirstItem = jsonBoolFALSE;
   
   return _JsonWriteR8(json, value);
}
