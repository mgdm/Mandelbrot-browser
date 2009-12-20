CFLAGS=-g
GTK_CFLAGS=

cli:
	gcc $(CFLAGS) -o mandel colour.c  io.c  main.c cli.c mandelbrot.c -lpng -lpthread
gui:
	gcc $(CFLAGS) -export-dynamic `pkg-config --libs --cflags gtk+-2.0 gmodule-export-2.0` -o gui_mandel colour.c  io.c  main.c gui.c mandelbrot.c -lpng -lpthread  
