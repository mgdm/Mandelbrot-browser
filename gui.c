#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "main.h"
#include "mandelbrot.h"

pixel *buffer = NULL;
double xmin = -2.0, xmax = 2.0;
double ymin = -2.0, ymax = 2.0;
long   xpx = 512, ypx = 512;

void close_app(GtkObject *object, gpointer user_data) {
    gtk_main_quit();
}

void trigger_render(GtkObject *object, gpointer user_data) {
    printf("Triggered rendering.\n");
}

int main(int argc, char *argv[]) {

    GtkBuilder* gtk_builder;
    GtkWidget *main_window, *box;
    GdkPixbuf *pixbuf;
    GtkImage *drawing_area;
    
    gtk_init(&argc, &argv);

    gtk_builder = gtk_builder_new();
    gtk_builder_add_from_file(gtk_builder, "main.glade", NULL);
    gtk_builder_connect_signals(gtk_builder, NULL);
    main_window = GTK_WIDGET(gtk_builder_get_object(gtk_builder, "main_window"));
    
    if(buffer == NULL) {
        buffer = malloc(xpx * ypx * sizeof(pixel));
    }
    
    run_threads(buffer, xmin, xmax, ymin, ymax, xpx, ypx);

    box = GTK_WIDGET(gtk_builder_get_object(gtk_builder, "event_box"));
    pixbuf = gdk_pixbuf_new_from_data(
            (const guchar *)buffer, GDK_COLORSPACE_RGB,
            0, 8, xpx, ypx, xpx * sizeof(pixel), NULL, NULL);

    drawing_area = (GtkImage *)gtk_image_new_from_pixbuf(pixbuf);
    gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(drawing_area));

    gtk_widget_show_all(main_window);    

    gtk_main();
    return 0;
}

