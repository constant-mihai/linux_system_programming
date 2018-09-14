/***************************************** 

* File Name : mmap.c

* Creation Date : 14-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

/**
 * ******************************************************
 * 
void * realloc (void *addr, size_t len)
{
        size_t old_size = look_up_mapping_size (addr);
        void *p;

        p = mremap (addr, old_size, len, MREMAP_MAYMOVE);
        if (p == MAP_FAILED)
                return NULL;
        return p;
}
 * ******************************************************
**/


/**
 * ******************************************************
 * #include <sys/mman.h>

int mprotect (const void *addr,
              size_t len,
              int prot);
 * ******************************************************
**/

/**
 * ******************************************************
 * #include <sys/mman.h>

int msync (void *addr, size_t len, int flags);
 * ******************************************************
**/


/**
 * ******************************************************
 * The only unfamiliar system call in this example should be 
 * fstat(), which we will cover in Chapter 8. All you need to
 * know at this point is that fstat() returns information 
 * about a given file. 
 *
 * The S_ISREG() macro can check some 
 * of this information so that we can ensure that the given
 * file is a regular file (as opposed to a device file or a 
 * directory) before we map it. 
 *
 * The behavior of nonregular
 * files when mapped depends on the backing device.
 * Some device files are mmap-able; other nonregular files 
 * are not mmap-able and will set errno to EACCES.
 *
 * The rest of the example should be straightforward. The program is
 * passed a filename as an argument. It opens the file, ensures 
 * it is a regular file, maps it, closes it, prints the file 
 * byte-by-byte to standard out, and then unmaps the file from memory.
 * ******************************************************
**/
int main (int argc, char *argv[])
{
        struct stat sb;
        off_t len;
        char *p;
        int fd;

        if (argc < 2) {
                fprintf (stderr, "usage: %s <file>\n", argv[0]);
                return 1;
        }

        fd = open (argv[1], O_RDONLY);
        if (fd == -1) {
                perror ("open");
                return 1;
        }

        if (fstat (fd, &sb) == -1) {
                perror ("fstat");
                return 1;
        }

        if (!S_ISREG (sb.st_mode)) {
                fprintf (stderr, "%s is not a file\n", argv[1]);
                return 1;
        }

        p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (p == MAP_FAILED) {
                perror ("mmap");
                return 1;
        }

        if (close (fd) == -1) {
                perror ("close");
                return 1;
        }

        for (len = 0; len < sb.st_size; len++)
                putchar (p[len]);

        if (munmap (p, sb.st_size) == -1) {
                perror ("munmap");
                return 1;
        }

        return 0;
}
