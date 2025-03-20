/**************************************************************************************************
file:       GmineInfo
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
The top level routines for the Mine Information
**************************************************************************************************/

/**************************************************************************************************
MIT License

Copyright (c) 2025, Robbert de Groot

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
static Gb _isStarted = gbFALSE;

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoBlockTypeNext

Block order is important for reading in the MineInfo file in one go.  Some data needs to be defined
before it is being referred to.  This is why writing out blocks in a specific order is important.
This function is only necessary for writing.  Reading will a MineInfo file works differently.
**************************************************************************************************/
GmineInfoBlockType gmineInfoBlockTypeNext(GmineInfo * const gmineInfo)
{
   returnIf(
         !_isStarted ||
         !gmineInfo  ||
         gmineInfo->currentBlockType == gmineInfoBlockType_END,
      gmineInfoBlockType_END);

   // If writing...
   if (gmineInfo->isWriting)
   {
      // Close up the previous block
      switch (gmineInfo->currentBlockType)
      {
      case gmineInfoBlockTypeDATA:
         //_MiUnionDlocData(       gmineInfo);
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeITEM:
         //_MiUnionDlocItem(       gmineInfo);
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeGEOMETRY:
         //_MiUnionDlocGeometry(   gmineInfo);
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeDRILL_HOLE:
         //_MiUnionDlocDrillHole(  gmineInfo);
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeMODEL:
         //_MiUniontDlocModel(     gmineInfo);
         _MiIoWriteBlockStop(    gmineInfo);
         break;
      }
   }

   // Move to the next block
   gmineInfo->currentBlockType++;

   // Clear the memory for the next block.
   _MiMemClearType(gmineInfo->block, GmineInfoUnion);

   switch (gmineInfo->currentBlockType)
   {
   case gmineInfoBlockTypeDATA:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_DATA);
      break;

   case gmineInfoBlockTypeITEM:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM);
      break;

   case gmineInfoBlockTypeGEOMETRY:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_GEOMETRY);
      break;

   case gmineInfoBlockTypeDRILL_HOLE:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_DRILL_HOLE);
      break;

   case gmineInfoBlockTypeMODEL:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_MODEL);
      break;
   }

   return gmineInfo->currentBlockType;
}

/**************************************************************************************************
func: gmineInfoClocReader
**************************************************************************************************/
GmineInfo *gmineInfoClocReader(GmineInfoFileType const fileType, GgetBuffer getBufferFunc,
   void * const dataRepo)
{
   GmineInfo *gmineInfo;

   returnNullIf(!_isStarted);

   gmineInfo = _MiMemClocType(GmineInfo);
   returnNullIf(!gmineInfo);

   if (!gmineInfoClocReaderContent(gmineInfo, fileType, getBufferFunc, dataRepo))
   {
      _MiMemDloc(gmineInfo);
      return NULL;
   }

   return gmineInfo;
}

/**************************************************************************************************
func: gmineInfoClocReaderContent
**************************************************************************************************/
Gb gmineInfoClocReaderContent(GmineInfo * const gmineInfo, GmineInfoFileType const fileType,
   GgetBuffer getBufferFunc, void * const dataRepo)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo);

   _MiMemClearType(gmineInfo, GmineInfo);

   gmineInfo->fileType  = fileType;
   gmineInfo->dataRepo  = dataRepo;
   gmineInfo->getBuffer = getBufferFunc;

   // Start reading the file header.
   return _MiIoClocReader(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoClocWriter
**************************************************************************************************/
GmineInfo *gmineInfoClocWriter(GmineInfoFileType const fileType, GsetBuffer setBufferFunc,
   void * const dataRepo)
{
   GmineInfo *gmineInfo;

   returnNullIf(!_isStarted);

   gmineInfo = _MiMemClocType(GmineInfo);
   returnNullIf(!gmineInfo);

   if (!gmineInfoClocWriterContent(gmineInfo, fileType, setBufferFunc, dataRepo))
   {
      _MiMemDloc(gmineInfo);
      return NULL;
   }

   return gmineInfo;
}

/**************************************************************************************************
func: gmineInfoClocWriterContent
**************************************************************************************************/
Gb gmineInfoClocWriterContent(GmineInfo * const gmineInfo, GmineInfoFileType const fileType,
   GsetBuffer setBufferFunc, void * const dataRepo)
{
   returnFalseIf(!_isStarted);

   _MiMemClearType(gmineInfo, GmineInfo);

   gmineInfo->isWriting = gbTRUE;
   gmineInfo->fileType  = fileType;
   gmineInfo->dataRepo  = dataRepo;
   gmineInfo->setBuffer = setBufferFunc;

   // Start by writing out the header for the file.
   return _MiIoClocWriter(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoDloc
**************************************************************************************************/
void gmineInfoDloc(GmineInfo * const gmineInfo)
{
   returnVoidIf(
      !_isStarted ||
      !gmineInfo);

   gmineInfoDlocContent(gmineInfo);

   _MiMemDloc(gmineInfo);
}

/**************************************************************************************************
func: gmineInfoDlocContent
**************************************************************************************************/
void gmineInfoDlocContent(GmineInfo * const gmineInfo)
{
   returnVoidIf(
      !_isStarted ||
      !gmineInfo);

   _MiIoDloc(gmineInfo);

   _MiMemClearType(gmineInfo, GmineInfo);
}

/**************************************************************************************************
func: gmineInfoSetDataAuthorName
**************************************************************************************************/
Gb gmineInfoSetDataAuthorName(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value     ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->block.data.authorName      = _MiStrClone(value);
   gmineInfo->block.data.isSetAuthorName = (gmineInfo->block.data.authorName != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataComment
**************************************************************************************************/
Gb gmineInfoSetDataComment(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value     ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->block.data.comment      = _MiStrClone(value);
   gmineInfo->block.data.isSetComment = (gmineInfo->block.data.comment != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataCompanyName
**************************************************************************************************/
Gb gmineInfoSetDataCompanyName(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value     ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->block.data.companyName      = _MiStrClone(value);
   gmineInfo->block.data.isSetCompanyName = (gmineInfo->block.data.companyName != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataCopyright
**************************************************************************************************/
Gb gmineInfoSetDataCopyright(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value     ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->block.data.copyright      = _MiStrClone(value);
   gmineInfo->block.data.isSetCopyright = (gmineInfo->block.data.copyright != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectMax
**************************************************************************************************/
Gb gmineInfoSetDataProjectMax(GmineInfo * const gmineInfo, GmineInfoPoint const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value);

   gmineInfo->block.data.projectMax      = *value;
   gmineInfo->block.data.isSetProjectMax = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectMin
**************************************************************************************************/
Gb gmineInfoSetDataProjectMin(GmineInfo * const gmineInfo, GmineInfoPoint const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value);

   gmineInfo->block.data.projectMin      = *value;
   gmineInfo->block.data.isSetProjectMin = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectName
**************************************************************************************************/
Gb gmineInfoSetDataProjectName(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value     ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->block.data.projectName      = _MiStrClone(value);
   gmineInfo->block.data.isSetProjectName = (gmineInfo->block.data.projectName != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataProjectSystem
**************************************************************************************************/
Gb gmineInfoSetDataProjectSystem(GmineInfo * const gmineInfo, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfo ||
      !value     ||
      !_MiStrGetCount(value, Gn4MAX));

   gmineInfo->block.data.projectSystem      = _MiStrClone(value);
   gmineInfo->block.data.isSetProjectSystem = (gmineInfo->block.data.projectSystem != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataSoftwareName
**************************************************************************************************/
Gb gmineInfoSetDataSoftwareName(GmineInfo * const gmineInfo, Gstr const * const value, Gstr * const version)
{
   returnFalseIf(
      !gmineInfo                       ||
      !value                           ||
      !_MiStrGetCount(value,   Gn4MAX) ||
      !version                         ||
      !_MiStrGetCount(version, Gn4MAX));

   gmineInfo->block.data.softwareName    = _MiStrClone(value);
   gmineInfo->block.data.softwareVersion = _MiStrClone(value);
   gmineInfo->block.data.isSetSoftware   =
      (gmineInfo->block.data.softwareName    != NULL &&
       gmineInfo->block.data.softwareVersion != NULL);

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetDataOther
**************************************************************************************************/
Gb gmineInfoSetDataOther(GmineInfo * const gmineInfo, Gstr const * const key, Gstr const * const value)
{
   GmineInfoKeyValue *keyValue;

   returnFalseIf(
      !gmineInfo                     ||
      !key                           ||
      !_MiStrGetCount(key,   Gn4MAX) ||
      !value                         ||
      !_MiStrGetCount(value, Gn4MAX));

   keyValue = _MiMemClocType(GmineInfoKeyValue);
   returnFalseIf(!keyValue);

   keyValue->key   = _MiStrClone(key);
   keyValue->value = _MiStrClone(value);

   if (!gmineInfo->block.data.otherListHead)
   {
      // Start the list.
      gmineInfo->block.data.otherListHead    =
         gmineInfo->block.data.otherListTail = keyValue;
   }
   else
   {
      // Append to the list.
      gmineInfo->block.data.otherListTail->next = keyValue;
      gmineInfo->block.data.otherListTail       = keyValue;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteData
**************************************************************************************************/
Gb gmineInfoWriteData(GmineInfo * const gmineInfo)
{
   GmineInfoKeyValue *keyValue;

   if (gmineInfo->block.data.isSetAuthorName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COMPANY_NAME,     gmineInfo->block.data.companyName);
   }
   if (gmineInfo->block.data.isSetCopyright)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COPYRIGHT,        gmineInfo->block.data.copyright);
   }
   if (gmineInfo->block.data.isSetProjectName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_PROJECT_NAME,     gmineInfo->block.data.projectName);
   }
   if (gmineInfo->block.data.isSetProjectSystem)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_PROJECT_SYSTEM,   gmineInfo->block.data.projectSystem);
   }
   if (gmineInfo->block.data.isSetProjectMin)
   {
      _MiIoWritePoint( gmineInfo, KEY_BLOCK_DATA_PROJECT_MIN,     &gmineInfo->block.data.projectMin);
   }
   if (gmineInfo->block.data.isSetProjectMax)
   {
      _MiIoWritePoint( gmineInfo, KEY_BLOCK_DATA_PROJECT_MAX,     &gmineInfo->block.data.projectMax);
   }
   if (gmineInfo->block.data.isSetAuthorName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_AUTHOR_NAME,      gmineInfo->block.data.authorName);
   }
   if (gmineInfo->block.data.isSetComment)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COMMENT,          gmineInfo->block.data.comment);
   }
   if (gmineInfo->block.data.isSetSoftware)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_SOFTWARE_NAME,    gmineInfo->block.data.softwareName);
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_SOFTWARE_VERSION, gmineInfo->block.data.softwareVersion);
   }

   keyValue = gmineInfo->block.data.otherListHead;
   loop
   {
      breakIf(!keyValue);

      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_OTHER_KEY,   keyValue->key);
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_OTHER_VALUE, keyValue->value);

      keyValue = keyValue->next;
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoStart
**************************************************************************************************/
Gb gmineInfoStart(GmemCloc memClocFunc, GmemDloc memDlocFunc)
{
   returnTrueIf(_isStarted);

   returnFalseIf(
      !memClocFunc ||
      !memDlocFunc)

   // Start support libraries.
   returnFalseIf(
      !gjsonStart(memClocFunc, memDlocFunc) ||
      !gmiffStart(memClocFunc, memDlocFunc));

   _isStarted = gbTRUE;

   _MiMemStart(memClocFunc, memDlocFunc);

   return gbTRUE;
}

/**************************************************************************************************
func: gmineInfoStop
**************************************************************************************************/
void gmineInfoStop(void)
{
   returnVoidIf(!_isStarted);

   _isStarted = gbFALSE;
}

