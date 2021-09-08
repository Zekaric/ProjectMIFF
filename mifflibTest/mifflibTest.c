/******************************************************************************
file:       MiffLibTest
author:     Robbert de Groot
company:    Zekaric
copyright:  2021, Zekaric

description:
Test program for MiffLib
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#include "zlib.h"
#include "mifflib.h"

/******************************************************************************
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/
static void    *_MemCreate(         MiffN4 const memByteCount);
static void     _MemDestroy(        void * const mem);
static MiffBool _MemCompress(       MiffN4 const memByteCount,         void const * const mem,         MiffN4 * const compressMemByteCount, void * const compressMem);
static MiffN4   _MemCompressBound(  MiffN4 const memByteCount);
static MiffBool _MemDecompress(     MiffN4 const compressMemByteCount, void const * const compressMem, MiffN4 * const memByteCount,         void * const mem);

static MiffBool _SetBuffer(         void * const dataRepo, MiffN4 const byteCount, MiffN1 const * const byteData);

static MiffBool _TestWrite(         MiffC2 * const fileName, MiffMode const mode);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: main
******************************************************************************/
int main(int acount, char **alist)
{
   int   result;

   acount; alist;

   result = 0;

   // Start miff library.
   if (!miffStart(_MemCreate, _MemDestroy, _MemCompressBound, _MemCompress, _MemDecompress))
   {
      result = 1;
      goto DONE;
   }

   // Test the writing.
   if (!_TestWrite(L"MiffFileTestTxt.miff", miffModeTEXT))
   {
      result = 2;
      goto DONE;
   }

//   if (!_TestWrite(L"MiffFileTestBin.miff", miffModeBINARY))
//   {
//      result = 3;
//      goto DONE;
//   }

DONE:
   // Stop miff library.
   miffStop();

   return result;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _MemCreate
******************************************************************************/
void *_MemCreate(MiffN4 const memByteCount)
{
   return calloc(1, (size_t) memByteCount);
}

/******************************************************************************
func: _MemDestroy
******************************************************************************/
void _MemDestroy(void * const mem)
{
   free(mem);
}

/******************************************************************************
func: _MemCompressBound
******************************************************************************/
MiffN4 _MemCompressBound(MiffN4 const memByteCount)
{
   return compressBound((uLong) memByteCount);
}

/******************************************************************************
func: _MemCompress
******************************************************************************/
MiffBool _MemCompress(MiffN4 const memByteCount, void const * const mem,
   MiffN4 * const compressMemByteCount, void * const compressMem)
{
   uLongf minSize;
   MiffN4 result;

   if (!memByteCount         || !mem ||
       !compressMemByteCount || !compressMem)
   {
      return miffBoolFALSE;
   }

   result                 = (MiffN4) ~Z_OK;
   minSize                = (uLongf) *compressMemByteCount;
   *compressMemByteCount  = 0;

   // Compress the buffer.
   result = compress2(
      (Bytef *) compressMem,
      &minSize,
      (Bytef const *) mem,
      (uLong) memByteCount,
      Z_DEFAULT_COMPRESSION);

   if (result != Z_OK)
   {
      return miffBoolFALSE;
   }

   *compressMemByteCount = minSize;

   return miffBoolTRUE;
}

/******************************************************************************
func: _MemDecompress
******************************************************************************/
MiffBool _MemDecompress(MiffN4 const compressMemByteCount, void const * const compressMem,
   MiffN4 * const memByteCount, void * const mem)
{
   MiffN4 result;
   uLongf size;

   if (!compressMem || !compressMemByteCount ||
       !mem         || !memByteCount)
   {
      return miffBoolFALSE;
   }

   size          = (uLongf) *memByteCount;
   *memByteCount = 0;

   // Uncompress the buffer.
   result = uncompress(
      (Bytef *) mem,
      &size,
      (Bytef const *) compressMem,
      (uLong) compressMemByteCount);

   if (result != Z_OK)
   {
      return miffBoolFALSE;
   }

   *memByteCount = size;

   return miffBoolTRUE;
}

/******************************************************************************
func: miffSetBuffer
******************************************************************************/
static MiffBool _SetBuffer(void * const dataRepo, MiffN4 const byteCount, MiffN1 const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _TestWrite
******************************************************************************/
static MiffBool _TestWrite(MiffC2 * const fileName, MiffMode const mode)
{
   FILE           *file;
   Miff           *miff;
   MiffValueType   idMatrix,
                   idPoint,
                   idPoly,
                   idBinary,
                   idBinaryZ;
   MiffI4          index;
   MiffN1          binary[] =
   {
        0,   0,   0,
        1,   1,   1,
        2,   2,   2,
        3,   3,   3,
        4,   4,   4,
        5,   5,   5,
        6,   6,   6,
        7,   7,   7,
        8,   8,   8,
        9,   9,   9,
       10,  10,  10,
       11,  11,  11,
       12,  12,  12,
       13,  13,  13,
       14,  14,  14,
       15,  15,  15,
       16,  16,  16,
       17,  17,  17,
       18,  18,  18,
       19,  19,  19,
       20,  20,  20,
       21,  21,  21,
       22,  22,  22,
       23,  23,  23,
       24,  24,  24,
       25,  25,  25,
       26,  26,  26,
       27,  27,  27,
       28,  28,  28,
       29,  29,  29,
       30,  30,  30,
       31,  31,  31,
       32,  32,  32,
       33,  33,  33,
       34,  34,  34,
       35,  35,  35,
       36,  36,  36,
       37,  37,  37,
       38,  38,  38,
       39,  39,  39,
       40,  40,  40,
       41,  41,  41,
       42,  42,  42,
       43,  43,  43,
       44,  44,  44,
       45,  45,  45,
       46,  46,  46,
       47,  47,  47,
       48,  48,  48,
       49,  49,  49,
       50,  50,  50,
       51,  51,  51,
       52,  52,  52,
       53,  53,  53,
       54,  54,  54,
       55,  55,  55,
       56,  56,  56,
       57,  57,  57,
       58,  58,  58,
       59,  59,  59,
       60,  60,  60,
       61,  61,  61,
       62,  62,  62,
       63,  63,  63,
       64,  64,  64,
       65,  65,  65,
       66,  66,  66,
       67,  67,  67,
       68,  68,  68,
       69,  69,  69,
       70,  70,  70,
       71,  71,  71,
       72,  72,  72,
       73,  73,  73,
       74,  74,  74,
       75,  75,  75,
       76,  76,  76,
       77,  77,  77,
       78,  78,  78,
       79,  79,  79,
       80,  80,  80,
       81,  81,  81,
       82,  82,  82,
       83,  83,  83,
       84,  84,  84,
       85,  85,  85,
       86,  86,  86,
       87,  87,  87,
       88,  88,  88,
       89,  89,  89,
       90,  90,  90,
       91,  91,  91,
       92,  92,  92,
       93,  93,  93,
       94,  94,  94,
       95,  95,  95,
       96,  96,  96,
       97,  97,  97,
       98,  98,  98,
       99,  99,  99,
      100, 100, 100,
      101, 101, 101,
      102, 102, 102,
      103, 103, 103,
      104, 104, 104,
      105, 105, 105,
      106, 106, 106,
      107, 107, 107,
      108, 108, 108,
      109, 109, 109,
      110, 110, 110,
      111, 111, 111,
      112, 112, 112,
      113, 113, 113,
      114, 114, 114,
      115, 115, 115,
      116, 116, 116,
      117, 117, 117,
      118, 118, 118,
      119, 119, 119,
      120, 120, 120,
      121, 121, 121,
      122, 122, 122,
      123, 123, 123,
      124, 124, 124,
      125, 125, 125,
      126, 126, 126,
      127, 127, 127,
      128, 128, 128,
      129, 129, 129,
      130, 130, 130,
      131, 131, 131,
      132, 132, 132,
      133, 133, 133,
      134, 134, 134,
      135, 135, 135,
      136, 136, 136,
      137, 137, 137,
      138, 138, 138,
      139, 139, 139,
      140, 140, 140,
      141, 141, 141,
      142, 142, 142,
      143, 143, 143,
      144, 144, 144,
      145, 145, 145,
      146, 146, 146,
      147, 147, 147,
      148, 148, 148,
      149, 149, 149,
      150, 150, 150,
      151, 151, 151,
      152, 152, 152,
      153, 153, 153,
      154, 154, 154,
      155, 155, 155,
      156, 156, 156,
      157, 157, 157,
      158, 158, 158,
      159, 159, 159,
      160, 160, 160,
      161, 161, 161,
      162, 162, 162,
      163, 163, 163,
      164, 164, 164,
      165, 165, 165,
      166, 166, 166,
      167, 167, 167,
      168, 168, 168,
      169, 169, 169,
      170, 170, 170,
      171, 171, 171,
      172, 172, 172,
      173, 173, 173,
      174, 174, 174,
      175, 175, 175,
      176, 176, 176,
      177, 177, 177,
      178, 178, 178,
      179, 179, 179,
      180, 180, 180,
      181, 181, 181,
      182, 182, 182,
      183, 183, 183,
      184, 184, 184,
      185, 185, 185,
      186, 186, 186,
      187, 187, 187,
      188, 188, 188,
      189, 189, 189,
      190, 190, 190,
      191, 191, 191,
      192, 192, 192,
      193, 193, 193,
      194, 194, 194,
      195, 195, 195,
      196, 196, 196,
      197, 197, 197,
      198, 198, 198,
      199, 199, 199,
      200, 200, 200,
      201, 201, 201,
      202, 202, 202,
      203, 203, 203,
      204, 204, 204,
      205, 205, 205,
      206, 206, 206,
      207, 207, 207,
      208, 208, 208,
      209, 209, 209,
      210, 210, 210,
      211, 211, 211,
      212, 212, 212,
      213, 213, 213,
      214, 214, 214,
      215, 215, 215,
      216, 216, 216,
      217, 217, 217,
      218, 218, 218,
      219, 219, 219,
      220, 220, 220,
      221, 221, 221,
      222, 222, 222,
      223, 223, 223,
      224, 224, 224,
      225, 225, 225,
      226, 226, 226,
      227, 227, 227,
      228, 228, 228,
      229, 229, 229,
      230, 230, 230,
      231, 231, 231,
      232, 232, 232,
      233, 233, 233,
      234, 234, 234,
      235, 235, 235,
      236, 236, 236,
      237, 237, 237,
      238, 238, 238,
      239, 239, 239,
      240, 240, 240,
      241, 241, 241,
      242, 242, 242,
      243, 243, 243,
      244, 244, 244,
      245, 245, 245,
      246, 246, 246,
      247, 247, 247,
      248, 248, 248,
      249, 249, 249,
      250, 250, 250,
      251, 251, 251,
      252, 252, 252,
      253, 253, 253,
      254, 254, 254,
      255, 255, 255
   };
   MiffN1          n1array[] =
   {
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
   };
   MiffN2          n2array[] =
   {
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
   };
   MiffN4          n4array[] =
   {
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
   };
   MiffN8          n8array[] =
   {
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
   };
   MiffR4          reals4[300] =
   {
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.14259,
   };
   MiffR8          reals8[300] =
   {
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.14259,
   };
   MiffC2         *strings[10] =
   {
      L"the quick brown fox jumped over the lazy dog.",
      L"THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.",
      L"I fart in your general direction.",
      L"No one expects the Spanish Inquisition!",
      L"And now for something completely different.",
      L"the quick brown fox jumped over the lazy dog.\tTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.",
      L"THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.\nthe quick brown fox jumped over the lazy dog.",
      L"I fart in your general direction.",
      L"No one expects the Spanish Inquisition!",
      L"And now for something completely different."
   };
   MiffBool        bools[100] =
   {
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
      miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolTRUE, miffBoolTRUE, miffBoolFALSE, miffBoolFALSE, miffBoolFALSE, miffBoolTRUE, miffBoolFALSE,
   };

   file = NULL;
   miff = NULL;

   for(;;)
   {
      if (_wfopen_s(&file, fileName, L"wb") != 0)
      {
         break;
      }

      // Create a miff file.
      miff = miffCreateWriter(miffBoolTRUE, _SetBuffer, mode, L"MiffTestFile", 1, (void *) file);
      if (!miff)
      {
         break;
      }

      miffSet1Boolean(      miff, L"True",     miffBoolTRUE);
      miffSet1Boolean(      miff, L"False",    miffBoolFALSE);

      miffSet1I1(           miff, L"I1_0",     0);
      miffSet1I1(           miff, L"I1_1",     1);
      miffSet1I1(           miff, L"I1_-1",    -1);
      miffSet1I1(           miff, L"I1_127",   127);
      miffSet1I1(           miff, L"I1_-128",  -128);

      miffSet1N1(           miff, L"N1_0",     0);
      miffSet1N1(           miff, L"N1_1",     1);
      miffSet1N1(           miff, L"N1_255",   255);

      miffSet1I2(           miff, L"I2_0",     0);
      miffSet1I2(           miff, L"I2_1",     1);
      miffSet1I2(           miff, L"I2_-1",    -1);

      miffSet1N2(           miff, L"N2_0",     0);
      miffSet1N2(           miff, L"N2_1",     1);

      miffSet1I4(           miff, L"I4_0",     0);
      miffSet1I4(           miff, L"I4_1",     1);
      miffSet1I4(           miff, L"I4_-1",    -1);

      miffSet1N4(           miff, L"N4_0",     0);
      miffSet1N4(           miff, L"N4_1",     1);

      miffSet1R4(           miff, L"R4_0",     0.0);
      miffSet1R4(           miff, L"R4_1",     1.0);
      miffSet1R4(           miff, L"R4_-1",    -1.0);
      miffSet1R4(           miff, L"R4_PI",    3.14159f);

      miffSet1I8(           miff, L"I8_0",     0);
      miffSet1I8(           miff, L"I8_1",     1);
      miffSet1I8(           miff, L"I8_-1",    -1);

      miffSet1N8(           miff, L"N8_0",     0);
      miffSet1N8(           miff, L"N8_1",     1);

      miffSet1R8(           miff, L"R8_0",     0.0);
      miffSet1R8(           miff, L"R8_1",     1.0);
      miffSet1R8(           miff, L"R8_-1",    -1.0);
      miffSet1R8(           miff, L"R8_PI",    3.14159);

      miffSet1Type(         miff, L"TypeBool", miffValueTypeBOOLEAN);
      miffSet1Type(         miff, L"TypeKey",  miffValueTypeTYPE);

      miffSet1StringC2(     miff, L"String",   L"The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

      miffSetNI1(           miff, L"I1_Array",     256,     (MiffI1 *) n1array);
      miffSetNN1(           miff, L"N1_Array",     256,     n1array);
      miffRecordSetBegin(   miff, miffValueTypeI1, L"I1 Array Compressed", 256 * 3, miffCompressFlagCHUNK_COMPRESS, 9999);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueI1(                 miff, ((MiffI1 *) n1array)[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      miffRecordSetEnd(miff);

      miffSetNI2(           miff, L"I2_Array",     256,     (MiffI2 *) n2array);
      miffSetNN2(           miff, L"N2_Array",     256,     n2array);
      miffRecordSetBegin(   miff, miffValueTypeI2, L"I2 Array Compressed", 256 * 3, miffCompressFlagCHUNK_COMPRESS, 9999);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueI2(                 miff, ((MiffI2 *) n2array)[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      miffRecordSetEnd(miff);

      miffSetNI4(           miff, L"I4_Array",     256,     (MiffI4 *) n4array);
      miffSetNN4(           miff, L"N4_Array",     256,     n4array);
      miffRecordSetBegin(   miff, miffValueTypeI4, L"I4 Array Compressed", 256 * 3, miffCompressFlagCHUNK_COMPRESS, 9999);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueI4(                 miff, ((MiffI4 *) n4array)[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      miffRecordSetEnd(miff);

      miffSetNI8(           miff, L"I8_Array",     256,     (MiffI8 *) n8array);
      miffSetNN8(           miff, L"N8_Array",     256,     n8array);
      miffRecordSetBegin(   miff, miffValueTypeI8, L"I8 Array Compressed", 256 * 3, miffCompressFlagCHUNK_COMPRESS, 4000);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueI8(                 miff, ((MiffI8 *) n8array)[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      miffRecordSetEnd(miff);

      miffSetNR4(           miff, L"R4_Array",     300,     reals4);
      miffSetNR8(           miff, L"R8_Array",     300,     reals8);
      miffRecordSetBegin(   miff, miffValueTypeR8, L"R8 Array Compressed", 300, miffCompressFlagCHUNK_COMPRESS, 9999);
      for (index = 0; index < 300; index++)
      {
         miffSetValueR8(                 miff, reals8[index]);
         miffRecordSetNextValueWithCheck(miff, index, 300);
      }
      miffRecordSetEnd(miff);

      miffSetNStringC2(     miff, L"String_Array", 10,   strings);

      miffSetNBoolean(      miff, L"Bool_Array",   100,  bools);

      idMatrix = miffValueTypeFIRST_USER_TYPE + 0;
      miffSetDefineHeader(  miff, idMatrix, L"Matrix", 1);
      {
         miffSetDefineValue(miff, miffValueTypeR4,       L"Cell",  16, miffCompressFlagNONE, 0);
      }

      idPoint  = miffValueTypeFIRST_USER_TYPE + 1;
      miffSetDefineHeader(  miff, idPoint, L"Point",  4);
      {
         miffSetDefineValue(miff, miffValueTypeSTRING,   L"ID",    1, miffCompressFlagNONE, 0);
         miffSetDefineValue(miff, miffValueTypeR4,       L"E",     1, miffCompressFlagNONE, 0);
         miffSetDefineValue(miff, miffValueTypeR4,       L"N",     1, miffCompressFlagNONE, 0);
         miffSetDefineValue(miff, miffValueTypeR4,       L"Z",     1, miffCompressFlagNONE, 0);
      }

      idPoly = miffValueTypeFIRST_USER_TYPE + 2;
      miffSetDefineHeader(  miff, idPoly, L"Poly", 2);
      {
         miffSetDefineValue(miff, miffValueTypeSTRING,   L"ID",    1,                     miffCompressFlagNONE, 0);
         miffSetDefineValue(miff, idPoint,               L"PList", miffArrayCountUNKNOWN, miffCompressFlagNONE, 0);
      }

      idBinary = miffValueTypeFIRST_USER_TYPE + 3;
      miffSetDefineHeader(  miff, idBinary, L"Blob",   2);
      {
         miffSetDefineValue(miff, miffValueTypeN4,       L"Count", 1,                     miffCompressFlagNONE, 0);
         miffSetDefineValue(miff, miffValueTypeN1,       L"Bytes", miffArrayCountUNKNOWN, miffCompressFlagNONE, 0);
      }

      idBinaryZ = miffValueTypeFIRST_USER_TYPE + 4;
      miffSetDefineHeader(  miff, idBinaryZ, L"BlobZ",   2);
      {
         miffSetDefineValue(miff, miffValueTypeN4,       L"Count", 1,                     miffCompressFlagNONE,           0);
         miffSetDefineValue(miff, miffValueTypeN1,       L"Bytes", miffArrayCountUNKNOWN, miffCompressFlagCHUNK_COMPRESS, 9999);
      }

      //miffRecordSetBegin(miff, idPoly, L"1Poly", 1, miffCompressFlagNONE, 0);

      miffRecordSetBegin(    miff, idBinary, L"BinValue", 1, miffCompressFlagNONE, 0);
      miffSetValueN4(        miff, 256 * 3);
      miffRecordSetNextValue(miff);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueN1(                 miff, binary[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      // Required because in the define Bytes is an unknown size.
      miffRecordSetEnd(      miff);

      miffRecordSetBegin(    miff, idBinary, L"BinValueArray", 2, miffCompressFlagNONE, 0);
      miffSetValueN4(        miff, 256 * 3);
      miffRecordSetNextValue(miff);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueN1(                 miff, binary[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      miffRecordSetNextVar(  miff);
      // Required because in the define Bytes is an unknown size.
      miffSetValueN4(        miff, 256 * 3);
      miffRecordSetNextValue(miff);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueN1(                 miff, n1array[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      // Required because in the define Bytes is an unknown size.
      miffRecordSetEnd(      miff);

      miffRecordSetBegin(    miff, idBinary, L"BinValueArray", 2, miffCompressFlagCHUNK_COMPRESS, 9999);
      // Required because in the define Bytes is an unknown size.
      miffSetValueN4(        miff, 256 * 3);
      miffRecordSetNextValue(miff);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueN1(                 miff, binary[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      miffRecordSetNextVar(  miff);
      // Required because in the define Bytes is an unknown size.
      miffSetValueN4(        miff, 256 * 3);
      miffRecordSetNextValue(miff);
      for (index = 0; index < 256 * 3; index++)
      {
         miffSetValueN1(                 miff, n1array[index]);
         miffRecordSetNextValueWithCheck(miff, index, 256 * 3);
      }
      // Required because in the define Bytes is an unknown size.
      miffRecordSetEnd(      miff);

      miffSetBlockStart(miff, L"KeyValueBlock");
      {
         miffSet1Boolean(      miff, L"True",     miffBoolTRUE);
         miffSet1Boolean(      miff, L"False",    miffBoolFALSE);

         miffSet1I1(           miff, L"I1_0",     0);
         miffSet1I1(           miff, L"I1_1",     1);
         miffSet1I1(           miff, L"I1_-1",    -1);
         miffSet1I1(           miff, L"I1_127",   127);
         miffSet1I1(           miff, L"I1_-128",  -128);

         miffSet1N1(           miff, L"N1_0",     0);
         miffSet1N1(           miff, L"N1_1",     1);
         miffSet1N1(           miff, L"N1_255",   255);

         miffSet1I2(           miff, L"I2_0",     0);
         miffSet1I2(           miff, L"I2_1",     1);
         miffSet1I2(           miff, L"I2_-1",    -1);

         miffSet1N2(           miff, L"N2_0",     0);
         miffSet1N2(           miff, L"N2_1",     1);

         miffSet1I4(           miff, L"I4_0",     0);
         miffSet1I4(           miff, L"I4_1",     1);
         miffSet1I4(           miff, L"I4_-1",    -1);

         miffSet1N4(           miff, L"N4_0",     0);
         miffSet1N4(           miff, L"N4_1",     1);

         miffSet1R4(           miff, L"R4_0",     0.0);
         miffSet1R4(           miff, L"R4_1",     1.0);
         miffSet1R4(           miff, L"R4_-1",    -1.0);
         miffSet1R4(           miff, L"R4_PI",    3.14159f);

         miffSet1I8(           miff, L"I8_0",     0);
         miffSet1I8(           miff, L"I8_1",     1);
         miffSet1I8(           miff, L"I8_-1",    -1);

         miffSet1N8(           miff, L"N8_0",     0);
         miffSet1N8(           miff, L"N8_1",     1);

         miffSet1R8(           miff, L"R8_0",     0.0);
         miffSet1R8(           miff, L"R8_1",     1.0);
         miffSet1R8(           miff, L"R8_-1",    -1.0);
         miffSet1R8(           miff, L"R8_PI",    3.14159);

         miffSet1Type(         miff, L"TypeBool", miffValueTypeBOOLEAN);
         miffSet1Type(         miff, L"TypeType", miffValueTypeTYPE);

         miffSet1StringC2(     miff, L"String",   L"The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

         miffSetNStringC2(     miff, L"String_Array", 10,      strings);
      }
      miffSetBlockStop(miff);

      break;
   }

   miffDestroy(miff);
   fclose(file);

   return miffBoolTRUE;
}
