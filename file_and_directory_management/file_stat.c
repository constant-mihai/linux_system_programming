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

#include <sys/statvfs.h>
#include <stdlib.h>
#include <sys/types.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <math.h>
#include <mntent.h>


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

        //return gnu_dev_major (sb.st_dev);
        return 0;
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

void process(const char *filename)
{
	FILE *fp;
	struct mntent *fs;

	fp = setmntent(filename, "r");	/* read only */
	if (fp == NULL) {
		fprintf(stderr, "%s: could not open: %s\n",
			filename, strerror(errno));
		exit(1);
	}

	while ((fs = getmntent(fp)) != NULL) {
		//do_statvfs(fs);
        printf("MNT_DIR %s", fs->mnt_dir);
        printf("fsname %s", fs->mnt_fsname);
    }

	endmntent(fp);
}

int main (int argc, char *argv[])
{
#if 0
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
#endif
        struct stat sb;

        if (argc != 2) {
            fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
            exit(1);
        }

        if (lstat(argv[1], &sb) == -1) {
            perror("lstat");
            exit(1);
        }

        printf("ID of containing device:  [%lx,%lx]\n",
               (long) major(sb.st_dev), (long) minor(sb.st_dev));

        printf("File type:		     ");

        switch (sb.st_mode & S_IFMT) {
            case S_IFBLK:  printf("block device\n");	       break;
            case S_IFCHR:  printf("character device\n");        break;
            case S_IFDIR:  printf("directory\n");	       break;
            case S_IFIFO:  printf("FIFO/pipe\n");	       break;
            case S_IFLNK:  printf("symlink\n");		       break;
            case S_IFREG:  printf("regular file\n");	       break;
            case S_IFSOCK: printf("socket\n");		       break;
            default:	  printf("unknown?\n"); 	       break;
        }

        printf("I-node number:	     %ld\n", (long) sb.st_ino);

        printf("Mode:		     %lo (octal)\n",
               (unsigned long) sb.st_mode);

        printf("Link count:		     %ld\n", (long) sb.st_nlink);
        printf("Ownership:		     UID=%ld   GID=%ld\n",
               (long) sb.st_uid, (long) sb.st_gid);

        printf("Preferred I/O block size: %ld bytes\n",
               (long) sb.st_blksize);
        printf("File size:		     %lld bytes\n",
               (long long) sb.st_size);
        printf("Blocks allocated:	     %lld\n",
               (long long) sb.st_blocks);

        printf("Last status change:	     %s", ctime(&sb.st_ctime));
        printf("Last file access:	     %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));

        struct statvfs stat;

        if (statvfs(argv[1], &stat) != 0) {
            // error happens, just quits here
            return -1;
        }

        printf("Total space: %.3f GB\n",  (stat.f_bsize * stat.f_blocks) / pow(1024, 3));
        printf("Available space: %.3f GB\n", (stat.f_bsize * stat.f_bavail) / pow(1024, 3));

        process(argv[1]);

        exit(0);
}
