#ifndef STACK
#define STACK

#include "def.h"
#include "point.c"

#define S_SIZE 10000

typedef struct Point stack_type;

struct Stack
{
    stack_type data[S_SIZE];
    int top;
};

struct Stack *Stack_new();

size_type
Stack_size(struct Stack *S);

gboolean
Stack_empty(struct Stack *S);

stack_type
Stack_top(struct Stack *S);

void
Stack_pop(struct Stack *S);

void
Stack_push(struct     Stack *S,
           stack_type value);

#endif
