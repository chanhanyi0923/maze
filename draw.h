#ifndef DRAW
#define DRAW

#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>

#include "def.h"
#include "graph.h"

#define BLOCK_SIZE 30

#define OBSTACLE_COLOR    0.77, 0.16, 0
#define ROAD_COLOR        0.35, 0.77, 1
#define DESTINATION_COLOR 0,    1,    0
#define OUTSET_COLOR      0,    0,    1

extern const char Graph_road;
extern const char Graph_obstacle;
extern const char Graph_destination;
extern const char Graph_outset;

void
drawGraph(struct Graph *G,
          cairo_surface_t *surface);

gboolean
drawPath(GtkWidget *widget,
         struct Graph *G,
         cairo_surface_t *surface);

#endif
