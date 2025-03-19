# M.I. File: Mine Information File

```
Author:             Robbert de Groot
Date:               2019-05-15
Copyright:          2019, Robbert de Groot
License (Library):  MIT License.
License (Document): Creative Commons Attribution-NoDerivs. CC BY-ND https://creativecommons.org/licenses/by-nd:4.0
```

## Table Of Contents:

- [M.I. File: Mine Information File](#mi-file-mine-information-file)
  - [Table Of Contents:](#table-of-contents)
- [1 - M.I.](#1---mi)
  - [1.1 - Discussion](#11---discussion)
  - [1.2 - Goals](#12---goals)
  - [1.3 - Disclosure](#13---disclosure)
- [2 - MI Format](#2---mi-format)
  - [2.1 - Header](#21---header)
- [3 - Mine Information Blocks](#3---mine-information-blocks)
  - [3.1 - Data Block](#31---data-block)
  - [3.2 - Property Block](#32---property-block)
  - [3.3 - Item Block](#33---item-block)
  - [3.4 - Image Block](#34---image-block)
  - [3.5 - Drillhole Block](#35---drillhole-block)
  - [3.6 - Geometry Block](#36---geometry-block)
  - [3.7 - Model Data](#37---model-data)
    - [3.7.1 - Model Block](#371---model-block)
  - [3.8 - Model Block:](#38---model-block)
    - [3.8.1 - SubBlock: Fixed](#381---subblock-fixed)
    - [3.8.2 - SubBlock: Semi](#382---subblock-semi)
    - [3.8.3 - SubBlock: Octree](#383---subblock-octree)
    - [3.8.4 - SubBlock: Free](#384---subblock-free)

# 1 - M.I.

## 1.1 - Discussion

**What is the purpose of M.I. (MI)?**

MI is intended to be a file format for exporting and importing data from one mining software package to another software package.

I would imagine users of software packages are annoyed at the lack of interoperability between software packages.  Currently some software packages attempt to do their best at supporting competitor file formats (read only) but there is often a lot of loss of data or, as data formats in each package evolves, suffer from compatibility issues.  Often data needs to be dumped to formats that come 'close' but are not exact, like DXF/DWG, or down to very basic ASCII files, like csv files, which are often lacking compared to the original data.  Users will need to massage this raw data in order to import it into their target software package.

A lot of the client's time is lost with all this work and often lock them into one system because getting data out or in from that system is too onerous.  The idea with an information format is to remove some of the pain of this data transfer task and put more of the pain on the software vendors of doing the hard part of reading in the data to their expectations.  The software users will not need to know some of the intricacies of the data or task.

## 1.2 - Goals

The main goal of this format is to be specifically a data passing format.  It is not intended to be a native format for any software package.  The focus is to make it easy for the developers from any software package to export their data to this format and, without too much difficulty, to import into another vendor's software.

For the users of the software packages, the process of moving data from package to package will be much easier, less time consuming, less error prone, and less data loss as they will only have to export and import to just one type of file instead of various file formats and requiring more intimate knowledge of the data transfer process specific to their software packages.


* **Simple**, The format should be simple for the developers to export their data.  Import may still be challenging because of the differences between software packages but the format to import the data should not impose too much pain for the developers either.  The simplicity should go a long way in getting the format adopted and implemented.
* **Inclusive**, All data from all venders should be representable with the least amount of compromises in the data being exported.
* **Brief**, The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.
* **Flexible**, The format needs to be able to accomodate change or account for vendor specific data.  A few software vendors started life in the 70's with time shared machines.  Over the years the amount of data and the types of data the clients want to maintain has changed and grown.  A format needs to try its best to keep up without importers and exporters to be completely remade.

## 1.3 - Disclosure

As of this writing I, Robbert de Groot, am a Principal Software Developer with Hexagon (previously known as (pka) Hexagon Mining, pka MineSight, pka Mintec Inc.), the current owners of MinePlan 3D software (pka MineSight 3D; pka MEDSystem.); Leica Geosystems, the makers of survey equipment and related material; as well as other mine related hardware and software products.

This format is currently not sanctioned by any software vendor, including Hexagon.  This was something I am doing as an experiment to satisfy my curiosity.  It was brought about by recent efforts being made by Global Mining Guidelines (GMG), with their Open Mining Format (OMF), which seems to have the backing of some big players in the industry.  My issue with OMF, for now, is that they only have a Python library that implements the format available publically.  The format is strictly binary, which is not entirely a bad idea considering the data that needs to be passed around, but binary formats can be very fussy.  The format is also based on a ZIP archive.  The good thing about ZIP is that it will have excellent compression.  The bad thing is that ZIP archives are, in my opinion, not a great file format.

So, I thought I would try my hand at defining a format that I think would be good as an information format.  I am trying to write MI independent of any software package.  However there may be some unintended bias as I am most familiar with MinePlan 3D for which I have worked on  since 1995.  I do intend to keep this format as inclusive as possible to address other software  package requirements as best as I understand them.  I know I will make mistakes there because I have only superficial knowledge of most of the competing software in the industry.

If anyone is interested in educating me on some features they would like to see with respect to their software package of choice I would love to hear from you.  You can reach me at the following email address.

zekaric@gmail.com

# 2 - MI Format

MI is not restricted to any particular format.  Here I will specify two, MIFF and JSON.  See the MIFF_Mixed_Information_File_Format document for the discussion of the MIFF file format.  Refer to JSON.org for information about JSON.  This document will cover specifics related to using MI but it will not be restricted to any particular underlying format.

The library connect to this document will read and write to either MIFF or JSON.  XML could be an option but I would not suggest it.

The data mining companies need to keep track of and transfer around.

```
- General information
  - Project limits
  - Project coordinate space
  - Project units
  - Company information
- Value Specifications
  - Project items and limits
  - Project item binning
  - Display properties
- Geometry
  - 3D
    - Points
    - Polylines (open and closed)
    - Surfaces:Generic (open and closed (solids))
    - Surfaces:Grid
    - Text
    - Symbol
  - 2D (Planar, Fence)
    - Points
    - Polylines (open and closed)
    - Text
    - Symbol
  - Survey
    - Points, Polylines but using angles instead of coordinates.
- Drillhole
  - Drillhole survery information.
  - Drillhole assay information.
  - Drillhole composite information.
  - Blast hole shot information.
  - Chip assay information.
  - Design vs Actual information.
- Model
  - Block model information.
    - Block
      - Regular (all blocks the same)
      - Plan Regular, Variable Bench (all blocks same i and j dimensions but k (bench) can vary.)
      - Octree
      - Regular sub-blocked (all sub-blocks are the same)
      - Multi-level sub-blocked
    - Seam
    - Surface
```

File extsion for the file is either...

| Extension | File Contents |
| --- | --- |
| &#42;.MIMIFF | For a MineInfo file using MIFF format |
| &#42;.MIJSON | For a MineInfo file using JSON format |

## 2.1 - Header

MIFF has fields for a Sub-Format Name and Version.  For an MI file this will be set to...

| Field | Value |
| --- | --- |
| Sub-Fromat Name | Mining Information |
| Sub-Format Version | 1 |

For a JSON file, since it doesn't have a header, create base object for all the blocks in the file.  First block should be the "format" block.

```
{
   "format":{
      "name":"Mining Information",
      "version":1,
   },
   ...
}
```

# 3 - Mine Information Blocks

An MI file can only have information for just one mining project or project area.  Use multiple files for different mines, sites, projects, or however you separate the information.

Every MI file contains a few specified blocks.  Order of these blocks are important for being able to read the file in one pass.  This means that types, limits or information that is required for other data in the file needs to be defined before it is used.

* **Information block** to hold general information about the project.  This must be the first block.
* **Image list block** to hold the images.  Images can be used for texturing, symbols, etc.
* **Property list block** to hold display properties for geometry or data.
* **Item list block** to hold the item definitions.  Items being the values that are stored in the drillhole, model, cuts, and whatever else that is being tracked.  There will only be one Item List block.
* **Geometry list block** to hold geometry information.  Depends on item, image, and property blocks.
* **Drillhole list block** to hold drillhole information.  Depends on item, image, and property blocks.
* **Model list block** to hold model information.  Depends on item, image, and property blocks.

## 3.1 - Data Block

**Block Name**: data

Common to all MI files for any of the types above, there will be an information block providing general, project wide information.

This information block should be before other blocks.

Inside the information block we have the following possible key values.  Not all are required.

| Name | Description |
| --- | --- |
| company | The name of the company related to the mine. |
| copyright | Copyright information |
| author | Author or person who prepared this data. |
| software | The software that was used to prepare this data. |
| softwareVersion | The version of the above software. |
| comment | Any comments provided by the author or software about the data.  This comment should hold no information that needs to be parsed by an importer of the data. |
| projectSystem | Indicates what coordinates system the all coordinate data should be considered to be in.  It will be a string as defined in a package like GDAL which can translate from one coordinate system to another.  Like **WGS84** or **NAD27**.  Use **local** to indicate that the project coordinates are in a local flat earth space. |
| projectName | The name of the project. |
| projectMin | Array of 3 real values in the given project system. |
| projectMax | Array of 3 real values in the given project system. |
| other | A place to put other information that is not accounted for by the MI format but will be useful by the original software that wrote the file.  This will be an array of string values. |

**projectMin** and **projectMax** should given an indication on the data range of the project.  This does not need to be exactly defining the outer limits of the all the data, just the rough range that the data should live inside or near.  It should be in the coordinate system defined by **projectSystem**.

## 3.2 - Property Block

**Block Name**: prop

This can be found on its own or inside a prop list of an item block below.  What is found in a property block are information about an item value range or a geometry.  It can include display/rendering cues for whatever software that reads the MI.

| Name | Description |
| --- | --- |
| key | A unique string key for the property. |
| name | A nice name for the property.  Optional. |
| value | If this property block is in an item block's prop list then a value will be present.  The value will match the value type of the item.  Depending on the type the value will be an exact match or, if it is a number value, it will be the minimum value that an attribute needs to be in order to use the properties. |
| vis, point vis, line vis, face vis, drill vis, model vis | boolean values for a global visibility, point visibility, line visibility, face visibility, drillhole geometry vis, and model geometry vis to be used when rendering the geometry.  Global trumps the rest of the visibilities.  Default visibilities will be used if these are missing. |
| color, point color, line color, face color, face style color, drill color, model color | An rgba value for a point color, line color, face color, drillhole color, and model color to be used when rendering the geometry.  Default color will be used if these are missing. |
| point style, line style, face style | A string representing the point style (symbol), line style (pattern), and face style (pattern) to be used when rendering the geometry.  Default styles as dictated by the software reading the MI will be used if these are missing. |
| other | A string key and value pair array of any information that the software feels necessary for the property. |

## 3.3 - Item Block

**Block Name**: item

Item will describe a value that will be stored in the various places where the item is being tracked.  For instance, a drillhole assay value, a block model item value, a geometry element value, a geometry point value, etc.

Items need to be defined before they are used elsewhere in the MI file.

| Name | Description |
| --- | --- |
| key | A unique key for the item.  This is stored as a string even if it is a number by the software that sets it.  This key needs to be unique so that no two items have the same key value. |
| name | The name of the item.  A UTF8 string limited to 255 bytes.  Not necessarily unique but generally, it would be a good idea. |
| type | The type of the item.  See lower down. |
| range | If a number type, the minimum and maximum value for the item. |
| value | If the item has limited number of values, list the array of values here. |
| precision | If a real type, how many decimal places is this values accurrate to. |
| default | The default value for something that did not have a value assigned. |
| calculation | The formula used to calculate the value of this item.  In other words, this item has not stored value.  The value will be calculated when needed. |
| prop list | An array of properties for rendering purposes in a view.  See Display block for what this might mean.  First property in this array will define the properties for something that does not match any other prop block's values.  The prop blocks in this array should be in order of increasing value if the item type is a number value. |
| other | A place to put other information that is not accounted for by the MI format but will be useful by the original software that wrote the file.  This will be an array of string values. |

Each item will be given an index value starting at 0 and incrementing with every new item added to the MI file.

**type** will define the value being stored for this item.  Type is one of string, real, real4, natural, integer, date, time, dateTime, or formula (formula meaning the item's value is calculated.)

**range** defines the limits for an item.  Only applicable if item type is a real, real4, natural, or integer.  If these are missing then the range is the full range as defined by the type.

**valueList** defines the actual values that will ever be used for this item.  This can exist for any type.  If values are used then what is stored in the drillhole assay, block model block, or geometry attribute will be an index into this list of values.  A string type and a valueList essentially defines an enumerated value.

**precision** is the accuracy of the number.  Only applicable if item type is a real and real4.

**default** will define the default value for this item if block is skipped.  If this value is not set then the default value is 'unset'.

**calculation** is an equation that is performed to obtain the item's value.  Only applicable if item type is formula.  Use "@[item key]@" in the formula to refer to another item value in the current context.  Full discussion about the formula composition will be discussed elsewhere.

When precision is used, it will change how the values are stored.  Say we have a min of 0 and an max of 10 and a precision of 0.1.  As a result we have a total of 101 unique values. 0.0, 0.1, ..., 9.9, 10.0.  102 unique values when we include 'unset value' as well.  If this is all the values that need to be recorded, then we do not really need to use a full real or real4 value to store it.  We will be storing instead a natural value such that the natural value, multiplied by the precision and added to the min value will recover the real value which is being stored.  In this case, 102 values only requires a single byte natural to encode all the values in the range.

```
Calculation example:
(@ore%@ &#42; @cu@) - @i_am_making_something_up@
```

## 3.4 - Image Block

**Block Name**: image

Defining an image.  Images are mainly used for texture mapping in geometry blocks.

| Name | Description |
| --- | --- |
| key | Unique identifier for the image. |
| name | A nice name for the image. |
| rgb | The pixels as a raw image.<br />[width int][tb][height int][tb][hex dump of image pixels in 8 bit rgb values, no compression.] |
| rgba | The pixels as a raw image.<br />[width int][tb][height int][tb][hex dump of image pixels in 8 bit rgba values, not compression.] |
| file | A hex dump of the image file. |
| filePath | A relative path to an image file.  The file should be relative to the MI file and should be in the same folder or in a sub folder of this folder.  The file should be an image file. |

An image will only have one of the following 4 items (rgb, rgba, file, filePath) and never more than one.

## 3.5 - Drillhole Block

Drillhole data holds information about drillholes.

## 3.6 - Geometry Block

**Block Name**: geom

Defining a piece of geometry.  There can be multiple geometry blocks within an MI file.

| Name | Description |
| --- | --- |
| key | A string that is a unique identifier for the geometry item. |
| name | A nice name for the geometry if any. |
| image | key to the image being used for texturing. |
| prop | Key to the property block to use to render the geometry. |
| attr item list geom | An array of item keys that are defined for the geometry as a whole. |
| attr item list point | An array of item keys that are defined for the geometry points.  Only used for Polyline geometry and is optional. |
| attr item list line | An array of item keys that are defined for the geometry segments.  A segment is the connection between two points.  Only used for Polyline geometry and is optional. |
| type | The type of the geometry.  One of "label", "point", "polyline", "surface", "mesh", "grid" |
| point list | The point list for the geometry.  This will depend on the type above. |
| face list | If the type is a surface there will be a face connectivity list.  Each face will be a triangle and be composed of 3 indicies into the point list. |
| uv list | If the type is a surface or mesh, there could be a uv texture list.  One for every point in the point list. |
| color list point | If the type is polyline, surface, or mesh then the geometry could be colored at the point level.  The blending over the face or line segment will be smooth from one color to the next. |
| color list face | If the type is surface, or mesh then the geometry could be colored at the face level.  The face will be a solid color. |
| attr list geom | The geometry level attribute values.  One value per item in the attr item list geom. |
| attr list point | The point level attribute values.  One per point in the point list.  One value per item in the attr item list point. |
| attr list line | The line level attribute values.  One per line segment in the point list.  One value per item in the attr item list line. |


Each piece of geometry can have some attributes.  The attributes need to be an item in the item list and the list of attributes is defined by attrList array.  When this array is read in, a user type is create by using the item list items' definition to build it up.  The values in the user type will be in the order listed.

Each piece of polyline geometry can have point and line attributes.  Like geometry attributes above, the user type will be dynamically created by the library based off of the items listed and in that order.

attrList, pointAttrList, and lineAttrList can change between geom blocks and they need to be defined before the geom blocks which will use them.

In binary the type number for the attr, pointAttr, and lineAttr will be all 1's (4095) as we will know what type to use based off of this format.

## 3.7 - Model Data

Some of the information found here is based off of the block model formats that are used at Hexagon, where I work, but also from information found freely on the internet.

[LINK](https://www.deswik.com/wp-content/uploads/2019/01/Block-model-knowledge-for-mining-engineers-An-introduction-1.pdf) Block Model Knowledge For Mining Engineers - An Introduction

There are 3 basic model formats that are in use as Hexagon.

* Block Model

* Seam Model

* Surface Model

**Block Model** is a regular collection of, usually, uniform blocks.  Each block will have values for items that are being tracked by the client.

**Seam Model** is a bit fancier Block Model.  This model is subdivided uniformly in the row and column directions of the model but instead of levels, seams are described.  Each block in a seam will have an elevation top and bottom to indicate the seam's limits, as well as any other values that are found within that seam block just like a normal block model.

**Surface Model** is much more limited than the other two as it just holds a stack of surfaces instead of defining any volume.  This model is subdivided uniformly in the row and column directions of the model but instead of seams you will have a surface.  Each 'block', or really the contact point, for a surface will have an elevation value as well as other values that are being tracked by the client at that surface point.

Even though only Block Models are sub-blocked, MI does not limit you to only allowing sub-blocks for Block Models.  The logic could be applied to the other two types of models.  As far as I know, no software vendors sub-block for Seam or Surface Models.  In general, if you are sub-blocking for these other two cases, you are just going to define a model at the resolution of the sub-block size instead of this two layered approach in my opinion.

### 3.7.1 - Model Block

After the general information block the next block will be the Model Information block.

```
{	model
"	type	1	[model type]
b	is subblocked	1	[true if subblocked]
"	subblock type	1	[subblock type]
abcn4	subblock option	1	[subblock fixed resolution]
abn4	subblock option	1	[subblock row col resolution]
n1	subblock option	1	[subblock octtree level count]
abcr8	origin	1	[origin point]
abcr8	column vector	1	[column vector, length if vector is the length of the block]
abcr4	row vector	1	[row vector, length of the vector is the length of the block]
abcr4	level vector	1	[level vector, length of the vector id the length of the block]
abcn4	block count	1	[number of column, row, level blocks]
b	is level size variable	1	[true if variable level heights]
r4	level list	[number of levels] [length]&#42;
"	item list	[item Count]	[name of item]&#42;
}
```

**[model type]** will be one of...

| Value | Description |
| --- | --- |
| block | For a block model. |
| seam | For a seam model. |
| surface | For a surface model. |

**[subblock type]** will be one of...

**fixed** - For a regular division of the block in the east, north, and z directions of the block.

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

**semi** - For a regular division of a block in the east and north directions but an infinitely variable z value for block height.

```
+--+--+--+--+
|  +--+  |  |
|  |  +--+  |
|  +--+  |  | Infinitely variable in the level direction.
+--+  |  |  |
+--+--+--+--+
Uniform divisions in the row and column directions.
```

**octree** - For an oct-tree subvision of a block.

```
+-----+-----+
|     |     |
+--+--+-----+  Recursive bisecting the blocks until a limit.
+--+--+     |
+--+--+-----+
```

Depending on what is set for **[subblock type]** there will be one of 3 **[subblockOption]** values.

* fixed subblocking requires a **v3n4** to define the number of divisions in the east, north and z directions.
* semiFixed subblocking requires a **v2n4** to define the number of divisions in the east and north directions.
* octree subblocking requires a **n1** to define the max number of oct-tree divisions that can occur for a block.

```
       \   /
        \ /
         +  Column 1
        / \
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
  |\  | <--------- levVec (Length can be block size or unit, depends see below. Direction is
  | \ | /                  direction of increasing level index.)
     \|/
      +
      | Level 1
      |
```

**origin** - The extreme most point of the first column, first row, and first level block of the model.

**colVec, rowVec, levVec** - The direction vectors of increasing block index for the column, row, and level directions.  The length of this vector is the size of the block in the column, row and level directions.  levVec does not need to be sized if the level height is variable.  It should at least be unit length.  All vectors need not be perpendicular to each other but usually this is the case in most software.  I believe there is one software where colVec and rowVec are not 90 degrees to the levVec.  Essentially creating non-square/cuboid blocks.

**blockCount** - The number of column, row, and level blocks.

**isLevVariable** - Flag to indicate the level heights are not uniform.  This does not need to be present if the levels are a fixed height.

**levList** - A list of level heights starting from first level.  Only required if isLevVariable is true.

## 3.8 - Model Block:

This is where the model data is placed.  Due to the size of the data, the keys are made very small.

```
{	model
```

The model data will be enclosed in a value stream chunk.  The contents are broken down in key, or key and value pairs.

The current block values to encode each item in a block will be the default values of those items.  Which might be 'missing' values or some specific value.

| Key (1 Byte) | Value |
| --- | --- |
| L | n2 value follows immediately.  Jump to current level index + N.  Reset current row, current column, and current item to 0.  All blocks that were jumped over are set to the last block's value. |
| l | No value.  Jump to next level.  Reset current row, current column, and current item to 0.  All blocks that were jumped over are set to the last block's value. |
| R | n2 value follows immediately.  Jump to current row index + N.  Reset currrent column and current item to 0.  All blocks that were jumped over are set to the last block's value. |
| r | No value.  Jump to next row.  Reset current column and current item to 0.  All blocks that were jumped over are set to the last block's value. |
| C | n2 value follows immediately.  Jump to current column index + N.  Reset current item to 0.  All blocks that were jumped over are set to the last block's value. |
| c | No value.  Jump to next column.  Reset current item to 0.  The block that was jumped over is set to the last block's value. |
| s | Defining subblock but parent block as values.  Provide parent block item values before subblock values.  'c' follows immediately if parent block has no values. |
| I | n1 value follows immediately.  Jump to item index + N.  Skipped over items get default values. |
| i | Jump to next item.  Skipped over item gets default value. |
| V | n1 value follows.  Provide the values for the next n1 items. |
| v | Provide the values for the rest of the block or subblock. |
| D | n1 value follows immediately. Duplicate the next n1 values from the last block or subblock. |
| d | Duplicate the remaining values from the last block or subblock. |
| X | n1 value follows immediately.  Clone last block n1 times.  This is for all the item values. |
| . | No value.  Indicates end of the item date or subblock data. |
| \n | No value.  Indicates end value stream. |


The above may not make that much sense yet so here is an example of a 5x5 block model where we have 3 i4 values per block.

```
File                   What is happening
--------------------   -----------------
model [...]-\n         Start of the model data value stream.
                       Current block is 0 (first) level, 0 (first) row, 0 (column)
B2                     Add 2 to the level index.
                       Current block jumps to 2, 0, 0.
                       All blocks on levels 0 and 1 have default values.
R2                     Add 2 to the row index.
                       Curent block jumps to 2, 2, 0.
                       All blocks on level 2, rows 0 and 1 have default values.
C2                     Add 2 to the column index.
                       Current block jumps to 2, 2, 2.
                       Block 2, 2, 0, and Block 2, 2, 1 have default values.
v1 2 3                 Block 2, 2, 2 has values 1, 2, 3.
                       Current block jumps to 2, 2, 3.
c                      Block 2, 2, 3 get default value.
                       Current block after this line is 2, 2, 4.
c                      Block 2, 2, 4 get default value.
                       Current block after this line is 2, 3, 0.
                       We jumped to the next row
\n                     This indicates that the rest of the model is set to default values.
```

Note that there are no \n after every key value pairs.  This is to keep the text representation as compact as possible.  So in actual text file this will look like...

```
{	model
B2R2C2v1 2 3cc.\n
}
```

Note, if you can removed a separator without losing then that should be done.

### 3.8.1 - SubBlock: Fixed


In the fixed case of subblocking, as soon as an 's' key is hit, that block is divided into the subblocks as defined in the infoModel section. Use B, b, R, r, C, c, and . in the same way as for the parent blocks.

### 3.8.2 - SubBlock: Semi


In the semi fixed case of subblocking, as soon as an 's' key is hit, that block is divided into the subblocks as defined in the infoModel section.  One difference...

In this scheme of subblocking, there is an infinite variability in the z direction for each subblock.  This is defined by an 'H' key for the first level of blocks of the subblocks.  The number of level subblocks will depend on how many times H is used for the same column.

| Key | Value |
| --- | --- |
| H | r4 value immediately follows.  The r4 value is a percent of the block height.  This record needs to preceed a v record. |
| h | No value.  The rest of the subblock cylinder is used. |


If H or h follows an H or h then the previous block will be holding default values, and the current block will be incremented to the next subblock.  The following H or h will determin the height of the new current block.

If a subblock cell has already defined the entire height of the block then these will be skipped over when jumping to the next subblock.  This means that some subblock cells could have a different number of levels that others.

### 3.8.3 - SubBlock: Octree


In the octree case, an 's' key will split the block or subblock in to 8 uniform pieces.  If we are already at a subblock then that subblock will be split into small 8 uniform pieces.  This should not exceed the subblock option count defined in the model information block.

The 's' key need to preceed the v record.

### 3.8.4 - SubBlock: Free


In the free block case, an 's' key will indicate the parent block is subblocked.  However after that you will need to define the blocks manually.

| Key | Value |
| --- | --- |
| c | 6 x r4 values follow.  First 3 r4 values are percents for the min point and the second 3 r4 values are percents for the max point of the subblock. |
| . | Indicates we are done with the subblocking and to process to the next parent block. |

