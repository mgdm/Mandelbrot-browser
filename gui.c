#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "main.h"
#include "mandelbrot.h"

pixel *buffer = NULL;
double xmin = -2.0, xmax = 2.0;
double ymin = -2.0, ymax = 2.0;
double cached_xmin, cached_xmax, cached_ymin, cached_ymax;
double dx, dy;

long   xpx = 512, ypx = 512;

GtkStatusbar *status_bar;
GtkImage *drawing_area;
GtkSpinButton *x_spin, *y_spin, *width_spin;

void close_app(GtkObject *object, gpointer user_data) {
    gtk_main_quit();
}

void trigger_render(GtkObject *object, gpointer user_data) {
    double width;
    
    printf("Triggered rendering.\n");

    run_threads(buffer, xmin, xmax, ymin, ymax, xpx, ypx);

    cached_xmin = xmin;
    cached_xmax = xmax;
    cached_ymin = ymin;
    cached_ymax = ymax; 
    dx = (xmax - xmin) / (double) xpx;
    dy = (ymax - ymin) / (double) ypx;
    width = xmax - xmin;
    
    gtk_widget_queue_draw(GTK_WIDGET(drawing_area));
    gtk_spin_button_set_value(x_spin, xmin + (width / 2));
    gtk_spin_button_set_value(y_spin, ymin + (width / 2));
    gtk_spin_button_set_value(width_spin, width);
    printf("Rendered: centre (%.3f, %.3f), width %.3f, %ldx%ldpx\n",
            xmin + (width / 2),
            ymin + (width / 2),
            width,
            xpx, ypx);
}

static void update_status(long xpos, long ypos) {
    char description[25];
    double x, y;

    x = cached_xmin + (xpos * dx);
    y = cached_ymax - (ypos * dy);
    
    snprintf(description, 25, "(%.3f, %.3f)", x, y);
    gtk_statusbar_push(status_bar, gtk_statusbar_get_context_id(status_bar, "information"), description);
}

static void recentre(long xpos, long ypos) {
    double x, y;
    double xsize = xmax - xmin;
    double ysize = ymax - ymin;
    char description[30];

    printf("dx: %f, dy: %f\n", dx, dy);
    printf("Xpos: %ld; Ypos: %ld\n", xpos, ypos);
    x = cached_xmin + ((double) xpos * dx);
    y = cached_ymax - ((double) ypos * dy);
    printf("Clicked on (%.3f, %.3f)\n", x, y);    

    xmin = x - (xsize / 2);
    xmax = x + (xsize / 2);
    ymin = y - (ysize / 2);
    ymax = y + (ysize / 2);

    snprintf(description, 35, "Recentred to (%.3f, %.3f)", x, y);
    gtk_statusbar_push(status_bar, gtk_statusbar_get_context_id(status_bar, "information"), description);
}

gboolean click_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    long xpos, ypos;

    if (event->motion.is_hint) {
        int x, y;
        gdk_window_get_pointer (widget->window, &x, &y, NULL);
        event->motion.x = x;
        event->motion.y = y;
    }

    xpos = event->motion.x;
    ypos = event->motion.y;
    recentre(xpos, ypos);
    return TRUE;

}

gboolean x_spin_changed(GtkWidget *widget, GdkEvent *event, gpointer data) {
    double new_x, width;
    
    new_x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    width = xmax - xmin;

    xmin = new_x - (width / 2);
    xmax = new_x + (width / 2);
    
    return TRUE;
}

gboolean y_spin_changed(GtkWidget *widget, GdkEvent *event, gpointer data) {
    double new_y, height;
    
    new_y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    height = ymax - ymin;

    ymin = new_y - (height / 2);
    ymax = new_y + (height / 2);
    
    return TRUE;
}

gboolean width_spin_changed(GtkWidget *widget, GdkEvent *event, gpointer data) {
    double x, y, new_width;
    
    new_width = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));
    x = xmin + (xmax - xmin) / 2; 
    y = ymin + (ymax - ymin) / 2; 

    xmin = x - (new_width / 2);
    xmax = x + (new_width / 2);
    ymin = y - (new_width / 2);
    ymax = y + (new_width / 2);
    
    return TRUE;
}

gboolean motion_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    long xpos, ypos;

    if (event->motion.is_hint) {
        int x, y;
        gdk_window_get_pointer (widget->window, &x, &y, NULL);
        event->motion.x = x;
        event->motion.y = y;
    }

    xpos = event->motion.x;
    ypos = event->motion.y;
    update_status(xpos, ypos);
    return TRUE;
}

int main(int argc, char *argv[]) {

    GtkBuilder* gtk_builder;
    GtkWidget *main_window, *box;
    GdkPixbuf *pixbuf;
    double width;

    dx = (xmax - xmin) / (double) xpx;
    dy = (ymax - ymin) / (double) ypx;
    cached_xmin = xmin;
    cached_xmax = xmax;
    cached_ymin = ymin;
    cached_ymax = ymax;
    
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

    width = xmax - xmin;
    printf("Width: %f\n", width);
    width_spin = GTK_SPIN_BUTTON(gtk_builder_get_object(gtk_builder, "width_spin"));
    gtk_spin_button_set_value(width_spin, width);

    x_spin = GTK_SPIN_BUTTON(gtk_builder_get_object(gtk_builder, "centre_x_spin"));
    gtk_spin_button_set_value(x_spin, xmin + width / 2);

    y_spin = GTK_SPIN_BUTTON(gtk_builder_get_object(gtk_builder, "centre_y_spin"));
    gtk_spin_button_set_value(y_spin, ymin + width / 2);


    pixbuf = gdk_pixbuf_new_from_data(
            (const guchar *)buffer, GDK_COLORSPACE_RGB,
            0, 8, xpx, ypx, xpx * sizeof(pixel), NULL, NULL);

    drawing_area = (GtkImage *)gtk_image_new_from_pixbuf(pixbuf);
    gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(drawing_area));

    gtk_widget_show_all(main_window); 

    gtk_main();
    return 0;
}

