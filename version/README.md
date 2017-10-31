## Version spoof

For some specific* reason Sketch does not allow a file to open with (version)
lower then currently used version to save file. I was simply interested what was
behind all that specification, so the story.

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
  

