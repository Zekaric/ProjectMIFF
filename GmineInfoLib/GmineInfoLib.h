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
GmineInfo   *gmineInfoClocReader(                                                                        GgetBuffer getBufferFunc, void * const dataRepo);
Gb           gmineInfoClocReaderContent(  GmineInfo * const gmineInfo,                                   GgetBuffer getBufferFunc, void * const dataRepo);
GmineInfo   *gmineInfoClocWriter(                                      GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);
Gb           gmineInfoClocWriterContent(  GmineInfo * const gmineInfo, GmineInfoFileType const fileType, GsetBuffer setBufferFunc, void * const dataRepo);

void         gmineInfoDloc(               GmineInfo * const gmineInfo);
void         gmineInfoDlocContent(        GmineInfo * const gmineInfo);

Gb           gmineInfoStart(              GmemCloc memClocFunc, GmemDloc memDlocFunc);
void         gmineInfoStop(               void);

#endif