/******************************************************************************
file:       mem
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
memory functions
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
local:
variable:
******************************************************************************/
static MiffMemCreate    _memCreate  = NULL;
static MiffMemDestroy   _memDestroy = NULL;

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: _MemCreate
******************************************************************************/
void *_MemCreate(MiffN4 const memByteCount)
{
   return _memCreate(memByteCount);
}

/******************************************************************************
func: _MemDestroy
******************************************************************************/
void _MemDestroy(void * const mem)
{
   _memDestroy(mem);
}

/******************************************************************************
func: _MemStart
******************************************************************************/
void _MemStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc)
{
   _memCreate        = memCreateFunc;
   _memDestroy       = memDestroyFunc;
}

/******************************************************************************
func: _MemStop
******************************************************************************/
void _MemStop(void)
{
   _memCreate        = NULL;
   _memDestroy       = NULL;
}
