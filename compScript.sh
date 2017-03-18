# !/bin/sh

echo 'Compiling c program...'
gcc -o hide src/ppmlib.c src/hide.c src/steglib.c
gcc -o unhide src/ppmlib.c src/unhide.c src/steglib.c
echo 'C programs compiled!'