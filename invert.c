/* Adapted from http://tools.suckless.org/farbfeld/invert.c */

#include "farbfeld.h"


int main(int argc, char** argv)
{
    uint32_t width, height;
    pixel_t rgba;

    if (argc != 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        return 1;
    }

    read_header(&width, &height);
    write_header(width, height);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            read_pixel(&rgba);

            /* invert colors */
            rgba.r = 65535 - rgba.r;
            rgba.g = 65535 - rgba.g;
            rgba.b = 65535 - rgba.b;

            write_pixel(&rgba);
        }
    }

    return 0;
}
