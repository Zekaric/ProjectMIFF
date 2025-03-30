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


// Main GmineInfo functions
GmineInfoBlockType       gmineInfoBlockTypeNext(                           GmineInfo       * const gmineInfo);

GmineInfo               *gmineInfoClocReader(                                                                 GmineInfoFileType const fileType, GgetBuffer getBufferFunc, void * const dataRepo);
Gb                       gmineInfoClocReaderContent(                       GmineInfo       * const gmineInfo, GmineInfoFileType const fileType, GgetBuffer getBufferFunc, void * const dataRepo);
GmineInfo               *gmineInfoClocWriter(                                                                 GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);
Gb                       gmineInfoClocWriterContent(                       GmineInfo       * const gmineInfo, GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);

void                     gmineInfoDloc(                                    GmineInfo       * const gmineInfo);
void                     gmineInfoDlocContent(                             GmineInfo       * const gmineInfo);

Gb                       gmineInfoIsStarted(                               void);

Gb                       gmineInfoSetData(                                 GmineInfo       * const gmineInfo, GmineInfoData * const value);
Gb                       gmineInfoSetItemAppend(                           GmineInfo       * const gmineInfo, GmineInfoItem * const gmineInfoItem);

Gb                       gmineInfoWriteDataBlock(                          GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteDrillHoleBlockStart(                GmineInfo       * const gmineInfo, GmineInfoDrillHole * const gmineInfoDrillHole);
//Gb                       gmineInfoWriteDrillHoleBlockData(                 GmineInfo       * const gmineInfo, ...);
//Gb                       gmineInfoWriteDrillHoleBlockStop(                 GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteGeometryBlockStart(                 GmineInfo       * const gmineInfo, GmineInfoGeometry * const gmineInfoGeometry);
//Gb                       gmineInfoWriteGeometryBlockData(                  GmineInfo       * const gmineInfo, ...);
//Gb                       gmineInfoWriteGeometryBlockStop(                  GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteImageBlock(                         GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteItemBlock(                          GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWriteModelBlockStart(                    GmineInfo       * const gmineInfo, GmineInfoModel * const gmineInfoModel);
//Gb                       gmineInfoWriteModelBlockData(                     GmineInfo       * const gmineInfo, ...);
//Gb                       gmineInfoWriteModelBlockStop(                     GmineInfo       * const gmineInfo);
//Gb                       gmineInfoWritePropertyBlock(                      GmineInfo       * const gmineInfo);

Gb                       gmineInfoStart(                                   GmemCloc memClocFunc, GmemDloc memDlocFunc);
void                     gmineInfoStop(                                    void);

// GmineInfoData functions
Gb                       gmineInfoDataAddKeyValue(                         GmineInfoData       * const gmineInfoData, Gstr const * const key, Gstr const * const value);

GmineInfoData           *gmineInfoDataCloc(                                void);
Gb                       gmineInfoDataClocContent(                         GmineInfoData       * const gmineInfoData);

void                     gmineInfoDataDloc(                                GmineInfoData       * const gmineInfoData);
void                     gmineInfoDataDlocContent(                         GmineInfoData       * const gmineInfoData);

Gstr const              *gmineInfoDataGetAuthorName(                       GmineInfoData const * const gmineInfoData);
Gstr const              *gmineInfoDataGetCompanyName(                      GmineInfoData const * const gmineInfoData);
Gstr const              *gmineInfoDataGetCopyright(                        GmineInfoData const * const gmineInfoData);
Gb                       gmineInfoDataGetKeyValueAt(                       GmineInfoData const * const gmineInfoData, Gindex const index, Gstr const ** const key, Gstr const ** const value);
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

// GmineInfoItem functions.
Gb                       gmineInfoItemAddKeyValue(                         GmineInfoItem       * const gmineInfoItem, Gstr const * const key, Gstr const * const value);

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
Gb                       gmineInfoItemGetKeyValueAt(                       GmineInfoItem const * const gmineInfoItem, Gindex const index, Gstr const ** const key, Gstr const ** const value);
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
Gb                       gmineInfoItemSetOther(                            GmineInfoItem       * const gmineInfoItem, Gstr const * const key, Gstr const * const value);
Gb                       gmineInfoItemSetPrecision(                        GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                       gmineInfoItemSetType(                             GmineInfoItem       * const gmineInfoItem, GmineInfoItemType const value);

// GmineInfoProperty
Gb                       gmineInfoPropertyAddKeyValue(                     GmineInfoProperty       * const gmineInfoProperty, Gstr const * const key, Gstr const * const value);

GmineInfoProperty       *gmineInfoPropertyCloc(                            void);
Gb                       gmineInfoPropertyClocContent(                     GmineInfoProperty       * const gmineInfoProperty);

void                     gmineInfoPropertyDloc(                            GmineInfoProperty       * const gmineInfoProperty);
void                     gmineInfoPropertyDlocContent(                     GmineInfoProperty       * const gmineInfoProperty);

Gi4                      gmineInfoPropertyGetArrowPosition(                GmineInfoProperty const * const gmineInfoProperty);
Gr4                      gmineInfoPropertyGetArrowSize(                    GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColor(                        GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorFace(                    GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorFacePattern(             GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorLine(                    GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorNode(                    GmineInfoProperty const * const gmineInfoProperty);
GmineInfoColor const    *gmineInfoPropertyGetColorText(                    GmineInfoProperty const * const gmineInfoProperty);
Gr4                      gmineInfoPropertyGetFaceTransparency(             GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetFontName(                     GmineInfoProperty const * const gmineInfoProperty);
Gr4                      gmineInfoPropertyGetFontSize(                     GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetKey(                          GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyGetKeyValueAt(                   GmineInfoProperty const * const gmineInfoProperty, Gindex const index, Gstr const ** const key, Gstr const ** const value);
Gstr const              *gmineInfoPropertyGetLabelData(                    GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gstr const              *gmineInfoPropertyGetLabelLine(                    GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gstr const              *gmineInfoPropertyGetLabelNode(                    GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gstr const              *gmineInfoPropertyGetName(                         GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternFace(                  GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternLine(                  GmineInfoProperty const * const gmineInfoProperty);
Gstr const              *gmineInfoPropertyGetPatternNode(                  GmineInfoProperty const * const gmineInfoProperty);

Gb                       gmineInfoPropertyIsSetIsArrowSizeRelative(        GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontBold(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontItalic(               GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontSizeRelative(         GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontStrikeOut(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsFontUnderline(            GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisible(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleDataLabel(         GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetIsVisibleFace(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleLine(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleLineLabel(         GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetIsVisibleNode(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetIsVisibleNodeLabel(         GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsSetIsVisibleText(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetArrowPosition(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetArrowSize(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColor(                      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorFace(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorFacePattern(           GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorLine(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsSetColorNode(                  GmineInfoProperty const * const gmineInfoProperty);
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
Gb                       gmineInfoPropertyIsFontBold(                      GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontItalic(                    GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontSizeRelative(              GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontStrikeOut(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsFontUnderline(                 GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisible(                       GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleArrow(                  GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleDataLabel(              GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsVisibleFace(                   GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleLine(                   GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleLineLabel(              GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsVisibleNode(                   GmineInfoProperty const * const gmineInfoProperty);
Gb                       gmineInfoPropertyIsVisibleNodeLabel(              GmineInfoProperty const * const gmineInfoProperty, Gindex const index);
Gb                       gmineInfoPropertyIsVisibleText(                   GmineInfoProperty const * const gmineInfoProperty);

Gb                       gmineInfoPropertySetArrowPosition(                GmineInfoProperty       * const gmineInfoProperty, Gi4 const value);
Gb                       gmineInfoPropertySetArrowSize(                    GmineInfoProperty       * const gmineInfoProperty, Gr4 const value);
Gb                       gmineInfoPropertySetColor(                        GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorFace(                    GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorFacePattern(             GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorLine(                    GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorNode(                    GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetColorText(                    GmineInfoProperty       * const gmineInfoProperty, GmineInfoColor const * const value);
Gb                       gmineInfoPropertySetIsArrowSizeRelative(          GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontBold(                   GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontItalic(                 GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontSizeRelative(           GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontStrikeOut(              GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsFontUnderline(              GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisible(                    GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleArrow(               GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleDataLabel(           GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleFace(                GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleLine(                GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleLineLabel(           GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleNode(                GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleNodeLabel(           GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gb const value);
Gb                       gmineInfoPropertySetIsVisibleText(                GmineInfoProperty       * const gmineInfoProperty, Gb const value);
Gb                       gmineInfoPropertySetFaceTransparency(             GmineInfoProperty       * const gmineInfoProperty, Gr4 const value);
Gb                       gmineInfoPropertySetFontName(                     GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetFontSize(                     GmineInfoProperty       * const gmineInfoProperty, Gr4 const value);
Gb                       gmineInfoPropertySetKey(                          GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetLabelData(                    GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gstr const * const value);
Gb                       gmineInfoPropertySetLabelLine(                    GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gstr const * const value);
Gb                       gmineInfoPropertySetLabelNode(                    GmineInfoProperty       * const gmineInfoProperty, Gindex const index, Gstr const * const value);
Gb                       gmineInfoPropertySetName(                         GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternFace(                  GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternLine(                  GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);
Gb                       gmineInfoPropertySetPatternNode(                  GmineInfoProperty       * const gmineInfoProperty, Gstr const * const value);

// GmineInfoPropertyItem
GmineInfoPropertyItem   *gmineInfoPropertyItemCloc(                        void);
Gb                       gmineInfoPropertyItemClocContent(                 GmineInfoPropertyItem       * const gmineInfoPropertyItem);
void                     gmineInfoPropertyItemDloc(                        GmineInfoPropertyItem       * const gmineInfoPropertyItem);
void                     gmineInfoPropertyItemDlocContent(                 GmineInfoPropertyItem       * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorDilllHole(           GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorDrillHoleFace(       GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorDrillHoleLine(       GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorDrillHoleNode(       GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorGeometry(            GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorGeometryFace(        GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorGeometryLine(        GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorGeometryNode(        GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorModel(               GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorModelFace(           GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorModelLine(           GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoColor const    *gmineInfoPropertyItemGetColorModelNode(           GmineInfoPropertyItem const * const gmineInfoPropertyItem);
GmineInfoProperty       *gmineInfoPropertyItemGetProperty(                 GmineInfoPropertyItem       * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemGetValueB(                   GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gi8                      gmineInfoPropertyItemGetValueI(                   GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gn8                      gmineInfoPropertyItemGetValueN(                   GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gr8                      gmineInfoPropertyItemGetValueR(                   GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gstr const              *gmineInfoPropertyItemGetValueStr(                 GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleDrillHole(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleDrillHoleFace( GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleDrillHoleLine( GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleDrillHoleNode( GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleGeometry(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleGeometryFace(  GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleGeometryLine(  GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleGeometryNode(  GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleModel(         GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleModelFace(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleModelLine(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetIsVisibleModelNode(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorDrillHole(         GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorDrillHoleFace(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorDrillHoleLine(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorDrillHoleNode(     GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorGeometry(          GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorGeometryFace(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorGeometryLine(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorGeometryNode(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorModelFace(         GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorModelLine(         GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsSetColorModelNode(         GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleDrillHole(          GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleDrillHoleFace(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleDrillHoleLine(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleDrillHoleNode(      GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleGeometry(           GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleGeometryFace(       GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleGeometryLine(       GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleGeometryNode(       GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleModelFace(          GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleModelLine(          GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemIsVisibleModelNode(          GmineInfoPropertyItem const * const gmineInfoPropertyItem);
Gb                       gmineInfoPropertyItemSetColorDrillHole(           GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorDrillHoleFace(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorDrillHoleLine(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorDrillHoleNode(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorGeometry(            GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorGeometryFace(        GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorGeometryLine(        GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorGeometryNode(        GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorModel(               GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorModelFace(           GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorModelLine(           GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetColorModelNode(           GmineInfoPropertyItem       * const gmineInfoPropertyItem, GmineInfoColor const * const value);
Gb                       gmineInfoPropertyItemSetIsVisibleDrillHole(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleDrillHoleFace(   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleDrillHoleLine(   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleDrillHoleNode(   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleGeometry(        GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleGeometryFace(    GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleGeometryLine(    GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleGeometryNode(    GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleModel(           GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleModelFace(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleModelLine(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetIsVisibleModelNode(       GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetValueB(                   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gb const value);
Gb                       gmineInfoPropertyItemSetValueI(                   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gi8 const value);
Gb                       gmineInfoPropertyItemSetValueN(                   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gn8 const value);
Gb                       gmineInfoPropertyItemSetValueR(                   GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gr8 const value);
Gb                       gmineInfoPropertyItemSetValueStr(                 GmineInfoPropertyItem       * const gmineInfoPropertyItem, Gstr const * const value);

#endif