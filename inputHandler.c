// Henrique Luiz Rieger
// GRR20190357

# include "inputHandler.h"
# include <stdio.h>
# include <string.h>

// Check number of arguments (if there are too many or if args don't have parameters)
int CheckNArguments(int argc)
{
    if (argc >= 8 || argc % 2 == 0)
    {
        perror("Incorrect number of arguments");
        return 1;
    }
    return 0;
}

// Check existence of a content in a array of strings
static int CheckIfInArray(int size, char **array, char *element)
{
    // First position in which element of array is
    int position = -1;

    // Iterate through array until element is found
    for(int i = 0; i < size; i++)
    {
        if(strcmp(array[i], element) == 0)
        {
            position = i;
            break;
        }
    }

    return position;
}

// Check the existence of input file given in argument and return it
char *CheckInputFile(int argc, char **argv)
{
    // Check existence of '-i' argument
    int position = CheckIfInArray(argc, argv, "-i");

    // Check if argument is called
    if(position < 0)
    {
        return "stdin";
    }
    // Check next positions content
    else if(position + 1 > argc || argv[position + 1][0] == '-' )
    {
        perror("Incorrect use of argument -i");
        return "\0";
    }

    return argv[position + 1];
}

// Check the existence of input file given in argument and return it
char *CheckOutputFile(int argc, char **argv)
{
    // Check existence of '-o' argument
    int position = CheckIfInArray(argc, argv, "-o");

    // Check if argument is called
    if(position < 0)
    {
        return "stdout";
    }
    // Check next positions content
    else if(position + 1 > argc || argv[position + 1][0] == '-' )
    {
        perror("Incorrect use of argument -o");
        return "\0";
    }

    return argv[position + 1];
}

// Check the existence of tile directory given in argument and return it
char *CheckTileDirectory(int argc, char **argv)
{
    // Check existence of '-p' argument
    int position = CheckIfInArray(argc, argv, "-p");

    // Check if argument is called
    if(position < 0)
    {
        return "tiles";
    }
    // Check next positions content
    else if(position + 1 > argc || argv[position + 1][0] == '-' )
    {
        perror("Incorrect use of argument -p");
        return "\0";
    }

    // If directory name has / at the end, take it out
    argv[position + 1][strcspn (argv[position + 1], "/")] = '\0';

    return argv[position + 1];
}
