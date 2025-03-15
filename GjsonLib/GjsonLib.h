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
   gjsonTypeOBJECT_START,
   gjsonTypeARRAY_START,
   gjsonTypeSTRING_START,
   gjsonTypeOBJECT_STOP,
   gjsonTypeARRAY_STOP,
   gjsonTypeKEY_VALUE_SEPARATOR,
   gjsonTypeSEPARATOR,
   gjsonTypeNUMBER_INTEGER,
   gjsonTypeNUMBER_NATURAL,
   gjsonTypeNUMBER_REAL,
   gjsonTypeCONSTANT_FALSE,
   gjsonTypeCONSTANT_NULL,
   gjsonTypeCONSTANT_TRUE,

   // Error returns
   gjsonTypeERROR_UNEXPECTED_CHAR          = 100,
   gjsonTypeERROR_CONSTANT_FALSE_EXPECTED,
   gjsonTypeERROR_CONSTANT_NULL_EXPECTED,
   gjsonTypeERROR_CONSTANT_TRUE_EXPECTED,
   gjsonTypeERROR_NUMBER_EXPECTED,
   gjsonTypeERROR_NUMBER_REAL_EXPECTED

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
   Gstr                    *key;

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
Gjson          *gjsonClocReader(                                    GgetBuffer getBufferFunc, void * const dataRepo);
Gb              gjsonClocReaderContent(   Gjson       * const json, GgetBuffer getBufferFunc, void * const dataRepo);
Gjson          *gjsonClocWriter(                                    GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted);
Gb              gjsonClocWriterContent(   Gjson       * const json, GsetBuffer setBufferFunc, void * const dataRepo, Gb const isFormatted);

void            gjsonDloc(                Gjson       * const json);
void            gjsonDlocContent(         Gjson       * const json);

GjsonType       gjsonGetTypeElem(         Gjson       * const json);
GjsonType       gjsonGetTypeFile(         Gjson       * const json);
GjsonType       gjsonGetTypeObj(          Gjson       * const json);

Gb              gjsonGetI(                Gjson       * const json,                        Gi8  *  const value);
Gb              gjsonGetKey(              Gjson       * const json,                        Gstr ** const key);
Gb              gjsonGetN(                Gjson       * const json,                        Gn8  *  const value);
Gb              gjsonGetR(                Gjson       * const json,                        Gr8  *  const value);
Gb              gjsonGetR4(               Gjson       * const json,                        Gr4  *  const value);
Gb              gjsonGetStr(              Gjson       * const json, Gcount const maxCount, Gstr *  const value);
GjsonStrLetter  gjsonGetStrBinByte(       Gjson       * const json,                        Gn1  *  const value);
GjsonStrLetter  gjsonGetStrLetter(        Gjson       * const json,                        Gstr *  const value);
Gb              gjsonGetStrHex(           Gjson       * const json, Gstr * const h1, Gstr * const h2, Gstr * const h3, Gstr * const h4);

Gb              gjsonSetArrayStart(       Gjson       * const json);
Gb              gjsonSetArrayStop(        Gjson       * const json);
Gb              gjsonSetKey(              Gjson       * const json, Gstr const * const key);
Gb              gjsonSetObjectStart(      Gjson       * const json);
Gb              gjsonSetObjectStop(       Gjson       * const json);
Gb              gjsonSetSeparator(        Gjson       * const json);
Gb              gjsonSetValueBin(         Gjson       * const json, Gcount const count, Gn1 const *  const value);
Gb              gjsonSetValueBool(        Gjson       * const json,                     Gb           const value);
Gb              gjsonSetValueI(           Gjson       * const json,                     Gi8          const value);
Gb              gjsonSetValueN(           Gjson       * const json,                     Gn8          const value);
Gb              gjsonSetValueNull(        Gjson       * const json);
Gb              gjsonSetValueR(           Gjson       * const json,                     Gr8          const value);
Gb              gjsonSetValueR4(          Gjson       * const json,                     Gr4          const value);
Gb              gjsonSetValueStr(         Gjson       * const json,                     Gstr const * const value);
Gb              gjsonSetValueStrBinByte(  Gjson       * const json,                     Gn1          const value);
Gb              gjsonSetValueStrLetter(   Gjson       * const json,                     Gstr         const value);
Gb              gjsonSetValueStrStart(    Gjson       * const json);
Gb              gjsonSetValueStrStop(     Gjson       * const json);
Gb              gjsonStart(               GmemCloc const memCreate, GmemDloc const memDestroy);
void            gjsonStop(                void);


#endif
