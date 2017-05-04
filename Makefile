# we define common dependencies so we don't have to write out the same
# thing over and over
COMMON_DEPS = src/ppmlib.c src/ppmlib.h src/steglib.c src/steglib.h src/hideUtils.c src/hideUtils.h src/unhideUtils.c src/unhideUtils.c
FLAGS=`sdl2-config --cflags`
SDLLIBS=`sdl2-config --libs`

# use make all if you want to easily compile both hide and unhide
all: src/hide.c src/unhide.c $(COMMON_DEPS)
	gcc -Wall -o hide src/ppmlib.c src/hide.c src/steglib.c src/hideUtils.c $(CFLAGS) $(SDLLIBS)
	gcc -Wall -o unhide src/ppmlib.c src/unhide.c src/steglib.c src/hideUtils.c $(CFLAGS) $(SDLLIBS)

# use make hide if you only wish to compile hide
hide: src/hide.c $(COMMON_DEPS)
	gcc -Wall -o hide src/ppmlib.c src/hide.c src/steglib.c src/hideUtils.c $(CFLAGS) $(SDLLIBS)

# use make unhide if you only wish to compile unhide
unhide: src/unhide.c $(COMMON_DEPS)
	gcc -Wall -o unhide src/ppmlib.c src/unhide.c src/steglib.c src/hideUtils.c $(CFLAGS) $(SDLLIBS)

# use make clean to remove the compiled files
clean: 
	rm -f unhide hide output/* standardHideUnhideWorked.jpg sFlagWorked.jpg mFlagWorked.gif someotherthing.jpg
	
