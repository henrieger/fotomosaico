# Standard make system variables
CC = gcc
CFLAGS = -Wall -std=c99

# Default rule
all: mosaico

# Linking rule
mosaico: mosaico.o ppmHandler.o inputHandler.o

# Temorary
testeOpenClose: testeOpenClose.o ppmHandler.o
testeOpenClose.o: testeOpenClose.c


# Compiling rules
mosaico.o: mosaico.c ppmHandler.h
ppmHandler.o: ppmHandler.c ppmHandler.h
inputHandler.o: inputHandler.c inputHandler.h

# Activate debug options
debug: CFLAGS += -g
debug: CPPFLAGS += -DDEBUG
debug: all

# Cleans linking files from project
clean:
	-rm -f *.o 

# Cleans all but source code and material
purge: clean
	-rm -f mosaico teste*.c