#include <stdlib.h>
#include <stdio.h>

#include "rb_tree.h"

#define RED 1
#define BLACK 0

#define DESCEND 0
#define ASCEND 1
#define FREENODE 2

typedef int (*data_cmp)(void *, void*);

struct tree_node{
    struct tree_node *left;
    struct tree_node *right;
    char color;
    void *data;
};

struct rb_tree{
    struct tree_node *root;
    data_cmp cmp_func;
};


struct rb_tree *
rb_tree_init(int (*cmp)(void *, void*))
{
    struct rb_tree *t = malloc(sizeof(*t));
    t->root=NULL;
    t->cmp_func = cmp;
    return t;
}

static void
_tree_iterate(struct tree_node *node, int order,
        void (*iterate_func)(void *)) 
{
    if(node)
    {
        switch(order){
            case ASCEND:
                {
                    _tree_iterate(node->left, order, iterate_func);
                    (*iterate_func)(node->data);
                    _tree_iterate(node->right, order, iterate_func);
                    break;
                }
            case DESCEND:
                {
                    _tree_iterate(node->right, order, iterate_func);
                    (*iterate_func)(node->data);
                    _tree_iterate(node->left, order, iterate_func);
                    break;
                }
            case FREENODE:
                {
                    _tree_iterate(node->left, order, iterate_func);
                    _tree_iterate(node->right, order, iterate_func);
                    free(node);
                }
            default:
                break;
        }
    }
}

struct tree_node *
_node_rotate_left(struct tree_node *n)
{
    struct tree_node *n_r = n->right;

    n->right = n_r->left;
    n_r->left = n;
    n_r->color = n_r->left->color;
    n_r->left->color = RED;

    return n_r;
}

struct tree_node *
_node_rotate_right(struct tree_node *n)
{
    struct tree_node *n_l = n->left;
    n->left = n_l->right;
    n_l->right = n;
    n_l->color = n_l->right->color;
    n_l->right->color = RED;
    
    return n_l;
}

struct tree_node *
color_flip(struct tree_node *n)
{
    n->color = !n->color;
    n->left->color = !n->left->color;
    n->right->color = !n->right->color;

    return n;
}

int
is_red(struct tree_node *n)
{
    if(n == NULL || n->color == BLACK)
        return 0;
    return 1;
}

struct tree_node *
fix_up(struct tree_node *n)
{
    if(is_red(n->right)){
        n = _node_rotate_left(n);
    }

    if(is_red(n->left) && is_red(n->left->left)){
        n = _node_rotate_right(n);
    }

    if(is_red(n->left) && is_red(n->right)){
        n = color_flip(n);
    }

    return n;
}

struct tree_node *
_node_insert(struct tree_node *n, void *data, data_cmp cmp_func)
{
    if(n == NULL)
    {
        n = malloc(sizeof(*n));
        n->left = n->right = NULL;
        n->data = data;
        n->color = RED;
        return n;
    }

    int cmp = cmp_func(n->data, data);
    if (cmp == 0){
        //do nothing
    } else if(cmp > 0){
        n->left = _node_insert(n->left, data, cmp_func);
    } else {
        n->right = _node_insert(n->right, data, cmp_func);
    }

    if(is_red(n->right)){
        n = _node_rotate_left(n);
    }

    if(is_red(n->left) && is_red(n->left->left)){
        n = _node_rotate_right(n);
    }

    if(is_red(n->left) && is_red(n->right)){
        color_flip(n);
    }

    return n;
}

struct tree_node *
_move_red_right(struct tree_node *n)
{
    color_flip(n);
    if(is_red(n->left->left)){
        n = _node_rotate_right(n);
        color_flip(n);
    }
    return n;    
}

struct tree_node *
_move_red_left(struct tree_node *n)
{
    color_flip(n);
    if(is_red(n->right->left)){
        n->right = _node_rotate_right(n->right);
        n = _node_rotate_left(n);
        color_flip(n);
    }

    return n;
}

struct tree_node *
_delete_min(struct tree_node *n)
{
    if(n->left == NULL){
        free(n);
        return NULL;
    }

    if(!is_red(n->left) && !is_red(n->left->left))
    {
        n = _move_red_left(n);
    }

    n->left = _delete_min(n);

    return fix_up(n);
}

struct tree_node *
_node_remove(struct tree_node *n, void *data, void **ret_data, data_cmp cmp_func) 
{
    if (n == NULL)
        return NULL;

    int cmp = cmp_func(data, n->data);
    if(cmp < 0) {
        if(!is_red(n->left) && !is_red(n->left->left))
            n = _move_red_left(n);
        n->left = _node_remove(n->left, data, ret_data, cmp_func);
    } else {
        if(is_red(n->left)){
            n = _node_rotate_right(n);
        }

        if(cmp == 0 && n->right == NULL){
            return NULL;
        }

        if(!is_red(n->right) && !is_red(n->right->left)){
            n = _move_red_right(n);
        }

        if(cmp == 0){
            struct tree_node *r = n->right;
            while(r->left != NULL){
                r = r->left;
            }
            *ret_data = n->data;
            n->data = r->data;
            n->right = _delete_min(n->right);
        } else {
            n->right = _node_remove(n->right, data, ret_data,cmp_func);
        }
    }

    return fix_up(n);
}


void 
rb_tree_insert(struct rb_tree *t, void *data)
{
    t->root = _node_insert(t->root, data, t->cmp_func);
    t->root->color = BLACK;
}

void * 
rb_tree_remove(struct rb_tree *t, void *data)
{
    void *delete_data = NULL;
    t->root = _node_remove(t->root, data, &delete_data, t->cmp_func);
    t->root->color = BLACK;
    return delete_data;
}


int 
rb_tree_empty(struct rb_tree *t){
    return t == NULL || t->root == NULL;
}

void 
rb_tree_ascend_iterate(struct rb_tree *t, void (*iterate_func)(void *))
{
    _tree_iterate(t->root,ASCEND, iterate_func);
}

void 
rb_tree_descend_iterate(struct rb_tree *t, void (*iterate_func)(void *))
{
    _tree_iterate(t->root,DESCEND, iterate_func);
}

void rb_tree_release(struct rb_tree **t){
    _tree_iterate((*t)->root, FREENODE, NULL);
    (*t)->root = NULL;
    free(*t);
    *t = NULL;
}


void * 
rb_tree_find_min(struct rb_tree *t)
{
    if (t->root == NULL)
        return NULL;
    struct tree_node *n = t->root;
    while(n->left != NULL){
        n = n->left;
    }
    return n->data;
}

void *
rb_tree_find_max(struct rb_tree *t)
{
    if (t->root == NULL)
        return NULL;
    struct tree_node *n = t->root;
    while(n->right != NULL){
        n = n->right;
    }
    return n->data;
}

