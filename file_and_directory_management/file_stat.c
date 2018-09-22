/***************************************** 

* File Name : file_stat.c

* Creation Date : 22-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

#include <sys/types.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


/*
 * is_on_physical_device - returns a positive
 * integer if 'fd' resides on a physical device,
 * 0 if the file resides on a nonphysical or
 * virtual device (e.g., on an NFS mount), and
 * −1 on error.
 */
int is_on_physical_device (int fd)
{
        struct stat sb;
        int ret;

        ret = fstat (fd, &sb);
        if (ret) {
                perror ("fstat");
                return -1;
        }

        return gnu_dev_major (sb.st_dev);
}

void wrapper_change_mod() {
    int ret;
    ret = chmod ("./map.png", S_IRUSR | S_IWUSR);

    if (ret) {
        perror("chmod");
    }
}

void wrapper_fchange_mod(int fd) {
    int ret;
    ret = fchmod (fd, S_IRUSR | S_IWUSR);

    if (ret) {
        perror("fchmod");
    }
}

void wrapper_owner() {
    struct group *gr;
    int ret;
    /*
     * getgrnam() returns information on a group
     * given its name.
     */
    gr = getgrnam ("officers");
    if (!gr) {
            /* likely an invalid group */
            perror ("getgrnam");
            return;
    }

    /* set manifest.txt's group to 'officers' */
    ret = chown("manifest.txt", -1, gr->gr_gid);
    if (ret)
            perror ("chown");
}


/*
 * make_root_owner - changes the owner and group of the file
 * given by 'fd' to root. Returns 0 on success and −1 on
 * failure.
 */
int make_root_owner (int fd)
{
        int ret;

        /* 0 is both the gid and the uid for root */
        ret = fchown (fd, 0, 0);
        if (ret)
                perror ("fchown");

        return ret;
}

/*
 * find_file_in_dir - searches the directory 'path' for a
 * file named 'file'.
 *
 * Returns 0 if 'file' exists in 'path' and a nonzero
 * value otherwise.
 */
int find_file_in_dir (const char *path, const char *file)
{
        struct dirent *entry;
        int ret = 1;
        DIR *dir;

        dir = opendir (path);

        errno = 0;
        while ((entry = readdir (dir)) != NULL) {
                if (strcmp(entry->d_name, file) == 0) {
                        ret = 0;
                        break;
                }
        }

        if (errno && !entry)
                perror ("readdir");

        closedir (dir);
        return ret;
}


int main (int argc, char *argv[])
{
        struct stat sb;
        int ret;

        if (argc < 2) {
                fprintf (stderr,
                         "usage: %s <file>\n", argv[0]);
                return 1;
        }

        ret = stat (argv[1], &sb);
        if (ret) {
                perror ("stat");
                return 1;
        }

        printf ("%s is %ld bytes\n",
                argv[1], sb.st_size);


        printf ("File type: ");
        switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:
                printf("block device node\n");
                break;
        case S_IFCHR:
                printf("character device node\n");
                break;
        case S_IFDIR:
                printf("directory\n");
                break;
        case S_IFIFO:
                printf("FIFO\n");
                break;
        case S_IFLNK:
                printf("symbolic link\n");
                break;
        case S_IFREG:
                printf("regular file\n");
                break;
        case S_IFSOCK:
                printf("socket\n");
                break;
        default:
                printf("unknown\n");
                break;
        }

        return 0;
}
