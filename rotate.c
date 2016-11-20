#include <stdlib.h>
#include <math.h>

#include "farbfeld.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIGN(n) (n > 0) - (n < 0)

pixel_t sampleat(pixel_t *image, int x, int y, int width, int height)
{
    // clamp to edges
    int clamp = 1;
    if (x < 0) x = 0, clamp = 0;
    else if (x >= width) x = width - 1, clamp = 0;
    if (y < 0) y = 0, clamp = 0;
    else if (y >= height) y = height - 1, clamp = 0;

    pixel_t pixel = PIXEL(image, x, y);
    pixel.a *= clamp;
    return pixel;
}

pixel_t interpolate(pixel_t a, pixel_t b, double t)
{
    double s = 1.0 - t;
    return (pixel_t){
        a.r * s + b.r * t,
        a.g * s + b.g * t,
        a.b * s + b.b * t,
        a.a * s + b.a * t
    };
}

int rotate(double angle)
{
    uint32_t inw, inh;
    pixel_t* image = NULL;

    read_header(&inw, &inh);

    if (!(image = calloc(inw * inh, sizeof(pixel_t)))) {
        perror("couldn't allocate memory");
        return 1;
    }

    read_image(image, inw, inh);

    const double rad = angle / 180.0 * M_PI;

    // calculate output dimensions
    int outw, outh;
    {
        const double S = sin(rad);
        const double C = cos(rad);
        double a = inw / 2 * C;
        double b = inh / 2 * S;
        outw = ceil(MAX(MAX(-a + b, a + b), MAX(-a - b, a - b)) * 2.0);
        double c = inh / 2 * C;
        double d = inw / 2 * S;
        outh = ceil(MAX(MAX(-c + d, c + d), MAX(-c - d, c - d)) * 2.0);
    }

    write_header(outw, outh);

    const double S = sin(-rad);
    const double C = cos(-rad);
    for (int outy = 0; outy < outh; outy++) {
        for (int outx = 0; outx < outw; outx++) {
            double cox = outx - outw / 2;
            double coy = outy - outh / 2;
            // rotate back to find input image coordinates
            double inx = cox * C - coy * S + inw / 2;
            double iny = coy * C + cox * S + inh / 2;
            // separate base coordinates and interpolation factors
            double basex, basey;
            double tx = modf(inx, &basex);
            double ty = modf(iny, &basey);
            int signx = SIGN(tx), signy = SIGN(ty);
            // bilinear interpolation
            pixel_t pixel = interpolate(
                interpolate(
                    sampleat(image, basex, basey, inw, inh),
                    sampleat(image, basex + signx, basey, inw, inh),
                    fabs(tx)),
                interpolate(
                    sampleat(image, basex, basey + signy, inw, inh),
                    sampleat(image, basex + signx, basey + signy, inw, inh),
                    fabs(tx)),
                fabs(ty));
            write_pixel(&pixel);
        }
   }
   return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s ANGLE\n", argv[0]);
        return 1;
    }

    double angle = atof(argv[1]);

    return rotate(angle);
}
