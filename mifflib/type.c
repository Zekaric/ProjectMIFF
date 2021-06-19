/******************************************************************************
file:       type
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
Type functions
******************************************************************************/

/******************************************************************************
MIT License

Copyright (c) !!!!YEAR!!!!, Robbert de Groot

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
MiffN4 _TypeGetSize(Miff const * const miff, MiffValueType const type)
{
   miff; 

   switch(type)
   {
   case miffValueTypeBOOLEAN:
   case miffValueTypeI1:
   case miffValueTypeN1:
      return 1;

   case miffValueTypeTYPE:
   case miffValueTypeI2:                               
   case miffValueTypeN2:
      return 2;

   case miffValueTypeI3:
   case miffValueTypeN3:
      return 3;

   case miffValueTypeI4:
   case miffValueTypeN4:
   case miffValueTypeR4:
      return 4;

   case miffValueTypeI8:
   case miffValueTypeN8:
   case miffValueTypeR8:
      return 8;

   case miffValueTypeI16:
   case miffValueTypeN16:
      return 16;

   case miffValueTypeI32:
   case miffValueTypeN32:
      return 32;

   case miffValueTypeI64:
   case miffValueTypeN64:
      return 64;

   case miffValueTypeI128:
   case miffValueTypeN128:
      return 128;

   case miffValueTypeI256:
   case miffValueTypeN256:
      return 256;
   }

   return 0;
}