/**************************************************************************************************
file:       GmineInfoData
author:     Robbert de Groot
company:    Robbert de Groot
copyright:  2025, Robbert de Groot

description:
Setting the data block information.
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
func: gmineInfoDataCloc
**************************************************************************************************/
GmineInfoData *gmineInfoDataCloc(void)
{
   GmineInfoData *gmineInfoData;

   returnNullIf(!gmineInfoIsStarted());

   gmineInfoData = _MiMemClocType(GmineInfoData);
   returnNullIf(!gmineInfoData);

   if (!gmineInfoDataClocContent(gmineInfoData))
   {
      _MiMemDloc(gmineInfoData);
      return NULL;
   }

   return gmineInfoData;
}

/**************************************************************************************************
func: gmineInfoDataClocContent
**************************************************************************************************/
Gb gmineInfoDataClocContent(GmineInfoData * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   _MiMemClearType(gmineInfoData, GmineInfoData);

   return gmineInfoBlockKeyValueArrayCloc(gmineInfoData);
}

/**************************************************************************************************
func: gmineInfoDataDloc
**************************************************************************************************/
void gmineInfoDataDloc(GmineInfoData * const gmineInfoData)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   gmineInfoDataDlocContent(gmineInfoData);

   _MiMemDloc(gmineInfoData);
}

/**************************************************************************************************
func: gmineInfoDataDlocContent
**************************************************************************************************/
void gmineInfoDataDlocContent(GmineInfoData * const gmineInfoData)
{
   returnVoidIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   _MiMemDloc(gmineInfoData->authorName);
   _MiMemDloc(gmineInfoData->companyName);
   _MiMemDloc(gmineInfoData->copyright);
   _MiMemDloc(gmineInfoData->projectName);
   _MiMemDloc(gmineInfoData->projectSystem);

   gmineInfoBlockKeyValueArrayDloc(gmineInfoData);

   _MiMemClearType(gmineInfoData, GmineInfoData);
}

/**************************************************************************************************
func: gmineInfoDataGetAuthorName
**************************************************************************************************/
Gstr const * gmineInfoDataGetAuthorName(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->authorName;
}

/**************************************************************************************************
func: gmineInfoDataGetCompanyName
**************************************************************************************************/
Gstr const * gmineInfoDataGetCompanyName(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->companyName;
}

/**************************************************************************************************
func: gmineInfoDataGetCopyright
**************************************************************************************************/
Gstr const * gmineInfoDataGetCopyright(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->copyright;
}

/**************************************************************************************************
func: gmineInfoDataGetProjectMax
**************************************************************************************************/
GmineInfoPoint const * gmineInfoDataGetProjectMax(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return &gmineInfoData->projectMax;
}

/**************************************************************************************************
func: gmineInfoDataGetProjectMin
**************************************************************************************************/
GmineInfoPoint const * gmineInfoDataGetProjectMin(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return &gmineInfoData->projectMin;
}

/**************************************************************************************************
func: gmineInfoDataGetProjectName
**************************************************************************************************/
Gstr const * gmineInfoDataGetProjectName(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->projectName;
}

/**************************************************************************************************
func: gmineInfoDataGetProjectSystem
**************************************************************************************************/
Gstr const * gmineInfoDataGetProjectSystem(GmineInfoData const * const gmineInfoData)
{
   returnNullIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->projectSystem;
}

/**************************************************************************************************
func: gmineInfoDataIsSetAuthorName
**************************************************************************************************/
Gb gmineInfoDataIsSetAuthorName(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetAuthorName;
}

/**************************************************************************************************
func: gmineInfoDataIsSetCompanyName
**************************************************************************************************/
Gb gmineInfoDataIsSetCompanyName(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetCompanyName;
}

/**************************************************************************************************
func: gmineInfoDataIsSetCopyright
**************************************************************************************************/
Gb gmineInfoDataIsSetCopyright(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetCopyright;
}

/**************************************************************************************************
func: gmineInfoDataIsSetProjectMax
**************************************************************************************************/
Gb gmineInfoDataIsSetProjectMax(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetProjectMax;
}

/**************************************************************************************************
func: gmineInfoDataIsSetProjectMin
**************************************************************************************************/
Gb gmineInfoDataIsSetProjectMin(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetProjectMin;
}

/**************************************************************************************************
func: gmineInfoDataIsSetProjectName
**************************************************************************************************/
Gb gmineInfoDataIsSetProjectName(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetProjectName;
}

/**************************************************************************************************
func: gmineInfoDataIsSetProjectSystem
**************************************************************************************************/
Gb gmineInfoDataIsSetProjectSystem(GmineInfoData const * const gmineInfoData)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData);

   return gmineInfoData->isSetProjectSystem;
}

/**************************************************************************************************
func: gmineInfoDataSetAuthorName
**************************************************************************************************/
Gb gmineInfoDataSetAuthorName(GmineInfoData * const gmineInfoData, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->authorName      = _MiStrClone(value);
   gmineInfoData->isSetAuthorName = (gmineInfoData->authorName) ? gbTRUE : gbFALSE;

   return gmineInfoData->isSetAuthorName;
}

/**************************************************************************************************
func: gmineInfoDataSetCompanyName
**************************************************************************************************/
Gb gmineInfoDataSetCompanyName(GmineInfoData * const gmineInfoData, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->companyName      = _MiStrClone(value);
   gmineInfoData->isSetCompanyName = (gmineInfoData->companyName) ? gbTRUE : gbFALSE;

   return gmineInfoData->isSetCompanyName;
}

/**************************************************************************************************
func: gmineInfoDataSetCopyright
**************************************************************************************************/
Gb gmineInfoDataSetCopyright(GmineInfoData * const gmineInfoData, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->copyright      = _MiStrClone(value);
   gmineInfoData->isSetCopyright = (gmineInfoData->copyright) ? gbTRUE : gbFALSE;

   return gmineInfoData->isSetCopyright;
}

/**************************************************************************************************
func: gmineInfoDataSetProjectMax
**************************************************************************************************/
Gb gmineInfoDataSetProjectMax(GmineInfoData * const gmineInfoData, GmineInfoPoint const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->projectMax = *value;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoDataSetProjectMin
**************************************************************************************************/
Gb gmineInfoDataSetProjectMin(GmineInfoData * const gmineInfoData, GmineInfoPoint const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->projectMin = *value;

   returnTrue;
}

/**************************************************************************************************
func: gmineInfoDataSetProjectName
**************************************************************************************************/
Gb gmineInfoDataSetProjectName(GmineInfoData * const gmineInfoData, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->projectName      = _MiStrClone(value);
   gmineInfoData->isSetProjectName = (gmineInfoData->projectName) ? gbTRUE : gbFALSE;

   return gmineInfoData->isSetProjectName;
}

/**************************************************************************************************
func: gmineInfoDataSetProjectSystem
**************************************************************************************************/
Gb gmineInfoDataSetProjectSystem(GmineInfoData * const gmineInfoData, Gstr const * const value)
{
   returnFalseIf(
      !gmineInfoIsStarted() ||
      !gmineInfoData        ||
      !value);

   gmineInfoData->projectSystem      = _MiStrClone(value);
   gmineInfoData->isSetProjectSystem = (gmineInfoData->projectSystem) ? gbTRUE : gbFALSE;

   return gmineInfoData->isSetProjectSystem;
}
