## Version spoof

For some specific* reason Sketch does not allow a file to open with (version)
lower then currently used version to save file. I was simply interested what was
behind all that specification, so the story.

**Abstract**: *We created two instances of save file from SketchApp and present
their package model. Based on that we shall write a plugin that could allow
import and export of based versions with a near distance diff. Our meta was
generated through Sketch (v)43.1 and respective newest (v)47.1.*

*specific reason is one who consists of magic, fog, SaaS or subscribtion
based model, call it whatever you like, for an enduser*

## Introduction

I've used two different version of SketchApp to save file and investigate
further information. Approach I took is to examine the file from perspective of
a blob since no prior information did I have about them.
  
Some resource may be found on interwebz about this file and these are referenced
at the end of the file.

## File

Lets start from the root of the problem, what excatly is `*.sketch`.
  
```sh
$ file version/blob43-1.sketch
version/blob43-1.sketch: Zip archive data, at least v2.0 to extract
```

Oh, great. So it's a typical ZIP format. Interesting thing is most of the files
are compressed in one way or another. A lot of times while reversing stuff or
application tree you will see ziparcs.
  
```sh
# After unpacking

$ tree version/unzip/43-1
version/unzip/43-1
├── document.json
├── meta.json
├── pages
│   └── 18D1FD02-FD9D-495C-A553-F174B9C89228.json
├── previews
│   └── preview.png
└── user.json

2 directories, 5 files
```

### META.JSON

Contains metadata about the document itself (a list of pages and artboards, 
Sketch version used to save the file, fonts used…). It's equivalent to the 
output you'd get from running sketchtool metadata on the file.

**Important notice:** This may be exactly what file we are looking for. I've
made this file readable by applying beauty. We can see some indication that this
file is being used as a Sketch identification.

```
   ... strip
   "appVersion":"43.1",
   "build":39012,

   ...
   "created":{  
      "commit":"335a30073fcb2dc64a0abd6148ae147d694c887d",
      "build":39012,
      "appVersion":"43.1",
      "version":88
   },

   "version":88,
   "saveHistory":[  
      "NONAPPSTORE.39012"
    ...
```


### DOCUMENT.JSON

Contains common data for all pages of a document, like shared styles, and a 
link to the JSON files in the pages folder.

### PAGES FOLDER

Contains a JSON file per page of the document. Each file describes the contents 
of a page, in a format similar to what you'd get by running sketchtool dump on 
the file.

### IMAGES FOLDER

The images folder contains all the bitmaps that are used in the document, at 
their original scales (so if the user imported a 1000x1000px image and then 
resized it to 200x200px, the full 1000x1000px file will be stored).

**Notice:** We don't have this folder for simplicity and staying on topic. Read
some other paper.

### USER.JSON

Contains user metadata for the file, like the canvas viewport & zoom level 
for each page, UI metadata for the app (panel dimensions, etc…) and whether the 
document has been uploaded to Sketch Cloud.

### PREVIEWS FOLDER

Contains a preview image of the last page edited by the user. If the page's 
size is less than 2048x2048 it will be stored at full size, otherwise it'll be 
scaled to fit a 2048x2048 square.

```sh
$ file version/unzip/43-1/previews/preview.png
version/unzip/43-1/previews/preview.png: PNG image data, 400 x 400, 8-bit/color RGBA, non-interlaced
```

*I just remembered I sometimes get questions how I get into all RE stuff. I can
explain it like this, exactly! I'm reading and writing to see whats behind.*

## Reversing meta

We took both saved file, one from version 43.1 and the other saved with version 
47.1 (*at the time the latest version*) - the differences stayed almost same.

Some differences may be seen if you look at `version/save.diff` and
`version/save.clean.diff` which is a cleanup version of the first one.

Each of these can be replaced. It's I guess our duty to found out either which
PE function catch this on File open or bypass through linked file data.

### Modular approach

This vector should purpose of faking a current SketchApp version we talked
previously, as a reminder: thats the one we saved the file in. In this example,
it will be (v)47.1 as a major and higher one. 
  
Therefore our new **blob4-mod.sketch** is born.
  
It is a single file that should be allowed and enabled for use in older
SketchApp version.
  
```sh
$ file version/blob4-mod.sketch
version/blob4-mod.sketch: Zip archive data, at least v2.0 to extract
```

### Catch

The thing is, it's not worth it. The file is being compressed with other then
zip flags so we should definitely check out what this archive start with. For
that a good approach is to look through first bytes.

```sh
$ od -t x1 -N 10 version/blob43-1.sketch
0000000    50  4b  03  04  14  00  02  00  08  00

$ xxd -l 10 version/blob43-1.sketch
00000000: 504b 0304 1400 0200 0800                 PK........
```

But if we try to open zipped file back in Sketch (for example - v43 save in v43
app) we will be presented to error same or similar to this:

hxxp://

But further examination can show that both of these are with same files but
different checksums:

```sh
$ unzip -v -l version/blob43-1.sketch
Archive:  version/blob43-1.sketch
 Length   Method    Size  Cmpr    Date    Time   CRC-32   Name
--------  ------  ------- ---- ---------- ----- --------  ----
     788  Defl:X      404  49% 00-00-1980 00:00 0dc8b716  document.json
    1709  Defl:X      536  69% 00-00-1980 00:00 58a041e3  pages/18D1FD02-FD9D-495C-A553-F174B9C89228.json
     166  Defl:X      143  14% 00-00-1980 00:00 37163421  user.json
     554  Defl:X      297  46% 00-00-1980 00:00 af3619f9  meta.json
    3678  Defl:N     3683  -0% 00-00-1980 00:00 ed995af0  previews/preview.png
--------          -------  ---                            -------
    6895             5063  27%                            5 files


$ unzip -v -l 43-1zip.sketch
Archive:  43-1zip.sketch
 Length   Method    Size  Cmpr    Date    Time   CRC-32   Name
--------  ------  ------- ---- ---------- ----- --------  ----
    1098  Defl:N      449  59% 10-31-2017 19:27 f646c9c9  document.json
     768  Defl:N      341  56% 10-31-2017 19:22 ca14f363  meta.json
       0  Stored        0   0% 10-31-2017 19:31 00000000  pages/
       0  Stored        0   0% 10-31-2017 19:13 00000000  previews/
     205  Defl:N      162  21% 10-31-2017 19:35 708a4b19  user.json
--------          -------  ---                            -------
    2071              952  54%                            5 files
```

## Outro

**Thanks TNT Edition <3**
