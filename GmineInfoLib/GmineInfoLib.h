/**************************************************************************************************
file:       MineInfo
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Library for import and exporting mine information.  Hopefully to aid in sharing information between
mining software packages.
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

#if !defined(MINE_INFO_H)
#define      MINE_INFO_H

/**************************************************************************************************
include:
**************************************************************************************************/
#include "gmifflib.h"
#include "gjsonlib.h"

#include "GMineInfoType.h"

/**************************************************************************************************
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
// Array functions.
Gb                       gmineInfoArrayAddLast(                            GmineInfoArray       * const gmineInfoArray, void * const value);

GmineInfoArray          *gmineInfoArrayCloc(                               void);
Gb                       gmineInfoArrayClocContent(                        GmineInfoArray       * const gmineInfoArray);

void                     gmineInfoArrayDloc(                               GmineInfoArray       * const gmineInfoArray);
void                     gmineInfoArrayDlocContent(                        GmineInfoArray       * const gmineInfoArray);

void                     gmineInfoArrayForEach(                            GmineInfoArray       * const gmineInfoArray, void (*func)(void *));

void                    *gmineInfoArrayGetAt(                              GmineInfoArray const * const gmineInfoArray, Gindex const index);
Gcount                   gmineInfoArrayGetCount(                           GmineInfoArray const * const gmineInfoArray);

// Key Value functions
GmineInfoKeyValue       *gmineInfoKeyValueCloc(                                                                         Gstr const * key, Gstr const * value);
Gb                       gmineInfoKeyValueClocContent(                     GmineInfoKeyValue       * const gmineInfoKeyValue, Gstr const * key, Gstr const * value);

void                     gmineInfoKeyValueDloc(                            GmineInfoKeyValue       * const gmineInfoKeyValue);
void                     gmineInfoKeyValueDlocContent(                     GmineInfoKeyValue       * const gmineInfoKeyValue);

Gstr const              *gmineInfoKeyValueGetKey(                          GmineInfoKeyValue const * const gmineInfoKeyValue);
Gstr const              *gmineInfoKeyValueGetValue(                        GmineInfoKeyValue const * const gmineInfoKeyValue);

Gb                       gmineInfoKeyValueSetKey(                          GmineInfoKeyValue       * const gmineInfoKeyValue, Gstr const * const value);
Gb                       gmineInfoKeyValueSetValue(                        GmineInfoKeyValue       * const gmineInfoKeyValue, Gstr const * const value);

Gb                       gmineInfoKeyValueArrayAdd(                        GmineInfoArray       * const array, Gstr const * const key, Gstr const * const value);
Gb                       gmineInfoKeyValueArrayGetAt(                      GmineInfoArray const * const array, Gindex const index, Gstr const ** const key, Gstr const ** const value);
Gb                       gmineInfoKeyValueArrayGetCount(                   GmineInfoArray const * const array);

#define gmineInfoBlockKeyValueArrayAdd(     DATA,        KEY, VALUE)   gmineInfoKeyValueArrayAdd(     &(DATA)->keyValueArray,          (KEY), (VALUE))
#define gmineInfoBlockKeyValueArrayCloc(    DATA)                      gmineInfoArrayClocContent(     &(DATA)->keyValueArray);
#define gmineInfoBlockKeyValueArrayDloc(    DATA)                      {                                                                                 \
                                                                          gmineInfoArrayForEach(      &(DATA)->keyValueArray, gmineInfoKeyValueDloc); \
                                                                          gmineInfoArrayDlocContent(  &(DATA)->keyValueArray);                        \
                                                                       }
#define gmineInfoBlockKeyValueArrayGetAt(   DATA, INDEX, KEY, VALUE)   gmineInfoKeyValueArrayGetAt(   &(DATA)->keyValueArray, (INDEX), (KEY), (VALUE))
#define gmineInfoBlockKeyValueArrayGetCount(DATA)                      gmineInfoKeyValueArrayGetCount(&(DATA)->keyValueArray)

// Main GmineInfo functions
Gb                       gmineInfoAddImage(                                GmineInfo       * const gmineInfo, GmineInfoImage * const image);

GmineInfoBlockType       gmineInfoBlockTypeNext(                           GmineInfo       * const gmineInfo);

GmineInfo               *gmineInfoClocReader(                                                                 GmineInfoFileType const fileType, GgetBuffer getBufferFunc, void * const dataRepo);
Gb                       gmineInfoClocReaderContent(                       GmineInfo       * const gmineInfo, GmineInfoFileType const fileType, GgetBuffer getBufferFunc, void * const dataRepo);
GmineInfo               *gmineInfoClocWriter(                                                                 GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);
Gb                       gmineInfoClocWriterContent(                       GmineInfo       * const gmineInfo, GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);

void                     gmineInfoDloc(                                    GmineInfo       * const gmineInfo);
void                     gmineInfoDlocContent(                             GmineInfo       * const gmineInfo);

Gb                       gmineInfoIsStarted(                               void);

Gb                       gmineInfoSetData(                                 GmineInfo       * const gmineInfo, GmineInfoData * const value);
Gb                       gmineInfoSetImageListAppend(                      GmineInfo       * const gmineInfo, GmineInfoImage * const gmineInfoImage);
Gb                       gmineInfoSetItemListAppend(                       GmineInfo       * const gmineInfo, GmineInfoItem * const gmineInfoItem);
Gb                       gmineInfoSetPropertyListAppend(                   GmineInfo       * const gmineInfo, GmineInfoProperty * const gmineInfoProperty);
Gb                       gmineInfoStart(                                   GmemCloc memClocFunc, GmemDloc memDlocFunc);
void                     gmineInfoStop(                                    void);

Gb                       gmineInfoWriteBlockContentData(                   GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockContentImageList(              GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockContentItemList(               GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockContentPropertyList(           GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockStartData(                     GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockStartImageList(                GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockStartItemList(                 GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockStartPropertyList(             GmineInfo       * const gmineInfo);
Gb                       gmineInfoWriteBlockStop(                          GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteDrillHoleBlockStart(                GmineInfo       * const gmineInfo, GmineInfoDrillHole * const gmineInfoDrillHole);
//Gb                       gmineInfoWriteDrillHoleBlockData(                 GmineInfo       * const gmineInfo, ...);
//Gb                       gmineInfoWriteDrillHoleBlockStop(                 GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteGeometryListBlockStart(             GmineInfo       * const gmineInfo, GmineInfoGeometry * const gmineInfoGeometry);
//Gb                       gmineInfoWriteGeometryListBlockData(              GmineInfo       * const gmineInfo, ...);
//Gb                       gmineInfoWriteGeometryListBlockStop(              GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteModelListBlockStart(                GmineInfo       * const gmineInfo, GmineInfoModel * const gmineInfoModel);
//Gb                       gmineInfoWriteModelListBlockData(                 GmineInfo       * const gmineInfo, ...);
//Gb                       gmineInfoWriteModelListBlockStop(                 GmineInfo       * const gmineInfo);

// GmineInfoData functions
GmineInfoData           *gmineInfoDataCloc(                                void);
Gb                       gmineInfoDataClocContent(                         GmineInfoData       * const gmineInfoData);

void                     gmineInfoDataDloc(                                GmineInfoData       * const gmineInfoData);
void                     gmineInfoDataDlocContent(                         GmineInfoData       * const gmineInfoData);

Gstr const              *gmineInfoDataGetAuthorName(                       GmineInfoData const * const gmineInfoData);
Gstr const              *gmineInfoDataGetCompanyName(                      GmineInfoData const * const gmineInfoData);
Gstr const              *gmineInfoDataGetCopyright(                        GmineInfoData const * const gmineInfoData);
GmineInfoPoint const    *gmineInfoDataGetProjectMax(                       GmineInfoData const * const gmineInfoData);
GmineInfoPoint const    *gmineInfoDataGetProjectMin(                       GmineInfoData const * const gmineInfoData);
Gstr const              *gmineInfoDataGetProjectName(                      GmineInfoData const * const gmineInfoData);
Gstr const              *gmineInfoDataGetProjectSystem(                    GmineInfoData const * const gmineInfoData);

Gb                       gmineInfoDataIsSetAuthorName(                     GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataIsSetCompanyName(                    GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataIsSetCopyright(                      GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataIsSetProjectMax(                     GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataIsSetProjectMin(                     GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataIsSetProjectName(                    GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataIsSetProjectSystem(                  GmineInfoData const * const gmineInfoData);

Gb                       gmineInfoDataSetAuthorName(                       GmineInfoData       * const gmineInfoData, Gstr const * const value);
Gb                       gmineInfoDataSetCompanyName(                      GmineInfoData       * const gmineInfoData, Gstr const * const value);
Gb                       gmineInfoDataSetCopyright(                        GmineInfoData       * const gmineInfoData, Gstr const * const value);
Gb                       gmineInfoDataSetProjectMax(                       GmineInfoData       * const gmineInfoData, GmineInfoPoint const * const value);
Gb                       gmineInfoDataSetProjectMin(                       GmineInfoData       * const gmineInfoData, GmineInfoPoint const * const value);
Gb                       gmineInfoDataSetProjectName(                      GmineInfoData       * const gmineInfoData, Gstr const * const value);
Gb                       gmineInfoDataSetProjectSystem(                    GmineInfoData       * const gmineInfoData, Gstr const * const value);


// GmineInfoImage functions.
GmineInfoImage          *gmineInfoImageCloc(                               void);
Gb                       gmineInfoImageClocContent(                        GmineInfoImage       * const gmineInfoImage);

void                     gmineInfoImageDloc(                               GmineInfoImage       * const gmineInfoImage);
void                     gmineInfoImageDlocContent(                        GmineInfoImage       * const gmineInfoImage);

Gstr const              *gmineInfoImageGetFileName(                        GmineInfoImage const * const gmineInfoImage);
Gstr const              *gmineInfoImageGetFilePath(                        GmineInfoImage const * const gmineInfoImage);
Gstr const              *gmineInfoImageGetKey(                             GmineInfoImage const * const gmineInfoImage);
Gstr const              *gmineInfoImageGetName(                            GmineInfoImage const * const gmineInfoImage);

Gb                       gmineInfoImageIsInline(                           GmineInfoImage const * const gmineInfoImage);
Gb                       gmineInfoImageIsSetFileName(                      GmineInfoImage const * const gmineInfoImage);
Gb                       gmineInfoImageIsSetFilePath(                      GmineInfoImage const * const gmineInfoImage);
Gb                       gmineInfoImageIsSetIsInline(                      GmineInfoImage const * const gmineInfoImage);
Gb                       gmineInfoImageIsSetKey(                           GmineInfoImage const * const gmineInfoImage);
Gb                       gmineInfoImageIsSetName(                          GmineInfoImage const * const gmineInfoImage);

Gb                       gmineInfoImageSetFileName(                        GmineInfoImage       * const gmineInfoImage, Gstr const * const value);
Gb                       gmineInfoImageSetFilePath(                        GmineInfoImage       * const gmineInfoImage, Gstr const * const value);
Gb                       gmineInfoImageSetIsInline(                        GmineInfoImage       * const gmineInfoImage, Gb const value);
Gb                       gmineInfoImageSetKey(                             GmineInfoImage       * const gmineInfoImage, Gstr const * const value);
Gb                       gmineInfoImageSetName(                            GmineInfoImage       * const gmineInfoImage, Gstr const * const value);


// GmineInfoItem functions.
GmineInfoItem           *gmineInfoItemCloc(                                void);
Gb                       gmineInfoItemClocContent(                         GmineInfoItem       * const gmineInfoItem);

void                     gmineInfoItemDloc(                                GmineInfoItem       * const gmineInfoItem);
void                     gmineInfoItemDlocContent(                         GmineInfoItem       * const gmineInfoItem);

Gb                       gmineInfoItemGetDefaultValueB(                    GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetDefaultValueDate(                 GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetDefaultValueDateTime(             GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetDefaultValueFormula(              GmineInfoItem const * const gmineInfoItem);
Gi8                      gmineInfoItemGetDefaultValueI(                    GmineInfoItem const * const gmineInfoItem);
Gn8                      gmineInfoItemGetDefaultValueN(                    GmineInfoItem const * const gmineInfoItem);
Gr8                      gmineInfoItemGetDefaultValuePercent(              GmineInfoItem const * const gmineInfoItem);
Gr8                      gmineInfoItemGetDefaultValueR(                    GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetDefaultValueStr(                  GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetDefaultValueTime(                 GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetKey(                              GmineInfoItem const * const gmineInfoItem);
Gi8                      gmineInfoItemGetMaxI(                             GmineInfoItem const * const gmineInfoItem);
Gn8                      gmineInfoItemGetMaxN(                             GmineInfoItem const * const gmineInfoItem);
Gr8                      gmineInfoItemGetMaxR(                             GmineInfoItem const * const gmineInfoItem);
Gi8                      gmineInfoItemGetMinI(                             GmineInfoItem const * const gmineInfoItem);
Gn8                      gmineInfoItemGetMinN(                             GmineInfoItem const * const gmineInfoItem);
Gr8                      gmineInfoItemGetMinR(                             GmineInfoItem const * const gmineInfoItem);
Gstr const              *gmineInfoItemGetName(                             GmineInfoItem const * const gmineInfoItem);
Gr8                      gmineInfoItemGetPrecision(                        GmineInfoItem const * const gmineInfoItem);
GmineInfoItemType        gmineInfoItemGetType(                             GmineInfoItem const * const gmineInfoItem);

Gb                       gmineInfoItemIsSetDefault(                        GmineInfoItem const * const gmineInfoItem);
Gb                       gmineInfoItemIsSetKey(                            GmineInfoItem const * const gmineInfoItem);
Gb                       gmineInfoItemIsSetMax(                            GmineInfoItem const * const gmineInfoItem);
Gb                       gmineInfoItemIsSetMin(                            GmineInfoItem const * const gmineInfoItem);
Gb                       gmineInfoItemIsSetName(                           GmineInfoItem const * const gmineInfoItem);
Gb                       gmineInfoItemIsSetPrecision(                      GmineInfoItem const * const gmineInfoItem);
Gb                       gmineInfoItemIsSetType(                           GmineInfoItem const * const gmineInfoItem);

Gb                       gmineInfoItemSetDefaultB(                         GmineInfoItem       * const gmineInfoItem, Gb const value);
Gb                       gmineInfoItemSetDefaultDate(                      GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetDefaultDateTime(                  GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetDefaultFormula(                   GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetDefaultI(                         GmineInfoItem       * const gmineInfoItem, Gi8 const value);
Gb                       gmineInfoItemSetDefaultN(                         GmineInfoItem       * const gmineInfoItem, Gn8 const value);
Gb                       gmineInfoItemSetDefaultPercent(                   GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                       gmineInfoItemSetDefaultR(                         GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                       gmineInfoItemSetDefaultStr(                       GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetDefaultTime(                      GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetKey(                              GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetMaxI(                             GmineInfoItem       * const gmineInfoItem, Gi8 const value);
Gb                       gmineInfoItemSetMaxN(                             GmineInfoItem       * const gmineInfoItem, Gn8 const value);
Gb                       gmineInfoItemSetMaxR(                             GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                       gmineInfoItemSetMinI(                             GmineInfoItem       * const gmineInfoItem, Gi8 const value);
Gb                       gmineInfoItemSetMinN(                             GmineInfoItem       * const gmineInfoItem, Gn8 const value);
Gb                       gmineInfoItemSetMinR(                             GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                       gmineInfoItemSetName(                             GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                       gmineInfoItemSetPrecision(                        GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                       gmineInfoItemSetType(                             GmineInfoItem       * const gmineInfoItem, GmineInfoItemType const value);

// GmineInfoProperty
GmineInfoProperty       *gmineInfoPropertyCloc(                            void);
Gb                       gmineInfoPropertyClocContent(                     GmineInfoProperty       * const gmineInfoProperty);

void                     gmineInfoPropertyDloc(                            GmineInfoProperty       * const gmineInfoProperty);
void                     gmineInfoPropertyDlocContent(                     GmineInfoProperty       * const gmineInfoProperty);

Gi4                      gmineInfoPropertyGetArrowPosition(                GmineInfoProperty const * const gmineInfoProperty);
Gr4                      gmineInfoPropertyGetArrowSize(                    GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColor(                        GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorPoint(                   GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorPolylineFace(            GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorPolylineFacePattern(     GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorPolylineLine(            GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorPolylineNode(            GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorSurfaceFace(             GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorSurfaceFacePattern(      GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorSurfaceLine(             GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorSurfaceNode(             GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorText(                    GmineInfoProperty const * const gmineInfoProperty);
Gr4                      gmineInfoPropertyGetFaceTransparency(             GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetFontName(                     GmineInfoProperty const * const gmineInfoProperty);
Gr4                      gmineInfoPropertyGetFontSize(                     GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetKey(                          GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetLabelData(                    GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gstr const              *gmineInfoPropertyGetLabelLine(                    GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gstr const              *gmineInfoPropertyGetLabelNode(                    GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gstr const              *gmineInfoPropertyGetName(                         GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternArrow(                 GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternFace(                  GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternLine(                  GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternNode(                  GmineInfoProperty const * const gmineInfoProperty);

Gb                       gmineInfoPropertyIsSetIsArrowSizeRelative(        GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsClippable(                GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontBold(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontItalic(               GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontSizeRelative(         GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontStrikeOut(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontUnderline(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsSelectable(               GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisible(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleDataLabel(         GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetIsVisibleLineLabel(         GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetIsVisibleNodeLabel(         GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetIsVisiblePoint(             GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisiblePolylineArrow(     GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisiblePolylineFace(      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisiblePolylineLine(      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisiblePolylineNode(      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleSurfaceFace(       GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleSurfaceLine(       GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleSurfaceNode(       GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleText(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetArrowPosition(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetArrowSize(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColor(                      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorPoint(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorPolylineFace(          GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorPolylineFacePattern(   GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorPolylineLine(          GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorPolylineNode(          GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorSurfaceFace(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorSurfaceFacePattern(    GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorSurfaceLine(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorSurfaceNode(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorText(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetFaceTransparency(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetFontName(                   GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetFontSize(                   GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetKey(                        GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetLabelData(                  GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetLabelLine(                  GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetLabelNode(                  GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetName(                       GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetPatternArrow(               GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetPatternFace(                GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetPatternLine(                GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetPatternNode(                GmineInfoProperty const * const gmineInfoProperty);

Gb                       gmineInfoPropertyIsArrowSizeRelative(             GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsClippable(                     GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontBold(                      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontItalic(                    GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontSizeRelative(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontStrikeOut(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontUnderline(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSelectable(                    GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisible(                       GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleDataLabel(              GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsVisibleLineLabel(              GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsVisibleNodeLabel(              GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsVisiblePoint(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisiblePolylineArrow(          GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisiblePolylineFace(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisiblePolylineLine(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisiblePolylineNode(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleSurfaceFace(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleSurfaceLine(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleSurfaceNode(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleText(                   GmineInfoProperty const * const gmineInfoProperty);

Gb                       gmineInfoPropertySetArrowPosition(                GmineInfoProperty       * const gmineInfoProperty, Gi4 const value);
Gb                       gmineInfoPropertySetArrowSize(                    GmineInfoProperty       * const gmineInfoProperty, Gr4 const value);
Gb                       gmineInfoPropertySetColor(                        GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorPoint(                   GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorPolylineFace(            GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorPolylineFacePattern(     GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorPolylineLine(            GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorPolylineNode(            GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorSurfaceFace(             GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorSurfaceFacePattern(      GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorSurfaceLine(             GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorSurfaceNode(             GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorText(                    GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetIsArrowSizeRelative(          GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsClippable(                  GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontBold(                   GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontItalic(                 GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontSizeRelative(           GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontStrikeOut(              GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontUnderline(              GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsSelectable(                 GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisible(                    GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleDataLabel(           GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleLineLabel(           GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleNodeLabel(           GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gb const value);
Gb                       gmineInfoPropertySetIsVisiblePoint(               GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisiblePolylineArrow(       GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisiblePolylineFace(        GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisiblePolylineLine(        GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisiblePolylineNode(        GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleSurfaceFace(         GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleSurfaceLine(         GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleSurfaceNode(         GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleText(                GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetFaceTransparency(             GmineInfoProperty       * const gmineInfoProperty, Gr4 const value);
Gb                       gmineInfoPropertySetFontName(                     GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetFontSize(                     GmineInfoProperty       * const gmineInfoProperty, Gr4 const value);
Gb                       gmineInfoPropertySetKey(                          GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetLabelData(                    GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gstr const * const value);
Gb                       gmineInfoPropertySetLabelLine(                    GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gstr const * const value);
Gb                       gmineInfoPropertySetLabelNode(                    GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gstr const * const value);
Gb                       gmineInfoPropertySetName(                         GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternArrow(                 GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternFace(                  GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternLine(                  GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternNode(                  GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);

// GmineInfoItemBin
GmineInfoItemBin        *gmineInfoItemBinCloc(                             void);
Gb                       gmineInfoItemBinClocContent(                      GmineInfoItemBin       * const gmineInfoItemBin);

void                     gmineInfoItemBinDloc(                             GmineInfoItemBin       * const gmineInfoItemBin);
void                     gmineInfoItemBinDlocContent(                      GmineInfoItemBin       * const gmineInfoItemBin);

GmineInfoColor const    *gmineInfoItemBinGetColorDilllHole(                GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorDrillHoleFace(            GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorDrillHoleLine(            GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorDrillHoleNode(            GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorGeometry(                 GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorGeometryFace(             GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorGeometryLine(             GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorGeometryNode(             GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorModel(                    GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorModelFace(                GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorModelLine(                GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoColor const    *gmineInfoItemBinGetColorModelNode(                GmineInfoItemBin const * const gmineInfoItemBin);
GmineInfoProperty       *gmineInfoItemBinGetProperty(                      GmineInfoItemBin       * const gmineInfoItemBin);
Gb                       gmineInfoItemBinGetValueB(                        GmineInfoItemBin const * const gmineInfoItemBin);
Gi8                      gmineInfoItemBinGetValueI(                        GmineInfoItemBin const * const gmineInfoItemBin);
Gn8                      gmineInfoItemBinGetValueN(                        GmineInfoItemBin const * const gmineInfoItemBin);
Gr8                      gmineInfoItemBinGetValueR(                        GmineInfoItemBin const * const gmineInfoItemBin);
Gstr const              *gmineInfoItemBinGetValueStr(                      GmineInfoItemBin const * const gmineInfoItemBin);

Gb                       gmineInfoItemBinIsSetIsVisibleDrillHole(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleDrillHoleFace(      GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleDrillHoleLine(      GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleDrillHoleNode(      GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleGeometry(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleGeometryFace(       GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleGeometryLine(       GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleGeometryNode(       GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleModel(              GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleModelFace(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleModelLine(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetIsVisibleModelNode(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorDrillHole(              GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorDrillHoleFace(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorDrillHoleLine(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorDrillHoleNode(          GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorGeometry(               GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorGeometryFace(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorGeometryLine(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorGeometryNode(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorModelFace(              GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorModelLine(              GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsSetColorModelNode(              GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleDrillHole(               GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleDrillHoleFace(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleDrillHoleLine(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleDrillHoleNode(           GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleGeometry(                GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleGeometryFace(            GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleGeometryLine(            GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleGeometryNode(            GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleModelFace(               GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleModelLine(               GmineInfoItemBin const * const gmineInfoItemBin);
Gb                       gmineInfoItemBinIsVisibleModelNode(               GmineInfoItemBin const * const gmineInfoItemBin);

Gb                       gmineInfoItemBinSetColorDrillHole(                GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorDrillHoleFace(            GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorDrillHoleLine(            GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorDrillHoleNode(            GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorGeometry(                 GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorGeometryFace(             GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorGeometryLine(             GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorGeometryNode(             GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorModel(                    GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorModelFace(                GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorModelLine(                GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetColorModelNode(                GmineInfoItemBin       * const gmineInfoItemBin, GmineInfoColor const * const value);
Gb                       gmineInfoItemBinSetIsVisibleDrillHole(            GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleDrillHoleFace(        GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleDrillHoleLine(        GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleDrillHoleNode(        GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleGeometry(             GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleGeometryFace(         GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleGeometryLine(         GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleGeometryNode(         GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleModel(                GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleModelFace(            GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleModelLine(            GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetIsVisibleModelNode(            GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetValueB(                        GmineInfoItemBin       * const gmineInfoItemBin, Gb const value);
Gb                       gmineInfoItemBinSetValueI(                        GmineInfoItemBin       * const gmineInfoItemBin, Gi8 const value);
Gb                       gmineInfoItemBinSetValueN(                        GmineInfoItemBin       * const gmineInfoItemBin, Gn8 const value);
Gb                       gmineInfoItemBinSetValueR(                        GmineInfoItemBin       * const gmineInfoItemBin, Gr8 const value);
Gb                       gmineInfoItemBinSetValueStr(                      GmineInfoItemBin       * const gmineInfoItemBin, Gstr const * const value);

#endif