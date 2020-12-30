
# M.I. M.I.F.F.: Mining Interchange MIFF

**Author:** Robbert de Groot

**Date:** 2019-05-15

**Copyright:** 2019, Robbert de Groot

**License (Library):** MIT License.

**License (Document):** Creative Commons Attribution-NoDerivs.[LINK](https://creativecommons.org/licenses/by-nd:4.0) (CC BY-ND)

## Table Of Contents:
**1 - M.I. M.I.F.F.**<br />
 1.1 - Discussion<br />
 1.2 - Goals<br />
 1.3 - Disclosure<br />
**2 - MI-MIFF Format**<br />
 2.1 - Header<br />
**3 - Information Block**<br />
**4 - MIFF Format: Mining: Model**<br />
 4.1 - Information Block: Model<br />
 4.2 - Model Block:<br />
  4.2.1 - SubBlock: Fixed<br />
  4.2.2 - SubBlock: Semi<br />
  4.2.3 - SubBlock: Octree<br />
  4.2.4 - SubBlock: Free<br />

# 1 - M.I. M.I.F.F.

## 1.1 - Discussion

What is the purpose of M.I. M.I.F.F. (MI-MIFF)?  MI-MIFF is intended to be a file format for exporting and importing data from one mining software package to another software package.

Users of software packages are becoming increasingly annoyed at the lack of interoperability between software packages.  Currently each software package attempts to do its best at supporting competitor file formats but there is often a lot of loss of data or, as data formats in each package evolves, suffer from compatibility issues.  Often data needs to be dumped to formats that come 'close' but are not exact or down to very basic, raw ASCII files, like csv files, which are often lacking compared to the original data.  Users will need to massage this raw data in order to import it into their target software package.

A lot of the client's time is lost with all this work.  The idea with an interchange format is to remove some of the pain of this data transfer task and put more of the pain on the software vendors of doing the hard part of reading in the data to their expectations.  The software users will not need to know some of the intricacies of the data or task.

## 1.2 - Goals

The main goal of this format is to be specifically an interchange format.  It is not intended to be a native format for any software package.  The focus is to make it easy for the developers from any software package to export their data to this interchange format and, without too much difficulty, to import other vendor's data.

For the users of the software packages, the process of moving data from package to package will be much easier, less time consuming, and less problematic as they will only have to export and import to just one type of file instead of various file formats and requiring more intimate knowledge of the data transfer process specific to their software packages.

* **Simple**.  The format should be simple for the developers to export their data.  Import may still be challenging because of the differences between software packages but the format to import the data should not impose too much pain for the developers either.  The simplicity should go a long way in getting the format adopted and implemented.

* **Inclusive**.  All data from all venders should be representable with the least amount of compromizes in the data being exported.

* **Brief**.  The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**.  The format needs to be able to accomodate change or vendor specific data.  A few software vendors started life in the 70's with time shared machines.  Over the years the amount of data and the types of data the clients want to maintain has changed and grown.  A format needs to try its best to keep up without importers and exporters to be completely remade.


## 1.3 - Disclosure

As of this writing I, Robbert de Groot, am a Principal Software Developer with Hexagon (previously known as (pka) Hexagon Mining, pka MineSight, pka Mintec Inc.), the current owners of MinePlan 3D software (pka MineSight 3D; pka MEDSystem.); Leica Geosystems, the makers of survey equipment and related material; as well as other mine related hardware and software products.

This format is currently not sanctioned by any software vendor, including Hexagon.  This was something I am doing as an experiment to satisfy my curiosity.  It was brought about by recent efforts being made by Global Mining Guidelines (GMG), with their Open Mining Format (OMF), which seems to have the backing of some big players in the industry.  My issue with OMF, for now, is that they only have a Python library that implements the format available publically.  They may have a specification document somewhere but currently it is not available, or at least I cannot find it anywhere in the 'open'.  Plus this format seems to be strictly binary, which is not entirely a bad idea considering the data that needs to be passed around, but binary formats can be very fussy.  And inspecting their sample OMF file, it does not look like something that is easily reverse engineered.

So, I thought I would try my hand at defining a format what I think would be good as an interchange format.  I am trying to write MIFF independent of any software package.  However there may be some unintended bias as I am most familiar with MinePlan 3D for which I have worked on  since 1995.  I do intend to keep this format as inclusive as possible to address other software  package requirements as best as I understand them.  I know I will make mistakes there because I have only cursory knowledge of most of the competing software in the industry.

If anyone is interested in educating me on some features they would like to see with respect to their software package of choice I would love to hear from you.  You can reach me at either of the following two email addresses.

robbert.degroot@hexagon.com<br />zekaric@gmail.com

# 2 - MI-MIFF Format

See the MIFF_Mixed_Information_File_Format.htm file for the discussion of the base file format which is being used for the interchange file format.  This document will cover specifics related to using MIFF for mining as an interchange file format to move data.

The data mining companies need to keep track of and transfer around.

Geometry

3D

* Points

* Polylines (open and closed)

* Surfaces:Generic (open and closed (solids))

* Surfaces:Grid

* Text

* Symbol


2D (Planar, Fence)

* Points

* Polylines (open and closed)

* Text

* Symbol



Drillhole

* Drillhole survery information.

* Drillhole assay information.

* Drillhole composite information.

* Blast hole shot information.

* Chip assay information.

* Design vs surveyed information.


Model

Block model information.

* Regular

* Sub-blocked

* Seam


Seam model information.

* Regular

* Sub-blocked


Surface model information.

* Regular

* Sub-blocked




MI-MIFF will handle all these types of data.  However, how each is actually stored in MIFF can vary.

File naming can help in MIFF file importing.  It is suggested to use the following file extensions...

| Extension | File Contents |
| --- | --- |
| *.miff | Still valid but can be anything.  What is it? |
| *.miffDrill | for Drillhole data. |
| *.miffGeom | for Geometry data. |
| *.miffModel | for Model data. |
| *.miffMI | for a very general Mine Interchange of anything above. |


This information is duplicated inside the file so reading in the file will confirm with the extension but it will help the user if the extension is properly set and will make it easier on the software on how to read in the file if these are properly set.

## 2.1 - Header

MIFF has fields for a Sub-Format Name and Version.  For a MI-MIFF file this will be set to...

|
| Sub-Fromat Name | Mining Interchange |
| Sub-Format Version | 1 |


# 3 - Information Block

Common to all MI-MIFF files for any of the types above, there will be an information block providing general, project wide information.

This information block will be the very first key value in the file right after the MIFF header.

```
info kv
```

Inside the information block we have the following possible key values.  Not all are required.

```
type              "" [file type]
company           "" [company name]
copyright         "" [copyright if any]
author            "" [client employee name]
dateCreated       t* [date created]
dateModified      t* [date modified]
comment           "" [comments]
software          "" [software preparing this file]
softwareVersion   "" [software version]
projectName       "" [name of project area]
projectMin        PR [minimum project area point]
projectMax        PR [maximum project area point]
projectSystem     "" [coordinate system the project is in]
...
```

[file type] is one of...

| Value | Description |
| --- | --- |
| Mixed | Combination of any of the types below. |
| Drillhole | Only holds drillhole data. |
| Geometry | Only holds geometry data. |
| Model | Only holds model data. |
| ... |


# 4 - MIFF Format: Mining: Model

Some of the information found here is based off of the block model formats that are used at Hexagon, where I work, but also from information found freely on the internet.

[LINK](https://www.deswik.com/wp-content/uploads/2019/01/Block-model-knowledge-for-mining-engineers-An-introduction-1.pdf) Block Model Knowledge For Mining Engineers - An Introduction

There are 3 basic model formats that are in use as Hexagon.

* Block Model

* Seam Model

* Surface Model


Block Model is a regular collection of, usually, uniform blocks.  Each block will have values for items that are being tracked by the client.

Seam Model is a bit fancier Block Model.  This model is subdivided uniformly in the row and column directions of the model but instead of levels, seams are described.  Each block in a seam will have an elevation top and bottom to indicate the seam's limits, as well as any other values that are found within that seam block just like a normal block model.

Surface Model is much more limited than the other two as it just holds a stack of surfaces instead of defining any volume.  This model is subdivided uniformly in the row and column directions of the model but instead of seams you will have a surface.  Each 'block', or really the contact point, for a surface will have an elevation value as well as other values that are being tracked by the client at that surface point.

Even though only Block Models are sub-blocked, Mi-MIFF does not limit you to only allowing sub-blocks for Block Models.  The logic could be applied to the other two types of models.  As far as I know, no software vendors sub-block for Seam or Surface Models.

## 4.1 - Information Block: Model

After the general information block the next block will be the Model Information block.

```
infoModel kv
 type             "" [model type]
 isSubblocked     bo [true if subblocked]
 subblockType     "" [subblock type]
 subblockOption   Pn [subblock fixed resolution]
 subblockOption   pn [subblock row col resolution]
 subblockOption   n1 [subblock octtree level count]
 origin           PR [origin point]
 colVec           PR [column vector, length if vector is the length of the block]
 rowVec           PR [row vector, length of the vector is the length of the block]
 levVec           PR [level vector, length of the vector id the length of the block]
 blockCount       Pn [number of column, row, level blocks]
 isLevVariable    bo [true if variable level heights]
 levList          r4a [number of levels] [length] [length] ...
 itemList         kv [item list count]
  item kv
   id          "" [item id]
   nameLong    "" [item name]
   nameShort   "" [item name short]
   type        ty [item type]
   min         r4 [item min value]
   max         r4 [item max value]
   precision   r4 [item precision]
   enumList    ""A [enum count]
   default     [type] [default value for this item]
   calculation "" [calculation field value]
   ...
   \n
  ...
  \n
 \n
\n
```

**[model type]** will be one of...

| Value | Description |
| --- | --- |
| block | For a block model. |
| seam | For a seam model. |
| surface | For a surface model. |


**[subblock type]** will be one of...

| Value | Description
t-
| --- | --- |
| fixed
For a regular division of the block in the east, north, and z directions of the block.

```
Using just a cross section.
Level +--+--+--+--+
  |   |  |  |  |  | <--- Each sub-block of the parent block is of uniform size.
  |   +--+--+--+--+
  |   |  |  |  |  |
  |   +--+--+--+--+
  |   |  |  |  |  |
  V   +--+--+--+--+
      Row or Column --->
```

t-
 | semi
For a regular division of a block in the east and north directions but an infinitely variable z value for block height.

[[ +--+--+--+--+ |  +--+  |  | |  |  +--+  | |  +--+  |  | Infinitely variable in the level direction. +--+  |  |  | +--+--+--+--+ Uniform divisions in the row and column directions.

t-
 | octree
For an oct-tree subvision of a block.

```
+-----+-----+
|     |     |
+--+--+-----+  Recursive bisecting the blocks until a limit.
+--+--+     |
+--+--+-----+
```

t-
 | free
For a free subdivision of a block where each subblock is individually defined as being some subvolume of the parent.

```
+-----------+
|    +----+ |
|    |    | |
|    +----+ | Bad diagram but essentially possible with this option.  This is a catchall case for
+--+        | those types of subblocking I have not addressed.
|  |        |
+--+--------+
```
 |


Depending on what is set for [subblock type] there will be one of 3 [subblockOption] values.

* fixed subblocking requires a Pn to define the number of divisions in the east, north and z directions.

* semiFixed subblocking requires a pn to define the number of divisions in the east and north directions.

* octree subblocking requires a n1 to define the max number of oct-tree divisions that can occur for a block.

* free subblocking does not require an option.


```
       \   /
        \ /
         +  Column 1
        /
 Row 1 /   \   /
      /     \ /
     /Lev 0  +
\   / Row 0 /|
 \ /  Col 0/ |
  +       / <----- colVec (Length is block size. Direction is direction of increasing column index.)
  |\     /   |/
  | \ <----------- rowVec (Length is block size. Direction is direction of increasing row index.)
\ |  \ /    /|
 \|   o <--------- origin (3D point indicating the outter most corner of the first block.)
  +   |   /
  |\  | <--------- levVec (Length can be block size or unit, depends see below. Direction is direction of increasing level index.)
  | \ | /
     \|/
      +
      | Level 1
      |
```

**origin** - The extreme most point of the first column, first row, and first level block of the model.

**colVec, rowVec, levVec** - The direction vectors of increasing block index for the column, row, and level directions.  The length of this vector is the size of the block in the column, row and level directions.  levVec does not need to be sized if the level height is variable.  It should at least be unit length.  All vectors need not be perpendicular to each other but usually this is the case in most software.  I believe there is one software where colVec and rowVec are not 90 degrees to the levVec.  Essentially creating sloped levels.

**blockCount** - The number of column, row, and level blocks.

**isLevVariable** - Flag to indicate the level heights are not uniform.  This does not need to be present if the levels are a fixed height.

**levList** - A list of level heights starting from first level.  Only required if isLevVariable is true.

**itemList** - A key value block for the list of items stored in each block.  Can be using the array notation for this block if item count is known.

**item** - A key value block describing the item.

**id** - A short unique string uniquely identifying the item.

**nameLong** - A 'nice' name for the item.

**nameShort** - A 'compact' name for the item.

**type** - What sort of value the item stores.  One of "", r4, r8, n1, n2, n4, n8, i1, i2, i4, i8, en ("en" for enumeration value), +x ("+x" for calculated field)

**min, max** - limits for an item if a number value.

**precision** - The accuracy of the number if type is r4 or r8.

**enumList** - Required only for "en" type.  This lists the enumeration values available in a string list.

**default** - default value for this item if block is skipped.

**calculation** - a formula that is used to calculate the contents of this value.  Use $[item id] in the formula to refer to another item value in the current block.  Full discussion about the formula composition will be discussed elsewhere.

```
Calculation example:
($ore% * $cu) - $i_am_making_something_up
```

## 4.2 - Model Block:

This is where the model data is placed.  The format is slightly different in that it is a key value list but without value headers and potentially optional values depending on the key.

```
model vs
```

The model data will be enclosed in a value stream chunk.  This stream can be in text, e, or z encodings.  The contents are broken down in key, or key and value pairs.

| Key&nbsp;(1&nbsp;Byte) | Value |
| --- | --- |
| B | n2 value follows immeadiately.  Jump to current level index + N.  Reset current row and current column to 0 |
| b | No value.  Jump to next level. |
| R | n2 value follows immeadiately.  Jump to current row index + N.  Reset currrent column to 0 |
| r | No value.  Jump to next row. |
| C | n2 value follows immeadiately.  Jump to current column index + N. |
| c | No value.  Jump to next column. |
| s | Defining subblock.  See subblocking information below to deal with the various subblock types. |
| V | For the current block or subblock, provide all values for the items defined in the infoModel section in the order they appear in the infoModel (not including calculated items), |
|  | each value separated by a \n in text mode.  In Binary mode values are using as many bytes as necessary as defined by the items. |
|  | Increment the column or subblock column (if applicable). |
|  | (If applicable) If this is the last column or subblock column, column restarts at the first column, increment the row. |
|  | (If applicable) If this is the last row or subblock row, row restarts at the first row, increment the level. |
|  | (If applicable) If this is the last subblock level, move to the next parent block following these rules. |
|  | If this is the last block and last subblock block, there should not be anything else in the value stream. |
| v | No value.  Copy the last blocks values to the current block.  Increment to the next block. |
| . | No value.  Indicates end of the subblock data or value stream. |


The above may not make that much sense yet so here is an example of a 5x5 block model where we have 3 r4 values per block.

```
File                   What is happening
--------------------   -----------------
model vs               Start of the model data value stream.  Current block is 0 (first) level, 0 (first) row, 0 (column)
B2                     Add 2 to the level index.  Current block jumps to 2, 0, 0.  All blocks on levels 0 and 1 have default values.
R3                     Add 2 to the row index. Curent block jumps to 2, 2, 0.  All blocks on level 2, rows 0 and 1 have default values.
C2                     Add 2 to the column index.  Current block jumps to 2, 2, 2.  Block 2, 2, 0, and Block 2, 2, 1 have default values.
v1.2\n2.3\n3.14\n      Block 2, 2, 2 has values 1.2, 2.3, 3.14.  Current block after this line is 2, 2, 2.
c                      Block 2, 2, 3 get default value.  Current block after this line is 2, 2, 4.
c                      Block 2, 2, 4 get default value.  Current block after this line is 2, 3, 0.  We jumped to the next row
s                      Block 2, 3, 0 is subblocked.  Depending on the subblocking used we start at 0, 0, 0 subblock and used the same B, b, R, r, C, c for
                       navigating the subblock blocks until we reach the subblock block or a '.' key is reacched.  See below for subblock filling.
S                      Similar to s but parent block has values as well as having subblocks with values.  First v encountered is for the parent block.
                       c to jump to the first subblock block.
...
.                      Current block can be anything, this indicates that the rest of the model blocks not yet reached are set to default values.

```

Note that there are no \n after every key value pairs.  This is to keep the text representation as compact as possible.  In binary the above will look like...

```
[n1:5]["model"],["vst "],["B"],[n2:2],["R"],[n2:2],["C"],[n2:2],["v"],[r4:1.2],[r4:2.3],[r4:3.14],["c"],["c"],["s"]...,["."]
```

### 4.2.1 - SubBlock: Fixed

In the fixed case of subblocking, as soon as an 's' key is hit, that block is divided into the subblocks as defined in the infoModel section. Use B, b, R, r, C, c, and . in the same way as for the parent blocks.

### 4.2.2 - SubBlock: Semi

In the semi fixed case of subblocking, as soon as an 's' key is hit, that block is divided into the subblocks as defined in the infoModel section.  One difference...

In this scheme of subblocking, there is an infinite variability in the z direction for each subblock.  This is defined by an 'H' key for the first level of blocks of the subblocks.  The number of level subblocks will depend on how many times H is used for the same column.

| Key | Value |
| --- | --- |
| H | r4 value immediately follows.  The r4 value is a percent of the block height.  This record needs to preceed a v record. |
| h | No value.  The rest of the subblock cylinder is used. |


If H or h follows an H or h then the previous block will be holding default values, and the current block will be incremented to the next subblock.  The following H or h will determin the height of the new current block.

If a subblock cell has already defined the entire height of the block then these will be skipped over when jumping to the next subblock.  This means that some subblock cells could have a different number of levels that others.

### 4.2.3 - SubBlock: Octree

In the octree case, an 's' key will split the block or subblock in to 8 uniform pieces.  If we are already at a subblock then that subblock will be split into small 8 uniform pieces.  This should not exceed the subblock option count defined in the model information block.

The 's' key need to preceed the v record.

### 4.2.4 - SubBlock: Free

In the free block case, an 's' key will indicate the parent block is subblocked.  However after that you will need to define the blocks manually.

| Key | Value |
| --- | --- |
| c | 6 x r4 values follow.  First 3 r4 values are percents for the min point and the second 3 r4 values are percents for the max point of the subblock. |
| . | Indicates we are done with the subblocking and to process to the next parent block. |

