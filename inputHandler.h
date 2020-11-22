// Henrique Luiz Rieger
// GRR20190357

#ifndef __INPUT__
# define __INPUT__

// Check number of arguments (if there are too many or if args don't have parameters)
int CheckNArguments(int argc);

// Check the existence of input file given in argument and return it
char *CheckInputFile(int argc, char **argv);

// Check the existence of input file given in argument and return it
char *CheckOutputFile(int argc, char **argv);

// Check the existence of tile directory given in argument and return it
char *CheckTileDirectory(int argc, char **argv);

# endif