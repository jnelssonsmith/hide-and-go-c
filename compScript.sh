# !/bin/sh

echo 'Compiling c program...'
gcc -o hide src/ppmlib.c src/hide.c src/steglib.c
echo 'C programs compiled!'