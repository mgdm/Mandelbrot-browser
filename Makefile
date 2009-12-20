CFLAGS=-O2
GTK_CFLAGS=

cli:
	gcc $(CFLAGS) -o mandel colour.c  io.c  main.c cli.c mandelbrot.c -lpng -lpthread
gui:
	gcc $(CFLAGS) `pkg-config --cflags gtk+-2.0` -o mandel colour.c  io.c  main.c gui.c mandelbrot.c -lpng -lpthread -lgtk
