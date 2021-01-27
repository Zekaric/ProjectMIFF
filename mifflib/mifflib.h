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
   miffValueTypeNONE,

   miffValueTypeKEY_VALUE_BLOCK,          // 1
   miffValueTypeVALUE_STREAM_BLOCK,       // 2
   miffValueTypeBINARY_DATA,              // 3
   miffValueTypeEMBEDDED_FILE,            // 4
   miffValueTypeTYPE,                     // 5
   miffValueTypeSTRING,                   // 6
   miffValueTypePATH,                     // 7
   miffValueTypeKEY_ONLY_NO_VALUE,        // 8
   miffValueTypeUSER_TYPE,                // 9
   miffValueTypeBOOLEAN,                  // 10
   miffValueTypeI1,                       // 11
   miffValueTypeI2,                       // 12
   miffValueTypeI3,                       // 13
   miffValueTypeI4,                       // 14
   miffValueTypeI8,                       // 15
   miffValueTypeI16,                      // 16
   miffValueTypeI32,                      // 17
   miffValueTypeI64,                      // 18
   miffValueTypeI128,                     // 19
   miffValueTypeI256,                     // 20
   miffValueTypeN1               = 31,    // 31
   miffValueTypeN2,                       // 32
   miffValueTypeN3,                       // 33
   miffValueTypeN4,                       // 34
   miffValueTypeN8,                       // 35
   miffValueTypeN16,                      // 36
   miffValueTypeN32,                      // 37
   miffValueTypeN64,                      // 38
   miffValueType128,                      // 39
   miffValueTypeN256,                     // 40
   miffValueTypeR2               = 52,    // 52
   miffValueTypeR4               = 54,    // 54
   miffValueTypeR8,                       // 55
   miffValueTypeR16,                      // 56
   miffValueTypeR32,                      // 57
   miffValueTypeR64,                      // 58
   miffValueTypeR128,                     // 59
   miffValueTypeR256,                     // 60
   
   miffValueTypeFIRST_USER_TYPE  = 64,

   miffValueTypeLAST_USER_TYPE   = 4095
} MiffValueType;

typedef enum
{
   miffArrayFlagIS_SINGLE,
   miffArrayFlagIS_ARRAY
} MiffArrayFlag;

typedef enum
{
   miffCompressionFlagIS_UNCOMPRESSED,
   miffCompressionFlagIS_COMPRESSED,
   miffCompressionFlagIS_CHUNKED_AND_COMPRESSED
} MiffCompressionFlag;

/******************************************************************************
type:
******************************************************************************/
// Integer types
#define MiffI1    char
#define MiffI2    short
#define MiffI4    int
#define MiffI8    int64_t
// Natural types
#define MiffN1    unsigned char
#define MiffN2    unsigned short
#define MiffN4    unsigned int
#define MiffN8    uint64_t
// Real types
#define MiffR4    float
#define MiffR8    double
// String type
#define MiffC     wchar_t
#define MiffKey   Mn1

typedef struct
{
   MiffI1   i;
   MiffN1   n;
   MiffN1   b;
} Miff1;

typedef struct
{
   MiffI2   i;
   MiffN2   n;
   MiffN1   byte[2];
} Miff2;

typedef struct
{
   MiffN1   byte[3];
} Miff3;

typedef struct
{
   MiffI4   i;
   MiffN4   n;
   MiffR4   r;
   MiffN1   byte[4];
} Miff4;

typedef struct
{
   MiffI8   i;
   MiffN8   n;
   MiffR8   r;
   MiffN1   byte[8];
} Miff8;

typedef struct
{
   MiffN1   b[16];
} Miff16;

typedef struct
{
   MiffN1   b[32];
} Miff32;

typedef struct
{
   MiffN1   b[64];
} Miff64;

typedef struct
{
   MiffN1   b[128];
} Miff128;

typedef struct
{
   MiffN1   b[256];
} Miff256;

typedef void    *(*MiffMemCreate)(     MiffN4 const memByteCount);
typedef void     (*MiffMemDestroy)(    void * const mem);
typedef MiffBool (*MiffMemCompress)(   MiffN4 const memByteCount,           void const * const mem,           MiffN4 * const compressedMemByteCount, MiffN1 ** const compressedMem);
typedef MiffBool (*MiffMemDecompress)( MiffN4 const compressedMemByteCount, MiffN1 const * const compressedMem, MiffN4 * const memByteCount,           void ** const mem);

typedef MiffBool (*MiffGetBuffer1)(    void * const dataRepository, MiffN4 const byteCount, Miff1       * const buffer);
typedef MiffBool (*MiffGetBuffer2)(    void * const dataRepository, MiffN4 const byteCount, Miff2       * const buffer);
typedef MiffBool (*MiffGetBuffer4)(    void * const dataRepository, MiffN4 const byteCount, Miff4       * const buffer);
typedef MiffBool (*MiffGetBuffer8)(    void * const dataRepository, MiffN4 const byteCount, Miff8       * const buffer);

typedef MiffBool (*MiffSetBuffer1)(    void * const dataRepository, MiffN4 const byteCount, Miff1 const * const buffer);
typedef MiffBool (*MiffSetBuffer2)(    void * const dataRepository, MiffN4 const byteCount, Miff2 const * const buffer);
typedef MiffBool (*MiffSetBuffer4)(    void * const dataRepository, MiffN4 const byteCount, Miff4 const * const buffer);
typedef MiffBool (*MiffSetBuffer8)(    void * const dataRepository, MiffN4 const byteCount, Miff8 const * const buffer);

typedef struct
{
   MiffMethod      method;
   MiffBool        isByteSwapping;
   MiffBool        isBinary;
   MiffC          *subFormatName;
   MiffN8          subFormatVersion;
   MiffN1          keyByteCount;
   MiffKey        *key;
   MiffN4          memByteCount;
   void           *mem;
   MiffN4          compressedMemByteCount;
   MiffN1         *compressedMem;
   MiffGetBuffer1 *getBuffer1;
   MiffGetBuffer2 *getBuffer2;
   MiffGetBuffer4 *getBuffer4;
   MiffGetBuffer8 *getBuffer8;
   MiffSetBuffer1 *setBuffer1;
   MiffSetBuffer2 *setBuffer2;
   MiffSetBuffer4 *setBuffer4;
   MiffSetBuffer8 *setBuffer8;
} Miff;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Miff     *miffCreateReader(                            MiffBool const isByteSwaping, MiffGetBuffer1 const getBuffer1Func, MiffGetBuffer2 const getBuffer2Func, MiffGetBuffer4 const getBuffer4Func, MiffGetBuffer8 const getBuffer8Func);
MiffBool  miffCreateReaderContent(  Miff * const miff, MiffBool const isByteSwaping, MiffGetBuffer1 const getBuffer1Func, MiffGetBuffer2 const getBuffer2Func, MiffGetBuffer4 const getBuffer4Func, MiffGetBuffer8 const getBuffer8Func);
Miff     *miffCreateWriter(                            MiffBool const isByteSwaping, MiffSetBuffer1 const setBuffer1Func, MiffSetBuffer2 const setBuffer2Func, MiffSetBuffer4 const setBuffer4Func, MiffSetBuffer8 const setBuffer8Func, MiffBool const isBinary, MiffN4 const subFormatNameLen, MiffC const * const subFormatName, MiffN8 const subFormatVersion);
MiffBool  miffCreateWriterContent(  Miff * const miff, MiffBool const isByteSwaping, MiffSetBuffer1 const setBuffer1Func, MiffSetBuffer2 const setBuffer2Func, MiffSetBuffer4 const setBuffer4Func, MiffSetBuffer8 const setBuffer8Func, MiffBool const isBinary, MiffN4 const subFormatNameLen, MiffC const * const subFormatName, MiffN8 const subFormatVersion);

void      miffDestroy(              Miff * const miff);
void      miffDestroyContent(       Miff * const miff);

MiffBool  miffGetVersion(           Miff * const miff, MiffN4 * const version, MiffBool * const isBinary);
MiffBool  miffGetVersionSubFormat(  Miff * const miff, MiffC * const subFormatName, MiffN8 * const subFormatVersion);

MiffBool  miffGetKey(               Miff * const miff, MiffC * const key);
MiffBool  miffGetValueHeader(       Miff * const miff, MiffValueType * const type, MiffValueArrayFlag * const arrayFlag);
MiffN4    miffGetArrayCount(        Miff * const miff);
Miff1     miffGetValue1(            Miff * const miff);
Miff2     miffGetValue2(            Miff * const miff);
Miff3     miffGetValue3(            Miff * const miff);
Miff4     miffGetValue4(            Miff * const miff);
Miff8     miffGetValue8(            Miff * const miff);
Miff16    miffGetValue16(           Miff * const miff);
Miff32    miffGetValue32(           Miff * const miff);
Miff64    miffGetValue64(           Miff * const miff);
Miff128   miffGetValue128(          Miff * const miff);
Miff256   miffGetValue256(          Miff * const miff);

MiffBool  miffSetKey(               Miff * const miff, MiffC const * const key, MiffValueType const type, MiffValueArrayFlag const arrayFlag, MiffValueCompressionFlag const compressionFlag, MiffN4 const chunkByteCount);
MiffBool  miffSetValue1(            Miff * const miff, Miff1   const value);
MiffBool  miffSetValue2(            Miff * const miff, Miff2   const value);
MiffBool  miffSetValue3(            Miff * const miff, Miff3   const value);
MiffBool  miffSetValue4(            Miff * const miff, Miff4   const value);
MiffBool  miffSetValue8(            Miff * const miff, Miff8   const value);
MiffBool  miffSetValue16(           Miff * const miff, Miff16  const value);
MiffBool  miffSetValue32(           Miff * const miff, Miff32  const value);
MiffBool  miffSetValue64(           Miff * const miff, Miff64  const value);
MiffBool  miffSetValue128(          Miff * const miff, Miff128 const value);
MiffBool  miffSetValue256(          Miff * const miff, Miff256 const value);

MiffBool  miffStart(                MiffMemCreate const memCreate, MiffMemDestroy const memDestroy, MiffMemCompress const compressBuffer, MiffMemDecompress const decompressBuffer);
void      miffStop(                 void);

#endif
