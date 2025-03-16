/**************************************************************************************************
file:       MineInfoType
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Types found in the library
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

#if !defined(MINE_INFO_TYPE_H)
#define      MINE_INFO_TYPE_H

/**************************************************************************************************
constant:
**************************************************************************************************/

/**************************************************************************************************
type:
**************************************************************************************************/
typedef enum
{
   gmineInfoFileTypeNONE,

   gmineInfoFileTypeJSON,
   gmineInfoFileTypeMIFF,
} GmineInfoFileType;

// Order of these items are in the order of the blocks in a MineInfo file.
typedef enum
{
   gmineInfoBlockTypeNONE,

   gmineInfoBlockTypeINFORMATION,
   gmineInfoBlockTypePROPERTIES,
   gmineInfoBlockTypeITEM,
   gmineInfoBlockTypeIMAGE,
   gmineInfoBlockTypeGEOMETRY,
   gmineInfoBlockTypeDRILL_HOLE,
   gmineInfoBlockTypeMODEL,

   // No more block types.
   gmineInfoBlockType_END,

   gmineInfoBlockTypeCOUNT
} GmineInfoBlockType;

typedef struct
{
   Gb                 isWriting;
   Gb                 isByteSwapping;
   GgetBuffer         getBuffer;
   GsetBuffer         setBuffer;

   void              *dataRepo;

   GmineInfoFileType  fileType;
   GmineInfoBlockType currentBlockType;

   Gmiff             *miffFile;
   Gjson             *jsonFile;
} GmineInfo;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/


#endif