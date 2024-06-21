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
#define streq(S1, S2)  (strcmp(S1, S2) == 0)

/******************************************************************************
variable:
******************************************************************************/
static MiffN1               _binary[] =
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

static MiffN               _narray[] =
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

static MiffR4               _reals4[300] =
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

static MiffR                _reals8[300] =
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

static MiffStr              *_strings[10] =
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
static MiffB             _bools[100] =
{
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
      miffTRUE, miffTRUE, miffFALSE, miffTRUE, miffTRUE, miffFALSE, miffFALSE, miffFALSE, miffTRUE, miffFALSE,
};

/******************************************************************************
prototype:
******************************************************************************/
static MiffB _JsonGetBuffer(     void * const dataRepo, JsonN4 const byteCount, JsonStr       * const byteData);
static void    *_JsonMemCreate(     JsonN4 const memByteCount);
static void     _JsonMemDestroy(    void * const mem);
static JsonBool _JsonTestRead(      JsonStr const * const fileName);
static JsonBool _JsonTestReadObject(Json * const json);
static JsonBool _JsonSetBuffer(     void * const dataRepo, JsonN4 const byteCount, JsonStr const * const byteData);
static JsonBool _JsonTestWrite(     JsonStr const * const fileName);

static MiffB _MiffGetBuffer(     void * const dataRepo, MiffN4 const byteCount, MiffStr       * const byteData);
static void    *_MiffMemCreate(     MiffN4 const memByteCount);
static void     _MiffMemDestroy(    void * const mem);
static MiffB _MiffSetBuffer(     void * const dataRepo, MiffN4 const byteCount, MiffStr const * const byteData);
static MiffB _MiffTestRead(      MiffStr const * const fileName);
static MiffB _MiffTestWrite(     MiffStr const * const fileName);

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
static JsonBool _JsonGetBuffer(void * const dataRepo, JsonN4 const byteCount, JsonStr * const byteData)
{
   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

static MiffB _MiffGetBuffer(void * const dataRepo, MiffN4 const byteCount, MiffStr * const byteData)
{
   return (_fread_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _MemCreate
******************************************************************************/
void *_JsonMemCreate(JsonN4 const memByteCount)
{
   return calloc(1, (size_t) memByteCount);
}

void *_MiffMemCreate(MiffN4 const memByteCount)
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
static JsonBool _JsonSetBuffer(void * const dataRepo, JsonN4 const byteCount, JsonStr const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

static MiffB _MiffSetBuffer(void * const dataRepo, MiffN4 const byteCount, MiffStr const * const byteData)
{
   return (_fwrite_nolock(byteData, 1, byteCount, (FILE *) dataRepo) == byteCount);
}

/******************************************************************************
func: _JsonTestRead
******************************************************************************/
static JsonBool _JsonTestRead(JsonStr const * const fileName)
{
   FILE           *file;
   Json           *json;
   JsonBool        result;
   JsonReadType    type;

   file   = NULL;
   json   = NULL;
   result = jsonBoolFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "rb") != 0)
      {
         break;
      }

      // Set Json up for reading.
      json = jsonCreateReader(_JsonGetBuffer, (void *) file);
      if (!json)
      {
         break;
      }

      for (;;)
      {
         if (!jsonRead(json, &type))
         {
            break;
         }

         if (type == jsonReadTypeOBJECT_START)
         {
            wprintf(L"Object Start {\n");
            if (!_JsonTestReadObject(json))
            {
               break;
            }
         }
         else
         {
            break;
         }
      }

      result = jsonBoolTRUE;
      break;
   }

   jsonDestroy(json);
   fclose(file);

   return result;
}

/******************************************************************************
func: _JsonTestReadObject
******************************************************************************/
static JsonBool _JsonTestReadObject(Json * const json)
{
   JsonReadType type;

   for (;;)
   {
      if (!jsonRead(json, &type))
      {
         break;
      }

      if (type == jsonReadTypeOBJECT_STOP)
      {
         wprintf(L"} Object Stop \n");
         continue;
      }
      else
      {
         // We got something we were not expecting.
         return jsonBoolFALSE;
      }
   }

   return jsonBoolTRUE;
}

/******************************************************************************
func: _JsonTestWrite
******************************************************************************/
static JsonBool _JsonTestWrite(JsonStr const * const fileName)
{
#if 0
   FILE     *file;
   Json     *json;
   JsonBool  result;

   file   = NULL;
   json   = NULL;
   result = jsonBoolFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "wb") != 0)
      {
         break;
      }

      // Create a json file.
      json = jsonCreateWriter(_JsonSetBuffer, (void *) file);
      if (!json)
      {
         break;
      }

      jsonWriteObjectStart(json);

      jsonWriteKey1Boolean(      json, "True",  jsonBoolTRUE);
      jsonWriteKey1Boolean(      json, "False", jsonBoolFALSE);

      jsonWriteKey1I1(           json, "I1_0",     0);
      jsonWriteKey1I1(           json, "I1_1",     1);
      jsonWriteKey1I1(           json, "I1_-1",    -1);
      jsonWriteKey1I1(           json, "I1_127",   127);
      jsonWriteKey1I1(           json, "I1_-128",  -128);
                    
      jsonWriteKey1N1(           json, "N1_0",     0);
      jsonWriteKey1N1(           json, "N1_1",     1);
      jsonWriteKey1N1(           json, "N1_255",   255);

      jsonWriteKey1I2(           json, "I2_0",     0);
      jsonWriteKey1I2(           json, "I2_1",     1);
      jsonWriteKey1I2(           json, "I2_-1",    -1);

      jsonWriteKey1N2(           json, "N2_0",     0);
      jsonWriteKey1N2(           json, "N2_1",     1);

      jsonWriteKey1I4(           json, "I4_0",     0);
      jsonWriteKey1I4(           json, "I4_1",     1);
      jsonWriteKey1I4(           json, "I4_-1",    -1);

      jsonWriteKey1N4(           json, "N4_0",     0);
      jsonWriteKey1N4(           json, "N4_1",     1);

      jsonWriteKey1R4(           json, "R4_0",     0.0);
      jsonWriteKey1R4(           json, "R4_1",     1.0);
      jsonWriteKey1R4(           json, "R4_-1",    -1.0);
      jsonWriteKey1R4(           json, "R4_PI",    3.1415926535897932f);

      jsonWriteKey1I8(           json, "I8_0",     0);
      jsonWriteKey1I8(           json, "I8_1",     1);
      jsonWriteKey1I8(           json, "I8_-1",    -1);

      jsonWriteKey1N8(           json, "N8_0",     0);
      jsonWriteKey1N8(           json, "N8_1",     1);

      jsonWriteKey1R8(           json, "R8_0",     0.0);
      jsonWriteKey1R8(           json, "R8_1",     1.0);
      jsonWriteKey1R8(           json, "R8_-1",    -1.0);
      jsonWriteKey1R8(           json, "R8_PI",    3.1415926535897932);

      jsonWriteKey1StringC2(     json, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

      jsonWriteKey(              json, "variableIntStrReal");
      jsonWriteArrayStart(       json);
      jsonWriteValueI(           json, 42);
      jsonWriteValueStringC2(    json, "Yes, but what is the question?");
      jsonWriteValueR8(          json, 3.1415926535897932);
      jsonWriteArrayStop(        json);

      jsonWriteKeyNI1(           json, "I1_Array",     256,     (MiffI1 *) _n1array);
      jsonWriteKeyNN1(           json, "N1_Array",     256,     _n1array);

      jsonWriteKeyNI2(           json, "I2_Array",     256,     (MiffI2 *) _n2array);
      jsonWriteKeyNN2(           json, "N2_Array",     256,     _n2array);

      jsonWriteKeyNI4(           json, "I4_Array",     256,     (MiffI4 *) _n4array);
      jsonWriteKeyNN4(           json, "N4_Array",     256,     _n4array);

      jsonWriteKeyNI8(           json, "I8_Array",     256,     (MiffI8 *) _n8array);
      jsonWriteKeyNN8(           json, "N8_Array",     256,     _n8array);

      jsonWriteKeyNR4(           json, "R4_Array",     300,     _reals4);
      jsonWriteKeyNR8(           json, "R8_Array",     300,     _reals8);

      jsonWriteKeyNStringC2(     json, "String_Array", 10,      (JsonStr **) _strings);

      jsonWriteKeyNBoolean(      json, "Bool_Array",   100,     (JsonBool *) _bools);

      jsonWriteKey(              json, "userVarIntStrReal");
      jsonWriteArrayStart(       json);
      jsonWriteValueI(           json, 42);
      jsonWriteValueStringC2(    json, "Yes, but what is the question?");
      jsonWriteValueR8(          json, 3.1415926535897932);
      jsonWriteArrayStop(        json);

      jsonWriteKey(        json, "KeyValueBlock");
      jsonWriteObjectStart(json);
      {
         jsonWriteKey1Boolean( json, "True",     jsonBoolTRUE);
         jsonWriteKey1Boolean( json, "False",    jsonBoolFALSE);

         jsonWriteKey1I1(      json, "I1_0",     0);
         jsonWriteKey1I1(      json, "I1_1",     1);
         jsonWriteKey1I1(      json, "I1_-1",    -1);
         jsonWriteKey1I1(      json, "I1_127",   127);
         jsonWriteKey1I1(      json, "I1_-128",  -128);

         jsonWriteKey1N1(      json, "N1_0",     0);
         jsonWriteKey1N1(      json, "N1_1",     1);
         jsonWriteKey1N1(      json, "N1_255",   255);

         jsonWriteKey1I2(      json, "I2_0",     0);
         jsonWriteKey1I2(      json, "I2_1",     1);
         jsonWriteKey1I2(      json, "I2_-1",    -1);

         jsonWriteKey1N2(      json, "N2_0",     0);
         jsonWriteKey1N2(      json, "N2_1",     1);

         jsonWriteKey1I4(      json, "I4_0",     0);
         jsonWriteKey1I4(      json, "I4_1",     1);
         jsonWriteKey1I4(      json, "I4_-1",    -1);

         jsonWriteKey1N4(      json, "N4_0",     0);
         jsonWriteKey1N4(      json, "N4_1",     1);

         jsonWriteKey1R4(      json, "R4_0",     0.0);
         jsonWriteKey1R4(      json, "R4_1",     1.0);
         jsonWriteKey1R4(      json, "R4_-1",    -1.0);
         jsonWriteKey1R4(      json, "R4_PI",    3.1415926535897932f);

         jsonWriteKey1I8(      json, "I8_0",     0);
         jsonWriteKey1I8(      json, "I8_1",     1);
         jsonWriteKey1I8(      json, "I8_-1",    -1);

         jsonWriteKey1N8(      json, "N8_0",     0);
         jsonWriteKey1N8(      json, "N8_1",     1);

         jsonWriteKey1R8(      json, "R8_0",     0.0);
         jsonWriteKey1R8(      json, "R8_1",     1.0);
         jsonWriteKey1R8(      json, "R8_-1",    -1.0);
         jsonWriteKey1R8(      json, "R8_PI",    3.1415926535897932);

         jsonWriteKey1StringC2(json, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");

         jsonWriteKeyNStringC2(json, "String_Array", 10,      _strings);
      }
      jsonWriteObjectStop(json);

      jsonWriteObjectStop(json);

      result = jsonBoolTRUE;
      break;
   }

   jsonDestroy(json);
   fclose(file);

   return result;
#endif
   return jsonBoolFALSE;
}

/******************************************************************************
func: _MiffTestRead
******************************************************************************/
static MiffB _MiffTestRead(MiffStr const * const fileName)
{
#if 0
   FILE           *file;
   Miff           *miff;
   MiffB           result;
   MiffStr         subFormatName[miffKeySIZE];
   MiffStr         typeName[miffKeySIZE];
   MiffN           subFormatVersion;
   MiffValueType   type;
   MiffStr         key[miffKeySIZE];
   MiffN           arrayIndex;
   MiffN           arrayCount;
   MiffValue       value;
   MiffN           index;

   file   = NULL;
   miff   = NULL;
   result = miffFALSE;

   for (;;)
   {
      if (fopen_s(&file, fileName, "rb") != 0)
      {
         break;
      }

      // Set Miff up for reading.
      miff = miffCreateReader(
         miffTRUE,
         _MiffGetBuffer,
         subFormatName,
         &subFormatVersion,
         (void *) file);
      if (!miff)
      {
         break;
      }

      // Read in the header information.
      wprintf(L"%S\t%d\n", (char *) subFormatName, (int) subFormatVersion);


#define TEST_B(KEY)                                                                                            \
      printf("b\t%s\t", KEY);                                                                                  \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeBOOL || !streq(key, KEY)) { printf("ERROR\t"); }                                  \
         else { printf("OK\t"); }                                                                              \
      }
#define TEST_B_VALUE(VALUE)                                                                                    \
      value = miffGetValueBoolean(miff);                                                                       \
      if (miffValueGetValueType(value) != miffTypeBOOL || miffValueGetBool(value) != VALUE) { printf("ERROR\n"); } else { printf("OK\n"); }

#define TEST_I(KEY)                                                                                            \
      printf("i\t%s\t", KEY);                                                                                  \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeI || !streq(key, KEY) { printf("ERROR\t"); }                                      \
         else { printf("OK\t"); }                                                                              \
      }
#define TEST_I_VALUE(VALUE)                                                                                    \
      value = miffGetValueI(miff);                                                                             \
      if (miffValueGetValueType(value) != miffTypeI || miffValueGetI(value) != VALUE) { printf("ERROR\n"); } else { printf("OK\n"); }

#define TEST_N(KEY)                                                                                            \
      printf("n\t%s\t", KEY);                                                                                  \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeN || !streq(key, KEY)) { printf("ERROR\t"); }                                     \
         else { printf("OK\t"); }                                                                              \
      }
#define TEST_N_VALUE(VALUE)                                                                                    \
      value = miffGetValueN(miff);                                                                             \
      if (miffValueGetValueType(value) != miffTypeN || miffValueGetN(value) != VALUE) { printf("ERROR\n"); } else { printf("OK\n"); }

#define TEST_R(KEY)                                                                                            \
      printf("r\t%s\t", KEY);                                                                                  \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeR || !streq(key, KEY)) { printf("ERROR\t"); }                                     \
         else { printf("OK\t"); }                                                                              \
      }
#define TEST_R4_VALUE(VALUE)                                                                                   \
      value = miffGetValueR(miff);                                                                             \
      if (miffValueGetValueType(value) != miffTypeR || miffValueGetRType(value) == 4 || miffValueGetR4(value) != VALUE)) { printf("ERROR\n"); } else { printf("OK\n"); }
#define TEST_R8_VALUE(VALUE)                                                                                   \
      value = miffGetValueR(miff);                                                                             \
      if (miffValueGetValueType(value) != miffTypeR || miffValueGetRType(value) == 8 || miffValueGetR4(value) != VALUE)) { printf("ERROR\n"); } else { printf("OK\n"); }

#define TEST_TYPE(KEY)                                                                                         \
      printf("t\t%s\t", KEY);                                                                                  \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeTYPE || !streq(key, KEY)) { printf("ERROR\t"); }                                  \
         else { printf("OK\t"); }                                                                              \
      }
#define TEST_TYPE_VALUE(VALUE)                                                                                 \
      value = miffGetValueType(miff);                                                                          \
      if (miffValueGetValueType(value) != miffTypeTYPE || miffValueGetType(value) != VALUE) { printf("ERROR\n"); } else { printf("OK\n"); }

#define TEST_STR(KEY)                                                                                          \
      printf("\"\t%s\t", KEY);                                                                                 \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeSTR || !streq(key, KEY)) { printf("ERROR\t"); }                                   \
         else { printf("OK\t"); }                                                                              \
      }
#define TEST_STR_VALUE(VALUE)                                                                                  \
      value = miffGetValueStr(miff);                                                                           \
      if (miffValueGetValueType(value) != miffTypeSTR || !streq(miffValueGetStr(value), VALUE)) { printf("ERROR\n"); } else { printf("OK\n"); }

#define TEST_COUNT(COUNT) if (COUNT != count) { printf("ERROR\t"); } else { printf("OK\t"); }

#define TEST_NEXT() if (!miffGetRecordEnd(miff)) { printf("ERROR, No Next Record\n"); }

#define TEST_VAR(KEY, TYPE)                                                                                    \
      printf("\"\t%s\t", KEY);                                                                                 \
      if (!miffGetInfo(miff, &type, typeName, key, &arrayCount)) { printf("ERROR, no record.\t"); }            \
      else {                                                                                                   \
         if (type != miffTypeOTHER || !streq(key, KEY) || !streq(typeName, TYPE)) { printf("ERROR\t"); }       \
         else { printf("OK\t"); }                                                                              \
      }


      // Unless you are in full control of the format, do not assume that there will be an order
      // to the values.
      TEST_B("True");      TEST_B_VALUE(miffTRUE);            TEST_NEXT();
      TEST_B("False");     TEST_B_VALUE(miffFALSE);           TEST_NEXT();

      TEST_I("I_0");       TEST_I_VALUE(0);                       TEST_NEXT();
      TEST_I("I_1");       TEST_I_VALUE(1);                       TEST_NEXT();
      TEST_I("I_-1");      TEST_I_VALUE(-1);                      TEST_NEXT();
      TEST_I("I_127");     TEST_I_VALUE(127);                     TEST_NEXT();
      TEST_I("I_-128");    TEST_I_VALUE(-128);                    TEST_NEXT();
                             
      TEST_N("N_0");       TEST_N_VALUE(0);                       TEST_NEXT();
      TEST_N("N_1");       TEST_N_VALUE(1);                       TEST_NEXT();
      TEST_N("N_255");     TEST_N_VALUE(255);                     TEST_NEXT();
                             
      TEST_R("R4_0");      TEST_R4_VALUE(0.0);                    TEST_NEXT();
      TEST_R("R4_1");      TEST_R4_VALUE(1.0);                    TEST_NEXT();
      TEST_R("R4_-1");     TEST_R4_VALUE(-1.0);                   TEST_NEXT();
      TEST_R("R4_PI");     TEST_R4_VALUE(3.1415926535897932f);    TEST_NEXT();
      
      TEST_R("R8_0");      TEST_R8_VALUE(0.0);                    TEST_NEXT();
      TEST_R("R8_1");      TEST_R8_VALUE(1.0);                    TEST_NEXT();
      TEST_R("R8_-1");     TEST_R8_VALUE(-1.0);                   TEST_NEXT();
      TEST_R("R8_PI");     TEST_R8_VALUE(3.1415926535897932);     TEST_NEXT();
                             
      TEST_TYPE("TypeBool"); TEST_TYPE_VALUE(miffTypeBOOL);       TEST_NEXT();
      TEST_TYPE("TypeKey");  TEST_TYPE_VALUE(miffTypeTYPE);       TEST_NEXT();
                             
      TEST_STR("String");    TEST_STR_VALUE("The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
                                                                  TEST_NEXT();
                             
      TEST_B("Bool_Array"); TEST_COUNT(100);
      forCount(index, count)
      {
         value = miffGetValueBoolean(miff);
         if (miffValueGetBool(value) != _bools[index]) { printf("ERROR, value mismatch %d\n", (int) index); break; }
      }
      if (index == count) { printf("OK\n"); }
      TEST_NEXT();

      TEST_I("I_Array"); TEST_COUNT(256);,
      forCount(index, count)
      {
         value = miffGetValueI(miff);
         if (miffValueGetI(value) != _narray[index]) { printf("ERROR, value mismatch %d\n", (int) index); break; }
      }
      if (index == count) { printf("OK\n"); }
      TEST_NEXT();

      TEST_N("N_Array"); TEST_COUNT(256(, );
      forCount(index, count)
      {
         value = miffGetValueI(miff);
         if (miffValueGetN(value) != _narray[index]) { printf("ERROR, value mismatch %d\n", (int) index); break; }
      }
      if (index == count) { printf("OK\n"); }
      TEST_NEXT();

      TEST_R("R4_Array"); TEST_COUNT(300);
      forCount(index, count)
      {
         value = miffGetValueR(miff);
         if (miffValueGetRType(value) != 4 || miffValueGetR4(value) != _reals4[index]) { printf("ERROR, value mismatch %d\n", (int) index); break; }
      }
      if (index == count) { printf("OK\n"); }
      TEST_NEXT();

      TEST_R("R8_Array"); TEST_COUNT(300);
      forCount(index, count)
      {
         value = miffGetValueR(miff);
         if (miffValueGetRType(value) != 8 || miffValueGetR8(value) != _reals8[index]) { printf("ERROR, value mismatch %d\n", (int) index); break; }
      }
      if (index == count) { printf("OK\n"); }
      TEST_NEXT();

      TEST_STR("String_Array"); TEST_COUNT(10);
      forCount(index, count)
      {
         value = miffGetValueSTR(miff);
         if (!streq(miffValueGetSTR(value) _strings[index])) { printf("ERROR, value mismatch %d\n", (int) index); break; }
      }
      if (index == count) { printf("OK\n"); }
      TEST_NEXT();

      TEST_VAR("userTypeIntStrReal", "userVarIntStrReal");
      value = miffGetValueI()
         miffSetValue(          miff, miffValueSetI(42));
         miffSetValue(          miff, miffValueSetString("Yes, but what is the question?"));
         miffSetValue(          miff, miffValueSetR8(3.1415926535897932));
         miffSetRecordEnd(      miff);

         miffSetBlockStart(     miff, "KeyValueBlock");
         {
            miffSetBoolean(        miff, "True",     miffTRUE);
            miffSetBoolean(        miff, "False",    miffFALSE);
                             
            miffSetI(              miff, "I_0",      0);
            miffSetI(              miff, "I_1",      1);
            miffSetI(              miff, "I_-1",     -1);
            miffSetI(              miff, "I_127",    127);
            miffSetI(              miff, "I_-128",   -128);
                             
            miffSetN(              miff, "N_0",      0);
            miffSetN(              miff, "N_1",      1);
            miffSetN(              miff, "N_255",    255);
                             
            miffSetR4(             miff, "R4_0",     0.0);
            miffSetR4(             miff, "R4_1",     1.0);
            miffSetR4(             miff, "R4_-1",    -1.0);
            miffSetR4(             miff, "R4_PI",    3.1415926535897932f);
                             
            miffSetR8(             miff, "R8_0",     0.0);
            miffSetR8(             miff, "R8_1",     1.0);
            miffSetR8(             miff, "R8_-1",    -1.0);
            miffSetR8(             miff, "R8_PI",    3.1415926535897932);
                             
            miffSetType(           miff, "TypeBool", miffTypeBOOL);
            miffSetType(           miff, "TypeKey",  miffTypeTYPE);
                             
            miffSetStr(            miff, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
                             
            miffSetBooleanArray(   miff, "Bool_Array",   100,  _bools);

            miffSetIArray(         miff, "I_Array",      256,  _narray);
            miffSetNArray(         miff, "N_Array",      256,  _narray);

            miffSetR4Array(        miff, "R4_Array",     300,  _reals4);
            miffSetR8Array(        miff, "R8_Array",     300,  _reals8);

            miffSetStrArray(       miff, "String_Array", 10,   _strings);

            miffSetVariableStart(  miff, "userTypeIntStrReal", "userVarIntStrReal");
            miffSetValue(          miff, miffValueSetI(42));
            miffSetValue(          miff, miffValueSetString("Yes, but what is the question?"));
            miffSetValue(          miff, miffValueSetR8(3.1415926535897932));
            miffSetRecordEnd(      miff);
         }
         miffSetBlockStop(miff);

         if (type == miffTypeOTHER)
         {
            wprintf(L"%S\t%S\t%d\t", (char *) typeName, (char *) key, (int) arrayCount);
         }
         else
         {
            wprintf(L"%S\t%S\t%d\t", (char *) miffTypeGetStr(type), (char *) key, (int) arrayCount);
         }

         if      (type == miffTypeBLOCK_STOP ||
                  type == miffTypeBLOCK_START)
         {
            wprintf(L"\n");
         }
         else if (type == miffTypeTYPE)
         {
            wprintf(L"\n");
         }
         else if (type == miffTypeSTR)
         {
            for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
            {
               // Separate strings.
               if (arrayIndex)
               {
                  wprintf(L"\n---");
               }

               value = miffGetValueStr(miff);
               if (miffValueGetValueType(value) == miffTypeNONE)
               {
                  wprintf(L"ERROR\n");
                  break;
               }
               wprintf(L"\n%S ", (char *) miffValueGetStr(value));
            }
            wprintf(L"\n");
         }
         else if (type == miffTypeOTHER)
         {
            value = miffGetValueI(miff);
            if (miffValueGetValueType(value) == miffTypeI &&
                miffValueGetI(        value) == 42)
            {
               wprintf(L"P1 ok, ");
            }
            else
            {
               wprintf(L"ERROR\n");
            }
            value = miffGetValueStr(miff);
            if (miffValueGetValueType(value) == miffTypeSTR &&
                memcmp(
                   (char *) miffValueGetStr(value), 
                   "Yes, but what is the question?", 
                   wcslen(L"Yes, but what is the question?")) == 0)
            {
               wprintf(L"P2 ok, ");
            }
            else
            {
               wprintf(L"ERROR\n");
            }
            value = miffGetValueR8(miff);
            if (miffValueGetValueType(value) == miffTypeR &&
                miffValueGetR8(       value) == 3.1415926535897932)
            {
               wprintf(L"P3 ok");
            }
            else
            {
               wprintf(L"ERROR\n");
            }
            wprintf(L"\n");
         }
         else if (type == miffTypeBOOL)
         {
            for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
            {
               value = miffGetValueBoolean(miff);
               if (miffValueGetValueType(value) == miffTypeNONE)
               {
                  wprintf(L"ERROR\n");
                  break;
               }
               wprintf(L"%c ", (miffValueGetBool(value) == miffTRUE) ? L'T' : L'F');
            }
            wprintf(L"\n");
         }
         else if (type == miffTypeI)
         {
            for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
            {
               value = miffGetValueI(miff);
               if (miffValueGetValueType(value) == miffTypeNONE)
               {
                  wprintf(L"ERROR\n");
                  break;
               }
               wprintf(L"%I64d ", miffValueGetI(value));
            }
            wprintf(L"\n");
         }
         else if (type == miffTypeN)
         {
            for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
            {
               value = miffGetValueN(miff);
               if (miffValueGetValueType(value) == miffTypeNONE)
               {
                  wprintf(L"ERROR\n");
                  break;
               }
               wprintf(L"%I64u ", miffValueGetN(value));
            }
            wprintf(L"\n");
         }
         else if (type == miffTypeR)
         {
            for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
            {
               value = miffGetValueR4(miff);
               if (miffValueGetValueType(value) == miffTypeNONE)
               {
                  wprintf(L"ERROR\n");
                  break;
               }
               wprintf(L"%.6g ", (double) miffValueGetR4(value));
            }
            wprintf(L"\n");
         }
         else if (type == miffTypeR)
         {
            for (arrayIndex = 0; arrayIndex < arrayCount; arrayIndex++)
            {
               value = miffGetValueR8(miff);
               if (miffValueGetValueType(value) == miffTypeNONE)
               {
                  wprintf(L"ERROR\n");
                  break;
               }
               wprintf(L"%.15g ", miffValueGetR8(value));
            }
            wprintf(L"\n");
         }
         else
         {
            wprintf(L"\n");
         }

         if (!miffGetRecordEnd(miff))
         {
            break;
         }
      }

      result = miffTRUE;
      break;
   }

   miffDestroy(miff);
   fclose(file);

   return result;
#endif
}

/******************************************************************************
func: _MiffTestWrite
******************************************************************************/
static MiffB _MiffTestWrite(MiffStr const * const fileName)
{
   FILE     *file;
   Miff     *miff;
   MiffB  result;

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
      miff = miffCreateWriter(miffTRUE, _MiffSetBuffer, "MiffTestFile", 1, (void *) file);
      if (!miff)
      {
         break;
      }

      miffSetBoolean(        miff, "True",     miffTRUE);
      miffSetBoolean(        miff, "False",    miffFALSE);
                             
      miffSetI(              miff, "I_0",      0);
      miffSetI(              miff, "I_1",      1);
      miffSetI(              miff, "I_-1",     -1);
      miffSetI(              miff, "I_127",    127);
      miffSetI(              miff, "I_-128",   -128);
                             
      miffSetN(              miff, "N_0",      0);
      miffSetN(              miff, "N_1",      1);
      miffSetN(              miff, "N_255",    255);
                             
      miffSetR4(             miff, "R4_0",     0.0);
      miffSetR4(             miff, "R4_1",     1.0);
      miffSetR4(             miff, "R4_-1",    -1.0);
      miffSetR4(             miff, "R4_PI",    3.1415926535897932f);
                             
      miffSetR8(             miff, "R8_0",     0.0);
      miffSetR8(             miff, "R8_1",     1.0);
      miffSetR8(             miff, "R8_-1",    -1.0);
      miffSetR8(             miff, "R8_PI",    3.1415926535897932);
                             
      miffSetType(           miff, "TypeBool", miffTypeBOOL);
      miffSetType(           miff, "TypeKey",  miffTypeTYPE);
                             
      miffSetStr(            miff, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
                             
      miffSetBooleanArray(   miff, "Bool_Array",   100,  _bools);

      miffSetIArray(         miff, "I_Array",      256,  _narray);
      miffSetNArray(         miff, "N_Array",      256,  _narray);

      miffSetR4Array(        miff, "R4_Array",     300,  _reals4);
      miffSetR8Array(        miff, "R8_Array",     300,  _reals8);

      miffSetStrArray(       miff, "String_Array", 10,   _strings);

      miffSetVariableStart(  miff, "userTypeIntStrReal", "userVarIntStrReal");
      miffSetValue(          miff, miffValueSetI(42));
      miffSetValue(          miff, miffValueSetString("Yes, but what is the question?"));
      miffSetValue(          miff, miffValueSetR8(3.1415926535897932));
      miffSetRecordEnd(      miff);

      miffSetBlockStart(     miff, "KeyValueBlock");
      {
         miffSetBoolean(        miff, "True",     miffTRUE);
         miffSetBoolean(        miff, "False",    miffFALSE);
                             
         miffSetI(              miff, "I_0",      0);
         miffSetI(              miff, "I_1",      1);
         miffSetI(              miff, "I_-1",     -1);
         miffSetI(              miff, "I_127",    127);
         miffSetI(              miff, "I_-128",   -128);
                             
         miffSetN(              miff, "N_0",      0);
         miffSetN(              miff, "N_1",      1);
         miffSetN(              miff, "N_255",    255);
                             
         miffSetR4(             miff, "R4_0",     0.0);
         miffSetR4(             miff, "R4_1",     1.0);
         miffSetR4(             miff, "R4_-1",    -1.0);
         miffSetR4(             miff, "R4_PI",    3.1415926535897932f);
                             
         miffSetR8(             miff, "R8_0",     0.0);
         miffSetR8(             miff, "R8_1",     1.0);
         miffSetR8(             miff, "R8_-1",    -1.0);
         miffSetR8(             miff, "R8_PI",    3.1415926535897932);
                             
         miffSetType(           miff, "TypeBool", miffTypeBOOL);
         miffSetType(           miff, "TypeKey",  miffTypeTYPE);
                             
         miffSetStr(            miff, "String",   "The quick brown fox\njumped over the lazy dog.\n\t0123456789\n\t`~!@#$%^&*()_+-={}|[]\\:\";\'<>?,./");
                             
         miffSetBooleanArray(   miff, "Bool_Array",   100,  _bools);

         miffSetIArray(         miff, "I_Array",      256,  _narray);
         miffSetNArray(         miff, "N_Array",      256,  _narray);

         miffSetR4Array(        miff, "R4_Array",     300,  _reals4);
         miffSetR8Array(        miff, "R8_Array",     300,  _reals8);

         miffSetStrArray(       miff, "String_Array", 10,   _strings);

         miffSetVariableStart(  miff, "userTypeIntStrReal", "userVarIntStrReal");
         miffSetValue(          miff, miffValueSetI(42));
         miffSetValue(          miff, miffValueSetString("Yes, but what is the question?"));
         miffSetValue(          miff, miffValueSetR8(3.1415926535897932));
         miffSetRecordEnd(      miff);
      }
      miffSetBlockStop(miff);

      result = miffTRUE;
      break;
   }

   miffDestroy(miff);
   fclose(file);

   return result;
}
