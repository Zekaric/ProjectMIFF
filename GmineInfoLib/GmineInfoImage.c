/**************************************************************************************************
file:       GmineInfoImage
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Image files section.  Images are used as textures on geometry usually.
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
func: gmineInfoImageCloc
**************************************************************************************************/
GmineInfoImage *gmineInfoImageCloc(void)
{
   GmineInfoImage *gmineInfoImage;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoImage = _MiMemClocType(GmineInfoImage);
   returnNullIf(!gmineInfoImage);

   if (!gmineInfoImageClocContent(gmineInfoImage))
   {
      _MiMemDloc(   gmineInfoImage);
      return NULL;
   }

   return gmineInfoImage;
}

/**************************************************************************************************
func: gmineInfoImageClocContent
**************************************************************************************************/
Gb gmineInfoImageClocContent(GmineInfoImage * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   _MiMemClearType(gmineInfoImage, GmineInfoImage);

   return gmineInfoBlockKeyValueArrayCloc(gmineInfoImage);
}

/**************************************************************************************************
func: gmineInfoImageDloc
**************************************************************************************************/
void gmineInfoImageDloc(GmineInfoImage * const gmineInfoImage)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoImageDlocContent(gmineInfoImage);

   _MiMemDloc(gmineInfoImage);
}

/**************************************************************************************************
func: gmineInfoImageDlocContent
**************************************************************************************************/
void gmineInfoImageDlocContent(GmineInfoImage * const gmineInfoImage)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoBlockKeyValueArrayDloc(gmineInfoImage);

   _MiMemClearType(gmineInfoImage, GmineInfoImage);
}

/**************************************************************************************************
func: gmineInfoImageGetFileName
**************************************************************************************************/
Gstr const * gmineInfoImageGetFileName(GmineInfoImage const * const gmineInfoImage)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->fileName;
}

/**************************************************************************************************
func: gmineInfoImageGetFilePath
**************************************************************************************************/
Gstr const * gmineInfoImageGetFilePath(GmineInfoImage const * const gmineInfoImage)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->filePath;
}

/**************************************************************************************************
func: gmineInfoImageGetKey
**************************************************************************************************/
Gstr const * gmineInfoImageGetKey(GmineInfoImage const * const gmineInfoImage)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->key;
}

/**************************************************************************************************
func: gmineInfoImageGetName
**************************************************************************************************/
Gstr const * gmineInfoImageGetName(GmineInfoImage const * const gmineInfoImage)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->name;
}

/**************************************************************************************************
func: gmineInfoImageIsInline
**************************************************************************************************/
Gb gmineInfoImageIsInline(GmineInfoImage const * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->isInline;
}

/**************************************************************************************************
func: gmineInfoImageIsSetFileName
**************************************************************************************************/
Gb gmineInfoImageIsSetFileName(GmineInfoImage const * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->isSetFileName;
}

/**************************************************************************************************
func: gmineInfoImageIsSetFilePath
**************************************************************************************************/
Gb gmineInfoImageIsSetFilePath(GmineInfoImage const * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->isSetFilePath;
}

/**************************************************************************************************
func: gmineInfoImageIsSetIsInline
**************************************************************************************************/
Gb gmineInfoImageIsSetIsInline(GmineInfoImage const * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->isSetIsInline;
}

/**************************************************************************************************
func: gmineInfoImageIsSetKey
**************************************************************************************************/
Gb gmineInfoImageIsSetKey(GmineInfoImage const * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->isSetKey;
}

/**************************************************************************************************
func: gmineInfoImageIsSetName
**************************************************************************************************/
Gb gmineInfoImageIsSetName(GmineInfoImage const * const gmineInfoImage)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   return gmineInfoImage->isSetName;
}

/**************************************************************************************************
func: gmineInfoImageSetFileName
**************************************************************************************************/
Gb gmineInfoImageSetFileName(GmineInfoImage * const gmineInfoImage, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoImage->fileName      = _MiStrClone(value);
   gmineInfoImage->isSetFileName = (gmineInfoImage->fileName) ? gbTRUE : gbFALSE;

   return gmineInfoImage->isSetFileName;
}

/**************************************************************************************************
func: gmineInfoImageSetFilePath
**************************************************************************************************/
Gb gmineInfoImageSetFilePath(GmineInfoImage * const gmineInfoImage, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoImage->filePath      = _MiStrClone(value);
   gmineInfoImage->isSetFilePath = (gmineInfoImage->filePath) ? gbTRUE : gbFALSE;

   return gmineInfoImage->isSetFilePath;
}

/**************************************************************************************************
func: gmineInfoImageSetIsInline
**************************************************************************************************/
Gb gmineInfoImageSetIsInline(GmineInfoImage * const gmineInfoImage, Gb const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoImage->isInline      = value;
   gmineInfoImage->isSetIsInline = gbTRUE;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoImageSetKey
**************************************************************************************************/
Gb gmineInfoImageSetKey(GmineInfoImage * const gmineInfoImage, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoImage->key      = _MiStrClone(value);
   gmineInfoImage->isSetKey = (gmineInfoImage->key) ? gbTRUE : gbFALSE;

   return gmineInfoImage->isSetKey;
}

/**************************************************************************************************
func: gmineInfoImageSetName
**************************************************************************************************/
Gb gmineInfoImageSetName(GmineInfoImage * const gmineInfoImage, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoImage);

   gmineInfoImage->name      = _MiStrClone(value);
   gmineInfoImage->isSetName = (gmineInfoImage->name) ? gbTRUE : gbFALSE;

   return gmineInfoImage->isSetName;
}
