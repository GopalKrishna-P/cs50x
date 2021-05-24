#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // validate the args
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // check if the file is valid or not
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file %s.\n", argv[1]);
        return 1;
    }

    // initiate variable for further use
    FILE *img;
    // limiting the file name to be of 8 chars
    char filename[8];
    unsigned char *buffer = malloc(512);
    int counter = 0;

    while (fread(buffer, 512, 1, file))
    {
        // finding new image files
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // close prev open file
            if (counter > 0)
            {
                fclose(img);
            }

            // creating new image filename
            sprintf(filename, "%03d.jpg", counter);

            // opening the new image file
            img = fopen(filename, "w");

            // validating the creation of img
            if (img == NULL)
            {
                fclose(file);
                free(buffer);
                fprintf(stderr, "Failed to create output JPG %s", filename);
                return 3;
            }

            counter++;
        }

        //write to the file
        if (counter)
        {
            fwrite(buffer, 512, 1, img);
        }
    }

    // deallocate the memory and terminate the execution
    fclose(img);
    fclose(file);
    free(buffer);
    return 0;
}