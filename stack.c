#include "stack.h"

struct Stack *Stack_new()
{
    struct Stack *S = (struct Stack *)malloc(sizeof(struct Stack));
    S->top = -1;
}

size_type
Stack_size(struct Stack *S)
{
    return S->top + 1;
}

gboolean
Stack_empty(struct Stack *S)
{
    return (gboolean)(S->top == -1);
}

stack_type
Stack_top(struct Stack *S)
{
    return S->data[S->top];
}

void
Stack_pop(struct Stack *S)
{
    S->top --;
}

void
Stack_push(struct Stack *S,
           stack_type    value)
{
    S->data[++ S->top] = value;
}
