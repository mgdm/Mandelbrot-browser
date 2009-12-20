#define MAXITERS 10240
#define COLOURS 256
#define NUMTHREADS 2

#include "mandelbrot.h"

void *thread_mandel(void *closure);
void run_threads(pixel *mandelset, double xmin, double xmax, double ymin, double ymax, double xpx, double ypx);
