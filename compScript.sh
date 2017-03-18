
# !/bin/sh

echo 'Compiling c program...'
gcc -o hide src/ppmlib.c src/hide.c src/hidelib.c
echo 'C programs compiled!'