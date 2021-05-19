/******************************************************************************
file:       mem
author:     Robbert de Groot
copyright:  2021, Robbert de Groot

description:
memory functions
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
static MiffMemCreate        _memCreate                = NULL;
static MiffMemDestroy       _memDestroy               = NULL;
static MiffMemCompress      _memCompress              = NULL;
static MiffMemDecompress    _memDecompress            = NULL;

/******************************************************************************
prototype:
******************************************************************************/

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
func: _MemCompress
******************************************************************************/
MiffBool _MemCompress(MiffN4 const memByteCount, void const * const mem, 
   MiffN4 * const compressMemByteCount, MiffN1 ** const compressMem)
{
   return _memCompress(memByteCount, mem, compressMemByteCount, compressMem);
}

/******************************************************************************
func: _MemDecompress
******************************************************************************/
MiffBool _MemDecompress(MiffN4 const compressMemByteCount, MiffN1 const * const compressMem, 
   MiffN4 * const memByteCount, void ** const mem)
{
   return _memDecompress(compressMemByteCount, compressMem, memByteCount, mem);
}

/******************************************************************************
func: _MemStart
******************************************************************************/
void _MemStart(MiffMemCreate const memCreateFunc, MiffMemDestroy const memDestroyFunc,
   MiffMemCompress const memCompressFunc, MiffMemDecompress const memDecompressFunc)
{
   _memCreate     = memCreateFunc;
   _memDestroy    = memDestroyFunc;
   _memCompress   = memCompressFunc;
   _memDecompress = memDecompressFunc;
}

/******************************************************************************
func: _MemStop
******************************************************************************/
void _MemStop(void)
{
   _memCreate     = NULL;
   _memDestroy    = NULL;
   _memCompress   = NULL;
   _memDecompress = NULL;
}
