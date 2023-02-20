#include "types.h"
#include "str/str.h"
#include "list/list.h"
#include "file/file.h"

int option_flags = 0;

int main(int argc, char **argv)
{
    int option = 0;
    while ((option = getopt(argc, argv, "lrh")) != -1)
    {
        switch (option)
        {
        case 'l': option_flags |= LIST_DIRECTORY;
            break;
        case 'r': option_flags |= REVERSED_ORDER;
            break;
        case 'h': option_flags |= HUMAN_READABLE;
            break;
        default:
            fprintf(stderr, "Usage: %s [-l] [-r] [-h] [path]\n", argv[0]);
            break;
        }
    }

    const char * dir_path = argv[optind];
    if (!dir_path)
    {
        dir_path = ".";
    }

    errno = 0;
    DIR * dir = opendir(dir_path);
    if (!dir)
    {
        fprintf(stderr, "Can't open directory: %s.\n", strerror(errno));
        return 0;
    }

    struct dirent ** dir_entry_list = NULL;
    int dir_entries = receive_dir_entries(dir_path, &dir_entry_list);
    if (dir_entries < 0)
    {
        fprintf(stderr, "Can't read directory: %s.\n", strerror(errno));
        errno = 0;
        if (closedir(dir) == -1)
        {
            fprintf(stderr, "Can't close directory %s.\n", strerror(errno));
            return 0;
        }
    }

    node_t * head = new_list();

    blkcnt_t blocks = 0;

    for (int count = 0; count < dir_entries; count++)
    {
        struct dirent * entry = dir_entry_list[count];
        struct stat fstat;

        char file_path[max_file_path_len] = {};
        cat_str(file_path, max_file_path_len, 3, dir_path, "/", entry->d_name);

        if (file_stat(file_path, entry, &fstat) == -1)
        {
            fprintf(stderr, "Can't read file info: %s.\n", strerror(errno));
            continue;
        }

        char file_name[name_len] = {};

        cat_str(file_name, max_file_name_len, 1, entry->d_name);

        if (entry->d_type == DT_LNK)
        {
            errno = 0;
            char link_dest[max_file_name_len + 1] = {};
            size_t link_dest_len = sizeof(link_dest);

            int bytes = readlink(file_path, link_dest, link_dest_len);
            if (bytes == -1)
            {
                fprintf(stderr, "Can't read link destination: %s\n", strerror(errno));
                continue;
            } else if (bytes > 0)
            {
                link_dest[bytes + 1] = '\0';
                cat_str(file_name, link_dest_len, 2, link_path, link_dest);
            }
        }

        blocks += fstat.st_blocks;

        info_t * info = new_file_info(&fstat, file_name);

        add_node(head, info);
    }

    fprintf(stdout, "total: %ld\n", blocks);

    traverse_list(head, print_node_data);
    traverse_list(head, remove_node_data);

    remove_list(head);

    remove_dir_entries(dir_entry_list, dir_entries);

    errno = 0;
    if (closedir(dir) == -1)
    {
        fprintf(stderr, "Can't close directory: %s.\n", strerror(errno));
        return 0;
    }

    return 0;
}