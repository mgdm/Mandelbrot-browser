#ifndef MANDELBROT_H
#define MANDELBROT_H

#define MAXITERS 10240
#define COLOURS 256

typedef struct _complex {
    double real;
    double imaginary;
} complex;

typedef struct _pixel {
    unsigned char r, g, b;
} pixel;

typedef void (*colour_function)(pixel *, long, long, long, long, long, long, long, long, long);

typedef struct _thread_closure {
    pixel *dest;
    double xmin, xmax, ymin, ymax;
    long xpx, ypx;
    long step;  /* How many lines to skip */
    long offset; /* The offset to start from */
    colour_function colour_func;
} thread_closure;

#endif
