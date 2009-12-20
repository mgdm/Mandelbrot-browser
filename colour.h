#ifndef MANDELBROT_COLOUR_H
#define MANDELBROT_COLOUR_H
#include "mandelbrot.h"

void colour_basic(pixel *dest, long x, long y, long xpx, long ypx, long m, 
        double min_r, double min_i, double max_r, double max_i);
void colour_bands(pixel *dest, long x, long y, long xpx, long ypx, long m,
        double min_r, double min_i, double max_r, double max_i);

#endif
