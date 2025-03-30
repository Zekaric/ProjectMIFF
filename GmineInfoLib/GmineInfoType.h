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
// Enumerations ///////////////////////////////////////////////////////////////////////////////////
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

   // Basic types.
   gmineInfoItemTypeB,
   gmineInfoItemTypeI,
   gmineInfoItemTypeN,
   gmineInfoItemTypeR,
   gmineInfoItemTypeSTR,

   // Time types.
   gmineInfoItemTypeDATE,
   gmineInfoItemTypeTIME,
   gmineInfoItemTypeDATETIME,

   // Precent type.
   gmineInfoItemTypePERCENT,

   // Formula type.
   gmineInfoItemTypeFORMULA,

   gmineInfoItemTypeCOUNT
} GmineInfoItemType;

// Basic types ////////////////////////////////////////////////////////////////////////////////////
typedef union
{
   Gb                    b;
   Gi8                   i;
   Gn8                   n;
   Gr8                   r;
   Gstr                 *str;
} GmineInfoValueBase;

typedef struct
{
   Gcount                count,
                         countMax;
   void                **buffer;
} GmineInfoArray;

typedef struct
{
   Gb                    isSet;
   GmineInfoValueBase    value;
} GmineInfoValue;

typedef struct
{
   // Using x, y, z because these can be interpreted differently depending on coordinate system used
   double                x, // Easting, longitude
                         y, // Northing, latitude,
                         z; // Elevation
} GmineInfoPoint;

typedef struct
{
   float                 r,
                         g,
                         b;
} GmineInfoColor;

typedef struct
{
   float                 r,
                         g,
                         b,
                         a;
} GmineInfoColorA;

typedef struct
{
   // Using x, y, z because these can be interpreted differently depending on coordinate system used
   double                x, // Easting, longitude
                         y, // Northing, latitude,
                         z; // Elevation
} GmineInfoVector;

typedef struct
{
   Gstr                 *key,
                        *value;
} GmineInfoKeyValue;

// GmineInfo Types ////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   Gb                    isSetAuthorName,
                         isSetCompanyName,
                         isSetCopyright,
                         isSetProjectMax,
                         isSetProjectMin,
                         isSetProjectName,
                         isSetProjectSystem;

   Gstr                 *authorName,
                        *companyName,
                        *copyright;
   GmineInfoPoint        projectMax,
                         projectMin;
   Gstr                 *projectName,
                        *projectSystem;

   GmineInfoArray        keyValueArray;
} GmineInfoData;

typedef struct
{
   Gb                    isSetIsArrowSizeRelative,
                         isSetIsFontBold,
                         isSetIsFontItalic,
                         isSetIsFontSizeRelative,
                         isSetIsFontStrikeOut,
                         isSetIsFontUnderline,
                         isSetIsVisible,
                         isSetIsVisibleArrow,
                         isSetIsVisibleDataLabel[10],
                         isSetIsVisibleFace,
                         isSetIsVisibleLine,
                         isSetIsVisibleLineLabel[10],
                         isSetIsVisibleNode,
                         isSetIsVisibleNodeLabel[20],
                         isSetIsVisibleText,
                         isSetArrowPosition,
                         isSetArrowSize,
                         isSetColor,
                         isSetColorFace,
                         isSetColorFacePattern,
                         isSetColorLine,
                         isSetColorNode,
                         isSetColorText,
                         isSetFaceTransparency,
                         isSetFontName,
                         isSetFontSize,
                         isSetKey,
                         isSetLabelData[10],
                         isSetLabelLine[10],
                         isSetLabelNode[10],
                         isSetName,
                         isSetPatternArrow,
                         isSetPatternFace,
                         isSetPatternLine,
                         isSetPatternNode,

                         isArrowSizeRelative,
                         isFontBold,
                         isFontItalic,
                         isFontSizeRelative,
                         isFontStrikeOut,
                         isFontUnderline,
                         isVisible,
                         isVisibleArrow,
                         isVisibleDataLabel[10],
                         isVisibleFace,
                         isVisibleLine,
                         isVisibleLineLabel[10],
                         isVisibleNode,
                         isVisibleNodeLabel[20],
                         isVisibleText;

   Gi4                   arrowPosition;
   Gr4                   arrowSize;

   GmineInfoColor        color,
                         colorFace,
                         colorFacePattern,
                         colorLine,
                         colorNode,
                         colorText;

   Gr4                   faceTransparency;

   Gstr                 *fontName;
   Gr4                   fontSize;

   Gstr                 *key;

   Gstr                 *labelData[10],
                        *labelLine[10],
                        *labelNode[20];

   Gstr                 *name;

   Gstr                 *patternArrow,
                        *patternFace,
                        *patternLine,
                        *patternNode;

   GmineInfoArray        keyValueArray;
} GmineInfoProperty;

typedef struct
{
   Gb                    isSetIsVisibleDrillHole,
                         isSetIsVisibleDrillHoleFace,
                         isSetIsVisibleDrillHoleLine,
                         isSetIsVisibleDrillHoleNode,
                         isSetIsVisibleGeometry,
                         isSetIsVisibleGeometryFace,
                         isSetIsVisibleGeometryLine,
                         isSetIsVisibleGeometryNode,
                         isSetIsVisibleModel,
                         isSetIsVisibleModelFace,
                         isSetIsVisibleModelLine,
                         isSetIsVisibleModelNode,
                         isSetColorDrillHole,
                         isSetColorDrillHoleFace,
                         isSetColorDrillHoleLine,
                         isSetColorDrillHoleNode,
                         isSetColorGeometry,
                         isSetColorGeometryFace,
                         isSetColorGeometryLine,
                         isSetColorGeometryNode,
                         isSetColorModel,
                         isSetColorModelFace,
                         isSetColorModelLine,
                         isSetColorModelNode,
                         isSetValue,

                         isVisibleDrillHole,
                         isVisibleDrillHoleFace,
                         isVisibleDrillHoleLine,
                         isVisibleDrillHoleNode,
                         isVisibleGeometry,
                         isVisibleGeometryFace,
                         isVisibleGeometryLine,
                         isVisibleGeometryNode,
                         isVisibleModel,
                         isVisibleModelFace,
                         isVisibleModelLine,
                         isVisibleModelNode;

   GmineInfoColor        colorDrillHole,
                         colorDrillHoleFace,
                         colorDrillHoleLine,
                         colorDrillHoleNode,
                         colorGeometry,
                         colorGeometryFace,
                         colorGeometryLine,
                         colorGeometryNode,
                         colorModel,
                         colorModelFace,
                         colorModelLine,
                         colorModelNode;

   GmineInfoProperty     property;

   GmineInfoValueBase    value;
} GmineInfoPropertyItem;

typedef struct
{
   Gb                    isSetDefault,
                         isSetKey,
                         isSetMax,
                         isSetMin,
                         isSetName,
                         isSetPrecision,
                         isSetType;

   GmineInfoValueBase    defaultValue;
   Gstr                 *key;
   GmineInfoValueBase    max;
   GmineInfoValueBase    min;
   Gstr                 *name;
   Gr8                   precision;
   GmineInfoItemType     type;

   GmineInfoArray        keyValueArray;
   GmineInfoArray        valueArray;
   GmineInfoArray        propertyItemArray;
} GmineInfoItem;

typedef struct
{
   Gstr                 *id;
} GmineInfoGeometry;

typedef struct
{
   Gstr                 *id;
} GmineInfoDrillHole;

typedef struct
{
   Gstr                 *id;
} GmineInfoModel;

typedef struct
{
   Gb                    isWriting;
   Gb                    isByteSwapping;
   GgetBuffer            getBuffer;
   GsetBuffer            setBuffer;

   void                 *dataRepo;

   GmineInfoFileType     fileType;
   GmineInfoBlockType    currentBlockType;

   Gmiff                *miffFile;
   Gjson                *jsonFile;

   GmineInfoData        *data;
   GmineInfoArray        imageArray;
   GmineInfoArray        itemArray;
   GmineInfoArray        propertyArray;
} GmineInfo;

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/


#endif