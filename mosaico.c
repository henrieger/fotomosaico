# include "ppmHandler.h"
# include "inputHandler.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

// Max number of tiles in array
# define TILE_MAX 2000

int main(int argc, char **argv)
{
    // Check if use of arguments is overall correct
    if(CheckNArguments(argc))
    {
        exit(1);
    }

    // Check use of input file argument and save it in a string
    char inputFile[256];
    strcpy(inputFile, CheckInputFile(argc, argv));
    if(inputFile[0] == '\0')
    {
        exit(1);
    }

    // Check use of output file argument and save it in a string
    char outputFile[256];
    strcpy(outputFile, CheckOutputFile(argc, argv));
    if(outputFile[0] == '\0')
    {
        exit(1);
    }

    // Check use of tile directory argument and save it in a string
    char tileDirectory[256];
    strcpy(tileDirectory, CheckTileDirectory(argc, argv));
    if(tileDirectory[0] == '\0')
    {
        exit(1);
    }

    // Open input image
    image_t *inputImage;
    inputImage = ReadImage(inputFile);

    // Open tile array
    image_t **tiles;
    tiles = ReadTiles(tileDirectory, 2000);

    // Read average of colors in tiles
    double **tileAvgs = NULL;
    for (int i = 0; tiles[i] != NULL; i++)
    {
        tileAvgs = realloc(tileAvgs, (i + 1) * sizeof(double *));
        tileAvgs[i] = AverageOfChannels(tiles[i], 0, tiles[i]->height, 0, tiles[i]->width);
    }

    // Assign tile that will serve as standard for all tiles
    image_t *tileExample = tiles[0];
    
    // Assign image height and width to floating point variables
    double imageHeight = (double) inputImage->height;
    double imageWidth = (double) inputImage->width;

    // Number of tiles that fit into the input image 
    double hBlocks = ceil(imageHeight / tileExample->height);
    double wBlocks = ceil(imageWidth / tileExample->width);
    // printf("%f %f\n", hBlocks, wBlocks);
        
    double leastMean; // Least difference between tile and region
    int leastMeanIndex; // Index of that tile
    
    // Auxiliar variables
    double *AvgOfImage;
    // double *AvgOfTile;
    double currMean;

    // Substitute all parts of input input image for a similar tile 
    for (int i = 0; i < hBlocks * tileExample->height; i += tileExample->height)
    {
        for (int j = 0; j < wBlocks * tileExample->width; j += tileExample->width)
        {
            // Initializes variables for measuring the least distance color pattern
            leastMean = inputImage->maxValue;
            leastMeanIndex = 0;

            // Assign average of color channels of current region in image
            AvgOfImage = AverageOfChannels(inputImage, i, i + tileExample->height, j, j + tileExample->width);

            // Find closest tile to current block
            for (int k = 0; tiles[k] != NULL; k++)
            {
                currMean = RedMean(AvgOfImage, tileAvgs[k]);
                if(leastMean > currMean)
                {
                    leastMeanIndex = k;
                    leastMean = currMean;
                }
            }

            // Change region for found tile
            ChangeContent(inputImage, i, i + tileExample->height, j, j + tileExample->width, tiles[leastMeanIndex]);

            // Free allocated space for image region color pattern
            free(AvgOfImage);
        }
        
    }

    // Free tile average array
    for (int i = 0; tiles[i] != NULL ; i++)
    {
        free(tileAvgs[i]);
    }
    free(tileAvgs);
    
    // Write image with tiles
    WriteImage(outputFile, inputImage);

    // Close input image
    CloseImage(inputImage);

    // Close tiles
    for (int i = 0; tiles[i] != NULL; i++)
    {
        CloseImage(tiles[i]);
    }
    
    return 0;
}