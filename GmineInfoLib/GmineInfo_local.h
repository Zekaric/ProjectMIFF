/**************************************************************************************************
file:       GmineInfo_local
author:     Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Function local to the library
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

#if !defined(GMINEINFO_LOCAL)
#define      GMINEINFO_LOCAL

/**************************************************************************************************
include:
**************************************************************************************************/
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>

#include "GmineInfoLib.h"

/**************************************************************************************************
local:
constant:
**************************************************************************************************/
#define STRIFY(X)                                        #X
#define STRINGIFY(X)                                     STRIFY(X)

#define HEADER_TYPE_STR                                  "Mining Information"
#define HEADER_TYPE_LEN                                  18
#define HEADER_VERSION_NUM                               1
#define HEADER_VERSION_STR                               STRINGIFY(HEADER_VERSION_NUM)
#define HEADER_VERSION_LEN                               1

#define KEY_FORMAT_STR                                   "format"
#define KEY_FORMAT_LEN                                   6
#define KEY_NAME_STR                                     "name"
#define KEY_NAME_LEN                                     4
#define KEY_VERSION_STR                                  "version"
#define KEY_VERSION_LEN                                  7

#define KEY_BLOCK_DATA                                   "data"
#define KEY_BLOCK_DATA_AUTHOR_NAME                       "author name"
#define KEY_BLOCK_DATA_COMMENT                           "comment"
#define KEY_BLOCK_DATA_COMPANY_NAME                      "company name"
#define KEY_BLOCK_DATA_COPYRIGHT                         "copyright"
#define KEY_BLOCK_DATA_PROJECT_MAX                       "project max"
#define KEY_BLOCK_DATA_PROJECT_MIN                       "project min"
#define KEY_BLOCK_DATA_PROJECT_NAME                      "project name"
#define KEY_BLOCK_DATA_PROJECT_SYSTEM                    "project system"
#define KEY_BLOCK_DATA_SOFTWARE_NAME                     "software name"
#define KEY_BLOCK_DATA_SOFTWARE_VERSION                  "software version"

#define KEY_BLOCK_PROPERTY_LIST                          "property list"
#define KEY_BLOCK_PROPERTY                               "property"
#define KEY_BLOCK_PROPERTY_IS_ARROW_SIZE_RELATIVE        "is arrow size relative"
#define KEY_BLOCK_PROPERTY_IS_CLIPPABLE                  "is clippable"
#define KEY_BLOCK_PROPERTY_IS_FONT_BOLD                  "is font bold"
#define KEY_BLOCK_PROPERTY_IS_FONT_ITALIC                "is font italic"
#define KEY_BLOCK_PROPERTY_IS_FONT_SIZE_RELATIVE         "is font size relative"
#define KEY_BLOCK_PROPERTY_IS_FONT_STRIKE_OUT            "is font strike out"
#define KEY_BLOCK_PROPERTY_IS_FONT_UNDERLINE             "is font underline"
#define KEY_BLOCK_PROPERTY_IS_SELECTABLE                 "is selectable"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE                    "is visible"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_POINT              "is visible point"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE           "is visible polyline"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_ARROW     "is visible polyline arrow"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_FACE      "is visible polyline face"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_LINE      "is visible polyline line"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_POLYLINE_NODE      "is visible polyline node"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_01      "is visible data label 01"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_02      "is visible data label 02"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_03      "is visible data label 03"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_04      "is visible data label 04"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_05      "is visible data label 05"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_06      "is visible data label 06"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_07      "is visible data label 07"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_08      "is visible data label 08"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_09      "is visible data label 09"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_DATA_LABEL_10      "is visible data label 10"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_01      "is visible line label 01"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_02      "is visible line label 02"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_03      "is visible line label 03"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_04      "is visible line label 04"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_05      "is visible line label 05"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_06      "is visible line label 06"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_07      "is visible line label 07"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_08      "is visible line label 08"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_09      "is visible line label 09"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_LINE_LABEL_10      "is visible line label 10"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_01      "is visible node label 01"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_02      "is visible node label 02"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_03      "is visible node label 03"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_04      "is visible node label 04"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_05      "is visible node label 05"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_06      "is visible node label 06"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_07      "is visible node label 07"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_08      "is visible node label 08"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_09      "is visible node label 09"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_10      "is visible node label 10"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_11      "is visible node label 11"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_12      "is visible node label 12"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_13      "is visible node label 13"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_14      "is visible node label 14"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_15      "is visible node label 15"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_16      "is visible node label 16"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_17      "is visible node label 17"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_18      "is visible node label 18"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_19      "is visible node label 19"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_NODE_LABEL_20      "is visible node label 20"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE            "is visible surface"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_FACE       "is visible surface face"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_LINE       "is visible surface line"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_SURFACE_NODE       "is visible surface node"
#define KEY_BLOCK_PROPERTY_IS_VISIBLE_TEXT               "is visible text"
#define KEY_BLOCK_PROPERTY_ARROW_POSITION                "arrow position"
#define KEY_BLOCK_PROPERTY_ARROW_SIZE                    "arrow size"
#define KEY_BLOCK_PROPERTY_COLOR                         "color"
#define KEY_BLOCK_PROPERTY_COLOR_POINT                   "color point"
#define KEY_BLOCK_PROPERTY_COLOR_POLYLINE                "color polyline"
#define KEY_BLOCK_PROPERTY_COLOR_POLYLINE_ARROW          "color polyline arrow"
#define KEY_BLOCK_PROPERTY_COLOR_POLYLINE_FACE           "color polyline face"
#define KEY_BLOCK_PROPERTY_COLOR_POLYLINE_FACE_PATTERN   "color polyline face pattern"
#define KEY_BLOCK_PROPERTY_COLOR_POLYLINE_LINE           "color polyline line"
#define KEY_BLOCK_PROPERTY_COLOR_POLYLINE_NODE           "color polyline node"
#define KEY_BLOCK_PROPERTY_COLOR_SURFACE                 "color surface"
#define KEY_BLOCK_PROPERTY_COLOR_SURFACE_FACE            "color surface face"
#define KEY_BLOCK_PROPERTY_COLOR_SURFACE_FACE_PATTERN    "color surface face pattern"
#define KEY_BLOCK_PROPERTY_COLOR_SURFACE_LINE            "color surface line"
#define KEY_BLOCK_PROPERTY_COLOR_SURFACE_NODE            "color surface node"
#define KEY_BLOCK_PROPERTY_COLOR_TEXT                    "color text"
#define KEY_BLOCK_PROPERTY_FACE_TRANSPARENCY             "face transparency"
#define KEY_BLOCK_PROPERTY_FONT_NAME                     "font name"
#define KEY_BLOCK_PROPERTY_FONT_SIZE                     "font size"
#define KEY_BLOCK_PROPERTY_KEY                           "key"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_01                 "data label 01"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_02                 "data label 02"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_03                 "data label 03"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_04                 "data label 04"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_05                 "data label 05"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_06                 "data label 06"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_07                 "data label 07"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_08                 "data label 08"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_09                 "data label 09"
#define KEY_BLOCK_PROPERTY_DATA_LABEL_10                 "data label 10"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_01                 "line label 01"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_02                 "line label 02"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_03                 "line label 03"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_04                 "line label 04"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_05                 "line label 05"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_06                 "line label 06"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_07                 "line label 07"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_08                 "line label 08"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_09                 "line label 09"
#define KEY_BLOCK_PROPERTY_LINE_LABEL_10                 "line label 10"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_01                 "node label 01"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_02                 "node label 02"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_03                 "node label 03"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_04                 "node label 04"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_05                 "node label 05"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_06                 "node label 06"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_07                 "node label 07"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_08                 "node label 08"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_09                 "node label 09"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_10                 "node label 10"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_11                 "node label 11"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_12                 "node label 12"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_13                 "node label 13"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_14                 "node label 14"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_15                 "node label 15"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_16                 "node label 16"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_17                 "node label 17"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_18                 "node label 18"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_19                 "node label 19"
#define KEY_BLOCK_PROPERTY_NODE_LABEL_20                 "node label 20"
#define KEY_BLOCK_PROPERTY_NAME                          "name"
#define KEY_BLOCK_PROPERTY_PATTERN_ARROW                 "pattern arrow"
#define KEY_BLOCK_PROPERTY_PATTERN_FACE                  "pattern face"
#define KEY_BLOCK_PROPERTY_PATTERN_LINE                  "pattern line"
#define KEY_BLOCK_PROPERTY_PATTERN_NODE                  "pattern node"

#define KEY_BLOCK_IMAGE_LIST                             "image list"
#define KEY_BLOCK_IMAGE                                  "image"
#define KEY_BLOCK_IMAGE_FILE                             "file"
#define KEY_BLOCK_IMAGE_FILE_NAME                        "file name"
#define KEY_BLOCK_IMAGE_KEY                              "key"
#define KEY_BLOCK_IMAGE_NAME                             "name"

#define KEY_BLOCK_ITEM_LIST                              "item list"
#define KEY_BLOCK_ITEM                                   "item"
#define KEY_BLOCK_ITEM_KEY                               "key"
#define KEY_BLOCK_ITEM_NAME                              "name"
#define KEY_BLOCK_ITEM_TYPE                              "type"
#define KEY_BLOCK_ITEM_DEFAULT                           "default"
#define KEY_BLOCK_ITEM_FORMULA                           "formula"
#define KEY_BLOCK_ITEM_MAX                               "max"
#define KEY_BLOCK_ITEM_MIN                               "min"
#define KEY_BLOCK_ITEM_PRECISION                         "precision"
#define KEY_BLOCK_ITEM_BIN_LIST                          "bin list"
#define KEY_BLOCK_ITEM_BIN                               "bin"
#define KEY_BLOCK_ITEM_BIN_VALUE                         "value"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE          "is visible drillhole"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_FACE     "is visible drillhole face"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_LINE     "is visible drillhole line"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_DRILLHOLE_NODE     "is visible drillhole node"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY           "is visible geometry"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_FACE      "is visible geometry face"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_LINE      "is visible geometry line"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_GEOMETRY_NODE      "is visible geometry node"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL              "is visible model"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_FACE         "is visible model face"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_LINE         "is visible model line"
#define KEY_BLOCK_ITEM_BIN_IS_VISIBLE_MODEL_NODE         "is visible model node"
#define KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE               "color drillhole"
#define KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_FACE          "color drillhole face"
#define KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_LINE          "color drillhole line"
#define KEY_BLOCK_ITEM_BIN_COLOR_DRILLHOLE_NODE          "color drillhole node"
#define KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY                "color geometry"
#define KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_FACE           "color geometry face"
#define KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_LINE           "color geometry line"
#define KEY_BLOCK_ITEM_BIN_COLOR_GEOMETRY_NODE           "color geometry node"
#define KEY_BLOCK_ITEM_BIN_COLOR_MODEL                   "color model"
#define KEY_BLOCK_ITEM_BIN_COLOR_MODEL_FACE              "color model face"
#define KEY_BLOCK_ITEM_BIN_COLOR_MODEL_LINE              "color model line"
#define KEY_BLOCK_ITEM_BIN_COLOR_MODEL_NODE              "color model node"

#define KEY_BLOCK_GEOMETRY_LIST                          "geometry list"
#define KEY_BLOCK_GEOMETRY                               "geometry"

#define KEY_BLOCK_DRILL_HOLE_LIST                        "drill hole list"
#define KEY_BLOCK_DRILL_HOLE                             "drill hole"

#define KEY_BLOCK_MODEL_LIST                             "model list"
#define KEY_BLOCK_MODEL                                  "model"

#define READ_BLOCK_SIZE                                  (1 << 20)

/**************************************************************************************************
type:
**************************************************************************************************/

/**************************************************************************************************
variable:
**************************************************************************************************/

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
Gb                 _MiIoClocReader(       GmineInfo       * const mineInfo);
Gb                 _MiIoClocWriter(       GmineInfo       * const mineInfo);

void               _MiIoDloc(             GmineInfo       * const mineInfo);

Gb                 _MiIoWriteB(           GmineInfo       * const gmineInfo, Gstr const * const key, Gb const value);
Gb                 _MiIoWriteBinBuffer(   GmineInfo       * const gmineInfo, Gcount const count, Gn1 const * const buffer);
Gb                 _MiIoWriteBinStart(    GmineInfo       * const gmineInfo, Gstr const * const key, size_t const fileSize);
Gb                 _MiIoWriteBinStop(     GmineInfo       * const gmineInfo);
Gb                 _MiIoWriteBlockStart(  GmineInfo       * const gmineInfo, Gstr       * const key);
Gb                 _MiIoWriteBlockStop(   GmineInfo       * const gmineInfo);
Gb                 _MiIoWriteColor(       GmineInfo       * const gmineInfo, Gstr const * const key, GmineInfoColor const * const value);
Gb                 _MiIoWriteI(           GmineInfo       * const gmineInfo, Gstr const * const key, Gi8 const value);
Gb                 _MiIoWriteN(           GmineInfo       * const gmineInfo, Gstr const * const key, Gn8 const value);
Gb                 _MiIoWritePoint(       GmineInfo       * const gmineInfo, Gstr const * const key, GmineInfoPoint const * const value);
Gb                 _MiIoWriteR(           GmineInfo       * const gmineInfo, Gstr const * const key, Gr8 const value);
Gb                 _MiIoWriteStr(         GmineInfo       * const gmineInfo, Gstr const * const key, Gstr const * const value);

void              *_MiMemCloc(            Gcount const memByteCount);
void               _MiMemDloc(            void * const mem);
void               _MiMemStart(           GmemCloc const memClocFunc, GmemDloc const memDlocFunc);
void               _MiMemStop(            void);

#define            _MiMemClearType(       MEM, TYPE)                      memset((MEM), 0,               sizeof(TYPE))
#define            _MiMemClearTypeArray(  MEM, TYPE, COUNT)               memset((MEM), 0,     (COUNT) * sizeof(TYPE))
#define            _MiMemClocType(               TYPE)           (TYPE *) _MiMemCloc(                    sizeof(TYPE))
#define            _MiMemClocTypeArray(   COUNT, TYPE)           (TYPE *) _MiMemCloc(          (COUNT) * sizeof(TYPE))
#define            _MiMemCopyTypeArray(   MEM, COUNT, TYPE, SRC)          memcpy((MEM), (SRC), (COUNT) * sizeof(TYPE))
#define            _MiMemIsEqual(         COUNT, MEM, COUNT_ALT, MEM_ALT) (memcmp(MEM, MEM_ALT, COUNT) == 0)

#define            _MiStrGetCount(        STR, MAX_LEN)                   (Gcount) (strnlen(STR, (size_t) (MAX_LEN)))
Gb                 _MiStrIsEmpty(         Gstr const * const value);
#define            _MiStrIsEqual(         STR, STR_ALT)                   _MiMemIsEqual(_MiStrGetCount(STR, gmiffCountDEFAULT), STR, _MiStrGetCount(STR_ALT, gmiffCountDEFAULT), STR_ALT)
Gstr              *_MiStrClone(           Gstr const * const value);

#endif
