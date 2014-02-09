#ifndef __AVL_TREE__
#define __AVL_TREE__

struct avl_tree;

struct avl_tree * avl_tree_init(int (*cmp)(void *, void *));
void avl_tree_insert(struct avl_tree *t, void *data);
void * avl_tree_remove(struct avl_tree *t, void *data);

int avl_tree_empty(struct avl_tree *t);
int avl_tree_count(struct avl_tree *t);

void avl_tree_ascend_iterate(struct avl_tree *t, void (*iterate_func)(void *));
void avl_tree_descend_iterate(struct avl_tree *t, void (*iterate_func)(void *));

int avl_tree_find(struct avl_tree *t, void *data);

void avl_tree_release(struct avl_tree **t);
void * avl_tree_find_min(struct avl_tree *t);
void * avl_tree_find_max(struct avl_tree *t);
#endif
