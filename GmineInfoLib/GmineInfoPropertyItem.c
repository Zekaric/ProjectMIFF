/**************************************************************************************************
file:       GmineInfoPropertyItem
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
func: gmineInfoPropertyItemCloc
**************************************************************************************************/
GmineInfoPropertyItem *gmineInfoPropertyItemCloc(void)
{
   GmineInfoPropertyItem *gmineInfoPropertyItem;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoPropertyItem = _MiMemClocType(GmineInfoPropertyItem);
   returnNullIf(!gmineInfoPropertyItem);

   if (!gmineInfoPropertyItemClocContent(gmineInfoPropertyItem))
   {
      _MiMemDloc(   gmineInfoPropertyItem);
      return NULL;
   }

   return gmineInfoPropertyItem;
}

/**************************************************************************************************
func: gmineInfoPropertyItemClocContent
**************************************************************************************************/
Gb gmineInfoPropertyItemClocContent(GmineInfoPropertyItem * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   _MiMemClearType(gmineInfoPropertyItem, GmineInfoPropertyItem);

   return gmineInfoPropertyClocContent(&gmineInfoPropertyItem->property);
}

/**************************************************************************************************
func: gmineInfoPropertyItemDloc
**************************************************************************************************/
void gmineInfoPropertyItemDloc(GmineInfoPropertyItem * const gmineInfoPropertyItem)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItemDlocContent(   gmineInfoPropertyItem);

   _MiMemDloc(   gmineInfoPropertyItem);
}

/**************************************************************************************************
func: gmineInfoPropertyItemDlocContent
**************************************************************************************************/
void gmineInfoPropertyItemDlocContent(GmineInfoPropertyItem * const gmineInfoPropertyItem)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyDlocContent(&gmineInfoPropertyItem->property);

   // TODO Clean up value str
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorDilllHole
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorDilllHole(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorDrillHole;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorDrillHoleFace
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorDrillHoleFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorDrillHoleLine
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorDrillHoleLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorDrillHoleNode
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorDrillHoleNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorGeometry
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorGeometry(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorGeometry;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorGeometryFace
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorGeometryFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorGeometryFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorGeometryLine
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorGeometryLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorGeometryLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorGeometryNode
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorGeometryNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorGeometryNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorModel
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorModel(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorModel;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorModelFace
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorModelFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorModelFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorModelLine
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorModelLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorModelLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetColorModelNode
**************************************************************************************************/
GmineInfoColor const * gmineInfoPropertyItemGetColorModelNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->colorModelNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetProperty
**************************************************************************************************/
GmineInfoProperty *gmineInfoPropertyItemGetProperty(GmineInfoPropertyItem * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return &gmineInfoPropertyItem->property;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetValueB
**************************************************************************************************/
Gb gmineInfoPropertyItemGetValueB(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->value.b;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetValueI
**************************************************************************************************/
Gi8 gmineInfoPropertyItemGetValueI(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->value.i;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetValueN
**************************************************************************************************/
Gn8 gmineInfoPropertyItemGetValueN(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->value.n;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetValueR
**************************************************************************************************/
Gr8 gmineInfoPropertyItemGetValueR(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->value.r;
}

/**************************************************************************************************
func: gmineInfoPropertyItemGetValueStr
**************************************************************************************************/
Gstr const * gmineInfoPropertyItemGetValueStr(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->value.str;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleDrillHole
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleDrillHole(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleDrillHole;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleDrillHoleFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleDrillHoleFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleDrillHoleLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleDrillHoleLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleDrillHoleNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleDrillHoleNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleGeometry
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleGeometry(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleGeometry;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleGeometryFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleGeometryFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleGeometryFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleGeometryLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleGeometryLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleGeometryLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleGeometryNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleGeometryNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleGeometryNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleModel
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleModel(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleModel;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleModelFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleModelFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleModelFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleModelLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleModelLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleModelLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetIsVisibleModelNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetIsVisibleModelNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetIsVisibleModelNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorDrillHole
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorDrillHole(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorDrillHole;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorDrillHoleFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorDrillHoleFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorDrillHoleLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorDrillHoleLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorDrillHoleNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorDrillHoleNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorGeometry
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorGeometry(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorGeometry;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorGeometryFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorGeometryFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorGeometryFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorGeometryLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorGeometryLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorGeometryLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorGeometryNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorGeometryNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorGeometryNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorModelFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorModelFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorModelFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorModelLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorModelLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorModelLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsSetColorModelNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsSetColorModelNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isSetColorModelNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleDrillHole
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleDrillHole(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleDrillHole;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleDrillHoleFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleDrillHoleFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleDrillHoleFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleDrillHoleLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleDrillHoleLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleDrillHoleLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleDrillHoleNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleDrillHoleNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleDrillHoleNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleGeometry
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleGeometry(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleGeometry;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleGeometryFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleGeometryFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleGeometryFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleGeometryLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleGeometryLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleGeometryLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleGeometryNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleGeometryNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleGeometryNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleModelFace
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleModelFace(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleModelFace;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleModelLine
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleModelLine(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleModelLine;
}

/**************************************************************************************************
func: gmineInfoPropertyItemIsVisibleModelNode
**************************************************************************************************/
Gb gmineInfoPropertyItemIsVisibleModelNode(GmineInfoPropertyItem const * const gmineInfoPropertyItem)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   return gmineInfoPropertyItem->isVisibleModelNode;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorDrillHole
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorDrillHole(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorDrillHole      = *value;
   gmineInfoPropertyItem->isSetColorDrillHole = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorDrillHoleFace
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorDrillHoleFace(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorDrillHoleFace      = *value;
   gmineInfoPropertyItem->isSetColorDrillHoleFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorDrillHoleLine
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorDrillHoleLine(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorDrillHoleLine      = *value;
   gmineInfoPropertyItem->isSetColorDrillHoleLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorDrillHoleNode
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorDrillHoleNode(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorDrillHoleNode      = *value;
   gmineInfoPropertyItem->isSetColorDrillHoleNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorGeometry
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorGeometry(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorGeometry      = *value;
   gmineInfoPropertyItem->isSetColorGeometry = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorGeometryFace
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorGeometryFace(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorGeometryFace      = *value;
   gmineInfoPropertyItem->isSetColorGeometryFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorGeometryLine
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorGeometryLine(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorGeometryLine      = *value;
   gmineInfoPropertyItem->isSetColorGeometryLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorGeometryNode
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorGeometryNode(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorGeometryNode      = *value;
   gmineInfoPropertyItem->isSetColorGeometryNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorModel
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorModel(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorModel      = *value;
   gmineInfoPropertyItem->isSetColorModel = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorModelFace
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorModelFace(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorModelFace      = *value;
   gmineInfoPropertyItem->isSetColorModelFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorModelLine
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorModelLine(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorModelLine      = *value;
   gmineInfoPropertyItem->isSetColorModelLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetColorModelNode
**************************************************************************************************/
Gb gmineInfoPropertyItemSetColorModelNode(GmineInfoPropertyItem * const gmineInfoPropertyItem, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->colorModelNode      = *value;
   gmineInfoPropertyItem->isSetColorModelNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleDrillHole
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleDrillHole(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleDrillHole      = value;
   gmineInfoPropertyItem->isSetIsVisibleDrillHole = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleDrillHoleFace
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleDrillHoleFace(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleDrillHoleFace      = value;
   gmineInfoPropertyItem->isSetIsVisibleDrillHoleFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleDrillHoleLine
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleDrillHoleLine(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleDrillHoleLine      = value;
   gmineInfoPropertyItem->isSetIsVisibleDrillHoleLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleDrillHoleNode
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleDrillHoleNode(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleDrillHoleNode      = value;
   gmineInfoPropertyItem->isSetIsVisibleDrillHoleNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleGeometry
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleGeometry(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleGeometry      = value;
   gmineInfoPropertyItem->isSetIsVisibleGeometry = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleGeometryFace
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleGeometryFace(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleGeometryFace      = value;
   gmineInfoPropertyItem->isSetIsVisibleGeometryFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleGeometryLine
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleGeometryLine(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleGeometryLine      = value;
   gmineInfoPropertyItem->isSetIsVisibleGeometryLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleGeometryNode
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleGeometryNode(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleGeometryNode      = value;
   gmineInfoPropertyItem->isSetIsVisibleGeometryNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleModel
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleModel(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleModel      = value;
   gmineInfoPropertyItem->isSetIsVisibleModel = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleModelFace
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleModelFace(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleModelFace      = value;
   gmineInfoPropertyItem->isSetIsVisibleModelFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleModelLine
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleModelLine(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleModelLine      = value;
   gmineInfoPropertyItem->isSetIsVisibleModelLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetIsVisibleModelNode
**************************************************************************************************/
Gb gmineInfoPropertyItemSetIsVisibleModelNode(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->isVisibleModelNode      = value;
   gmineInfoPropertyItem->isSetIsVisibleModelNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetValueB
**************************************************************************************************/
Gb gmineInfoPropertyItemSetValueB(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->value.b    = value;
   gmineInfoPropertyItem->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetValueI
**************************************************************************************************/
Gb gmineInfoPropertyItemSetValueI(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gi8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->value.i    = value;
   gmineInfoPropertyItem->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetValueN
**************************************************************************************************/
Gb gmineInfoPropertyItemSetValueN(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gn8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->value.n    = value;
   gmineInfoPropertyItem->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetValueR
**************************************************************************************************/
Gb gmineInfoPropertyItemSetValueR(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gr8 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->value.r    = value;
   gmineInfoPropertyItem->isSetValue = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertyItemSetValueStr
**************************************************************************************************/
Gb gmineInfoPropertyItemSetValueStr(GmineInfoPropertyItem * const gmineInfoPropertyItem, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoPropertyItem);

   gmineInfoPropertyItem->value.str  = _MiStrClone(value);
   gmineInfoPropertyItem->isSetValue = gbTRUE;

   returnTrue;
}
