#include "disjoint_set.h"

struct DisjointSet*
DisjointSet_new(int size)
{
    struct DisjointSet *DSU = (struct DisjointSet*)
                 malloc(sizeof(struct DisjointSet));
    DSU->size = size;

    int i;
    for (i = 0; i < DSU->size; i ++) {
        DSU->data[i] = i;
    }

    return DSU;
}

int
DisjointSet_find(struct DisjointSet *DSU,
                 int                 a)
{
    return (DSU->data[a] == a) ? a :
           (DSU->data[a] = DisjointSet_find(DSU, DSU->data[a]));
}

void
DisjointSet_merge(struct DisjointSet *DSU,
                  int                 a,
                  int                 b)
{
    a = DisjointSet_find(DSU, a);
    b = DisjointSet_find(DSU, b);
    if (a != b) {
        DSU->data[a] = b;
    }
}

gboolean
DisjointSet_sameSet(struct DisjointSet *DSU,
                    int                 a,
                    int                 b)
{
    return (gboolean)
           (DisjointSet_find(DSU, a) == DisjointSet_find(DSU, b));
}
