// Henrique Luiz Rieger
// GRR20190357

# include <stdio.h>

# ifndef __PPM__
# define __PPM__

// Definition of a pixel
typedef struct pixel
{
    unsigned int r; // Red channel value of pixel [0..255]
    unsigned int g; // Green channel value of pixel [0..255]
    unsigned int b; // Blue channel value of pixel [0..255]
} pixel_t;

// Definition of an image
typedef struct image
{
    char *magicNumber; // Either P3 or P6, defining a raw or plain type
    unsigned int height; // number of rows of pixels
    unsigned int width; // number of columns of pixels
    unsigned int maxValue; // max value of a color channel
    pixel_t **raster; // pixel_t matrix
    FILE *source; // source stream used to open and close the file
} image_t;

// Read a given '.ppm' file and and interpretate it as an image_t
image_t *ReadImage(const char *name);

// Read the given tiles directory and return an array of amount tiles
image_t **ReadTiles(const char *directory, int amount);

// Close and free a given image
int CloseImage(image_t *image);

// Returns the average color of each channel of a certain region of an image. Return type is a double array
double *AverageOfChannels(image_t *image, int hStart, int hEnd, int wStart, int wEnd);

// Calculates the Red Mean between two arrays of average colors.
// Function assumes size of array = 3 
double RedMean(double *img1Avg, double *img2Avg);

// Changes a region of an image with the contents of a tile
int ChangeContent(image_t * image, int hStart, int hEnd, int wStart, int wEnd, image_t *tile);

// Write a '.ppm' file from a given image_t.
// If file "name" already exists, THIS ACTION WILL ERASE THE CONTENT OF THE FILE
int WriteImage(const char *name, image_t *image);

# endif