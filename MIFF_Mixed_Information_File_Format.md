# M.I.F.F.: Mixed Information File Format

```
Author:             Robbert de Groot
Date:               2019-05-15
Copyright:          2019, Robbert de Groot
License (Library):  MIT License.
License (Document): Creative Commons Attribution-NoDerivs.[LINK](https://creativecommons.org/licenses/by-nd:4.0) (CC BY-ND)
```

## Table Of Contents:

- [M.I.F.F.: Mixed Information File Format](#miff-mixed-information-file-format)
  - [Table Of Contents:](#table-of-contents)
- [1 - M.I.F.F.](#1---miff)
  - [1.1 - Discussion](#11---discussion)
  - [1.2 - Goals](#12---goals)
  - [1.3 - Design Decisions:](#13---design-decisions)
  - [1.4 - Disclosure](#14---disclosure)
- [2 - Format](#2---format)
  - [2.1 - File Header](#21---file-header)
  - [2.2 - Content](#22---content)
    - [2.2.1 - Header Section](#221---header-section)
    - [2.2.2 - Key Section](#222---key-section)
    - [2.2.3 - Value Section](#223---value-section)
  - [Examples](#examples)
- [Notes](#notes)
  - [Comparing with JSON.](#comparing-with-json)
    - [File Size](#file-size)
    - [Comments](#comments)
  - [Comments on MIFF](#comments-on-miff)


# 1 - M.I.F.F.

## 1.1 - Discussion

**What is the purpose of M.I.F.F. (MIFF)?**

MIFF is intended to be a simple file format for storing data.  Any data.  And large amounts of data without being too fat.

## 1.2 - Goals

* **Simple**    The format should be simple for the developers to export their data and still be fairly simple to re-import that data.

* **Brief**     The format should not produce unnecessary waste.  The data in some cases will be quite large so it should not bloat the data too much.  Meaning, file sizes should not become overly large.  However, because of point 1 there will always be some bloat.

* **Flexible**  The format needs to be able to accomodate change or specific data.  As years go by, software will also change and requirements will change.  The format needs to try its best to keep up without importers and exporters to be completely remade.

* **Accurate**  The format needs to be able to maintain accuracy of the data.  Namely floating point values.  The in memory value when exported should be reimported without any change.

## 1.3 - Design Decisions:

**Why not XML or JSON?**

I find XML is too verbose.

JSON is much better than XML and is a reasonable alternative to XML.  I do have a few issues that I found out with JSON.  JSON format doesn't specify any limits on numbers.  Which is great.  However implementations can do what they like when it comes to numbers they can't handle.  I.E. Javascript change handle 64 bit integers and unsigned integers.  So it tends to write those values out as strings.  The JSON format has no problems but Javascript does.  JSON format doesn't have NAN, Infinity defined for any of the numbers.  So those too are written out as strings and more worryingly, there is no standard on how to write that information out.

Both are very flexible but that can be a double edged sword.  I feel there should be something better.  I do not claim MIFF is better, it is just different.

**Why big endian for multibyte data types?**

In the past my company was multi-platform, SGI IRIX, SUN OS, SUN Solaris and Windows (NT/2000/XP/etc.)  At that time the architecture on some of the other platforms was big endian and we stored the data in the native format of the machine.  However this lead to issues when users moved their data over to a machine with a different endian.  The problems that we faced were trivial to solve but were very annoying and yet anothering thing to remember.  So standardizing on one option is easier than having to support two options.  I go with simplicity, only one option to rule them all!  It keeps things simpler even if it does mean a potential performance hit for a platform.  This format is not meant to be good in performance; its main goal is to be good in getting data moved from one place to another, accurately.

## 1.4 - Disclosure

I, Robbert de Groot, have been a professional Software Developer since 1995.

This format is currently not sanctioned by any software vendor.  This was an attempt on developing something in place of existing or developing options which I find lacking or distasteful in some fashion.

I can be reached at the following email address.

zekaric@gmail.com

# 2 - Format

A MIFF File is a UTF8 text file.

Each line in the text file is a 'record' or piece of data.  A record is delimited by [tb], '\t', tab characters and is terminated by a UNIX [nl], '\n', new line character.  The contents of the file is a collection of key value pairs.

The intent with this format is to make only one read pass over the file.  There is no focus on making the file randomly accessable or modifiable.  This format is not intended as a substitute for native formats for any software package.  This file is meant to be a data transfer file format between softwared packages or a general format to store data.  It could be used as a native format if desired, just like where JSON is used.

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

If a sub-format name exists it is not limited to any characters.  A MIFF header is in UTF8 format always.  The only limit to the format name is that it can not be longer than 255 bytes in length and it cannot contain [tb]s or [nl]s.  If there are leading and trailing spaces, then that is part of the format name as silly as that may sound.  Spaces internal to the format name are also allowed and are significant.  I say 255 bytes instead of characters because in UTF8, one UNICODE character or codepoint can span multiple bytes.  So this name can not exceed this byte count.

## 2.2 - Content

Each line is split into 3 sections.  The header for the line, the key for the line, and the value section.

Each section is separated by a [tb].  The entire line ends in a [nl].

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
| [count] | This will be a hexidecimal number 0 or larger to indicate an array count for the value.  Whatever data the key is suppose to hold, there should be that many of them.  If you have an array of objects, use a separate count key-value and a scope for each individual object in the array instead. |
| &#42; | This indicates an array of unknown size.  In generaly I would advise trying to avoid this as much as possible.  It is better to know beforehand how many bytes or how many items are needed.  However sometimes this is unavoidable, but in my opinion, avoid as much as you can. |


The header can have leading spaces or [tb]s to make it more obvious which scope it belongs to.  However these spaces and [tb]s are not needed unless you want to make it more readable.  They are ignored when reading the header.

### 2.2.2 - Key Section

Keys are any string that is up to 255 bytes long.  I say bytes because in UTF8 format, UNICODE characters can take up multiple bytes.  So as long as the key is not larger than 255 bytes, the key is valid.  The key can have spaces or any special characters you like except [tb]s and [nl]s.  This does mean leading, trailing, and internal spaces in the key are significant.

### 2.2.3 - Value Section

The sub-format of the file should be dictating what values follow the key.  This should be defined somewhere in the documentation of the sub-format.

There can be 0 or more values associated with the key.  Again depending on what the sub-format documenation dictates.  Each value will be separated by a [tb].

If there is a count in the line header other than 1, then there should be multiple copies of whatever type that is associated with the key.

Values have a simple format.

```
[header][valueData]
```

The header is a single character.

The valueData will depend on the header.

| Header | Description |
| --- | --- |
| &#126; | The value data is unset/null.  Nothing else follows the header. |
| 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, - | A hexidecimal number.  If a boolean value this will be 0 (false) or 1 (true).  If an integer or natural then it will be that number in hexidecimal form.  Leading 0's will be trimmed off.  If a real value then it will be the byte value of the real (8 characters for a float, 16 characters for a double).  For integers we can have a negative hexidecimal. |
| G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V,<br />Z80, Z8+M, Z8-M, Z8+I, Z8-I, Z8?,<br />Z40, Z4+M, Z4-M, Z4+I, Z4-I, Z4? | A hexidecimal representation of a real value (double).  Leading 0 half bytes are removed (G characters).  The Z values are constants for 0, maximum positive value, maximum negative value, positive infinity, negative infinity, and not a number for doubles (8) and floats (4).  They are to save a little bit of space when written out.  All real values follow IEEE 754 formats. |
| "[count] | The value data will be a string.  There is no closing **"**.  [count] denotes the size of the string in bytes represented in hexidecimal form.  A single space follows the [count] value and the string immediately follows up to the next [tb] or [nl] which ever is next.  All [tb], [nl], and '\' characters inside the string are escaped.  Meaning a [tb] inside the string will be '\t', a [nl] inside teh string will be a '\n', and a '\' inside the string will be a '\\'.  Nothing else is escaped and should be written as is in the file. |
| &#42;[count] | The value data is a binary buffer.  [count] denotes the size of the binary buffer in bytes represented in hexidecimal form.  A single space follows the [count] value and the binary buffer in hexidecimal encoding immediately follows. |
| [Any other letter] | The value data is define by the subformat and decoded by an external to the library function. |

a, b, c, d, e, f are not accepted for hex values.  The G-V are used for real value storage and basically map to the hex numbers 0-9,A-F.  It is so a real value can be differenciated from an integer or natural value.  Lower case g-v are not accepted for real hex values.

## Examples

```
- A null value.
1[tb]Null[tb]&#126;[nl]
-
- A boolean true without explicit setting.  If this was missing then isVisible is false.
0[tb]isVisible[nl]
-
- Or explicitly setting the value.  True is 1, False is 0.
1[tb]isVisible[tb]1[nl]
1[tb]isOpen[tb]0[nl]
-
- A true and false array.
2[tb]True And False[tb]1[tb]0[nl]
-
- Some integers, reals, and complex numbers.  "Z80" is (double) 0, "Z40" is (float) 0.  HOI... is (double) PI, TRG... is (float) PI.
7[tb]Numbers[tb]0[tb]1[tb]-1[tb]Z80[tb]HOITKKLKVRIHGPKG[tb]Z8+M[tb]Z8?[tb]TRGVKPKG[tb]Z4+M[nl]
-
- Some strings.
3[tb]Strings[tb]"2D The quick brown fox\njumped over the lazy dog.[tb]"11 Salut, c'est été.[tb]"&#42; 你好马[nl]
-
- Some binary data.
1[tb]binary buffer[tb]&#42;20 FEEDFACEDEADBEEFFEEDFACEDEADBEEFFEEDFACEDEADBEEFFEEDFACEDEADBEEF[nl]
```

# Notes

## Comparing with JSON.

### File Size

File size from the test program which dumps out every data type MIFF can support and also dumps out the exact same data to a similar JSON file.

| File | Size In Bytes Text | Size In Bytes Zipped |
| --- | --- | --- |
| JSON compressed to one line | 32,335 | 2,178 |
| JSON Miff style formatted | 32,550 | 2,316 |
| MIFF | 29,432 | 2,191 |


JSON compressed to one line removed quite a bit of formatting bytes.  When compressed this difference becomes almost negligable.

### Comments

Doing an apples to apples comparison with JSON, MIFF saves quite a few bytes in the file size.  Compressed, MIFF fares slightly better but not an easy win.  We aren't talking huge differences.

Where MIFF saves some space.

| Part | Comments | Size | Readable | Speed |
| --- | --- | --- | --- | --- |
| Keys | MIFF does not need keys to be enclosed in quotes.<br />JSON encloses keys within quotes. | + MIFF | - | - |
| Array Count | MIFF uses a line header that includes an array count.<br />JSON does not have such concept. | + JSON | + MIFF | + MIFF (Memory resizing not needed) |
| Record Ender | MIFF needs just a single [nl].<br />JSON will depend on formatting. | - (+ MIFF if JSON has formatting) | - | - |
| Header | MIFF has a header with versions<br />JSON does not have a header. | + JSON | + MIFF | - |
| True, False, Null | MIFF uses "1", "0", "&#126;".<br />JSON uses "true", "false", "null" | + MIFF | - | + MIFF |
| Numbers (integer, natural) | MIFF writes all integral values in HEX format.<br />JSON writes numbers in human readable format. | + MIFF | + JSON | + MIFF |
| Numbers (real) | MIFF writes all real values in a binary dump hex format.<br />JSON writes numbers in human readable format. | - (depends) | + JSON | + MIFF (+ accuracy) |
| Numbers (real, special values) | Infinity, -Infinity, Not a Number, double/float Maximums.<br />MIFF has constants for these values.<br />JSON has no standard for these values.  Implementations vary. | + MIFF | + MIFF | + MIFF |
| Array of objects | MIFF will require you to just make duplicate key blocks<br />JSON has object definition built in. | + JSON | + JSON | - |

**Is MIFF better?**

No.  It will depend on how much value you place on readability and editability of the format.

I suspect MIFF will have a slight advantage over reading and writing speed over JSON because there is less processing of numbers from the text format to binary in memory representation since the text format of the number is basically binary.

The question is, how important is it to read the file and edit it manually?  Maybe for testing and debugging but in general, common everyday users are never going to.  So for the most part, whoever is actually reading the file will be a developer or similar.  Granted the real numbers will be a pain in the ass even for developers.

## Comments on MIFF

**Are there parts of the format that I dislike?**

Currently the real number handling I am a bit on the fence on.  I use different characters for the hex numbers just so that I can differentiate them from integer and natural numbers.  And to figure out if they are representing a double (8 bytes) or a float (4 bytes) I have to write all the bytes out.  Which will mean 16 bytes for a double and 8 bytes for a float.  However this allows me to potentially support short floats (2 bytes) and long doubles (10 bytes), or larger if need be.

Why write real values out this way?  I want quick and accurate store and load of real values.  Writing them out in human readable form is nice and all but there is a conversion cost and when dealing with large numbers, they will take up a lot more space.  Meaning, a non-rational real number as a double will mean 15 decimals of precision plus the decimal point and sign possibly with an exponent portion.  You are looking at 21 bytes potentially instead of 16.  I could encode in base64 for a more compact text representation but I will talk about that further down.

I do like human readable numbers but I find that there is a risk of losing accuracy with that over using a binary dump of the number.  And doing a conversion from human readable numbers to binary numbers is not cheap when you have to convert a huge amount of them.

**Why not use Base64 for binary data?**

I started with base 64.  The problem is that base64 encoding adds complication.  And in the end, zipping up a similar wild with base64 encoded binary data (even for double and floats) results in a larger zip file.  Or at least with my basic test with the test program.  It seems Hex dumping of binary data results in better data compression.  I suspect it is because half byte hex dumping matches the raw byte data more closely and results in a lot more pattern matching; whereas the base64 encoding, which shifts bits around and causes, what appears to be, a little more 'randomness', give compression algorithms a harder time.  Compression algorithms do not like randomness.  Base64 saves in text file size but negatively affects compressed file sizes.

So I figured, why bother with the added complexity of base64.  If you want to save space, compress the file.  Text files are inefficient to begin with.
