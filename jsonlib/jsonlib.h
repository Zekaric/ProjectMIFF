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
Json           *jsonCreateReader(                                                JsonGetBuffer getBufferFunc, void * const dataRepo);
JsonBool        jsonCreateReaderContent(                Json       * const json, JsonGetBuffer getBufferFunc, void * const dataRepo);
Json           *jsonCreateWriter(                                                JsonSetBuffer setBufferFunc, void * const dataRepo);
JsonBool        jsonCreateWriterContent(                Json       * const json, JsonSetBuffer setBufferFunc, void * const dataRepo);
                                                        
void            jsonDestroy(                            Json       * const json);
void            jsonDestroyContent(                     Json       * const json);
                                                        
JsonBool        jsonStart(                              JsonMemCreate const memCreate, JsonMemDestroy const memDestroy);
void            jsonStop(                               void);
                                                        
JsonBool        jsonWriteArrayStart(                    Json       * const json);
JsonBool        jsonWriteArrayStop(                     Json       * const json);
JsonBool        jsonWriteKey(                           Json       * const json, JsonC2 const * const key);
JsonBool        jsonWriteObjectStart(                   Json       * const json);
JsonBool        jsonWriteObjectStop(                    Json       * const json);
JsonBool        jsonWriteSeparator(                     Json       * const json);
JsonBool        jsonWriteValueABI(                      Json       * const json, JsonI8                  const va, JsonI8 const vb);
JsonBool        jsonWriteValueABN(                      Json       * const json, JsonN8                  const va, JsonN8 const vb);
JsonBool        jsonWriteValueABR4(                     Json       * const json, JsonABR4        const * const value);
JsonBool        jsonWriteValueABR8(                     Json       * const json, JsonABR8        const * const value);
JsonBool        jsonWriteValueABCI(                     Json       * const json, JsonI8                  const va, JsonI8 const vb, JsonI8 const vc);
JsonBool        jsonWriteValueABCN(                     Json       * const json, JsonN8                  const va, JsonN8 const vb, JsonN8 const vc);
JsonBool        jsonWriteValueABCR4(                    Json       * const json, JsonABCR4       const * const value);
JsonBool        jsonWriteValueABCR8(                    Json       * const json, JsonABCR8       const * const value);
JsonBool        jsonWriteValueABCDI(                    Json       * const json, JsonI8                  const va, JsonI8 const vb, JsonI8 const vc, JsonI8 const vd);
JsonBool        jsonWriteValueABCDN(                    Json       * const json, JsonN8                  const va, JsonN8 const vb, JsonN8 const vc, JsonN8 const vd);
JsonBool        jsonWriteValueABCDR4(                   Json       * const json, JsonABCDR4      const * const value);
JsonBool        jsonWriteValueABCDR8(                   Json       * const json, JsonABCDR8      const * const value);
JsonBool        jsonWriteValueBoolean(                  Json       * const json, JsonBool                const value);
JsonBool        jsonWriteValueI(                        Json       * const json, JsonI8                  const value);
JsonBool        jsonWriteValueN(                        Json       * const json, JsonN8                  const value);
JsonBool        jsonWriteMatrix2x2R4(                   Json       * const json, JsonMatrix2x2R4 const * const value);
JsonBool        jsonWriteMatrix2x2R8(                   Json       * const json, JsonMatrix2x2R8 const * const value);
JsonBool        jsonWriteMatrix3x3R4(                   Json       * const json, JsonMatrix3x3R4 const * const value);
JsonBool        jsonWriteMatrix3x3R8(                   Json       * const json, JsonMatrix3x3R8 const * const value);
JsonBool        jsonWriteMatrix4x4R4(                   Json       * const json, JsonMatrix4x4R4 const * const value);
JsonBool        jsonWriteMatrix4x4R8(                   Json       * const json, JsonMatrix4x4R8 const * const value);
JsonBool        jsonWriteValueR4(                       Json       * const json, JsonR4                  const value);
JsonBool        jsonWriteValueR8(                       Json       * const json, JsonR8                  const value);
JsonBool        jsonWriteValueStringC2(                 Json       * const json, JsonC2          const * const value);

// Helper functions
JsonBool        jsonWriteKey1ABI1(                      Json       * const json, JsonC2 const * const key,                     JsonABI1        const * const value);
JsonBool        jsonWriteKey1ABI2(                      Json       * const json, JsonC2 const * const key,                     JsonABI2        const * const value);
JsonBool        jsonWriteKey1ABI4(                      Json       * const json, JsonC2 const * const key,                     JsonABI4        const * const value);
JsonBool        jsonWriteKey1ABN1(                      Json       * const json, JsonC2 const * const key,                     JsonABN1        const * const value);
JsonBool        jsonWriteKey1ABN2(                      Json       * const json, JsonC2 const * const key,                     JsonABN2        const * const value);
JsonBool        jsonWriteKey1ABN4(                      Json       * const json, JsonC2 const * const key,                     JsonABN4        const * const value);
JsonBool        jsonWriteKey1ABR4(                      Json       * const json, JsonC2 const * const key,                     JsonABR4        const * const value);
JsonBool        jsonWriteKey1ABR8(                      Json       * const json, JsonC2 const * const key,                     JsonABR8        const * const value);
JsonBool        jsonWriteKey1ABCI1(                     Json       * const json, JsonC2 const * const key,                     JsonABCI1       const * const value);
JsonBool        jsonWriteKey1ABCI2(                     Json       * const json, JsonC2 const * const key,                     JsonABCI2       const * const value);
JsonBool        jsonWriteKey1ABCI4(                     Json       * const json, JsonC2 const * const key,                     JsonABCI4       const * const value);
JsonBool        jsonWriteKey1ABCN1(                     Json       * const json, JsonC2 const * const key,                     JsonABCN1       const * const value);
JsonBool        jsonWriteKey1ABCN2(                     Json       * const json, JsonC2 const * const key,                     JsonABCN2       const * const value);
JsonBool        jsonWriteKey1ABCN4(                     Json       * const json, JsonC2 const * const key,                     JsonABCN4       const * const value);
JsonBool        jsonWriteKey1ABCR4(                     Json       * const json, JsonC2 const * const key,                     JsonABCR4       const * const value);
JsonBool        jsonWriteKey1ABCR8(                     Json       * const json, JsonC2 const * const key,                     JsonABCR8       const * const value);
JsonBool        jsonWriteKey1ABCDI1(                    Json       * const json, JsonC2 const * const key,                     JsonABCDI1      const * const value);
JsonBool        jsonWriteKey1ABCDI2(                    Json       * const json, JsonC2 const * const key,                     JsonABCDI2      const * const value);
JsonBool        jsonWriteKey1ABCDI4(                    Json       * const json, JsonC2 const * const key,                     JsonABCDI4      const * const value);
JsonBool        jsonWriteKey1ABCDN1(                    Json       * const json, JsonC2 const * const key,                     JsonABCDN1      const * const value);
JsonBool        jsonWriteKey1ABCDN2(                    Json       * const json, JsonC2 const * const key,                     JsonABCDN2      const * const value);
JsonBool        jsonWriteKey1ABCDN4(                    Json       * const json, JsonC2 const * const key,                     JsonABCDN4      const * const value);
JsonBool        jsonWriteKey1ABCDR4(                    Json       * const json, JsonC2 const * const key,                     JsonABCDR4      const * const value);
JsonBool        jsonWriteKey1ABCDR8(                    Json       * const json, JsonC2 const * const key,                     JsonABCDR8      const * const value);
JsonBool        jsonWriteKey1Boolean(                   Json       * const json, JsonC2 const * const key,                     JsonBool                const value);
JsonBool        jsonWriteKey1I1(                        Json       * const json, JsonC2 const * const key,                     JsonI1                  const value);
JsonBool        jsonWriteKey1I2(                        Json       * const json, JsonC2 const * const key,                     JsonI2                  const value);
JsonBool        jsonWriteKey1I4(                        Json       * const json, JsonC2 const * const key,                     JsonI4                  const value);
JsonBool        jsonWriteKey1I8(                        Json       * const json, JsonC2 const * const key,                     JsonI8                  const value);
JsonBool        jsonWriteKey1Matrix2x2R4(               Json       * const json, JsonC2 const * const key,                     JsonMatrix2x2R4 const * const value);
JsonBool        jsonWriteKey1Matrix2x2R8(               Json       * const json, JsonC2 const * const key,                     JsonMatrix2x2R8 const * const value);
JsonBool        jsonWriteKey1Matrix3x3R4(               Json       * const json, JsonC2 const * const key,                     JsonMatrix3x3R4 const * const value);
JsonBool        jsonWriteKey1Matrix3x3R8(               Json       * const json, JsonC2 const * const key,                     JsonMatrix3x3R8 const * const value);
JsonBool        jsonWriteKey1Matrix4x4R4(               Json       * const json, JsonC2 const * const key,                     JsonMatrix4x4R4 const * const value);
JsonBool        jsonWriteKey1Matrix4x4R8(               Json       * const json, JsonC2 const * const key,                     JsonMatrix4x4R8 const * const value);
JsonBool        jsonWriteKey1N1(                        Json       * const json, JsonC2 const * const key,                     JsonN1                  const value);
JsonBool        jsonWriteKey1N2(                        Json       * const json, JsonC2 const * const key,                     JsonN2                  const value);
JsonBool        jsonWriteKey1N4(                        Json       * const json, JsonC2 const * const key,                     JsonN4                  const value);
JsonBool        jsonWriteKey1N8(                        Json       * const json, JsonC2 const * const key,                     JsonN8                  const value);
JsonBool        jsonWriteKey1R4(                        Json       * const json, JsonC2 const * const key,                     JsonR4                  const value);
JsonBool        jsonWriteKey1R8(                        Json       * const json, JsonC2 const * const key,                     JsonR8                  const value);
JsonBool        jsonWriteKey1StringC2(                  Json       * const json, JsonC2 const * const key,                     JsonC2          const * const value);

JsonBool        jsonWriteKeyNABI1(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABI1        const * const value);
JsonBool        jsonWriteKeyNABI2(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABI2        const * const value);
JsonBool        jsonWriteKeyNABI4(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABI4        const * const value);
JsonBool        jsonWriteKeyNABN1(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABN1        const * const value);
JsonBool        jsonWriteKeyNABN2(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABN2        const * const value);
JsonBool        jsonWriteKeyNABN4(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABN4        const * const value);
JsonBool        jsonWriteKeyNABR4(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABR4        const * const value);
JsonBool        jsonWriteKeyNABR8(                      Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABR8        const * const value);
JsonBool        jsonWriteKeyNABCI1(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCI1       const * const value);
JsonBool        jsonWriteKeyNABCI2(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCI2       const * const value);
JsonBool        jsonWriteKeyNABCI4(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCI4       const * const value);
JsonBool        jsonWriteKeyNABCN1(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCN1       const * const value);
JsonBool        jsonWriteKeyNABCN2(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCN2       const * const value);
JsonBool        jsonWriteKeyNABCN4(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCN4       const * const value);
JsonBool        jsonWriteKeyNABCR4(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCR4       const * const value);
JsonBool        jsonWriteKeyNABCR8(                     Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCR8       const * const value);
JsonBool        jsonWriteKeyNABCDI1(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDI1      const * const value);
JsonBool        jsonWriteKeyNABCDI2(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDI2      const * const value);
JsonBool        jsonWriteKeyNABCDI4(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDI4      const * const value);
JsonBool        jsonWriteKeyNABCDN1(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDN1      const * const value);
JsonBool        jsonWriteKeyNABCDN2(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDN2      const * const value);
JsonBool        jsonWriteKeyNABCDN4(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDN4      const * const value);
JsonBool        jsonWriteKeyNABCDR4(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDR4      const * const value);
JsonBool        jsonWriteKeyNABCDR8(                    Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonABCDR8      const * const value);
JsonBool        jsonWriteKeyNBoolean(                   Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonBool        const * const value);
JsonBool        jsonWriteKeyNI1(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonI1          const * const value);
JsonBool        jsonWriteKeyNI2(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonI2          const * const value);
JsonBool        jsonWriteKeyNI4(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonI4          const * const value);
JsonBool        jsonWriteKeyNI8(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonI8          const * const value);
JsonBool        jsonWriteKeyNMatrix2x2R4(               Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix2x2R4 const * const value);
JsonBool        jsonWriteKeyNMatrix2x2R8(               Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix2x2R8 const * const value);
JsonBool        jsonWriteKeyNMatrix3x3R4(               Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix3x3R4 const * const value);
JsonBool        jsonWriteKeyNMatrix3x3R8(               Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix3x3R8 const * const value);
JsonBool        jsonWriteKeyNMatrix4x4R4(               Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix4x4R4 const * const value);
JsonBool        jsonWriteKeyNMatrix4x4R8(               Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonMatrix4x4R8 const * const value);
JsonBool        jsonWriteKeyNN1(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonN1          const * const value);
JsonBool        jsonWriteKeyNN2(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonN2          const * const value);
JsonBool        jsonWriteKeyNN4(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonN4          const * const value);
JsonBool        jsonWriteKeyNN8(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonN8          const * const value);
JsonBool        jsonWriteKeyNR4(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonR4          const * const value);
JsonBool        jsonWriteKeyNR8(                        Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonR8          const * const value);
JsonBool        jsonWriteKeyNStringC2(                  Json       * const json, JsonC2 const * const key, JsonN4 const count, JsonC2  const * const * const value);

#endif
