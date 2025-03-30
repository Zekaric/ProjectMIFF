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
func: gmineInfoPropertyAddKeyValue
**************************************************************************************************/
Gb gmineInfoPropertyAddKeyValue(GmineInfoProperty * const gmineInfoProperty, Gstr const * const key,
   Gstr const * const value)
{
   GmineInfoKeyValue *kv;

   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   kv = gmineInfoKeyValueCloc(key, value);
   returnFalseIf(!kv);

   return gmineInfoArrayAddLast(&gmineInfoProperty->keyValueArray, kv);
}

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

   return gmineInfoArrayClocContent(&gmineInfoProperty->keyValueArray);
}

/**************************************************************************************************
func: gmineInfoPropertyDloc
**************************************************************************************************/
void gmineInfoPropertyDloc(GmineInfoProperty * const gmineInfoProperty)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoPropertyDlocContent(gmineInfoProperty);

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
func: gmineInfoPropertyGetColorFace
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorFace;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorFacePattern
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorFacePattern(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorFacePattern;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorLine
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorLine;
}

/**************************************************************************************************
func: gmineInfoPropertyGetColorNode
**************************************************************************************************/
GmineInfoColor const *gmineInfoPropertyGetColorNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return &gmineInfoProperty->colorNode;
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
func: gmineInfoPropertyGetFaceTransparency
**************************************************************************************************/
Gr4 gmineInfoPropertyGetFaceTransparency(GmineInfoProperty const * const gmineInfoProperty)
{
   return0If(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->faceTransparency;
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
func: gmineInfoPropertyGetKeyValueAt
**************************************************************************************************/
Gb gmineInfoPropertyGetKeyValueAt(GmineInfoProperty const * const gmineInfoProperty, Gindex const index,
   Gstr const ** const key, Gstr const ** const value)
{
   GmineInfoKeyValue *kv;

   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   kv = gmineInfoArrayGetAt(&gmineInfoProperty->keyValueArray, index);
   returnFalseIf(!kv);

   *key   = gmineInfoKeyValueGetKey(  kv);
   *value = gmineInfoKeyValueGetValue(kv);

   returnTrue;
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
func: gmineInfoPropertyIsSetIsVisibleDataLabel
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleDataLabel(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isSetIsVisibleDataLabel[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleLineLabel
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleLineLabel(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isSetIsVisibleLineLabel[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetIsVisibleNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetIsVisibleNodeLabel
**************************************************************************************************/
Gb gmineInfoPropertyIsSetIsVisibleNodeLabel(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   return gmineInfoProperty->isSetIsVisibleNodeLabel[index];
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
func: gmineInfoPropertyIsSetColorFace
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorFacePattern
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorFacePattern(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorFacePattern;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorLine
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsSetColorNode
**************************************************************************************************/
Gb gmineInfoPropertyIsSetColorNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetColorNode;
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
func: gmineInfoPropertyIsSetFaceTransparency
**************************************************************************************************/
Gb gmineInfoPropertyIsSetFaceTransparency(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isSetFaceTransparency;
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
func: gmineInfoPropertyIsVisibleArrow
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleArrow(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleArrow;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleDataLabel
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleDataLabel(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isVisibleDataLabel[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleFace
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleFace(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleFace;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleLine
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleLine(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleLine;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleLineLabel
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleLineLabel(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   return gmineInfoProperty->isVisibleLineLabel[index];
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleNode
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleNode(GmineInfoProperty const * const gmineInfoProperty)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   return gmineInfoProperty->isVisibleNode;
}

/**************************************************************************************************
func: gmineInfoPropertyIsVisibleNodeLabel
**************************************************************************************************/
Gb gmineInfoPropertyIsVisibleNodeLabel(GmineInfoProperty const * const gmineInfoProperty,
   Gindex const index)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   return gmineInfoProperty->isVisibleNodeLabel[index];
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
func: gmineInfoPropertySetColorFace
**************************************************************************************************/
Gb gmineInfoPropertySetColorFace(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorFace      = *value;
   gmineInfoProperty->isSetColorFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorFacePattern
**************************************************************************************************/
Gb gmineInfoPropertySetColorFacePattern(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorFacePattern      = *value;
   gmineInfoProperty->isSetColorFacePattern = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorLine
**************************************************************************************************/
Gb gmineInfoPropertySetColorLine(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorLine      = *value;
   gmineInfoProperty->isSetColorLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetColorNode
**************************************************************************************************/
Gb gmineInfoPropertySetColorNode(GmineInfoProperty * const gmineInfoProperty, GmineInfoColor const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->colorNode      = *value;
   gmineInfoProperty->isSetColorNode = gbTRUE;

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
func: gmineInfoPropertySetIsVisibleArrow
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleArrow(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleArrow      = value;
   gmineInfoProperty->isSetIsVisibleArrow = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleDataLabel
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleDataLabel(GmineInfoProperty * const gmineInfoProperty,
   Gindex const index, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   gmineInfoProperty->isVisibleDataLabel[index]      = value;
   gmineInfoProperty->isSetIsVisibleDataLabel[index] = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleFace
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleFace(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleFace      = value;
   gmineInfoProperty->isSetIsVisibleFace = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleLine
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleLine(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleLine      = value;
   gmineInfoProperty->isSetIsVisibleLine = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleLineLabel
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleLineLabel(GmineInfoProperty * const gmineInfoProperty,
   Gindex const index, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 10 <= index);

   gmineInfoProperty->isVisibleLineLabel[index]      = value;
   gmineInfoProperty->isSetIsVisibleLineLabel[index] = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleNode
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleNode(GmineInfoProperty * const gmineInfoProperty, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->isVisibleNode      = value;
   gmineInfoProperty->isSetIsVisibleNode = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoPropertySetIsVisibleNodeLabel
**************************************************************************************************/
Gb gmineInfoPropertySetIsVisibleNodeLabel(GmineInfoProperty * const gmineInfoProperty,
   Gindex const index, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty    ||
      index < 0 || 20 <= index);

   gmineInfoProperty->isVisibleNodeLabel[index]      = value;
   gmineInfoProperty->isSetIsVisibleNodeLabel[index] = gbTRUE;

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
func: gmineInfoPropertySetFaceTransparency
**************************************************************************************************/
Gb gmineInfoPropertySetFaceTransparency(GmineInfoProperty * const gmineInfoProperty, Gr4 const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoProperty);

   gmineInfoProperty->faceTransparency      = value;
   gmineInfoProperty->isSetFaceTransparency = gbTRUE;

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

