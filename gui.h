#ifndef GUI
#define GUI

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>

#include "pointer_contianer.h"
#include "graph.h"
#include "draw.h"

extern cairo_surface_t *surface;
extern struct Graph *graph;

gboolean
configureEvent(GtkWidget *widget,
                 gpointer data);

gboolean
drawMaze(GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data);

gboolean
drawPathTimer(gpointer data);

void
closeWindowMaze();

void
closeWindowInput(GtkWidget *window_input,
                 gpointer   data);

gboolean
buttonClickedCallback(GtkWidget *button,
                      gpointer   data);

void
activate(GtkApplication *app,
         gpointer        user_data);

#endif
