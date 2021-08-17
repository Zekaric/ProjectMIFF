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
   miffValueTypeNONE,

   miffValueTypeKEY_VALUE_BLOCK_STOP,
   miffValueTypeKEY_VALUE_BLOCK_START,
   miffValueTypeTYPE,
   miffValueTypeDEFINE,
   miffValueTypeSTRING                       = 5,
   miffValueTypeBOOLEAN                      = 8,
   miffValueTypeI1                           = 10,
   miffValueTypeI2,
   miffValueTypeI3,
   miffValueTypeI4,
   miffValueTypeI8,
   miffValueTypeI16,
   miffValueTypeI32,
   miffValueTypeI64,
   miffValueTypeI128,
   miffValueTypeI256,
   miffValueTypeN1                           = 20,
   miffValueTypeN2,
   miffValueTypeN3,
   miffValueTypeN4,
   miffValueTypeN8,
   miffValueTypeN16,
   miffValueTypeN32,
   miffValueTypeN64,
   miffValueTypeN128,
   miffValueTypeN256,
   miffValueTypeR4                           = 33,
   miffValueTypeR8,

   miffValueTypeFIRST_USER_TYPE              = 64,

   miffValueTypeLAST_USER_TYPE               = 0x0FFF
} MiffValueType;

#define miffArrayCountUNKNOWN                ((MiffN4) 0xFFFFFFFF)
#define miffArrayCountUNKNOWN_C2             L"*"

typedef enum
{
   miffCompressFlagNONE                      = 0x0,
   miffCompressFlagCHUNK_COMPRESS            = 0x1
} MiffCompressFlag;

/******************************************************************************
type:
******************************************************************************/
// Integer types
#define MiffI1       char
#define MiffI2       short
#define MiffI4       int
#define MiffI8       int64_t
// Natural types
#define MiffN1       unsigned char
#define MiffN2       unsigned short
#define MiffN4       unsigned int
#define MiffN8       uint64_t
// Real types
#define MiffR4       float
#define MiffR8       double
// String char types
// C1 = UTF8
#define MiffC1       MiffN1
// C2 = UTF16
#define MiffC2       wchar_t
// C4 = UTF32
#define MiffC4       MiffN4

typedef union
{
   MiffI1   i;
   MiffN1   n;
   MiffN1   byte[1];
} Miff1;

typedef union
{
   MiffI2   i;
   MiffN2   n;
   MiffN1   byte[2];
} Miff2;

typedef union
{
   MiffN1   byte[3];
} Miff3;

typedef union
{
   MiffI4   i;
   MiffN4   n;
   MiffR4   r;
   MiffN1   byte[4];
} Miff4;

typedef union
{
   MiffI8   i;
   MiffN8   n;
   MiffR8   r;
   MiffN1   byte[8];
} Miff8;

typedef struct
{
   MiffN1   byte[16];
} Miff16;

typedef struct
{
   MiffN1   byte[32];
} Miff32;

typedef struct
{
   MiffN1   byte[64];
} Miff64;

typedef struct
{
   MiffN1   byte[128];
} Miff128;

typedef struct
{
   MiffN1   byte[256];
} Miff256;

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
   MiffValueType               type;
   MiffN4                      arrayCount;
   MiffCompressFlag            compressFlag;
   MiffN4                      chunkByteCount;
} MiffTypeRecord;

typedef struct
{
   MiffC2                     *nameC2;
   MiffTypeRecord              type;
} MiffUnrollRecord;

typedef struct
{
   MiffBool                    isSet;
   MiffC2                      nameC2[257];
   MiffN4                      varCount;
   MiffTypeRecord             *varList;
} MiffUserType;

typedef struct
{
   // MIFF file information and configuration.
   MiffMode                    mode;
   MiffN8                      version;
   MiffMethod                  method;
   MiffBool                    isByteSwapping;

   MiffC2                      subFormatNameC2[257];
   MiffN8                      subFormatVersion;

   // The list of user types
   MiffValueType               defineCurrentType;
   MiffN4                      defineIndex;
   MiffUserType                defineList[4096];

   MiffValueType               defineUnrolledType;
   MiffN4                      defineUnrolledCount;
   MiffUnrollRecord           *defineUnrolledArray;
   MiffN4                      defineUnrolledArrayCount;

   // Data repo getters and setters.
   void                       *dataRepo;
   MiffGetBuffer               getBuffer;
   MiffSetBuffer               setBuffer;

   // Current record key value.
   MiffTypeRecord              currentRecord;
   MiffN4                      currentIndex;

   // Temporary storage for uncompressed memory.
   MiffN4                      memByteCount;
   void                       *memByteData;

   // Temporary storage for compressed memory.
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
Miff           *miffCreateWriter(                                     MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffMode const mode, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo);
MiffBool        miffCreateWriterContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffMode const mode, MiffC2 const * const subFormatName, MiffN8 const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Miff       * const miff);
void            miffDestroyContent(                Miff       * const miff);

// void            miffGetCurrentVar(                 Miff const * const miff, MiffC2 ** const name, MiffValueType * const type, MiffN4 * const arrayCount);
// MiffBool        miffGetFileFlagIsBinary(           Miff const * const miff, MiffBool * const isBinary);
// MiffBool        miffGetFileVersion(                Miff const * const miff, MiffN8 * const version);
// MiffBool        miffGetFileVersionSubFormat(       Miff const * const miff, MiffC2 const ** const subFormatName, MiffN8 * const subFormatVersion);
// MiffBool        miffGetKey(                        Miff const * const miff, MiffC2 const ** const key);
// MiffBool        miffGetNextRecord(                 Miff       * const miff);
// MiffBool        miffGetValueHeader(                Miff const * const miff, MiffValueType * const type, MiffArrayFlag * const arrayFlag, MiffN4 * const arrayCount, MiffCompressFlag * const compressFlag, MiffN4 * const chunkByteCount);
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

MiffBool        miffSetBlockStart(           Miff       * const miff, MiffC2 const * const key);
MiffBool        miffSetBlockStop(            Miff       * const miff);
MiffBool        miffSetHeader(               Miff       * const miff, MiffValueType const type, MiffC2 const * const key, MiffN4 const count, MiffCompressFlag const compressFlag, MiffN4 const chunkByteCount);
MiffBool        miffSetHeaderDefine(         Miff       * const miff, MiffValueType const type, MiffC2 const * const key, MiffN4 const count);
MiffBool        miffSet1Boolean(             Miff       * const miff, MiffC2 const * const key, MiffBool              const value);
MiffBool        miffSet1I1(                  Miff       * const miff, MiffC2 const * const key, MiffI1                const value);
MiffBool        miffSet1I2(                  Miff       * const miff, MiffC2 const * const key, MiffI2                const value);
MiffBool        miffSet1I4(                  Miff       * const miff, MiffC2 const * const key, MiffI4                const value);
MiffBool        miffSet1I8(                  Miff       * const miff, MiffC2 const * const key, MiffI8                const value);
MiffBool        miffSet1N1(                  Miff       * const miff, MiffC2 const * const key, MiffN1                const value);
MiffBool        miffSet1N2(                  Miff       * const miff, MiffC2 const * const key, MiffN2                const value);
MiffBool        miffSet1N4(                  Miff       * const miff, MiffC2 const * const key, MiffN4                const value);
MiffBool        miffSet1N8(                  Miff       * const miff, MiffC2 const * const key, MiffN8                const value);
MiffBool        miffSet1R4(                  Miff       * const miff, MiffC2 const * const key, MiffR4                const value);
MiffBool        miffSet1R8(                  Miff       * const miff, MiffC2 const * const key, MiffR8                const value);
MiffBool        miffSet1Type(                Miff       * const miff, MiffC2 const * const key, MiffValueType         const value);
MiffBool        miffSet1StringC2(            Miff       * const miff, MiffC2 const * const key, MiffC2        const * const value);
MiffBool        miffSetNBoolean(             Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffBool              const * const value);
MiffBool        miffSetNI1(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffI1                const * const value);
MiffBool        miffSetNI2(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffI2                const * const value);
MiffBool        miffSetNI4(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffI4                const * const value);
MiffBool        miffSetNI8(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffI8                const * const value);
MiffBool        miffSetNN1(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffN1                const * const value);
MiffBool        miffSetNN2(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffN2                const * const value);
MiffBool        miffSetNN4(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffN4                const * const value);
MiffBool        miffSetNN8(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffN8                const * const value);
MiffBool        miffSetNR4(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffR4                const * const value);
MiffBool        miffSetNR8(                  Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffR8                const * const value);
MiffBool        miffSetNType(                Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffValueType         const * const value);
MiffBool        miffSetNStringC2(            Miff       * const miff, MiffC2 const * const key, MiffN4 const counst, MiffC2        const * const * const value);
MiffBool        miffSetValueBoolean(         Miff       * const miff, MiffBool              const value);
MiffBool        miffSetValueDefine(          Miff       * const miff, MiffValueType         const type, MiffC2 const * const name, MiffN4 const count, MiffCompressFlag const compressFlag, MiffN4 const chunkByteCount);
MiffBool        miffSetValueI1(              Miff       * const miff, MiffI1                const value);
MiffBool        miffSetValueI2(              Miff       * const miff, MiffI2                const value);
MiffBool        miffSetValueI4(              Miff       * const miff, MiffI4                const value);
MiffBool        miffSetValueI8(              Miff       * const miff, MiffI8                const value);
MiffBool        miffSetValueN1(              Miff       * const miff, MiffN1                const value);
MiffBool        miffSetValueN2(              Miff       * const miff, MiffN2                const value);
MiffBool        miffSetValueN4(              Miff       * const miff, MiffN4                const value);
MiffBool        miffSetValueN8(              Miff       * const miff, MiffN8                const value);
MiffBool        miffSetValueR4(              Miff       * const miff, MiffR4                const value);
MiffBool        miffSetValueR8(              Miff       * const miff, MiffR8                const value);
MiffBool        miffSetValueStringC2(        Miff       * const miff, MiffC2        const * const value);
MiffBool        miffSetValueType(            Miff       * const miff, MiffValueType         const value);

MiffBool        miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy, MiffMemCompressBound const memCompressBound, MiffMemCompress const memCompress, MiffMemDecompress const memDecompress);
void            miffStop(                          void);

#endif
