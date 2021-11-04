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
static MiffMemCreate        _memCreate          = NULL;
static MiffMemDestroy       _memDestroy         = NULL;
static MiffMemCompressBound _memCompressBound   = NULL;
static MiffMemCompress      _memCompress        = NULL;
static MiffMemDecompress    _memDecompress      = NULL;

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
func: _MemCompare
******************************************************************************/
MiffI4 _MemCompare(MiffN4 const byteCount, void const * const buffA, void const * const buffB)
{
   MiffN4 index;
   MiffN1 *ba,
          *bb;
   MiffI4  result;

   ba = (MiffN1 *) buffA;
   bb = (MiffN1 *) buffB;
   for (index = 0; index < byteCount; index++)
   {
      result = ((MiffI4) ba[index]) - ((MiffI4) bb[index]);

      if (result < 0) return -1;
      if (result > 0) return  1;
   }

   return 0;
}

/******************************************************************************
func: _MemCompressBound
******************************************************************************/
MiffN4 _MemCompressBound(MiffN4 const memByteCount)
{
   return _memCompressBound(memByteCount);
}

/******************************************************************************
func: _MemCompress
******************************************************************************/
MiffBool _MemCompress(MiffN4 const memByteCount, void const * const mem,
   MiffN4 * const compressMemByteCount, void * const compressMem)
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
   MiffMemCompressBound const memCcompressBoundFunc, MiffMemCompress const memCompressFunc,
   MiffMemDecompress const memDecompressFunc)
{
   _memCreate        = memCreateFunc;
   _memDestroy       = memDestroyFunc;
   _memCompressBound = memCcompressBoundFunc;
   _memCompress      = memCompressFunc;
   _memDecompress    = memDecompressFunc;
}

/******************************************************************************
func: _MemStop
******************************************************************************/
void _MemStop(void)
{
   _memCreate        = NULL;
   _memDestroy       = NULL;
   _memCompressBound = NULL;
   _memCompress      = NULL;
   _memDecompress    = NULL;
}
