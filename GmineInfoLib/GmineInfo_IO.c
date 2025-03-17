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
Gb _MiIoClocReader(GmineInfo * const mineInfo)
{
   Gstr headerTypeStr[   GkeySIZE],
        headerVersionStr[GkeySIZE];
   Gn4  headerVersionNum;

   headerVersionNum = 0;
   _MiMemClearTypeArray(headerTypeStr,    Gstr, GkeySIZE);
   _MiMemClearTypeArray(headerVersionStr, Gstr, GkeySIZE);

   // MIFF
   if (mineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      mineInfo->miffFile = gmiffClocReader(
         mineInfo->getBuffer,
         headerTypeStr,
         headerVersionStr,
         mineInfo->dataRepo);
      returnFalseIf(
         !mineInfo->miffFile                                ||
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
         mineInfo->jsonFile = gjsonClocReader(mineInfo->getBuffer, mineInfo->dataRepo);
         breakIf(!mineInfo->jsonFile);

         // Get the parent object.
         type = gjsonGetType_FileElement(mineInfo->jsonFile);
         breakIf(type != gjsonTypeOBJECT_START);

         // Get the format block which should be the first block in the JSON.
         breakIf(gjsonGetType_ObjectKeyOrStop(mineInfo->jsonFile) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(mineInfo->jsonFile, KEY_FORMAT_STR));
         breakIf(gjsonGetType_ObjectValue(mineInfo->jsonFile) != gjsonTypeOBJECT_START);
         {
            loop
            {
               type = gjsonGetType_ObjectKeyOrStop(mineInfo->jsonFile);
               breakIf(type == gjsonTypeOBJECT_STOP);
               breakIf(type != gjsonTypeOBJECT_KEY);

               if      (gjsonIsKeyEqual(mineInfo->jsonFile, KEY_NAME_STR))
               {
                  breakIf(!gjsonIsTypeStr(gjsonGetType_ObjectValue(mineInfo->jsonFile)));
                  breakIf(!gjsonGetStr(mineInfo->jsonFile, GkeyBYTE_COUNT, headerTypeStr));
                  breakIf(!_MiStrIsEqual(headerTypeStr, HEADER_TYPE_STR));

                  isTypeOk = gbTRUE;
               }
               else if (gjsonIsKeyEqual(mineInfo->jsonFile, KEY_VERSION_STR))
               {
                  breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(mineInfo->jsonFile)));
                  breakIf(!gjsonGetN(mineInfo->jsonFile, &version));
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
Gb _MiIoClocWriter(GmineInfo * const mineInfo)
{
   // MIFF
   if (mineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      mineInfo->miffFile = gmiffClocWriter(
         mineInfo->setBuffer,
         HEADER_TYPE_STR,
         HEADER_VERSION_STR,
         mineInfo->dataRepo);
      returnFalseIf(!mineInfo->miffFile);
   }
   // JSON
   else
   {
      mineInfo->jsonFile = gjsonClocWriter(mineInfo->setBuffer, mineInfo->dataRepo, gbTRUE);
      returnFalseIf(!mineInfo->jsonFile);

      // Start an unnamed parent object.  This will be closed on Dloc.
      returnFalseIf(!gjsonSetFileValueObjectStart(mineInfo->jsonFile));
      {
         // Create a format information object block
         returnFalseIf(!gjsonSetObjectValueObjectStart(mineInfo->jsonFile, KEY_FORMAT_STR));
         {
            returnFalseIf(!gjsonSetObjectValueStr(mineInfo->jsonFile, KEY_NAME_STR,    HEADER_TYPE_STR));
            returnFalseIf(!gjsonSetObjectValueN(  mineInfo->jsonFile, KEY_VERSION_STR, HEADER_VERSION_NUM));
         }
         returnFalseIf(!gjsonSetObjectValueObjectStop( mineInfo->jsonFile));
      }
   }

   returnTrue;
}

/**************************************************************************************************
func: _MiIODloc
**************************************************************************************************/
void _MiIoDloc(GmineInfo * const mineInfo)
{
   // MIFF
   if (mineInfo->fileType == gmineInfoFileTypeMIFF)
   {
      gmiffDloc(mineInfo->miffFile);
   }
   // JSON
   else
   {
      // If writing...
      if (mineInfo->isWriting)
      {
         // Close the unnamed parent object.
         returnVoidIf(!gjsonSetObjectStop(mineInfo->jsonFile));
      }

      gjsonDloc(mineInfo->jsonFile);
   }
}
