#include <stdlib.h>
#include <stdio.h>
#include "avl_tree.h"


#define DESCEND 0
#define ASCEND 1
#define FREENODE 2

#define DEBUG_PRINT(args1, node)\
    if(node){ \
        printf(args1" address=%p\n", (node)); \
        printf(args1" data =%d\n",*(int*)(node)->data); \
    }else{ \
        printf(args1" is NULL\n"); \
    }


typedef int (*data_cmp)(void *, void *);
struct tree_node{
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *parent;
    unsigned height;
    void *data;
};

struct avl_tree{
    struct tree_node *root;
    int  count;
    data_cmp cmp_func;
};

static inline unsigned 
_get_height(struct tree_node *n)
{
    return (n == NULL) ? 0 : n->height;
}

static void
_update_height(struct tree_node *n)
{
    unsigned lh = _get_height(n->left);
    unsigned rh = _get_height(n->right);
    n->height = (lh > rh) ? lh+1 : rh+1;
}

//please escaple three nodes and more attach in one node!!!
static void 
_set_parent(struct tree_node *parent, struct tree_node *old_child, 
        struct tree_node *new_child)   
{
    if(parent){
        if(parent->left == old_child)
            parent->left = new_child; 
        else 
            parent->right = new_child; 
        _update_height(parent);
    }
    if(new_child)
        (new_child)->parent = parent;
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
                    _set_parent(node->parent, node, NULL);
                    free(node);
                }
            default:
                break;
        }
    }
}

static struct tree_node * 
_tree_find(struct tree_node *node, void *data, data_cmp cmp_func) { 
    if(node == NULL){
        return NULL;
    }

    struct tree_node *n;
    int cmp = cmp_func(node->data, data);

    if(cmp == 0 )
        n = node;
    else if ( cmp > 0)
        n = _tree_find(node->left, data, cmp_func);
    else 
        n = _tree_find(node->right, data, cmp_func); 

    return n;
}

static void
_tree_rotate_left(struct tree_node *node, struct avl_tree *t)
{
    struct tree_node *right = node->right;
    struct tree_node *parent = node->parent;
    if((node->right = right->left))
        (right->left)->parent = node;

    _set_parent(parent, node, right);
    if(!parent)
        t->root = right;

    right->left = node;
    node->parent = right;

    _update_height(node);
    _update_height(right);
}

static void
_tree_rotate_right(struct tree_node *node, struct avl_tree *t)
{
    struct tree_node *left = node->left;
    struct tree_node *parent = node->parent;

    if((node->left = left->right))
        (left->right)->parent = node;

    _set_parent(node->parent, node, left);
    if(!parent)
        t->root = left;

    left->right = node;
    node->parent = left;
    node->height = 1;
    _update_height(node);
    _update_height(left);
}
static void 
_keep_balance(struct avl_tree *t, struct tree_node *node)
{
    struct tree_node *iter = node;
    struct tree_node *parent;
    //bottom to up scan tree 
    while(iter && iter->parent){
        parent = iter->parent;
        _update_height(parent);
        unsigned lh = _get_height(parent->left);
        unsigned rh = _get_height(parent->right);
        struct tree_node *child;
        if(lh - rh == 2) {
            child = parent->left;
            if(_get_height(child->right) > _get_height(child->left)) {
                _tree_rotate_left(child, t);
            }
            _tree_rotate_right(parent, t);
        } else if (rh - lh == 2) {
            child = parent->right;
            if(_get_height(child->left) > _get_height(child->right)) {
                _tree_rotate_right(child, t);
            } 
            _tree_rotate_left(parent, t);
        }
        iter = iter->parent;
    }
}


struct avl_tree * 
avl_tree_init(int (*cmp)(void *, void *))
{
    struct avl_tree *t = malloc(sizeof(*t));
    t->root = NULL;
    t->count = 0;
    t->cmp_func = cmp;
    
    return t;
}

void 
avl_tree_insert(struct avl_tree *t, void *data)
{
    struct tree_node **n = &t->root, *parent = NULL;

    while(1) {
        if(*n == NULL) {
            *n = malloc(sizeof(struct tree_node));
            (*n)->parent = parent;
            (*n)->left = (*n)->right = NULL;
            (*n)->height = 1;
            (*n)->data = data;
            break;
        }
        parent = *n;
        int cmp_result = (*t->cmp_func)((*n)->data,data);
        if( cmp_result > 0)
            n = &(*n)->left;
        else if(cmp_result < 0)
            n = &(*n)->right;
        else
            return; //insert cost is too costly
    }
    ++t->count;

    _keep_balance(t, *n);
    /*struct tree_node *iter= *n;*/
    /*while(iter && iter->parent) {*/
        /*parent = iter->parent;*/
        /*_update_height(parent);*/
        /*unsigned lh = _get_height(parent->left);*/
        /*unsigned rh = _get_height(parent->right);*/
        /*if(lh - rh == 2) {*/
            /*if((*t->cmp_func)(iter->data,data) < 0)  //LR rotate*/
                /*_tree_rotate_left(iter, t);*/
            /*_tree_rotate_right(parent, t);*/
        /*} else if (rh - lh == 2) {*/
            /*if((*t->cmp_func)(iter->data,data) > 0)  //RL rotate*/
                /*_tree_rotate_right(iter, t);*/
            /*_tree_rotate_left(parent, t);*/
        /*}*/
        /*iter = iter->parent;*/
    /*}*/
}

void * 
avl_tree_remove(struct avl_tree *t, void *data)
{
    struct tree_node *erase_node = _tree_find(t->root, data, t->cmp_func); 
    void * remove_data = NULL;
    if(erase_node)
    {
        struct tree_node *free_node;
        if(erase_node->left == NULL && erase_node->right == NULL){
            _set_parent(erase_node->parent, erase_node, NULL);
            if(t->root == erase_node)
                t->root = NULL;
            free_node = erase_node;
            goto keep_balance;
        }

        struct tree_node *it = erase_node;
        if(it->right != NULL) {  //find right child min node
            it = it->right;
            while(it->left)
                it = it->left;

            void * tmp = erase_node->data;  //exchange remove element
            erase_node->data = it->data; 
            it->data = tmp;

            if(it->right){
                _set_parent(it->parent, it, it->right);
            } else {
                _set_parent(it->parent, it, NULL);
            }
        } else {
            _set_parent(it->parent, it, it->left);
            if(t->root == it) {
                t->root = it->left;
            } 
        }
        free_node = it;

    keep_balance:
        {
            _keep_balance(t, free_node);
            remove_data = free_node->data;
            free(free_node);
            --t->count; 
        }
    }

    return remove_data;
}

int 
avl_tree_empty(struct avl_tree *t)
{
    if(!t)
        return 0;
    return t->root == NULL;
}

int 
avl_tree_count(struct avl_tree *t)
{
    return t->count;
}

void 
avl_tree_ascend_iterate(struct avl_tree *t, void (*iterate_func)(void *))
{
    _tree_iterate(t->root, ASCEND, iterate_func);
}

void 
avl_tree_descend_iterate(struct avl_tree *t, void (*iterate_func)(void *))
{
    _tree_iterate(t->root, DESCEND, iterate_func);
}

int 
avl_tree_find(struct avl_tree *t, void *data)
{
    if(_tree_find(t->root, data, t->cmp_func))
        return 1;
    return 0;
}

void 
avl_tree_release(struct avl_tree **t)
{
    _tree_iterate((*t)->root, FREENODE, NULL);
    (*t)->count = 0;
    (*t)->root = NULL;
    free(*t);
    *t = NULL;
}

void * 
avl_tree_find_min(struct avl_tree *t)
{
    struct tree_node *n = t->root;
    if(!n)
        return NULL;
    
    while(n->left)
        n = n->left;
    return n->data;
}

void * 
avl_tree_find_max(struct avl_tree *t)
{
    struct tree_node *n = t->root;
    if(!n)
        return NULL;
    while(n->right)
        n = n->right;
    return n->data;
}
