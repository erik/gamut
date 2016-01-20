#include <stdlib.h>

#include "farbfeld.h"


const float SHARPEN_KERNEL[3][3] = {
    { 0, -1,  0},
    {-1,  5, -1},
    { 0, -1,  0},
};


const float BLUR_KERNEL[3][3] = {
    { 0.0625, 0.125, 0.0625 },
    { 0.1250, 0.250, 0.1250 },
    { 0.0625, 0.125, 0.0625 },
};


int convolve(const float kernel[3][3])
{
    uint32_t width, height;
    pixel_t* image = NULL;
    char** argv = "i didn't plan this out.";

    read_header(&width, &height);

    if (!(image = calloc(width * height, sizeof(pixel_t)))) {
        perror("couldn't allocate memory");
        return 1;
    }

    read_image(image, width, height);
    write_header(width, height);

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            pixel_t accumulator = {0, 0, 0, 0};

            for (int ky = -1; ky < 2; ++ky) {
                for (int kx = -1; kx < 2; ++kx) {
                    pixel_t* current = NULL;
                    float mult = kernel[kx + 1][ky + 1];
                    int idx = x + kx,
                        idy = y + ky;

                    if (idx < 0 || (uint32_t) idx >= width) idx = x;
                    if (idy < 0 || (uint32_t) idy >= height) idy = y;

                    current = PIXEL(&image, idx, idy);

                    accumulator.r += current->r * mult;
                    accumulator.g += current->g * mult;
                    accumulator.b += current->b * mult;
                    accumulator.a += current->a * mult;
                }
            }

            write_pixel(&accumulator);
        }
    }

    return 0;
}


int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s KERNEL\n", argv[0]);
        return 1;
    }

    const void *kernel = NULL;

    if (!strcmp("blur", argv[1]))
        kernel = &BLUR_KERNEL;
    else if (!strcmp("sharpen", argv[1]))
        kernel = &SHARPEN_KERNEL;
    else {
        fprintf(stderr, "unknown image kernel: %s\n", argv[1]);
        return 1;
    }

    return convolve(kernel);
}
