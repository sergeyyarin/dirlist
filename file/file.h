#ifndef DIRLIST_FILE_H
#define DIRLIST_FILE_H

#include "../types.h"

int file_stat(const char *, const struct dirent *, struct stat *);
info_t * new_file_info(const struct stat *, const char []);
void remove_dir_entries(struct dirent **, int);
int receive_dir_entries(const char *, struct dirent ***);

#endif /* DIRLIST_FILE_H */