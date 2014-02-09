#ifndef _BINARY_TREE_H__
#define _BINARY_TREE_H__

struct binary_tree;

struct binary_tree * binary_tree_init(int (*cmp)(void *, void *));
void binary_tree_insert(struct binary_tree *t, void *data);
void * binary_tree_remove(struct binary_tree *t, void *data);

int binary_tree_empty(struct binary_tree *t);
int binary_tree_count(struct binary_tree *t);

void binary_tree_ascend_iterate(struct binary_tree *t, void (*iterate_func)(void *));
void binary_tree_descend_iterate(struct binary_tree *t, void (*iterate_func)(void *));

int binary_tree_find(struct binary_tree *t, void *data);
void * binary_tree_find_min(struct binary_tree *t);
void * binary_tree_find_max(struct binary_tree *t);

void binary_tree_release(struct binary_tree **t);
#endif
