#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // initialize variable
    int height = 0;

    // prompt to get the input
    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);

    // loop to traverse pixel by pixel and display
    for (int i = 1; i <= height; i++)
    {
        // right-handed triangle
        for (int j = 1; j <= height; j++)
        {
            // conditional to check which char to print
            if (i + j > height)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        // spaces in between
        printf("  ");
        // left-handed triangle
        for (int j = 1; j <= height; j++)
        {
            // conditional to check which char to print
            if (i - j >= 0)
            {
                printf("#");
            }
        }
        // next line
        printf("\n");
    }
}