
# M.I.F.F.: Mixed Information File Format


**`Author:            `** Robbert de Groot

**`Date:              `** 2019-05-15

**`Copyright:         `** 2019, Robbert de Groot

**`License (Library): `** MIT License.

**`License (Document):`** Creative Commons Attribution-NoDerivs.[LINK](https://creativecommons.org/licenses/by-nd:4.0) (CC BY-ND)

## Table Of Contents:

**1 - M.I.F.F.**<br />
    1.1 - Discussion<br />
    1.2 - Goals<br />
    1.3 - Design Decisions:<br />
    1.4 - Disclosure<br />
**2 - Format: Base**<br />
    2.1 - File Header<br />
    2.2 - Content<br />
        2.2.1 - Numbers<br />
        2.2.2 - Record Composition<br />
            2.2.2.1 - Key<br />
            2.2.2.2 - Type Code<br />
            2.2.2.3 - Array Count<br />
            2.2.2.4 - Value<br />
    2.3 - Format<br />
        2.3.1 - Whitespace Characters<br />
        2.3.2 - Separator Characters<br />
        2.3.3 - Printable Characters<br />
        2.3.4 - Base64<br />
        2.3.5 - Real Number Representation<br />
        2.3.6 - Record Composition<br />
**3 - Format: Value Specifics**<br />
    3.1 - Key-Value Block Begin and End.<br />
    3.2 - Basic Values<br />
    3.3 - String values<br />
    3.4 - Variable values<br />

# 1 - M.I.F.F.


## 1.1 - Discussion


What is the purpose of M.I.F.F. (MIFF)?  MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals



* **Simple**    The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **Brief**     The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **Accurate**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3 - Design Decisions:


**Why not XML or JSON?**

I find XML is too verbose.  JSON is much better than XML and is a reasonable alternative to XML.  Both are very flexible but that can be a double edged sword.  I feel there should be something better.  I do not claim MIFF is better, it is just different.

**Why Big Endian for multibyte data types?**

In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows (NT/2000/XP/etc.)  At that time the architecture on some of the other platforms was Big Endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a machine with a different Endian.  The problems that we faced were trivial to solve but were very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler even if it does mean a potential performance hit for a platform.  This format is not meant to be good in performance; its main goal is to be good in getting data moved from one place to another.

## 1.4 - Disclosure


I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing or developing options which I find lacking or distasteful in some fashion.

I can be reached at the following email address.

zekaric@gmail.com

# 2 - Format: Base


A MIFF File is a UTF8 text file where items inside are tab delimited and each record is terminated by a UNIX \n new line.  The contents of the file a collection of key value pairs.  Values are typed.  Nesting is allowed.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This file is meant to be a transfer file format to move data from one program to another.

Any byte data that is encode or stored as a binary byte sequence will be in big endian order.  In a text file the data is stored in Base64 encoding.

## 2.1 - File Header


There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.  The header is 4 lines long.

&gt; MIFF![image](nl.png)

&gt; 1![image](nl.png)

&gt; [Sub-Format Name string]![image](nl.png)

&gt; [Sub-Format Version string]![image](nl.png)

![image](tab.png) means a tab character.  ![image](nl.png) means a new line character.  The &gt; is not part of the line, is it denoting the start of the line.


* **Line 1** - Format of the file.  MIFF.

* **Line 2** - Version of the file.  Version 1.

* **Line 4** - Sub-Format name.  A MIFF file is potentially a container for an sub format of the file.

* **Line 5** - Sub-Format version identifier.

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain ![image](tab.png)s or ![image](nl.png)s.  If there are leading and trailing spaces, then that is part of the format name as silly as that may sound.  Spaces internal to the format name are also allowed and are significant.  I say 255 bytes instead of characters because in UTF8 one character/codepoint can span multiple bytes.  So this name can not exceed this byte count.

## 2.2 - Content


Before going into the specifics of the format we will cover some things at a high level.

### 2.2.1 - Numbers


**Natural numbers** (unsigned integers for programmers) are whole numbers ranging from 0 to some positive max value.

**Integer numbers** are whole numbers ranging from some negative max value to some positive max value.

**Real numbers**    (floating point numbers for programmers) are positive and negative numbers that may not be whole.  I.E. PI, 3.14159, is a Real number.

### 2.2.2 - Record Composition


A record basically defines a key - value pair.  However the composition of that record is composed of a few parts.

**[key]**              will be a sequence of characters that will somewhat uniquely identify the value.

**[type code]**        will be a sequence of characters that will dictate what the value will look like.

**[array count]**      will be a number or special character to indicate the number of values of a particular type is associated with the key.

**[value]**            will depend on the the above; it will vary in appearance.

#### 2.2.2.1 - Key


Keys are always a single string of any character in UTF8 encoding as long as none are ![image](tab.png) and ![image](nl.png).  Whitespace, leading, trailing, and internal to the key string are significant and cannot be trimmed or thinned out.  Keys are limited to being 255 bytes long.  In UTF8 that may not mean 255 letters/characters as some characters may end up requiring more than 1 byte.  I would suggest to limit the whitespace to just spaces and nothing else.  Everything else should be a printable character.  We do not need another "Whitespace Language" monster.

12345
;':][.,&lt;&gt;]&#96;
a_b_c
$cost
été
This is also a valid key with internal spaces

#### 2.2.2.2 - Type Code


| Type Code | Description |
| --- | --- |
| <br />**Block Types**<br /> |
| { | Key-Value block begins. |
| } | Key-Value block ends. |
| <br />**Basic Value Types**<br /> |
| type | Type value.  One of these type codes. |
| " | String (UTF8) data, can be of any length. |
| b | Boolean value. |
| i1 i2 i3 i4 i8 i16 i32 i64 i128 i256 | An integer number.  Min and Max value will depend on how much the bytes can encode.  Yes I know there are no native types for some of these, like i3, but I include these byte counts because they may be useful in certain cases. |
| n1 n2 n3 n4 n8 n16 n32 n64 n128 n256 | A natural number.  Ranges from 0 to max value.  Max value will depend on how much the bytes can encode.  Yes I know there are no native types for a lot of these, like n3, but I include these byte counts because they may be useful in certain cases. |
| r4 r8 | A real value. |
| abi1 abi2 abi4 abi8 | A 2D point using integers. |
| abn1 abn2 abn4 abn8 | A 2D point using naturals. |
| abr4 abr8 | A 2D point using reals. |
| abci1 abci2 abci4 abci8 | A 3D point using integers. |
| abcn1 abcn2 abcn4 abcn8 | A 3D point using naturals. |
| abcr4 abcr8 | A 3D point using reals. |
| abcdi1 abcdi2 abcdi4 abcdi8 | A 4D point using integers. |
| abcdn1 abcdn2 abcdn4 abcdn8 | A 4D point using naturals. |
| abcdr4 abcdr8 | A 4D point using reals. |
| mat2x2r4 mat2x2r8 | A 2x2 r4 or r8 matrix |
| mat3x3r4 mat3x3r8 | A 3x3 r4 or r8 matrix |
| mat4x4r4 mat4x4r8 | A 4x4 r4 or r8 matrix |
| <br />** Variable Value Type**<br /> |
| v | Variable data follows.  Could be and combination of any Value type.  What follows should be defined somewhere in the format somewhere before this is used. |


#### 2.2.2.3 - Array Count


| Encoding Code | Description |
| --- | --- |
| 1 | A single value follows.  In binary, no array count present. |
| [number of items] | An array of values follows.  In binary, 001 means an n1 count follows; 010 means an n2 count follows; 011 means an n4 count follows; 100 means an n8 count follows; 101 means an n16 count follows; 110 means an n32 count follows. |
| &#42; | An unknown count array of values follows. |


#### 2.2.2.4 - Value


See the types respective sections for details.

## 2.3 - Format


The format is a UTF8 text file.

The format is only somewhat human readable, and only somewhat text editor friendly.  It was not designed to be "human friendly" but rather "debug friendly."  What I mean is that you will be able to look at the format in a text editor, if that text editor is capable of handling really large files with really long lines; and if you know this format, you will be able to debug it.

The format does not adhere to any line length limit.  So if you are using a text editor that does have line length limits you may end up corrupting the file or you may not be seeing the whole file.

All lines are terminated with a single ![image](nl.png) (\n).  NOT an MSDOS/Windows Cursor Return-New Line pair (\r\n) or a MAC \n\r or just a \r.  This is a strict rule.  If the file has \r\n, \n\r, or \r line endings then it is not a MIFF file.

### 2.3.1 - Whitespace Characters


A whitespace character is any character that does not make a blemish on a piece of paper if printed out; this includes control characters like bell, NULL, form feed, etc.

### 2.3.2 - Separator Characters


MIFF only uses ![image](tab.png) as a separater character.  And only one ![image](tab.png) between fields.  Whitespace is not considered a separater character and will be part of a key or value if it is present.

### 2.3.3 - Printable Characters


Just to be clear what a printable character is.  If you had a blank piece of paper with just one character on it, if the character causes ink to be placed on the paper, it is printable.  If it does not make a mark, it is not printable (like spaces, control characters, etc.)

### 2.3.4 - Base64


A few items are stored in Base64.  This is to add a little compression to the items in question but mainly it is to ensure binary exactness of the data when writing the values out and reading the values back in.  Unlike normal Base64 which will break the buffer into lines of a certain length, Base64 values in a MIFF are one long continuous stream of characters.  No \n or line breaks of any kind will be present.

### 2.3.5 - Real Number Representation


All real numbers are stored as a Base64 string.  No exceptions.  This is to ensure that when read back into memory from file, the number is exactly as it was when it was stored in the file.  The real value in the MIFF file will be in Big Endian order.

### 2.3.6 - Record Composition


&gt; [type code]![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)[value]![image](nl.png)

Any space found in the key that is not a ![image](tab.png) will be part of the key.  They will be significant.  Do not strip or reduce them.

There has to be one ![image](tab.png) separator between each part.

How the value appears will depend on the definition here.

To be clear...

**Invalid:** Absolutely no ![image](nl.png) anywhere before or within the key value line&#42;.  Absolutely no blank lines.  Absolutely no extra ![image](tab.png) anywhere in the format.

&gt; [type code]![image](tab.png)![image](tab.png)[key]![image](nl.png)[array count] [value]![image](nl.png)

What [value] will look like will depend on what is being stored for the record.  Values will be discussed lower in section 3.

# 3 - Format: Value Specifics


## 3.1 - Key-Value Block Begin and End.


Key-Value blocks are special.  They are needed to allow nesting of key values.

&gt; {![image](tab.png)[key]![image](nl.png)
&gt; ...
&gt; }![image](nl.png)

A block is terminated when a value type of "}" is reached.  There is no key for this line value type.  Every begin block value type record requires an end block value type record, there must not be extras of each in the file.  Begin Block record must match with an End Block record.  Do not take a shortcut and omit them even if they are the last few records of the file.

Array Count is never used with this value type and there is not value that follows.

**Example 1:**

&gt; {![image](tab.png)docInfo![image](nl.png)
&gt; ...
&gt; }![image](nl.png)

**Example 2:**

&gt; {![image](tab.png)level1![image](nl.png)
&gt; {![image](tab.png)level2![image](nl.png)
&gt; {![image](tab.png)level3![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
&gt; {![image](tab.png)anotherLevel3![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
&gt; }![image](nl.png)
&gt; {![image](tab.png)anotherLevel2![image](nl.png)
&gt; ...
&gt; }![image](nl.png)
&gt; }![image](nl.png)
&gt; {![image](tab.png)anotherLevel1![image](nl.png)
&gt; ...
&gt; }![image](nl.png)<br />

## 3.2 - Basic Values


Basic value encoding.  Based on what is being stored the byte streams only look slighly different.

&gt; [text type code]![image](tab.png)[key]![image](tab.png)1![image](tab.png)[type value]![image](nl.png)
&gt; [text type code]![image](tab.png)[key]![image](tab.png)[array count](![image](tab.png)[type value])&#42;![image](nl.png)

Text representation for a value will be...

| Type Code | Representation |
| --- | --- |
| type | text string for the type. |
| b | 'T' or 'F'. |
| i&#42;, n&#42; | 1 value in regular digits. |
| r&#42; | 1 Base64 stream of the 1 value.  Big endian order. |


If using an array flag, the above is repeated as many times as there are array elements.  Each array element will be separated by one ![image](tab.png).

**Example 1:**

&gt; type![image](tab.png)type1![image](tab.png)1![image](tab.png)b![image](nl.png)

**Example 2:**

&gt; type![image](tab.png)type2![image](tab.png)2![image](tab.png)b![image](tab.png)n4![image](nl.png)

**Example 4:**

&gt; b![image](tab.png)Bool1![image](tab.png)1![image](tab.png)T![image](nl.png)

**Example 5:**

&gt; b![image](tab.png)Bool2![image](tab.png)10![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)T![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](tab.png)F![image](nl.png)

**Example 6:**

&gt; i4![image](tab.png)1Int![image](tab.png)1![image](tab.png)1024![image](nl.png)

**Example 7:**

&gt; n4![image](tab.png)&#42;Nat![image](tab.png)8![image](tab.png)1![image](tab.png)2![image](tab.png)4![image](tab.png)8![image](tab.png)16![image](tab.png)32![image](tab.png)64![image](tab.png)128![image](nl.png)

**Example 8:**

&gt; r4![image](tab.png)1Real![image](tab.png)1![image](tab.png)[Base64 encoded value]![image](nl.png)

**Example 9:**

&gt; r8![image](tab.png)&#42;Real![image](tab.png)3![image](tab.png)[Base64 encoded value]![image](tab.png)[Base64 encoded value]![image](tab.png)[Base64 encoded value]![image](nl.png)

## 3.3 - String values


&gt; "![image](tab.png)[key]![image](tab.png)1![image](tab.png)[string value]![image](nl.png)
&gt; "![image](tab.png)[key]![image](tab.png)[array count](![image](tab.png)[string value])&#42;![image](nl.png)

The string is preprocessed before storing.  All ![image](tab.png), ![image](nl.png), and \ characters are escaped with a \.  This way when you see an actual tab in the record, these are used to separate strings in an array of strings.  And the actual new line character is the terminater of the record.

\t - tab            - 0x09
\n - new line       - 0x0a
\\ - \

**Example 1:**

&gt; "![image](tab.png)string1![image](tab.png)1![image](tab.png)This is line1.\nThis is line 2.![image](nl.png)

**Example 2:**

&gt; "![image](tab.png)stringList1![image](tab.png)3![image](tab.png)This is string 1, line 1.\nThis is string1, line 2.\n![image](tab.png)This is string 2.![image](tab.png)This is string 3.![image](nl.png)

## 3.4 - Variable values


Variable records should be used sparingly or when using the basic types would make the format too unweildy.  The contents of the variable record should be defined somewhere in the format documentation or through other records in the file so that the file reader will be able to parse the contents properly and safely.

&gt; v![image](tab.png)[key]![image](tab.png)1![image](tab.png)[tab delimited values]![image](nl.png)
&gt; v![image](tab.png)[key]![image](tab.png)[array count]![image](tab.png)[tab delimited values]![image](nl.png)

A single variable type is already an array of mixed values.  And array of variables is not really any different than the single value because the API has no idea what one value would mean.  But this is potentially useful information for a reader.  In other words the reader and writer of a variable type is in control of what goes into the variable type.
