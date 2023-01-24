#ifndef DIRLIST_LIST_H
#define DIRLIST_LIST_H

#include "../types.h"

void print_node_data(node_t *);
void traverse_list(node_t *, void(func)(node_t*));
void remove_node_data(node_t *);
void remove_list(node_t *);
node_t * new_list();
void add_node(node_t *, info_t *);

#endif /* DIRLIST_LIST_H */