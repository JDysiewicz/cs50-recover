#include <stdio.h>
#include <stdlib.h>

// define unsigned char - used for buffer
typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage: ./recover image\n");
        return 1;
    }

    // setup raw_file in read mode
    FILE *raw_file = fopen(argv[1], "r");

    if (raw_file == NULL)
    {
        fprintf(stderr, "Could not open image.\n");
        return 2;
    }

    // setup buffer of unsigned char array of 512 bytes long
    BYTE buffer[512];

    // setup image file to write to
    FILE *image;

    // setup counter
    int counter = 0;

    // setup array for image file - xxx.jpg\0
    char image_name[8];

    // now being going through the read file in 512 byte chunks - will return 1 if 1 512 chunk found, therefore successful

    while (fread(buffer, 512, 1, raw_file) == 1)
    {
        // if jpeg header found, write a new file and write to it
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // close previous file if already one open
            if (counter > 0)
            {
                fclose(image);
            }
            // write filename with sprintf, increase counter by 1, write buffer to an output file
            sprintf(image_name, "%03i.jpg", counter);
            counter += 1;
            image = fopen(image_name, "w");
            fwrite(buffer, 512, 1, image);

        }
        // if no jpeg header found, just write the file
        else
        {
            fwrite(buffer, 512, 1, image);
        }

    }
    // success
    return 0;
}