# we define common dependencies so we don't have to write out the same
# thing over and over
COMMON_DEPS = src/ppmlib.c src/ppmlib.h src/steglib.c src/steglib.h

# use make all if you want to easily compile both hide and unhide
all: src/hide.c src/unhide.c $(COMMON_DEPS)
	gcc -o hide src/ppmlib.c src/hide.c src/steglib.c
	gcc -o unhide src/ppmlib.c src/unhide.c src/steglib.c

# use make hide if you only wish to compile hide
hide: src/hide.c $(COMMON_DEPS)
	gcc -o hide src/ppmlib.c src/hide.c src/steglib.c

# use make unhide if you only wish to compile unhide
unhide: src/unhide.c $(COMMON_DEPS)
	gcc -o unhide src/ppmlib.c src/unhide.c src/steglib.c

# use make clean to remove the compiled files
clean: 
	rm -f unhide hide
