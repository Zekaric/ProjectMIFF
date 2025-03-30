/**************************************************************************************************
file:       GmineInfoItemBin
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Property Item routines
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

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gmineInfoItemBinCloc
**************************************************************************************************/
GmineInfoItemBin *gmineInfoItemBinCloc(void)
{
   GmineInfoItemBin *gmineInfoItemBin;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoItemBin = _MiMemClocType(GmineInfoItemBin);
   returnNullIf(!gmineInfoItemBin);

   if (!gmineInfoItemBinClocContent(gmineInfoItemBin))
   {
      _MiMemDloc(   gmineInfoItemBin);
      return NULL;
   }

   return gmineInfoItemBin;
}

/**************************************************************************************************
func: gmineInfoItemBinClocContent
**************************************************************************************************/
Gb gmineInfoItemBinClocContent(GmineInfoItemBin * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   _MiMemClearType(gmineInfoItemBin, GmineInfoItemBin);

   return gmineInfoPropertyClocContent(&gmineInfoItemBin->property);
}

/**************************************************************************************************
func: gmineInfoItemBinDloc
**************************************************************************************************/
void gmineInfoItemBinDloc(GmineInfoItemBin * const gmineInfoItemBin)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBinDlocContent(gmineInfoItemBin);

   _MiMemDloc(gmineInfoItemBin);
}

/**************************************************************************************************
func: gmineInfoItemBinDlocContent
**************************************************************************************************/
void gmineInfoItemBinDlocContent(GmineInfoItemBin * const gmineInfoItemBin)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoPropertyDlocContent(&gmineInfoItemBin->property);

   // TODO Clean up value str
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorDilllHole
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorDilllHole(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorDrillHole;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorDrillHoleFace
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorDrillHoleFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorDrillHoleLine
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorDrillHoleLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorDrillHoleNode
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorDrillHoleNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorGeometry
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorGeometry(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorGeometry;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorGeometryFace
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorGeometryFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorGeometryFace;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorGeometryLine
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorGeometryLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorGeometryLine;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorGeometryNode
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorGeometryNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorGeometryNode;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorModel
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorModel(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorModel;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorModelFace
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorModelFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorModelFace;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorModelLine
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorModelLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorModelLine;
}

/**************************************************************************************************
func: gmineInfoItemBinGetColorModelNode
**************************************************************************************************/
GmineInfoColor const * gmineInfoItemBinGetColorModelNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->colorModelNode;
}

/**************************************************************************************************
func: gmineInfoItemBinGetProperty
**************************************************************************************************/
GmineInfoProperty *gmineInfoItemBinGetProperty(GmineInfoItemBin * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return &gmineInfoItemBin->property;
}

/**************************************************************************************************
func: gmineInfoItemBinGetValueB
**************************************************************************************************/
Gb gmineInfoItemBinGetValueB(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->value.b;
}

/**************************************************************************************************
func: gmineInfoItemBinGetValueI
**************************************************************************************************/
Gi8 gmineInfoItemBinGetValueI(GmineInfoItemBin const * const gmineInfoItemBin)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->value.i;
}

/**************************************************************************************************
func: gmineInfoItemBinGetValueN
**************************************************************************************************/
Gn8 gmineInfoItemBinGetValueN(GmineInfoItemBin const * const gmineInfoItemBin)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->value.n;
}

/**************************************************************************************************
func: gmineInfoItemBinGetValueR
**************************************************************************************************/
Gr8 gmineInfoItemBinGetValueR(GmineInfoItemBin const * const gmineInfoItemBin)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->value.r;
}

/**************************************************************************************************
func: gmineInfoItemBinGetValueStr
**************************************************************************************************/
Gstr const * gmineInfoItemBinGetValueStr(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->value.str;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleDrillHole
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleDrillHole(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleDrillHole;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleDrillHoleFace
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleDrillHoleFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleDrillHoleLine
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleDrillHoleLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleDrillHoleNode
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleDrillHoleNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleGeometry
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleGeometry(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleGeometry;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleGeometryFace
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleGeometryFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleGeometryFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleGeometryLine
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleGeometryLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleGeometryLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleGeometryNode
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleGeometryNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleGeometryNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleModel
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleModel(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleModel;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleModelFace
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleModelFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleModelFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleModelLine
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleModelLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleModelLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetIsVisibleModelNode
**************************************************************************************************/
Gb gmineInfoItemBinIsSetIsVisibleModelNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetIsVisibleModelNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorDrillHole
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorDrillHole(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorDrillHole;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorDrillHoleFace
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorDrillHoleFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorDrillHoleLine
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorDrillHoleLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorDrillHoleNode
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorDrillHoleNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorGeometry
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorGeometry(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorGeometry;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorGeometryFace
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorGeometryFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorGeometryFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorGeometryLine
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorGeometryLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorGeometryLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorGeometryNode
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorGeometryNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorGeometryNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorModelFace
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorModelFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorModelFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorModelLine
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorModelLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorModelLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsSetColorModelNode
**************************************************************************************************/
Gb gmineInfoItemBinIsSetColorModelNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isSetColorModelNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleDrillHole
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleDrillHole(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleDrillHole;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleDrillHoleFace
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleDrillHoleFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleDrillHoleLine
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleDrillHoleLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleDrillHoleNode
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleDrillHoleNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleGeometry
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleGeometry(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleGeometry;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleGeometryFace
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleGeometryFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleGeometryFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleGeometryLine
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleGeometryLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleGeometryLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleGeometryNode
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleGeometryNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleGeometryNode;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleModelFace
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleModelFace(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleModelFace;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleModelLine
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleModelLine(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleModelLine;
}

/**************************************************************************************************
func: gmineInfoItemBinIsVisibleModelNode
**************************************************************************************************/
Gb gmineInfoItemBinIsVisibleModelNode(GmineInfoItemBin const * const gmineInfoItemBin)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   return gmineInfoItemBin->isVisibleModelNode;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorDrillHole
**************************************************************************************************/
Gb gmineInfoItemBinSetColorDrillHole(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorDrillHole      = *value;
   gmineInfoItemBin->isSetColorDrillHole = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorDrillHoleFace
**************************************************************************************************/
Gb gmineInfoItemBinSetColorDrillHoleFace(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorDrillHoleFace      = *value;
   gmineInfoItemBin->isSetColorDrillHoleFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorDrillHoleLine
**************************************************************************************************/
Gb gmineInfoItemBinSetColorDrillHoleLine(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorDrillHoleLine      = *value;
   gmineInfoItemBin->isSetColorDrillHoleLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorDrillHoleNode
**************************************************************************************************/
Gb gmineInfoItemBinSetColorDrillHoleNode(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorDrillHoleNode      = *value;
   gmineInfoItemBin->isSetColorDrillHoleNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorGeometry
**************************************************************************************************/
Gb gmineInfoItemBinSetColorGeometry(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorGeometry      = *value;
   gmineInfoItemBin->isSetColorGeometry = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorGeometryFace
**************************************************************************************************/
Gb gmineInfoItemBinSetColorGeometryFace(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorGeometryFace      = *value;
   gmineInfoItemBin->isSetColorGeometryFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorGeometryLine
**************************************************************************************************/
Gb gmineInfoItemBinSetColorGeometryLine(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorGeometryLine      = *value;
   gmineInfoItemBin->isSetColorGeometryLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorGeometryNode
**************************************************************************************************/
Gb gmineInfoItemBinSetColorGeometryNode(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorGeometryNode      = *value;
   gmineInfoItemBin->isSetColorGeometryNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorModel
**************************************************************************************************/
Gb gmineInfoItemBinSetColorModel(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorModel      = *value;
   gmineInfoItemBin->isSetColorModel = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorModelFace
**************************************************************************************************/
Gb gmineInfoItemBinSetColorModelFace(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorModelFace      = *value;
   gmineInfoItemBin->isSetColorModelFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorModelLine
**************************************************************************************************/
Gb gmineInfoItemBinSetColorModelLine(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorModelLine      = *value;
   gmineInfoItemBin->isSetColorModelLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetColorModelNode
**************************************************************************************************/
Gb gmineInfoItemBinSetColorModelNode(GmineInfoItemBin * const gmineInfoItemBin, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->colorModelNode      = *value;
   gmineInfoItemBin->isSetColorModelNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleDrillHole
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleDrillHole(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleDrillHole      = value;
   gmineInfoItemBin->isSetIsVisibleDrillHole = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleDrillHoleFace
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleDrillHoleFace(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleDrillHoleFace      = value;
   gmineInfoItemBin->isSetIsVisibleDrillHoleFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleDrillHoleLine
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleDrillHoleLine(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleDrillHoleLine      = value;
   gmineInfoItemBin->isSetIsVisibleDrillHoleLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleDrillHoleNode
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleDrillHoleNode(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleDrillHoleNode      = value;
   gmineInfoItemBin->isSetIsVisibleDrillHoleNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleGeometry
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleGeometry(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleGeometry      = value;
   gmineInfoItemBin->isSetIsVisibleGeometry = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleGeometryFace
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleGeometryFace(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleGeometryFace      = value;
   gmineInfoItemBin->isSetIsVisibleGeometryFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleGeometryLine
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleGeometryLine(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleGeometryLine      = value;
   gmineInfoItemBin->isSetIsVisibleGeometryLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleGeometryNode
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleGeometryNode(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleGeometryNode      = value;
   gmineInfoItemBin->isSetIsVisibleGeometryNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleModel
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleModel(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleModel      = value;
   gmineInfoItemBin->isSetIsVisibleModel = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleModelFace
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleModelFace(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleModelFace      = value;
   gmineInfoItemBin->isSetIsVisibleModelFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleModelLine
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleModelLine(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleModelLine      = value;
   gmineInfoItemBin->isSetIsVisibleModelLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetIsVisibleModelNode
**************************************************************************************************/
Gb gmineInfoItemBinSetIsVisibleModelNode(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->isVisibleModelNode      = value;
   gmineInfoItemBin->isSetIsVisibleModelNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetValueB
**************************************************************************************************/
Gb gmineInfoItemBinSetValueB(GmineInfoItemBin * const gmineInfoItemBin, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->value.b    = value;
   gmineInfoItemBin->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetValueI
**************************************************************************************************/
Gb gmineInfoItemBinSetValueI(GmineInfoItemBin * const gmineInfoItemBin, Gi8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->value.i    = value;
   gmineInfoItemBin->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetValueN
**************************************************************************************************/
Gb gmineInfoItemBinSetValueN(GmineInfoItemBin * const gmineInfoItemBin, Gn8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->value.n    = value;
   gmineInfoItemBin->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetValueR
**************************************************************************************************/
Gb gmineInfoItemBinSetValueR(GmineInfoItemBin * const gmineInfoItemBin, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->value.r    = value;
   gmineInfoItemBin->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoItemBinSetValueStr
**************************************************************************************************/
Gb gmineInfoItemBinSetValueStr(GmineInfoItemBin * const gmineInfoItemBin, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoItemBin);

   gmineInfoItemBin->value.str  = _MiStrClone(value);
   gmineInfoItemBin->isSetValue = gbTRUE;

   returnTrue;
}
