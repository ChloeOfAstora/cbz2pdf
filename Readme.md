# cbz2pdf

## About
cbz2pdf is a simple utility that converts manga/comic files (cbz) to pdf format.

## Usage: 
Run the program normally for the GUI version, fill in the settings you want, select your cbz files and just hit convert!
The program will convert all your cbz files to pdf files and save them in the same directories as the original files.
To use the command line version, run the program with -c and -h to learn about the settings, the default settings result in very large file sizes.

## Why?
This program is intended to help people who want to read comics/manga/etc. on e-readers or other devices that might not support cbz files or have limited storage space.

## Making for linux:
Make sure you have clang++-9 and fltk installed and then just run make.

## Making for windows
Making this program from the source files for windows is not recommended, but can be done using MYSYS2 Mingw64 with some effort.

## Features:
* A gui interface and a command line version.
* Converting the images to grayscale.
* Resizing the images to make them smaller.
* Selecting jpg quality (1-100).
* Converting multiple files at the same time.

## Todo:
* Allow merging multiple cbz files to one pdf file.
* Improving input validation for the gui version.
* Release a good windows build.
* Please feel free to suggest more features.


### Used headers:
| library/header     | link|
|--------------------| ------------- |
| PDFGen             |         https://github.com/AndreRenaud/PDFGen  |
| Miniz              |        https://github.com/richgel999/miniz    |
| Flags              |       https://github.com/sailormoon/flags    |
| stb_image.h        |      https://github.com/nothings/stb        |
| stb_image_write.h  |   https://github.com/nothings/stb        |
| stb_image_resize.h |   https://github.com/nothings/stb        |
| FLTK               |   https://www.fltk.org/index.php        |