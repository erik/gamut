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
    uint32_t width = 0, height = 0;
    pixel_t* image = {0};
    bool flip_v = false, flip_h = false;

    if (argc < 2) {
        fprintf(stderr, USAGE, argv[0]);
        return 1;
    }

    ARGBEGIN {
    case 'v':
        flip_v = true;
        break;
    case 'h':
        flip_h = true;
        break;
    default:
        fprintf(stderr, USAGE, argv[0]);
        return 1;
        ARGEND;
    }

    if (!flip_v && !flip_h) {
        fprintf(stderr, "need to specify a flip direction\n");
        return 1;
    }

    if (read_header(&width, &height))
        FATALF();

    if (!(image = calloc(width * height, sizeof(pixel_t)))) {
        perror("couldn't allocate memory");
        return 1;
    }

    if (read_image(image, width, height))
        FATALF();

    if (write_header(width, height)) {
        perror("write header");
        return 1;
    }

    if (flip_h) {
        for (uint32_t i = 0; i < height; i++) {
            uint32_t row = i * width;

            for (uint32_t j = 0; j < width / 2; j++) {
                uint32_t swap = (width - 1) - j;

                pixel_t temp;

                memcpy(&temp, &image[row + j], sizeof(pixel_t));
                memcpy(&image[row + j], &image[row + swap], sizeof(pixel_t));
                memcpy(&image[row + swap], &temp, sizeof(pixel_t));
            }
        }
    }

    if (flip_v) {
        for (uint32_t i = 0; i < height / 2; i++) {
            uint32_t swap = (height - 1) - i;
            for (uint32_t j = 0; j < width; j++) {
                pixel_t temp = {0};

                memcpy(&temp, &image[swap * width + j], sizeof(pixel_t));
                memcpy(&image[swap * width + j], &image[i * width + j], sizeof(pixel_t));
                memcpy(&image[i * width + j], &temp, sizeof(pixel_t));
            }
        }
    }

    if (write_image(image, width, height))
        FATALF();

    return 0;
}
