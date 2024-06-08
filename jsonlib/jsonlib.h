/******************************************************************************
file:       JsonLib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw JSON files.

Used jscon.org for information about reading format.
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

#if !defined(JSONLIB_H)
#define      JSONLIB_H

/******************************************************************************
include:
******************************************************************************/
#include <stdint.h>

/******************************************************************************
local:
constant:
******************************************************************************/
#define jsonKeySIZE                       257

typedef enum
{
   jsonBoolFALSE,
   jsonBoolTRUE
} JsonBool;

typedef enum
{
   jsonMethodREADING,
   jsonMethodWRITING,
} JsonMethod;

typedef enum
{
   jsonScopeNONE,

   jsonScopeOBJECT,
   jsonScopeARRAY,

   jsonScopeCOUNT
} JsonScope;

typedef enum
{
   jsonReadTypeNONE,
   jsonReadTypeOBJECT_START,
   jsonReadTypeOBJECT_STOP,
   jsonReadTypeARRAY_START,
   jsonReadTypeARRAY_STOP,
   jsonReadTypeKEY,
   jsonReadTypeVALUE_STRING,
   jsonReadTypeVALUE_NUMBER_I_N,
   jsonReadTypeVALUE_NUMBER_R
} JsonReadType;

typedef enum
{
   jsonReadStateEXPECTING_OBJECT_START,
   jsonReadStateEXPECTING_KEY_OR_OBJECT_STOP,
   jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY_OR_ARRAY_STOP,
   jsonReadStateEXPECTING_VALUE_OBJECT_ARRAY
} JsonReadState;

/******************************************************************************
type:
******************************************************************************/
// Integer types
typedef char                             JsonI1;
typedef short                            JsonI2;
typedef int                              JsonI4;
typedef int64_t                          JsonI8;
typedef JsonI8                           JsonI;
// Natural types
typedef unsigned char                    JsonN1;
typedef unsigned short                   JsonN2;
typedef int                              JsonN4;
typedef uint64_t                         JsonN8;
typedef JsonN8                           JsonN;
// Real types
typedef float                            JsonR4;
typedef double                           JsonR8;
// String char types
// C1 = UTF8
typedef char                             JsonStr;

typedef void    *(*JsonMemCreate)(        JsonN4 const memByteCount);
typedef void     (*JsonMemDestroy)(       void * const mem);

typedef JsonBool (*JsonGetBuffer)(        void * const dataRepo, JsonN4 const byteCount, JsonStr       * const data);

typedef JsonBool (*JsonSetBuffer)(        void * const dataRepo, JsonN4 const byteCount, JsonStr const * const data);

typedef struct
{
   // JSON file information and configuration.
   JsonN8                      version;
   JsonMethod                  method;
   JsonI4                      scope;
   JsonScope                   scopeType[256];
   JsonBool                    isFirstItem;
   JsonStr                    *key;

   // Data repo getters and setters.
   void                       *dataRepo;
   JsonGetBuffer               getBuffer;
   JsonSetBuffer               setBuffer;

   // Read value
   JsonReadState               readState;
   JsonN4                      readByteCountActual;
   JsonN4                      readByteCount;
   JsonStr                    *readByteData;
} Json;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Json           *jsonCreateReader(                                  JsonGetBuffer getBufferFunc, void * const dataRepo);
JsonBool        jsonCreateReaderContent(  Json       * const json, JsonGetBuffer getBufferFunc, void * const dataRepo);
Json           *jsonCreateWriter(                                  JsonSetBuffer setBufferFunc, void * const dataRepo);
JsonBool        jsonCreateWriterContent(  Json       * const json, JsonSetBuffer setBufferFunc, void * const dataRepo);

void            jsonDestroy(              Json       * const json);
void            jsonDestroyContent(       Json       * const json);

JsonBool        jsonRead(                 Json       * const json, JsonReadType * const type);
JsonBool        jsonReadKey(              Json       * const json, JsonStr ** const key);
JsonBool        jsonReadI(                Json       * const json, JsonI8 *  const value);
JsonBool        jsonReadN(                Json       * const json, JsonN8 *  const value);
JsonBool        jsonReadR4(               Json       * const json, JsonR4 *  const value);
JsonBool        jsonReadR8(               Json       * const json, JsonR8 *  const value);
JsonBool        jsonReadStr(         Json       * const json, JsonStr ** const value);
JsonBool        jsonReadStringC2Letter(   Json       * const json, JsonStr *  const value);

JsonBool        jsonStart(                JsonMemCreate const memCreate, JsonMemDestroy const memDestroy);
void            jsonStop(                 void);

JsonBool        jsonWriteArrayStart(      Json       * const json);
JsonBool        jsonWriteArrayStop(       Json       * const json);
JsonBool        jsonWriteKey(             Json       * const json, JsonStr const * const key);
JsonBool        jsonWriteObjectStart(     Json       * const json);
JsonBool        jsonWriteObjectStop(      Json       * const json);
JsonBool        jsonWriteSeparator(       Json       * const json);
JsonBool        jsonWriteValueBoolean(    Json       * const json, JsonBool                const value);
JsonBool        jsonWriteValueI(          Json       * const json, JsonI8                  const value);
JsonBool        jsonWriteValueN(          Json       * const json, JsonN8                  const value);
JsonBool        jsonWriteValueR4(         Json       * const json, JsonR4                  const value);
JsonBool        jsonWriteValueR8(         Json       * const json, JsonR8                  const value);
JsonBool        jsonWriteValueStr(        Json       * const json, JsonStr         const * const value);

#endif
