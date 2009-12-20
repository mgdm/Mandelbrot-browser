#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include "mandelbrot.h"

int write_png_output(pixel *buf, long xpx, long ypx, const char *filename) {
    int y;
    pixel *buf_row = buf;
    png_structp png_ptr;
    png_infop info_ptr;

    png_byte **row_pointers = malloc(ypx * sizeof(png_byte *));
    for(y = 0; y < ypx; y++) {
        row_pointers[y] = (png_byte *)buf_row;
        buf_row += xpx;
    }

    if(filename == NULL) {
        filename = "mb.png";
    }

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL) return -1;

    /* Initialize the write struct. */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        fclose(fp);
        return -1;
    }

    
    /* Initialize the info struct. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return -1;
    }

    /* Set up error handling. */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return -1;
    }

    png_init_io(png_ptr, fp);

    /* Set image attributes. */
    png_set_IHDR(png_ptr,
                 info_ptr,
                 xpx,
                 ypx,
                 8,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, info_ptr);
    fclose(fp);
    free(row_pointers);
}
