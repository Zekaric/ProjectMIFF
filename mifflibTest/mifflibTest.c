/******************************************************************************
file:       MiffLibTest
author:     Robbert de Groot
company:    Zekaric
copyright:  2021, Zekaric

description:
Test program for MiffLib
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>

//#include "zlib.h"
#include "mifflib.h"
#include "jsonlib.h"

/******************************************************************************
local:
macro
******************************************************************************/
#if 1
#define INCLUDE_BIN
#endif

#define streq(S1, S2)  (strcmp(S1, S2) == 0)

/******************************************************************************
variable:
******************************************************************************/
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

/******************************************************************************
prototype:
******************************************************************************/
static Gb    _JsonGetBuffer(     void * const dataRepo, Gn4 const byteCount, Gn1       * const byteData);
static void    *_JsonMemCreate(     Gn4 const memByteCount);
static void     _JsonMemDestroy(    void * const mem);
static Gb    _JsonTestRead(      Gstr const * const fileName);
static Gb    _JsonTestReadObject(Gjson * const json);
static Gb    _JsonSetBuffer(     void * const dataRepo, Gn4 const byteCount, Gn1 const * const byteData);
static Gb    _JsonTestWrite(     Gstr const * const fileName);

static Gb    _MiffGetBuffer(     void * const dataRepo, Gn4 const byteCount, Gn1       * const byteData);
static void    *_MiffMemCreate(     Gn4 const memByteCount);
static void     _MiffMemDestroy(    void * const mem);
static Gb    _MiffSetBuffer(     void * const dataRepo, Gn4 const byteCount, Gn1 const * const byteData);
static Gb    _MiffTestRead(      Gstr const * const fileName);
static Gb    _MiffTestWrite(     Gstr const * const fileName);

/******************************************************************************
global:
function:
******************************************************************************/
/******************************************************************************
func: main
******************************************************************************/
int main(int acount, char **alist)
{
   int   result;

   acount; alist;

   result = 0;

   // Start miff library.
   if (!miffStart(_MiffMemCreate, _MiffMemDestroy) ||
       !jsonStart(_JsonMemCreate, _JsonMemDestroy))
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
   miffStop();
   jsonStop();

   return result;
}

/******************************************************************************
local:
function:
******************************************************************************/
/******************************************************************************
func: _GetBuffer
******************************************************************************/
static Gb _JsonGetBuffer(void * const dataRepo, Gn4 const byteCount, Gn1 * const byteData)
{
   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

static Gb _MiffGetBuffer(void * const dataRepo, Gn4 const byteCount, Gn1 * const byteData)
{
   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _MemCreate
******************************************************************************/
void *_JsonMemCreate(Gn4 const memByteCount)
{
   return calloc(1, (size_t) memByteCount);
}

void *_MiffMemCreate(Gn4 const memByteCount)
{
   return calloc(1, (size_t) memByteCount);
}

/******************************************************************************
func: _MemDestroy
******************************************************************************/
void _JsonMemDestroy(void * const mem)
{
   free(mem);
}

void _MiffMemDestroy(void * const mem)
{
   free(mem);
}

/******************************************************************************
func: _SetBuffer
******************************************************************************/
static Gb _JsonSetBuffer(void * const dataRepo, Gn4 const byteCount, Gn1 const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

static Gb _MiffSetBuffer(void * const dataRepo, Gn4 const byteCount, Gn1 const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _JsonTestRead
******************************************************************************/
static Gb _JsonTestKey(Gjson * const JSON, char const * const KEY)
{
   Gstr key[32];

   if (jsonGetTypeObj(JSON) != jsonTypeSTRING_START)                          return gbFALSE;
   if (!(jsonGetStr(  JSON, 32, key) && strcmp(key, KEY) == 0))               return gbFALSE;
   if (jsonGetTypeObj(JSON) != jsonTypeKEY_VALUE_SEPARATOR)                   return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestInt(Gjson * const JSON, Gi8 const VALUE)
{
   Gi8 itemp;

   if (jsonGetTypeObj(JSON) != jsonTypeNUMBER_INTEGER)                        return gbFALSE;
   if (!jsonGetI(     JSON, &itemp))                                          return gbFALSE;
   if (itemp != VALUE)                                                        return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestNat(Gjson * const JSON, Gn8 const VALUE)
{
   GjsonType type;
   Gn8    ntemp;

   type = jsonGetTypeObj(JSON);
   if (!(type == jsonTypeNUMBER_INTEGER || type == jsonTypeNUMBER_NATURAL))   return gbFALSE;
   if (!jsonGetN(      JSON, &ntemp))                                         return gbFALSE;
   if (ntemp != VALUE)                                                        return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestReal(Gjson * const JSON, Gr8 VALUE)
{
   GjsonType        type;
   Gr8           rtemp;
   GjsonStrLetter   jletter;
   int             sindex;
   Gstr         sletter,
                   stemp[256];

   type = jsonGetTypeObj(JSON);

   // potential for a constant
   if (type == jsonTypeSTRING_START)
   {
      for (sindex = 0; ; sindex++)
      {
         jletter = jsonGetStrLetter(JSON, &sletter);
         if (jletter == jsonStrLetterDONE)  break;
         if (jletter == jsonStrLetterERROR) break;
         stemp[sindex] = sletter;
      }
      stemp[sindex] = 0;

      if      (strcmp(stemp, "Infinity") == 0)
      {
         rtemp = HUGE_VAL;
      }
      else if (strcmp(stemp, "-Infinity") == 0)
      {
         rtemp = -HUGE_VAL;
      }
      else if (strcmp(stemp, "NaN") == 0)
      {
         rtemp = NAN;
      }
      else                                                                    return gbFALSE;

      goto CHECK_VAL;
   }

   if (!(type == jsonTypeNUMBER_REAL    ||
         type == jsonTypeNUMBER_INTEGER ||
         type == jsonTypeNUMBER_NATURAL))                                     return gbFALSE;
   if (!jsonGetR(JSON, &rtemp))                                               return gbFALSE;

CHECK_VAL:
   if ( isnan(VALUE) && !isnan(rtemp))                                        return gbFALSE;
   if (!isnan(VALUE) && rtemp != VALUE)                                       return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestReal4(Gjson * const JSON, Gr4 VALUE)
{
   GjsonType        type;
   Gr4          r4temp;
   GjsonStrLetter   jletter;
   int             sindex;
   Gstr         sletter,
                   stemp[256];

   type = jsonGetTypeObj(JSON);

   // potential for a constant
   if (type == jsonTypeSTRING_START)
   {
      for (sindex = 0; ; sindex++)
      {
         jletter = jsonGetStrLetter(JSON, &sletter);
         if (jletter == jsonStrLetterDONE)  break;
         if (jletter == jsonStrLetterERROR) break;
         stemp[sindex] = sletter;
      }
      stemp[sindex] = 0;

      if      (strcmp(stemp, "Infinity") == 0)
      {
         r4temp = HUGE_VAL;
      }
      else if (strcmp(stemp, "-Infinity") == 0)
      {
         r4temp = -HUGE_VAL;
      }
      else if (strcmp(stemp, "NaN") == 0)
      {
         r4temp = NAN;
      }
      else                                                                    return gbFALSE;

      goto CHECK_VAL;
   }

   if (!(type == jsonTypeNUMBER_REAL    ||
         type == jsonTypeNUMBER_INTEGER ||
         type == jsonTypeNUMBER_NATURAL))                                     return gbFALSE;
   if (!jsonGetR4(     JSON, &r4temp))                                        return gbFALSE;

CHECK_VAL:
   if ( isnan(VALUE) && !isnan(r4temp))                                       return gbFALSE;
   if (!isnan(VALUE) && r4temp != VALUE)                                      return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestStr(Gjson * const JSON, char const * const VALUE)
{
   int            sindex;
   GjsonStrLetter  jletter;
   Gstr        sletter,
                  stemp[256];

   if (jsonGetTypeObj(JSON) != jsonTypeSTRING_START)                          return gbFALSE;
   for (sindex = 0; ; sindex++)
   {
      jletter = jsonGetStrLetter(JSON, &sletter);
      if (jletter == jsonStrLetterDONE)  break;
      if (jletter == jsonStrLetterERROR) break;
      stemp[sindex] = sletter;
   }
   if (jletter == jsonStrLetterERROR)                                         return gbFALSE;

   stemp[sindex] = 0;
   if (strcmp(stemp, VALUE) != 0)                                             return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestBin(Gjson * const JSON, int binCount, Gn1 const * const binData)
{
   int    bindex;
   Gn1 byte;
   Gn1 binBuffer[3*256];


   if (jsonGetTypeObj(JSON) != jsonTypeSTRING_START)                          return gbFALSE;
   for (bindex = 0; bindex < binCount; bindex++)
   {
      if (jsonGetStrBinByte(JSON, &byte) != jsonStrLetterNORMAL)              return gbFALSE;
      binBuffer[bindex] = byte;
   }
   if (jsonGetStrLetter(JSON, (Gstr *) &byte) != jsonStrLetterDONE)        return gbFALSE;

   if (memcmp(binBuffer, binData, binCount) != 0)                             return gbFALSE;
   return gbTRUE;
}

static Gb _JsonTestGetNull(Gjson * const json, char const * const key)
{
   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeCONSTANT_NULL)                         return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)                             return gbFALSE;

   return gbTRUE;
}

static Gb _JsonTestGetBool(Gjson * const json, char const * const key, Gb const value)
{
   GjsonType type;

   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   type = jsonGetTypeObj(json);
   if (value == gbTRUE  && type != jsonTypeCONSTANT_TRUE)                   return gbFALSE;
   if (value == gbFALSE && type != jsonTypeCONSTANT_FALSE)                  return gbFALSE;
   if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)                           return gbFALSE;

   return gbTRUE;
}

static Gb _JsonTestGetInt(Gjson * const json, char const * const key, Gi8 const value)
{
   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   if (!_JsonTestInt(json, value))                                            return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)                             return gbFALSE;

   return gbTRUE;
}

static Gb _JsonTestGetNat(Gjson * const json, char const * const key, Gn8 const value)
{
   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   if (!_JsonTestNat(json, value))                                            return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)                             return gbFALSE;

   return gbTRUE;
}

static Gb _JsonTestGetReal(Gjson * const json, char const * const key, Gr8 const value)
{
   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   if (!_JsonTestReal(json, value))                                           return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)                             return gbFALSE;

   return gbTRUE;
}

static Gb _JsonTestGetReal4(Gjson * const json, char const * const key, Gr4 const value)
{
   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   if (!_JsonTestReal4(json, value))                                          return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)                             return gbFALSE;

   return gbTRUE;
}

static Gb _JsonTestGetStr(Gjson * const json, char const * const key, Gstr const * const value)
{
   if (!_JsonTestKey(json, key))                                              return gbFALSE;
   if (!_JsonTestStr(json, value))                                            return gbFALSE;
   if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)                             return gbFALSE;

   return gbTRUE;
}

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

   for (;;)
   {
      msg = "Open file";
      if (fopen_s(&file, fileName, "rb") != 0) break;

      // Set Gjson up for reading.
      msg = "Create reader";
      json = jsonCreateReader(_JsonGetBuffer, (void *) file);
      if (!json) break;

      // Start of an object.
      msg = "Looking for object start";
      if (jsonGetTypeFile( json) != jsonTypeOBJECT_START)      break;

      msg = "Null";
      if (!_JsonTestGetNull(json, msg))                        break;

      msg = "True";
      if (!_JsonTestGetBool(json, msg, gbTRUE))              break;

      msg = "False";
      if (!_JsonTestGetBool(json, msg, gbFALSE))             break;

      msg = "I 0";
      if (!_JsonTestGetInt(json, msg, 0))                      break;

      msg = "I 1";
      if (!_JsonTestGetInt(json, msg, 1))                      break;

      msg = "I -1";
      if (!_JsonTestGetInt(json, msg, -1))                     break;

      msg = "I MAX";
      if (!_JsonTestGetInt(json, msg, JsonI_MAX))              break;

      msg = "I MIN";
      if (!_JsonTestGetInt(json, msg, JsonI_MIN))              break;

      msg = "N 0";
      if (!_JsonTestGetNat(json, msg, 0))                      break;

      msg = "N 1";
      if (!_JsonTestGetNat(json, msg, 1))                      break;

      msg = "N MAX";
      if (!_JsonTestGetNat(json, msg, JsonN_MAX))              break;

      msg = "R 0";
      if (!_JsonTestGetReal(json, msg, 0.0))                   break;

      msg = "R 1";
      if (!_JsonTestGetReal(json, msg, 1.0))                   break;

      msg = "R -1";
      if (!_JsonTestGetReal(json, msg, -1.0))                  break;

      msg = "R PI";
      if (!_JsonTestGetReal(json, msg, 3.1415926535897932))    break;

      msg = "R EPS";
      if (!_JsonTestGetReal(json, msg, JsonR_EPSILON))         break;

      msg = "R MAX";
      if (!_JsonTestGetReal(json, msg, JsonR_MAX))             break;

      msg = "R -MAX";
      if (!_JsonTestGetReal(json, msg, -JsonR_MAX))            break;

      msg = "R INF";
      if (!_JsonTestGetReal(json, msg, HUGE_VAL))              break;

      msg = "R -INF";
      if (!_JsonTestGetReal(json, msg, -HUGE_VAL))             break;

      msg = "R NAN";
      if (!_JsonTestGetReal(json, msg, NAN))                   break;

      msg = "R4 0";
      if (!_JsonTestGetReal4(json, msg, 0.0f))                 break;

      msg = "R4 1";
      if (!_JsonTestGetReal4(json, msg, 1.0f))                 break;

      msg = "R4 -1";
      if (!_JsonTestGetReal4(json, msg, -1.0f))                break;

      msg = "R4 PI";
      if (!_JsonTestGetReal4(json, msg, 3.1415926535897932f))  break;

      msg = "R4 EPS";
      if (!_JsonTestGetReal4(json, msg, JsonR4_EPSILON))       break;

      msg = "R4 MAX";
      if (!_JsonTestGetReal4(json, msg, JsonR4_MAX))           break;

      msg = "R4 -MAX";
      if (!_JsonTestGetReal4(json, msg, -JsonR4_MAX))          break;

      msg = "R4 INF";
      if (!_JsonTestGetReal4(json, msg, HUGE_VALF))            break;

      msg = "R4 -INF";
      if (!_JsonTestGetReal4(json, msg, -HUGE_VALF))           break;

      msg = "R4 NAN";
      if (!_JsonTestGetReal4(json, msg, NAN))                  break;

      msg = "String";
      if (!_JsonTestGetStr(json, msg, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;

#if defined(INCLUDE_BIN)
      msg = "Binary";
      if (!_JsonTestKey(  json, msg))                      break;
      if (!_JsonTestBin(  json, 3 * 256, _binary))         break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
#endif

      msg = "Bool Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 100; index++)
      {
         type = jsonGetTypeElem(json);
         if (!((type == jsonTypeCONSTANT_TRUE  && _bools[index] == 1) ||
               (type == jsonTypeCONSTANT_FALSE && _bools[index] == 0)))
            break;

         type = jsonGetTypeElem(json);
         if ((index <  99 && type != jsonTypeSEPARATOR) ||
             (index == 99 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 100)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "I Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 256; index++)
      {
         type = jsonGetTypeElem(json);
         if (type != jsonTypeNUMBER_INTEGER ||
             !jsonGetI(json, &itemp)        ||
             (Gi8) _narray[index] != itemp)
            break;

         type = jsonGetTypeElem(json);
         if ((index <  255 && type != jsonTypeSEPARATOR) ||
             (index == 255 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 256)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "N Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 256; index++)
      {
         type = jsonGetTypeElem(json);
         if (!(type == jsonTypeNUMBER_NATURAL ||
               type == jsonTypeNUMBER_INTEGER)  ||
             !jsonGetN(json, &ntemp)            ||
             _narray[index] != ntemp)
            break;

         type = jsonGetTypeElem(json);
         if ((index <  255 && type != jsonTypeSEPARATOR) ||
             (index == 255 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 256)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 300; index++)
      {
         type = jsonGetTypeElem(json);
         if (!(type == jsonTypeNUMBER_REAL    ||
               type == jsonTypeNUMBER_INTEGER ||
               type == jsonTypeNUMBER_NATURAL)  ||
             !jsonGetR(json, &rtemp)            ||
             _reals8[index] != rtemp)
            break;

         type = jsonGetTypeElem(json);
         if ((index <  299 && type != jsonTypeSEPARATOR) ||
             (index == 299 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 300)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "R4 Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 300; index++)
      {
         type = jsonGetTypeElem(json);
         if (!(type == jsonTypeNUMBER_REAL    ||
               type == jsonTypeNUMBER_INTEGER ||
               type == jsonTypeNUMBER_NATURAL)  ||
             !jsonGetR4(json, &r4temp)          ||
             _reals4[index] != r4temp)
            break;

         type = jsonGetTypeElem(json);
         if ((index <  299 && type != jsonTypeSEPARATOR) ||
             (index == 299 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 300)                                     break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

      msg = "String Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 10; index++)
      {
         type = jsonGetTypeElem(json);
         if (type != jsonTypeSTRING_START  ||
             !jsonGetStr(json, 256, stemp) ||
             strcmp(stemp, _strings[index]) != 0)
            break;

         type = jsonGetTypeElem(json);
         if ((index <  9 && type != jsonTypeSEPARATOR) ||
             (index == 9 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 10)                                      break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

#if defined(INCLUDE_BIN)
      msg = "Binary Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      for (index = 0; index < 3; index++)
      {
         if (!_JsonTestBin(json, 3 * 256, _binary))        break;

         type = jsonGetTypeElem(json);
         if ((index <  2 && type != jsonTypeSEPARATOR) ||
             (index == 2 && type != jsonTypeARRAY_STOP))
            break;
      }
      if (index != 3)                                       break;
      if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
#endif

      msg = "User Type IntStrReal";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      {
         if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
         if (!jsonGetI(json, &itemp))                       break;
         if (itemp != 42)                                   break;

         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

         if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
         if (!jsonGetStr(json, 256, stemp))                 break;
         if (strcmp(stemp, "Yes, but what is the question?"))  break;

         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

         if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
         if (!jsonGetR(json, &rtemp))                       break;
         if (rtemp != 3.1415926535897932)                   break;

         if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
      }
      if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)        break;

      msg = "User Type IntStrReal Array";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
      {
         if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
         if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
         if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
      }
      if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)        break;

      msg = "Block";
      if (!_JsonTestKey(  json, msg))                      break;
      if (jsonGetTypeObj(json) != jsonTypeOBJECT_START)     break;
      {
         msg = "Null";
         if (!_JsonTestGetNull(json, msg))                        break;

         msg = "True";
         if (!_JsonTestGetBool(json, msg, gbTRUE))              break;

         msg = "False";
         if (!_JsonTestGetBool(json, msg, gbFALSE))             break;

         msg = "I 0";
         if (!_JsonTestGetInt(json, msg, 0))                      break;

         msg = "I 1";
         if (!_JsonTestGetInt(json, msg, 1))                      break;

         msg = "I -1";
         if (!_JsonTestGetInt(json, msg, -1))                     break;

         msg = "I MAX";
         if (!_JsonTestGetInt(json, msg, JsonI_MAX))              break;

         msg = "I MIN";
         if (!_JsonTestGetInt(json, msg, JsonI_MIN))              break;

         msg = "N 0";
         if (!_JsonTestGetNat(json, msg, 0))                      break;

         msg = "N 1";
         if (!_JsonTestGetNat(json, msg, 1))                      break;

         msg = "N MAX";
         if (!_JsonTestGetNat(json, msg, JsonN_MAX))              break;

         msg = "R 0";
         if (!_JsonTestGetReal(json, msg, 0.0))                   break;

         msg = "R 1";
         if (!_JsonTestGetReal(json, msg, 1.0))                   break;

         msg = "R -1";
         if (!_JsonTestGetReal(json, msg, -1.0))                  break;

         msg = "R PI";
         if (!_JsonTestGetReal(json, msg, 3.1415926535897932))    break;

         msg = "R EPS";
         if (!_JsonTestGetReal(json, msg, JsonR_EPSILON))         break;

         msg = "R MAX";
         if (!_JsonTestGetReal(json, msg, JsonR_MAX))             break;

         msg = "R -MAX";
         if (!_JsonTestGetReal(json, msg, -JsonR_MAX))            break;

         msg = "R INF";
         if (!_JsonTestGetReal(json, msg, HUGE_VAL))              break;

         msg = "R -INF";
         if (!_JsonTestGetReal(json, msg, -HUGE_VAL))             break;

         msg = "R NAN";
         if (!_JsonTestGetReal(json, msg, NAN))                   break;

         msg = "R4 0";
         if (!_JsonTestGetReal4(json, msg, 0.0f))                 break;

         msg = "R4 1";
         if (!_JsonTestGetReal4(json, msg, 1.0f))                 break;

         msg = "R4 -1";
         if (!_JsonTestGetReal4(json, msg, -1.0f))                break;

         msg = "R4 PI";
         if (!_JsonTestGetReal4(json, msg, 3.1415926535897932f))  break;

         msg = "R4 EPS";
         if (!_JsonTestGetReal4(json, msg, JsonR4_EPSILON))       break;

         msg = "R4 MAX";
         if (!_JsonTestGetReal4(json, msg, JsonR4_MAX))           break;

         msg = "R4 -MAX";
         if (!_JsonTestGetReal4(json, msg, -JsonR4_MAX))          break;

         msg = "R4 INF";
         if (!_JsonTestGetReal4(json, msg, HUGE_VALF))            break;

         msg = "R4 -INF";
         if (!_JsonTestGetReal4(json, msg, -HUGE_VALF))           break;

         msg = "R4 NAN";
         if (!_JsonTestGetReal4(json, msg, NAN))                  break;

         msg = "String";
         if (!_JsonTestGetStr(json, msg, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;

   #if defined(INCLUDE_BIN)
         msg = "Binary";
         if (!_JsonTestKey(  json, msg))                      break;
         if (!_JsonTestBin(  json, 3 * 256, _binary))         break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
   #endif

         msg = "Bool Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 100; index++)
         {
            type = jsonGetTypeElem(json);
            if (!((type == jsonTypeCONSTANT_TRUE  && _bools[index] == 1) ||
                  (type == jsonTypeCONSTANT_FALSE && _bools[index] == 0)))
               break;

            type = jsonGetTypeElem(json);
            if ((index <  99 && type != jsonTypeSEPARATOR) ||
                (index == 99 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 100)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "I Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 256; index++)
         {
            type = jsonGetTypeElem(json);
            if (type != jsonTypeNUMBER_INTEGER ||
                !jsonGetI(json, &itemp)        ||
                (Gi8) _narray[index] != itemp)
               break;

            type = jsonGetTypeElem(json);
            if ((index <  255 && type != jsonTypeSEPARATOR) ||
                (index == 255 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 256)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "N Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 256; index++)
         {
            type = jsonGetTypeElem(json);
            if (!(type == jsonTypeNUMBER_NATURAL ||
                  type == jsonTypeNUMBER_INTEGER)  ||
                !jsonGetN(json, &ntemp)            ||
                _narray[index] != ntemp)
               break;

            type = jsonGetTypeElem(json);
            if ((index <  255 && type != jsonTypeSEPARATOR) ||
                (index == 255 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 256)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 300; index++)
         {
            type = jsonGetTypeElem(json);
            if (!(type == jsonTypeNUMBER_REAL    ||
                  type == jsonTypeNUMBER_INTEGER ||
                  type == jsonTypeNUMBER_NATURAL)  ||
                !jsonGetR(json, &rtemp)            ||
                _reals8[index] != rtemp)
               break;

            type = jsonGetTypeElem(json);
            if ((index <  299 && type != jsonTypeSEPARATOR) ||
                (index == 299 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 300)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "R4 Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 300; index++)
         {
            type = jsonGetTypeElem(json);
            if (!(type == jsonTypeNUMBER_REAL    ||
                  type == jsonTypeNUMBER_INTEGER ||
                  type == jsonTypeNUMBER_NATURAL)  ||
                !jsonGetR4(json, &r4temp)          ||
                _reals4[index] != r4temp)
               break;

            type = jsonGetTypeElem(json);
            if ((index <  299 && type != jsonTypeSEPARATOR) ||
                (index == 299 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 300)                                     break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

         msg = "String Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 10; index++)
         {
            type = jsonGetTypeElem(json);
            if (type != jsonTypeSTRING_START  ||
                !jsonGetStr(json, 256, stemp) ||
                strcmp(stemp, _strings[index]) != 0)
               break;

            type = jsonGetTypeElem(json);
            if ((index <  9 && type != jsonTypeSEPARATOR) ||
                (index == 9 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 10)                                      break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;

   #if defined(INCLUDE_BIN)
         msg = "Binary Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         for (index = 0; index < 3; index++)
         {
            if (!_JsonTestBin(json, 3 * 256, _binary))        break;

            type = jsonGetTypeElem(json);
            if ((index <  2 && type != jsonTypeSEPARATOR) ||
                (index == 2 && type != jsonTypeARRAY_STOP))
               break;
         }
         if (index != 3)                                       break;
         if (jsonGetTypeObj(  json) != jsonTypeSEPARATOR)      break;
   #endif

         msg = "User Type IntStrReal";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         {
            if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
            if (!jsonGetI(json, &itemp))                       break;
            if (itemp != 42)                                   break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
            if (!jsonGetStr(json, 256, stemp))                 break;
            if (strcmp(stemp, "Yes, but what is the question?"))  break;

            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

            if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
            if (!jsonGetR(json, &rtemp))                       break;
            if (rtemp != 3.1415926535897932)                   break;

            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }
         if (jsonGetTypeObj(json) != jsonTypeSEPARATOR)        break;

         msg = "User Type IntStrReal Array";
         if (!_JsonTestKey(  json, msg))                      break;
         if (jsonGetTypeObj(  json) != jsonTypeARRAY_START)    break;
         {
            if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
            {
               if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
               if (!jsonGetI(json, &itemp))                       break;
               if (itemp != 42)                                   break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
               if (!jsonGetStr(json, 256, stemp))                 break;
               if (strcmp(stemp, "Yes, but what is the question?"))  break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
               if (!jsonGetR(json, &rtemp))                       break;
               if (rtemp != 3.1415926535897932)                   break;

               if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
            }
            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
            if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
            {
               if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
               if (!jsonGetI(json, &itemp))                       break;
               if (itemp != 42)                                   break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
               if (!jsonGetStr(json, 256, stemp))                 break;
               if (strcmp(stemp, "Yes, but what is the question?"))  break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
               if (!jsonGetR(json, &rtemp))                       break;
               if (rtemp != 3.1415926535897932)                   break;

               if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
            }
            if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;
            if (jsonGetTypeObj(json) != jsonTypeARRAY_START)   break;
            {
               if (jsonGetTypeElem(json) != jsonTypeNUMBER_INTEGER) break;
               if (!jsonGetI(json, &itemp))                       break;
               if (itemp != 42)                                   break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeSTRING_START) break;
               if (!jsonGetStr(json, 256, stemp))                 break;
               if (strcmp(stemp, "Yes, but what is the question?"))  break;

               if (jsonGetTypeElem(json) != jsonTypeSEPARATOR)    break;

               if (jsonGetTypeElem(json) != jsonTypeNUMBER_REAL)  break;
               if (!jsonGetR(json, &rtemp))                       break;
               if (rtemp != 3.1415926535897932)                   break;

               if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
            }
            if (jsonGetTypeElem(json) != jsonTypeARRAY_STOP)   break;
         }

         if (jsonGetTypeObj(json) != jsonTypeOBJECT_STOP)       break;
      }

      if (jsonGetTypeObj(json) != jsonTypeOBJECT_STOP)          break;

      msg    = "NO";
      result = gbTRUE;
      break;
   }
   printf("JSON Read: %s ERROR\n", msg);

   jsonDestroy(json);
   fclose(file);

   return result;
}

/******************************************************************************
func: _JsonTestWrite
******************************************************************************/
static Gb _JsonTestWrite(Gstr const * const fileName)
{
   FILE     *file;
   Gjson     *json;
   Gb     result;
   Gi4    index;

   file   = NULL;
   json   = NULL;
   result = gbFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "wb") != 0)
      {
         break;
      }

      // Create a json file.
      json = jsonCreateWriter(_JsonSetBuffer, (void *) file, gbFALSE);
      if (!json)
      {
         break;
      }

      jsonSetObjectStart(json);

      jsonSetKey(              json, "Null");
      jsonSetValueNull(        json);

      jsonSetKey(              json, "True");
      jsonSetValueBool(        json, gbTRUE);
      jsonSetKey(              json, "False");
      jsonSetValueBool(        json, gbFALSE);

      jsonSetKey(              json, "I 0");
      jsonSetValueI(           json, 0);
      jsonSetKey(              json, "I 1");
      jsonSetValueI(           json, 1);
      jsonSetKey(              json, "I -1");
      jsonSetValueI(           json, -1);
      jsonSetKey(              json, "I MAX");
      jsonSetValueI(           json, JsonI_MAX);
      jsonSetKey(              json, "I MIN");
      jsonSetValueI(           json, JsonI_MIN);

      jsonSetKey(              json, "N 0");
      jsonSetValueN(           json, 0);
      jsonSetKey(              json, "N 1");
      jsonSetValueN(           json, 1);
      jsonSetKey(              json, "N MAX");
      jsonSetValueN(           json, JsonN_MAX);

      jsonSetKey(              json, "R 0");
      jsonSetValueR(           json, 0.0);
      jsonSetKey(              json, "R 1");
      jsonSetValueR(           json, 1.0);
      jsonSetKey(              json, "R -1");
      jsonSetValueR(           json, -1.0);
      jsonSetKey(              json, "R PI");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetKey(              json, "R EPS");
      jsonSetValueR(           json, JsonR_EPSILON);
      jsonSetKey(              json, "R MAX");
      jsonSetValueR(           json, JsonR_MAX);
      jsonSetKey(              json, "R -MAX");
      jsonSetValueR(           json, -JsonR_MAX);
      jsonSetKey(              json, "R INF");
      jsonSetValueR(           json, HUGE_VAL);
      jsonSetKey(              json, "R -INF");
      jsonSetValueR(           json, -HUGE_VAL);
      jsonSetKey(              json, "R NAN");
      jsonSetValueR(           json, NAN);

      jsonSetKey(              json, "R4 0");
      jsonSetValueR4(          json, 0.0f);
      jsonSetKey(              json, "R4 1");
      jsonSetValueR4(          json, 1.0f);
      jsonSetKey(              json, "R4 -1");
      jsonSetValueR4(          json, -1.0f);
      jsonSetKey(              json, "R4 PI");
      jsonSetValueR4(          json, 3.1415926535897932f);
      jsonSetKey(              json, "R4 EPS");
      jsonSetValueR4(          json, JsonR4_EPSILON);
      jsonSetKey(              json, "R4 MAX");
      jsonSetValueR4(          json, JsonR4_MAX);
      jsonSetKey(              json, "R4 -MAX");
      jsonSetValueR4(          json, -JsonR4_MAX);
      jsonSetKey(              json, "R4 INF");
      jsonSetValueR4(          json, HUGE_VALF);
      jsonSetKey(              json, "R4 -INF");
      jsonSetValueR4(          json, -HUGE_VALF);
      jsonSetKey(              json, "R4 NAN");
      jsonSetValueR4(          json, NAN);

      jsonSetKey(              json, "String");
      jsonSetValueStr(         json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

#if defined(INCLUDE_BIN)
      jsonSetKey(              json, "Binary");
      jsonSetValueBin(         json, 3 * 256, _binary);
#endif

      jsonSetKey(              json, "Bool Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 100; index++)
      {
         jsonSetValueBool(json, _bools[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "I Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 256; index++)
      {
         jsonSetValueI(json, _narray[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "N Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 256; index++)
      {
         jsonSetValueN(json, _narray[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "R Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 300; index++)
      {
         jsonSetValueR(json, _reals8[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "R4 Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 300; index++)
      {
         jsonSetValueR4(json, _reals4[index]);
      }
      jsonSetArrayStop(        json);

      jsonSetKey(              json, "String Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 10; index++)
      {
         jsonSetValueStr(json, _strings[index]);
      }
      jsonSetArrayStop(        json);

#if defined(INCLUDE_BIN)
      jsonSetKey(              json, "Binary Array");
      jsonSetArrayStart(       json);
      for (index = 0; index < 3; index++)
      {
         jsonSetValueBin(json, 3 * 256, _binary);
      }
      jsonSetArrayStop(        json);
#endif

      jsonSetKey(              json, "User Type IntStrReal");
      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);


      jsonSetKey(              json, "User Type IntStrReal Array");
      jsonSetArrayStart(       json);

      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);

      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);

      jsonSetArrayStart(       json);
      jsonSetValueI(           json, 42);
      jsonSetValueStr(         json, "Yes, but what is the question?");
      jsonSetValueR(           json, 3.1415926535897932);
      jsonSetArrayStop(        json);

      jsonSetArrayStop(        json);


      jsonSetKey(        json, "Block");
      jsonSetObjectStart(json);
      {
         jsonSetKey(              json, "Null");
         jsonSetValueNull(        json);

         jsonSetKey(              json, "True");
         jsonSetValueBool(        json, gbTRUE);
         jsonSetKey(              json, "False");
         jsonSetValueBool(        json, gbFALSE);

         jsonSetKey(              json, "I 0");
         jsonSetValueI(           json, 0);
         jsonSetKey(              json, "I 1");
         jsonSetValueI(           json, 1);
         jsonSetKey(              json, "I -1");
         jsonSetValueI(           json, -1);
         jsonSetKey(              json, "I MAX");
         jsonSetValueI(           json, JsonI_MAX);
         jsonSetKey(              json, "I MIN");
         jsonSetValueI(           json, JsonI_MIN);

         jsonSetKey(              json, "N 0");
         jsonSetValueN(           json, 0);
         jsonSetKey(              json, "N 1");
         jsonSetValueN(           json, 1);
         jsonSetKey(              json, "N MAX");
         jsonSetValueN(           json, JsonN_MAX);

         jsonSetKey(              json, "R 0");
         jsonSetValueR(           json, 0.0);
         jsonSetKey(              json, "R 1");
         jsonSetValueR(           json, 1.0);
         jsonSetKey(              json, "R -1");
         jsonSetValueR(           json, -1.0);
         jsonSetKey(              json, "R PI");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetKey(              json, "R EPS");
         jsonSetValueR(           json, JsonR_EPSILON);
         jsonSetKey(              json, "R MAX");
         jsonSetValueR(           json, JsonR_MAX);
         jsonSetKey(              json, "R -MAX");
         jsonSetValueR(           json, -JsonR_MAX);
         jsonSetKey(              json, "R INF");
         jsonSetValueR(           json, HUGE_VAL);
         jsonSetKey(              json, "R -INF");
         jsonSetValueR(           json, -HUGE_VAL);
         jsonSetKey(              json, "R NAN");
         jsonSetValueR(           json, NAN);

         jsonSetKey(              json, "R4 0");
         jsonSetValueR4(          json, 0.0f);
         jsonSetKey(              json, "R4 1");
         jsonSetValueR4(          json, 1.0f);
         jsonSetKey(              json, "R4 -1");
         jsonSetValueR4(          json, -1.0f);
         jsonSetKey(              json, "R4 PI");
         jsonSetValueR4(          json, 3.1415926535897932f);
         jsonSetKey(              json, "R4 EPS");
         jsonSetValueR4(          json, JsonR4_EPSILON);
         jsonSetKey(              json, "R4 MAX");
         jsonSetValueR4(          json, JsonR4_MAX);
         jsonSetKey(              json, "R4 -MAX");
         jsonSetValueR4(          json, -JsonR4_MAX);
         jsonSetKey(              json, "R4 INF");
         jsonSetValueR4(          json, HUGE_VALF);
         jsonSetKey(              json, "R4 -INF");
         jsonSetValueR4(          json, -HUGE_VALF);
         jsonSetKey(              json, "R4 NAN");
         jsonSetValueR4(          json, NAN);

         jsonSetKey(              json, "String");
         jsonSetValueStr(         json, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

#if defined(INCLUDE_BIN)
         jsonSetKey(              json, "Binary");
         jsonSetValueBin(         json, 3 * 256, _binary);
#endif

         jsonSetKey(              json, "Bool Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 100; index++)
         {
            jsonSetValueBool(json, _bools[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "I Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 256; index++)
         {
            jsonSetValueI(json, _narray[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "N Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 256; index++)
         {
            jsonSetValueN(json, _narray[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "R Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 300; index++)
         {
            jsonSetValueR(json, _reals8[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "R4 Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 300; index++)
         {
            jsonSetValueR4(json, _reals4[index]);
         }
         jsonSetArrayStop(        json);

         jsonSetKey(              json, "String Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 10; index++)
         {
            jsonSetValueStr(json, _strings[index]);
         }
         jsonSetArrayStop(        json);

#if defined(INCLUDE_BIN)
         jsonSetKey(              json, "Binary Array");
         jsonSetArrayStart(       json);
         for (index = 0; index < 3; index++)
         {
            jsonSetValueBin(json, 3 * 256, _binary);
         }
         jsonSetArrayStop(        json);
#endif

         jsonSetKey(              json, "User Type IntStrReal");
         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);


         jsonSetKey(              json, "User Type IntStrReal Array");
         jsonSetArrayStart(       json);

         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);

         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);

         jsonSetArrayStart(       json);
         jsonSetValueI(           json, 42);
         jsonSetValueStr(         json, "Yes, but what is the question?");
         jsonSetValueR(           json, 3.1415926535897932);
         jsonSetArrayStop(        json);

         jsonSetArrayStop(        json);
      }
      jsonSetObjectStop(json);

      jsonSetObjectStop(json);

      result = gbTRUE;
      break;
   }

   jsonDestroy(json);
   fclose(file);

   return result;
}

/******************************************************************************
func: _MiffTestRead
******************************************************************************/
static Gb _MiffTestKey(Gmiff *miff, char const * const KEY, GmiffRecType *recType, Gn8 *arrayCount)
{
   Gstr key[GkeySIZE];

   memset(key, 0, GkeySIZE);

   if (!miffGetRecordStart(miff, recType, arrayCount, key)) return miffFALSE;
   if (!streq(key, KEY))                                    return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestNullValue(Gmiff *miff)
{
   GmiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNULL)        return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestBValue(Gmiff *miff, Gb testValue)
{
   GmiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUM_INT ||
       miffValueGetB(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestIValue(Gmiff *miff, Gi8 testValue)
{
   GmiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUM_INT ||
       miffValueGetI(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestNValue(Gmiff *miff, Gn8 testValue)
{
   GmiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUM_INT ||
       miffValueGetN(   value) != testValue)                return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestRValue(Gmiff *miff, Gr8 testValue)
{
   GmiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUM_REAL ||
       miffValueIs4(    value))                             return miffFALSE;
   if ( isnan(testValue) && !isnan(miffValueGetR(value)))   return miffFALSE;
   if (!isnan(testValue) && miffValueGetR(value) != testValue) return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestR4Value(Gmiff *miff, Gr4 testValue)
{
   GmiffValue value;

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeNUM_REAL ||
       !miffValueIs4(   value))                             return miffFALSE;
   if ( isnan(testValue) && !isnan(miffValueGetR4(value)))  return miffFALSE;
   if (!isnan(testValue) && miffValueGetR4(value) != testValue) return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestBinValue(Gmiff *miff, Gn8 testCount, Gn1 *testValue)
{
   GmiffValue value;
   Gn1    svalue[256 * 3];

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeBIN &&
       testCount               != miffValueGetBinCount(value))        return miffFALSE;
   if (!miffGetValueBin(miff, miffValueGetBinCount(value), svalue))   return miffFALSE;
   if (memcmp(svalue, testValue, testCount) != 0)           return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestStrValue(Gmiff *miff, Gstr const *testValue)
{
   GmiffValue value;
   Gn8     testValueLen;
   Gstr   svalue[256];

   testValueLen = strlen(testValue);

   value = miffGetValue(miff);
   if (miffValueGetType(value) != miffValueTypeSTR &&
       testValueLen            != miffValueGetStrCount(value))        return miffFALSE;
   if (!miffGetValueStr(miff, miffValueGetStrCount(value), svalue))   return miffFALSE;
   if (strcmp(testValue, svalue) != 0)                      return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestCount(Gn8 COUNT, Gn8 arrayCount)
{
   if (COUNT != arrayCount) return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestNext(Gmiff *miff)
{
   if (!miffGetRecordEnd(miff)) return miffFALSE;
   return miffTRUE;
}

static Gb _MiffTestGetNull(Gmiff *miff, char const *key)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestNullValue(miff))                        return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestGetBool(Gmiff *miff, char const *key, Gb const value)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestBValue(miff, value))                    return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestGetI(Gmiff *miff, char const *key, Gi8 const value)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestIValue(miff, value))                    return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestGetN(Gmiff *miff, char const *key, Gn8 const value)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestNValue(miff, value))                    return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestGetR(Gmiff *miff, char const *key, Gr8 const value)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestRValue(miff, value))                    return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestGetR4(Gmiff *miff, char const *key, Gr4 const value)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestR4Value(miff, value))                   return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestGetStr(Gmiff *miff, char const *key, Gstr const *value)
{
   GmiffRecType     recType;
   Gn8           arrayCount;

   if (!_MiffTestKey(miff, key, &recType, &arrayCount))  return miffFALSE;
   if (!_MiffTestCount(1, arrayCount))                   return miffFALSE;
   if (!_MiffTestStrValue(miff, value))                  return miffFALSE;
   if (!_MiffTestNext(miff))                             return miffFALSE;

   return miffTRUE;
}

static Gb _MiffTestRead(Gstr const * const fileName)
{
   FILE           *file;
   Gmiff           *miff;
   Gb           result;
   Gstr         subFormatName[GkeySIZE];
   Gstr         subFormatVersion[GkeySIZE];
   GmiffRecType     recType;
   Gn8           arrayCount;
   Gn8           index;
   char const     *msg;

   file   = NULL;
   miff   = NULL;
   result = miffFALSE;

   for (;;)
   {
      msg = "Open file";
      if (fopen_s(&file, fileName, "rb") != 0) break;

      // Set Gmiff up for reading.
      msg = "Create reader";
      miff = miffCreateReader(
         miffTRUE,
         _MiffGetBuffer,
         subFormatName,
         subFormatVersion,
         (void *) file);
      if (!miff) break;

      // Read in the header information.
      //wprintf(L"%S\t%d\n", (char *) subFormatName, (int) subFormatVersion);

      // Unless you are in full control of the format, do not assume that there will be an order
      // to the values.
      msg = "Null";
      if (!_MiffTestGetNull(miff, msg))                     break;

      msg = "True";
      if (!_MiffTestGetBool(miff, msg, miffTRUE))           break;

      msg = "False";
      if (!_MiffTestGetBool(miff, msg, miffFALSE))          break;

      msg = "I 0";
      if (!_MiffTestGetI(miff, msg, 0))                     break;

      msg = "I 1";
      if (!_MiffTestGetI(miff, msg, 1))                     break;

      msg = "I -1";
      if (!_MiffTestGetI(miff, msg, -1))                    break;

      msg = "I MAX";
      if (!_MiffTestGetI(miff, msg, MiffI_MAX))             break;

      msg = "I MIN";
      if (!_MiffTestGetI(miff, msg, MiffI_MIN))             break;

      msg = "N 0";
      if (!_MiffTestGetN(miff, msg, 0))                     break;

      msg = "N 1";
      if (!_MiffTestGetN(miff, msg, 1))                     break;

      msg = "N MAX";
      if (!_MiffTestGetN(miff, msg, MiffN_MAX))             break;

      msg = "R 0";
      if (!_MiffTestGetR(miff, msg, 0.0))                   break;

      msg = "R 1";
      if (!_MiffTestGetR(miff, msg, 1.0))                   break;

      msg = "R -1";
      if (!_MiffTestGetR(miff, msg, -1.0))                  break;

      msg = "R PI";
      if (!_MiffTestGetR(miff, msg, 3.1415926535897932))    break;

      msg = "R EPS";
      if (!_MiffTestGetR(miff, msg, MiffR_EPSILON))         break;

      msg = "R MAX";
      if (!_MiffTestGetR(miff, msg, MiffR_MAX))             break;

      msg = "R -MAX";
      if (!_MiffTestGetR(miff, msg, -MiffR_MAX))            break;

      msg = "R INF";
      if (!_MiffTestGetR(miff, msg, HUGE_VAL))              break;

      msg = "R -INF";
      if (!_MiffTestGetR(miff, msg, -HUGE_VAL))             break;

      msg = "R NAN";
      if (!_MiffTestGetR(miff, msg, NAN))                   break;

      msg = "R4 0";
      if (!_MiffTestGetR4(miff, msg, 0.0))                  break;

      msg = "R4 1";
      if (!_MiffTestGetR4(miff, msg, 1.0))                  break;

      msg = "R4 -1";
      if (!_MiffTestGetR4(miff, msg, -1.0))                 break;

      msg = "R4 PI";
      if (!_MiffTestGetR4(miff, msg, 3.1415926535897932f))  break;

      msg = "R4 EPS";
      if (!_MiffTestGetR4(miff, msg, MiffR4_EPSILON))       break;

      msg = "R4 MAX";
      if (!_MiffTestGetR4(miff, msg, MiffR4_MAX))           break;

      msg = "R4 -MAX";
      if (!_MiffTestGetR4(miff, msg, -MiffR4_MAX))          break;

      msg = "R4 INF";
      if (!_MiffTestGetR4(miff, msg, HUGE_VALF))            break;

      msg = "R4 -INF";
      if (!_MiffTestGetR4(miff, msg, -HUGE_VALF))           break;

      msg = "R4 NAN";
      if (!_MiffTestGetR4(miff, msg, NAN))                  break;

      msg = "String";
      if (!_MiffTestGetStr(miff, msg, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;

#if defined(INCLUDE_BIN)
      msg = "Binary";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(1, arrayCount))                   break;
      if (!_MiffTestBinValue(miff, 256 * 3, _binary))       break;
      if (!_MiffTestNext(miff))                             break;
#endif

      msg = "Bool Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(100, arrayCount))                 break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!_MiffTestBValue(miff, _bools[index]))         break;
      }
      if (!_MiffTestNext(miff))                             break;

      msg = "I Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(256, arrayCount))                 break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!_MiffTestIValue(miff, (int) _narray[index]))  break;
      }
      if (!_MiffTestNext(miff))                             break;

      msg = "N Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(256, arrayCount))                 break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!_MiffTestNValue(miff, _narray[index]))        break;
      }
      if (!_MiffTestNext(miff))                             break;

      msg = "R Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(300, arrayCount))                 break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!_MiffTestRValue(miff, _reals8[index]))        break;
      }
      if (!_MiffTestNext(miff))                             break;

      msg = "R4 Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(300, arrayCount))                 break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!_MiffTestR4Value(miff, _reals4[index]))       break;
      }
      if (!_MiffTestNext(miff))                             break;

      msg = "String Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(10, arrayCount))                  break;
      for (index = 0; index < arrayCount; index++)
      {
         if (!_MiffTestStrValue(miff, _strings[index]))     break;
      }
      if (!_MiffTestNext(miff))                             break;

#if defined(INCLUDE_BIN)
      msg = "Binary Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(3, arrayCount))                   break;
      if (!_MiffTestBinValue(miff, 3 * 256, _binary))       break;
      if (!_MiffTestBinValue(miff, 3 * 256, _binary))       break;
      if (!_MiffTestBinValue(miff, 3 * 256, _binary))       break;
      if (!_MiffTestNext(miff))                             break;
#endif

      msg = "User Type IntStrReal";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(1, arrayCount))                   break;
      if (!_MiffTestIValue(  miff, 42))                     break;
      if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
      if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
      if (!_MiffTestNext(miff))                             break;

      msg = "User Type IntStrReal Array";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (!_MiffTestCount(3, arrayCount))                   break;
      if (!_MiffTestIValue(  miff, 42))                     break;
      if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
      if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
      if (!_MiffTestIValue(  miff, 42))                     break;
      if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
      if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
      if (!_MiffTestIValue(  miff, 42))                     break;
      if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
      if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
      if (!_MiffTestNext(miff))                             break;

      msg = "Block";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
      if (recType != miffRecTypeBLOCK_START)                break;
      if (!_MiffTestNext(miff))                             break;
      {
         msg = "Null";
         if (!_MiffTestGetNull(miff, msg))                     break;

         msg = "True";
         if (!_MiffTestGetBool(miff, msg, miffTRUE))           break;

         msg = "False";
         if (!_MiffTestGetBool(miff, msg, miffFALSE))          break;

         msg = "I 0";
         if (!_MiffTestGetI(miff, msg, 0))                     break;

         msg = "I 1";
         if (!_MiffTestGetI(miff, msg, 1))                     break;

         msg = "I -1";
         if (!_MiffTestGetI(miff, msg, -1))                    break;

         msg = "I MAX";
         if (!_MiffTestGetI(miff, msg, MiffI_MAX))             break;

         msg = "I MIN";
         if (!_MiffTestGetI(miff, msg, MiffI_MIN))             break;

         msg = "N 0";
         if (!_MiffTestGetN(miff, msg, 0))                     break;

         msg = "N 1";
         if (!_MiffTestGetN(miff, msg, 1))                     break;

         msg = "N MAX";
         if (!_MiffTestGetN(miff, msg, MiffN_MAX))             break;

         msg = "R 0";
         if (!_MiffTestGetR(miff, msg, 0.0))                   break;

         msg = "R 1";
         if (!_MiffTestGetR(miff, msg, 1.0))                   break;

         msg = "R -1";
         if (!_MiffTestGetR(miff, msg, -1.0))                  break;

         msg = "R PI";
         if (!_MiffTestGetR(miff, msg, 3.1415926535897932))    break;

         msg = "R EPS";
         if (!_MiffTestGetR(miff, msg, MiffR_EPSILON))         break;

         msg = "R MAX";
         if (!_MiffTestGetR(miff, msg, MiffR_MAX))             break;

         msg = "R -MAX";
         if (!_MiffTestGetR(miff, msg, -MiffR_MAX))            break;

         msg = "R INF";
         if (!_MiffTestGetR(miff, msg, HUGE_VAL))              break;

         msg = "R -INF";
         if (!_MiffTestGetR(miff, msg, -HUGE_VAL))             break;

         msg = "R NAN";
         if (!_MiffTestGetR(miff, msg, NAN))                   break;

         msg = "R4 0";
         if (!_MiffTestGetR4(miff, msg, 0.0))                  break;

         msg = "R4 1";
         if (!_MiffTestGetR4(miff, msg, 1.0))                  break;

         msg = "R4 -1";
         if (!_MiffTestGetR4(miff, msg, -1.0))                 break;

         msg = "R4 PI";
         if (!_MiffTestGetR4(miff, msg, 3.1415926535897932f))  break;

         msg = "R4 EPS";
         if (!_MiffTestGetR4(miff, msg, MiffR4_EPSILON))       break;

         msg = "R4 MAX";
         if (!_MiffTestGetR4(miff, msg, MiffR4_MAX))           break;

         msg = "R4 -MAX";
         if (!_MiffTestGetR4(miff, msg, -MiffR4_MAX))          break;

         msg = "R4 INF";
         if (!_MiffTestGetR4(miff, msg, HUGE_VALF))            break;

         msg = "R4 -INF";
         if (!_MiffTestGetR4(miff, msg, -HUGE_VALF))           break;

         msg = "R4 NAN";
         if (!_MiffTestGetR4(miff, msg, NAN))                  break;

         msg = "String";
         if (!_MiffTestGetStr(miff, msg, "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./")) break;

   #if defined(INCLUDE_BIN)
         msg = "Binary";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(1, arrayCount))                   break;
         if (!_MiffTestBinValue(miff, 256 * 3, _binary))       break;
         if (!_MiffTestNext(miff))                             break;
   #endif

         msg = "Bool Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(100, arrayCount))                 break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!_MiffTestBValue(miff, _bools[index]))         break;
         }
         if (!_MiffTestNext(miff))                             break;

         msg = "I Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(256, arrayCount))                 break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!_MiffTestIValue(miff, (int) _narray[index]))  break;
         }
         if (!_MiffTestNext(miff))                             break;

         msg = "N Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(256, arrayCount))                 break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!_MiffTestNValue(miff, _narray[index]))        break;
         }
         if (!_MiffTestNext(miff))                             break;

         msg = "R Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(300, arrayCount))                 break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!_MiffTestRValue(miff, _reals8[index]))        break;
         }
         if (!_MiffTestNext(miff))                             break;

         msg = "R4 Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(300, arrayCount))                 break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!_MiffTestR4Value(miff, _reals4[index]))       break;
         }
         if (!_MiffTestNext(miff))                             break;

         msg = "String Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(10, arrayCount))                  break;
         for (index = 0; index < arrayCount; index++)
         {
            if (!_MiffTestStrValue(miff, _strings[index]))     break;
         }
         if (!_MiffTestNext(miff))                             break;

   #if defined(INCLUDE_BIN)
         msg = "Binary Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(3, arrayCount))                   break;
         if (!_MiffTestBinValue(miff, 3 * 256, _binary))       break;
         if (!_MiffTestBinValue(miff, 3 * 256, _binary))       break;
         if (!_MiffTestBinValue(miff, 3 * 256, _binary))       break;
         if (!_MiffTestNext(miff))                             break;
   #endif

         msg = "User Type IntStrReal";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(1, arrayCount))                   break;
         if (!_MiffTestIValue(  miff, 42))                     break;
         if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
         if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
         if (!_MiffTestNext(miff))                             break;

         msg = "User Type IntStrReal Array";
         if (!_MiffTestKey(miff, msg, &recType, &arrayCount))  break;
         if (!_MiffTestCount(3, arrayCount))                   break;
         if (!_MiffTestIValue(  miff, 42))                     break;
         if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
         if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
         if (!_MiffTestIValue(  miff, 42))                     break;
         if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
         if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
         if (!_MiffTestIValue(  miff, 42))                     break;
         if (!_MiffTestStrValue(miff, "Yes, but what is the question?")) break;
         if (!_MiffTestRValue(  miff, 3.1415926535897932))     break;
         if (!_MiffTestNext(miff))                             break;
      }

      msg = "";
      if (!_MiffTestKey(miff, msg, &recType, &arrayCount))     break;
      if (recType != miffRecTypeBLOCK_STOP)                    break;
      if (!_MiffTestNext(miff))                                break;

      msg = "NO";
      result = miffTRUE;

      break;
   }

   printf("MIFF Read: %s ERROR\n", msg);

   miffDestroy(miff);
   fclose(file);

   return result;
}

/******************************************************************************
func: _MiffTestWrite
******************************************************************************/
static Gb _MiffTestWrite(Gstr const * const fileName)
{
   FILE     *file;
   Gmiff     *miff;
   Gb  result;

   file   = NULL;
   miff   = NULL;
   result = miffFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "wb") != 0)
      {
         break;
      }

      // Create a miff file.
      miff = miffCreateWriter(miffTRUE, _MiffSetBuffer, "MiffTestFile", "1", (void *) file);
      if (!miff)
      {
         break;
      }

      miffSetRecordNull(           miff, "Null");

      miffSetRecordBool(           miff, "True",     miffTRUE);
      miffSetRecordBool(           miff, "False",    miffFALSE);

      miffSetRecordI(              miff, "I 0",      0);
      miffSetRecordI(              miff, "I 1",      1);
      miffSetRecordI(              miff, "I -1",     -1);
      miffSetRecordI(              miff, "I MAX",    MiffI_MAX);
      miffSetRecordI(              miff, "I MIN",    MiffI_MIN);

      miffSetRecordN(              miff, "N 0",      0);
      miffSetRecordN(              miff, "N 1",      1);
      miffSetRecordN(              miff, "N MAX",    MiffN_MAX);

      miffSetRecordR(              miff, "R 0",     0.0);
      miffSetRecordR(              miff, "R 1",     1.0);
      miffSetRecordR(              miff, "R -1",    -1.0);
      miffSetRecordR(              miff, "R PI",    3.1415926535897932);
      miffSetRecordR(              miff, "R EPS",   MiffR_EPSILON);
      miffSetRecordR(              miff, "R MAX",   MiffR_MAX);
      miffSetRecordR(              miff, "R -MAX",  -MiffR_MAX);
      miffSetRecordR(              miff, "R INF",   HUGE_VAL);
      miffSetRecordR(              miff, "R -INF",  -HUGE_VAL);
      miffSetRecordR(              miff, "R NAN",   NAN);

      miffSetRecordR4(             miff, "R4 0",    0.0);
      miffSetRecordR4(             miff, "R4 1",    1.0);
      miffSetRecordR4(             miff, "R4 -1",   -1.0);
      miffSetRecordR4(             miff, "R4 PI",   3.1415926535897932f);
      miffSetRecordR4(             miff, "R4 EPS",  MiffR4_EPSILON);
      miffSetRecordR4(             miff, "R4 MAX",  MiffR4_MAX);
      miffSetRecordR4(             miff, "R4 -MAX", -MiffR4_MAX);
      miffSetRecordR4(             miff, "R4 INF",  HUGE_VALF);
      miffSetRecordR4(             miff, "R4 -INF", -HUGE_VALF);
      miffSetRecordR4(             miff, "R4 NAN",  NAN);

      miffSetRecordStr(            miff, "String",  "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
#if defined(INCLUDE_BIN)
      miffSetRecordBinBuffer(      miff, "Binary",  256 * 3, _binary);
#endif

      miffSetRecordBoolArray(      miff, "Bool Array",   100,  _bools);

      miffSetRecordIArray(         miff, "I Array",      256,  _narray);
      miffSetRecordNArray(         miff, "N Array",      256,  _narray);

      miffSetRecordRArray(         miff, "R Array",      300,  _reals8);
      miffSetRecordR4Array(        miff, "R4 Array",     300,  _reals4);

      miffSetRecordStrArray(       miff, "String Array", 10,   _strings);

#if defined(INCLUDE_BIN)
      miffSetRecordStartBinArray(  miff, "Binary Array", 3);
      miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
      miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
      miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
      miffSetRecordStop(           miff);
#endif

      miffSetRecordStartValue(     miff, "User Type IntStrReal");
      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);
      miffSetRecordStop(           miff);

      miffSetRecordStartValueArray(miff, "User Type IntStrReal Array", 3);
      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);

      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);

      miffSetRecordValueI(         miff, 42);
      miffSetRecordValueStr(       miff, "Yes, but what is the question?");
      miffSetRecordValueR(         miff, 3.1415926535897932);
      miffSetRecordStop(           miff);

      miffSetRecordBlockStart(     miff, "Block");
      {
         miffSetRecordNull(           miff, "Null");

         miffSetRecordBool(           miff, "True",     miffTRUE);
         miffSetRecordBool(           miff, "False",    miffFALSE);

         miffSetRecordI(              miff, "I 0",      0);
         miffSetRecordI(              miff, "I 1",      1);
         miffSetRecordI(              miff, "I -1",     -1);
         miffSetRecordI(              miff, "I MAX",    MiffI_MAX);
         miffSetRecordI(              miff, "I MIN",    MiffI_MIN);

         miffSetRecordN(              miff, "N 0",      0);
         miffSetRecordN(              miff, "N 1",      1);
         miffSetRecordN(              miff, "N MAX",    MiffN_MAX);

         miffSetRecordR(              miff, "R 0",      0.0);
         miffSetRecordR(              miff, "R 1",      1.0);
         miffSetRecordR(              miff, "R -1",     -1.0);
         miffSetRecordR(              miff, "R PI",     3.1415926535897932);
         miffSetRecordR(              miff, "R EPS",    MiffR_EPSILON);
         miffSetRecordR(              miff, "R MAX",    MiffR_MAX);
         miffSetRecordR(              miff, "R -MAX",   -MiffR_MAX);
         miffSetRecordR(              miff, "R INF",    HUGE_VAL);
         miffSetRecordR(              miff, "R -INF",   -HUGE_VAL);
         miffSetRecordR(              miff, "R NAN",    NAN);

         miffSetRecordR4(             miff, "R4 0",     0.0);
         miffSetRecordR4(             miff, "R4 1",     1.0);
         miffSetRecordR4(             miff, "R4 -1",    -1.0);
         miffSetRecordR4(             miff, "R4 PI",    3.1415926535897932f);
         miffSetRecordR4(             miff, "R4 EPS",   MiffR4_EPSILON);
         miffSetRecordR4(             miff, "R4 MAX",   MiffR4_MAX);
         miffSetRecordR4(             miff, "R4 -MAX",  -MiffR4_MAX);
         miffSetRecordR4(             miff, "R4 INF",   HUGE_VALF);
         miffSetRecordR4(             miff, "R4 -INF",  -HUGE_VALF);
         miffSetRecordR4(             miff, "R4 NAN",   NAN);

         miffSetRecordStr(            miff, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
#if defined(INCLUDE_BIN)
         miffSetRecordBinBuffer(      miff, "Binary",   256 * 3, _binary);
#endif

         miffSetRecordBoolArray(      miff, "Bool Array",   100,  _bools);

         miffSetRecordIArray(         miff, "I Array",      256,  _narray);
         miffSetRecordNArray(         miff, "N Array",      256,  _narray);

         miffSetRecordRArray(         miff, "R Array",      300,  _reals8);
         miffSetRecordR4Array(        miff, "R4 Array",     300,  _reals4);

         miffSetRecordStrArray(       miff, "String Array", 10,   _strings);

#if defined(INCLUDE_BIN)
         miffSetRecordStartBinArray(  miff, "Binary Array", 3);
         miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
         miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
         miffSetValue(                miff, miffValueSetBinBuffer(3 * 256, _binary));
         miffSetRecordStop(           miff);
#endif

         miffSetRecordStartValue(     miff, "User Type IntStrReal");
         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);
         miffSetRecordStop(           miff);

         miffSetRecordStartValueArray(miff, "User Type IntStrReal Array", 3);
         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);

         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);

         miffSetRecordValueI(         miff, 42);
         miffSetRecordValueStr(       miff, "Yes, but what is the question?");
         miffSetRecordValueR(         miff, 3.1415926535897932);
         miffSetRecordStop(           miff);
      }
      miffSetRecordBlockStop(miff);

      result = miffTRUE;
      break;
   }

   miffDestroy(miff);
   fclose(file);

   return result;
}
