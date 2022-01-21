
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
    3.1 - Information Block<br />
    3.2 - Type List Block<br />
        3.2.1 - Type Block<br />
    3.3 - Item List Block<br />
        3.3.1 - Item Block<br />
    3.4 - Image List Block<br />
        3.4.1 - Image Block<br />
    3.5 - Drillhole Block<br />
    3.6 - Geometry List Block<br />
        3.6.1 - Geometry Block<br />
    3.7 - Model Data<br />
        3.7.1 - Model Information<br />
    3.8 - Model Block:<br />
        3.8.1 - SubBlock: Fixed<br />
        3.8.2 - SubBlock: Semi<br />
        3.8.3 - SubBlock: Octree<br />
        3.8.4 - SubBlock: Free<br />

# 1 - M.I. M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I. M.I.F.F. (MI-MIFF)?  MI-MIFF is intended to be a file format for exporting and importing data from one mining software package to another software package.

I would imagine users of software packages are annoyed at the lack of interoperability between software packages.  Currently some software packages attempt to do their best at supporting competitor file formats (reading only) but there is often a lot of loss of data or, as data formats in each package evolves, suffer from compatibility issues.  Often data needs to be dumped to formats that come 'close' but are not exact, like DXF/DWG, or down to very basic ASCII files, like csv files, which are often lacking compared to the original data.  Users will need to massage this raw data in order to import it into their target software package.

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

So, I thought I would try my hand at defining a format what I think would be good as an information format.  I am trying to write MI-MIFF independent of any software package.  However there may be some unintended bias as I am most familiar with MinePlan 3D for which I have worked on  since 1995.  I do intend to keep this format as inclusive as possible to address other software  package requirements as best as I understand them.  I know I will make mistakes there because I have only superficial knowledge of most of the competing software in the industry.

If anyone is interested in educating me on some features they would like to see with respect to their software package of choice I would love to hear from you.  You can reach me at the following email address.

zekaric@gmail.com

# 2 - MI-MIFF Format


See the MIFF_Mixed_Information_File_Format.html file for the discussion of the base file format which is being used for the information file format.  This document will cover specifics related to using MIFF for mining as an information file format to move data.

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
| &#42;.miffMI | for a very general Mine Information of anything mine related. |
| &#42;.miffMIDrill | for only Drillhole related data. |
| &#42;.miffMIGeom | for only Geometry related data. |
| &#42;.miffMIModel | for only Model related data. |


This information is duplicated inside the file so reading in the file will confirm with the extension but it will help the user if the extension is properly set and will make it easier on the software on how to read in the file if these are properly set.

## 2.1 - Header


MIFF has fields for a Sub-Format Name and Version.  For an MI-MIFF file this will be set to...

| Field | Value |
| --- | --- |
| Sub-Fromat Name | Mining Information |
| Sub-Format Version | 1 |


# 3 - Mine Information Block (MIB)


An MI-MIFF file can have 1 or more MIBs.  One MIB represents a unique mining project or project area.

```
{	mine
...
}
```

Inside a MIB you will have the following blocks.


* **Information block** to hold general information about the project.  This must be the first block.

* **Type List block** to hold the user types that are used in the format.

* **Item List block** to hold the item definitions.  Items being the values that are stored in the drillhole, model, cuts, and whatever else that is being tracked.  There will only be one Item List block.

* **Image List block** to hold the images.  Images can be used for texturing, symbols, etc.

* **Geometry Block** to hold geometry information.  There can be more than one Geoemtry block.

* **Drillhole block** to hold drillhole information.  Item List block must already be defined or else you are not providing assay or survey values with your drillhole data.  There can be more than one Drillhole block.

* **Model Block** to hold model information.  Item List block must already be defined or else you are not providing any values with your model data.  There can be more than one Model block.

## 3.1 - Information Block


Common to all MI-MIFF files for any of the types above, there will be an information block providing general, project wide information.

This information block will be the very first key value in the file right after the MIFF header.

Inside the information block we have the following possible key values.  Not all are required.

```
{	info
"	type	1	[file type]
"	company	1	[company name]
"	copyright	1	[copyright if any]
"	author	1	[client employee name]
"	software	1	[software preparing this file]
"	softwareVersion	1	[software version]
"	comment	1	[comments]
"	projectSystem	1	[coordinate system the project is in]
"	projectName	1	[name of project area]
abcr8	projectMin	1	[minimum project area point]
abcr8	projectMax	1	[maximum project area point]
}
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

## 3.2 - Type List Block


This block is optional.  Types could be defined anywhere in the file as long as the type is defined before it is used.  It is better to have them all located in one place and near the beginning of the file.

```
{	type list
...
}
```

The type list will have 1 or more **type** blocks inside of it.

### 3.2.1 - Type Block


```
{	type
"	name	1	[name of user type]
v	[name of variable]	[array count of variable]	[type of variable]
v	[name of enumeration]	[array count of variable]	[type of variable]	([int code of enum]	[string value of enum])&#42;
}
```

**name** appears first in the block.  It will be the name of the type.

There will be 1 or more of the following two lines.  Each line indicates a member of the type.  The second line indicates an enumeration where after the [type of variable] will be a series of code and string pairs.

When data is provided for the type, the order of the members will be the order of the values.

```
{	type list
{	type
" 	name 	1	id
v	value	1	n4
}
{	type
"	name	1	date
v	year	1	i4
v	month	1	n1
v	day	1	n1
}
{	type
"	name	1	time
v	hour	1	n1
v	minute	1	n1
v	second	1	n1
}
{	type
"	name	1	dateTime
v	year	1	i4
v	month	1	n1
v	day	1	n1
v	hour	1	n1
v	minute	1	n1
v	second	1	n1
}
{	type
"	name	1	imageId
v	value	1	n4
}
{	type
"	name	1	enumCode
v	value	1	n1	0	value1	1	value2	999	value3
}
{	type
"	name	1	azimuth
v	value	1	"
}
{	type
"	name	1	degree
v	value	1	r4
}
{	type
"	name	1	dipDegree
	1	value	r4
}
{	type
"	name	1	dipRatio
	1	value	r4
}
{	type
"	name	1	matrix
	1	value	matrix4x4r8
}
{	type
"	name	1	color
	1	value	abcn1
}
{	type
"	name	1	colorA
	1	value	abcdn1
}
{	type
"	name	1	normal
	1	value	abcr8
}
{	type
"	name	1	point
	1	value	abcr8
}
{	type
"	name	1	textureUV
	1	value	abr4
}
{	type
"	name	1	planeOUR
	1	origin	abcr8	up	abcr8	right	abcr8
}
{	type
"	name	1	planeEquation
	1	value	abcdr8
}
{	type
"	name	1	enumPointOption
	1	value	n1	0	no options	1	coloring
}
{	type
"	name	1	pointGroup
v	pointCount	1	n4
v	pointList	&#42;	abcr8
v	pointOption	1	enumPointOption
v	colorList	&#42;	abcn1
}
{	type
"	name	1	enumPolyOption
v	value	1	n1	0	no options	1	node coloring	2	line coloring	10	texture coloring
}
{	type
"	name	1	polyline
v	pointCount	1	n4
v	pointList	&#42;	abcr8
v	polyOption	1	enumPolyOption
v	colorList	&#42;	abcn1
v	textureUV	&#42;	abr4
v	texture	1	imageId
}
{	type
"	name	1	enumSurfaceOption
v	value	1	n1	0	no options	1	node coloring	2	face coloring	10	normals	11	normals with node coloring	12	normals with face coloring	100	textured	110	textured with normals
}
{	type
"	name	1	surface
v	pointCount	1	n4
v	pointList	&#42;	abcr8
v	faceCount	1	n4
v	faceList	&#42;	abcn4
v	surfaceOption	1	enumSurfaceOption
v	colorList	&#42;	abcdn1
v	uvList	&#42;	abr4
v	texture	1	imageId
v	normalList	&#42;	abcr4
}
{	type
"	name	1	enumCodeMeshOption
v	value	1	n1	0	no options	1	face vis	10	node coloring	11	node coloring and face vis	20	face coloring	21	face coloring and face vis	100	textured	101	textured and face vis	100	textured and normals	101	textured and normals and face vis
}
{	type
"	name	1	mesh
v	pointCountX	1	n4
v	pointCountY	1	n4
v	pointList	&#42;	abcr4
v	meshOption	1	enumMeshOption
v	faceVisList	&#42;	b
v	colorList	&#42;	abcdn1
v	textureUVList	&#42;	abr4
v	textureId	1	imageId
}
{	type
"	name	1	textGeneric
v	location	1	planeOUR
v	value	1	"
}
{	type
"	name	1	formula
v	text	1	"
}
```

All MI-MIFF files will have the above types defined as they are listed above.  These types ARE part of the MI-MIFF subformat.  However these types are only required to be present if they are used in the file.

For polyline, surface, and mesh, if the options don't require the presence of a field then that field will be default.  Meaning, if it is an array, the array size is 0; if an id, the id is 0; etc.

## 3.3 - Item List Block


Item List will define items that are stored in Drillhole, Geometry, and Model data.  There can be more items defined here than what is actually being stored in the various places but an item needs to be defined here before it is used in those places.

```
{	item list
...
}
```

### 3.3.1 - Item Block


Item will describe the value that will be stored in the various places where the item is being tracked.

```
{	item
id	id	1	[id of item]
"	nameLong	1	[item name]
"	nameShort	1	[item name short]
type	type	1	[item type]
[item type]	min	1	[item min value]
[item type]	max	1	[item max value]
[item type]	precision	1	[item precision]
[item type]	default	1	[default value for this item, missing value otherwise.]
"	calculation	1	[calculation field value]
}
```

Each item will have a unique **id**.  This id may or may not be provided by the software.  This id is used by the other places in this MI-MIFF where this item's values are stored.  It has to be unique compared to any other items in the item list.

**nameLong** and **nameShort** are two names for this item.  These names may not be unique.  There can be multiple items with the same names but need to have different ids.

**type** will define the value being stored for this item.  Type is one of "", r&#42;, n&#42;, i&#42;, date, time, dateTime, enumCode, or formula (formula meaning the item's value is calculated.)

**min| b<sub>b |max** define the limits for an item.  Only applicable if item type is a r&#42;, n&#42;, or i&#42;.  If these are missing then the range is the full range as defined by the type.

**precision** is the accuracy of the number.  Only applicable if item type is a r&#42;.

**default** will define the default value for this item if block is skipped.  If this is missing then the default value is 'missing'.

**calculation** is an equation that is performed to obtain the item's value.  Only applicable if item type is formula.  Use "@[item id]@" in the formula to refer to another item value in the current context.  Full discussion about the formula composition will be discussed elsewhere.

When precision is used, it will change how the values are stored.  Say we have a min of 0 and an max of 10 and a precision of 0.1.  As a result we have a total of 101 unique values. 0.0, 0.1, ..., 9.9, 10.0.  102 unique values when we include 'missing value' as well.  If this is all the values that need to be recorded then we don't really need to use a full r4 or r8 to store it.  We will be storing instead an n&#42; value such that the n value, multiplied by the precision and added to the min value will recover the r value which is being stored.  In this case, 102 values only requires an n1 to encode all the values in the range.

If there is an enumeration item value then define the enumeration as a type in the type list and use that type for the item's type.

```
Calculation example:
(@ore%@ &#42; @cu@) - @i_am_making_something_up@
```

Missing values for the various types

| Type | Missing Value Binary | Missing Value Text |
| --- | --- | --- |
| r4, r8 | FLT_MAX, DBL_MAX | FLT_MAX, DBL_MAX |
| i&#42;, n&#42; | MAX value for the size | "-" without the quotes |
| date, dateTime | year is MAX value for that field. | "-" without the quotes |
| time | hour is MAX value for that field. | "-" without the quotes |
| enumCode | MAX value for the code. | "-" without the quotes |
| "" | 0 string length.  An actual 0 string will have 1 character and that character will 0. | 0 string length.  An actual 0 string will have 1 character and that character will 0. |


Examples:

```
{	type list
...
{	type
"	name	1	rock type	15	Granite	17	Sandstone	19	Shale
}
...
}
{	item list
...
{	item
id	id	1	0
"	nameLong	1	Copper
"	nameShort	1	cu
type	type	1	r4
r4	min	1	[base64 0]
r4	max	1	[base64 5]
r4	precision	1	[base64 0.01]
r4	default	1	[base64 missing value]
}
...
{	item
id	id	1	1
"	nameLong	1	Rock Type
"	nameShort	1	rock
type	type	1	rock type
rock type	default	1	0
}
...
}
```

An enumeration requires the types defined before the item is defined.  Place the enumeration in the type list.  Note, the enumeration has values 15-Granite, 17-Sandstone, and 19-Shale.  Yet the default for Rock Type item is 0.  This is because it is the index of the value and not the code value.  0 being Granite, 1 being Sandstone, and 2 being Shale in this case.

## 3.4 - Image List Block


Defining the images used by surfaces and such.

```
{	image list
...
}
```

### 3.4.1 - Image Block


Defining an image.

```
{	image
id	id	1	[image id]
n4	width	1	[width of image]
n4	height	1	[height of image]
[color|colorA]	pixels	[pixel count]
```
