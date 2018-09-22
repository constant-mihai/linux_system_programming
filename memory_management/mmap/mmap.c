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
