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
   jsonFALSE,
   jsonTRUE
} JsonB;

typedef enum
{
   jsonStrLetterNORMAL,
   jsonStrLetterHEX,
   jsonStrLetterDONE,
   jsonStrLetterERROR
} JsonStrLetter;

typedef enum
{
   jsonMethodREADING,
   jsonMethodWRITING_COMPACT,
   jsonMethodWRITING_INDENTED,
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
   jsonTypeNONE,

   // Reading the first json file value 
   jsonTypeOBJECT_START,
   jsonTypeARRAY_START,
   jsonTypeSTRING_START,
   jsonTypeOBJECT_STOP,
   jsonTypeARRAY_STOP,
   jsonTypeKEY_VALUE_SEPARATOR,
   jsonTypeSEPARATOR,
   jsonTypeNUMBER_INTEGER,
   jsonTypeNUMBER_NATURAL,
   jsonTypeNUMBER_REAL,
   jsonTypeCONSTANT_FALSE,
   jsonTypeCONSTANT_NULL,
   jsonTypeCONSTANT_TRUE,

   // Error returns
   jsonTypeERROR_UNEXPECTED_CHAR          = 100,
   jsonTypeERROR_CONSTANT_FALSE_EXPECTED,
   jsonTypeERROR_CONSTANT_NULL_EXPECTED,
   jsonTypeERROR_CONSTANT_TRUE_EXPECTED,
   jsonTypeERROR_NUMBER_EXPECTED,
   jsonTypeERROR_NUMBER_REAL_EXPECTED

} JsonType;

/******************************************************************************
type:
******************************************************************************/
// Integer types
typedef int64_t                           JsonI;
typedef int32_t                           JsonI4;
// Natural types
typedef uint64_t                          JsonN;
typedef uint32_t                          JsonN4;
typedef uint8_t                           JsonN1;
// Real types
typedef double                            JsonR;
typedef float                             JsonR4;
// String (UTF8) types
typedef char                              JsonStr;

#define JsonN_MAX                         UINT64_MAX
#define JsonN4_MAX                        UINT32_MAX
#define JsonI_MAX                         INT64_MAX
#define jsonI_MIN                         INT64_MIN
#define JsonI4_MAX                        INT32_MAX

typedef void *(*JsonMemCreate)(        JsonN4 const memByteCount);
typedef void  (*JsonMemDestroy)(       void * const mem);

typedef JsonB (*JsonGetBuffer)(        void * const dataSource,      JsonN4 const byteCount, JsonN1       * const data);

typedef JsonB (*JsonSetBuffer)(        void * const dataDestination, JsonN4 const byteCount, JsonN1 const * const data);

typedef struct
{
   JsonType                    type;
   JsonN                       n;
   JsonI                       i;
   JsonR                       r;
   JsonR4                      r4;
} JsonValue;

typedef struct
{
   // JSON file information and configuration.
   JsonN                       version;
   JsonMethod                  method;
   JsonI4                      scope;
   JsonScope                   scopeType[1024];
   JsonB                       isFirstItem;
   JsonStr                    *key;

   // Data repo getters and setters.
   void                       *dataRepo;
   JsonGetBuffer               getBuffer;
   JsonSetBuffer               setBuffer;
   JsonN1                      lastByte;

   // Read value
   JsonValue                   value;
   JsonStr                     hex[4];
} Json;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Json           *jsonCreateReader(                                  JsonGetBuffer getBufferFunc, void * const dataRepo);
JsonB           jsonCreateReaderContent(  Json       * const json, JsonGetBuffer getBufferFunc, void * const dataRepo);
Json           *jsonCreateWriter(                                  JsonSetBuffer setBufferFunc, void * const dataRepo, JsonB const isFormatted);
JsonB           jsonCreateWriterContent(  Json       * const json, JsonSetBuffer setBufferFunc, void * const dataRepo, JsonB const isFormatted);

void            jsonDestroy(              Json       * const json);
void            jsonDestroyContent(       Json       * const json);

JsonType        jsonGetTypeElem(          Json       * const json);
JsonType        jsonGetTypeFile(          Json       * const json);
JsonType        jsonGetTypeObj(           Json       * const json);

JsonB           jsonGetKey(               Json       * const json, JsonStr ** const key);
JsonB           jsonGetI(                 Json       * const json, JsonI *  const value);
JsonB           jsonGetN(                 Json       * const json, JsonN *  const value);
JsonB           jsonGetR(                 Json       * const json, JsonR *  const value);
JsonB           jsonGetR4(                Json       * const json, JsonR4 *  const value);
JsonB           jsonGetStr(               Json       * const json, JsonI4 const maxCount, JsonStr *value);
JsonStrLetter   jsonGetStrLetter(         Json       * const json, JsonStr * const value);
JsonB           jsonGetStrHex(            Json       * const json, JsonStr * const h1, JsonStr * const h2, JsonStr * const h3, JsonStr * const h4);

JsonB           jsonSetArrayStart(        Json       * const json);
JsonB           jsonSetArrayStop(         Json       * const json);
JsonB           jsonSetKey(               Json       * const json, JsonStr const * const key);
JsonB           jsonSetObjectStart(       Json       * const json);
JsonB           jsonSetObjectStop(        Json       * const json);
JsonB           jsonSetSeparator(         Json       * const json);
JsonB           jsonSetValueBool(         Json       * const json, JsonB           const value);
JsonB           jsonSetValueI(            Json       * const json, JsonI           const value);
JsonB           jsonSetValueN(            Json       * const json, JsonN           const value);
JsonB           jsonSetValueNull(         Json       * const json);
JsonB           jsonSetValueR(            Json       * const json, JsonR           const value);
JsonB           jsonSetValueR4(           Json       * const json, JsonR4          const value);
JsonB           jsonSetValueStr(          Json       * const json, JsonStr const * const value);
JsonB           jsonSetValueStrStart(     Json       * const json);
JsonB           jsonSetValueStrLetter(    Json       * const json, JsonStr         const value);
JsonB           jsonSetValueStrStop(      Json       * const json);
JsonB           jsonStart(                JsonMemCreate const memCreate, JsonMemDestroy const memDestroy);
void            jsonStop(                 void);


#endif
