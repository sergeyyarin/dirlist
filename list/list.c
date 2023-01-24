#include "list.h"

void print_node_data(node_t * node)
{
    if (!node || !node->data)
    {
        return;
    }
    const info_t * info = (info_t *)node->data;

    if (!(option_flags & LIST_DIRECTORY))
    {
        fprintf(stdout, "%s\t", info->name);
        return;
    }
    fprintf(stdout, "%c", info->type);
    fprintf(stdout, "%s ", info->mode);
    fprintf(stdout, "%5lu ", info->links);
    fprintf(stdout, "%8s ", info->uname);
    fprintf(stdout, "%8s ", info->gname);
    if (option_flags & HUMAN_READABLE)
    {
        fprintf(stdout, "%5s ", info->rsize);
    } else
    {
        fprintf(stdout, "%10ld ", info->size);
    }
    
    fprintf(stdout, "%12s ", info->time);
    fprintf(stdout, "%s\n", info->name);
}


void traverse_list(node_t * head, void(func)(node_t*))
{
    if (!head)
    {
        return;
    }
    node_t * node = head->next;

    while (node)
    {
        if (func)
        {
            func(node);
        }
        node = node->next;
    }
}

void remove_node_data(node_t * node)
{
    assert(node != NULL);

    free(node->data->name);
    free(node->data->rsize);
    free(node->data);
}

void remove_list(node_t * head)
{
    node_t * node = head->next;
    while(node)
    {
        node_t * waste = node;
        node = node->next;
        free(waste);
    }
    free(head);
}

node_t * new_list()
{
    node_t * head = malloc(sizeof(node_t));
    assert(head != NULL);
    head->data = NULL;
    head->next = NULL;
    return head;
}

void add_node(node_t * head, info_t * data)
{
    if (!head || !data)
    {
        return;
    }

    node_t * new_node = malloc(sizeof(node_t));
    assert(new_node != NULL);

    new_node->data = (node_data_t*)data;
    new_node->next = NULL;
    node_t * node = head;

    while(node->next != NULL)
    {
        node = node->next;
    }
    node->next = new_node;
}
