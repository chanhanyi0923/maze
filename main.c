#include <stdlib.h>
#include <gtk/gtk.h>
#include "gui.h"

/* Surface to store current scribbles */
cairo_surface_t *surface = NULL;
struct Graph *graph;

int
main(int    argc,
     char **argv)
{
    srand(time(NULL));

    GtkApplication *app;
    int status;

    app = gtk_application_new("gtk.maze.animation", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
