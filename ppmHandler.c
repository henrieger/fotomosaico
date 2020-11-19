# include "ppmHandler.h"
# include <stdlib.h>
# include <string.h>
# include <math.h>

// Parse through and fill header params of a '.ppm' image
static int ParseHeader(image_t *image)
{
    // Defines parameter read from file and iterated through loop
    char param[7]; 

    // Read header of file and get the 4 parameters
    int i = 0;
    char c;
    while(i < 4)
    {
        fgets(param, 6, image->source);
        param[strcspn (param, "\n")] = '\0';

        // Checks if line is a comment and goes through file until a new line is found
        if(param[0] == '#')
        {
            c = fgetc(image->source);
            while (c != '\n')
            {
                c = fgetc(image->source);
            }
        }
        else
        {
            // Check which parameter is being read and assigns it
            switch (i)
            {
            // Case is file type (P3 or P6)
            case 0:
                strncpy(image->magicNumber, param, 3);
                break;

            // Case is image width
            case 1:
                image->width = atoi(param);
                break;

            // Case is image height
            case 2:
                image->height = atoi(param);
                break;

            // Case is color channel max value 
            case 3:
                image->maxValue = atoi(param);
                break;
            
            default:
                break;
            }

            // Configures to check next parameter
            i++;
        }
    }

    return 0;
} 

// Parse through and fill raster of a P3 image
static int ParseP3(image_t *image)
{
    for(int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            fscanf(image->source, "%d %d %d", &(image->raster[i][j].r), &(image->raster[i][j].g), &(image->raster[i][j].b));
        }
    }
    
    return 0;
}

// Parse through and fill raster of a P6 image
static int ParseP6(image_t *image)
{
    for(int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            image->raster[i][j].r = fgetc(image->source);
            image->raster[i][j].g = fgetc(image->source);
            image->raster[i][j].b = fgetc(image->source);
        }
    }
    
    return 0;
}

// Read a given '.ppm' file and and interpretate it as an image_t
image_t *ReadImage(const char *name)
{
    // Creates the return object
    image_t *image = malloc(sizeof(image_t));
    if (!image)
    {
        perror("Image alloc failed");
        exit(1);
    }

    // Alloc space for magic number
    image->magicNumber = malloc(sizeof(char) * 3);
    if (!image->magicNumber)
    {
        perror("Image magic number alloc failed");
        exit(1);
    }

    //open the file and assign it to image source file
    image->source = fopen(name, "r");
    if(!image->source)
    {
        perror("No image with name found");
        exit(1);
    }

    // Read header of file
    ParseHeader(image);

    // Allocate enough size for pixel raster
    image->raster = malloc(sizeof(pixel_t *) * image->height);
    if(!image->raster)
    {
        perror("Image raster alloc failed");
        exit(1);
    }
    
    // Allocate enough size for pixel raster first line
    image->raster[0] = malloc(sizeof(pixel_t) * image->height * image->width);
    if (!image->raster[0])
    {
        perror("Image raster line alloc failed");
        exit(1);
    }

    // Set line pointers 
    for (int i = 1; i < image->height; i++)
    {
        image->raster[i] = image->raster[0] + i * image->width;
    }

    // Choose read method from magic number and read the raster
    if(strcmp(image->magicNumber, "P3") == 0)
    {
        ParseP3(image);
    }
    else if(strcmp(image->magicNumber, "P6") == 0)
    {
        ParseP6(image);
    }

    // Close the image source stream
    fclose(image->source);
    image->source = NULL;

    return image;
}

// Read the given tiles directory and from a certain start point and return an array of amount tiles
image_t *ReadTiles(const char *directory, int start, int amount);

// Close and free a given image
int CloseImage(image_t *image)
{
    // Check if image source stream is close and close it
    if(image->source != NULL)
    {
        fclose(image->source);
        image->source = NULL;
    }

    // Free allocated raster of image
    free(image->raster[0]);
    free(image->raster);

    // Free allocated image
    free(image);

    return 0;
}

// Returns the average color of each channel of a certain region of an image. Return type is a double array
double *AverageOfChannels(image_t *image, int hStart, int hEnd, int wStart, int wEnd)
{
    // Error treatment
    if(hStart < 0 || wStart < 0)
    {
        perror("Beginning of row/column parser smaller than 0");
        exit(1);
    }
    if(hStart > image->height || wStart > image->width)
    {
        perror("Beginning of row/column parser greater than image");
        exit(1);
    }
    if(hEnd < hStart || wEnd < wStart)
    {
        perror("Ending of row/column parser smaller than beginning");
        exit(1);
    }

    // Array containing the average of squares of each color channel
    double *avg = malloc(3 * sizeof(double));

    for (int i = 0; i < 3; i++)
    {
        avg[i] = 0;
    }
    
    // Count actual amount of pixels in region
    int count = 0;

    // Sum the square of each pixel color
    for (int i = hStart; i < hEnd; i++)
    {
        for (int j = wStart; j < wEnd; j++)
        {
            // Check if pixel exists inside image
            if (i < image->height && j < image->width)
            {
                avg[0] += pow(image->raster[i][j].r, 2);
                avg[1] += pow(image->raster[i][j].g, 2);
                avg[2] += pow(image->raster[i][j].b, 2);
                count++;
            }
        }
    }

    // Divide sums and takes square root
    for (int i = 0; i < 3; i++)
    {
        avg[i] = sqrt(avg[i]/count);
    }

    return avg;
}

// Calculates the Red Mean between two arrays of average colors.
// Function assumes size of array = 3 
double RedMean(double *img1Avg, double *img2Avg)
{
    // Calculate the difference between the averages of each channel
    double deltaR = img1Avg[0] - img2Avg[0];
    double deltaG = img1Avg[1] - img2Avg[1];
    double deltaB = img1Avg[2] - img2Avg[2];

    // Calculate average between red channels
    double r = (img1Avg[0] + img2Avg[0])/2;

    
    // Calculate the Red Mean
    double c = sqrt((2 + r/256) * pow(deltaR, 2) +
                    (4 * pow(deltaG, 2)) +
                    (2 + (255 - r)/256) * pow(deltaB, 2));

    return c;
}

// Changes a region of an image with the contents of a tile
int ChangeContent(image_t * image, int hStart, int hEnd, int wStart, int wEnd, image_t *tile);

// Write pixel raster in P3 format
static int WriteP3(image_t *image, FILE *output)
{
    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            fprintf(output, "%d %d %d ", image->raster[i][j].r, image->raster[i][j].g, image->raster[i][j].b);
        }
        fprintf(output, "\n");
    }

    return 0;
}

// Write pixel raster in P6 format
static int WriteP6(image_t *image, FILE *output)
{
    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            fprintf(output, "%c%c%c", image->raster[i][j].r, image->raster[i][j].g, image->raster[i][j].b);
        }
    }

    return 0;
}

// Write a '.ppm' file from a given image_t.
// If file "name" already exists, THIS ACTION WILL ERASE THE CONTENT OF THE FILE
int WriteImage(const char *name, image_t *image)
{
    // Create output stream
    FILE *output;
    output = fopen(name, "w");

    // Prints header of file
    fprintf(output, "%s\n%d %d\n%d\n", image->magicNumber, image->width, image->height, image->maxValue);

    // Select pixel raster format and write into output
    if(strcmp(image->magicNumber, "P3") == 0)
    {
        WriteP3(image, output);
    }
    else if(strcmp(image->magicNumber, "P6") == 0)
    {
        WriteP6(image, output);
    }
    
    // Close output stream
    fclose(output);

    return 0;
}