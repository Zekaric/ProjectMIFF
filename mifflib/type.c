/******************************************************************************
file:       type
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Type functions
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) 2021, Robbert de Groot

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
******************************************************************************/

/******************************************************************************
include:
******************************************************************************/
#include "local.h"

/******************************************************************************
lib local:
function:
******************************************************************************/
/******************************************************************************
func: _TypeGetSize
******************************************************************************/
MiffN4 _TypeGetSize(Miff const * const miff, MiffType const type)
{
   miff; 

   switch(type)
   {
   case miffTypeBOOLEAN:
   case miffTypeI1:
   case miffTypeN1:
      return 1;

   case miffTypeTYPE:
   case miffTypeI2:
   case miffTypeN2:
   case miffTypeABI1:
   case miffTypeABN1:
      return 2;

   case miffTypeABCI1:
   case miffTypeABCN1:
      return 3;

   case miffTypeI4:
   case miffTypeN4:
   case miffTypeR4:
   case miffTypeABI2:
   case miffTypeABN2:
   case miffTypeABCDI1:
   case miffTypeABCDN1:
      return 4;

   case miffTypeI8:
   case miffTypeN8:
   case miffTypeR8:
   case miffTypeABI4:
   case miffTypeABN4:
   case miffTypeABR4:
   case miffTypeABCDI2:
   case miffTypeABCDN2:
      return 8;

   case miffTypeABCI4:
   case miffTypeABCN4:
   case miffTypeABCR4:
      return 12;

   case miffTypeI16:
   case miffTypeN16:
   case miffTypeABI8:
   case miffTypeABN8:
   case miffTypeABR8:
   case miffTypeABCDI4:
   case miffTypeABCDN4:
   case miffTypeABCDR4:
   case miffTypeMATRIX2X2R4:
      return 16;

   case miffTypeABCI8:
   case miffTypeABCN8:
   case miffTypeABCR8:
      return 24;

   case miffTypeI32:
   case miffTypeN32:
   case miffTypeABCDI8:
   case miffTypeABCDN8:
   case miffTypeABCDR8:
   case miffTypeMATRIX2X2R8:
      return 32;

   case miffTypeMATRIX3X3R4:
      return 36;

   case miffTypeI64:
   case miffTypeN64:
   case miffTypeMATRIX4X4R4:
      return 64;

   case miffTypeMATRIX3X3R8:
      return 72;

   case miffTypeI128:
   case miffTypeN128:
   case miffTypeMATRIX4X4R8:
      return 128;

   case miffTypeI256:
   case miffTypeN256:
      return 256;
   }

   return 0;
}