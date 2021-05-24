#include "helpers.h"
#include <math.h>
//#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // initializa the buffer vaiable to be used later
    float buffer = 0.0;

    // iterating the image pixel-by-pixel
    //printf("**** height,width = %i, %i ******",height,width);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // replacing value with avarage of the three colours to get the grey scale image
            buffer = round((image[j][i].rgbtBlue + image[j][i].rgbtRed + image[j][i].rgbtGreen) / 3.00);
            image[j][i].rgbtBlue = buffer;
            image[j][i].rgbtRed = buffer;
            image[j][i].rgbtGreen = buffer;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // initializa the buffer vaiable to be used later
    int buffer_R = 0, buffer_G = 0, buffer_B = 0;

    // iterating the image pixel-by-pixel
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // calculating the new values from old ones
            buffer_R = round(0.393 * image[j][i].rgbtRed + 0.769 * image[j][i].rgbtGreen + 0.189 * image[j][i].rgbtBlue);
            buffer_G = round(0.349 * image[j][i].rgbtRed + 0.686 * image[j][i].rgbtGreen + 0.168 * image[j][i].rgbtBlue);
            buffer_B = round(0.272 * image[j][i].rgbtRed + 0.534 * image[j][i].rgbtGreen + 0.131 * image[j][i].rgbtBlue);

            // replacing the values with new ones
            image[j][i].rgbtRed   = (buffer_R < 255) ? buffer_R : 255;
            image[j][i].rgbtGreen = (buffer_G < 255) ? buffer_G : 255;
            image[j][i].rgbtBlue  = (buffer_B < 255) ? buffer_B : 255;
        }
    }

    return;
}

// function to swap pixel values
void swap(RGBTRIPLE *L, RGBTRIPLE *R)
{
    RGBTRIPLE temp = *L;
    *L = *R;
    *R = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //printf("**** height,width = %i, %i ******",height,width);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width / 2; i++)
        {
            // swapping using the function we have written
            swap(&image[j][i], &image[j][width - i - 1]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // creating a temp 2d matrix to hold the values of the Blur image
    RGBTRIPLE tmp[height][width];

    // initalizing the buffer variable for further use
    int Red = 0, Green = 0, Blue = 0;
    float count = 0.0;

    // iteration pixel-by-pixel to calulate the new values
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Blue = Green = Red = 0;
            count = 0.0;

            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > height - 1)
                {
                    continue;
                }

                for (int h = -1; h < 2; h++)
                {
                    if (i + h < 0 || i + h > width - 1)
                    {
                        continue;
                    }

                    Red += image[j + k][i + h].rgbtRed;
                    Green += image[j + k][i + h].rgbtGreen;
                    Blue += image[j + k][i + h].rgbtBlue;

                    count++;
                }
            }

            // averages the sum to make picture look blurrier
            tmp[j][i].rgbtRed = round(Red / count);
            tmp[j][i].rgbtGreen = round(Green / count);
            tmp[j][i].rgbtBlue = round(Blue / count);
        }
    }

    // replacing with calculated ones pixel-by-pixel
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[j][i].rgbtBlue = tmp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = tmp[j][i].rgbtGreen;
            image[j][i].rgbtRed = tmp[j][i].rgbtRed;
        }
    }

    return;
}
