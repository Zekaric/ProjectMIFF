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
#define gmineInfoPROJECT_SYSTEM_LOCAL  "local"

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

   gmineInfoBlockTypeDATA,
   gmineInfoBlockTypeITEM,
   gmineInfoBlockTypeGEOMETRY,
   gmineInfoBlockTypeDRILL_HOLE,
   gmineInfoBlockTypeMODEL,

   //gmineInfoBlockTypeIMAGE,
   //gmineInfoBlockTypePROPERTIES,

   // No more block types.
   gmineInfoBlockType_END,

   gmineInfoBlockTypeCOUNT
} GmineInfoBlockType;

typedef enum
{
   gmineInfoItemTypeNONE,

   gmineInfoItemTypeI,
   gmineInfoItemTypeN,
   gmineInfoItemTypeR,
   gmineInfoItemTypeSTR,
   gmineInfoItemType
};

typedef struct
{
   // Using x, y, z because these can be interpreted differently depending on coordinate system used
   double x, // Easting, longitude
          y, // Northing, latitude,
          z; // Elevation
} GmineInfoPoint;

typedef struct
{
   // Using x, y, z because these can be interpreted differently depending on coordinate system used
   double x, // Easting, longitude
          y, // Northing, latitude,
          z; // Elevation
} GmineInfoVector;

typedef struct GmineInfoKeyValue GmineInfoKeyValue;
struct GmineInfoKeyValue
{
   Gstr              *key,
                     *value;
   GmineInfoKeyValue *next;
};

typedef struct
{
   Gbit               isSetAuthorName        : 1,
                      isSetComment           : 1,
                      isSetCompanyName       : 1,
                      isSetCopyright         : 1,
                      isSetSoftware          : 1,
                      isSetProjectName       : 1,
                      isSetProjectSystem     : 1,
                      isSetProjectMax        : 1,
                      isSetProjectMin        : 1;
   Gstr              *authorName,
                     *comment,
                     *companyName,
                     *copyright,
                     *softwareName,
                     *softwareVersion,
                     *projectName,
                     *projectSystem;
   GmineInfoPoint     projectMax,
                      projectMin;
   GmineInfoKeyValue *otherListHead,
                     *otherListTail;
} GmineInfoData;

typedef struct GmineInfoItem;
struct GmineInfoItem
{
   // Unique id for this item, library created.
   Gn4                id;
   Gstr              *name;
   Gstr              *description;
   // Unique id for this item, client provided.
   Gstr              *uid;
   GmineInfoItemType  type;
};

typedef struct
{
   Gstr              *id;
} GmineInfoItem;

typedef struct
{
   Gstr              *id;
} GmineInfoGeometry;

typedef struct
{
   Gstr              *id;
} GmineInfoDrillHole;

typedef struct
{
   Gstr              *id;
} GmineInfoModel;

typedef union
{
   GmineInfoData      data;
   GmineInfoItem      item;
   GmineInfoGeometry  geometry;
   GmineInfoDrillHole drillhole;
   GmineInfoModel     model;
} GmineInfoUnion;

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

   GmineInfoUnion     block;
} GmineInfo;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/


#endif