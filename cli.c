#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "mandelbrot.h"

int main(int argc, char *argv[]) {

    double xmin, xmax, ymin, ymax;
    long   xpx, ypx, arraysize;
    char *tail;

    /* default: -2.0, 2.0, -2.0, 2.0, 1024, 1024 */
    if(argc < 6) {
        xmin = -2.0;
        xmax = 2.0;
        ymin = -2.0;
        ymax = 2.0;
        xpx = 2048;
        ypx = 2048;
    } else {
        xmin =  strtod( argv[1], &tail );
        xmax =  strtod( argv[2], &tail );
        ymin =  strtod( argv[3], &tail );
        ymax =  strtod( argv[4], &tail );
        xpx  =  strtol( argv[5], &tail, 0 );
        ypx  =  strtol( argv[6], &tail, 0 );
    }

    printf( "Xmin: %f, Xmax: %f, Ymin: %f, Ymax: %f, Xpx: %ld, Ypx: %ld\n", xmin, xmax, ymin, ymax, xpx, ypx );

    arraysize = xpx * ypx;
    pixel *mandelset = malloc(sizeof(pixel) * arraysize);
    printf("Allocated buffer of size %ld\n", sizeof(pixel) * arraysize);

    run_threads(mandelset, xmin, xmax, ymin, ymax, xpx, ypx);

    write_png_output(mandelset, xpx, ypx, NULL);
}

