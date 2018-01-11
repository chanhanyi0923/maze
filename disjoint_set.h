#ifndef DISJOINT_SET
#define DISJOINT_SET

#include <stdlib.h>
#include <gtk/gtk.h>

#include "def.h"

#define DSU_SIZE 10000

struct DisjointSet
{
    int data[DSU_SIZE];
    size_type size;
};

struct DisjointSet*
DisjointSet_new(int size);

int
DisjointSet_find(struct DisjointSet *DSU,
                 int                 a);

void
DisjointSet_merge(struct DisjointSet *DSU,
                  int                 a,
                  int                 b);

gboolean
DisjointSet_sameSet(struct DisjointSet *DSU,
                    int                 a,
                    int                 b);

#endif
