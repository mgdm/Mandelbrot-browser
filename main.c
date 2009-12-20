#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "main.h"
#include "mandelbrot.h"
#include "colour.h"
#include "io.h"


void *thread_mandel(void *closure) {
    thread_closure *tc = (thread_closure *) closure;
    
    printf("Entering continuous_mandel in thread %ld\n", tc->offset);
    mandel_basic(tc->dest, 
            tc->xmin, tc->xmax, tc->ymin, tc->ymax, 
            tc->xpx, tc->ypx, tc->step, tc->offset, tc->colour_func );
    printf("Exiting thread %ld\n", tc->offset);
    pthread_exit(closure);
}

void run_threads(pixel *mandelset, double xmin, double xmax, double ymin, double ymax, double xpx, double ypx) {

    thread_closure thread_info[NUMTHREADS];
    pthread_t threads[NUMTHREADS];
    pthread_attr_t attr;
    int t;
    long rc;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    for(t=0; t < NUMTHREADS; t++) {
        thread_info[t].dest = mandelset;
        thread_info[t].xmin = xmin;
        thread_info[t].xmax = xmax;
        thread_info[t].ymin = ymin;
        thread_info[t].ymax = ymax;
        thread_info[t].xpx = xpx;
        thread_info[t].ypx = ypx;
        thread_info[t].offset = t;
        thread_info[t].step = NUMTHREADS;
        thread_info[t].colour_func = (colour_function)&colour_basic;

        rc = pthread_create(&threads[t], NULL, thread_mandel, (void *) &thread_info[t]);
        if(rc) {
            printf("Creating thread failed with error %ld\n", rc);
        }
    }

    pthread_attr_destroy(&attr);
    for(t = 0; t < NUMTHREADS; t++) {
        rc = pthread_join(threads[t], &status);
        if(rc) {
            printf("ERROR; return code from pthread_join() is %ld\n", rc);
            exit(-1);
        }
        printf("Main: completed join with thread %d\n", t, (long)status);
    }
}

