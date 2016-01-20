/* Utility functions, adapted from farbfeld's ff2png.c and png2ff.c */

#pragma once

#include <arpa/inet.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FF_HEADER "farbfeld########"

#define PIXEL(img, x, y) img[(y) * width + (x)]

typedef struct { uint16_t r, g, b, a; } pixel_t;


static void ntoh_pixel(pixel_t* dst, pixel_t* src)
{
    dst->r = ntohs(src->r);
    dst->g = ntohs(src->g);
    dst->b = ntohs(src->b);
    dst->a = ntohs(src->a);
}


static void hton_pixel(pixel_t* dst, pixel_t* src)
{
    dst->r = htons(src->r);
    dst->g = htons(src->g);
    dst->b = htons(src->b);
    dst->a = htons(src->a);
}


static void read_header(uint32_t* width, uint32_t* height)
{
    uint8_t hdr[16] = {0};

    if (fread(hdr, 1, strlen(FF_HEADER), stdin) != strlen(FF_HEADER)) {
        fprintf(stderr, "incomplete header\n");
        exit(1);
    }

    if (memcmp("farbfeld", hdr, strlen("farbfeld"))) {
        fprintf(stderr, "invalid magic value\n");
        exit(1);
    }

    *width = ntohl(*((uint32_t *)(hdr + 8)));
    *height = ntohl(*((uint32_t *)(hdr + 12)));
}


static void write_header(uint32_t width, uint32_t height)
{
    uint32_t tmp;

    fputs("farbfeld", stdout);

    tmp = htonl(width);
    if (fwrite(&tmp, sizeof(uint32_t), 1, stdout) != 1)
        exit(1);

    tmp = htonl(height);
    if (fwrite(&tmp, sizeof(uint32_t), 1, stdout) != 1)
        exit(1);
}


static inline void read_pixel(pixel_t* rgba)
{
    if (fread(rgba, sizeof(uint16_t), 4, stdin) != 4) {
        fprintf(stderr, "unexpected EOF\n");
        exit(1);
    }

    ntoh_pixel(rgba, rgba);
}


static inline void write_pixel(pixel_t* rgba)
{
    pixel_t copy;

    hton_pixel(&copy, rgba);

    if (fwrite(&copy, sizeof(uint16_t), 4, stdout) != 4) {
        fprintf(stderr, "write error\n");
        exit(1);
    }
}


/* Slurp entire image into memory. */
static inline void read_image(pixel_t* pixels, uint32_t width, uint32_t height)
{
    uint32_t size = width * height;

    if (fread(pixels, sizeof(uint16_t) * 4, size, stdin) != size) {
        fprintf(stderr, "unexpected EOF\n");
        exit(1);
    }

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            hton_pixel(&pixels[i * width + j], &pixels[i * width + j]);
        }
    }
}


static inline void write_image(pixel_t* pixels, uint32_t width, uint32_t height)
{
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            if (write_pixel(&pixels[i * width + j])) return 1;
        }
    }

    if (fwrite(copy, sizeof(pixel_t), width * height, stdout) != width * height) {
        fprintf(stderr, "write failed\n");
        exit(1);
    }

    free(copy);
}
