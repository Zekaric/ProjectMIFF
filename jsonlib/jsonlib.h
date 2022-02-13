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

/******************************************************************************
type:
******************************************************************************/
// Integer types
typedef char                             JsonI1;
typedef short                            JsonI2;
typedef int                              JsonI4;
typedef int64_t                          JsonI8;
// Natural types
typedef unsigned char                    JsonN1;
typedef unsigned short                   JsonN2;
typedef int                              JsonN4;
typedef uint64_t                         JsonN8;
// Real types
typedef float                            JsonR4;
typedef double                           JsonR8;
// String char types
// C1 = UTF8
typedef JsonN1                           JsonC1;
// C2 = UTF16
typedef wchar_t                          JsonC2;
// C4 = UTF32
typedef JsonN4                           JsonC4;

#pragma pack(1)
typedef struct { JsonI1   a, b;        } JsonABI1;
typedef struct { JsonN1   a, b;        } JsonABN1;
typedef struct { JsonI2   a, b;        } JsonABI2;
typedef struct { JsonN2   a, b;        } JsonABN2;
typedef struct { JsonI4   a, b;        } JsonABI4;
typedef struct { JsonN4   a, b;        } JsonABN4;
typedef struct { JsonR4   a, b;        } JsonABR4;
typedef struct { JsonI8   a, b;        } JsonABI8;
typedef struct { JsonN8   a, b;        } JsonABN8;
typedef struct { JsonR8   a, b;        } JsonABR8;

typedef struct { JsonI1   a, b, c;     } JsonABCI1;
typedef struct { JsonN1   a, b, c;     } JsonABCN1;
typedef struct { JsonI2   a, b, c;     } JsonABCI2;
typedef struct { JsonN2   a, b, c;     } JsonABCN2;
typedef struct { JsonI4   a, b, c;     } JsonABCI4;
typedef struct { JsonN4   a, b, c;     } JsonABCN4;
typedef struct { JsonR4   a, b, c;     } JsonABCR4;
typedef struct { JsonI8   a, b, c;     } JsonABCI8;
typedef struct { JsonN8   a, b, c;     } JsonABCN8;
typedef struct { JsonR8   a, b, c;     } JsonABCR8;

typedef struct { JsonI1   a, b, c, d;  } JsonABCDI1;
typedef struct { JsonN1   a, b, c, d;  } JsonABCDN1;
typedef struct { JsonI2   a, b, c, d;  } JsonABCDI2;
typedef struct { JsonN2   a, b, c, d;  } JsonABCDN2;
typedef struct { JsonI4   a, b, c, d;  } JsonABCDI4;
typedef struct { JsonN4   a, b, c, d;  } JsonABCDN4;
typedef struct { JsonR4   a, b, c, d;  } JsonABCDR4;
typedef struct { JsonI8   a, b, c, d;  } JsonABCDI8;
typedef struct { JsonN8   a, b, c, d;  } JsonABCDN8;
typedef struct { JsonR8   a, b, c, d;  } JsonABCDR8;

typedef struct { JsonR4   cell[2][2];  } JsonMatrix2x2R4;
typedef struct { JsonR8   cell[2][2];  } JsonMatrix2x2R8;

typedef struct { JsonR4   cell[3][3];  } JsonMatrix3x3R4;
typedef struct { JsonR8   cell[3][3];  } JsonMatrix3x3R8;

typedef struct { JsonR4   cell[4][4];  } JsonMatrix4x4R4;
typedef struct { JsonR8   cell[4][4];  } JsonMatrix4x4R8;

typedef union
{
   JsonI1             i;
   JsonN1             n;
   JsonN1             byte[1];
} Json1;

typedef union
{
   JsonI2             i;
   JsonN2             n;
   JsonABI1           abi;
   JsonABN1           abn;
   JsonN1             byte[2];
} Json2;

typedef union
{
   JsonABCI1          abci;
   JsonABCN1          abcn;
   JsonN1             byte[3];
} Json3;

typedef union
{
   JsonI4             i;
   JsonN4             n;
   JsonR4             r;
   JsonABI2           abi;
   JsonABN2           abn;
   JsonABCDI1         abcdi;
   JsonABCDN1         abcdn;
   JsonN1             byte[4];
} Json4;

typedef union
{
   JsonABCI2          abci;
   JsonABCN2          abcn;
   JsonN1             byte[6];
} Json6;

typedef union
{
   JsonI8             i;
   JsonN8             n;
   JsonR8             r;
   JsonABI4           abi;
   JsonABN4           abn;
   JsonABR4           abr;
   JsonABCDI2         abcdi;
   JsonABCDN2         abcdn;
   JsonN1             byte[8];
} Json8;

typedef union
{
   JsonABCI4          abci;
   JsonABCN4          abcn;
   JsonABCR4          abcr;
   JsonN1             byte[12];
} Json12;

typedef struct
{
   JsonABR8           abr;
   JsonABCDI4         abcdi;
   JsonABCDN4         abcdn;
   JsonABCDR4         abcdr;
   JsonMatrix2x2R4    matrix2x2r;
   JsonN1             byte[16];
} Json16;

typedef struct
{
   JsonABCR8          abcr;
   JsonN1             byte[24];
} Json24;

typedef struct
{
   JsonABCDR8         abcdr;
   JsonMatrix2x2R8    matrix2x2r;
   JsonN1             byte[32];
} Json32;

typedef struct
{
   JsonMatrix3x3R4    matrix3x3r;
   JsonN1             byte[36];
} Json36;

typedef struct
{
   JsonMatrix4x4R4    matrix4x4r;
   JsonN1             byte[64];
} Json64;

typedef struct
{
   JsonMatrix3x3R8    matrix3x3r;
   JsonN1             byte[72];
} Json72;

typedef struct
{
   JsonMatrix4x4R8    matrix4x4r;
   JsonN1             byte[128];
} Json128;

typedef struct
{
   JsonN1             byte[256];
} Json256;
#pragma pack()

typedef void    *(*JsonMemCreate)(        JsonN4 const memByteCount);
typedef void     (*JsonMemDestroy)(       void * const mem);

typedef JsonBool (*JsonGetBuffer)(        void * const dataRepo, JsonN4 const byteCount, JsonN1       * const data);

typedef JsonBool (*JsonSetBuffer)(        void * const dataRepo, JsonN4 const byteCount, JsonN1 const * const data);

typedef struct
{
   // JSON file information and configuration.
   JsonN8                      version;
   JsonMethod                  method;
   JsonI4                      scope;
   JsonBool                    isIndentMissing;
   JsonBool                    isLastWriteValue;

   // Data repo getters and setters.
   void                       *dataRepo;
   JsonGetBuffer               getBuffer;
   JsonSetBuffer               setBuffer;
} Json;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Json           *jsonCreateReader(                                           JsonGetBuffer getBufferFunc, void * const dataRepo);
JsonBool        jsonCreateReaderContent(           Json       * const json, JsonGetBuffer getBufferFunc, void * const dataRepo);
Json           *jsonCreateWriter(                                           JsonSetBuffer setBufferFunc, void * const dataRepo);
JsonBool        jsonCreateWriterContent(           Json       * const json, JsonSetBuffer setBufferFunc, void * const dataRepo);

void            jsonDestroy(                       Json       * const json);
void            jsonDestroyContent(                Json       * const json);

JsonBool        jsonWriteArrayStart(               Json       * const json);
JsonBool        jsonWriteArrayStop(                Json       * const json);
JsonBool        jsonWriteKey(                      Json       * const json, JsonC2 const * const key);
JsonBool        jsonWriteObjectStart(              Json       * const json);
JsonBool        jsonWriteObjectStop(               Json       * const json);
JsonBool        jsonWriteSeparator(                Json       * const json);
JsonBool        jsonWriteValueI(                   Json       * const json, JsonI8 const value);
JsonBool        jsonWriteValueN(                   Json       * const json, JsonN8 const value);
JsonBool        jsonWriteValueR4(                  Json       * const json, JsonR4 const value);
JsonBool        jsonWriteValueR8(                  Json       * const json, JsonR8 const value);
JsonBool        jsonWriteValueString(              Json       * const json, JsonC2 const * const value);

JsonBool        jsonStart(                         JsonMemCreate const memCreate, JsonMemDestroy const memDestroy);
void            jsonStop(                          void);

// Helper functions
JsonBool        jsonWriteKey1Boolean(              Json       * const json, JsonC2 const * const key, JsonBool const value);
JsonBool        jsonWriteKey1I(                    Json       * const json, JsonC2 const * const key, JsonI8 const value);
JsonBool        jsonWriteKey1N(                    Json       * const json, JsonC2 const * const key, JsonN8 const value);
JsonBool        jsonWriteKey1R4(                   Json       * const json, JsonC2 const * const key, JsonR4 const value);
JsonBool        jsonWriteKey1R8(                   Json       * const json, JsonC2 const * const key, JsonR8 const value);
JsonBool        jsonWriteKey1String(               Json       * const json, JsonC2 const * const key, JsonC2 const * const value);

JsonBool        jsonWriteObject1Boolean(           Json       * const json, JsonC2 const * const key, JsonBool const value);
JsonBool        jsonWriteObject1I(                 Json       * const json, JsonC2 const * const key, JsonI8 const value);
JsonBool        jsonWriteObject1N(                 Json       * const json, JsonC2 const * const key, JsonN8 const value);
JsonBool        jsonWriteObject1R4(                Json       * const json, JsonC2 const * const key, JsonR4 const value);
JsonBool        jsonWriteObject1R8(                Json       * const json, JsonC2 const * const key, JsonR8 const value);
JsonBool        jsonWriteObject1String(            Json       * const json, JsonC2 const * const key, JsonC2 const * const value);

#endif
