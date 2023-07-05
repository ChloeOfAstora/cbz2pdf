# cbz2pdf

## About
cbz2pdf is a simple utility that converts manga/comic files (cbz) to pdf format.  
Right now only jpg compression is supported which can be controlled using the -q parameter.

## Usage: 
```cbz2pdf "filename"```

## Making:
Run make, current compiler is clang++-9.


## Todo:
* Optimize file conversion in order to save space.
* Allow batch converting.
* Allow resizing the images.
* Add more tools.
* Please feel free to suggest more features.


## Used headers:
| library/header | link|
| ------------- | ------------- |
| PDFGen     |         https://github.com/AndreRenaud/PDFGen  |
| Miniz       |        https://github.com/richgel999/miniz    |
| Flags        |       https://github.com/sailormoon/flags    |
| stb_image.h   |      https://github.com/nothings/stb        |
| stb_image_write.h|   https://github.com/nothings/stb        |