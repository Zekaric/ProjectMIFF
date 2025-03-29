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
   gmineInfoItemTypeDATE,
   gmineInfoItemTypeTIME,
   gmineInfoItemTypeDATETIME,
   gmineInfoItemTypePERCENT,

   gmineInfoItemTypeFORMULA,

   gmineInfoItemTypeCOUNT
} GmineInfoItemType;

typedef union
{
   Gn8                n;
   Gi8                i;
   Gr8                r;
   Gstr              *str;
} GmineInfoValue;

typedef struct
{
   Gcount             count,
                      countMax;
   void             **buffer;
} GmineInfoArray;

typedef struct
{
   Gb                 isSet;
   GmineInfoValue     value;
} GmineInfoValuable;

typedef struct
{
   // Using x, y, z because these can be interpreted differently depending on coordinate system used
   double             x, // Easting, longitude
                      y, // Northing, latitude,
                      z; // Elevation
} GmineInfoPoint;

typedef struct
{
   float              r,
                      g,
                      b,
                      a;
} GmineInfoRGBA;

typedef struct
{
   // Using x, y, z because these can be interpreted differently depending on coordinate system used
   double             x, // Easting, longitude
                      y, // Northing, latitude,
                      z; // Elevation
} GmineInfoVector;

typedef struct
{
   Gstr              *key,
                     *value;
} GmineInfoKeyValue;

typedef struct
{
   Gb                 isVisibleDrillHole,
                      isVisibleModel,
                      isVisibleGeometry,
                      isVisibleNode,
                      isVisibleLine,
                      isVisibleFace,
                      isVisibleText;

   GmineInfoRGBA      colorDrillHole,
                      colorModel,
                      colorGeometry,
                      colorNode,
                      colorLine,
                      colorFace,
                      colorFacePattern,
                      colorText;

   Gstr              *patternNode,
                     *patternLine,
                     *patternFace;

   Gr4                faceTransparency;

   Gstr              *font;
   Gr4                fontSize;
   Gb                 fontIsBold,
                      fontIsItalic,
                      fontIsUnderline,
                      fontIsStrikeout;
} GmineInfoProperty;

typedef struct
{
   GmineInfoValuable  value;
   GmineInfoProperty  property;
};

typedef struct
{
   Gbit               isSetAuthorName     : 1,
                      isSetCompanyName    : 1,
                      isSetCopyright      : 1,
                      isSetProjectName    : 1,
                      isSetProjectSystem  : 1,
                      isSetProjectMax     : 1,
                      isSetProjectMin     : 1;

   Gstr              *authorName,
                     *companyName,
                     *copyright,
                     *projectName,
                     *projectSystem;
   GmineInfoPoint     projectMax,
                      projectMin;

   GmineInfoArray     keyValueArray;
} GmineInfoData;

typedef struct GmineInfoItem GmineInfoItem;
struct GmineInfoItem
{
   Gbit               isSetDefault        : 1,
                      isSetMax            : 1,
                      isSetMin            : 1,
                      isSetName           : 1,
                      isSetPrecision      : 1,
                      isSetType           : 1,
                      isSetUid            : 1;

   // Unique id for this item, library provided.
   Gn4                id;

   // User provided
   GmineInfoValuable  defaultValue;
   Gstr              *key;
   GmineInfoValuable  max;
   GmineInfoValuable  min;
   Gstr              *name;
   Gr8                precision;
   GmineInfoItemType  type;

   GmineInfoArray     keyValueArray;
   GmineInfoArray     valueArray;
   GmineInfoArray     binArray;

   GmineInfoItem     *next;
};

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

   GmineInfoData      data;
   GmineInfoItem     *itemHead,
                     *itemTail;
   GmineInfoGeometry  geometry;
   GmineInfoDrillHole drillhole;
   GmineInfoModel     model;
} GmineInfo;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/


#endif