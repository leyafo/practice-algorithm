#ifndef __STACK__H__
#define __STACK__H__

struct stack;

struct stack* stack_init(void);
int stack_empty(struct stack *stk);
void stack_push(struct stack *stk, void *p);
void * stack_pop(struct stack *stk);
void stack_release(struct stack **stk);

#endif
