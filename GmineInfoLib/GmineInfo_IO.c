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
      Gstr      key[GkeySIZE];
      Gb        isTypeOk,
                isVersionOk;
      Gn8       version;

      mineInfo->jsonFile = gjsonClocReader(mineInfo->getBuffer, mineInfo->dataRepo);
      returnFalseIf(!mineInfo->jsonFile);

      // Get the parent object.
      type = gjsonGetType_FileElement(mineInfo->jsonFile);
      returnFalseIf(type != gjsonTypeOBJECT_START);

      // Get the format block
      isTypeOk    = gbFALSE;
      isVersionOk = gbFALSE;
      loop
      {
         type = gjsonGetType_ObjectKeyOrEnd(mineInfo->jsonFile);

         breakIf(type != gjsonTypeOBJECT_STOP);

         if      (_MiStrIsEqual(gjsonGetKey(mineInfo->jsonFile), KEY_FORMAT_STR))
         {
            type = gjsonGetType_ObjectValue(mineInfo->jsonFile);
            returnFalseIf(type != gjsonTypeVALUE_STRING_START);
            gjsonGetStr(mineInfo->jsonFile, GkeyBYTE_COUNT, headerTypeStr);
            returnFalseIf(!_MiStrIsEqual(headerTypeStr, HEADER_TYPE_STR));
         }
         else if (_MiStrIsEqual(gjsonGetKey(mineInfo->jsonFile), KEY_VERSION_STR))
         {
            type = gjsonGetType_ObjectValue(mineInfo->jsonFile);
            returnFalseIf(
               !(type == gjsonTypeVALUE_NUMBER_INTEGER ||
                 type == gjsonTypeVALUE_NUMBER_NATURAL));
            returnFalseIf(!gjsonGetN(mineInfo->jsonFile, &version));
            returnFalseIf(version != HEADER_VERSION_NUM);
         }
         else
         {
            // Something unexpected inside the format block.
            returnFalse;
         }
      }
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
      returnFalseIf(!gjsonSetObjectStart(mineInfo->jsonFile));

      // Create a format information object block
      returnFalseIf(!gjsonSetKey(        mineInfo->jsonFile, KEY_FORMAT_STR));
      returnFalseIf(!gjsonSetObjectStart(mineInfo->jsonFile));
      {
         returnFalseIf(!gjsonSetKey(     mineInfo->jsonFile, KEY_NAME_STR));
         returnFalseIf(!gjsonSetValueStr(mineInfo->jsonFile, HEADER_TYPE_STR));

         returnFalseIf(!gjsonSetKey(     mineInfo->jsonFile, KEY_VERSION_STR));
         returnFalseIf(!gjsonSetValueN(  mineInfo->jsonFile, HEADER_VERSION_NUM));
      }
      returnFalseIf(!gjsonSetObjectStop( mineInfo->jsonFile));
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
