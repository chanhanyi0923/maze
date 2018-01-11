#include "point.h"

struct Point
Point_makePoint(point_data_type x,
                point_data_type y)
{
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

gboolean
Point_equal(struct Point a,
            struct Point b)
{
    return (gboolean)(a.x == b.x && a.y == b.y);
}

struct Point
Point_add(struct Point a, struct Point b)
{
    struct Point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

struct Point
Point_emptyPoint()
{
    return Point_makePoint(-1, -1);
}

point_data_type
Point_randomInt(int size)
{
    return rand() % (size - 2) + 1;
}

struct Point
Point_randomPosition(int size)
{
    struct Point p;
    p.x = Point_randomInt(size);
    p.y = Point_randomInt(size);
    return p;
}
