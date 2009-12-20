#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "main.h"
#include "mandelbrot.h"

pixel *buffer = NULL;
double xmin = -2.0, xmax = 2.0;
double ymin = 0.0, ymax = 4.0;
double dx, dy;

long   xpx = 512, ypx = 512;

GtkStatusbar *status_bar;

void close_app(GtkObject *object, gpointer user_data) {
    gtk_main_quit();
}

void trigger_render(GtkObject *object, gpointer user_data) {
    printf("Triggered rendering.\n");

    run_threads(buffer, xmin, xmax, ymin, ymax, xpx, ypx);
}

static void update_status(long xpos, long ypos) {
    char description[25];
    double x, y;

    printf("dx: %f, dy: %f\n", dx, dy);
    printf("%ld, %ld\n", xpos, ypos);
    x = xmin + (xpos * dx);
    y = ymax - (ypos * dy);
    
    snprintf(description, 25, "(%.3f, %.3f)", x, y);
    gtk_statusbar_push(status_bar, gtk_statusbar_get_context_id(status_bar, "information"), description);
}

gboolean motion_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gboolean handled = FALSE;
    long xpos, ypos;

    switch(event->type) {
        case GDK_MOTION_NOTIFY:
        /*    if (event->motion.is_hint)
            {
                int x, y;

                gdk_window_get_pointer (widget->window, &x, &y, NULL);
                event->motion.x = x;
                event->motion.y = y;
            } */

            xpos = event->motion.x;
            ypos = event->motion.y;
            update_status(xpos, ypos);
            break;            
    }
}

int main(int argc, char *argv[]) {

    GtkBuilder* gtk_builder;
    GtkWidget *main_window, *box;
    GdkPixbuf *pixbuf;
    GtkImage *drawing_area;

    dx = (xmax - xmin) / (double) xpx;
    dy = (ymax - ymin) / (double) ypx;
    
    gtk_init(&argc, &argv);

    gtk_builder = gtk_builder_new();
    gtk_builder_add_from_file(gtk_builder, "main.glade", NULL);
    gtk_builder_connect_signals(gtk_builder, NULL);
    main_window = GTK_WIDGET(gtk_builder_get_object(gtk_builder, "main_window"));
    
    if(buffer == NULL) {
        buffer = malloc(xpx * ypx * sizeof(pixel));
    }
    
    run_threads(buffer, xmin, xmax, ymin, ymax, xpx, ypx);

    status_bar = GTK_STATUSBAR(gtk_builder_get_object(gtk_builder, "status_bar"));
    gtk_statusbar_push(status_bar, gtk_statusbar_get_context_id(status_bar, "information"), "");

    box = GTK_WIDGET(gtk_builder_get_object(gtk_builder, "event_box"));
    gtk_widget_set_events(box, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK );

    pixbuf = gdk_pixbuf_new_from_data(
            (const guchar *)buffer, GDK_COLORSPACE_RGB,
            0, 8, xpx, ypx, xpx * sizeof(pixel), NULL, NULL);

    drawing_area = (GtkImage *)gtk_image_new_from_pixbuf(pixbuf);
    gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(drawing_area));

    gtk_widget_show_all(main_window); 

    gtk_main();
    return 0;
}

