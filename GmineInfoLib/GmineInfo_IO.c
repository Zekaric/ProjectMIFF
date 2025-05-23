/**************************************************************************************************
file:       GmineInfo_IO
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Handles the writing of information to the output in the desired format.
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "GmineInfo_local.h"

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

/**************************************************************************************************
global:
function:
**************************************************************************************************/
Gb _MiIoClocReader(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      gmineInfo->miffFile = gmiffClocReader(gmineInfo->getBuffer, gmineInfo->dataRepo);
      returnFalseIf(
         !gmineInfo->miffFile                                                         ||
         !_MiStrIsEqual(gmiffGetSubFormatName(gmineInfo->miffFile), HEADER_TYPE_STR)  ||
         gmiffGetSubFormatVersion(gmineInfo->miffFile) != HEADER_VERSION_NUM);
   }
   // JSON
   else
   {
      GjsonType type;
      Gb        isTypeOk,
                isVersionOk;
      Gn8       version;
      Gstr      headerTypeStr[gjsonCountDEFAULT + 1];

      _MiMemClearTypeArray(headerTypeStr, Gstr, gjsonCountDEFAULT + 1);

      isTypeOk    = gbFALSE;
      isVersionOk = gbFALSE;

      breakScope
      {
         gmineInfo->jsonFile = gjsonClocReader(gmineInfo->getBuffer, gmineInfo->dataRepo);
         breakIf(!gmineInfo->jsonFile);

         // Get the parent object.
         type = gjsonGetType_FileElement(gmineInfo->jsonFile);
         breakIf(type != gjsonTypeOBJECT_START);

         // Get the format block which should be the first block in the JSON.
         breakIf(gjsonGetType_ObjectKeyOrStop(gmineInfo->jsonFile) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(gmineInfo->jsonFile, KEY_FORMAT_STR));
         breakIf(gjsonGetType_ObjectValue(gmineInfo->jsonFile) != gjsonTypeOBJECT_START);
         {
            loop
            {
               type = gjsonGetType_ObjectKeyOrStop(gmineInfo->jsonFile);
               breakIf(type == gjsonTypeOBJECT_STOP);
               breakIf(type != gjsonTypeOBJECT_KEY);

               if      (gjsonIsKeyEqual(gmineInfo->jsonFile, KEY_NAME_STR))
               {
                  breakIf(!gjsonIsTypeStr(gjsonGetType_ObjectValue(gmineInfo->jsonFile)));
                  breakIf(!gjsonGetStr(gmineInfo->jsonFile, gjsonCountDEFAULT, headerTypeStr));
                  breakIf(!_MiStrIsEqual(headerTypeStr, HEADER_TYPE_STR));

                  isTypeOk = gbTRUE;
               }
               else if (gjsonIsKeyEqual(gmineInfo->jsonFile, KEY_VERSION_STR))
               {
                  breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(gmineInfo->jsonFile)));
                  breakIf(!gjsonGetN(gmineInfo->jsonFile, &version));
                  breakIf(version != HEADER_VERSION_NUM);

                  isVersionOk = gbTRUE;
               }
               else
               {
                  // Something unexpected inside the format block.
                  break;
               }
            }
         }
      }

      // Format block is not quite right.
      returnFalseIf(
         !isTypeOk ||
         !isVersionOk);
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiIoClocWriter
**************************************************************************************************/
Gb _MiIoClocWriter(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      gmineInfo->miffFile = gmiffClocWriter(
         gmineInfo->setBuffer,
         gmineInfo->dataRepo,
         HEADER_TYPE_STR,
         HEADER_VERSION_NUM);
      returnFalseIf(!gmineInfo->miffFile);
   }
   // JSON
   else
   {
      gmineInfo->jsonFile = gjsonClocWriter(gmineInfo->setBuffer, gmineInfo->dataRepo, gbTRUE);
      returnFalseIf(!gmineInfo->jsonFile);

      // Start an unnamed parent object.  This will be closed on Dloc.
      returnFalseIf(!gjsonSetFileValueObjectStart(gmineInfo->jsonFile));
      {
         // Create a format information object block
         returnFalseIf(!gjsonSetObjectValueObjectStart(gmineInfo->jsonFile, KEY_FORMAT_STR));
         {
            returnFalseIf(!gjsonSetObjectValueStr(gmineInfo->jsonFile, KEY_NAME_STR,    HEADER_TYPE_STR));
            returnFalseIf(!gjsonSetObjectValueN(  gmineInfo->jsonFile, KEY_VERSION_STR, HEADER_VERSION_NUM));
         }
         returnFalseIf(!gjsonSetObjectValueObjectStop( gmineInfo->jsonFile));
      }
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiIODloc
**************************************************************************************************/
void _MiIoDloc(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      gmiffDloc(gmineInfo->miffFile);
   }
   // JSON
   else
   {
      // If writing...
      if (gmineInfo->isWriting)
      {
         // Close the unnamed parent object.
         returnVoidIf(!gjsonSetObjectStop(gmineInfo->jsonFile));
      }

      gjsonDloc(gmineInfo->jsonFile);
   }
}

/**************************************************************************************************
func: _MiIoWriteBlockB
**************************************************************************************************/
Gb _MiIoWriteBlockB(GmineInfo * const gmineInfo, Gstr const * const key, Gb const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetB(gmineInfo->miffFile, key, value);
   }

   // JSON
   return gjsonSetObjectValueB(gmineInfo->jsonFile, key, value);
}

/**************************************************************************************************
func: _MiIoWriteBlockBinBuffer
**************************************************************************************************/
Gb _MiIoWriteBlockBinBuffer(GmineInfo * const gmineInfo, Gcount const count, Gn1 const * const buffer)
{
   int index;

   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      forCount(index, count)
      {
         returnFalseIf(!gmiffSetValueBinData(gmineInfo->miffFile, buffer[index]));
      }
      returnTrue;
   }

   // JSON
   forCount(index, count)
   {
      gjsonSetObjectValueBinByte(gmineInfo->jsonFile, buffer[index]);
   }
   returnTrue;
}

/**************************************************************************************************
func: _MiIoWriteBlockBinStart
**************************************************************************************************/
Gb _MiIoWriteBlockBinStart(GmineInfo * const gmineInfo, Gstr const * const key, size_t const fileSize)
{
   returnFalseIf(fileSize >= (size_t) GcountMAX);

   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      returnFalseIf(!gmiffSetValueStrFinite(gmineInfo->miffFile, key));
      return         gmiffSetValueBinStart( gmineInfo->miffFile, (Gcount) fileSize);
   }

   // JSON
   return gjsonSetObjectValueBinStart(gmineInfo->jsonFile, key);
}

/**************************************************************************************************
func: _MiIoWriteBlockBinStop
**************************************************************************************************/
Gb _MiIoWriteBlockBinStop(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetValueBinStop(gmineInfo->miffFile);
   }

   // JSON
   return gjsonSetObjectValueBinStop(gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteBlockColor
**************************************************************************************************/
Gb _MiIoWriteBlockColor(GmineInfo * const gmineInfo, Gstr const * const key,
   GmineInfoColor const * const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      returnFalseIf(!gmiffSetValueStrFinite( gmineInfo->miffFile, key));
      {
         returnFalseIf(!gmiffSetValueR4(     gmineInfo->miffFile, value->r));
         returnFalseIf(!gmiffSetValueR4(     gmineInfo->miffFile, value->g));
         returnFalseIf(!gmiffSetValueR4(     gmineInfo->miffFile, value->b));
      }
      return gmiffSetRecordStop(             gmineInfo->miffFile);
   }

   // JSON
   returnFalseIf(!gjsonSetObjectValueArrayStart(gmineInfo->jsonFile, key));
   {
      returnFalseIf(!gjsonSetArrayValueR4(      gmineInfo->jsonFile, value->r));
      returnFalseIf(!gjsonSetArrayValueR4(      gmineInfo->jsonFile, value->g));
      returnFalseIf(!gjsonSetArrayValueR4(      gmineInfo->jsonFile, value->b));
   }

   return gjsonSetObjectValueArrayStop(         gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteBlockI
**************************************************************************************************/
Gb _MiIoWriteBlockI(GmineInfo * const gmineInfo, Gstr const * const key, Gi8 const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetI(gmineInfo->miffFile, key, value);
   }

   // JSON
   return gjsonSetObjectValueI(gmineInfo->jsonFile, key, value);
}

/**************************************************************************************************
func: _MiIoWriteBlockN
**************************************************************************************************/
Gb _MiIoWriteBlockN(GmineInfo * const gmineInfo, Gstr const * const key, Gn8 const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetN(gmineInfo->miffFile, key, value);
   }

   // JSON
   return gjsonSetObjectValueN(gmineInfo->jsonFile, key, value);
}

/**************************************************************************************************
func: _MiIoWriteBlockPoint
**************************************************************************************************/
Gb _MiIoWriteBlockPoint(GmineInfo * const gmineInfo, Gstr const * const key,
   GmineInfoPoint const * const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      returnFalseIf(!gmiffSetValueStrFinite( gmineInfo->miffFile, key));
      {
         returnFalseIf(!gmiffSetValueR(      gmineInfo->miffFile, value->x));
         returnFalseIf(!gmiffSetValueR(      gmineInfo->miffFile, value->y));
         returnFalseIf(!gmiffSetValueR(      gmineInfo->miffFile, value->z));
      }
      return gmiffSetRecordStop(             gmineInfo->miffFile);
   }

   // JSON
   returnFalseIf(!gjsonSetObjectValueArrayStart(gmineInfo->jsonFile, key));
   {
      returnFalseIf(!gjsonSetArrayValueR(       gmineInfo->jsonFile, value->x));
      returnFalseIf(!gjsonSetArrayValueR(       gmineInfo->jsonFile, value->y));
      returnFalseIf(!gjsonSetArrayValueR(       gmineInfo->jsonFile, value->z));
   }

   return gjsonSetObjectValueArrayStop(         gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteBlockR
**************************************************************************************************/
Gb _MiIoWriteBlockR(GmineInfo * const gmineInfo, Gstr const * const key, Gr8 const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetR(gmineInfo->miffFile, key, value);
   }

   // JSON
   return gjsonSetObjectValueR(gmineInfo->jsonFile, key, value);
}

/**************************************************************************************************
func: _MiIoWriteBlockStart
**************************************************************************************************/
Gb _MiIoWriteBlockStart(GmineInfo * const gmineInfo, Gstr const * const key)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetBlockStart(gmineInfo->miffFile, key);
   }

   //JSON
   return gjsonSetObjectValueObjectStart(gmineInfo->jsonFile, key);
}

/**************************************************************************************************
func: _MiIoWriteBlockStartList
**************************************************************************************************/
Gb _MiIoWriteBlockStartList(GmineInfo * const gmineInfo, Gstr const * const key)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetBlockStart(gmineInfo->miffFile, key);
   }

   //JSON
   return gjsonSetObjectValueArrayStart(gmineInfo->jsonFile, key);
}

/**************************************************************************************************
func: _MiIoWriteBlockStartListItem
**************************************************************************************************/
Gb _MiIoWriteBlockStartListItem(GmineInfo * const gmineInfo, Gstr const * const key)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetBlockStart(gmineInfo->miffFile, key);
   }

   //JSON
   return gjsonSetArrayValueObjectStart(gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteBlockStop
**************************************************************************************************/
Gb _MiIoWriteBlockStop(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetBlockStop(gmineInfo->miffFile);
   }

   // JSON
   return gjsonSetObjectValueObjectStop(gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteBlockStopList
**************************************************************************************************/
Gb _MiIoWriteBlockStopList(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetBlockStop(gmineInfo->miffFile);
   }

   // JSON
   return gjsonSetObjectValueArrayStop(gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteBlockStr
**************************************************************************************************/
Gb _MiIoWriteBlockStr(GmineInfo * const gmineInfo, Gstr const * const key, Gstr const * const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffRecordSetStr(gmineInfo->miffFile, key, value);
   }

   // JSON
   return gjsonSetObjectValueStr(gmineInfo->jsonFile, key, value);
}

/**************************************************************************************************
func: _MiIoWriteListI
**************************************************************************************************/
Gb _MiIoWriteListI(GmineInfo * const gmineInfo, Gi8 const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetValueI(gmineInfo->miffFile, value);
   }

   // JSON
   return gjsonSetArrayValueI(gmineInfo->jsonFile, value);
}

/**************************************************************************************************
func: _MiIoWriteListN
**************************************************************************************************/
Gb _MiIoWriteListN(GmineInfo * const gmineInfo, Gn8 const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetValueN(gmineInfo->miffFile, value);
   }

   // JSON
   return gjsonSetArrayValueN(gmineInfo->jsonFile, value);
}

/**************************************************************************************************
func: _MiIoWriteListR
**************************************************************************************************/
Gb _MiIoWriteListR(GmineInfo * const gmineInfo, Gr8 const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetValueR(gmineInfo->miffFile, value);
   }

   // JSON
   return gjsonSetArrayValueR(gmineInfo->jsonFile, value);
}

/**************************************************************************************************
func: _MiIoWriteListStart
**************************************************************************************************/
Gb _MiIoWriteListStart(GmineInfo * const gmineInfo, Gstr const * const key, Gcount const count)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      returnFalseIf(!gmiffSetValueStrFinite( gmineInfo->miffFile, key));
      return         gmiffSetValueArrayCount(gmineInfo->miffFile, count);
   }

   // JSON
   return gjsonSetObjectValueArrayStart(gmineInfo->jsonFile, key);
}

/**************************************************************************************************
func: _MiIoWriteListStop
**************************************************************************************************/
Gb _MiIoWriteListStop(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetRecordStop(gmineInfo->miffFile);
   }

   // JSON
   return gjsonSetObjectValueArrayStop(gmineInfo->jsonFile);
}

/**************************************************************************************************
func: _MiIoWriteListStr
**************************************************************************************************/
Gb _MiIoWriteListStr(GmineInfo * const gmineInfo, Gstr const * const value)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetValueStrFinite(gmineInfo->miffFile, value);
   }

   // JSON
   return gjsonSetArrayValueStr(gmineInfo->jsonFile, value);
}
