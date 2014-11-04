#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"
#include "binary_tree.h"
#include "avl_tree.h"
#include "rb_tree.h"

#define NEED_FREE(data)\
    do{\
    void *p = (data);\
    if(p)\
        free(p);\
    }while(0)

static int 
cmp_int(void *value1, void *value2)
{
    int A =  *(int *) value1;
    int B =  *(int *) value2;
    if(A > B)
        return 1;
    if(A < B)
        return -1;
    return 0;
}

static void 
print_int(void *value)
{
    printf("node data is: %d\n", *(int *) value);
}

void 
stack_test(void)
{
    struct stack *s = stack_init();
    int a = 1;
    stack_push(s,(void*)&a);
    char b = '2';
    stack_push(s,&b);
    float f = 3.5;
    stack_push(s,&f);
    stack_push(s,"hello world");

    printf("%s\n", (char*)stack_pop(s));
    printf("%f\n", *(float*)stack_pop(s));
    printf("%c\n", *(char*)stack_pop(s));
    printf("%d\n", *(int*)stack_pop(s));
    stack_release(&s);
}

void avl_tree_test(void)
{
    struct avl_tree *t = avl_tree_init(&cmp_int);
    printf("test for avl tree insert\n");
    for(int i = 0; i != 15; i++) {
        int *val = malloc(sizeof(int));
        *val = i;
        avl_tree_insert(t, val);
    }
    avl_tree_ascend_iterate(t, &print_int);
    printf("test for avl tree remove\n");
    int a = 0;
    NEED_FREE(avl_tree_remove(t, &a));
    a = 1;
    NEED_FREE(avl_tree_remove(t, &a));
    a = 2;
    NEED_FREE(avl_tree_remove(t, &a));
    a = 3;
    NEED_FREE(avl_tree_remove(t, &a));
    a = 4;
    NEED_FREE(avl_tree_remove(t, &a));
    a = 5;
    NEED_FREE(avl_tree_remove(t, &a));
    a = 6;
    NEED_FREE(avl_tree_remove(t, &a));
    avl_tree_ascend_iterate(t, &print_int);
    
    avl_tree_descend_iterate(t, &free);
    //avl_tree_release(&t);
}

void 
binary_tree_test(void)
{
    struct binary_tree *t = binary_tree_init(&cmp_int);
    printf("test for binary tree insert\n");
    int repeat = 0;
    for(int i = 0; i != 15; i++) {
        int *val = malloc(sizeof(int));
        *val = i;
        binary_tree_insert(t, val);
        if(i == 5 && repeat != 3)
        {
            i=0;
            repeat++;
        }
    }
    
    printf("tree node count is %d\n", binary_tree_count(t));

    printf("test for binary tree remove\n");
    //remove test
    binary_tree_descend_iterate(t, &print_int);
    int a = 9;
    NEED_FREE(binary_tree_remove(t, &a));
    a = 1;
    NEED_FREE(binary_tree_remove(t, &a));
    a = 2;
    NEED_FREE(binary_tree_remove(t, &a));
    a = 3;
    NEED_FREE(binary_tree_remove(t, &a));
    a = 4;
    NEED_FREE(binary_tree_remove(t, &a));
    a = 5;
    NEED_FREE(binary_tree_remove(t, &a));
    a = 6;
    NEED_FREE(binary_tree_remove(t, &a));

    printf("tree node count is %d\n", binary_tree_count(t));

    binary_tree_descend_iterate(t, &print_int);
    binary_tree_ascend_iterate(t, &free);

    binary_tree_release(&t);
    if(binary_tree_empty(t))
       printf("success release all binary tree nodes\n");
}


void 
rb_tree_test(void)
{
    printf("test red black tree\n");
    const int node_count = 15;
    struct rb_tree *t = rb_tree_init(&cmp_int);
    for(int i = 0; i != node_count; i++) {
        int *val = malloc(sizeof(int));
        *val = i;
        rb_tree_insert(t, val);
    }
    rb_tree_ascend_iterate(t, &print_int);
    printf("test for redblack tree remove\n");

    int a = 9;
    NEED_FREE(rb_tree_remove(t, &a));
    a = 1;
    NEED_FREE(rb_tree_remove(t, &a));
    a = 2;
    NEED_FREE(rb_tree_remove(t, &a));
    a = 3;
    NEED_FREE(rb_tree_remove(t, &a));
    a = 4;
    NEED_FREE(rb_tree_remove(t, &a));
    a = 5;
    NEED_FREE(rb_tree_remove(t, &a));
    a = 6;
    NEED_FREE(rb_tree_remove(t, &a));

    rb_tree_descend_iterate(t, &print_int);
    rb_tree_release(&t);
    if(rb_tree_empty(t))
       printf("success release all red black tree nodes\n");
}

int 
main(int argc, char* argv[])
{
    //stack_test();
    //binary_tree_test();
    //avl_tree_test();
    rb_tree_test();

    return 0;
}
