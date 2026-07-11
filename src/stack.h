#ifndef SOVIMTER_STACK_H
#define SOVIMTER_STACK_H

typedef struct _STACK {
    void *stack;
    int size;
} STACK;

bool is_empty(STACK *stack);
bool is_full(STACK *stack);
bool push(STACK *stack, void *item);
int pop(STACK *stack);

#endif //SOVIMTER_STACK_H