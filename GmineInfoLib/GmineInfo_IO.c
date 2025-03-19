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
   Gstr headerTypeStr[   GkeySIZE],
        headerVersionStr[GkeySIZE];
   Gn4  headerVersionNum;

   headerVersionNum = 0;
   _MiMemClearTypeArray(headerTypeStr,    Gstr, GkeySIZE);
   _MiMemClearTypeArray(headerVersionStr, Gstr, GkeySIZE);

   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      gmineInfo->miffFile = gmiffClocReader(
         gmineInfo->getBuffer,
         headerTypeStr,
         headerVersionStr,
         gmineInfo->dataRepo);
      returnFalseIf(
         !gmineInfo->miffFile                                ||
         !_MiStrIsEqual(headerTypeStr,    HEADER_TYPE_STR)  ||
         !_MiStrIsEqual(headerVersionStr, HEADER_VERSION_STR));
   }
   // JSON
   else
   {
      GjsonType type;
      Gb        isTypeOk,
                isVersionOk;
      Gn8       version;

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
                  breakIf(!gjsonGetStr(gmineInfo->jsonFile, GkeyBYTE_COUNT, headerTypeStr));
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
         HEADER_TYPE_STR,
         HEADER_VERSION_STR,
         gmineInfo->dataRepo);
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
func: _MiIoWriteBlockStart
**************************************************************************************************/
Gb _MiIoWriteBlockStart(GmineInfo * const gmineInfo, Gstr * const key)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetRecordBlockStart(gmineInfo->miffFile, key);
   }

   //JSON
   return gjsonSetObjectValueObjectStart(gmineInfo->jsonFile, key);
}

/**************************************************************************************************
func: _MiIoWriteBlockStop
**************************************************************************************************/
Gb _MiIoWriteBlockStop(GmineInfo * const gmineInfo)
{
   // MIFF
   if (gmineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      return gmiffSetRecordBlockStop(gmineInfo->miffFile);
   }

   // JSON
   return gjsonSetObjectValueObjectStop(gmineInfo->jsonFile);
}
