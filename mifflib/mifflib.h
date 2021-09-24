/******************************************************************************
file:       MiffLib
author:     Robbert de Groot
copyright:  Robbert de Groot, 2021

description:
Library for dealing with raw MIFF files.
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

#if !defined(MIFFLIB_H)
#define      MIFFLIB_H

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
   miffBoolFALSE,
   miffBoolTRUE
} MiffBool;

typedef enum
{
   miffMethodREADING,
   miffMethodWRITING,
} MiffMethod;

typedef enum
{
   miffModeTEXT,
   miffModeBINARY
} MiffMode;

typedef enum
{
   miffTypeNONE,

   miffTypeKEY_VALUE_BLOCK_STOP,
   miffTypeKEY_VALUE_BLOCK_START,
   miffTypeTYPE,
   miffTypeSTRING                       = 5,
   miffTypeVARIABLE                     = 7,
   miffTypeBOOLEAN                      = 8,
   miffTypeI1                           = 10,
   miffTypeI2,
   miffTypeI4,
   miffTypeI8,
   miffTypeI16,
   miffTypeI32,
   miffTypeI64,
   miffTypeI128,
   miffTypeI256,
   miffTypeN1                           = 20,
   miffTypeN2,
   miffTypeN4,
   miffTypeN8,
   miffTypeN16,
   miffTypeN32,
   miffTypeN64,
   miffTypeN128,
   miffTypeN256,
   miffTypeR4                           = 32,
   miffTypeR8,
   miffTypeABI1                         = 40,
   miffTypeABI2,
   miffTypeABI4,
   miffTypeABI8,
   miffTypeABN1                         = 50,
   miffTypeABN2,
   miffTypeABN4,
   miffTypeABN8,
   miffTypeABR4                         = 62,
   miffTypeABR8,
   miffTypeABCI1                        = 70,
   miffTypeABCI2,
   miffTypeABCI4,
   miffTypeABCI8,
   miffTypeABCN1                        = 80,
   miffTypeABCN2,
   miffTypeABCN4,
   miffTypeABCN8,
   miffTypeABCR4                        = 92,
   miffTypeABCR8,
   miffTypeABCDI1                       = 100,
   miffTypeABCDI2,
   miffTypeABCDI4,
   miffTypeABCDI8,
   miffTypeABCDN1                       = 110,
   miffTypeABCDN2,
   miffTypeABCDN4,
   miffTypeABCDN8,
   miffTypeABCDR4                       = 122,
   miffTypeABCDR8,
   miffTypeMATRIX2X2R4                  = 132,
   miffTypeMATRIX2X2R8,
   miffTypeMATRIX3X3R4                  = 142,
   miffTypeMATRIX3X3R8,
   miffTypeMATRIX4X4R4                  = 152,
   miffTypeMATRIX4X4R8,

   miffTypeCOUNT
} MiffType;

#define miffArrayCountUNKNOWN                ((MiffN4) 0xFFFFFFFF)
#define miffArrayCountUNKNOWN_C2             L"*"

/******************************************************************************
type:
******************************************************************************/
// Integer types
typedef char                             MiffI1;
typedef short                            MiffI2;
typedef int                              MiffI4;
typedef int64_t                          MiffI8;
// Natural types
typedef unsigned char                    MiffN1;
typedef unsigned short                   MiffN2;
typedef int                              MiffN4;
typedef uint64_t                         MiffN8;
// Real types
typedef float                            MiffR4;
typedef double                           MiffR8;
// String char types
// C1 = UTF8
typedef MiffN1                           MiffC1;
// C2 = UTF16
typedef wchar_t                          MiffC2;
// C4 = UTF32
typedef MiffN4                           MiffC4;

#pragma pack(1)
typedef struct { MiffI1   a, b;        } MiffABI1;
typedef struct { MiffN1   a, b;        } MiffABN1;
typedef struct { MiffI2   a, b;        } MiffABI2;
typedef struct { MiffN2   a, b;        } MiffABN2;
typedef struct { MiffI4   a, b;        } MiffABI4;
typedef struct { MiffN4   a, b;        } MiffABN4;
typedef struct { MiffR4   a, b;        } MiffABR4;
typedef struct { MiffI8   a, b;        } MiffABI8;
typedef struct { MiffN8   a, b;        } MiffABN8;
typedef struct { MiffR8   a, b;        } MiffABR8;

typedef struct { MiffI1   a, b, c;     } MiffABCI1;
typedef struct { MiffN1   a, b, c;     } MiffABCN1;
typedef struct { MiffI2   a, b, c;     } MiffABCI2;
typedef struct { MiffN2   a, b, c;     } MiffABCN2;
typedef struct { MiffI4   a, b, c;     } MiffABCI4;
typedef struct { MiffN4   a, b, c;     } MiffABCN4;
typedef struct { MiffR4   a, b, c;     } MiffABCR4;
typedef struct { MiffI8   a, b, c;     } MiffABCI8;
typedef struct { MiffN8   a, b, c;     } MiffABCN8;
typedef struct { MiffR8   a, b, c;     } MiffABCR8;

typedef struct { MiffI1   a, b, c, d;  } MiffABCDI1;
typedef struct { MiffN1   a, b, c, d;  } MiffABCDN1;
typedef struct { MiffI2   a, b, c, d;  } MiffABCDI2;
typedef struct { MiffN2   a, b, c, d;  } MiffABCDN2;
typedef struct { MiffI4   a, b, c, d;  } MiffABCDI4;
typedef struct { MiffN4   a, b, c, d;  } MiffABCDN4;
typedef struct { MiffR4   a, b, c, d;  } MiffABCDR4;
typedef struct { MiffI8   a, b, c, d;  } MiffABCDI8;
typedef struct { MiffN8   a, b, c, d;  } MiffABCDN8;
typedef struct { MiffR8   a, b, c, d;  } MiffABCDR8;

typedef struct { MiffR4   cell[2][2];  } MiffMatrix2x2R4;
typedef struct { MiffR8   cell[2][2];  } MiffMatrix2x2R8;

typedef struct { MiffR4   cell[3][3];  } MiffMatrix3x3R4;
typedef struct { MiffR8   cell[3][3];  } MiffMatrix3x3R8;

typedef struct { MiffR4   cell[4][4];  } MiffMatrix4x4R4;
typedef struct { MiffR8   cell[4][4];  } MiffMatrix4x4R8;

typedef union
{
   MiffI1             i;
   MiffN1             n;
   MiffN1             byte[1];
} Miff1;

typedef union
{
   MiffI2             i;
   MiffN2             n;
   MiffABI1           abi;
   MiffABN1           abn;
   MiffN1             byte[2];
} Miff2;

typedef union
{
   MiffABCI1          abci;
   MiffABCN1          abcn;
   MiffN1             byte[3];
} Miff3;

typedef union
{
   MiffI4             i;
   MiffN4             n;
   MiffR4             r;
   MiffABI2           abi;
   MiffABN2           abn;
   MiffABCDI1         abcdi;
   MiffABCDN1         abcdn;
   MiffN1             byte[4];
} Miff4;

typedef union
{
   MiffABCI2          abci;
   MiffABCN2          abcn;
   MiffN1             byte[6];
} Miff6;

typedef union
{
   MiffI8             i;
   MiffN8             n;
   MiffR8             r;
   MiffABI4           abi;
   MiffABN4           abn;
   MiffABR4           abr;
   MiffABCDI2         abcdi;
   MiffABCDN2         abcdn;
   MiffN1             byte[8];
} Miff8;

typedef union
{
   MiffABCI4          abci;
   MiffABCN4          abcn;
   MiffABCR4          abcr;
   MiffN1             byte[12];
} Miff12;

typedef struct       
{
   MiffABR8           abr;
   MiffABCDI4         abcdi;
   MiffABCDN4         abcdn;
   MiffABCDR4         abcdr;
   MiffMatrix2x2R4    matrix2x2r;
   MiffN1             byte[16];
} Miff16;

typedef struct
{
   MiffABCR8          abcr;
   MiffN1             byte[24];
} Miff24;

typedef struct
{
   MiffABCDR8         abcdr;
   MiffMatrix2x2R8    matrix2x2r;
   MiffN1             byte[32];
} Miff32;

typedef struct
{
   MiffMatrix3x3R4    matrix3x3r;
   MiffN1             byte[36];
} Miff36;

typedef struct
{
   MiffMatrix4x4R4    matrix4x4r;
   MiffN1             byte[64];
} Miff64;

typedef struct
{
   MiffMatrix3x3R8    matrix3x3r;
   MiffN1             byte[72];
} Miff72;

typedef struct
{
   MiffMatrix4x4R8    matrix4x4r;
   MiffN1             byte[128];
} Miff128;

typedef struct
{
   MiffN1             byte[256];
} Miff256;
#pragma pack()

typedef void    *(*MiffMemCreate)(        MiffN4 const memByteCount);
typedef void     (*MiffMemDestroy)(       void * const mem);
typedef MiffN4   (*MiffMemCompressBound)( MiffN4 const memByteCount);
typedef MiffBool (*MiffMemCompress)(      MiffN4 const memByteCount,         void const * const mem,         MiffN4 * const compressMemByteCount, void * const compressMem);
typedef MiffBool (*MiffMemDecompress)(    MiffN4 const compressMemByteCount, void const * const compressMem, MiffN4 * const memByteCount,         void * const mem);

typedef MiffBool (*MiffGetBuffer)(        void * const dataRepo, MiffN4 const byteCount, MiffN1       * const data);

typedef MiffBool (*MiffSetBuffer)(        void * const dataRepo, MiffN4 const byteCount, MiffN1 const * const data);

typedef struct
{
   MiffC2                      nameC2[257];
   MiffType                    type;
   MiffN4                      arrayCount;
   MiffBool                    isCompressed;
   MiffN4                      compressedChunkByteCount;
} MiffRecord;

typedef struct
{
   // MIFF file information and configuration.
   MiffMode                    mode;
   MiffN8                      version;
   MiffMethod                  method;
   MiffBool                    isByteSwapping;

   MiffC2                      subFormatNameC2[257];
   MiffN8                      subFormatVersion;

   // The list of types, all types even the predefined base types.
   MiffRecord                  currentRecord;
   MiffN4                      currentArrayIndex;

   // Data repo getters and setters.
   void                       *dataRepo;
   MiffGetBuffer               getBuffer;
   MiffSetBuffer               setBuffer;

   // Temporary storage for uncompressed memory.
   MiffN4                      memByteCount;
   void                       *memByteData;

   // Temporary storage for compressed memory.
   MiffBool                    isCompressed;
   MiffN4                      compressedChunkByteCount;

   MiffN4                      compressMemByteCountActual;
   MiffN4                      compressMemByteCount;
   MiffN4                      compressMemByteIndex;
   MiffN1                     *compressMemByteData;
} Miff;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
// Miff           *miffCreateReader(                                           MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, void * const dataRepo);
// MiffBool        miffCreateReaderContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, void * const dataRepo);
Miff           *miffCreateWriter(                                           MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffMode const mode, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo);
MiffBool        miffCreateWriterContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffMode const mode, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Miff       * const miff);
void            miffDestroyContent(                Miff       * const miff);

// void            miffGetCurrentVar(                 Miff const * const miff, MiffC2 ** const name, MiffType * const type, MiffN4 * const arrayCount);
// MiffBool        miffGetFileFlagIsBinary(           Miff const * const miff, MiffBool * const isBinary);
// MiffBool        miffGetFileVersion(                Miff const * const miff, MiffN8 * const version);
// MiffBool        miffGetFileVersionSubFormat(       Miff const * const miff, MiffC2 const ** const subFormatName, MiffN8 * const subFormatVersion);
// MiffBool        miffGetKey(                        Miff const * const miff, MiffC2 const ** const key);
// MiffBool        miffGetNextRecord(                 Miff       * const miff);
// MiffBool        miffGetValueHeader(                Miff const * const miff, MiffType * const type, MiffArrayFlag * const arrayFlag, MiffN4 * const arrayCount, MiffCompressFlag * const compressFlag, MiffN4 * const chunkByteCount);
// Miff1           miffGetValue1(                     Miff const * const miff);
// Miff2           miffGetValue2(                     Miff const * const miff);
// Miff3           miffGetValue3(                     Miff const * const miff);
// Miff4           miffGetValue4(                     Miff const * const miff);
// Miff8           miffGetValue8(                     Miff const * const miff);
// Miff16          miffGetValue16(                    Miff const * const miff);
// Miff32          miffGetValue32(                    Miff const * const miff);
// Miff64          miffGetValue64(                    Miff const * const miff);
// Miff128         miffGetValue128(                   Miff const * const miff);
// Miff256         miffGetValue256(                   Miff const * const miff);

MiffN8          miffRecordGetCount(                Miff       * const miff);
MiffC2         *miffRecordGetName(                 Miff       * const miff);
MiffType        miffRecordGetType(                 Miff       * const miff);

MiffBool        miffRecordSetBegin(                Miff       * const miff, MiffType const type, MiffC2 const * const key, MiffN4 const count, MiffBool const isCompressed, MiffN4 const compressedChunkByteCount);
MiffBool        miffRecordSetEnd(                  Miff       * const miff);
MiffBool        miffRecordSetNextArrayItem(        Miff       * const miff, MiffN8 const index, MiffN8 const count);

MiffBool        miffSetBlockStart(                 Miff       * const miff, MiffC2 const * const key);
MiffBool        miffSetBlockStop(                  Miff       * const miff);
MiffBool        miffSet1ABI1(                      Miff       * const miff, MiffC2 const * const key, MiffABI1        const * const value);
MiffBool        miffSet1ABI2(                      Miff       * const miff, MiffC2 const * const key, MiffABI2        const * const value);
MiffBool        miffSet1ABI4(                      Miff       * const miff, MiffC2 const * const key, MiffABI4        const * const value);
MiffBool        miffSet1ABN1(                      Miff       * const miff, MiffC2 const * const key, MiffABN1        const * const value);
MiffBool        miffSet1ABN2(                      Miff       * const miff, MiffC2 const * const key, MiffABN2        const * const value);
MiffBool        miffSet1ABN4(                      Miff       * const miff, MiffC2 const * const key, MiffABN4        const * const value);
MiffBool        miffSet1ABR4(                      Miff       * const miff, MiffC2 const * const key, MiffABR4        const * const value);
MiffBool        miffSet1ABR8(                      Miff       * const miff, MiffC2 const * const key, MiffABR8        const * const value);
MiffBool        miffSet1ABCI1(                     Miff       * const miff, MiffC2 const * const key, MiffABCI1       const * const value);
MiffBool        miffSet1ABCI2(                     Miff       * const miff, MiffC2 const * const key, MiffABCI2       const * const value);
MiffBool        miffSet1ABCI4(                     Miff       * const miff, MiffC2 const * const key, MiffABCI4       const * const value);
MiffBool        miffSet1ABCN1(                     Miff       * const miff, MiffC2 const * const key, MiffABCN1       const * const value);
MiffBool        miffSet1ABCN2(                     Miff       * const miff, MiffC2 const * const key, MiffABCN2       const * const value);
MiffBool        miffSet1ABCN4(                     Miff       * const miff, MiffC2 const * const key, MiffABCN4       const * const value);
MiffBool        miffSet1ABCR4(                     Miff       * const miff, MiffC2 const * const key, MiffABCR4       const * const value);
MiffBool        miffSet1ABCR8(                     Miff       * const miff, MiffC2 const * const key, MiffABCR8       const * const value);
MiffBool        miffSet1ABCDI1(                    Miff       * const miff, MiffC2 const * const key, MiffABCDI1      const * const value);
MiffBool        miffSet1ABCDI2(                    Miff       * const miff, MiffC2 const * const key, MiffABCDI2      const * const value);
MiffBool        miffSet1ABCDI4(                    Miff       * const miff, MiffC2 const * const key, MiffABCDI4      const * const value);
MiffBool        miffSet1ABCDN1(                    Miff       * const miff, MiffC2 const * const key, MiffABCDN1      const * const value);
MiffBool        miffSet1ABCDN2(                    Miff       * const miff, MiffC2 const * const key, MiffABCDN2      const * const value);
MiffBool        miffSet1ABCDN4(                    Miff       * const miff, MiffC2 const * const key, MiffABCDN4      const * const value);
MiffBool        miffSet1ABCDR4(                    Miff       * const miff, MiffC2 const * const key, MiffABCDR4      const * const value);
MiffBool        miffSet1ABCDR8(                    Miff       * const miff, MiffC2 const * const key, MiffABCDR8      const * const value);
MiffBool        miffSet1Boolean(                   Miff       * const miff, MiffC2 const * const key, MiffBool                const value);
MiffBool        miffSet1I1(                        Miff       * const miff, MiffC2 const * const key, MiffI1                  const value);
MiffBool        miffSet1I2(                        Miff       * const miff, MiffC2 const * const key, MiffI2                  const value);
MiffBool        miffSet1I4(                        Miff       * const miff, MiffC2 const * const key, MiffI4                  const value);
MiffBool        miffSet1I8(                        Miff       * const miff, MiffC2 const * const key, MiffI8                  const value);
MiffBool        miffSet1Matrix2x2R4(               Miff       * const miff, MiffC2 const * const key, MiffMatrix2x2R4 const * const value);
MiffBool        miffSet1Matrix2x2R8(               Miff       * const miff, MiffC2 const * const key, MiffMatrix2x2R8 const * const value);
MiffBool        miffSet1Matrix3x3R4(               Miff       * const miff, MiffC2 const * const key, MiffMatrix3x3R4 const * const value);
MiffBool        miffSet1Matrix3x3R8(               Miff       * const miff, MiffC2 const * const key, MiffMatrix3x3R8 const * const value);
MiffBool        miffSet1Matrix4x4R4(               Miff       * const miff, MiffC2 const * const key, MiffMatrix4x4R4 const * const value);
MiffBool        miffSet1Matrix4x4R8(               Miff       * const miff, MiffC2 const * const key, MiffMatrix4x4R8 const * const value);
MiffBool        miffSet1N1(                        Miff       * const miff, MiffC2 const * const key, MiffN1                  const value);
MiffBool        miffSet1N2(                        Miff       * const miff, MiffC2 const * const key, MiffN2                  const value);
MiffBool        miffSet1N4(                        Miff       * const miff, MiffC2 const * const key, MiffN4                  const value);
MiffBool        miffSet1N8(                        Miff       * const miff, MiffC2 const * const key, MiffN8                  const value);
MiffBool        miffSet1R4(                        Miff       * const miff, MiffC2 const * const key, MiffR4                  const value);
MiffBool        miffSet1R8(                        Miff       * const miff, MiffC2 const * const key, MiffR8                  const value);
MiffBool        miffSet1Type(                      Miff       * const miff, MiffC2 const * const key, MiffType                const value);
MiffBool        miffSet1StringC2(                  Miff       * const miff, MiffC2 const * const key, MiffC2          const * const value);
MiffBool        miffSetNABI1(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI1        const * const value);
MiffBool        miffSetNABI2(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI2        const * const value);
MiffBool        miffSetNABI4(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABI4        const * const value);
MiffBool        miffSetNABN1(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN1        const * const value);
MiffBool        miffSetNABN2(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN2        const * const value);
MiffBool        miffSetNABN4(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABN4        const * const value);
MiffBool        miffSetNABR4(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR4        const * const value);
MiffBool        miffSetNABR8(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABR8        const * const value);
MiffBool        miffSetNABCI1(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI1       const * const value);
MiffBool        miffSetNABCI2(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI2       const * const value);
MiffBool        miffSetNABCI4(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCI4       const * const value);
MiffBool        miffSetNABCN1(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN1       const * const value);
MiffBool        miffSetNABCN2(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN2       const * const value);
MiffBool        miffSetNABCN4(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCN4       const * const value);
MiffBool        miffSetNABCR4(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR4       const * const value);
MiffBool        miffSetNABCR8(                     Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCR8       const * const value);
MiffBool        miffSetNABCDI1(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI1      const * const value);
MiffBool        miffSetNABCDI2(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI2      const * const value);
MiffBool        miffSetNABCDI4(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDI4      const * const value);
MiffBool        miffSetNABCDN1(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN1      const * const value);
MiffBool        miffSetNABCDN2(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN2      const * const value);
MiffBool        miffSetNABCDN4(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDN4      const * const value);
MiffBool        miffSetNABCDR4(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR4      const * const value);
MiffBool        miffSetNABCDR8(                    Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffABCDR8      const * const value);
MiffBool        miffSetNBoolean(                   Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffBool        const * const value);
MiffBool        miffSetNI1(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffI1          const * const value);
MiffBool        miffSetNI2(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffI2          const * const value);
MiffBool        miffSetNI4(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffI4          const * const value);
MiffBool        miffSetNI8(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffI8          const * const value);
MiffBool        miffSetNMatrix2x2R4(               Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R4 const * const value);
MiffBool        miffSetNMatrix2x2R8(               Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix2x2R8 const * const value);
MiffBool        miffSetNMatrix3x3R4(               Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R4 const * const value);
MiffBool        miffSetNMatrix3x3R8(               Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix3x3R8 const * const value);
MiffBool        miffSetNMatrix4x4R4(               Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R4 const * const value);
MiffBool        miffSetNMatrix4x4R8(               Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffMatrix4x4R8 const * const value);
MiffBool        miffSetNN1(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffN1          const * const value);
MiffBool        miffSetNN2(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffN2          const * const value);
MiffBool        miffSetNN4(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffN4          const * const value);
MiffBool        miffSetNN8(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffN8          const * const value);
MiffBool        miffSetNR4(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffR4          const * const value);
MiffBool        miffSetNR8(                        Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffR8          const * const value);
MiffBool        miffSetNType(                      Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffType        const * const value);
MiffBool        miffSetNStringC2(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const count, MiffC2  const * const * const value);
MiffBool        miffSetValueABI1(                  Miff       * const miff, MiffABI1        const * const value);
MiffBool        miffSetValueABI2(                  Miff       * const miff, MiffABI2        const * const value);
MiffBool        miffSetValueABI4(                  Miff       * const miff, MiffABI4        const * const value);
MiffBool        miffSetValueABN1(                  Miff       * const miff, MiffABN1        const * const value);
MiffBool        miffSetValueABN2(                  Miff       * const miff, MiffABN2        const * const value);
MiffBool        miffSetValueABN4(                  Miff       * const miff, MiffABN4        const * const value);
MiffBool        miffSetValueABR4(                  Miff       * const miff, MiffABR4        const * const value);
MiffBool        miffSetValueABR8(                  Miff       * const miff, MiffABR8        const * const value);
MiffBool        miffSetValueABCI1(                 Miff       * const miff, MiffABCI1       const * const value);
MiffBool        miffSetValueABCI2(                 Miff       * const miff, MiffABCI2       const * const value);
MiffBool        miffSetValueABCI4(                 Miff       * const miff, MiffABCI4       const * const value);
MiffBool        miffSetValueABCN1(                 Miff       * const miff, MiffABCN1       const * const value);
MiffBool        miffSetValueABCN2(                 Miff       * const miff, MiffABCN2       const * const value);
MiffBool        miffSetValueABCN4(                 Miff       * const miff, MiffABCN4       const * const value);
MiffBool        miffSetValueABCR4(                 Miff       * const miff, MiffABCR4       const * const value);
MiffBool        miffSetValueABCR8(                 Miff       * const miff, MiffABCR8       const * const value);
MiffBool        miffSetValueABCDI1(                Miff       * const miff, MiffABCDI1      const * const value);
MiffBool        miffSetValueABCDI2(                Miff       * const miff, MiffABCDI2      const * const value);
MiffBool        miffSetValueABCDI4(                Miff       * const miff, MiffABCDI4      const * const value);
MiffBool        miffSetValueABCDN1(                Miff       * const miff, MiffABCDN1      const * const value);
MiffBool        miffSetValueABCDN2(                Miff       * const miff, MiffABCDN2      const * const value);
MiffBool        miffSetValueABCDN4(                Miff       * const miff, MiffABCDN4      const * const value);
MiffBool        miffSetValueABCDR4(                Miff       * const miff, MiffABCDR4      const * const value);
MiffBool        miffSetValueABCDR8(                Miff       * const miff, MiffABCDR8      const * const value);
MiffBool        miffSetValueBoolean(               Miff       * const miff, MiffBool                const value);
MiffBool        miffSetValueI1(                    Miff       * const miff, MiffI1                  const value);
MiffBool        miffSetValueI2(                    Miff       * const miff, MiffI2                  const value);
MiffBool        miffSetValueI4(                    Miff       * const miff, MiffI4                  const value);
MiffBool        miffSetValueI8(                    Miff       * const miff, MiffI8                  const value);
MiffBool        miffSetValueMatrix2x2R4(           Miff       * const miff, MiffMatrix2x2R4 const * const value);
MiffBool        miffSetValueMatrix2x2R8(           Miff       * const miff, MiffMatrix2x2R8 const * const value);
MiffBool        miffSetValueMatrix3x3R4(           Miff       * const miff, MiffMatrix3x3R4 const * const value);
MiffBool        miffSetValueMatrix3x3R8(           Miff       * const miff, MiffMatrix3x3R8 const * const value);
MiffBool        miffSetValueMatrix4x4R4(           Miff       * const miff, MiffMatrix4x4R4 const * const value);
MiffBool        miffSetValueMatrix4x4R8(           Miff       * const miff, MiffMatrix4x4R8 const * const value);
MiffBool        miffSetValueN1(                    Miff       * const miff, MiffN1                  const value);
MiffBool        miffSetValueN2(                    Miff       * const miff, MiffN2                  const value);
MiffBool        miffSetValueN4(                    Miff       * const miff, MiffN4                  const value);
MiffBool        miffSetValueN8(                    Miff       * const miff, MiffN8                  const value);
MiffBool        miffSetValueR4(                    Miff       * const miff, MiffR4                  const value);
MiffBool        miffSetValueR8(                    Miff       * const miff, MiffR8                  const value);
MiffBool        miffSetValueStringC2(              Miff       * const miff, MiffC2          const * const value);
MiffBool        miffSetValueType(                  Miff       * const miff, MiffType                const value);

MiffBool        miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy, MiffMemCompressBound const memCompressBound, MiffMemCompress const memCompress, MiffMemDecompress const memDecompress);
void            miffStop(                          void);

#endif
