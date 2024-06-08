/******************************************************************************
file:       MiffLib
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

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
#define miffKeyBYTE_COUNT                 256
#define miffKeySIZE                       257

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
   miffTypeNONE,

   miffTypeKEY_VALUE_BLOCK_STOP,
   miffTypeKEY_VALUE_BLOCK_START,
   miffTypeTYPE,
   miffTypeSTR,
   miffTypeVARIABLE,
   miffTypeUSER_TYPE,
   miffTypeBOOLEAN,
   miffTypeI1,
   miffTypeI2,
   miffTypeI4,
   miffTypeI8,
   miffTypeI,
   miffTypeN1,
   miffTypeN2,
   miffTypeN4,
   miffTypeN8,
   miffTypeN,
   miffTypeR4,
   miffTypeR4S,
   miffTypeR8,
   miffTypeR8S,

   miffTypeCOUNT
} MiffType;

#if 0
typedef enum
{
   miffTypeComplexNONE,

   miffTypeComplexABI1,
   miffTypeComplexABI2,
   miffTypeComplexABI4,
   miffTypeComplexABI8,
   miffTypeComplexABN1,
   miffTypeComplexABN2,
   miffTypeComplexABN4,
   miffTypeComplexABN8,
   miffTypeComplexABR4,
   miffTypeComplexABR4S,
   miffTypeComplexABR8,
   miffTypeComplexABR8S,
   miffTypeComplexABCI1,
   miffTypeComplexABCI2,
   miffTypeComplexABCI4,
   miffTypeComplexABCI8,
   miffTypeComplexABCN1,
   miffTypeComplexABCN2,
   miffTypeComplexABCN4,
   miffTypeComplexABCN8,
   miffTypeComplexABCR4,
   miffTypeComplexABCR4S,
   miffTypeComplexABCR8,
   miffTypeComplexABCR8S,
   miffTypeComplexABCDI1,
   miffTypeComplexABCDI2,
   miffTypeComplexABCDI4,
   miffTypeComplexABCDI8,
   miffTypeComplexABCDN1,
   miffTypeComplexABCDN2,
   miffTypeComplexABCDN4,
   miffTypeComplexABCDN8,
   miffTypeComplexABCDR4,
   miffTypeComplexABCDR4S,
   miffTypeComplexABCDR8,
   miffTypeComplexABCDR8S,
   miffTypeComplexCOMPLEX,
   miffTypeComplexMATRIX2X2R4,
   miffTypeComplexMATRIX2X2R4S,
   miffTypeComplexMATRIX2X2R8,
   miffTypeComplexMATRIX2X2R8S,
   miffTypeComplexMATRIX3X3R4,
   miffTypeComplexMATRIX3X3R4S,
   miffTypeComplexMATRIX3X3R8,
   miffTypeComplexMATRIX3X3R8S,
   miffTypeComplexMATRIX4X4R4,
   miffTypeComplexMATRIX4X4R4S,
   miffTypeComplexMATRIX4X4R8,
   miffTypeComplexMATRIX4X4R8S,
} MiffTypeComplex;
#endif

#define miffArrayCountUNKNOWN          UINT64_MAX
#define miffArrayCountUNKNOWN_MiffStr  "*"

/******************************************************************************
type:
******************************************************************************/
// Integer types
typedef char                           MiffI1;
typedef short                          MiffI2;
typedef int                            MiffI4;
typedef int64_t                        MiffI8;
typedef MiffI8                         MiffI;
// Natural types
typedef unsigned char                  MiffN1;
typedef unsigned short                 MiffN2;
typedef int                            MiffN4;
typedef uint64_t                       MiffN8;
typedef MiffN8                         MiffN;
// Real types
typedef float                          MiffR4;
typedef double                         MiffR8;
// String char types
// C1 = UTF8
typedef MiffN1                         MiffStr;

#define MiffN4_MAX                     UINT32_MAX

#pragma pack(1)
#if 0
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
#endif

typedef struct
{
   MiffType           type;
   MiffType           t;
   MiffBool           b;
   MiffI              i;
   MiffN              n;
   MiffR4             r4;
   MiffR8             r8;
   MiffN              strLen;
   MiffStr           *str;
} MiffValue;

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
#if 0
   MiffABI1           abi;
   MiffABN1           abn;
#endif
   MiffN1             byte[2];
} Miff2;

typedef union
{
#if 0
   MiffABCI1          abci;
   MiffABCN1          abcn;
#endif
   MiffN1             byte[3];
} Miff3;

typedef union
{
   MiffI4             i;
   MiffN4             n;
   MiffR4             r;
#if 0
   MiffABI2           abi;
   MiffABN2           abn;
   MiffABCDI1         abcdi;
   MiffABCDN1         abcdn;
#endif
   MiffN1             byte[4];
} Miff4;

typedef union
{
#if 0
   MiffABCI2          abci;
   MiffABCN2          abcn;
#endif
   MiffN1             byte[6];
} Miff6;

typedef union
{
   MiffI8             i;
   MiffN8             n;
   MiffR8             r;
#if 0
   MiffABI4           abi;
   MiffABN4           abn;
   MiffABR4           abr;
   MiffABCDI2         abcdi;
   MiffABCDN2         abcdn;
#endif
   MiffN1             byte[8];
} Miff8;

typedef union
{
#if 0
   MiffABCI4          abci;
   MiffABCN4          abcn;
   MiffABCR4          abcr;
#endif
   MiffN1             byte[12];
} Miff12;

typedef struct
{
#if 0
   MiffABR8           abr;
   MiffABCDI4         abcdi;
   MiffABCDN4         abcdn;
   MiffABCDR4         abcdr;
   MiffMatrix2x2R4    matrix2x2r;
#endif
   MiffN1             byte[16];
} Miff16;

typedef struct
{
#if 0
   MiffABCR8          abcr;
#endif
   MiffN1             byte[24];
} Miff24;

typedef struct
{
#if 0
   MiffABCDR8         abcdr;
   MiffMatrix2x2R8    matrix2x2r;
#endif
   MiffN1             byte[32];
} Miff32;

typedef struct
{
#if 0
   MiffMatrix3x3R4    matrix3x3r;
#endif
   MiffN1             byte[36];
} Miff36;

typedef struct
{
#if 0
   MiffMatrix4x4R4    matrix4x4r;
#endif
   MiffN1             byte[64];
} Miff64;

typedef struct
{
#if 0
   MiffMatrix3x3R8    matrix3x3r;
#endif
   MiffN1             byte[72];
} Miff72;

typedef struct
{
#if 0
   MiffMatrix4x4R8    matrix4x4r;
#endif
   MiffN1             byte[128];
} Miff128;

typedef struct
{
   MiffN1             byte[256];
} Miff256;
#pragma pack()

typedef void    *(*MiffMemCreate)(        MiffN4 const memByteCount);
typedef void     (*MiffMemDestroy)(       void * const mem);

typedef MiffBool (*MiffGetBuffer)(        void * const dataSource,      MiffN4 const byteCount, MiffN1       * const data);

typedef MiffBool (*MiffSetBuffer)(        void * const dataDestination, MiffN4 const byteCount, MiffN1 const * const data);

typedef struct
{
   MiffStr                     name[miffKeySIZE];
   MiffType                    type;
   MiffN                       arrayCount;
} MiffRecord;

typedef struct
{
   // MIFF file information and configuration.
   MiffN                       version;
   MiffMethod                  method;
   MiffBool                    isByteSwapping;

   MiffStr                     subFormatName[miffKeySIZE];
   MiffN                       subFormatVersion;

   // The list of types, all types even the predefined base types.
   MiffRecord                  currentRecord;
   MiffN                       currentArrayIndex;

   // Data repo getters and setters.
   void                       *dataRepo;
   MiffGetBuffer               getBuffer;
   MiffSetBuffer               setBuffer;

   // Temporary storage for uncompressed memory.
   MiffN                       memByteCount;
   void                       *memByteData;

   // Temporary storage for reading
   MiffBool                    readRecordIsDone;
   MiffN                       readByteCountActual;
   MiffN                       readByteCount;
   MiffN1                     *readByteData;

   MiffN                       readStrCountActual;
   MiffN                       readStrCount;
   MiffStr                    *readStrData;
} Miff;

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
Miff           *miffCreateReader(                                           MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffN8  * const subFormatVersion, void * const dataRepo);
MiffBool        miffCreateReaderContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffGetBuffer getBufferFunc, MiffStr       * const subFormatName, MiffN8  * const subFormatVersion, void * const dataRepo);
Miff           *miffCreateWriter(                                           MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN8    const subFormatVersion, void * const dataRepo);
MiffBool        miffCreateWriterContent(           Miff       * const miff, MiffBool const isByteSwaping, MiffSetBuffer setBufferFunc, MiffStr const * const subFormatName, MiffN8    const subFormatVersion, void * const dataRepo);

void            miffDestroy(                       Miff       * const miff);
void            miffDestroyContent(                Miff       * const miff);

MiffBool        miffRecordGetInfo(                 Miff       * const miff, MiffType * const type, MiffStr       * const typeName, MiffStr       * const key, MiffN * const count);
MiffBool        miffRecordGetEnd(                  Miff       * const miff);
#if 0
MiffN8          miffRecordGetCount(                Miff       * const miff);
MiffStr        *miffRecordGetName(                 Miff       * const miff);
MiffType        miffRecordGetType(                 Miff       * const miff);
#endif
MiffValue       miffRecordGetValueBoolean(         Miff       * const miff);
MiffValue       miffRecordGetValueI(               Miff       * const miff);
MiffValue       miffRecordGetValueN(               Miff       * const miff);
MiffValue       miffRecordGetValueR4(              Miff       * const miff);
MiffValue       miffRecordGetValueR4S(             Miff       * const miff);
MiffValue       miffRecordGetValueR8(              Miff       * const miff);
MiffValue       miffRecordGetValueR8S(             Miff       * const miff);
MiffValue       miffRecordGetValueStr(             Miff       * const miff);

MiffBool        miffRecordSetBlockStart(           Miff       * const miff, MiffStr const * const key);
MiffBool        miffRecordSetBlockStop(            Miff       * const miff);
MiffBool        miffRecordSetEnd(                  Miff       * const miff);
MiffBool        miffRecordSetInfo(                 Miff       * const miff, MiffType   const type, MiffStr const * const typeName, MiffStr const * const key, MiffN const count);
MiffBool        miffRecordSetSeparator(            Miff       * const miff);
MiffBool        miffRecordSetValue(                Miff       * const miff, MiffValue value);

MiffType        miffValueGetValueType(             MiffValue value);
MiffBool        miffValueGetBool(                  MiffValue value);
MiffI           miffValueGetI(                     MiffValue value);
MiffN           miffValueGetN(                     MiffValue value);
MiffR4          miffValueGetR4(                    MiffValue value);
MiffR8          miffValueGetR8(                    MiffValue value);
MiffStr        *miffValueGetStr(                   MiffValue value);
MiffN           miffValueGetStrLen(                MiffValue value);
MiffType        MiffValueGetType(                  MiffValue value);

MiffValue       miffValueSetBoolean(               MiffBool const value);
MiffValue       miffValueSetI(                     MiffI    const value);
MiffValue       miffValueSetN(                     MiffN    const value);
MiffValue       miffValueSetR4(                    MiffR4   const value);
MiffValue       miffValueSetR(                     MiffR8   const value);
MiffValue       miffValueSetStr(                   MiffN const strLen, MiffStr const * const str);
MiffValue       miffValueSetType(                  MiffType const value);

MiffBool        miffStart(                         MiffMemCreate const memCreate, MiffMemDestroy const memDestroy);
void            miffStop(                          void);

MiffStr const  *miffTypeGetStr(                    MiffType const type);

#endif
