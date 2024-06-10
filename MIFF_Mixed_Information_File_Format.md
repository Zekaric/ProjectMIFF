
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
**2 - Format**<br />
    2.1 - File Header<br />
    2.2 - Content<br />
        2.2.1 - Header Section<br />
        2.2.2 - Key Section<br />
        2.2.3 - Value Section<br />
    Examples<br />

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

**Why big endian for multibyte data types?**

In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows (NT/2000/XP/etc.)  At that time the architecture on some of the other platforms was big endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a machine with a different endian.  The problems that we faced were trivial to solve but were very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler even if it does mean a potential performance hit for a platform.  This format is not meant to be good in performance; its main goal is to be good in getting data moved from one place to another, accurately.

## 1.4 - Disclosure


I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing or developing options which I find lacking or distasteful in some fashion.

I can be reached at the following email address.

zekaric@gmail.com

# 2 - Format


A MIFF File is a UTF8 text file where items inside are tab delimited and each record is a single line terminated by a UNIX \n new line.  The contents of the file a collection of key value pairs.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This file is meant to be a transfer file format to move data from one program to another.

Any data that is encode or stored as a binary byte sequence will be in big endian order.

## 2.1 - File Header


There will always be a file header so that you can be sure the file you recieved is actually a MIFF file and not some other file.  The header is 4 lines long.

```
MIFF[nl]
1[nl]
[Sub-Format Name string][nl]
[Sub-Format Version string][nl]
```

[nl] means a new line character.


* **Line 1** - Format of the file.  MIFF.

* **Line 2** - Version of the file.  Version 1.

* **Line 4** - Sub-Format name.  A MIFF file is potentially a container for an sub format of the file.

* **Line 5** - Sub-Format version identifier.

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain [tb]s or [nl]s.  If there are leading and trailing spaces, then that is part of the format name as silly as that may sound.  Spaces internal to the format name are also allowed and are significant.  I say 255 bytes instead of characters because in UTF8 one character/codepoint can span multiple bytes.  So this name can not exceed this byte count.

## 2.2 - Content


Each line is split into 3 sections.  The header for the line, the key for the line, and the value section.

Each section is separated by a [tb] character.  The entire line ends in a [nl] character.

```
[header][tb][key][tb][value][nl]
```

### 2.2.1 - Header Section


The header for the line is one of the following.

| Code | Description |
| --- | --- |
| - | Comment line.  This has no meaning whatsoever in the file.  It should never contain any data or meta-data.  These lines will be ignored completely when reading the file. |
| { | Open new scope.  This is so you can group data into sections or objects. |
| } | Close scope.  Denotes the end of a scope. |
| [count] | This will be a number larger than 0 to indicate an array count for the key.  Whatever data the key is suppose to hold, there should be that many of them.  If you have an array of complex objects, use scoped above and a separate count. |
| &#42; | This will be to indicate an array of unknown size.  In generaly I would advise trying to avoid this as muck as possible.  It is better to know beforehand howmuch space data should take up.  However sometimes this is unavoidable. |


The header can have leading spaces or [tb] characters to make it more obvious which scope it belongs to.  However these spaces and [tb] characters are not needed unless you want to make it more readable.  They are stripped when reading the file.

### 2.2.2 - Key Section


Keys are any string that is up to 255 bytes long.  I say bytes because in UTF8 format, some characters can take up multiple bytes.  So as long as the key is not larger than 255 bytes the key is valid.  The key can have spaces or any special characters you like except [tb] and [nl] characters.  This does mean leading, trailing, and internal spaces in the key are significant.

### 2.2.3 - Value Section


The sub-format of the file should be dictating what values follow the key.  This should be defined somewhere in the documentation of the sub-format.

There can be 0 or more values associated with the key.  Again depending on what the sub-format documenation dictates.  Each value will be separated by a [tb] character.

If there is a count in the line header other than 1 then there should be multiple copies of whatever type that is associated with the key.

Values have a simple format.

```
[header][valueData]
```

The header is a single character.

The valueData will depend on the header.

| Header | Description |
| --- | --- |
| &#126; | The value data is unset/null.  Nothing else follows the header. |
| T, F | The value data is a boolean and the header is already the value data.  Nothing else follows the header.  **T** for true, **F** for false. |
| +, -, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 | The value data will be a number of some sort.  An integer, real, or complex number, in human readable form.  The header is part of the number. |
| x, o, b | The value data will be a number of some sort.  But encoded in **x** hexidecimal (0123456789abcdef), **o** octal (01234567), **b** binary (01) digits. |
| I, R | The value data will be an **I** integer or a **R** real number encoded in Base64.  Why have this instead of just output the number in human readable form.  I believe when dealing with reals that there is a loss of precision possible when writing them out in human readable form.  Writing out reals this way will ensure the bits are exactly the same as they were when you were storing the values.  Integers are on average unnecessary to write this way as they are exactly the same in human readable form.  The human readable form may also be more compact depending on what sort of values you end up writing out.  However this option may be faster to convert to and from text if that is a concern. |
| n, N | The value data will be a big integer or big real number.  **n** will be in human readable format.  **N** will be in Base64 encoding. |
| " | The value data will be a string.  There is no closing **"**.  Whatever follows the header **"** to the next [tb] or [nl] which ever is next, is the string.  The string is preprocessed before storing.  All [tb], [nl], and \ characters are escaped with a \. |
| &#42; | The value data is a binary chunk.  It will be two parts separated by a space.  First part immediately follows the &#42; an it denotes the size of the binary chunk.  After the space will be the binary chunk in Base64 encoding. |


## Examples


```
- A null value.
1[tb]Null[tb]&#126;[nl]
- A true and false array.
2[tb]True And False[tb]T[tb]F[nl]
- Some integers and reals.  Last two are 4 Byte 3.1415926535897932f, and 8 Byte 3.1415926535897932.
7[tb]Numbers[tb]0[tb]1[tb]-1[tb]3.14159[tb]-3.14159[tb]RQEkP2w[tb]RQAkh+1RELRg[nl]
- Some integers in different encoding..
3[tb]Encoded Numbers[tb]xfeedface[tb]o12345670[tb]b0000111100110101[nl]
- Some strings.
3[tb]Strings[tb]"The quick brown fox\njumped over the lazy dog.[tb]"Salut, c'est été.[tb]"你好马[nl]
- Some binary data.
1[tb]binary buffer[tb]&#42;32 QAkh+1RELRgQAkh+1RELRgQAkh+1RELRgQAkh+1RELRg[nl]
```

Same with removing the tabs and newlines to give you a better idea what you might see in an actual file.

```
- A null value.
1        Null             &#126;
- A true and false array.
2        True And False   T        F
- Some integers and reals.  Last two are 4 Byte 3.1415926535897932f, and 8 Byte 3.1415926535897932.
7        Numbers          0        1        -1        3.14159        -3.14159        RQEkP2w        RQAkh+1RELRg
- Some integers in different encoding..
3        Encoded Numbers  xfeedface        o12345670        b0000111100110101
- Some strings.
3        Strings          "The quick brown fox\njumped over the lazy dog.        "Salut, c'est été.        "你好马
- Some binary data.
1        binary buffer    &#42;32 QAkh+1RELRgQAkh+1RELRgQAkh+1RELRgQAkh+1RELRg
```
