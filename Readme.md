# cbz2pdf

## About
cbz2pdf is a simple utility that converts manga/comic files (cbz) to pdf format.  
Right now only jpg compression is supported which can be controlled using the -q parameter.

## Usage: 
The base command is this:
```cbz2pdf "filename"```

But that results in very large files as it is uncompressed, I would recommend using at least -q 65.
To get even lower file sizes you can scale the resolution of the images and/or convert the images to greyscale.

## Why?
This program is intended to help people who want to read comics/manga/etc. on e-readers or other devices that might not support cbz files or have limited storage space.

## Making for linux:
Run make, current compiler is clang++-9.

## Features:
* Converting the images to grayscale.
* Resizing the images to make them smaller.
* Selecting jpg quality (1-100).

## Todo:
* (Maybe) Optimize file conversion in order to save space.
* Allow batch converting.
* Allow merging multiple cbz files to one pdf file.
* Add more tools.
* Please feel free to suggest more features.


### Used headers:
| library/header | link|
| ------------- | ------------- |
| PDFGen     |         https://github.com/AndreRenaud/PDFGen  |
| Miniz       |        https://github.com/richgel999/miniz    |
| Flags        |       https://github.com/sailormoon/flags    |
| stb_image.h   |      https://github.com/nothings/stb        |
| stb_image_write.h|   https://github.com/nothings/stb        |
|stb_image_resize.h|   https://github.com/nothings/stb        |