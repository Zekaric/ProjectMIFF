/**************************************************************************************************
file:       MiffLibTest
author:     Robbert de Groot
company:    Zekaric
copyright:  2021, Zekaric

description:
Test program for MiffLib
**************************************************************************************************/

/**************************************************************************************************
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
**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

//#include "zlib.h"
#include "gmifflib.h"
#include "gjsonlib.h"

/**************************************************************************************************
local:
macro
**************************************************************************************************/
#if 1
#define INCLUDE_BIN
#endif

#define streq(S1, S2)  (strcmp(S1, S2) == 0)

/**************************************************************************************************
variable:
**************************************************************************************************/
static uint8_t _binary[] =
{
        0,   0,   0,
        1,   1,   1,
        2,   2,   2,
        3,   3,   3,
        4,   4,   4,
        5,   5,   5,
        6,   6,   6,
        7,   7,   7,
        8,   8,   8,
        9,   9,   9,
       10,  10,  10,
       11,  11,  11,
       12,  12,  12,
       13,  13,  13,
       14,  14,  14,
       15,  15,  15,
       16,  16,  16,
       17,  17,  17,
       18,  18,  18,
       19,  19,  19,
       20,  20,  20,
       21,  21,  21,
       22,  22,  22,
       23,  23,  23,
       24,  24,  24,
       25,  25,  25,
       26,  26,  26,
       27,  27,  27,
       28,  28,  28,
       29,  29,  29,
       30,  30,  30,
       31,  31,  31,
       32,  32,  32,
       33,  33,  33,
       34,  34,  34,
       35,  35,  35,
       36,  36,  36,
       37,  37,  37,
       38,  38,  38,
       39,  39,  39,
       40,  40,  40,
       41,  41,  41,
       42,  42,  42,
       43,  43,  43,
       44,  44,  44,
       45,  45,  45,
       46,  46,  46,
       47,  47,  47,
       48,  48,  48,
       49,  49,  49,
       50,  50,  50,
       51,  51,  51,
       52,  52,  52,
       53,  53,  53,
       54,  54,  54,
       55,  55,  55,
       56,  56,  56,
       57,  57,  57,
       58,  58,  58,
       59,  59,  59,
       60,  60,  60,
       61,  61,  61,
       62,  62,  62,
       63,  63,  63,
       64,  64,  64,
       65,  65,  65,
       66,  66,  66,
       67,  67,  67,
       68,  68,  68,
       69,  69,  69,
       70,  70,  70,
       71,  71,  71,
       72,  72,  72,
       73,  73,  73,
       74,  74,  74,
       75,  75,  75,
       76,  76,  76,
       77,  77,  77,
       78,  78,  78,
       79,  79,  79,
       80,  80,  80,
       81,  81,  81,
       82,  82,  82,
       83,  83,  83,
       84,  84,  84,
       85,  85,  85,
       86,  86,  86,
       87,  87,  87,
       88,  88,  88,
       89,  89,  89,
       90,  90,  90,
       91,  91,  91,
       92,  92,  92,
       93,  93,  93,
       94,  94,  94,
       95,  95,  95,
       96,  96,  96,
       97,  97,  97,
       98,  98,  98,
       99,  99,  99,
      100, 100, 100,
      101, 101, 101,
      102, 102, 102,
      103, 103, 103,
      104, 104, 104,
      105, 105, 105,
      106, 106, 106,
      107, 107, 107,
      108, 108, 108,
      109, 109, 109,
      110, 110, 110,
      111, 111, 111,
      112, 112, 112,
      113, 113, 113,
      114, 114, 114,
      115, 115, 115,
      116, 116, 116,
      117, 117, 117,
      118, 118, 118,
      119, 119, 119,
      120, 120, 120,
      121, 121, 121,
      122, 122, 122,
      123, 123, 123,
      124, 124, 124,
      125, 125, 125,
      126, 126, 126,
      127, 127, 127,
      128, 128, 128,
      129, 129, 129,
      130, 130, 130,
      131, 131, 131,
      132, 132, 132,
      133, 133, 133,
      134, 134, 134,
      135, 135, 135,
      136, 136, 136,
      137, 137, 137,
      138, 138, 138,
      139, 139, 139,
      140, 140, 140,
      141, 141, 141,
      142, 142, 142,
      143, 143, 143,
      144, 144, 144,
      145, 145, 145,
      146, 146, 146,
      147, 147, 147,
      148, 148, 148,
      149, 149, 149,
      150, 150, 150,
      151, 151, 151,
      152, 152, 152,
      153, 153, 153,
      154, 154, 154,
      155, 155, 155,
      156, 156, 156,
      157, 157, 157,
      158, 158, 158,
      159, 159, 159,
      160, 160, 160,
      161, 161, 161,
      162, 162, 162,
      163, 163, 163,
      164, 164, 164,
      165, 165, 165,
      166, 166, 166,
      167, 167, 167,
      168, 168, 168,
      169, 169, 169,
      170, 170, 170,
      171, 171, 171,
      172, 172, 172,
      173, 173, 173,
      174, 174, 174,
      175, 175, 175,
      176, 176, 176,
      177, 177, 177,
      178, 178, 178,
      179, 179, 179,
      180, 180, 180,
      181, 181, 181,
      182, 182, 182,
      183, 183, 183,
      184, 184, 184,
      185, 185, 185,
      186, 186, 186,
      187, 187, 187,
      188, 188, 188,
      189, 189, 189,
      190, 190, 190,
      191, 191, 191,
      192, 192, 192,
      193, 193, 193,
      194, 194, 194,
      195, 195, 195,
      196, 196, 196,
      197, 197, 197,
      198, 198, 198,
      199, 199, 199,
      200, 200, 200,
      201, 201, 201,
      202, 202, 202,
      203, 203, 203,
      204, 204, 204,
      205, 205, 205,
      206, 206, 206,
      207, 207, 207,
      208, 208, 208,
      209, 209, 209,
      210, 210, 210,
      211, 211, 211,
      212, 212, 212,
      213, 213, 213,
      214, 214, 214,
      215, 215, 215,
      216, 216, 216,
      217, 217, 217,
      218, 218, 218,
      219, 219, 219,
      220, 220, 220,
      221, 221, 221,
      222, 222, 222,
      223, 223, 223,
      224, 224, 224,
      225, 225, 225,
      226, 226, 226,
      227, 227, 227,
      228, 228, 228,
      229, 229, 229,
      230, 230, 230,
      231, 231, 231,
      232, 232, 232,
      233, 233, 233,
      234, 234, 234,
      235, 235, 235,
      236, 236, 236,
      237, 237, 237,
      238, 238, 238,
      239, 239, 239,
      240, 240, 240,
      241, 241, 241,
      242, 242, 242,
      243, 243, 243,
      244, 244, 244,
      245, 245, 245,
      246, 246, 246,
      247, 247, 247,
      248, 248, 248,
      249, 249, 249,
      250, 250, 250,
      251, 251, 251,
      252, 252, 252,
      253, 253, 253,
      254, 254, 254,
      255, 255, 255
};

static uint64_t _narray[] =
{
      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,

      0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,
      34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
      65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
      96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,
      120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,
      143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,
      166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,
      189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,
      212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,
      235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};

static float _reals4[300] =
{
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
      0.0, 1.0, -1.0, FLT_MAX, -FLT_MAX, (float) 3.141592653589793226535897932,
};

static double _reals8[300] =
{
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
      0.0, 1.0, -1.0, DBL_MAX, -DBL_MAX, 3.141592653589793226535897932,
};

static char *_strings[10] =
{
      "the quick brown fox jumped over the lazy dog.",
      "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.",
      "I fart in your general direction.",
      "No one expects the Spanish Inquisition!",
      "And now for something completely different.",
      "the quick brown fox jumped over the lazy dog.\tTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.",
      "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.\nthe quick brown fox jumped over the lazy dog.",
      "I fart in your general direction.",
      "No one expects the Spanish Inquisition!",
      "And now for something completely different."
};
static int                  _bools[100] =
{
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
      1, 1, 0, 1, 1, 0, 0, 0, 1, 0,
};

/**************************************************************************************************
prototype:
**************************************************************************************************/
static Gb       _GetBuffer(         void * const dataRepo, Gcount const byteCount, Gn1       * const byteData);

static void    *_MemCloc(           Gcount const memByteCount);
static void     _MemDloc(           void * const mem);

static Gb       _SetBuffer(         void * const dataRepo, Gcount const byteCount, Gn1 const * const byteData);

static Gb       _JsonTestRead(      Gstr const * const fileName);
static Gb       _JsonTestReadObject(Gjson * const json);
static Gb       _JsonTestWrite(     Gstr const * const fileName);

static Gb       _MiffTestRead(      Gstr const * const fileName);
static Gb       _MiffTestWrite(     Gstr const * const fileName);

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: main
**************************************************************************************************/
int main(int acount, char **alist)
{
   int   result;

   acount; alist;

   result = 0;

   // Start miff library.
   if (!gmiffStart(_MemCloc, _MemDloc) ||
       !gjsonStart(_MemCloc, _MemDloc))
   {
      result = 1;
      goto DONE;
   }

   // Test the writing.
   if (!_MiffTestWrite("FileTest.miff"))
   {
      result = 2;
      goto DONE;
   }

   if (!_MiffTestRead("FileTest.miff"))
   {
      result = 4;
      goto DONE;
   }

   if (!_JsonTestWrite("FileTest.json"))
   {
      result = 8;
      goto DONE;
   }

   if (!_JsonTestRead("FileTest.json"))
   {
      result = 16;
      goto DONE;
   }

DONE:
   // Stop miff library.
   gmiffStop();
   gjsonStop();

   return result;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _GetBuffer
**************************************************************************************************/
static Gb _GetBuffer(void * const dataRepo, Gcount const byteCount, Gn1 * const byteData)
{
   returnFalseIf(byteCount <= 0);

   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == (size_t) byteCount);
}

/**************************************************************************************************
func: _MemCloc
**************************************************************************************************/
void *_MemCloc(Gcount const memByteCount)
{
   returnNullIf(memByteCount <= 0);

   return calloc(1, (size_t) memByteCount);
}

/**************************************************************************************************
func: _MemDloc
**************************************************************************************************/
void _MemDloc(void * const mem)
{
   returnVoidIf(!mem);

   free(mem);
}

/**************************************************************************************************
func: _SetBuffer
**************************************************************************************************/
static Gb _SetBuffer(void * const dataRepo, Gcount const byteCount, Gn1 const * const byteData)
{
   returnFalseIf(byteCount <= 0);

   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == (size_t) byteCount);
}

/**************************************************************************************************
func: _JsonTestInt
**************************************************************************************************/
static Gb _JsonTestInt(Gjson * const json, Gi8 const value)
{
   Gi8 itemp;

   returnFalseIf(
      !gjsonGetI(json, &itemp) ||
      itemp != value);
   returnTrue;
}

/**************************************************************************************************
func: _JsonTestNat
**************************************************************************************************/
static Gb _JsonTestNat(Gjson * const json, Gn8 const value)
{
   Gn8 ntemp;

   returnFalseIf(
      !gjsonGetN(json, &ntemp) ||
      ntemp != value);

   returnTrue;
}

/**************************************************************************************************
func: _JsonTestReal
**************************************************************************************************/
static Gb _JsonTestReal(Gjson * const json, Gr8 const value)
{
   Gr8 rtemp;

   returnFalseIf(!gjsonGetR(json, &rtemp));

   returnFalseIf(
      ( isnan(value) && !isnan(rtemp)) ||
      (!isnan(value) && rtemp != value));

   returnTrue;
}

/**************************************************************************************************
func: _JsonTestReal4
**************************************************************************************************/
static Gb _JsonTestReal4(Gjson * const json, Gr4 value)
{
   Gr4 r4temp;

   returnFalseIf(!gjsonGetR4(json, &r4temp));

   returnFalseIf(
      ( isnan(value) && !isnan(r4temp)) ||
      (!isnan(value) && r4temp != value));

   returnTrue;
}

/**************************************************************************************************
func: _JsonTestStr
**************************************************************************************************/
static Gb _JsonTestStr(Gjson * const json, char const * const value)
{
   int            sindex;
   GjsonStrLetter jletter;
   Gstr           sletter,
                  stemp[256];

   loopCount(sindex)
   {
      jletter = gjsonGetStrLetter(json, &sletter);

      breakIf(
         jletter == gjsonStrLetterDONE ||
         jletter == gjsonStrLetterERROR);

      stemp[sindex] = sletter;
   }
   returnFalseIf(jletter == gjsonStrLetterERROR);

   stemp[sindex] = 0;
   returnFalseIf(strcmp(stemp, value) != 0);

   returnTrue;
}

/**************************************************************************************************
func: _JsonTestBin
**************************************************************************************************/
static Gb _JsonTestBin(Gjson * const json, int binCount, Gn1 const * const binData)
{
   int bindex;
   Gn1 byte;
   Gn1 binBuffer[3 * 256];

   forCount(bindex, binCount)
   {
      returnFalseIf(gjsonGetStrBinByte(json, &byte) != gjsonStrLetterNORMAL);

      binBuffer[bindex] = byte;
   }
   returnFalseIf(gjsonGetStrLetter(json, (Gstr *) &byte) != gjsonStrLetterDONE);

   returnFalseIf(memcmp(binBuffer, binData, binCount) != 0);

   returnTrue;
}

/**************************************************************************************************
func: _JsonTestRead
**************************************************************************************************/
static Gb _JsonTestRead(Gstr const * const fileName)
{
   int           index;
   FILE         *file;
   Gjson         *json;
   Gb         result;
   GjsonType      type;
   char const   *msg;
   Gi8         itemp;
   Gn8         ntemp;
   Gr8         rtemp;
   Gr4        r4temp;
   Gstr       stemp[256];

   file   = NULL;
   json   = NULL;
   result = gbFALSE;

   breakScope
   {
      msg = "Open file";
      breakIf(fopen_s(&file, fileName, "rb") != 0);

      // Set Gjson up for reading.
      msg = "Create reader";
      json = gjsonClocReader(_GetBuffer, (void *) file);
      breakIf(!json);

      // Start of an object.
      msg = "Looking for object start";
      breakIf(gjsonGetType_FileElement( json) != gjsonTypeOBJECT_START);

      msg = "Null";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(    json) != gjsonTypeVALUE_NULL);

      msg = "True";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(    json) != gjsonTypeVALUE_TRUE);

      msg = "False";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(    json) != gjsonTypeVALUE_FALSE);

      msg = "I 0";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestInt(json, 0));

      msg = "I 1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestInt(json, 1));

      msg = "I -1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestInt(json, -1));

      msg = "I MAX";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestInt(json, Gi8MAX));

      msg = "I MIN";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestInt(json, Gi8MIN));

      msg = "N 0";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestNat(json, 0));

      msg = "N 1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestNat(json, 1));

      msg = "N MAX";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestNat(json, Gn8MAX));

      msg = "R 0";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, 0.0));

      msg = "R 1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, 1.0));

      msg = "R -1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, -1.0));

      msg = "R PI";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, 3.1415926535897932));

      msg = "R EPS";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, Gr8EPSILON));

      msg = "R MAX";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, Gr8MAX));

      msg = "R -MAX";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, -Gr8MAX));

      msg = "R INF";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, Gr8INF));

      msg = "R -INF";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, -Gr8INF));

      msg = "R NAN";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal(json, GrNAN));

      msg = "R4 0";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, 0.0f));

      msg = "R4 1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, 1.0f));

      msg = "R4 -1";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, -1.0f));

      msg = "R4 PI";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, 3.1415926535897932f));

      msg = "R4 EPS";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, Gr4EPSILON));

      msg = "R4 MAX";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, Gr4MAX));

      msg = "R4 -MAX";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, -Gr4MAX));

      msg = "R4 INF";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, Gr4INF));

      msg = "R4 -INF";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, -Gr4INF));

      msg = "R4 NAN";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestReal4(json, GrNAN));

      msg = "String";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeStr(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestStr(json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./"));

#if defined(INCLUDE_BIN)
      msg = "Binary";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(!gjsonIsTypeBin(gjsonGetType_ObjectValue(json)));
      breakIf(!_JsonTestBin(json, 3 * 256, _binary));
#endif

      msg = "Bool Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 100);

         breakIf(
            !((type == gjsonTypeVALUE_TRUE  && _bools[index] == 1) ||
              (type == gjsonTypeVALUE_FALSE && _bools[index] == 0)));
      }
      breakIf(index != 100);

      msg = "I Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 256);

         breakIf(
            !gjsonIsTypeI(type)        ||
            !gjsonGetI(json, &itemp)   ||
            (Gi8) _narray[index] != itemp);
      }
      breakIf(index != 256);

      msg = "N Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 256);

         breakIf(
            !gjsonIsTypeN(type)        ||
            !gjsonGetN(json, &ntemp)   ||
            _narray[index] != ntemp);
      }
      breakIf(index != 256);

      msg = "R Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 300);

         breakIf(
            !gjsonIsTypeR(type) ||
            !_JsonTestReal(json, _reals8[index]));
      }
      breakIf(index != 300);

      msg = "R4 Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 300);

         breakIf(
            !gjsonIsTypeR(type) ||
            !_JsonTestReal4(json, _reals4[index]));
      }
      breakIf(index != 300);

      msg = "String Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 10);

         breakIf(
            !gjsonIsTypeStr(type)            ||
            !gjsonGetStr(json, 256, stemp)   ||
            strcmp(stemp, _strings[index]) != 0);
      }
      breakIf(index != 10);

#if defined(INCLUDE_BIN)
      msg = "Binary Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      loopCount(index)
      {
         type = gjsonGetType_ArrayValueOrStop(json);
         breakIf(type == gjsonTypeARRAY_STOP);
         continueIf(index >= 3);

         breakIf(
            !gjsonIsTypeBin(type) ||
            !_JsonTestBin(json, 3 * 256, _binary));
      }
      breakIf(index != 3);
#endif

      msg = "User Type IntStrReal";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      {
         breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
         breakIf(!_JsonTestInt(json, 42));

         breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
         breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

         breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
         breakIf(!_JsonTestReal(json, 3.1415926535897932));

         breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
      }

      msg = "User Type IntStrReal Array";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
      {
         breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_START);
         {
            breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestInt(json, 42));

            breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

            breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestReal(json, 3.1415926535897932));

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
         }

         breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_START);
         {
            breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestInt(json, 42));

            breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

            breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestReal(json, 3.1415926535897932));

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
         }

         breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_START);
         {
            breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestInt(json, 42));

            breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

            breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestReal(json, 3.1415926535897932));

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
         }

         breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
      }

      msg = "Block";
      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
      breakIf(!gjsonIsKeyEqual(json, msg));
      breakIf(gjsonGetType_ObjectValue(json) != gjsonTypeOBJECT_START);
      {
         msg = "Null";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(    json) != gjsonTypeVALUE_NULL);

         msg = "True";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(    json) != gjsonTypeVALUE_TRUE);

         msg = "False";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(    json) != gjsonTypeVALUE_FALSE);

         msg = "I 0";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestInt(json, 0));

         msg = "I 1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestInt(json, 1));

         msg = "I -1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestInt(json, -1));

         msg = "I MAX";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestInt(json, Gi8MAX));

         msg = "I MIN";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeI(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestInt(json, Gi8MIN));

         msg = "N 0";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestNat(json, 0));

         msg = "N 1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestNat(json, 1));

         msg = "N MAX";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeN(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestNat(json, Gn8MAX));

         msg = "R 0";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, 0.0));

         msg = "R 1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, 1.0));

         msg = "R -1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, -1.0));

         msg = "R PI";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, 3.1415926535897932));

         msg = "R EPS";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, Gr8EPSILON));

         msg = "R MAX";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, Gr8MAX));

         msg = "R -MAX";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, -Gr8MAX));

         msg = "R INF";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, Gr8INF));

         msg = "R -INF";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, -Gr8INF));

         msg = "R NAN";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal(json, GrNAN));

         msg = "R4 0";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, 0.0f));

         msg = "R4 1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, 1.0f));

         msg = "R4 -1";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, -1.0f));

         msg = "R4 PI";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, 3.1415926535897932f));

         msg = "R4 EPS";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, Gr4EPSILON));

         msg = "R4 MAX";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, Gr4MAX));

         msg = "R4 -MAX";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, -Gr4MAX));

         msg = "R4 INF";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, Gr4INF));

         msg = "R4 -INF";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, -Gr4INF));

         msg = "R4 NAN";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeR(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestReal4(json, GrNAN));

         msg = "String";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeStr(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestStr(json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./"));

   #if defined(INCLUDE_BIN)
         msg = "Binary";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(!gjsonIsTypeBin(gjsonGetType_ObjectValue(json)));
         breakIf(!_JsonTestBin(json, 3 * 256, _binary));
   #endif

         msg = "Bool Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 100);

            breakIf(
               !((type == gjsonTypeVALUE_TRUE  && _bools[index] == 1) ||
                 (type == gjsonTypeVALUE_FALSE && _bools[index] == 0)));
         }
         breakIf(index != 100);

         msg = "I Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 256);

            breakIf(
               !gjsonIsTypeI(type)        ||
               !gjsonGetI(json, &itemp)   ||
               (Gi8) _narray[index] != itemp);
         }
         breakIf(index != 256);

         msg = "N Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 256);

            breakIf(
               !gjsonIsTypeN(type)        ||
               !gjsonGetN(json, &ntemp)   ||
               _narray[index] != ntemp);
         }
         breakIf(index != 256);

         msg = "R Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 300);

            breakIf(
               !gjsonIsTypeR(type)        ||
               !gjsonGetR(json, &rtemp)   ||
               _reals8[index] != rtemp);
         }
         breakIf(index != 300);

         msg = "R4 Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 300);

            breakIf(
               !gjsonIsTypeR(type)        ||
               !gjsonGetR4(json, &r4temp) ||
               _reals4[index] != r4temp);
         }
         breakIf(index != 300);

         msg = "String Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 10);

            breakIf(
               !gjsonIsTypeStr(type)            ||
               !gjsonGetStr(json, 256, stemp)   ||
               strcmp(stemp, _strings[index]) != 0);
         }
         breakIf(index != 10);

   #if defined(INCLUDE_BIN)
         msg = "Binary Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         loopCount(index)
         {
            type = gjsonGetType_ArrayValueOrStop(json);
            breakIf(type == gjsonTypeARRAY_STOP);
            continueIf(index >= 3);

            breakIf(
               !gjsonIsTypeBin(type) ||
               !_JsonTestBin(json, 3 * 256, _binary));
         }
         breakIf(index != 3);
   #endif

         msg = "User Type IntStrReal";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         {
            breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestInt(json, 42));

            breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

            breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
            breakIf(!_JsonTestReal(json, 3.1415926535897932));

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
         }

         msg = "User Type IntStrReal Array";
         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_KEY);
         breakIf(!gjsonIsKeyEqual(json, msg));
         breakIf(gjsonGetType_ObjectValue(  json) != gjsonTypeARRAY_START);
         {
            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_START);
            {
               breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestInt(json, 42));

               breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

               breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestReal(json, 3.1415926535897932));

               breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
            }

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_START);
            {
               breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestInt(json, 42));

               breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

               breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestReal(json, 3.1415926535897932));

               breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
            }

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_START);
            {
               breakIf(!gjsonIsTypeI(  gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestInt(json, 42));

               breakIf(!gjsonIsTypeStr(gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestStr(json, "Yes, but what is the question?"));

               breakIf(!gjsonIsTypeR(  gjsonGetType_ArrayValueOrStop(json)));
               breakIf(!_JsonTestReal(json, 3.1415926535897932));

               breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
            }

            breakIf(gjsonGetType_ArrayValueOrStop(json) != gjsonTypeARRAY_STOP);
         }

         breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_STOP);
      }

      breakIf(gjsonGetType_ObjectKeyOrStop(json) != gjsonTypeOBJECT_STOP);

      msg    = "NO";
      result = gbTRUE;
   }
   printf("JSON Read: %s ERROR\n", msg);

   gjsonDloc(json);
   fclose(file);

   return result;
}

/**************************************************************************************************
func: _JsonTestWrite
**************************************************************************************************/
static Gb _JsonTestWrite(Gstr const * const fileName)
{
   FILE  *file;
   Gjson *json;
   Gb     result;
   Gi4    index;

   file   = NULL;
   json   = NULL;
   result = gbFALSE;

   breakScope
   {
      breakIf(fopen_s(&file, fileName, "wb") != 0);

      // Create a json file.
      json = gjsonClocWriter(_SetBuffer, (void *) file, gbTRUE);
      breakIf(!json);

      // File Level Element is an unnamed object.
      gjsonSetFileValueObjectStart(json);
      {
         gjsonSetObjectValueNull(   json, "Null");

         gjsonSetObjectValueB(      json, "True",      gbTRUE);
         gjsonSetObjectValueB(      json, "False",     gbFALSE);

         gjsonSetObjectValueI(      json, "I 0",       0);
         gjsonSetObjectValueI(      json, "I 1",       1);
         gjsonSetObjectValueI(      json, "I -1",      -1);
         gjsonSetObjectValueI(      json, "I MAX",     Gi8MAX);
         gjsonSetObjectValueI(      json, "I MIN",     Gi8MIN);

         gjsonSetObjectValueN(      json, "N 0",       0);
         gjsonSetObjectValueN(      json, "N 1",       1);
         gjsonSetObjectValueN(      json, "N MAX",     Gn8MAX);

         gjsonSetObjectValueR(      json, "R 0",       0.0);
         gjsonSetObjectValueR(      json, "R 1",       1.0);
         gjsonSetObjectValueR(      json, "R -1",     -1.0);
         gjsonSetObjectValueR(      json, "R PI",      3.1415926535897932);
         gjsonSetObjectValueR(      json, "R EPS",     Gr8EPSILON);
         gjsonSetObjectValueR(      json, "R MAX",     Gr8MAX);
         gjsonSetObjectValueR(      json, "R -MAX",   -Gr8MAX);
         gjsonSetObjectValueR(      json, "R INF",     Gr8INF);
         gjsonSetObjectValueR(      json, "R -INF",   -Gr8INF);
         gjsonSetObjectValueR(      json, "R NAN",     GrNAN);

         gjsonSetObjectValueR4(     json, "R4 0",      0.0f);
         gjsonSetObjectValueR4(     json, "R4 1",      1.0f);
         gjsonSetObjectValueR4(     json, "R4 -1",    -1.0f);
         gjsonSetObjectValueR4(     json, "R4 PI",     3.1415926535897932f);
         gjsonSetObjectValueR4(     json, "R4 EPS",    Gr4EPSILON);
         gjsonSetObjectValueR4(     json, "R4 MAX",    Gr4MAX);
         gjsonSetObjectValueR4(     json, "R4 -MAX",  -Gr4MAX);
         gjsonSetObjectValueR4(     json, "R4 INF",    Gr4INF);
         gjsonSetObjectValueR4(     json, "R4 -INF",  -Gr4INF);
         gjsonSetObjectValueR4(     json, "R4 NAN",    GrNAN);

         gjsonSetObjectValueStr(    json, "String",    "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

#if defined(INCLUDE_BIN)
         gjsonSetObjectValueBin(    json, "Binary",    3 * 256, _binary);
#endif

         gjsonSetObjectValueArrayStart(json, "Bool Array");
         forCount(index, 100)
         {
            gjsonSetArrayValueB(       json, _bools[index]);
         }
         gjsonSetObjectValueArrayStop( json);

         gjsonSetObjectValueArrayStart(json, "I Array");
         forCount(index, 256)
         {
            gjsonSetArrayValueI(       json, _narray[index]);
         }
         gjsonSetObjectValueArrayStop( json);

         gjsonSetObjectValueArrayStart(json, "N Array");
         forCount(index, 256)
         {
            gjsonSetArrayValueN(       json, _narray[index]);
         }
         gjsonSetObjectValueArrayStop( json);

         gjsonSetObjectValueArrayStart(json, "R Array");
         forCount(index, 300)
         {
            gjsonSetArrayValueR(       json, _reals8[index]);
         }
         gjsonSetObjectValueArrayStop( json);

         gjsonSetObjectValueArrayStart(json, "R4 Array");
         forCount(index, 300)
         {
            gjsonSetArrayValueR4(      json, _reals4[index]);
         }
         gjsonSetObjectValueArrayStop( json);

         gjsonSetObjectValueArrayStart(json, "String Array");
         forCount(index, 10)
         {
            gjsonSetArrayValueStr(     json, _strings[index]);
         }
         gjsonSetObjectValueArrayStop( json);

#if defined(INCLUDE_BIN)
         gjsonSetObjectValueArrayStart(json, "Binary Array");
         forCount(index, 3)
         {
            gjsonSetArrayValueBin(     json, 3 * 256, _binary);
         }
         gjsonSetObjectValueArrayStop( json);
#endif

         gjsonSetObjectValueArrayStart(json, "User Type IntStrReal");
         {
            gjsonSetArrayValueI(       json, 42);
            gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
            gjsonSetArrayValueR(       json, 3.1415926535897932);
         }
         gjsonSetObjectValueArrayStop( json);


         gjsonSetObjectValueArrayStart(json, "User Type IntStrReal Array");
         {
            gjsonSetArrayValueArrayStart( json);
            {
               gjsonSetArrayValueI(       json, 42);
               gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
               gjsonSetArrayValueR(       json, 3.1415926535897932);
            }
            gjsonSetArrayValueArrayStop(  json);

            gjsonSetArrayValueArrayStart( json);
            {
               gjsonSetArrayValueI(       json, 42);
               gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
               gjsonSetArrayValueR(       json, 3.1415926535897932);
            }
            gjsonSetArrayValueArrayStop(  json);

            gjsonSetArrayValueArrayStart( json);
            {
               gjsonSetArrayValueI(       json, 42);
               gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
               gjsonSetArrayValueR(       json, 3.1415926535897932);
            }
            gjsonSetArrayValueArrayStop(  json);
         }
         gjsonSetObjectValueArrayStop( json);

         gjsonSetObjectValueObjectStart(json, "Block");
         {
            gjsonSetObjectValueNull(   json, "Null");

            gjsonSetObjectValueB(      json, "True",      gbTRUE);
            gjsonSetObjectValueB(      json, "False",     gbFALSE);

            gjsonSetObjectValueI(      json, "I 0",       0);
            gjsonSetObjectValueI(      json, "I 1",       1);
            gjsonSetObjectValueI(      json, "I -1",      -1);
            gjsonSetObjectValueI(      json, "I MAX",     Gi8MAX);
            gjsonSetObjectValueI(      json, "I MIN",     Gi8MIN);

            gjsonSetObjectValueN(      json, "N 0",       0);
            gjsonSetObjectValueN(      json, "N 1",       1);
            gjsonSetObjectValueN(      json, "N MAX",     Gn8MAX);

            gjsonSetObjectValueR(      json, "R 0",       0.0);
            gjsonSetObjectValueR(      json, "R 1",       1.0);
            gjsonSetObjectValueR(      json, "R -1",     -1.0);
            gjsonSetObjectValueR(      json, "R PI",      3.1415926535897932);
            gjsonSetObjectValueR(      json, "R EPS",     Gr8EPSILON);
            gjsonSetObjectValueR(      json, "R MAX",     Gr8MAX);
            gjsonSetObjectValueR(      json, "R -MAX",   -Gr8MAX);
            gjsonSetObjectValueR(      json, "R INF",     Gr8INF);
            gjsonSetObjectValueR(      json, "R -INF",   -Gr8INF);
            gjsonSetObjectValueR(      json, "R NAN",     GrNAN);

            gjsonSetObjectValueR4(     json, "R4 0",      0.0f);
            gjsonSetObjectValueR4(     json, "R4 1",      1.0f);
            gjsonSetObjectValueR4(     json, "R4 -1",    -1.0f);
            gjsonSetObjectValueR4(     json, "R4 PI",     3.1415926535897932f);
            gjsonSetObjectValueR4(     json, "R4 EPS",    Gr4EPSILON);
            gjsonSetObjectValueR4(     json, "R4 MAX",    Gr4MAX);
            gjsonSetObjectValueR4(     json, "R4 -MAX",  -Gr4MAX);
            gjsonSetObjectValueR4(     json, "R4 INF",    Gr4INF);
            gjsonSetObjectValueR4(     json, "R4 -INF",  -Gr4INF);
            gjsonSetObjectValueR4(     json, "R4 NAN",    GrNAN);

            gjsonSetObjectValueStr(    json, "String",    "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

#if defined(INCLUDE_BIN)
            gjsonSetObjectValueBin(    json, "Binary",    3 * 256, _binary);
#endif

            gjsonSetObjectValueArrayStart(json, "Bool Array");
            forCount(index, 100)
            {
               gjsonSetArrayValueB(       json, _bools[index]);
            }
            gjsonSetObjectValueArrayStop( json);

            gjsonSetObjectValueArrayStart(json, "I Array");
            forCount(index, 256)
            {
               gjsonSetArrayValueI(       json, _narray[index]);
            }
            gjsonSetObjectValueArrayStop( json);

            gjsonSetObjectValueArrayStart(json, "N Array");
            forCount(index, 256)
            {
               gjsonSetArrayValueN(       json, _narray[index]);
            }
            gjsonSetObjectValueArrayStop( json);

            gjsonSetObjectValueArrayStart(json, "R Array");
            forCount(index, 300)
            {
               gjsonSetArrayValueR(       json, _reals8[index]);
            }
            gjsonSetObjectValueArrayStop( json);

            gjsonSetObjectValueArrayStart(json, "R4 Array");
            forCount(index, 300)
            {
               gjsonSetArrayValueR4(      json, _reals4[index]);
            }
            gjsonSetObjectValueArrayStop( json);

            gjsonSetObjectValueArrayStart(json, "String Array");
            forCount(index, 10)
            {
               gjsonSetArrayValueStr(     json, _strings[index]);
            }
            gjsonSetObjectValueArrayStop( json);

   #if defined(INCLUDE_BIN)
            gjsonSetObjectValueArrayStart(json, "Binary Array");
            forCount(index, 3)
            {
               gjsonSetArrayValueBin(     json, 3 * 256, _binary);
            }
            gjsonSetObjectValueArrayStop( json);
   #endif

            gjsonSetObjectValueArrayStart(json, "User Type IntStrReal");
            {
               gjsonSetArrayValueI(       json, 42);
               gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
               gjsonSetArrayValueR(       json, 3.1415926535897932);
            }
            gjsonSetObjectValueArrayStop( json);


            gjsonSetObjectValueArrayStart(json, "User Type IntStrReal Array");
            {
               gjsonSetArrayValueArrayStart( json);
               {
                  gjsonSetArrayValueI(       json, 42);
                  gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
                  gjsonSetArrayValueR(       json, 3.1415926535897932);
               }
               gjsonSetArrayValueArrayStop(  json);

               gjsonSetArrayValueArrayStart( json);
               {
                  gjsonSetArrayValueI(       json, 42);
                  gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
                  gjsonSetArrayValueR(       json, 3.1415926535897932);
               }
               gjsonSetArrayValueArrayStop(  json);

               gjsonSetArrayValueArrayStart( json);
               {
                  gjsonSetArrayValueI(       json, 42);
                  gjsonSetArrayValueStr(     json, "Yes, but what is the question?");
                  gjsonSetArrayValueR(       json, 3.1415926535897932);
               }
               gjsonSetArrayValueArrayStop(  json);
            }
            gjsonSetObjectValueArrayStop( json);
         }
         gjsonSetObjectValueObjectStop(json);
      }
      gjsonSetFileValueObjectStop(json);

      result = gbTRUE;
   }

   gjsonDloc(json);
   fclose(file);

   return result;
}

/**************************************************************************************************
func: _MiffTestRead
**************************************************************************************************/
static Gb _MiffTestArrayCountValue(Gmiff *miff, Gcount count)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(gmiffValueGetType(      value) != gmiffValueTypeARRAY_COUNT);
   returnFalseIf(gmiffValueGetArrayCount(value) != count);

   returnTrue;
}

static Gb _MiffTestGroupCountValue(Gmiff *miff, Gcount count)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(gmiffValueGetType(      value) != gmiffValueTypeGROUP_COUNT);
   returnFalseIf(gmiffValueGetGroupCount(value) != count);

   returnTrue;
}

static Gb _MiffTestNullValue(Gmiff *miff)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(gmiffValueGetType(value) != gmiffValueTypeNULL);
   returnTrue;
}

static Gb _MiffTestBValue(Gmiff *miff, Gb testValue)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeNUM ||
      gmiffValueGetB(   value) != testValue);

   returnTrue;
}

static Gb _MiffTestBlockStartValue(Gmiff *miff)
{
   return gmiffValueGetType(gmiffGetValue(miff)) != gmiffValueTypeBLOCK_START;
}

static Gb _MiffTestBlockStopValue(Gmiff *miff)
{
   return gmiffValueGetType(gmiffGetValue(miff)) != gmiffValueTypeBLOCK_STOP;
}

static Gb _MiffTestIValue(Gmiff *miff, Gi8 testValue)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeNUM ||
      gmiffValueGetI(   value) != testValue);

   returnTrue;
}

static Gb _MiffTestNValue(Gmiff *miff, Gn8 testValue)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeNUM ||
      gmiffValueGetN(   value) != testValue);

   returnTrue;
}

static Gb _MiffTestRValue(Gmiff *miff, Gr8 testValue)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeNUM ||
      !gmiffValueIsR8(  value));
   returnFalseIf( isnan(testValue) && !isnan(gmiffValueGetR(value)));
   returnFalseIf(!isnan(testValue) && gmiffValueGetR(value) != testValue);

   returnTrue;
}

static Gb _MiffTestR4Value(Gmiff *miff, Gr4 testValue)
{
   GmiffValue value;

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeNUM ||
      !gmiffValueIsR4(  value));
   returnFalseIf( isnan(testValue) && !isnan(gmiffValueGetR4(value)));
   returnFalseIf(!isnan(testValue) && gmiffValueGetR4(value) != testValue);

   returnTrue;
}

static Gb _MiffTestBinValue(Gmiff *miff, Gcount testCount, Gn1 *testValue)
{
   GmiffValue value;
   Gn1    svalue[256 * 3];

   value = gmiffGetValue(miff);
   returnFalseIf(
      gmiffValueGetType(value) != gmiffValueTypeBIN &&
      testCount                != gmiffValueGetBinCount(value));
   returnFalseIf(!gmiffGetValueBin(miff, gmiffValueGetBinCount(value), svalue));
   returnFalseIf(memcmp(svalue, testValue, testCount) != 0);

   returnTrue;
}

static Gb _MiffTestStrValue(Gmiff *miff, Gstr const *testValue)
{
   return gstrIsEqual(gmiffValueGetStr(gmiffGetValue(miff)), testValue);
}

static Gb _MiffTestGetNull(Gmiff *miff, char const *key)
{
   returnFalseIf(!_MiffTestStrValue( miff, key));
   returnFalseIf(!_MiffTestNullValue(miff));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestGetB(Gmiff *miff, char const *key, Gb const value)
{
   returnFalseIf(!_MiffTestStrValue(miff, key));
   returnFalseIf(!_MiffTestBValue(  miff, value));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestGetI(Gmiff *miff, char const *key, Gi8 const value)
{
   returnFalseIf(!_MiffTestStrValue(miff, key));
   returnFalseIf(!_MiffTestIValue(  miff, value));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestGetN(Gmiff *miff, char const *key, Gn8 const value)
{
   returnFalseIf(!_MiffTestStrValue(miff, key));
   returnFalseIf(!_MiffTestNValue(  miff, value));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestGetR(Gmiff *miff, char const *key, Gr8 const value)
{
   returnFalseIf(!_MiffTestStrValue(miff, key));
   returnFalseIf(!_MiffTestRValue(  miff, value));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestGetR4(Gmiff *miff, char const *key, Gr4 const value)
{
   returnFalseIf(!_MiffTestStrValue(miff, key));
   returnFalseIf(!_MiffTestR4Value( miff, value));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestGetStr(Gmiff *miff, char const *key, Gstr const *value)
{
   returnFalseIf(!_MiffTestStrValue(miff, key));
   returnFalseIf(!_MiffTestStrValue(miff, value));

   return gmiffGetRecordStop(miff);
}

static Gb _MiffTestRead(Gstr const * const fileName)
{
   FILE           *file;
   Gmiff          *miff;
   Gb              result;
   Gcount          arrayCount;
   Gindex          index;
   char const     *msg;

   file   = NULL;
   miff   = NULL;
   result = gbFALSE;

   breakScope
   {
      msg = "Open file";
      if (fopen_s(&file, fileName, "rb") != 0) break;

      // Set Gmiff up for reading.
      msg = "Create reader";
      miff = gmiffClocReader(_GetBuffer, (void *) file);
      if (!miff) break;

      msg = "Header";
      breakIf(!gstrIsEqual(gmiffGetSubFormatName(miff), "MiffTestFile"));
      breakIf(gmiffGetSubFormatVersion(miff) != 2)

      // Read in the header information.
      //wprintf(L"%S\t%d\n", (char *) subFormatName, (int) subFormatVersion);

      // Unless you are in full control of the format, do not assume that there will be an order
      // to the values.
      msg = "Null";
      breakIf(!_MiffTestGetNull(miff, msg));

      msg = "True";
      breakIf(!_MiffTestGetB(miff, msg, gbTRUE));

      msg = "False";
      breakIf(!_MiffTestGetB(miff, msg, gbFALSE));

      msg = "I 0";
      breakIf(!_MiffTestGetI(miff, msg, 0));

      msg = "I 1";
      breakIf(!_MiffTestGetI(miff, msg, 1));

      msg = "I -1";
      breakIf(!_MiffTestGetI(miff, msg, -1));

      msg = "I MAX";
      breakIf(!_MiffTestGetI(miff, msg, Gi8MAX));

      msg = "I MIN";
      breakIf(!_MiffTestGetI(miff, msg, Gi8MIN));

      msg = "N 0";
      breakIf(!_MiffTestGetN(miff, msg, 0));

      msg = "N 1";
      breakIf(!_MiffTestGetN(miff, msg, 1));

      msg = "N MAX";
      breakIf(!_MiffTestGetN(miff, msg, Gn8MAX));

      msg = "R 0";
      breakIf(!_MiffTestGetR(miff, msg, 0.0));

      msg = "R 1";
      breakIf(!_MiffTestGetR(miff, msg, 1.0));

      msg = "R -1";
      breakIf(!_MiffTestGetR(miff, msg, -1.0));

      msg = "R PI";
      breakIf(!_MiffTestGetR(miff, msg, 3.1415926535897932));

      msg = "R EPS";
      breakIf(!_MiffTestGetR(miff, msg, Gr8EPSILON));

      msg = "R MAX";
      breakIf(!_MiffTestGetR(miff, msg, Gr8MAX));

      msg = "R -MAX";
      breakIf(!_MiffTestGetR(miff, msg, -Gr8MAX));

      msg = "R INF";
      breakIf(!_MiffTestGetR(miff, msg, Gr8INF));

      msg = "R -INF";
      breakIf(!_MiffTestGetR(miff, msg, -Gr8INF));

      msg = "R NAN";
      breakIf(!_MiffTestGetR(miff, msg, GrNAN));

      msg = "R4 0";
      breakIf(!_MiffTestGetR4(miff, msg, 0.0));

      msg = "R4 1";
      breakIf(!_MiffTestGetR4(miff, msg, 1.0));

      msg = "R4 -1";
      breakIf(!_MiffTestGetR4(miff, msg, -1.0));

      msg = "R4 PI";
      breakIf(!_MiffTestGetR4(miff, msg, 3.1415926535897932f));

      msg = "R4 EPS";
      breakIf(!_MiffTestGetR4(miff, msg, Gr4EPSILON));

      msg = "R4 MAX";
      breakIf(!_MiffTestGetR4(miff, msg, Gr4MAX));

      msg = "R4 -MAX";
      breakIf(!_MiffTestGetR4(miff, msg, -Gr4MAX));

      msg = "R4 INF";
      breakIf(!_MiffTestGetR4(miff, msg, Gr4INF));

      msg = "R4 -INF";
      breakIf(!_MiffTestGetR4(miff, msg, -Gr4INF));

      msg = "R4 NAN";
      breakIf(!_MiffTestGetR4(miff, msg, GrNAN));

      msg = "String";
      breakIf(!_MiffTestGetStr(miff, msg, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./"));

#if defined(INCLUDE_BIN)
      msg = "Binary";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestBinValue(miff, 256 * 3, _binary));
      breakIf(!gmiffGetRecordStop(miff));
#endif

      msg = "Bool Array";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestArrayCountValue(miff, 100));
      forCount(index, arrayCount)
      {
         breakIf(!_MiffTestBValue(miff, _bools[index]));
      }
      breakIf(!gmiffGetRecordStop(miff));

      msg = "I Array";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestArrayCountValue(miff, 256));
      forCount(index, arrayCount)
      {
         breakIf(!_MiffTestIValue(miff, (int) _narray[index]));
      }
      breakIf(!gmiffGetRecordStop(miff));

      msg = "N Array";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestArrayCountValue(miff, 256));
      forCount(index, arrayCount)
      {
         breakIf(!_MiffTestNValue(miff, _narray[index]));
      }
      breakIf(!gmiffGetRecordStop(miff));

      msg = "R Array";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestArrayCountValue(miff, 300));
      forCount(index, arrayCount)
      {
         breakIf(!_MiffTestRValue(miff, _reals8[index]));
      }
      breakIf(!gmiffGetRecordStop(miff));

      msg = "R4 Array";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestArrayCountValue(miff, 300));
      forCount(index, arrayCount)
      {
         breakIf(!_MiffTestR4Value(miff, _reals4[index]));
      }
      breakIf(!gmiffGetRecordStop(miff));

      msg = "String Array";
      breakIf(!_MiffTestStrValue(miff, msg));
      breakIf(!_MiffTestArrayCountValue(miff, 10));
      forCount(index, arrayCount)
      {
         breakIf(!_MiffTestStrValue(miff, _strings[index]));
      }
      breakIf(!gmiffGetRecordStop(miff));

#if defined(INCLUDE_BIN)
      msg = "Binary Array";
      breakIf(!_MiffTestStrValue(         miff, msg));
      breakIf(!_MiffTestArrayCountValue(  miff, 3));
      breakIf(!_MiffTestBinValue(         miff, 3 * 256, _binary));
      breakIf(!_MiffTestBinValue(         miff, 3 * 256, _binary));
      breakIf(!_MiffTestBinValue(         miff, 3 * 256, _binary));
      breakIf(!gmiffGetRecordStop(        miff));
#endif

      msg = "User Type IntStrReal";
      breakIf(!_MiffTestStrValue(         miff, msg));
      breakIf(!_MiffTestGroupCountValue(  miff, 3));
      breakIf(!_MiffTestIValue(           miff, 42));
      breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
      breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
      breakIf(!gmiffGetRecordStop(        miff));

      msg = "User Type IntStrReal Array";
      breakIf(!_MiffTestStrValue(         miff, msg));
      breakIf(!_MiffTestArrayCountValue(  miff, 3));
      breakIf(!_MiffTestGroupCountValue(  miff, 3));
      breakIf(!_MiffTestIValue(           miff, 42));
      breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
      breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
      breakIf(!_MiffTestIValue(           miff, 42));
      breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
      breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
      breakIf(!_MiffTestIValue(           miff, 42));
      breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
      breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
      breakIf(!gmiffGetRecordStop(        miff));

      msg = "Block";
      breakIf(!_MiffTestStrValue(       miff, msg));
      breakIf(!_MiffTestBlockStartValue(miff));
      breakIf(!gmiffGetRecordStop(      miff));
      {
         msg = "Null";
         breakIf(!_MiffTestGetNull(miff, msg));

         msg = "True";
         breakIf(!_MiffTestGetB(miff, msg, gbTRUE));

         msg = "False";
         breakIf(!_MiffTestGetB(miff, msg, gbFALSE));

         msg = "I 0";
         breakIf(!_MiffTestGetI(miff, msg, 0));

         msg = "I 1";
         breakIf(!_MiffTestGetI(miff, msg, 1));

         msg = "I -1";
         breakIf(!_MiffTestGetI(miff, msg, -1));

         msg = "I MAX";
         breakIf(!_MiffTestGetI(miff, msg, Gi8MAX));

         msg = "I MIN";
         breakIf(!_MiffTestGetI(miff, msg, Gi8MIN));

         msg = "N 0";
         breakIf(!_MiffTestGetN(miff, msg, 0));

         msg = "N 1";
         breakIf(!_MiffTestGetN(miff, msg, 1));

         msg = "N MAX";
         breakIf(!_MiffTestGetN(miff, msg, Gn8MAX));

         msg = "R 0";
         breakIf(!_MiffTestGetR(miff, msg, 0.0));

         msg = "R 1";
         breakIf(!_MiffTestGetR(miff, msg, 1.0));

         msg = "R -1";
         breakIf(!_MiffTestGetR(miff, msg, -1.0));

         msg = "R PI";
         breakIf(!_MiffTestGetR(miff, msg, 3.1415926535897932));

         msg = "R EPS";
         breakIf(!_MiffTestGetR(miff, msg, Gr8EPSILON));

         msg = "R MAX";
         breakIf(!_MiffTestGetR(miff, msg, Gr8MAX));

         msg = "R -MAX";
         breakIf(!_MiffTestGetR(miff, msg, -Gr8MAX));

         msg = "R INF";
         breakIf(!_MiffTestGetR(miff, msg, Gr8INF));

         msg = "R -INF";
         breakIf(!_MiffTestGetR(miff, msg, -Gr8INF));

         msg = "R NAN";
         breakIf(!_MiffTestGetR(miff, msg, GrNAN));

         msg = "R4 0";
         breakIf(!_MiffTestGetR4(miff, msg, 0.0));

         msg = "R4 1";
         breakIf(!_MiffTestGetR4(miff, msg, 1.0));

         msg = "R4 -1";
         breakIf(!_MiffTestGetR4(miff, msg, -1.0));

         msg = "R4 PI";
         breakIf(!_MiffTestGetR4(miff, msg, 3.1415926535897932f));

         msg = "R4 EPS";
         breakIf(!_MiffTestGetR4(miff, msg, Gr4EPSILON));

         msg = "R4 MAX";
         breakIf(!_MiffTestGetR4(miff, msg, Gr4MAX));

         msg = "R4 -MAX";
         breakIf(!_MiffTestGetR4(miff, msg, -Gr4MAX));

         msg = "R4 INF";
         breakIf(!_MiffTestGetR4(miff, msg, Gr4INF));

         msg = "R4 -INF";
         breakIf(!_MiffTestGetR4(miff, msg, -Gr4INF));

         msg = "R4 NAN";
         breakIf(!_MiffTestGetR4(miff, msg, GrNAN));

         msg = "String";
         breakIf(!_MiffTestGetStr(miff, msg, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./"));

   #if defined(INCLUDE_BIN)
         msg = "Binary";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 1));
         breakIf(!_MiffTestBinValue(miff, 256 * 3, _binary));
         breakIf(!gmiffGetRecordStop(miff));
   #endif

         msg = "Bool Array";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 100));
         forCount(index, arrayCount)
         {
            breakIf(!_MiffTestBValue(miff, _bools[index]));
         }
         breakIf(!gmiffGetRecordStop(miff));

         msg = "I Array";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 256));
         forCount(index, arrayCount)
         {
            breakIf(!_MiffTestIValue(miff, (int) _narray[index]));
         }
         breakIf(!gmiffGetRecordStop(miff));

         msg = "N Array";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 256));
         forCount(index, arrayCount)
         {
            breakIf(!_MiffTestNValue(miff, _narray[index]));
         }
         breakIf(!gmiffGetRecordStop(miff));

         msg = "R Array";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 300));
         forCount(index, arrayCount)
         {
            breakIf(!_MiffTestRValue(miff, _reals8[index]));
         }
         breakIf(!gmiffGetRecordStop(miff));

         msg = "R4 Array";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 300));
         forCount(index, arrayCount)
         {
            breakIf(!_MiffTestR4Value(miff, _reals4[index]));
         }
         breakIf(!gmiffGetRecordStop(miff));

         msg = "String Array";
         breakIf(!_MiffTestStrValue(miff, msg));
         breakIf(!_MiffTestArrayCountValue(miff, 10));
         forCount(index, arrayCount)
         {
            breakIf(!_MiffTestStrValue(miff, _strings[index]));
         }
         breakIf(!gmiffGetRecordStop(miff));

   #if defined(INCLUDE_BIN)
         msg = "Binary Array";
         breakIf(!_MiffTestStrValue(         miff, msg));
         breakIf(!_MiffTestArrayCountValue(  miff, 3));
         breakIf(!_MiffTestBinValue(         miff, 3 * 256, _binary));
         breakIf(!_MiffTestBinValue(         miff, 3 * 256, _binary));
         breakIf(!_MiffTestBinValue(         miff, 3 * 256, _binary));
         breakIf(!gmiffGetRecordStop(        miff));
   #endif

         msg = "User Type IntStrReal";
         breakIf(!_MiffTestStrValue(         miff, msg));
         breakIf(!_MiffTestGroupCountValue(  miff, 3));
         breakIf(!_MiffTestIValue(           miff, 42));
         breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
         breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
         breakIf(!gmiffGetRecordStop(        miff));

         msg = "User Type IntStrReal Array";
         breakIf(!_MiffTestStrValue(         miff, msg));
         breakIf(!_MiffTestArrayCountValue(  miff, 3));
         breakIf(!_MiffTestGroupCountValue(  miff, 3));
         breakIf(!_MiffTestIValue(           miff, 42));
         breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
         breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
         breakIf(!_MiffTestIValue(           miff, 42));
         breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
         breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
         breakIf(!_MiffTestIValue(           miff, 42));
         breakIf(!_MiffTestStrValue(         miff, "Yes, but what is the question?"));
         breakIf(!_MiffTestRValue(           miff, 3.1415926535897932));
         breakIf(!gmiffGetRecordStop(        miff));
      }

      msg = "Block Stop";
      breakIf(!_MiffTestBlockStopValue(miff));
      breakIf(!gmiffGetRecordStop(miff));

      msg = "NO";
      result = gbTRUE;
   }

   printf("MIFF Read: %s ERROR\n", msg);

   gmiffDloc(miff);
   fclose(file);

   return result;
}

/**************************************************************************************************
func: _MiffTestWrite
**************************************************************************************************/
static Gb _MiffTestWrite(Gstr const * const fileName)
{
   FILE  *file;
   Gmiff *miff;
   Gb     result;
   Gindex index;
   Gcount count;

   file   = NULL;
   miff   = NULL;
   result = gbFALSE;

   breakScope
   {
      if (fopen_s(&file, fileName, "wb") != 0)
      {
         break;
      }

      // Create a miff file.
      miff = gmiffClocWriter(_SetBuffer, (void *) file, "MiffTestFile", 2);
      if (!miff)
      {
         break;
      }

      gmiffRecordSetNull(           miff, "Null");

      gmiffRecordSetB(              miff, "True",     gbTRUE);
      gmiffRecordSetB(              miff, "False",    gbFALSE);

      gmiffRecordSetI(              miff, "I 0",      0);
      gmiffRecordSetI(              miff, "I 1",      1);
      gmiffRecordSetI(              miff, "I -1",     -1);
      gmiffRecordSetI(              miff, "I MAX",    Gi8MAX);
      gmiffRecordSetI(              miff, "I MIN",    Gi8MIN);

      gmiffRecordSetN(              miff, "N 0",      0);
      gmiffRecordSetN(              miff, "N 1",      1);
      gmiffRecordSetN(              miff, "N MAX",    Gn8MAX);

      gmiffRecordSetR(              miff, "R 0",     0.0);
      gmiffRecordSetR(              miff, "R 1",     1.0);
      gmiffRecordSetR(              miff, "R -1",    -1.0);
      gmiffRecordSetR(              miff, "R PI",    3.1415926535897932);
      gmiffRecordSetR(              miff, "R EPS",   Gr8EPSILON);
      gmiffRecordSetR(              miff, "R MAX",   Gr8MAX);
      gmiffRecordSetR(              miff, "R -MAX",  -Gr8MAX);
      gmiffRecordSetR(              miff, "R INF",   Gr8INF);
      gmiffRecordSetR(              miff, "R -INF",  -Gr8INF);
      gmiffRecordSetR(              miff, "R NAN",   GrNAN);

      gmiffRecordSetR4(             miff, "R4 0",    0.0);
      gmiffRecordSetR4(             miff, "R4 1",    1.0);
      gmiffRecordSetR4(             miff, "R4 -1",   -1.0);
      gmiffRecordSetR4(             miff, "R4 PI",   3.1415926535897932f);
      gmiffRecordSetR4(             miff, "R4 EPS",  Gr4EPSILON);
      gmiffRecordSetR4(             miff, "R4 MAX",  Gr4MAX);
      gmiffRecordSetR4(             miff, "R4 -MAX", -Gr4MAX);
      gmiffRecordSetR4(             miff, "R4 INF",  Gr4INF);
      gmiffRecordSetR4(             miff, "R4 -INF", -Gr4INF);
      gmiffRecordSetR4(             miff, "R4 NAN",  GrNAN);


      // Usually for larger than memory or stream writing.
      //gmiffRecordSetStr(            miff, "String",  "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
      count = gstrGetCount("The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
      gmiffSetValueStrFinite(      miff, "String");
      gmiffSetValueStrStart(       miff, count);
      forCount(index, count)
      {
         gmiffSetValueStrData(
            miff,
            "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./"[index]);
      }
      gmiffSetValueStrStop(        miff);
      gmiffSetRecordStop(          miff);

#if defined(INCLUDE_BIN)
      // Usually for larger than memory or stream writing.
      //gmiffRecordSetBin(      miff, "Binary",  256 * 3, _binary);
      gmiffSetValueStrFinite(       miff, "Binary");
      gmiffSetValueBinStart(        miff, 256 * 3);
      forCount(index, 256 * 3)
      {
         gmiffSetValueBinData(      miff, _binary[index]);
      }
      gmiffSetValueBinStop(         miff);
      gmiffSetRecordStop(           miff);
#endif

      gmiffRecordSetBArray(         miff, "Bool Array",   100,  _bools);

      gmiffRecordSetIArray(         miff, "I Array",      256,  (Gi8 *) _narray);
      gmiffRecordSetNArray(         miff, "N Array",      256,  _narray);

      gmiffRecordSetRArray(         miff, "R Array",      300,  _reals8);
      gmiffRecordSetR4Array(        miff, "R4 Array",     300,  _reals4);

      gmiffSetValueStrFinite(       miff, "String Array");
      gmiffSetValueArrayCount(      miff, 10);
      forCount(index, 10)
      {
         gmiffSetValueStrFinite(    miff, _strings[index]);
      }
      gmiffSetRecordStop(           miff);

#if defined(INCLUDE_BIN)
      gmiffSetValueStrFinite(       miff, "Binary Array");
      gmiffSetValueArrayCount(      miff, 3);
      gmiffSetValueBin(             miff, 3 * 256, _binary);
      gmiffSetValueBin(             miff, 3 * 256, _binary);

      // Usually for larger than memory or stream writing.
      gmiffSetValueBinStart(        miff, 3 * 256);
      forCount(index, 256 * 3)
      {
         gmiffSetValueBinData(      miff, _binary[index]);
      }
      gmiffSetValueBinStop(         miff);
      gmiffSetRecordStop(           miff);
#endif

      gmiffSetValueStrFinite(       miff, "User Type IntStrReal");
      gmiffSetValueGroupCount(      miff, 3);
      gmiffSetValueI(               miff, 42);
      gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
      gmiffSetValueR(               miff, 3.1415926535897932);
      gmiffSetRecordStop(           miff);

      gmiffSetValueStrFinite(       miff, "User Type IntStrReal Array");
      gmiffSetValueArrayCount(      miff, 3);
      gmiffSetValueGroupCount(      miff, 3);

      gmiffSetValueI(               miff, 42);
      gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
      gmiffSetValueR(               miff, 3.1415926535897932);

      gmiffSetValueI(               miff, 42);
      gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
      gmiffSetValueR(               miff, 3.1415926535897932);

      gmiffSetValueI(               miff, 42);
      // Usually for larger than memory or stream writing.
      count = gstrGetCount("Yes, but what is the question?");
      gmiffSetValueStrStart(        miff, count);
      forCount(index, count)
      {
         gmiffSetValueStrData(      miff, "Yes, but what is the question?"[index]);
      }
      gmiffSetValueStrStop(         miff);
      gmiffSetValueR(               miff, 3.1415926535897932);
      gmiffSetRecordStop(           miff);

      gmiffRecordSetBlockStart(     miff, "Block");
      {
         gmiffRecordSetNull(           miff, "Null");

         gmiffRecordSetB(              miff, "True",     gbTRUE);
         gmiffRecordSetB(              miff, "False",    gbFALSE);

         gmiffRecordSetI(              miff, "I 0",      0);
         gmiffRecordSetI(              miff, "I 1",      1);
         gmiffRecordSetI(              miff, "I -1",     -1);
         gmiffRecordSetI(              miff, "I MAX",    Gi8MAX);
         gmiffRecordSetI(              miff, "I MIN",    Gi8MIN);

         gmiffRecordSetN(              miff, "N 0",      0);
         gmiffRecordSetN(              miff, "N 1",      1);
         gmiffRecordSetN(              miff, "N MAX",    Gn8MAX);

         gmiffRecordSetR(              miff, "R 0",      0.0);
         gmiffRecordSetR(              miff, "R 1",      1.0);
         gmiffRecordSetR(              miff, "R -1",     -1.0);
         gmiffRecordSetR(              miff, "R PI",     3.1415926535897932);
         gmiffRecordSetR(              miff, "R EPS",    Gr8EPSILON);
         gmiffRecordSetR(              miff, "R MAX",    Gr8MAX);
         gmiffRecordSetR(              miff, "R -MAX",   -Gr8MAX);
         gmiffRecordSetR(              miff, "R INF",    Gr8INF);
         gmiffRecordSetR(              miff, "R -INF",   -Gr8INF);
         gmiffRecordSetR(              miff, "R NAN",    GrNAN);

         gmiffRecordSetR4(             miff, "R4 0",     0.0);
         gmiffRecordSetR4(             miff, "R4 1",     1.0);
         gmiffRecordSetR4(             miff, "R4 -1",    -1.0);
         gmiffRecordSetR4(             miff, "R4 PI",    3.1415926535897932f);
         gmiffRecordSetR4(             miff, "R4 EPS",   Gr4EPSILON);
         gmiffRecordSetR4(             miff, "R4 MAX",   Gr4MAX);
         gmiffRecordSetR4(             miff, "R4 -MAX",  -Gr4MAX);
         gmiffRecordSetR4(             miff, "R4 INF",   Gr4INF);
         gmiffRecordSetR4(             miff, "R4 -INF",  -Gr4INF);
         gmiffRecordSetR4(             miff, "R4 NAN",   GrNAN);

         gmiffRecordSetStr(            miff, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
#if defined(INCLUDE_BIN)
         gmiffRecordSetBin(            miff, "Binary",   256 * 3, _binary);
#endif

         gmiffRecordSetBArray(         miff, "Bool Array",   100,  _bools);

         gmiffRecordSetIArray(         miff, "I Array",      256,  (Gi8 *) _narray);
         gmiffRecordSetNArray(         miff, "N Array",      256,  _narray);

         gmiffRecordSetRArray(         miff, "R Array",      300,  _reals8);
         gmiffRecordSetR4Array(        miff, "R4 Array",     300,  _reals4);

         gmiffSetValueStrFinite(       miff, "String Array");
         gmiffSetValueArrayCount(       miff, 10);
         forCount(index, 10)
         {
            gmiffSetValueStrFinite(    miff, _strings[index]);
         }
         gmiffSetRecordStop(           miff);

#if defined(INCLUDE_BIN)
         gmiffSetValueStrFinite(       miff, "Binary Array");
         gmiffSetValueArrayCount(      miff, 3);
         gmiffSetValueBin(             miff, 3 * 256, _binary);
         gmiffSetValueBin(             miff, 3 * 256, _binary);
         gmiffSetValueBin(             miff, 3 * 256, _binary);
         gmiffSetRecordStop(           miff);
#endif

         gmiffSetValueStrFinite(       miff, "User Type IntStrReal");
         gmiffSetValueGroupCount(      miff, 3);
         gmiffSetValueI(               miff, 42);
         gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
         gmiffSetValueR(               miff, 3.1415926535897932);
         gmiffSetRecordStop(           miff);

         gmiffSetValueStrFinite(       miff, "User Type IntStrReal Array");
         gmiffSetValueArrayCount(      miff, 3);
         gmiffSetValueGroupCount(      miff, 3);

         gmiffSetValueI(               miff, 42);
         gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
         gmiffSetValueR(               miff, 3.1415926535897932);

         gmiffSetValueI(               miff, 42);
         gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
         gmiffSetValueR(               miff, 3.1415926535897932);

         gmiffSetValueI(               miff, 42);
         gmiffSetValueStrFinite(       miff, "Yes, but what is the question?");
         gmiffSetValueR(               miff, 3.1415926535897932);
         gmiffSetRecordStop(           miff);
      }
      gmiffRecordSetBlockStop(miff);

      result = gbTRUE;
   }

   gmiffDloc(miff);
   fclose(file);

   return result;
}
