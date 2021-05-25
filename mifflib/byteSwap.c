/******************************************************************************
file:       byteSwap
author:     Robbert de Groot
copyright:  2021, 

description:
Functions to swap bytes from little endian to big endian and vice versa.
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
/******************************************************************************
func: _ByteSwap2
******************************************************************************/
void _ByteSwap2(Miff const * const miff, Miff2 * const value)
{
   if (miff->isByteSwapping)
   {
      value->byte[0] ^= value->byte[1];
      value->byte[1] ^= value->byte[0];
      value->byte[0] ^= value->byte[1];
   }
}

/******************************************************************************
func: _ByteSwap4
******************************************************************************/
void _ByteSwap4(Miff const * const miff, Miff4 * const value)
{
   if (miff->isByteSwapping)
   {
      value->byte[0] ^= value->byte[3];
      value->byte[3] ^= value->byte[0];
      value->byte[0] ^= value->byte[3];

      value->byte[1] ^= value->byte[2];
      value->byte[2] ^= value->byte[1];
      value->byte[1] ^= value->byte[2];
   }
}

/******************************************************************************
func: _ByteSwap8
******************************************************************************/
void _ByteSwap8(Miff const * const miff, Miff8 * const value)
{
   if (miff->isByteSwapping)
   {
      value->byte[0] ^= value->byte[7];
      value->byte[7] ^= value->byte[0];
      value->byte[0] ^= value->byte[7];

      value->byte[1] ^= value->byte[6];
      value->byte[6] ^= value->byte[1];
      value->byte[1] ^= value->byte[6];

      value->byte[2] ^= value->byte[5];
      value->byte[5] ^= value->byte[2];
      value->byte[2] ^= value->byte[5];

      value->byte[3] ^= value->byte[4];
      value->byte[4] ^= value->byte[3];
      value->byte[3] ^= value->byte[4];
   }
}
