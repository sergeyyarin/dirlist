#ifndef DIRLIST_STR_H
#define DIRLIST_STR_H

#include "../types.h"

char * str_group(gid_t);
char * str_user(uid_t);
char * str_mode(mode_t);
char   str_type(mode_t);
char * str_size(off_t);
char * str_time(time_t);
void cat_str(char *, size_t, uint, ...);

#endif /* DIRLIST_STR_H */