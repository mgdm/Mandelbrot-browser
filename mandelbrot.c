#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mandelbrot.h"
#include "colour.h"

void mandel_basic(pixel *dest, 
        double xmin, double xmax, double ymin, double ymax, 
        long xpx, long ypx, long step, long offset, 
        colour_function colour_func) {    

    long ni, ix, iy, m;
    double min_r = 100000, min_i = 100000, max_r = -100000, max_i = -100000;
    complex c;
    complex original;
    double dx, dy, tmp;

    dx   =  (xmax - xmin) / (double) xpx;
    dy   =  (ymax - ymin) / (double) ypx;    

    for ( iy = 0; iy < ypx; iy++ ) {
        for ( ix = offset; ix < xpx; ix += step ) {
            c.real      = xmin + ix * dx;
            c.imaginary = ymin + iy * dy;
            original.real = c.real;
            original.imaginary = c.imaginary;
            ni = 0;
            while ( ni < MAXITERS && (c.real * c.real * c.imaginary * c.imaginary < 4.0 ) ) {
                tmp = c.real * c.real - c.imaginary * c.imaginary + original.real;
                c.imaginary = c.imaginary * c.real * 2 + original.imaginary;
                c.real = tmp;
            
                if(c.real < min_r) min_r = c.real;
                if(c.imaginary < min_i) min_i = c.imaginary;
                if(c.real > max_r) max_r = c.real;
                if(c.imaginary > max_i) max_i = c.imaginary;
                
                ni++;
            }
            m = ni - log( log( sqrt( c.real * c.real * c.imaginary * c.imaginary ) ) ) / log( 2 );
            colour_func(dest, ix, iy, xpx, ypx, m, min_r, min_i, max_r, max_i);
        }
    }
}

