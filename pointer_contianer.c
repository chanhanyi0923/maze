#include "pointer_contianer.h"

gpointer makePC()
{
    return (gpointer)malloc(sizeof(struct PointerContianer));
}

gpointer PointerContianer_get(struct PointerContianer *PC,
                              char *name)
{
    int i;
    for (i = 0; i < PC->size; i ++) {
        if (strcmp(name, PC->name[i]) == 0) {
            return PC->data[i];
        }
    }
    return NULL;
}

struct PointerContianer*
PointerContianer_new(gpointer start, ...)
{
    struct PointerContianer *PC =
        (struct PointerContianer*)malloc(sizeof(struct PointerContianer));
    PC->size = 0;

    va_list arg_ptr;
    gpointer value = start;
    va_start(arg_ptr, start);
    while (value) {
        PC->name[PC->size] = value;
        value = va_arg(arg_ptr, gpointer);
        PC->data[PC->size] = value;
        PC->size ++;

        value = va_arg(arg_ptr, gpointer);
    }
    va_end(arg_ptr);

    return PC;
}

struct PointerContianer
PointerContianer_make(gpointer start, ...)
{
    struct PointerContianer PC;
    PC.size = 0;

    va_list arg_ptr;
    gpointer value = start;
    va_start(arg_ptr, start);
    do {
        PC.name[PC.size] = value;
        value = va_arg(arg_ptr, gpointer);
        PC.data[PC.size] = value;
        PC.size ++;
    } while (value = va_arg(arg_ptr, gpointer));
    va_end(arg_ptr);

    return PC;
}
