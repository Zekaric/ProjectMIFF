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
static void    *_MemCreate(      MiffN4 const memByteCount);
static void     _MemDestroy(     void * const mem);
static MiffBool _MemCompress(    MiffN4 const memByteCount, void const * const mem, MiffN4 * const compressMemByteCount, MiffN1 ** const compressMem);
static MiffBool _MemDecompress(  MiffN4 const compressMemByteCount, MiffN1 const * const compressMem, MiffN4 * const memByteCount, void ** const mem);

static MiffBool _SetBuffer1(     void * const dataRepo, MiffN4 const data1Count, Miff1 const * const data1);
static MiffBool _SetBuffer2(     void * const dataRepo, MiffN4 const data2Count, Miff2 const * const data2);
static MiffBool _SetBuffer4(     void * const dataRepo, MiffN4 const data4Count, Miff4 const * const data4);
static MiffBool _SetBuffer8(     void * const dataRepo, MiffN4 const data8Count, Miff8 const * const data8);

static MiffBool _TestWrite(      MiffC * const fileName, MiffMode const mode);

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
   if (!miffStart(_MemCreate, _MemDestroy, _MemCompress, _MemDecompress))
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

   if (!_TestWrite(L"MiffFileTestBin.miff", miffModeBINARY))
   {
      result = 3;
      goto DONE;
   }

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
func: _MemCompress
******************************************************************************/
MiffBool _MemCompress(MiffN4 const memByteCount, void const * const mem, MiffN4 * const compressMemByteCount, MiffN1 ** const compressMem)
{
   uLongf minSize;
   MiffN4 result;

   if (!memByteCount         || !mem ||
       !compressMemByteCount || compressMem)
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
MiffBool _MemDecompress(MiffN4 const compressMemByteCount, MiffN1 const * const compressMem, MiffN4 * const memByteCount, void ** const mem)
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
func: miffSetBuffer1
******************************************************************************/
static MiffBool _SetBuffer1(void * const dataRepo, MiffN4 const data1Count, Miff1 const * const data1)
{
   return (_fwrite_nolock(data1, 1, data1Count, (FILE *) dataRepo) == data1Count);
}

/******************************************************************************
func: miffSetBuffer2
******************************************************************************/
static MiffBool _SetBuffer2(void * const dataRepo, MiffN4 const data2Count, Miff2 const * const data2)
{
   return (_fwrite_nolock(data2, 2, data2Count, (FILE *) dataRepo) == data2Count);
}

/******************************************************************************
func: miffSetBuffer4
******************************************************************************/
static MiffBool _SetBuffer4(void * const dataRepo, MiffN4 const data4Count, Miff4 const * const data4)
{
   return (_fwrite_nolock(data4, 4, data4Count, (FILE *) dataRepo) == data4Count);
}

/******************************************************************************
func: miffSetBuffer8
******************************************************************************/
static MiffBool _SetBuffer8(void * const dataRepo, MiffN4 const data8Count, Miff8 const * const data8)
{
   return (_fwrite_nolock(data8, 8, data8Count, (FILE *) dataRepo) == data8Count);
}

/******************************************************************************
func: _TestWrite
******************************************************************************/
static MiffBool _TestWrite(MiffC * const fileName, MiffMode const mode)
{
   FILE *file;
   Miff *miff;

   file = NULL;
   miff = NULL;

   for (;;)
   {
      if (_wfopen_s(&file, fileName, L"wb") != 0)
      {
         break;
      }

      // Create a miff file.
      miff = miffCreateWriter(
         miffBoolTRUE,
         _SetBuffer1,
         _SetBuffer2,
         _SetBuffer4,
         _SetBuffer8,
         mode,
         (MiffN1) wcslen(L"MiffTestFile"),
         L"MiffTestFile",
         1,
         (void *) file);
      if (!miff)
      {
         break;
      }

      miffSetBoolean(miff, L"True",  miffBoolTRUE);
      miffSetBoolean(miff, L"False", miffBoolFALSE);

      miffSetI1(miff, L"I1_0",       0);
      miffSetI1(miff, L"I1_1",       1);
      miffSetI1(miff, L"I1_-1",     -1);
      miffSetI1(miff, L"I1_127",   127);
      miffSetI1(miff, L"I1_-128", -128);

      miffSetN1(miff, L"N1_0",       0);
      miffSetN1(miff, L"N1_1",       1);
      miffSetN1(miff, L"N1_255",   255);

      miffSetI2(miff, L"I2_0",       0);
      miffSetI2(miff, L"I2_1",       1);
      miffSetI2(miff, L"I2_-1",     -1);

      miffSetN2(miff, L"N2_0",       0);
      miffSetN2(miff, L"N2_1",       1);

      miffSetI4(miff, L"I4_0",       0);
      miffSetI4(miff, L"I4_1",       1);
      miffSetI4(miff, L"I4_-1",     -1);

      miffSetN4(miff, L"N4_0",       0);
      miffSetN4(miff, L"N4_1",       1);

      miffSetR4(miff, L"R4_0",       0.0);
      miffSetR4(miff, L"R4_1",       1.0);
      miffSetR4(miff, L"R4_-1",     -1.0);
      miffSetR4(miff, L"R4_PI",      3.14159f);

      miffSetI8(miff, L"I8_0",       0);
      miffSetI8(miff, L"I8_1",       1);
      miffSetI8(miff, L"I8_-1",     -1);

      miffSetN8(miff, L"N8_0",       0);
      miffSetN8(miff, L"N8_1",       1);

      miffSetR8(miff, L"R8_0",       0.0);
      miffSetR8(miff, L"R8_1",       1.0);
      miffSetR8(miff, L"R8_-1",     -1.0);
      miffSetR8(miff, L"R8_PI",      3.14159);

      miffSetType(miff, L"TypeBool", miffValueTypeBOOLEAN);
      miffSetType(miff, L"TypeKey",  miffValueTypeKEY_ONLY_NO_VALUE);

      break;
   }

   miffDestroy(miff);
   fclose(file);

   return miffBoolTRUE;
}
