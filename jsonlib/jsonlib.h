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

#include "b64lib.h"

/******************************************************************************
local:
constant:
******************************************************************************/
typedef enum
{
   jsonStrLetterNORMAL,
   jsonStrLetterHEX,
   jsonStrLetterDONE,
   jsonStrLetterERROR
} GjsonStrLetter;

typedef enum
{
   jsonScopeTypeNONE,

   jsonScopeTypeOBJECT,
   jsonScopeTypeARRAY,

   jsonScopeTypeCOUNT
} GjsonScopeType;

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

} GjsonType;

/******************************************************************************
type:
******************************************************************************/
typedef struct
{
   GjsonType                    type;
   Gn8                       n;
   Gi8                       i;
   Gr8                       r;
   Gr4                      r4;
} GjsonValue;

typedef struct
{
   GjsonScopeType               type;
   Gmethod                  method;
} GjsonScope;

typedef struct
{
   // JSON file information and configuration.
   Gn8                       version;
   Gmethod                  method;
   Gi4                      scope;
   GjsonScope                   scopeType[1024];
   Gb                       isFirstItem;
   Gstr                    *key;

   // Data repo getters and setters.
   void                       *dataRepo;
   GgetBuffer               getBuffer;
   GsetBuffer               setBuffer;
   Gn1                      lastByte;

   // Read value
   GjsonValue                   value;
   Gstr                     hex[4];
} Gjson;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Gjson           *jsonCreateReader(                                  GgetBuffer getBufferFunc, void * const dataRepo);
Gb           jsonCreateReaderContent(  Gjson       * const json, GgetBuffer getBufferFunc, void * const dataRepo);
Gjson           *jsonCreateWriter(                                  GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted);
Gb           jsonCreateWriterContent(  Gjson       * const json, GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted);

void            jsonDestroy(              Gjson       * const json);
void            jsonDestroyContent(       Gjson       * const json);

GjsonType        jsonGetTypeElem(          Gjson       * const json);
GjsonType        jsonGetTypeFile(          Gjson       * const json);
GjsonType        jsonGetTypeObj(           Gjson       * const json);

Gb           jsonGetI(                 Gjson       * const json, Gi8 *  const value);
Gb           jsonGetKey(               Gjson       * const json, Gstr ** const key);
Gb           jsonGetN(                 Gjson       * const json, Gn8 *  const value);
Gb           jsonGetR(                 Gjson       * const json, Gr8 *  const value);
Gb           jsonGetR4(                Gjson       * const json, Gr4 *  const value);
Gb           jsonGetStr(               Gjson       * const json, Gi4 const maxCount, Gstr *value);
GjsonStrLetter   jsonGetStrBinByte(        Gjson       * const json, Gn1 * const value);
GjsonStrLetter   jsonGetStrLetter(         Gjson       * const json, Gstr * const value);
Gb           jsonGetStrHex(            Gjson       * const json, Gstr * const h1, Gstr * const h2, Gstr * const h3, Gstr * const h4);

Gb           jsonSetArrayStart(        Gjson       * const json);
Gb           jsonSetArrayStop(         Gjson       * const json);
Gb           jsonSetKey(               Gjson       * const json, Gstr const * const key);
Gb           jsonSetObjectStart(       Gjson       * const json);
Gb           jsonSetObjectStop(        Gjson       * const json);
Gb           jsonSetSeparator(         Gjson       * const json);
Gb           jsonSetValueBin(          Gjson       * const json, Gn8 const count, Gn1 const * const value);
Gb           jsonSetValueBool(         Gjson       * const json, Gb           const value);
Gb           jsonSetValueI(            Gjson       * const json, Gi8           const value);
Gb           jsonSetValueN(            Gjson       * const json, Gn8           const value);
Gb           jsonSetValueNull(         Gjson       * const json);
Gb           jsonSetValueR(            Gjson       * const json, Gr8           const value);
Gb           jsonSetValueR4(           Gjson       * const json, Gr4          const value);
Gb           jsonSetValueStr(          Gjson       * const json, Gstr const * const value);
Gb           jsonSetValueStrBinByte(   Gjson       * const json, Gn1 const value);
Gb           jsonSetValueStrLetter(    Gjson       * const json, Gstr         const value);
Gb           jsonSetValueStrStart(     Gjson       * const json);
Gb           jsonSetValueStrStop(      Gjson       * const json);
Gb           jsonStart(                GmemCloc const memCreate, GmemDloc const memDestroy);
void            jsonStop(                 void);


#endif
