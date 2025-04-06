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
func: gmineInfoAddImage
**************************************************************************************************/
Gb gmineInfoAddImage(GmineInfo * const gmineInfo, GmineInfoImage * const image)
{
   return gmineInfoArrayAddLast(&gmineInfo->imageArray, image);
}

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
      case gmineInfoBlockTypePROPERTY_LIST:
      case gmineInfoBlockTypeIMAGE_LIST:
      case gmineInfoBlockTypeITEM_LIST:
      case gmineInfoBlockTypeGEOMETRY_LIST:
      case gmineInfoBlockTypeDRILL_HOLE_LIST:
      case gmineInfoBlockTypeMODEL_LIST:
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

   case gmineInfoBlockTypePROPERTY_LIST:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_PROPERTY_LIST);
      break;

   case gmineInfoBlockTypeIMAGE_LIST:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_IMAGE_LIST);
      break;

   case gmineInfoBlockTypeITEM_LIST:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_ITEM_LIST);
      break;

   case gmineInfoBlockTypeGEOMETRY_LIST:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_GEOMETRY_LIST);
      break;

   case gmineInfoBlockTypeDRILL_HOLE_LIST:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_DRILL_HOLE_LIST);
      break;

   case gmineInfoBlockTypeMODEL_LIST:
      _MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_MODEL_LIST);
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
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->imageArray));
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->propertyArray));

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
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->imageArray));
   returnFalseIf(!gmineInfoArrayClocContent(&gmineInfo->propertyArray));

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

   gmineInfoArrayForEach(    &gmineInfo->imageArray, gmineInfoImageDloc);
   gmineInfoArrayDlocContent(&gmineInfo->imageArray);

   gmineInfoArrayForEach(    &gmineInfo->propertyArray, gmineInfoPropertyDloc);
   gmineInfoArrayDlocContent(&gmineInfo->propertyArray);

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
func: gmineInfoSetItemListAppend
**************************************************************************************************/
Gb gmineInfoSetItemListAppend(GmineInfo * const gmineInfo, GmineInfoItem * const gmineInfoItem)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoItem);

   return gmineInfoArrayAddLast(&gmineInfo->itemArray, gmineInfoItem);
}

/**************************************************************************************************
func: gmineInfoSetImageListAppend
**************************************************************************************************/
Gb gmineInfoSetImageListAppend(GmineInfo * const gmineInfo, GmineInfoImage * const gmineInfoImage)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoImage);

   return gmineInfoArrayAddLast(&gmineInfo->imageArray, gmineInfoImage);
}

/**************************************************************************************************
func: gmineInfoSetPropertyListAppend
**************************************************************************************************/
Gb gmineInfoSetPropertyListAppend(GmineInfo * const gmineInfo, GmineInfoProperty * const gmineInfoProperty)
{
   returnFalseIf(
      !_isStarted ||
      !gmineInfo  ||
      !gmineInfoProperty);

   return gmineInfoArrayAddLast(&gmineInfo->propertyArray, gmineInfoProperty);
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
      kv = (GmineInfoKeyValue *) gmineInfoArrayGetAt(&gmineInfo->data->keyValueArray, index);
      _MiIoWriteString(gmineInfo, kv->key, kv->value);
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteItemListBlock
**************************************************************************************************/
Gb gmineInfoWriteItemListBlock(GmineInfo * const gmineInfo)
{
   Gcount         count;
   Gindex         index;
   GmineInfoItem *item;

   // Ensure writing is in order.
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeITEM_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->itemArray);
   forCount(index, count)
   {
      item = (GmineInfoItem *) gmineInfoArrayGetAt(&gmineInfo->itemArray, index);


   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWriteImageListBlock
**************************************************************************************************/
Gb gmineInfoWriteImageListBlock(GmineInfo * const gmineInfo)
{
   Gcount          count;
   Gindex          index;
   GmineInfoImage *image;

   // Ensure writing in order
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeIMAGE_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->imageArray);
   forCount(index, count)
   {
      image = (GmineInfoImage *) gmineInfoArrayGetAt(&gmineInfo->imageArray, index);

      // File path is mandatory.  An image without an image is not an image.
      returnFalseIf(!gmineInfoImageGetFilePath(image));

      returnFalseIf(!_MiIoWriteBlockStart(gmineInfo, KEY_BLOCK_IMAGE));
      returnFalseIf(!_MiIoWriteString(    gmineInfo, KEY_BLOCK_IMAGE_KEY,  image->key));
      returnFalseIf(!_MiIoWriteString(    gmineInfo, KEY_BLOCK_IMAGE_NAME, image->name));
      if (image->isInline)
      {
         size_t readCount,
                fileSize;
         Gcount bufferSize;
         Gn1   *buffer;
         Gn1    bufferSmall[1024];
         FILE  *file;
         Gb     isWritten;

         isWritten  = gbFALSE;
         file       = NULL;

         bufferSize = SIZE_1MB;
         buffer     = _MiMemClocTypeArray(count, Gn1);
         if (!buffer)
         {
            bufferSize = 1024;
            buffer     = bufferSmall;
         }

         breakScope
         {
            // Open the file to read.
            returnFalseIf(!fopen_s(&file, gmineInfoImageGetFilePath(image), "rb"));

            // Get the raw file size.
            fileSize = 0;
            _fseeki64_nolock(file, 0, SEEK_END);
            fileSize = _ftelli64(file);

            // Start the binary buffer.
            returnFalseIf(!_MiIoWriteBinStart(gmineInfo, KEY_BLOCK_IMAGE_FILE, fileSize));

            // Return to the start of the file.
            _fseeki64_nolock(file, 0, SEEK_SET);

            // Copy the contents to the MI file.
            loop
            {
               readCount = fread_s(buffer, bufferSize, 1, bufferSize, file);
               breakIf(_MiIoWriteBinBuffer(gmineInfo, (Gcount) readCount, buffer));

               // Read everything there is for the file.
               breakIf(readCount != bufferSize);
            }

            breakIf(_MiIoWriteBinStop(gmineInfo));

            isWritten = gbTRUE;
         }

         // Close the file.
         fclose(file);

         if (count == SIZE_1MB)
         {
            _MiMemDloc(buffer);
         }

         returnFalseIf(!isWritten);
      }
      else
      {
         returnFalseIf(_MiIoWriteString(gmineInfo, KEY_BLOCK_IMAGE_FILE_PATH, image->filePath));
      }

      returnFalseIf(!_MiIoWriteBlockStop(gmineInfo));
   }

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoWritePropertyListBlock
**************************************************************************************************/
Gb gmineInfoWritePropertyListBlock(GmineInfo * const gmineInfo)
{
   Gcount             count;
   Gindex             index;
   GmineInfoProperty *prop;

   // Ensure writing is in order.
   returnFalseIf(gmineInfo->currentBlockType != gmineInfoBlockTypeITEM_LIST);

   count = gmineInfoArrayGetCount(&gmineInfo->propertyArray);
   forCount(index, count)
   {
      prop = (GmineInfoProperty *) gmineInfoArrayGetAt(&gmineInfo->propertyArray, index);


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
