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
GmineInfoBlockType gmineInfoBlockTypeNext(                     GmineInfo       * const gmineInfo);

GmineInfo         *gmineInfoClocReader(                                                           GmineInfoFileType const fileType, GgetBuffer getBufferFunc, void * const dataRepo);
Gb                 gmineInfoClocReaderContent(                 GmineInfo       * const gmineInfo, GmineInfoFileType const fileType, GgetBuffer getBufferFunc, void * const dataRepo);
GmineInfo         *gmineInfoClocWriter(                                                           GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);
Gb                 gmineInfoClocWriterContent(                 GmineInfo       * const gmineInfo, GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);

void               gmineInfoDloc(                              GmineInfo       * const gmineInfo);
void               gmineInfoDlocContent(                       GmineInfo       * const gmineInfo);

Gb                 gmineInfoSetDataAuthorName(                 GmineInfo       * const gmineInfo, Gstr const * const value);
Gb                 gmineInfoSetDataCompanyName(                GmineInfo       * const gmineInfo, Gstr const * const value);
Gb                 gmineInfoSetDataCopyright(                  GmineInfo       * const gmineInfo, Gstr const * const value);
Gb                 gmineInfoSetDataProjectMax(                 GmineInfo       * const gmineInfo, GmineInfoPoint const * const value);
Gb                 gmineInfoSetDataProjectMin(                 GmineInfo       * const gmineInfo, GmineInfoPoint const * const value);
Gb                 gmineInfoSetDataProjectName(                GmineInfo       * const gmineInfo, Gstr const * const value);
Gb                 gmineInfoSetDataProjectSystem(              GmineInfo       * const gmineInfo, Gstr const * const value);
Gb                 gmineInfoSetDataOther(                      GmineInfo       * const gmineInfo, Gstr const * const key, Gstr const * const value);

Gb                 gmineInfoSetItemAppend(                     GmineInfo       * const gmineInfo, GmineInfoItem * const gmineInfoItem);

Gb                 gmineInfoWriteData(                         GmineInfo       * const gmineInfo);

Gb                 gmineInfoStart(                             GmemCloc memClocFunc, GmemDloc memDlocFunc);
void               gmineInfoStop(                              void);

// Item structure functions.
GmineInfoItem     *gmineInfoItemCloc(                          void);
Gb                 gmineInfoItemClocContent(                   GmineInfoItem       * const gmineInfoItem);

void               gmineInfoItemDloc(                          GmineInfoItem       * const gmineInfoItem);
void               gmineInfoItemDlocContent(                   GmineInfoItem       * const gmineInfoItem);

Gb                 gmineInfoItemSetDefaultDate(                GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetDefaultDateTime(            GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetDefaultFormula(             GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetDefaultI(                   GmineInfoItem       * const gmineInfoItem, Gi8 const value);
Gb                 gmineInfoItemSetDefaultN(                   GmineInfoItem       * const gmineInfoItem, Gn8 const value);
Gb                 gmineInfoItemSetDefaultPercent(             GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                 gmineInfoItemSetDefaultR(                   GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                 gmineInfoItemSetDefaultStr(                 GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetDefaultTime(                GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetKey(                        GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetMaxI(                       GmineInfoItem       * const gmineInfoItem, Gi8 const value);
Gb                 gmineInfoItemSetMaxN(                       GmineInfoItem       * const gmineInfoItem, Gn8 const value);
Gb                 gmineInfoItemSetMaxR(                       GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                 gmineInfoItemSetMinI(                       GmineInfoItem       * const gmineInfoItem, Gi8 const value);
Gb                 gmineInfoItemSetMinN(                       GmineInfoItem       * const gmineInfoItem, Gn8 const value);
Gb                 gmineInfoItemSetMinR(                       GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                 gmineInfoItemSetName(                       GmineInfoItem       * const gmineInfoItem, Gstr const * const value);
Gb                 gmineInfoItemSetOther(                      GmineInfoItem       * const gmineInfoItem, Gstr const * const key, Gstr const * const value);
Gb                 gmineInfoItemSetPrecision(                  GmineInfoItem       * const gmineInfoItem, Gr8 const value);
Gb                 gmineInfoItemSetType(                       GmineInfoItem       * const gmineInfoItem, GmineInfoItemType const value);

#endif