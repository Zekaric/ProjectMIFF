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
static Gb    _isStarted = gbFALSE;

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
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeITEM:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeGEOMETRY:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeDRILL_HOLE:
         _MiIoWriteBlockStop(    gmineInfo);
         break;

      case gmineInfoBlockTypeMODEL:
         _MiIoWriteBlockStop(    gmineInfo);
         break;
      }
   }

   // Move to the next block
   gmineInfo->currentBlockType++;

   // Clear the memory for the next block.
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

   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->itemArray));

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

   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->itemArray));

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

   gmineInfoDataDloc(gmineInfo->data);

   gmineInfoArrayForEach(    &gmineInfo->itemArray, gmineInfoItemDloc);
   gmineInfoArrayDlocContent(&gmineInfo->itemArray);

   _MiIoDloc(gmineInfo);

   _MiMemClearType(gmineInfo, GmineInfo);
}

/**************************************************************************************************
func: gmineInfoIsStarted
**************************************************************************************************/
Gb gmineInfoIsStarted(void)
{
   return _isStarted;
}

/**************************************************************************************************
func: gmineInfoSetData
**************************************************************************************************/
Gb gmineInfoSetData(GmineInfo * const gmineInfo, GmineInfoData * const value)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !value);

   gmineInfo->data = value;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoSetItemAppend
**************************************************************************************************/
Gb gmineInfoSetItemAppend(GmineInfo * const gmineInfo, GmineInfoItem * const gmineInfoItem)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoItem);

   return gmineInfoArrayAddLast(&gmineInfo->itemArray, gmineInfoItem);
}

/**************************************************************************************************
func: gmineInfoWriteDataBlock
**************************************************************************************************/
Gb gmineInfoWriteDataBlock(GmineInfo * const gmineInfo)
{
   Gcount             count;
   Gindex             index;
   GmineInfoKeyValue *kv;

   // Ensure writing in order.
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeDATA);

   if (gmineInfo->data->isSetAuthorName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COMPANY_NAME,     gmineInfo->data->companyName);
   }
   if (gmineInfo->data->isSetCopyright)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_COPYRIGHT,        gmineInfo->data->copyright);
   }
   if (gmineInfo->data->isSetProjectName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_PROJECT_NAME,     gmineInfo->data->projectName);
   }
   if (gmineInfo->data->isSetProjectSystem)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_PROJECT_SYSTEM,   gmineInfo->data->projectSystem);
   }
   if (gmineInfo->data->isSetProjectMin)
   {
      _MiIoWritePoint( gmineInfo, KEY_BLOCK_DATA_PROJECT_MIN,     &gmineInfo->data->projectMin);
   }
   if (gmineInfo->data->isSetProjectMax)
   {
      _MiIoWritePoint( gmineInfo, KEY_BLOCK_DATA_PROJECT_MAX,     &gmineInfo->data->projectMax);
   }
   if (gmineInfo->data->isSetAuthorName)
   {
      _MiIoWriteString(gmineInfo, KEY_BLOCK_DATA_AUTHOR_NAME,      gmineInfo->data->authorName);
   }

   // Write out the user key values.
   count = gmineInfoArrayGetCount(&gmineInfo->data->keyValueArray);
   forCount(index, count);
   {
      kv = gmineInfoArrayGetAt(&gmineInfo->data->keyValueArray, index);
      _MiIoWriteString(gmineInfo, kv->key, kv->value);
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

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoStop
**************************************************************************************************/
void gmineInfoStop(void)
{
   returnVoidIf(!_isStarted);

   _isStarted = gbFALSE;
}
