/**************************************************************************************************
file:       gjsonlib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Library for dealing with raw JSON files.

Used jscon.org for information about reading format.
**************************************************************************************************/

/**************************************************************************************************
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
**************************************************************************************************/

#if !defined(GJSONLIB_H)
#define      GJSONLIB_H

/**************************************************************************************************
include:
**************************************************************************************************/
#include <stdint.h>

#include "GrlMiffLib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/
#define gjsonCountDEFAULT          4096

typedef enum
{
   gjsonStrLetterNORMAL,
   gjsonStrLetterHEX,
   gjsonStrLetterDONE,
   gjsonStrLetterERROR
} GjsonStrLetter;

typedef enum
{
   gjsonScopeTypeNONE,

   gjsonScopeTypeOBJECT,
   gjsonScopeTypeARRAY,

   gjsonScopeTypeCOUNT
} GjsonScopeType;

typedef enum
{
   gjsonTypeNONE,

   // Reading the first json file value
   gjsonTypeARRAY_START,
   gjsonTypeARRAY_STOP,

   gjsonTypeOBJECT_KEY,
   gjsonTypeOBJECT_START,
   gjsonTypeOBJECT_STOP,

   gjsonTypeSEPARATOR,
   gjsonTypeARRAY_SEPARATOR  = gjsonTypeSEPARATOR,
   gjsonTypeOBJECT_SEPARATOR = gjsonTypeSEPARATOR,

   gjsonTypeVALUE_STRING_START,
   gjsonTypeVALUE_NUMBER_INTEGER,
   gjsonTypeVALUE_NUMBER_NATURAL,
   gjsonTypeVALUE_NUMBER_REAL,
   gjsonTypeVALUE_FALSE,
   gjsonTypeVALUE_NULL,
   gjsonTypeVALUE_TRUE,

   // Error returns
   gjsonTypeERROR_UNEXPECTED_CHAR          = 100,
   gjsonTypeERROR_CONSTANT_FALSE_EXPECTED,
   gjsonTypeERROR_CONSTANT_NULL_EXPECTED,
   gjsonTypeERROR_CONSTANT_TRUE_EXPECTED,
   gjsonTypeERROR_NUMBER_EXPECTED,
   gjsonTypeERROR_NUMBER_REAL_EXPECTED,

   // Internal
   gjsonTypeINTERNAL_KEY_VALUE_SEPARATOR
} GjsonType;

/**************************************************************************************************
type:
**************************************************************************************************/
typedef struct
{
   GjsonType                type;
   Gn8                      n;
   Gi8                      i;
   Gr8                      r;
   Gr4                      r4;
} GjsonValue;

typedef struct
{
   GjsonScopeType           type;
   Gmethod                  method;
} GjsonScope;

typedef struct
{
   // JSON file information and configuration.
   Gn8                      version;
   Gmethod                  method;
   Gindex                   scope;
   GjsonScope               scopeType[1024];
   Gb                       isFirstItem;
   Gstr                     key[gjsonCountDEFAULT + 1];

   // Data repo getters and setters.
   void                    *dataRepo;
   GgetBuffer               getBuffer;
   GsetBuffer               setBuffer;
   Gn1                      lastByte;

   // Read value
   GjsonValue               value;
   Gstr                     hex[4];
} Gjson;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/
Gjson          *gjsonClocReader(                                          GgetBuffer getBufferFunc, void * const dataRepo);
Gb              gjsonClocReaderContent(         Gjson       * const json, GgetBuffer getBufferFunc, void * const dataRepo);
Gjson          *gjsonClocWriter(                                          GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted);
Gb              gjsonClocWriterContent(         Gjson       * const json, GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted);

void            gjsonDloc(                      Gjson       * const json);
void            gjsonDlocContent(               Gjson       * const json);

GjsonType       gjsonGetType_ArrayValueOrStop(  Gjson       * const json);
GjsonType       gjsonGetType_FileElement(       Gjson       * const json);
GjsonType       gjsonGetType_ObjectKeyOrStop(   Gjson       * const json);
GjsonType       gjsonGetType_ObjectValue(       Gjson       * const json);

Gb              gjsonGetI(                      Gjson       * const json,                        Gi8  *  const value);
Gstr const     *gjsonGetKey(                    Gjson       * const json);
Gb              gjsonGetN(                      Gjson       * const json,                        Gn8  *  const value);
Gb              gjsonGetR(                      Gjson       * const json,                        Gr8  *  const value);
Gb              gjsonGetR4(                     Gjson       * const json,                        Gr4  *  const value);
Gb              gjsonGetStr(                    Gjson       * const json, Gcount const maxCount, Gstr *  const value);
GjsonStrLetter  gjsonGetStrBinByte(             Gjson       * const json,                        Gn1  *  const value);
GjsonStrLetter  gjsonGetStrLetter(              Gjson       * const json,                        Gstr *  const value);
Gb              gjsonGetStrHex(                 Gjson       * const json, Gstr * const h1, Gstr * const h2, Gstr * const h3, Gstr * const h4);

Gb              gjsonIsKeyEqual(                Gjson const * const json, Gstr const * const value);
Gb              gjsonIsTypeBin(                 GjsonType const type);
Gb              gjsonIsTypeI(                   GjsonType const type);
Gb              gjsonIsTypeN(                   GjsonType const type);
Gb              gjsonIsTypeR(                   GjsonType const type);
Gb              gjsonIsTypeStr(                 GjsonType const type);

Gb              gjsonSetArrayStart(             Gjson       * const json);
Gb              gjsonSetArrayStop(              Gjson       * const json);
Gb              gjsonSetKey(                    Gjson       * const json, Gstr const * const key);
Gb              gjsonSetObjectStart(            Gjson       * const json);
Gb              gjsonSetObjectStop(             Gjson       * const json);
Gb              gjsonSetSeparator(              Gjson       * const json);
Gb              gjsonSetValueBin(               Gjson       * const json, Gcount const count, Gn1 const *  const value);
Gb              gjsonSetValueBinByte(           Gjson       * const json,                     Gn1          const value);
Gb              gjsonSetValueBinStart(          Gjson       * const json);
Gb              gjsonSetValueBinStop(           Gjson       * const json);
Gb              gjsonSetValueB(                 Gjson       * const json,                     Gb           const value);
Gb              gjsonSetValueI(                 Gjson       * const json,                     Gi8          const value);
Gb              gjsonSetValueN(                 Gjson       * const json,                     Gn8          const value);
Gb              gjsonSetValueNull(              Gjson       * const json);
Gb              gjsonSetValueR(                 Gjson       * const json,                     Gr8          const value);
Gb              gjsonSetValueR4(                Gjson       * const json,                     Gr4          const value);
Gb              gjsonSetValueStr(               Gjson       * const json,                     Gstr const * const value);
Gb              gjsonSetValueStrLetter(         Gjson       * const json,                     Gstr         const value);
Gb              gjsonSetValueStrStart(          Gjson       * const json);
Gb              gjsonSetValueStrStop(           Gjson       * const json);
Gb              gjsonStart(                     GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void            gjsonStop(                      void);

#define gjsonSetArrayValueArrayStart(   JSON)                  gjsonSetArrayStart(     JSON)
#define gjsonSetArrayValueArrayStop(    JSON)                  gjsonSetArrayStop(      JSON)
#define gjsonSetArrayValueB(            JSON,       VALUE)     gjsonSetValueB(         JSON,       VALUE)
#define gjsonSetArrayValueBin(          JSON, SIZE, VALUE)     gjsonSetValueBin(       JSON, SIZE, VALUE)
#define gjsonSetArrayValueBinByte(      JSON,       VALUE)     gjsonSetValueBinByte(   JSON,       VALUE)
#define gjsonSetArrayValueBinStart(     JSON)                  gjsonSetValueBinStart(  JSON)
#define gjsonSetArrayValueBinStop(      JSON)                  gjsonSetValueBinStop(   JSON)
#define gjsonSetArrayValueI(            JSON,       VALUE)     gjsonSetValueI(         JSON,       VALUE)
#define gjsonSetArrayValueN(            JSON,       VALUE)     gjsonSetValueN(         JSON,       VALUE)
#define gjsonSetArrayValueNull(         JSON)                  gjsonSetValueNull(      JSON)
#define gjsonSetArrayValueObjectStart(  JSON)                  gjsonSetObjectStart(    JSON)
#define gjsonSetArrayValueObjectStop(   JSON)                  gjsonSetObjectStop(     JSON)
#define gjsonSetArrayValueR(            JSON,       VALUE)     gjsonSetValueR(         JSON,       VALUE)
#define gjsonSetArrayValueR4(           JSON,       VALUE)     gjsonSetValueR4(        JSON,       VALUE)
#define gjsonSetArrayValueStr(          JSON,       VALUE)     gjsonSetValueStr(       JSON,       VALUE)
#define gjsonSetArrayValueStrLetter(    JSON,       VALUE)     gjsonSetValueStrLetter( JSON,       VALUE)
#define gjsonSetArrayValueStrStart(     JSON)                  gjsonSetValueStrStart(  JSON)
#define gjsonSetArrayValueStrStop(      JSON)                  gjsonSetValueStrStop(   JSON)

#define gjsonSetFileValueArrayStart(   JSON)                   gjsonSetArrayStart(     JSON)
#define gjsonSetFileValueArrayStop(    JSON)                   gjsonSetArrayStop(      JSON)
#define gjsonSetFileValueB(            JSON,       VALUE)      gjsonSetValueB(         JSON,       VALUE)
#define gjsonSetFileValueBin(          JSON, SIZE, VALUE)      gjsonSetValueBin(       JSON, SIZE, VALUE)
#define gjsonSetFileValueBinByte(      JSON,       VALUE)      gjsonSetValueBinByte(   JSON,       VALUE)
#define gjsonSetFileValueBinStart(     JSON)                   gjsonSetValueBinStart(  JSON)
#define gjsonSetFileValueBinStop(      JSON)                   gjsonSetValueBinStop(   JSON)
#define gjsonSetFileValueI(            JSON,       VALUE)      gjsonSetValueI(         JSON,       VALUE)
#define gjsonSetFileValueN(            JSON,       VALUE)      gjsonSetValueN(         JSON,       VALUE)
#define gjsonSetFileValueNull(         JSON)                   gjsonSetValueNull(      JSON)
#define gjsonSetFileValueObjectStart(  JSON)                   gjsonSetObjectStart(    JSON)
#define gjsonSetFileValueObjectStop(   JSON)                   gjsonSetObjectStop(     JSON)
#define gjsonSetFileValueR(            JSON,       VALUE)      gjsonSetValueR(         JSON,       VALUE)
#define gjsonSetFileValueR4(           JSON,       VALUE)      gjsonSetValueR4(        JSON,       VALUE)
#define gjsonSetFileValueStr(          JSON,       VALUE)      gjsonSetValueStr(       JSON,       VALUE)
#define gjsonSetFileValueStrLetter(    JSON,       VALUE)      gjsonSetValueStrLetter( JSON,       VALUE)
#define gjsonSetFileValueStrStart(     JSON)                   gjsonSetValueStrStart(  JSON)
#define gjsonSetFileValueStrStop(      JSON)                   gjsonSetValueStrStop(   JSON)

#define gjsonSetObjectValueArrayStart( JSON, KEY)              (gjsonSetKey(           JSON, KEY) && gjsonSetArrayStart(   JSON))
#define gjsonSetObjectValueArrayStop(  JSON)                   gjsonSetArrayStop(      JSON)
#define gjsonSetObjectValueB(          JSON, KEY,       VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueB(       JSON,       VALUE))
#define gjsonSetObjectValueBin(        JSON, KEY, SIZE, VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueBin(     JSON, SIZE, VALUE))
#define gjsonSetObjectValueBinByte(    JSON,            VALUE) gjsonSetValueBinByte(   JSON,       VALUE)
#define gjsonSetObjectValueBinStart(   JSON, KEY)              (gjsonSetKey(           JSON, KEY) && gjsonSetValueBinStart(JSON))
#define gjsonSetObjectValueBinStop(    JSON)                   gjsonSetValueBinStop(   JSON)
#define gjsonSetObjectValueI(          JSON, KEY,       VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueI(       JSON,       VALUE))
#define gjsonSetObjectValueN(          JSON, KEY,       VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueN(       JSON,       VALUE))
#define gjsonSetObjectValueNull(       JSON, KEY)              (gjsonSetKey(           JSON, KEY) && gjsonSetValueNull(    JSON))
#define gjsonSetObjectValueObjectStart(JSON, KEY)              (gjsonSetKey(           JSON, KEY) && gjsonSetObjectStart(  JSON))
#define gjsonSetObjectValueObjectStop( JSON)                   gjsonSetObjectStop(     JSON)
#define gjsonSetObjectValueR(          JSON, KEY,       VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueR(       JSON,       VALUE))
#define gjsonSetObjectValueR4(         JSON, KEY,       VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueR4(      JSON,       VALUE))
#define gjsonSetObjectValueStr(        JSON, KEY,       VALUE) (gjsonSetKey(           JSON, KEY) && gjsonSetValueStr(     JSON,       VALUE))
#define gjsonSetObjectValueStrLetter(  JSON,            VALUE) gjsonSetValueStr(       JSON,       VALUE)
#define gjsonSetObjectValueStrStart(   JSON, KEY)              (gjsonSetKey(           JSON, KEY) && gjsonSetValueStrStart(JSON))
#define gjsonSetObjectValueStrStop(    JSON)                   gjsonSetValueStrStop(   JSON)

#endif
