#ifndef POINT
#define POINT

#include <stdlib.h>
#include <gtk/gtk.h>

typedef short int point_data_type;

struct Point
{
    point_data_type x, y;
};

static struct Point
Point_makePoint(point_data_type x,
                point_data_type y);

static gboolean
Point_equal(struct Point a,
            struct Point b);

static struct Point
Point_add(struct Point a,
          struct Point b);

static struct Point
Point_emptyPoint();

static point_data_type
Point_randomInt(int size);

static struct Point
Point_randomPosition(int size);

#endif
