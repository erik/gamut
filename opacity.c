#include <stdlib.h>

#include "farbfeld.h"


int main(int argc, char** argv)
{
    uint32_t width, height;
    uint16_t opacity;
    pixel_t rgba;

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

    read_header(&width, &height);
    write_header(width, height);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            read_pixel(&rgba);
            rgba.a = opacity;
            write_pixel(&rgba);
        }
    }

    return 0;
}
