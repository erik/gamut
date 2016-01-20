#include <stdlib.h>
#include <stdbool.h>

#include "arg.h"
#include "farbfeld.h"


#define USAGE                                   \
    "usage: %s [-v] [-h]\n\n"                   \
    "  -v      flip vertically\n"               \
    "  -h      flip horizontally"


int main(int argc, char** argv)
{
    uint32_t width = 0,
             height = 0;
    bool flip_v = false,
         flip_h = false;
    pixel_t* image = NULL, temp;

    if (argc < 2) {
        fprintf(stderr, USAGE, argv[0]);
        return 1;
    }

    ARGBEGIN {
    case 'v':
        flip_v = true; break;
    case 'h':
        flip_h = true; break;
    default:
        fprintf(stderr, USAGE, argv[0]);
        return 1;
        ARGEND;
    }

    if (!flip_v && !flip_h) {
        fprintf(stderr, "need to specify a flip direction\n");
        return 1;
    }

    read_header(&width, &height);

    if (!(image = calloc(width * height, sizeof(pixel_t)))) {
        perror("couldn't allocate memory");
        return 1;
    }

    read_image(image, width, height);
    write_header(width, height);

    if (flip_h) {
        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width / 2; x++) {
                uint32_t swap = (width - 1) - x;

                memcpy(&temp, PIXEL(&image, x, y), sizeof(pixel_t));
                memcpy(PIXEL(&image, x, y), PIXEL(&image, swap, y), sizeof(pixel_t));
                memcpy(PIXEL(&image, swap, y), &temp, sizeof(pixel_t));
            }
        }
    }

    if (flip_v) {
        for (uint32_t y = 0; y < height / 2; y++) {
            uint32_t swap = (height - 1) - y;
            for (uint32_t x = 0; x < width; x++) {
                memcpy(&temp, PIXEL(&image, x, y), sizeof(pixel_t));
                memcpy(PIXEL(&image, x, y), PIXEL(&image, x, swap), sizeof(pixel_t));
                memcpy(PIXEL(&image, x, swap), &temp, sizeof(pixel_t));
            }
        }
    }

    write_image(image, width, height);

    return 0;
}
