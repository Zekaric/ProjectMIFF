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
static Gb       _BlockWrite(           GmineInfo       * const gmineInfo);
static Gb       _BlockWriteData(       GmineInfo       * const gmineInfo);
static Gb       _BlockWriteDrillHole(  GmineInfo       * const gmineInfo);
static Gb       _BlockWriteGeometry(   GmineInfo       * const gmineInfo);
static Gb       _BlockWriteItem(       GmineInfo       * const gmineInfo);
static Gb       _BlockWriteModel(      GmineInfo       * const gmineInfo);

static Gb       _GetBuffer(            void * const dataRepo, Gcount const byteCount, Gn1       * const byteData);

static void    *_MemCloc(              Gcount const memByteCount);
static void     _MemDloc(              void * const mem);

static Gb       _ReadMiJson(           void);
static Gb       _ReadMiMiff(           void);

static Gb       _SetBuffer(            void * const dataRepo, Gcount const byteCount, Gn1 const * const byteData);

static Gb       _WriteMiJson(          void);
static Gb       _WriteMiMiff(          void);

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: main
**************************************************************************************************/
int main(int acount, char ** alist)
{
   GmineInfo *gmineInfo;
   FILE      *file;
   char      *msg;

   acount;
   alist;
   gmineInfo   = NULL;
   file = NULL;

   returnIf(!gmineInfoStart(_MemCloc, _MemDloc), 1);

   breakScope
   {
      // Create a MIMIFF file.
      msg = "MIMIFF Create";
      breakIf(!_WriteMiMiff());

      // Create a MIJSON file.
      msg = "MIJSON Create";
      breakIf(!_WriteMiJson());

      // Read in the MIMIFF file.
      msg = "MIMIFF Read";
      breakIf(!_ReadMiMiff());

      // Read in the MIJSON file.
      msg = "MIJSON Read";
      breakIf(!_ReadMiJson());

      // No errors if we reached here.
      msg = "NO";
   }

   printf("MI: %s ERROR.\n", msg);

   // Clean up
   gmineInfoDloc(gmineInfo);

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
func: _BlockWrite
**************************************************************************************************/
static Gb _BlockWrite(GmineInfo * const gmineInfo)
{
   GmineInfoBlockType block;

   // For all the blocks...
   loop
   {
      // Get the block we are writing out now.
      block = gmineInfoBlockTypeNext(gmineInfo);
      breakIf(block == gmineInfoBlockType_END);

      // Write out the block.
      if      (block == gmineInfoBlockTypeDATA)
      {
         breakIf(!_BlockWriteData(gmineInfo));
      }
      else if (block == gmineInfoBlockTypeITEM)
      {
         breakIf(!_BlockWriteItem(gmineInfo));
      }
      else if (block == gmineInfoBlockTypeGEOMETRY)
      {
         breakIf(!_BlockWriteGeometry(gmineInfo));
      }
      else if (block == gmineInfoBlockTypeDRILL_HOLE)
      {
         breakIf(!_BlockWriteDrillHole(gmineInfo));
      }
      else if (block == gmineInfoBlockTypeMODEL)
      {
         breakIf(!_BlockWriteModel(gmineInfo));
      }
   }

   returnTrue;
}

/**************************************************************************************************
func: _BlockWriteData
**************************************************************************************************/
static Gb _BlockWriteData(GmineInfo * const gmineInfo)
{
   GmineInfoPoint min = {1000, 2000, 3000},
                  max = {2500, 3500, 3100};

   gmineInfoSetDataAuthorName(   gmineInfo, "Robbert de Groot");
   gmineInfoSetDataCompanyName(  gmineInfo, "Zekaric");
   gmineInfoSetDataCopyright(    gmineInfo, "2025 (c) Robbert de Groot");
   gmineInfoSetDataProjectMax(   gmineInfo, &max);
   gmineInfoSetDataProjectMin(   gmineInfo, &min);
   gmineInfoSetDataProjectName(  gmineInfo, "Mines Of Moria");
   gmineInfoSetDataProjectSystem(gmineInfo, gmineInfoPROJECT_SYSTEM_LOCAL);
   gmineInfoSetDataOther(        gmineInfo, "project lead",  "Ghimli");
   gmineInfoSetDataOther(        gmineInfo, "project sme",   "Gandalf The Grey");
   gmineInfoSetDataOther(        gmineInfo, "project theif", "Bilbo Baggins");
   gmineInfoSetDataOther(        gmineInfo, "risk 1",        "Gollum");
   gmineInfoSetDataOther(        gmineInfo, "risk 2",        "The Balrog");

   return gmineInfoWriteData(gmineInfo);
}

/**************************************************************************************************
func: _BlockWriteDrillHole
**************************************************************************************************/
static Gb _BlockWriteDrillHole(GmineInfo * const gmineInfo)
{
   gmineInfo;
   returnTrue;
}

/**************************************************************************************************
func: _BlockWriteGeometry
**************************************************************************************************/
static Gb _BlockWriteGeometry(GmineInfo * const gmineInfo)
{
   gmineInfo;
   returnTrue;
}

/**************************************************************************************************
func: _BlockWriteItem
**************************************************************************************************/
static Gb _BlockWriteItem(GmineInfo * const gmineInfo)
{
   GmineInfoItem *item;

   // Create the list of items.
   item = gmineInfoItemCloc();
   returnFalseIf(!item);

   returnFalseIf(!gmineInfoItemSetName(    item, "copper"));
   returnFalseIf(!gmineInfoItemSetKey(     item, "CU"));
   returnFalseIf(!gmineInfoItemSetType(    item, gmineInfoItemTypeR));
   returnFalseIf(!gmineInfoItemSetOther(   item, "Comment", "Cold Copper Tears."));
   returnFalseIf(!gmineInfoItemSetOther(   item, "unit",    "ppm"));
   // No default set, then default value is "missing".
   // No min or max set, then min and max are full range of reals.

   returnFalseIf(!gmineInfoSetItemAppend(gmineInfo, item));


   item = gmineInfoItemCloc();
   returnFalseIf(!item);

   returnFalseIf(!gmineInfoItemSetName(    item, "gold"));
   returnFalseIf(!gmineInfoItemSetKey(     item, "AU"));
   returnFalseIf(!gmineInfoItemSetType(    item, gmineInfoItemTypeR));
   returnFalseIf(!gmineInfoItemSetOther(   item, "Saying",  "All that glitters is not gold."));
   returnFalseIf(!gmineInfoItemSetOther(   item, "unit",    "ppm"));
   returnFalseIf(!gmineInfoItemSetDefaultR(item, 0.0));
   returnFalseIf(!gmineInfoItemSetMinR(    item, 0.0));
   returnFalseIf(!gmineInfoItemSetMaxR(    item, 5.0));

   returnFalseIf(!gmineInfoSetItemAppend(gmineInfo, item));


   item = gmineInfoItemCloc();
   returnFalseIf(!item);

   returnFalseIf(!gmineInfoItemSetName(    item, "topography percent"));
   returnFalseIf(!gmineInfoItemSetKey(     item, "topo%"));
   returnFalseIf(!gmineInfoItemSetType(    item, gmineInfoItemTypePERCENT));
   returnFalseIf(!gmineInfoItemSetOther(   item, "Saying",  "Top o' da' world baby!"));
   returnFalseIf(!gmineInfoItemSetDefaultR(item, 100.0));

   returnFalseIf(!gmineInfoSetItemAppend(gmineInfo, item));


   item = gmineInfoItemCloc();
   returnFalseIf(!item);

   returnFalseIf(!gmineInfoItemSetName(    item, "rock type"));
   returnFalseIf(!gmineInfoItemSetKey(     item, "rtype"));
   returnFalseIf(!gmineInfoItemSetType(    item, gmineInfoItemTypeN));
   returnFalseIf(!gmineInfoItemSetOther(   item, "ship",    "It go pew pew!"));

   returnFalseIf(!gmineInfoSetItemAppend(gmineInfo, item));

   returnTrue;
}

/**************************************************************************************************
func: _BlockWriteModel
**************************************************************************************************/
static Gb _BlockWriteModel(GmineInfo * const gmineInfo)
{
   gmineInfo;
   returnTrue;
}

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
func: _ReadMiJson
**************************************************************************************************/
static Gb _ReadMiJson(void)
{
   GmineInfo *gmineInfo;
   FILE      *file;
   Gb         result;

   gmineInfo = NULL;
   result    = gbFALSE;
   breakScope
   {
      fopen_s(&file, "test.mijson", "rb");
      breakIf(!file);

      gmineInfo = gmineInfoClocReader(gmineInfoFileTypeJSON, _GetBuffer, (void *) file);
      breakIf(!gmineInfo);

      result = gbTRUE;
   }

   // Clean up
   gmineInfoDloc(gmineInfo);

   if (file)
   {
      fclose(file);
   }

   return result;
}

/**************************************************************************************************
func: _ReadMiMiff
**************************************************************************************************/
static Gb _ReadMiMiff(void)
{
   GmineInfo *gmineInfo;
   FILE      *file;
   Gb         result;

   gmineInfo = NULL;
   result    = gbFALSE;
   breakScope
   {
      fopen_s(&file, "test.mimiff", "rb");
      breakIf(!file);

      gmineInfo = gmineInfoClocReader(gmineInfoFileTypeMIFF, _GetBuffer, (void *) file);
      breakIf(!gmineInfo);

      result = gbTRUE;
   }

   // Clean up
   gmineInfoDloc(gmineInfo);

   if (file)
   {
      fclose(file);
   }

   return result;
}

/**************************************************************************************************
func: _SetBuffer
**************************************************************************************************/
static Gb _SetBuffer(void * const dataRepo, Gcount const byteCount, Gn1 const * const byteData)
{
   returnFalseIf(byteCount <= 0);

   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == (size_t) byteCount);
}

/**************************************************************************************************
func: _WriteMiJson
**************************************************************************************************/
static Gb _WriteMiJson(void)
{
   GmineInfo *gmineInfo;
   FILE      *file;
   Gb         result;

   gmineInfo = NULL;
   result    = gbFALSE;
   breakScope
   {
      fopen_s(&file, "test.mijson", "wb");
      breakIf(!file);

      gmineInfo = gmineInfoClocWriter(gmineInfoFileTypeJSON, _SetBuffer, (void *) file);
      breakIf(!gmineInfo);

      breakIf(!_BlockWrite(gmineInfo));

      result = gbTRUE;
   }

   // Clean up
   gmineInfoDloc(gmineInfo);

   if (file)
   {
      fclose(file);
   }

   return result;
}

/**************************************************************************************************
func: _WriteMiMiff
**************************************************************************************************/
static Gb _WriteMiMiff(void)
{
   GmineInfo         *gmineInfo;
   FILE              *file;
   Gb                 result;

   gmineInfo = NULL;
   result    = gbFALSE;
   breakScope
   {
      fopen_s(&file, "test.mimiff", "wb");
      breakIf(!file);

      gmineInfo = gmineInfoClocWriter(gmineInfoFileTypeMIFF, _SetBuffer, (void *) file);
      breakIf(!gmineInfo);

      breakIf(!_BlockWrite(gmineInfo));

      result = gbTRUE;
   }

   // Clean up
   gmineInfoDloc(gmineInfo);

   if (file)
   {
      fclose(file);
   }

   return result;
}

