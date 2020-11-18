
CC = gcc
CFLAGS = -Wall -std=c99

# Default rule
all: mosaico

# Linking rule
mosaico: mosaico.o ppmHandler.o

# Compiling rules
mosaico.o: mosaico.c ppmHandler.h
ppmHandler.o: ppmHandler.c ppmHandler.h

# Activate debug options
debug: CFLAGS += -g
debug: CPPFLAGS += -DDEBUG
debug: all

# Cleans linking files from project
clean:
	-rm -f *.o

# Cleans all but source code and material
purge: clean
	-rm -f mosaico