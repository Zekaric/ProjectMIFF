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

static MiffBool _SetBuffer(      void * const dataRepo, MiffN4 const byteCount, Miff1 const * const byteData);

static MiffBool _TestWrite(      MiffC2 * const fileName, MiffMode const mode);

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
         _SetBuffer,
         mode,
         (MiffN1) _C2GetCount(L"MiffTestFile"),
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

      miffSetString(miff, L"String", L"The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

      break;
   }

   miffDestroy(miff);
   fclose(file);

   return miffBoolTRUE;
}
