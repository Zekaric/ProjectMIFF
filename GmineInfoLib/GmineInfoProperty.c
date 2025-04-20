/**************************************************************************************************
file:       GmineInfoProperty
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:

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
func: gmineInfoPropertyCloc
**************************************************************************************************/
GmineInfoProperty *gmineInfoPropertyCloc(void)
{
   GmineInfoProperty *gmineInfoProperty;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoProperty = _MiMemClocType(GmineInfoProperty);
   returnNullIf(!gmineInfoProperty);

   if (!gmineInfoPropertyClocContent(gmineInfoProperty))
   {
      _MiMemDloc(gmineInfoProperty);
      return NULL;
   }

   return gmineInfoProperty;
}

/**************************************************************************************************
func: gmineInfoPropertyClocContent
**************************************************************************************************/
Gb gmineInfoPropertyClocContent(GmineInfoProperty * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   _MiMemClearType(gmineInfoProperty, GmineInfoProperty);

   return gmineInfoBlockKeyValueArrayCloc(gmineInfoProperty);
}

/**************************************************************************************************
func: gmineInfoPropertyDloc
**************************************************************************************************/
void gmineInfoPropertyDloc(GmineInfoProperty * const gmineInfoProperty)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoBlockKeyValueArrayDloc(gmineInfoProperty);

   _MiMemDloc(gmineInfoProperty);
}

/**************************************************************************************************
func: gmineInfoPropertyDlocContent
**************************************************************************************************/
void gmineInfoPropertyDlocContent(GmineInfoProperty * const gmineInfoProperty)
{
   Gindex index;

   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   _MiMemDloc(gmineInfoProperty->fontName);
   forCount(index, 10)
   {
      _MiMemDloc(gmineInfoProperty->labelData[index]);
      _MiMemDloc(gmineInfoProperty->labelLine[index]);
   }
   forCount(index, 20)
   {
      _MiMemDloc(gmineInfoProperty->labelNode[index]);
   }
   _MiMemDloc(gmineInfoProperty->patternArrow);
   _MiMemDloc(gmineInfoProperty->patternFace);
   _MiMemDloc(gmineInfoProperty->patternLine);
   _MiMemDloc(gmineInfoProperty->patternNode);

   _MiMemClearType(gmineInfoProperty, GmineInfoProperty);
}

/**************************************************************************************************
func: gmineInfoPropertyGetArrowPosition

-2  = Arrows at both ends of the polyline.
-1  = Arrow at end of the polyline.
0   = Arrow at the start of the polyline.
>0  = Arrows at every n'th node of the polyline.
**************************************************************************************************/
Gi4 gmineInfoPropertyGetArrowPosition(GmineInfoProperty const * const gmineInfoProperty)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->arrowPosition;
}

/**************************************************************************************************
func: gmineInfoPropertyGetArrowSize
**************************************************************************************************/
Gr4 gmineInfoPropertyGetArrowSize(GmineInfoProperty const * const gmineInfoProperty)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->arrowSize;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColor
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColor(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->color;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPoint
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPoint(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPoint;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPolyline
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPolyline(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPolyline;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPolylineArrow
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPolylineArrow(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPolylineArrow;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPolylineFace
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPolylineFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPolylineFace;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPolylineFacePattern
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPolylineFacePattern(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPolylineFacePattern;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPolylineLine
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPolylineLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPolylineLine;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorPolylineNode
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorPolylineNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorPolylineNode;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorSurface
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorSurface(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorSurface;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorSurfaceFace
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorSurfaceFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorSurfaceFace;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorSurfaceFacePattern
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorSurfaceFacePattern(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorSurfaceFacePattern;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorSurfaceLine
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorSurfaceLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorSurfaceLine;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorSurfaceNode
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorSurfaceNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorSurfaceNode;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorText
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorText(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorText;
}

/**************************************************************************************************
func: gmineInfoPropertyGetFontName
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetFontName(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->fontName;
}

/**************************************************************************************************
func: gmineInfoPropertyGetFontSize
**************************************************************************************************/
Gr4 gmineInfoPropertyGetFontSize(GmineInfoProperty const * const gmineInfoProperty)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->fontSize;
}

/**************************************************************************************************
func: gmineInfoPropertyGetKey
**************************************************************************************************/
Gstr const * gmineInfoPropertyGetKey(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->key;
}

/**************************************************************************************************
func: gmineInfoPropertyGetLabelData
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetLabelData(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->labelData[index];
}

/**************************************************************************************************
func: gmineInfoPropertyGetLabelLine
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetLabelLine(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->labelLine[index];
}

/**************************************************************************************************
func: gmineInfoPropertyGetLabelNode
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetLabelNode(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   return gmineInfoProperty->labelNode[index];
}

/**************************************************************************************************
func: gmineInfoPropertyGetName
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetName(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->name;
}

/**************************************************************************************************
func: gmineInfoPropertyGetPatternArrow
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetPatternArrow(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->patternArrow;
}

/**************************************************************************************************
func: gmineInfoPropertyGetPatternFace
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetPatternFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->patternFace;
}

/**************************************************************************************************
func: gmineInfoPropertyGetPatternLine
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetPatternLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->patternLine;
}

/**************************************************************************************************
func: gmineInfoPropertyGetPatternNode
**************************************************************************************************/
Gstr const *gmineInfoPropertyGetPatternNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->patternNode;
}

/**************************************************************************************************
func: gmineInfoPropertyGetTransparency
**************************************************************************************************/
Gr4 gmineInfoPropertyGetTransparency(GmineInfoProperty const * const gmineInfoProperty)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->faceTransparency;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsArrowSizeRelative
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsArrowSizeRelative(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsArrowSizeRelative;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsClippable
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsClippable(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsClippable;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsFontBold
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsFontBold(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsFontBold;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsFontItalic
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsFontItalic(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsFontItalic;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsFontSizeRelative
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsFontSizeRelative(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsFontSizeRelative;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsFontStrikeOut
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsFontStrikeOut(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsFontStrikeOut;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsFontUnderline
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsFontUnderline(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsFontUnderline;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsSelectable
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsSelectable(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsSelectable;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisible
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisible(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisible;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleLabelData
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleLabelData(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isSetIsVisibleLabelData[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleLabelLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleLabelLine(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isSetIsVisibleLabelLine[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleLabelNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleLabelNode(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   return gmineInfoProperty->isSetIsVisibleLabelNode[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisiblePoint
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisiblePoint(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisiblePoint;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisiblePolyline
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisiblePolyline(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisiblePolyline;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisiblePolylineArrow
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisiblePolylineArrow(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisiblePolylineArrow;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisiblePolylineFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisiblePolylineFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisiblePolylineFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisiblePolylineLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisiblePolylineLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisiblePolylineLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisiblePolylineNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisiblePolylineNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisiblePolylineNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleSurfaceFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleSurfaceFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleSurfaceFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleSurfaceLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleSurfaceLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleSurfaceLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleSurfaceNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleSurfaceNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleSurfaceNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleText
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleText(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleText;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetArrowPosition
**************************************************************************************************/
Gb gmineInfoPropertyIsSetArrowPosition(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetArrowPosition;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetArrowSize
**************************************************************************************************/
Gb gmineInfoPropertyIsSetArrowSize(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetArrowSize;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColor
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColor(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColor;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorPoint
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorPoint(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorPoint;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorPolyline
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorPolyline(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorPolyline;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorPolylineFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorPolylineFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorPolylineFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorPolylineFacePattern
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorPolylineFacePattern(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorPolylineFacePattern;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorPolylineLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorPolylineLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorPolylineLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorPolylineNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorPolylineNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorPolylineNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorSurface
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorSurface(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorSurface;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorSurfaceFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorSurfaceFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorSurfaceFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorSurfaceFacePattern
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorSurfaceFacePattern(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorSurfaceFacePattern;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorSurfaceLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorSurfaceLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorSurfaceLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorSurfaceNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorSurfaceNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorSurfaceNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorText
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorText(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorText;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetFontName
**************************************************************************************************/
Gb gmineInfoPropertyIsSetFontName(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetFontName;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetFontSize
**************************************************************************************************/
Gb gmineInfoPropertyIsSetFontSize(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetFontSize;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetKey
**************************************************************************************************/
Gb gmineInfoPropertyIsSetKey(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetKey;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetLabelData
**************************************************************************************************/
Gb gmineInfoPropertyIsSetLabelData(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isSetLabelData[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetLabelLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetLabelLine(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isSetLabelLine[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetLabelNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetLabelNode(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   return gmineInfoProperty->isSetLabelNode[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetName
**************************************************************************************************/
Gb gmineInfoPropertyIsSetName(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetName;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetPatternArrow
**************************************************************************************************/
Gb gmineInfoPropertyIsSetPatternArrow(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetPatternArrow;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetPatternFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetPatternFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetPatternFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetPatternLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetPatternLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetPatternLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetPatternNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetPatternNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetPatternNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetTransparency
**************************************************************************************************/
Gb gmineInfoPropertyIsSetTransparency(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetFaceTransparency;
}

/**************************************************************************************************
func: gmineInfoPropertyIsArrowSizeRelative
**************************************************************************************************/
Gb gmineInfoPropertyIsArrowSizeRelative(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isArrowSizeRelative;
}

/**************************************************************************************************
func: gmineInfoPropertyIsClippable
**************************************************************************************************/
Gb gmineInfoPropertyIsClippable(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isClippable;
}

/**************************************************************************************************
func: gmineInfoPropertyIsFontBold
**************************************************************************************************/
Gb gmineInfoPropertyIsFontBold(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isFontBold;
}

/**************************************************************************************************
func: gmineInfoPropertyIsFontItalic
**************************************************************************************************/
Gb gmineInfoPropertyIsFontItalic(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isFontItalic;
}

/**************************************************************************************************
func: gmineInfoPropertyIsFontSizeRelative
**************************************************************************************************/
Gb gmineInfoPropertyIsFontSizeRelative(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isFontSizeRelative;
}

/**************************************************************************************************
func: gmineInfoPropertyIsFontStrikeOut
**************************************************************************************************/
Gb gmineInfoPropertyIsFontStrikeOut(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isFontStrikeOut;
}

/**************************************************************************************************
func: gmineInfoPropertyIsFontUnderline
**************************************************************************************************/
Gb gmineInfoPropertyIsFontUnderline(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isFontUnderline;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSelectable
**************************************************************************************************/
Gb gmineInfoPropertyIsSelectable(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSelectable;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisible
**************************************************************************************************/
Gb gmineInfoPropertyIsVisible(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisible;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleLabelData
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleLabelData(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isVisibleLabelData[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleLabelLine
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleLabelLine(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isVisibleLabelLine[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleLabelNode
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleLabelNode(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   return gmineInfoProperty->isVisibleLabelNode[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisiblePoint
**************************************************************************************************/
Gb gmineInfoPropertyIsVisiblePoint(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisiblePoint;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisiblePolyline
**************************************************************************************************/
Gb gmineInfoPropertyIsVisiblePolyline(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisiblePolyline;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisiblePolylineArrow
**************************************************************************************************/
Gb gmineInfoPropertyIsVisiblePolylineArrow(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisiblePolylineArrow;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisiblePolylineFace
**************************************************************************************************/
Gb gmineInfoPropertyIsVisiblePolylineFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisiblePolylineFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisiblePolylineLine
**************************************************************************************************/
Gb gmineInfoPropertyIsVisiblePolylineLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisiblePolylineLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisiblePolylineNode
**************************************************************************************************/
Gb gmineInfoPropertyIsVisiblePolylineNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisiblePolylineNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleSurface
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleSurface(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleSurface;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleSurfaceFace
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleSurfaceFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleSurfaceFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleSurfaceLine
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleSurfaceLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleSurfaceLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleSurfaceNode
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleSurfaceNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleSurfaceNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleText
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleText(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleText;
}

/**************************************************************************************************
func: gmineInfoPropertySetArrowPosition
**************************************************************************************************/
Gb gmineInfoPropertySetArrowPosition(GmineInfoProperty * const gmineInfoProperty, Gi4 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->arrowPosition      = value;
   gmineInfoProperty->isSetArrowPosition = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetArrowSize
**************************************************************************************************/
Gb gmineInfoPropertySetArrowSize(GmineInfoProperty * const gmineInfoProperty, Gr4 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->arrowSize      = value;
   gmineInfoProperty->isSetArrowSize = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColor
**************************************************************************************************/
Gb gmineInfoPropertySetColor(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->color      = *value;
   gmineInfoProperty->isSetColor = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPoint
**************************************************************************************************/
Gb gmineInfoPropertySetColorPoint(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPoint      = *value;
   gmineInfoProperty->isSetColorPoint = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPolyline
**************************************************************************************************/
Gb gmineInfoPropertySetColorPolyline(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPolyline      = *value;
   gmineInfoProperty->isSetColorPolyline = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPolylineArrow
**************************************************************************************************/
Gb gmineInfoPropertySetColorPolylineArrow(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPolylineArrow      = *value;
   gmineInfoProperty->isSetColorPolylineArrow = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPolylineFace
**************************************************************************************************/
Gb gmineInfoPropertySetColorPolylineFace(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPolylineFace      = *value;
   gmineInfoProperty->isSetColorPolylineFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPolylineFacePattern
**************************************************************************************************/
Gb gmineInfoPropertySetColorPolylineFacePattern(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPolylineFacePattern      = *value;
   gmineInfoProperty->isSetColorPolylineFacePattern = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPolylineLine
**************************************************************************************************/
Gb gmineInfoPropertySetColorPolylineLine(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPolylineLine      = *value;
   gmineInfoProperty->isSetColorPolylineLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorPolylineNode
**************************************************************************************************/
Gb gmineInfoPropertySetColorPolylineNode(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorPolylineNode      = *value;
   gmineInfoProperty->isSetColorPolylineNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorSurface
**************************************************************************************************/
Gb gmineInfoPropertySetColorSurface(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorSurface      = *value;
   gmineInfoProperty->isSetColorSurface = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorSurfaceFace
**************************************************************************************************/
Gb gmineInfoPropertySetColorSurfaceFace(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorSurfaceFace      = *value;
   gmineInfoProperty->isSetColorSurfaceFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorSurfaceFacePattern
**************************************************************************************************/
Gb gmineInfoPropertySetColorSurfaceFacePattern(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorSurfaceFacePattern      = *value;
   gmineInfoProperty->isSetColorSurfaceFacePattern = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorSurfaceLine
**************************************************************************************************/
Gb gmineInfoPropertySetColorSurfaceLine(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorSurfaceLine      = *value;
   gmineInfoProperty->isSetColorSurfaceLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorSurfaceNode
**************************************************************************************************/
Gb gmineInfoPropertySetColorSurfaceNode(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorSurfaceNode      = *value;
   gmineInfoProperty->isSetColorSurfaceNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorText
**************************************************************************************************/
Gb gmineInfoPropertySetColorText(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorText      = *value;
   gmineInfoProperty->isSetColorText = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsArrowSizeRelative
**************************************************************************************************/
Gb gmineInfoPropertySetIsArrowSizeRelative(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isArrowSizeRelative      = value;
   gmineInfoProperty->isSetIsArrowSizeRelative = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsClippable
**************************************************************************************************/
Gb gmineInfoPropertySetIsClippable(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isClippable      = value;
   gmineInfoProperty->isSetIsClippable = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsFontBold
**************************************************************************************************/
Gb gmineInfoPropertySetIsFontBold(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isFontBold      = value;
   gmineInfoProperty->isSetIsFontBold = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsFontItalic
**************************************************************************************************/
Gb gmineInfoPropertySetIsFontItalic(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isFontItalic      = value;
   gmineInfoProperty->isSetIsFontItalic = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsFontSizeRelative
**************************************************************************************************/
Gb gmineInfoPropertySetIsFontSizeRelative(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isFontSizeRelative      = value;
   gmineInfoProperty->isSetIsFontSizeRelative = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsFontStrikeOut
**************************************************************************************************/
Gb gmineInfoPropertySetIsFontStrikeOut(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isFontStrikeOut      = value;
   gmineInfoProperty->isSetIsFontStrikeOut = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsFontUnderline
**************************************************************************************************/
Gb gmineInfoPropertySetIsFontUnderline(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isFontUnderline      = value;
   gmineInfoProperty->isSetIsFontUnderline = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsSelectable
**************************************************************************************************/
Gb gmineInfoPropertySetIsSelectable(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isSelectable      = value;
   gmineInfoProperty->isSetIsSelectable = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisible
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisible(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisible      = value;
   gmineInfoProperty->isSetIsVisible = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleLabelData
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleLabelData(GmineInfoProperty * const gmineInfoProperty,
   Gindex const index, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   gmineInfoProperty->isVisibleLabelData[index]      = value;
   gmineInfoProperty->isSetIsVisibleLabelData[index] = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleLabelLine
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleLabelLine(GmineInfoProperty * const gmineInfoProperty,
   Gindex const index, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   gmineInfoProperty->isVisibleLabelLine[index]      = value;
   gmineInfoProperty->isSetIsVisibleLabelLine[index] = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleLabelNode
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleLabelNode(GmineInfoProperty * const gmineInfoProperty,
   Gindex const index, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   gmineInfoProperty->isVisibleLabelNode[index]      = value;
   gmineInfoProperty->isSetIsVisibleLabelNode[index] = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisiblePoint
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisiblePoint(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisiblePoint      = value;
   gmineInfoProperty->isSetIsVisiblePoint = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisiblePolyline
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisiblePolyline(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisiblePolyline      = value;
   gmineInfoProperty->isSetIsVisiblePolyline = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisiblePolylineArrow
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisiblePolylineArrow(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisiblePolylineArrow      = value;
   gmineInfoProperty->isSetIsVisiblePolylineArrow = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisiblePolylineFace
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisiblePolylineFace(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisiblePolylineFace      = value;
   gmineInfoProperty->isSetIsVisiblePolylineFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisiblePolylineLine
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisiblePolylineLine(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisiblePolylineLine      = value;
   gmineInfoProperty->isSetIsVisiblePolylineLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisiblePolylineNode
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisiblePolylineNode(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisiblePolylineNode      = value;
   gmineInfoProperty->isSetIsVisiblePolylineNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleSurface
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleSurface(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleSurface      = value;
   gmineInfoProperty->isSetIsVisibleSurface = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleSurfaceFace
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleSurfaceFace(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleSurfaceFace      = value;
   gmineInfoProperty->isSetIsVisibleSurfaceFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleSurfaceLine
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleSurfaceLine(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleSurfaceLine      = value;
   gmineInfoProperty->isSetIsVisibleSurfaceLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleSurfaceNode
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleSurfaceNode(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleSurfaceNode      = value;
   gmineInfoProperty->isSetIsVisibleSurfaceNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleText
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleText(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleText      = value;
   gmineInfoProperty->isSetIsVisibleText = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetFontName
**************************************************************************************************/
Gb gmineInfoPropertySetFontName(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->fontName      = _MiStrClone(value);
   gmineInfoProperty->isSetFontName = (gmineInfoProperty->fontName) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetFontName;
}

/**************************************************************************************************
func: gmineInfoPropertySetFontSize
**************************************************************************************************/
Gb gmineInfoPropertySetFontSize(GmineInfoProperty * const gmineInfoProperty, Gr4 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->fontSize      = value;
   gmineInfoProperty->isSetFontSize = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetKey
**************************************************************************************************/
Gb gmineInfoPropertySetKey(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->key      = _MiStrClone(value);
   gmineInfoProperty->isSetKey = (gmineInfoProperty->key) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetKey;
}

/**************************************************************************************************
func: gmineInfoPropertySetLabelData
**************************************************************************************************/
Gb gmineInfoPropertySetLabelData(GmineInfoProperty * const gmineInfoProperty, Gindex const index,
   Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   gmineInfoProperty->labelData[index]      = _MiStrClone(value);
   gmineInfoProperty->isSetLabelData[index] = (gmineInfoProperty->labelData) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetLabelData[index];
}

/**************************************************************************************************
func: gmineInfoPropertySetLabelLine
**************************************************************************************************/
Gb gmineInfoPropertySetLabelLine(GmineInfoProperty * const gmineInfoProperty, Gindex const index,
   Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   gmineInfoProperty->labelLine[index]      = _MiStrClone(value);
   gmineInfoProperty->isSetLabelLine[index] = (gmineInfoProperty->labelLine) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetLabelLine[index];
}

/**************************************************************************************************
func: gmineInfoPropertySetLabelNode
**************************************************************************************************/
Gb gmineInfoPropertySetLabelNode(GmineInfoProperty * const gmineInfoProperty, Gindex const index,
   Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   gmineInfoProperty->labelNode[index]      = _MiStrClone(value);
   gmineInfoProperty->isSetLabelNode[index] = (gmineInfoProperty->labelNode) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetLabelNode[index];
}

/**************************************************************************************************
func: gmineInfoPropertySetName
**************************************************************************************************/
Gb gmineInfoPropertySetName(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->name      = _MiStrClone(value);
   gmineInfoProperty->isSetName = (gmineInfoProperty->name) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetName;
}

/**************************************************************************************************
func: gmineInfoPropertySetPatternArrow
**************************************************************************************************/
Gb gmineInfoPropertySetPatternArrow(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->patternArrow      = _MiStrClone(value);
   gmineInfoProperty->isSetPatternArrow = (gmineInfoProperty->patternArrow) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetPatternArrow;
}

/**************************************************************************************************
func: gmineInfoPropertySetPatternFace
**************************************************************************************************/
Gb gmineInfoPropertySetPatternFace(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->patternFace      = _MiStrClone(value);
   gmineInfoProperty->isSetPatternFace = (gmineInfoProperty->patternFace) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetPatternFace;
}

/**************************************************************************************************
func: gmineInfoPropertySetPatternLine
**************************************************************************************************/
Gb gmineInfoPropertySetPatternLine(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->patternLine      = _MiStrClone(value);
   gmineInfoProperty->isSetPatternLine = (gmineInfoProperty->patternLine) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetPatternLine;
}

/**************************************************************************************************
func: gmineInfoPropertySetPatternNode
**************************************************************************************************/
Gb gmineInfoPropertySetPatternNode(GmineInfoProperty * const gmineInfoProperty, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->patternNode      = _MiStrClone(value);
   gmineInfoProperty->isSetPatternNode = (gmineInfoProperty->patternNode) ? gbTRUE : gbFALSE;

   return gmineInfoProperty->isSetPatternNode;
}

/**************************************************************************************************
func: gmineInfoPropertySetTransparency
**************************************************************************************************/
Gb gmineInfoPropertySetTransparency(GmineInfoProperty * const gmineInfoProperty, Gr4 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->faceTransparency      = value;
   gmineInfoProperty->isSetFaceTransparency = gbTRUE;

   returnTrue;
}
