
# M.I. M.I.F.F.: Mine Information MIFF


**`Author:            `** Robbert de Groot

**`Date:              `** 2019-05-15

**`Copyright:         `** 2019, Robbert de Groot

**`License (Library): `** MIT License.

**`License (Document):`** Creative Commons Attribution-NoDerivs.[LINK](https://creativecommons.org/licenses/by-nd:4.0) (CC BY-ND)

## Table Of Contents:

**1 - M.I. M.I.F.F.**<br />
    1.1 - Discussion<br />
    1.2 - Goals<br />
    1.3 - Disclosure<br />
**2 - MI-MIFF Format**<br />
    2.1 - Header<br />
**3 - Mine Information Block (MIB)**<br />
    3.1 - Type List Block<br />
    3.2 - Information Block<br />
    3.3 - Item List Data<br />
        3.3.1 - Item Data<br />
    3.4 - Drillhole Data<br />
    3.5 - Geometry List Data<br />
        3.5.1 - Geometry Data<br />
    3.6 - Model Data<br />
        3.6.1 - Model Information<br />
    3.7 - Model Block:<br />
        3.7.1 - SubBlock: Fixed<br />
        3.7.2 - SubBlock: Semi<br />
        3.7.3 - SubBlock: Octree<br />
        3.7.4 - SubBlock: Free<br />

# 1 - M.I. M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I. M.I.F.F. (MI-MIFF)?  MI-MIFF is intended to be a file format for exporting and importing data from one mining software package to another software package.

I would imagine users of software packages are annoyed at the lack of interoperability between software packages.  Currently each software package attempts to do its best at supporting competitor file formats (reading only) but there is often a lot of loss of data or, as data formats in each package evolves, suffer from compatibility issues.  Often data needs to be dumped to formats that come 'close' but are not exact, like DXF/DWG, or down to very basic ASCII files, like csv files, which are often lacking compared to the original data.  Users will need to massage this raw data in order to import it into their target software package.

A lot of the client's time is lost with all this work and often lock them into one system because getting data out or in from that system is too onerous.  The idea with an Information format is to remove some of the pain of this data transfer task and put more of the pain on the software vendors of doing the hard part of reading in the data to their expectations.  The software users will not need to know some of the intricacies of the data or task.

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

So, I thought I would try my hand at defining a format what I think would be good as an Information format.  I am trying to write MIFF independent of any software package.  However there may be some unintended bias as I am most familiar with MinePlan 3D for which I have worked on  since 1995.  I do intend to keep this format as inclusive as possible to address other software  package requirements as best as I understand them.  I know I will make mistakes there because I have only superficial knowledge of most of the competing software in the industry.

If anyone is interested in educating me on some features they would like to see with respect to their software package of choice I would love to hear from you.  You can reach me at the following email address.

zekaric@gmail.com

# 2 - MI-MIFF Format


See the MIFF_Mixed_Information_File_Format.html file for the discussion of the base file format which is being used for the Information file format.  This document will cover specifics related to using MIFF for mining as an Information file format to move data.

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
- Drillhole
  - Drillhole survery information.
  - Drillhole assay information.
  - Drillhole composite information.
  - Blast hole shot information.
  - Chip assay information.
  - Design vs surveyed information.
- Model
  - Block model information.
    - Regular
    - Sub-blocked
    - Seam
  - Seam model information.
    - Regular
    - Sub-blocked
  - Surface model information.
    - Sub-blocked
```

File naming can help in MIFF file importing.  It is suggested to use the following file extensions...

| Extension | File Contents |
| --- | --- |
| &#42;.miff | Still valid but can be anything.  What is it? |
| &#42;.miffMI | for a very general Mine Information of anything above. |
| &#42;.miffMIDrill | for only Drillhole related data. |
| &#42;.miffMIGeom | for only Geometry related data. |
| &#42;.miffMIModel | for only Model related data. |


This information is duplicated inside the file so reading in the file will confirm with the extension but it will help the user if the extension is properly set and will make it easier on the software on how to read in the file if these are properly set.

## 2.1 - Header


MIFF has fields for a Sub-Format Name and Version.  For a MI-MIFF file this will be set to...

| Field | Value |
| --- | --- |
| Sub-Fromat Name | MiningInformation |
| Sub-Format Version | 1 |


# 3 - Mine Information Block (MIB)


An MIB is not limited to live in just an MI MIFF file.  Also multiple MIBs could potentially live in one MI MIFF files where each MIB represents a totally different mining project.

```
MineInfo []-
...
\n
```

See MIFF format on how to represent in binary.

Inside a MIB you will have the following blocks.


* **Type List block** to hold the user types that are used in the format.

* **Information block** to hold general information about the project.  This must be the first block.

* **Item List block** to hold the item definitions.  Items being the values that are stored in the drillhole, model, cuts, and whatever else that is being tracked.  There will only be one Item List block.

* **Image List block** to hold the images.  Images can be used for texturing, symbols, etc.

* **Geometry Block** to hold geometry information.  There can be more than one Geoemtry block.

* **Drillhole block** to hold drillhole information.  Item List block must already be defined or else you are not providing assay or survey values with your drillhole data.  There can be more than one Drillhole block.

* **Model Block** to hold model information.  Item List block must already be defined or else you are not providing any values with your model data.  There can be more than one Model block.

## 3.1 - Type List Block


This block is optional.  Types could be anywhere as long as the types are defined before they are used.  It's just nice to have them all located in one place.

```
typeList []-\n
 date             userType-\n
  year            i4-\n
  mon             n1- Jan = 1, Dec = 12\n
  day             n1- day 1 = 1, day 31 = 31\n
 \n
 time             userType-\n
  hour            n1- 24 hour time in GMT time zone, no daylights adjustment, midnight = 0\n
  min             n1- 0 - 59\n
  sec             n1- 0 - 59\n
 \n
 dateTime         userType-\n
  year            i4-\n
  mon             n1- Jan = 1, Dec = 12\n
  day             n1- day 1 = 1, day 31 = 31\n
  hour            n1- 24 hour time in GMT time zone, no daylights adjustment, midnight = 0\n
  min             n1- 0 - 59\n
  sec             n1- 0 - 59\n
 \n
 imageId          userType-\n
  id              n4-\n
 \n
 enumCode         userType- There should be enum definitions for all your enums.\n
  code            n4- This can vary.  Use an appropriat size n* to cover the values.\n
 \n
 enumValue        userType-\n
  code            enumCode-\n
  value           ""-\n
 \n
 enumList         enumValue= 3\n
  0\n
  `value1\n
  1\n
  `value2\n
  999\n
  `value3\n
 \n
 azimuth          userType-\n
  degree          r4- North = 0.0, East = 90.0, South = 180.0, and West = 270.0\n
 \n
 dipDegree        userType-\n
  degree          r4- Up = -90, down = 90, level = 0\n
 \n
 dipRatio         userType-\n
  ratio           r4- 1 : value\n
 \n
 matrix           userType- 4x4 matrix\n
  value           r8= 16 top row first, left to right, then second row, etc.\n
 \n
 color            userType- simple rgb\n
  r               n1-\n
  g               n1-\n
  b               n1-\n
 \n
 colorA           userType- simple rgba\n
  r               n1-\n
  g               n1-\n
  b               n1-\n
  a               n1-\n
 \n
 normal           userType- normal vector, mush be unit length.\n
  easting         r4-\n
  northing        r4-\n
  elevation       r4-\n
 \n
 point            userType- easting, northing, and elevation in the specified coordinate system.\n
  easting         r8-\n
  northing        r8-\n
  elevation       r8-\n
 \n
 texturePoint     userType- texture coordinate\n
  u               r4-\n
  v               r4-\n
 \n
 planeOUR         userType- Z axis is cross product of up and right.\n
  origin          point- the origin point of the plane.\n
  up              point- the point 1 unit in the up/north direction of the plane.\n
  right           point- the point 1 unit in the right/east direction of the plane.\n
 \n
 planeEquation    userType- ax + by + cz + d = 0\n
  a               r8-\n
  b               r8-\n
  c               r8-\n
  d               r8-\n
 \n
 enumCodePolyOption userType-\n
  code            n1\n
 \n
 enumListPolyOption enumCodePolyOption= 3\n
  0 no options\n
  1 node coloring\n
  2 line coloring\n
 \n
 polyline         userType-\n
  pointCount      n4-\n
  pointList       point* pointCount points in the point list.  Order is important.  Polygons have dup end point\n
  polyOption      enumCodePolyOption-\n
  colorList       color*  color per point.\n
 \n
 enumCodeSurfaceOption userType-\n
  code            n1\n
 \n
 enumListSurfaceOption enumCodeSurfaceOption= 8\n
    0 no options\n
    1 node coloring\n
    2 face coloring\n
   10 normals\n
   11 normals node coloring\n
   12 normals face coloring\n
  100 textured\n
  110 textured and normals\n
 \n
 surfaceFace      userType-\n
  pointListIndex  n4= 3  A triangle where the values are indicies into a pointList.\n
 \n
 surface          userType-\n
  pointCount      n4-\n
  faceCount       n4-\n
  pointList       point*        pointCount points in the point list.  Order is not important.\n
  faceList        surfaceFace*  faceCount faces in the face list.
  surfaceOption   enumCodeSurfaceOption- \n
  colorList       color*        color per face.\n
  uvList          texturePoint* texture point per point.\n
  texture         imageId-      image id.\n
  normalList      normal*       normal per point.\n
 \n
 enumCodeMeshOption userType-\n
  code            n1\n
 \n
 enumListMeshOption enumCodeMeshOption= 16\n
     0 no options\n
     1 face vis\n
    10 node coloring\n
    11 node coloring and face vis\n
    20 face coloring\n
    21 face coloring and face vis\n
   100 normals\n
   101 normals and face vis\n
   110 normals node coloring\n
   111 normals node coloring and face vis\n
   120 normals face coloring\n
   121 normals face coloring and face vis\n
  1000 textured\n
  1001 textured and face vis\n
  1100 textured and normals\n
  1101 textured and normals and face vis\n
 \n
 mesh             userType-\n
  pointCountX     n4-\n
  pointCountY     n4-\n
  pointList       point*        pointCountX * pointCountY points in the point list.  Order is important.\n
  mechOption      enumCodeMeshOption-\n
  faceVisList     bool*         face visibility list.  0 size if all faces are visible.
  colorList       color*        0 size if not node or face coloring.
  textureUVList   texturePoint* texture uv per point.\n
  textureId       imageId-      image id.\n
  normalList      normal*       normal per point.\n
 \n
 textGeneric      userType-\n
  location        planeOUR-\n
  value           ""-\n
 \n
 formula          userType-
  text            ""-\n
 \n
\n
```

All MI-MIFF files will have the above userTypes defined as they are listed above.  These types ARE part of the MI-MIFF subformat.  However these types are only required to be present if they are used in the file.

## 3.2 - Information Block


Common to all MI-MIFF files for any of the types above, there will be an information block providing general, project wide information.

This information block will be the very first key value in the file right after the MIFF header.

Inside the information block we have the following possible key values.  Not all are required.

```
info []-
 type              ""-    [file type]
 company           ""-    [company name]
 copyright         ""-    [copyright if any]
 author            ""-    [client employee name]
 software          ""-    [software preparing this file]
 softwareVersion   ""-    [software version]
 comment           ""-    [comments]
 projectSystem     ""-    [coordinate system the project is in]
 projectName       ""-    [name of project area]
 projectMin        point- [minimum project area point]
 projectMax        point- [maximum project area point]
\n
```

Most keys are self explanatory.

**[file type]** is one of...

| Value | Description |
| --- | --- |
| Mixed | Combination of any of the types below. |
| Drillhole | Only holds drillhole data. |
| Geometry | Only holds geometry data. |
| Model | Only holds model data. |


**[projectSystem]** will be a string as defined in a package like GDAL which can translate from one coordinate system to another.  Specifying the system here will mean all coordinate values will be in that system.  Like "WGS84" or "NAD27".  Even the [projectMin] and [projectMax] will be in this system.  Use "local" to indicate that the project coordinates are in a local flat earth space.

**[projectMin]** and **[projectMax]** should given an indication on the data range of the project.  This does not need to be exactly defining the outer limits of the all the data, just the rough range that the data should live inside or around.

## 3.3 - Item List Data


Item List will define items that are stored in Drillhole, Geometry, and Model data.  There can be more items defined here than what is actually being stored in the various places but an item needs to be defined here before it is used in those places.

```
itemList []= [item list count]
 ...
```

An item list is just a collection of items.  It should be known how many items are being tracked so this list is often using the array flag with the key-value block.

### 3.3.1 - Item Data


Within the itemList you will only find Item Key-Value blocks.

Item data will describe the value that will be stored in the various places where the item is being tracked.

```
item []-
 id          ""-          [item id]
 nameLong    ""-          [item name]
 nameShort   ""-          [item name short]
 type        type-        [item type]
 min         [item type]- [item min value]
 max         [item type]- [item max value]
 precision   [item type]- [item precision]
 default     [item type]- [default value for this item, missing value otherwise.]
 calculation ""-          [calculation field value]
 ...
\n
```

Each item will have a unique **id**.  This id may or may not be provided by the software.  This id is used by the other places in this MI MIFF where this item's values are stored.  It has to be unique compared to any other items in the item list.

**nameLong** and **nameShort** are two names for this item.  These names may not be unique.  There can be multiple items with the same names but need to have different ids.

**type** will define the value being stored for this item.  Any of "", r&#42;, n&#42;, i&#42;, date, time, dateTime, enumCode, or formula (formula meaning the item's value is calculated.)

**min| b<sub>b |max** define the limits for an item.  Only applicable if item type is a r&#42;, n&#42;, or i&#42;.  If these are missing then the range is the full range as defined by the type.

**precision** is the accuracy of the number.  Only applicable if item type is a r&#42;.

**default** will define the default value for this item if block is skipped.  If this is missing then the default value is 'missing'.

**calculation** is an equation that is performed to obtain the item's value.  Only applicable if item type is formula.  Use "@[item id]@" in the formula to refer to another item value in the current context.  Full discussion about the formula composition will be discussed elsewhere.

When precision is used, it will change how the values are stored.  Say we have a min of 0 and an max of 10 and a precision of 0.1.  As a result we have a total of 101 unique values. 0.0, 0.1, ..., 9.9, 10.0.  102 unique values when we include 'missing value' as well.  If this is all the values that need to be recorded then we don't really need to use a full r4 or r8 to store it.  We will be storing instead an n&#42; value such that the n value, multiplied by the precision and added to the min value will recover the r value which is being stored.  In this case, 102 values only requires an n1 to encode all the values in the range.

```
Calculation example:
(@ore%@ * @cu@) - @i_am_making_something_up@
```

Missing values for the various types

| Type | Missing Value Binary | Missing Value Text |
| --- | --- | --- |
| r4, r8 | FLT_MAX, DBL_MAX | FLT_MAX, DBL_MAX |
| i&#42;, n&#42; | MAX value for the size | "-" without the quotes |
| date, dateTime | year is MAX value for that field. | "-" without the quotes |
| time | hour is MAX value for that field. | "-" without the quotes |
| enumCode | MAX value for the code. | "-" without the quotes |
| "" | 0 string length.  An actual 0 string will have 1 character and that character will 0. |


Examples:

```
item []-\n
 id        ""-   `cu\n
 nameLong  ""-   `Copper\n
 nameShort ""-   `cu\n
 type      type- r4\n
 min       r4-   [base64 0]\n
 max       r4-   [base64 5]\n
 precision r4-   [base64 0.01]\n
 default   r4-   [base64 missing value]\n
\n


enumCodeRockType  userType-\n
 enumCode  n1-\n
\n
enumValueRockType userType-\n
 enumCode  enumCodeRockType-\n
 enumValue ""-\n
\n
enumListRockType enumValueRockType= 3\n
 0\n
 `Granite\n
 1\n
 `Sandstone\n
 2\n
 `Shale\n
\n
item []-\n
 id        ""-              `rt\n
 nameLong  ""-              `Rock Type\n
 nameShort ""-              `rock\n
 type      type-            enumCodeRockType\n
 default   enumCodeRockType -\n
\n
```

An enumeration requires the enumeration usertypes defined before the item is defined.  Usually in the Type List and not here.

## 3.4 - Drillhole Data


Drillhole data holds information about drillholes.

## 3.5 - Geometry List Data


Geometry List will hold geometric data like points, polylines, polygons, surfaces, text, that live in the project space.

```
geometryList []-\n
...
\n
```

### 3.5.1 - Geometry Data


Within the geometryList you will only find geometry Key-Value blocks.

```
geom []-\n
 id       ""-          [geom id]\n
 type     type-        [geom type]\n
 data     [geom type]-
  ...
 geomAttr []-\n
  [item id] [item type]- ...\n
  ...
 \n
 pointAttr []= [pointCount]\n
  pnt []-\n
   [item id] [item type]- ...\n
   ...
  \n
  ...
 segmentAttr []= [pointCount]\n
  seg []-\n
   [item id] [item type]- ...\n
   ...
  \n
  ...
```

Each geom has a unique id.

Each geom will have a type.  One of point, poly, surface, grid, text.

Each geom will have data of the given type.

Each geom may have an optional geomAttr key value block.  Inside are item values for the geometry.  See the Item Data section on what to store here.

If the geometry is a poly, then there may be an optional pointAttr Key-Value block which contains pnt Key-Value blocks, 1 for each point in the polyline.  Inside are item values for that point.

If the geometry is a poly, then there may be an optional segmentAttr Key-Value block which contains seg Key-Value blocks, 1 for each segment in the polyline.  Inside are item values for that segment.

## 3.6 - Model Data


Some of the information found here is based off of the block model formats that are used at Hexagon, where I work, but also from information found freely on the internet.

[LINK](https://www.deswik.com/wp-content/uploads/2019/01/Block-model-knowledge-for-mining-engineers-An-introduction-1.pdf) Block Model Knowledge For Mining Engineers - An Introduction

There are 3 basic model formats that are in use as Hexagon.


* Block Model

* Seam Model

* Surface Model

**Block Model** is a regular collection of, usually, uniform blocks.  Each block will have values for items that are being tracked by the client.

**Seam Model** is a bit fancier Block Model.  This model is subdivided uniformly in the row and column directions of the model but instead of levels, seams are described.  Each block in a seam will have an elevation top and bottom to indicate the seam's limits, as well as any other values that are found within that seam block just like a normal block model.

**Surface Model** is much more limited than the other two as it just holds a stack of surfaces instead of defining any volume.  This model is subdivided uniformly in the row and column directions of the model but instead of seams you will have a surface.  Each 'block', or really the contact point, for a surface will have an elevation value as well as other values that are being tracked by the client at that surface point.

Even though only Block Models are sub-blocked, MI-MIFF does not limit you to only allowing sub-blocks for Block Models.  The logic could be applied to the other two types of models.  As far as I know, no software vendors sub-block for Seam or Surface Models.  In general, if you are sub-blocking for these other two cases, you are just going to define a model at the resolution of the sub-block size instead of this two layered approach in my opinion.

### 3.6.1 - Model Information


After the general information block the next block will be the Model Information block.

```
infoModel []-
 type             ""-   [model type]
 isSubblocked     bool  [true if subblocked]
 subblockType     ""-   [subblock type]
 subblockOption   v3n4- [subblock fixed resolution]
 subblockOption   v2n4- [subblock row col resolution]
 subblockOption   n1-   [subblock octtree level count]
 origin           v3r8- [origin point]
 colVec           v3r4- [column vector, length if vector is the length of the block]
 rowVec           v3r4- [row vector, length of the vector is the length of the block]
 levVec           v3r4- [level vector, length of the vector id the length of the block]
 blockCount       v3n4- [number of column, row, level blocks]
 isLevVariable    bool- [true if variable level heights]
 levList          r4=   [number of levels] [length] [length] ...
\n
```

**[model type]** will be one of...

| Value | Description |
| --- | --- |
| block | For a block model. |
| seam | For a seam model. |
| surface | For a surface model. |


**[subblock type]** will be one of...

| Value | Description |
| --- | --- |
| fixed | 
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
 |
| semi | 
For a regular division of a block in the east and north directions but an infinitely variable z value for block height.

```
+--+--+--+--+
|  +--+  |  |
|  |  +--+  |
|  +--+  |  | Infinitely variable in the level direction.
+--+  |  |  |
+--+--+--+--+
Uniform divisions in the row and column directions.
```
 |
| octree | 
For an oct-tree subvision of a block.

```
+-----+-----+
|     |     |
+--+--+-----+  Recursive bisecting the blocks until a limit.
+--+--+     |
+--+--+-----+
```
 |
| free | 
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


* fixed subblocking requires a v3n4 to define the number of divisions in the east, north and z directions.

* semiFixed subblocking requires a v2n4 to define the number of divisions in the east and north directions.

* octree subblocking requires a n1 to define the max number of oct-tree divisions that can occur for a block.

* free subblocking does not require an option.

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
  |\  | <--------- levVec (Length can be block size or unit, depends see below. Direction is direction of increasing level index.)
  | \ | /
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

## 3.7 - Model Block:


This is where the model data is placed.  The format is slightly different in that it is a key value list but without value headers and potentially optional values depending on the key.

```
model [...]-
```

The model data will be enclosed in a value stream chunk.  The contents are broken down in key, or key and value pairs.

| Key (1 Byte) | Value |
| --- | --- |
| B | n2 value follows immediately.  Jump to current level index + N.  Reset current row, current column, and current item to 0 |
| b | No value.  Jump to next level.  Reset current row, current column, and current item to 0 |
| R | n2 value follows immediately.  Jump to current row index + N.  Reset currrent column and current item to 0 |
| r | No value.  Jump to next row.  Reset current column and current item to 0. |
| C | n2 value follows immediately.  Jump to current column index + N.  Reset current item to 0 |
| c | No value.  Jump to next column.  Reset current item to 0 |
| s | Defining subblock but parent block as values.  Provide parent block item values before subblock values.  'c' follows immediately if parent block has no values. |
| I | n1 value follows immediately.  Jump to item index + N.  Skipped over items get default values. |
| i | Jump to next item.  Skipped over item gets default value. |
| V | n1 value follows.  Provide the values for the next n1 items. |
| v | Provide the values for the rest of the block or subblock. |
| D | n1 value follows immediately. Duplicate the next n1 values from the last block or subblock. |
| d | Duplicate the remaining values from the last block or subblock. |
| X | n1 value follows immediately.  Clone last block n1 times.  This is for all the item values. |
| . | No value.  Indicates end of the item date or subblock data. |
| \n (text), 0 (binary) | No value.  Indicates end value stream. |


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
model [...]-
B2R2C2v1 2 3cc.\n
```

Note, if you can removed a separator without losing then that should be done.

In binary the above will look like...

```
[n1:5][5 :model][n2:2]
[1 :B][n2:2]
[1 :R][n2:2]
[1 :C][n2:2]
[1 :v][i4:1][i4:2][i4:3]
[1 :c]
[1 :c]
[n1:0]
```

### 3.7.1 - SubBlock: Fixed


In the fixed case of subblocking, as soon as an 's' key is hit, that block is divided into the subblocks as defined in the infoModel section. Use B, b, R, r, C, c, and . in the same way as for the parent blocks.

### 3.7.2 - SubBlock: Semi


In the semi fixed case of subblocking, as soon as an 's' key is hit, that block is divided into the subblocks as defined in the infoModel section.  One difference...

In this scheme of subblocking, there is an infinite variability in the z direction for each subblock.  This is defined by an 'H' key for the first level of blocks of the subblocks.  The number of level subblocks will depend on how many times H is used for the same column.

| Key | Value |
| --- | --- |
| H | r4 value immediately follows.  The r4 value is a percent of the block height.  This record needs to preceed a v record. |
| h | No value.  The rest of the subblock cylinder is used. |


If H or h follows an H or h then the previous block will be holding default values, and the current block will be incremented to the next subblock.  The following H or h will determin the height of the new current block.

If a subblock cell has already defined the entire height of the block then these will be skipped over when jumping to the next subblock.  This means that some subblock cells could have a different number of levels that others.

### 3.7.3 - SubBlock: Octree


In the octree case, an 's' key will split the block or subblock in to 8 uniform pieces.  If we are already at a subblock then that subblock will be split into small 8 uniform pieces.  This should not exceed the subblock option count defined in the model information block.

The 's' key need to preceed the v record.

### 3.7.4 - SubBlock: Free


In the free block case, an 's' key will indicate the parent block is subblocked.  However after that you will need to define the blocks manually.

| Key | Value |
| --- | --- |
| c | 6 x r4 values follow.  First 3 r4 values are percents for the min point and the second 3 r4 values are percents for the max point of the subblock. |
| . | Indicates we are done with the subblocking and to process to the next parent block. |

