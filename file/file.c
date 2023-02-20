#include "file.h"
#include "../str/str.h"

static int ascending(const struct dirent ** ent1, const struct dirent ** ent2)
{
    assert(ent1 != NULL);
    assert(ent2 != NULL);
    return alphasort(ent1, ent2);
}

static int descending(const struct dirent ** ent1, const struct dirent ** ent2)
{
    assert(ent1 != NULL);
    assert(ent2 != NULL);
    return alphasort(ent2, ent1);
}

static int filter(const struct dirent * ent)
{
    assert(ent != NULL);
    if (*ent->d_name == '.' || strncmp(ent->d_name, "..", strlen("..")) == 0)
    {
        return 0;
    }
    return 1;
}

int receive_dir_entries(const char * dir_path, struct dirent *** dir_entry_list)
{
    if (!dir_path)
    {
        return 0;
    }

    int (*comparator)(const struct dirent **, const struct dirent **) = ascending;
    if (option_flags & REVERSED_ORDER)
    {
        comparator = descending;
    }
    errno = 0;

    return scandir(dir_path, dir_entry_list, filter, comparator);
}

int file_stat(const char * path, const struct dirent * entry, struct stat * fstat)
{
    int stat_ret = 0;
    errno = 0;
    switch (entry->d_type)
    {
    case DT_LNK:
        stat_ret = lstat(path, fstat);
        break;
    default:
        stat_ret = stat(path, fstat);
    }
    return stat_ret;
}

info_t * new_file_info(const struct stat * fstat, const char name[])
{
    if (!fstat)
    {
        return NULL;
    }

    info_t * new_info = calloc(1, sizeof(info_t));
    assert(new_info != NULL);

    new_info->type  = str_type(fstat->st_mode);
    new_info->mode  = str_mode(fstat->st_mode);
    new_info->links = fstat->st_nlink;
    new_info->uname = str_user(fstat->st_uid);
    new_info->gname = str_group(fstat->st_gid);
    new_info->size  = fstat->st_size;
    new_info->rsize = str_size(fstat->st_size);
    new_info->time  = str_time(fstat->st_mtim.tv_sec);

    strncpy(new_info->name, name, name_len);

    return new_info;
}

void remove_dir_entries(struct dirent ** list, int index)
{
    if (!list)
    {
        return;
    }

    while(index--)
    {
        free(list[index]);
    }
    free(list);
}
