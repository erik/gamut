#include <stdlib.h>

#include "farbfeld.h"


int main(int argc, char** argv)
{
    uint32_t width, height;
    uint16_t opacity;

    if (argc != 2) {
        fprintf(stderr, "usage: %s OPACITY_PCT\n", argv[0]);
        return 1;
    }

    long opacity_pct = strtol(argv[1], NULL, 10);

    if (opacity_pct > 100 || opacity_pct < 0) {
        fprintf(stderr, "opacity out of range: %ld%%\n", opacity_pct);
        return 1;
    }

    opacity = (uint16_t)(65535 * (opacity_pct / 100.0));

    if (read_header(&width, &height)) FATALF();

    if (write_header(width, height)) {
        perror("write header");
        return 1;
    }

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            pixel_t rgba;

            if (read_pixel(&rgba))
                FATALF();

            rgba[3] = opacity;

            if (write_pixel(rgba))
                FATALF();
        }
    }

    return 0;
}
