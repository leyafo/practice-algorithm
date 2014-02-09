#include "stack.h"
#include <stdlib.h>

struct stack{
    int count;
    struct element{
        void *data;
        struct element *link;
    } *head;
};


struct stack* 
stack_init(void) {
    struct stack *stk;
    stk = malloc(sizeof(struct stack));
    stk->count = 0;
    stk->head = NULL;

    return stk;
}


int 
stack_empty(struct stack *stk) {
    return stk->count == 0;
}

void 
stack_push(struct stack *stk, void *p) {
    struct element *e;
    e = malloc(sizeof(struct element));
    e->data = p;
    e->link = stk->head;
    stk->head = e;
    stk->count++;
}

void * 
stack_pop(struct stack *stk) {
    void *data;
    struct element *e;
    e = stk->head;
    stk->head = e->link;
    stk->count--;
    data = e->data;
    free(e);

    return data;
}

void 
stack_release(struct stack **stk) {
    struct element *e, *u;
    e = (*stk)->head;
    while(e) {
        u = e;
        e = e->link;
        free(u);
    }
    free(*stk);
    *stk = NULL;
}
