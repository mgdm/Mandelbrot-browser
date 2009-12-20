#include <stdio.h>
#include "mandelbrot.h"

void colour_basic(pixel *dest, 
        long x, long y, long xpx, long ypx, long m,
        double min_r, double min_i, double max_r, double max_i) {
    if(m == MAXITERS) {
        m = 0;
    } else if(m > 255) {
        m = 255;
    }

    dest[y * xpx + x].r = 0;
    dest[y * xpx + x].g = 0;
    dest[y * xpx + x].b = m;
}

void colour_bands(pixel *dest, 
        long x, long y, long xpx, long ypx, long m,
        double min_r, double min_i, double max_r, double max_i) {

    printf("min_r: %f, max_r: %f, min_i: %f, max_i: %f\n", min_r, max_r, min_i, max_i);
    
    if(m == MAXITERS) {
        m = 0;
    } else if(m > 255) {
        m = 255;
    }

    dest[y * xpx + x].r = 0;
    dest[y * xpx + x].g = 0;
    dest[y * xpx + x].b = m;

}   
    
