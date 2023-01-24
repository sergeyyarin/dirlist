#include "str.h"

char * str_group(gid_t id)
{
    errno = 0;
    struct group * gr_ptr = getgrgid(id);
    if (!gr_ptr)
    {
        fprintf(stderr, "Can't find group: %s\n", strerror(errno));
        return NULL;
    }
    return gr_ptr->gr_name;
}

char * str_user(uid_t id)
{
    errno = 0;
    struct passwd * pw_ptr = getpwuid(id);
    if (!pw_ptr)
    {
        fprintf(stderr, "Can't find user: %s\n", strerror(errno));
        return NULL;
    }
    return pw_ptr->pw_name;
}

char * str_mode(mode_t mode)
{
    #define decode_flag_r_or_w(mode, scope, permission, flag) \
        (mode & S_I##permission##scope) ? flag : '-'

    #define decode_mode_usr(mode) \
        decode_flag_r_or_w(mode, USR, R, 'r'), \
        decode_flag_r_or_w(mode, USR, W, 'w'), \
        (mode & S_IXUSR) ? \
            ((mode & S_ISUID) ? 's' : 'x') : \
            ((mode & S_ISUID) ? 'S' : '-')

    #define decode_mode_grp(mode) \
        decode_flag_r_or_w(mode, GRP, R, 'r'), \
        decode_flag_r_or_w(mode, GRP, W, 'w'), \
        (mode & S_IXGRP) ? \
            ((mode & S_ISGID) ? 's' : 'x') : \
            ((mode & S_ISGID) ? 'S' : '-')

    #define decode_mode_oth(mode) \
        decode_flag_r_or_w(mode, OTH, R, 'r'), \
        decode_flag_r_or_w(mode, OTH, W, 'w'), \
        (mode & S_IXOTH) ? \
        ((mode & S_ISVTX) ? 't' : 'x') : \
        ((mode & S_ISVTX) ? 'T' : '-')

    static char mode_str[file_mode_str_len];
    snprintf(mode_str, file_mode_str_len, "%c%c%c%c%c%c%c%c%c",
             decode_mode_usr(mode), decode_mode_grp(mode), decode_mode_oth(mode));
    return mode_str;
}

char str_type(mode_t mode)
{
    switch (mode & S_IFMT)
    {
    case S_IFREG:
        return '-';
    case S_IFDIR:
        return 'd';
    case S_IFLNK:
        return 'l';
    case S_IFCHR:
        return 'c';
    case S_IFBLK:
        return 'b';
    case S_IFIFO:
        return 'p';
    case S_IFSOCK:
        return 's';
    default:
        break;
    }
    return 0;
}

char * str_size(off_t st_size)
{
    int u_index = 0;
    float size = st_size;

    while (size >= 1024 && u_index <= sizeof(units))
    {
        size /= 1024.0;
        if (size > 0)
            u_index++;
    }
    float r_size = roundf(size);

    const char * format = "%.0lf%c";
    if (r_size <= 9 && u_index > 0)
        format = "%.1lf%c";

    char * readable = malloc(readable_size_len + 1);

    snprintf(readable, readable_size_len, format, r_size, units[u_index]);

    return readable;
}

char * str_time(time_t time)
{
    struct tm local_time;
    localtime_r(&time, &local_time);
    static char buffer[time_str_len] = {};
    strftime(buffer, time_str_len, "%b %d %H:%M", &local_time);
    return buffer;
}

void cat_str(char * buf, size_t buf_len, uint n_agrs, ...)
{
    if (buf == NULL || n_agrs == 0)
    {
        return;
    }

    va_list list;
    va_start(list, n_agrs);

    for (int arg = 0; arg < n_agrs; ++arg)
    {
        const char * str = va_arg(list, char *);
        size_t str_len = strnlen(str, max_file_name_len + 1);
        if (str_len == 0 || str_len == max_file_name_len)
        {
            continue;
        }
        strncat(buf, str, str_len);
    }
    va_end(list);
}
