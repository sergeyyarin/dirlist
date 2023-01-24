#ifndef DIRLIST_TYPES_H
#define DIRLIST_TYPES_H

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif /* _DEFAULT_SOURCE */

#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdarg.h>

#define file_mode_str_len sizeof("rwxrwxrwx")
#define max_file_path_len 4096
#define max_file_name_len 255
#define name_len (2 * max_file_name_len) + sizeof(link_path) + 1
#define time_str_len 15
#define readable_size_len 7

static const char units[] = { '\0', 'K', 'M', 'G', 'T' };
static const char * const link_path = " -> ";
extern int option_flags;

enum
{
    LIST_DIRECTORY = 1 << 1,
    REVERSED_ORDER = 1 << 2,
    HUMAN_READABLE = 1 << 3
};

typedef struct
{
    char    type;
    char    *mode;
    nlink_t links;
    char    *uname;
    char    *gname;
    off_t   size;
    char    *rsize;
    char    *time;
    char    *name;
} info_t;

typedef info_t node_data_t;

typedef struct node
{
    node_data_t * data;
    struct node * next;
} node_t;
#endif /* DIRLIST_TYPES_H */