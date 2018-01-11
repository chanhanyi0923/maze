#ifndef GRAPH
#define GRAPH

#include <stdlib.h>
#include <gtk/gtk.h>

#include "def.h"
#include "point.h"
#include "stack.h"
#include "disjoint_set.h"

#define GRAPH_MAX_SIZE 100
#define OBSTACLE_WEIGHT 0.3

struct Graph
{
    size_type size;
    char data[GRAPH_MAX_SIZE][GRAPH_MAX_SIZE];
    short int tag[GRAPH_MAX_SIZE][GRAPH_MAX_SIZE];
    gboolean visited[GRAPH_MAX_SIZE][GRAPH_MAX_SIZE];
    struct Point outset, destination;
    struct Stack *path;
    gboolean solved;
};

struct Graph*
Graph_new(int size);

void
Graph_randomlyGenerateGraph(struct Graph *G, mode_type mode);

void
Graph_randomlyGenerateGraphKruskal(struct Graph *G);

char
Graph_makeElement();

void
Graph_randomlyGenerateGraphNaive(struct Graph *G);

void
Graph_dlsUpdateTag(struct Graph *G,
                   struct Point  pos,
                   int           depth);

gboolean
Graph_dls(struct Graph *G,
          struct Point  pos,
          int           depth);

void Graph_initVisited(struct Graph *G);

gboolean
Graph_iddfs(struct Graph *G,
            struct Point  pos);

gboolean
Graph_solved(struct Graph *G);

gboolean
Graph_findDestination(struct Graph *G);

#endif
