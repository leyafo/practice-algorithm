#ifndef __RB_TREE__
#define __RB_TREE__

struct rb_tree;

struct rb_tree * rb_tree_init(int (*cmp)(void *, void *));
void rb_tree_insert(struct rb_tree *t, void *data);
void * rb_tree_remove(struct rb_tree *t, void *data);

int rb_tree_empty(struct rb_tree *t);

void rb_tree_ascend_iterate(struct rb_tree *t, void (*iterate_func)(void *));
void rb_tree_descend_iterate(struct rb_tree *t, void (*iterate_func)(void *));

void rb_tree_release(struct rb_tree **t);
void * rb_tree_find_min(struct rb_tree *t);
void * rb_tree_find_max(struct rb_tree *t);

#endif
