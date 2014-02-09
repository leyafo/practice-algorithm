#include "binary_tree.h"
#include <stdlib.h>
#include <stdio.h>

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

#define SET_PARENT_POINT(parent, old_child, new_child)\
    if(parent){\
        if((parent)->left == old_child)\
            (parent)->left = new_child; \
        else if((parent)->right)\
            (parent)->right = new_child; \
    }

#define FREE_NODE(node)\
    if(node){\
        SET_PARENT_POINT((node)->parent, node, NULL)\
        free(node);\
    }


#define REPLACE_NODE(node, replace_node)\
    struct tree_node *n = (node);\
    struct tree_node *r = (replace_node);\
    n->left = r->left; \
    n->right = r->right; \
    n->data = r->data; \
    r->parent = n->parent; \
    SET_PARENT_POINT(r->parent, n, r)\
    free(n);

#define FOR_EACH_NODE(node, each_func) \
    (*each_func)((node)->data);\
    for(struct list *l = node->equal_list; l != NULL; l = l->next)\
        (*each_func)(l->data);

#define FREE_NODE_LIST(node)\
        struct list *l = (node)->equal_list;\
        while(l) { \
            struct list *tmp = l;\
            l = l->next; \
            free(tmp); \
        } \
        (node)->equal_list = NULL;

    
struct list{
    struct list *next;
    void *data;
};
            
struct tree_node{
    void *data;
    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *parent;
    struct list *equal_list;
};

typedef int (*data_cmp)(void *, void *);

struct binary_tree{
    struct tree_node *root;
    data_cmp cmp_func;
    int count;
};

static void 
_set_parent(struct tree_node *parent, struct tree_node *old_child, 
        struct tree_node *new_child)   
{
    if(parent){
        if(parent->left == old_child)
            parent->left = new_child; 
        else 
            parent->right = new_child; 
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
                    FOR_EACH_NODE(node, iterate_func) 
                    _tree_iterate(node->right, order, iterate_func);
                    break;
                }
            case DESCEND:
                {
                    _tree_iterate(node->right, order, iterate_func);
                    FOR_EACH_NODE(node, iterate_func) 
                    _tree_iterate(node->left, order, iterate_func);
                    break;
                }
            case FREENODE:
                {
                    _tree_iterate(node->left, order, iterate_func);
                    _tree_iterate(node->right, order, iterate_func);
                    FREE_NODE_LIST(node)
                    FREE_NODE(node)
                }
            default:
                break;
        }
    }
}

static void
_tree_insert(struct tree_node **node, void* data, 
        data_cmp cmp_func, struct tree_node *parent) 
{
    if(!(*node)){
        struct tree_node *new_node = malloc(sizeof(*new_node));
        new_node->data = data;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->equal_list = NULL;
        new_node->parent = parent;

        *node = new_node;
        return;
    }

    int result = cmp_func((*node)->data, data);
    if(result > 0)
        _tree_insert(&(*node)->left, data, cmp_func, *node);
    else if(result < 0)
        _tree_insert(&(*node)->right, data, cmp_func, *node);
    else if(result == 0) {
        struct list *list_node = malloc(sizeof(*list_node));
        list_node->data = data;
        list_node->next = (*node)->equal_list; 
        (*node)->equal_list = list_node;
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

struct binary_tree * 
binary_tree_init(int (*cmp)(void *, void *)) {
    struct binary_tree *t = malloc(sizeof(*t));
    t->root = NULL;
    t->cmp_func = cmp;
    t->count = 0;

    return t;
}

int 
binary_tree_empty(struct binary_tree *t) {
    if(t)
        return t->root == NULL;
    return 1;
}

void 
binary_tree_insert(struct binary_tree *t, void *data) {
    _tree_insert(&t->root, data, t->cmp_func, NULL);
    t->count++;
}

void * 
binary_tree_remove(struct binary_tree *t, void *data) {
    struct tree_node *erase_node = _tree_find(t->root, data, t->cmp_func); 
    void * remove_data = NULL;

    if(erase_node)
    {
        struct tree_node *free_node;
        if(erase_node->left == NULL && erase_node->right == NULL){
            _set_parent(erase_node->parent, erase_node, NULL);
            if(t->root == erase_node)
                t->root = NULL;
            remove_data=erase_node->data;
            free(erase_node);
            return remove_data;
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
        remove_data = it->data;
        free(it);
    }

    return remove_data;
}

void 
binary_tree_descend_iterate(struct binary_tree *t, void (*iterate_func)(void *)) {
    _tree_iterate(t->root,  DESCEND, iterate_func); 
}

void 
binary_tree_ascend_iterate(struct binary_tree *t, void (*iterate_func)(void *)) {
    _tree_iterate(t->root,  ASCEND, iterate_func); 
}


int
binary_tree_find(struct binary_tree *t, void *data) {
    if(_tree_find(t->root, data, t->cmp_func))
        return 1;
    return 0;
}

void 
binary_tree_release(struct binary_tree **t) { 
    _tree_iterate((*t)->root, FREENODE, NULL);
    (*t)->count = 0;
    (*t)->root = NULL;
    free(*t);
    *t = NULL;
}

int 
binary_tree_count(struct binary_tree *t) {
    return t->count;
}

void * 
binary_tree_find_min(struct binary_tree *t)
{
    struct tree_node *n = t->root;
    if(!n)
        return NULL;

    while(n->left)
        n = n->left;
    return n->data;
}

void * 
binary_tree_find_max(struct binary_tree *t)
{
    struct tree_node *n = t->root;
    if(!n)
        return NULL;

    while(n->right)
        n = n->right;
    return n->data;
}
