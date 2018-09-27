/***************************************** 

* File Name : mmap.c

* Creation Date : 22-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

/**
 * ******************************************************
 * As an alternative to standard file I/O, the kernel
 * provides an interface that allows an application to
 * map a file into memory, meaning that there is a 
 * one-to-one correspondence between a memory address 
 * and a word in the file. The programmer can then access
 * the file directly through memory, identically to any
 * other chunk of memory-resident data—it is even possible 
 * to allow writes to the memory region to transparently 
 * map back to the file on disk.
 *
 *  - or -
 *
 *  Perhaps because you want to force the use of a memory 
 *  mapping over the heap for a specific allocation, or 
 *  perhaps because you are writing your own memory allocation 
 *  system, you may want to manually create your own 
 *  anonymous memory mapping—either way, Linux makes it easy. 
 *  Recall from Chapter 4 that the system call mmap()
 *  creates a memory mapping and the system call munmap() 
 *  destroys a mapping:
 * ******************************************************
**/
int main(int argc, char ** argv) {
    void *p;
    int fd;

    /* open /dev/zero for reading and writing */
    fd = open ("/dev/zero", O_RDWR);
    if (fd < 0) {
            perror ("open");
            return −1;
    }

    /* map [0,page size) of /dev/zero */
    p = mmap (NULL,                   /* do not care where */
              getpagesize (),         /* map one page */
              PROT_READ | PROT_WRITE, /* map read/write */
              MAP_PRIVATE,            /* private mapping */
              fd,                     /* map /dev/zero */
              0);                     /* no offset */

    if (p == MAP_FAILED) {
            perror ("mmap");
            if (close (fd))
                    perror ("close");
            return −1;
    }

    /* close /dev/zero, no longer needed */
    if (close (fd))
            perror ("close");

    /* 'p' points at one page of memory, use it... */  
}
