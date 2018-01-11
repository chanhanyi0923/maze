#ifndef POINTER_CONTIANER
#define POINTER_CONTIANER

#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#define CONTIANER_SIZE 10

struct PointerContianer
{
    int size;
    char *name[CONTIANER_SIZE];
    gpointer data[CONTIANER_SIZE];
};

gpointer makePC();

gpointer PointerContianer_get(struct PointerContianer *PC,
                              char *name);

struct PointerContianer*
PointerContianer_new(gpointer start, ...);

struct PointerContianer
PointerContianer_make(gpointer start, ...);

#endif
