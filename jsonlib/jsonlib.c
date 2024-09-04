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
   json->readBinCountActual   = 1024;
   json->readStrCountActual   = 1024;
   json->readBinData          = _JsonMemCreateTypeArray(json->readBinCountActual, JsonN1);
   json->readStrData          = _JsonMemCreateTypeArray(json->readStrCountActual, JsonStr);
   returnFalseIf(!json->readBinData);
   returnFalseIf(!json->readStrData);

   returnFalseIf(!json->readBinData);

   returnTrue;
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

   _JsonMemDestroy(json->readBinData);
   _JsonMemDestroy(json->readStrData);
   _JsonMemDestroy(json);
}

/******************************************************************************
func: jsonRead
******************************************************************************/
JsonB jsonRead(Json * const json, JsonReadType * const type)
{
#if 0
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
         breakIf(!json->getBuffer(json->dataRepo, 1, &byte));

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
         breakIf(!json->getBuffer(json->dataRepo, 1, &byte));

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
         breakIf(!json->getBuffer(json->dataRepo, 1, &byte));

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
      if (json->readBinCountActual == index)
      {
         // Create a new buffer double the size.
         bufTemp = _JsonMemCreateTypeArray(json->readBinCountActual * 2, JsonN1);
         returnFalseIf(!bufTemp);

         // Copy over the olde buffer to the new buffer.
         _JsonMemCopyTypeArray(json->readBinCountActual, JsonN1, bufTemp, json->readBinData);
         // Destroy the old buffer.
         _JsonMemDestroy(json->readBinData);

         // Reset the internal buffer to the new larger buffer.
         json->readBinCountActual = json->readBinCountActual * 2;
         json->readBinData        = (JsonStr *) bufTemp;
      }

      // End of file?
      breakIf(!json->getBuffer(json->dataRepo, 1, &byte));
      // End of line or part, we done reading.
      breakIf(byte == '\n' ||
              byte == '\t');

      // Add the letter to the byte array.
      json->readBinData[index++] = byte;
   }

   // Need NULL terminator.
   json->readBinCount       = index;
   json->readBinData[index] = 0;

   if (byte == '\n')
   {
      //json->isRecordDone = jsonTRUE;
   }
#endif
   returnTrue;
}

/******************************************************************************
func: jsonReadKey
******************************************************************************/
JsonB jsonReadKey(Json * const json, JsonStr ** const key)
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
JsonB jsonReadI(Json * const json, JsonI * const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadN
******************************************************************************/
JsonB jsonReadN(Json * const json, JsonN *  const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadR
******************************************************************************/
JsonB jsonReadR(Json * const json, JsonR *  const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadR4
******************************************************************************/
JsonB jsonReadR4(Json * const json, JsonR4 *  const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadStr
******************************************************************************/
JsonB jsonReadStr(Json * const json, JsonStr ** const value)
{
   returnTrue;
}

/******************************************************************************
func: jsonReadStringC2Letter
******************************************************************************/
JsonB jsonReadStringC2Letter(Json * const json, JsonStr *  const value)
{
   returnTrue;
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

   returnFalseIf(!_JsonGetStart());

   _isStarted = jsonTRUE;

   returnTrue;
}

/******************************************************************************
func: jsonStop
******************************************************************************/
void jsonStop(void)
{
   returnVoidIf(!_isStarted);

   _JsonGetStop();
   _JsonMemStop();

   _isStarted = jsonFALSE;
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

   returnTrue;
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
   returnTrue;
}

/******************************************************************************
func: jsonSetKey
******************************************************************************/
JsonB jsonSetKey(Json * const json, JsonStr const * const key)
{
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !key);

   returnFalseIf(!jsonSetSeparator(json));
   
   json->isFirstItem = jsonFALSE;

   returnFalseIf(!jsonSetValueStr(json, key));
   returnFalseIf(!_JsonSetBuffer( json, 1, (JsonN1 *) jsonKEY_VALUE_SEPARATOR_STR));
   returnTrue;
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

   returnTrue;
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
   returnTrue;
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
   returnTrue;
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
   returnFalseIf(
      !_isStarted ||
      !json       ||
      !str);

   if (json->scopeType[json->scope - 1] == jsonScopeARRAY)
   {
      returnFalseIf(!jsonSetSeparator(json));
   }
   json->isFirstItem = jsonFALSE;
   
   return _JsonSetStr(json, strlen(str), str);
}
