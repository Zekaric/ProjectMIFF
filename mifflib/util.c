/******************************************************************************
file:       util
author:     Robbert de Groot
company:    
copyright:  2021, Robbert de Groot

description:
General utils
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "local.h"

/******************************************************************************
local:
constant:
******************************************************************************/

/******************************************************************************
type:
******************************************************************************/

/******************************************************************************
variable:
******************************************************************************/

/******************************************************************************
prototype:
******************************************************************************/

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _CurrentIndexInc
******************************************************************************/
MiffBool _CurrentIndexInc(Miff * const miff)
{
   miff->currentIndex++;
   if (miff->currentRecord.type == miffValueTypeKEY_VALUE_BLOCK_START ||
       miff->currentRecord.type == miffValueTypeKEY_VALUE_BLOCK_STOP  ||
       miff->currentIndex       == miff->currentRecord.arrayCount)
   {
      miff->currentRecord.type = miffValueTypeNONE;
      returnFalseIf(!_WriteTxtRecordEnder(miff));
   }
   else
   {
      returnFalseIf(!_WriteTxtRecordSeparator(miff));
   }

   returnTrue;
}

/******************************************************************************
func: _MemIsEqual

Compare two binary buffers for equality.
******************************************************************************/
MiffBool _MemIsEqual(MiffN4 const countA, MiffN1 const * const memA, MiffN4 const countB, 
   MiffN1 const * const memB)
{
   returnFalseIf(countA != countB);
   returnFalseIf(memcmp(memA, memB, countA) != 0);
   returnTrue;
}
