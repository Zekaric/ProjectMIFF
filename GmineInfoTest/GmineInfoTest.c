/**************************************************************************************************
file:       GmineInfoTest
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Simple test of the Mining Information library
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

#include "GmineInfoLib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/

/**************************************************************************************************
type:
**************************************************************************************************/

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/
static Gb       _GetBuffer(         void * const dataRepo, Gcount const byteCount, Gn1       * const byteData);

static void    *_MemCloc(           Gcount const memByteCount);
static void     _MemDloc(           void * const mem);

static Gb       _SetBuffer(         void * const dataRepo, Gcount const byteCount, Gn1 const * const byteData);

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: main
**************************************************************************************************/
int main(int acount, char ** alist)
{
   GmineInfo *mi;
   FILE      *file;
   char      *msg;

   acount;
   alist;
   mi   = NULL;
   file = NULL;

   returnIf(!gmineInfoStart(_MemCloc, _MemDloc), 1);

   breakScope
   {
      // Create a MIMIFF file.
      msg = "MIMIFF Create";
      fopen_s(&file, "test.mimiff", "wb");
      breakIf(!file);

      mi = gmineInfoClocWriter(gmineInfoFileTypeMIFF, _SetBuffer, (void *) file);
      breakIf(!mi);

      gmineInfoDloc(mi);
      mi = NULL;

      fclose(file);
      file = NULL;

      // Create a MIJSON file.
      msg = "MIJSON Create";
      fopen_s(&file, "test.mijson", "wb");
      breakIf(!file);

      mi = gmineInfoClocWriter(gmineInfoFileTypeJSON, _SetBuffer, (void *) file);
      breakIf(!mi);

      gmineInfoDloc(mi);
      mi = NULL;

      fclose(file);
      file = NULL;

      // Read in the MIMIFF file.
      msg = "MIMIFF Read";
      fopen_s(&file, "test.mimiff", "rb");
      breakIf(!file);

      mi = gmineInfoClocReader(gmineInfoFileTypeMIFF, _GetBuffer, (void *) file);
      breakIf(!mi);

      gmineInfoDloc(mi);
      mi = NULL;

      fclose(file);
      file = NULL;

      // Read in the MIJSON file.
      msg = "MIJSON Read";
      fopen_s(&file, "test.mijson", "rb");
      breakIf(!file);

      mi = gmineInfoClocReader(gmineInfoFileTypeJSON, _GetBuffer, (void *) file);
      breakIf(!mi);

      gmineInfoDloc(mi);
      mi = NULL;

      fclose(file);
      file = NULL;

      // No errors if we reached here.
      msg = "NO";
   }

   printf("MI: %s ERROR.\n", msg);

   // Clean up
   gmineInfoDloc(mi);

   if (file)
   {
      fclose(file);
   }

   gmineInfoStop();

   return 0;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _GetBuffer
**************************************************************************************************/
static Gb _GetBuffer(void * const dataRepo, Gcount const byteCount, Gn1 * const byteData)
{
   returnFalseIf(byteCount <= 0);

   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == (size_t) byteCount);
}

/**************************************************************************************************
func: _MemCloc
**************************************************************************************************/
void *_MemCloc(Gcount const memByteCount)
{
   returnNullIf(memByteCount <= 0);

   return calloc(1, (size_t) memByteCount);
}

/**************************************************************************************************
func: _MemDloc
**************************************************************************************************/
void _MemDloc(void * const mem)
{
   returnVoidIf(!mem);

   free(mem);
}

/**************************************************************************************************
func: _SetBuffer
**************************************************************************************************/
static Gb _SetBuffer(void * const dataRepo, Gcount const byteCount, Gn1 const * const byteData)
{
   returnFalseIf(byteCount <= 0);

   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == (size_t) byteCount);
}

