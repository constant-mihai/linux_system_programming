/***************************************** 

* File Name : main.c

* Creation Date : 13-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

#include <stdbool.h>

/* open(), creat(), lseek(), ftruncate */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* getcwd(), read(), write(), fsync(), close() */
/* lseek(), pread(), pwrite(), ftruncate() */
#include <unistd.h>

/* printf() */
#include <stdio.h>

/* PATH_MAX */
#include <limits.h>

/* str* calls */
#include <string.h>

/* errno */
#include <errno.h>

/* */
#include "multiplexed_io.h"


/**
 * ******************************************************
 * Opens a file. `man open` for all the flags.
 * ******************************************************
**/
int wrapper_open(const char* str, int flags) {
    int fd = 0;

    fd = open (str, flags);
    if (fd == -1) {
        perror("Error opening file:");
        return 1;
    } else {
        printf("Opened: %s\n", str);
    }

    return fd;
}


/**
 * ******************************************************
 * The combination of O_WRONLY | O_CREAT | O_TRUNC is so 
 * common that a system call exists to provide just that 
 * behavior:
 * ******************************************************
**/
int wrapper_creat() {
    int fd;
    fd = creat ("some_file", 0644);
    if (fd == -1) {      /* error */
    }

    return 0;
}

/**
 * ******************************************************
 * Wrapper for read.
 * read() can have a plethora of results.
 * `man read` for  possible return values.
 * * also check pread()
 * ******************************************************
**/
ssize_t wrapper_blocking_read(int fd, void *buf, size_t len) {

    ssize_t ret, total_ret = 0;

    if (len > SSIZE_MAX) len = SSIZE_MAX;

    while (len != 0 && (ret = read(fd, buf, len)) != 0) {
        if (ret == -1) {
            if (errno == EINTR) continue;
            /* else */
            perror("blocking read");
            break;
        }
        len -= ret;
        buf += ret;
        total_ret += ret;
    }

    return total_ret;
}

ssize_t wrapper_nonblocking_read(int fd, void *buf, size_t len) {
    ssize_t ret = 0;

    if (len > SSIZE_MAX) len = SSIZE_MAX;

start:
    ret = read(fd, buf, len);
    if (ret == -1) {
        if (errno == EINTR) goto start;
        if (errno == EAGAIN) {
            /* Do some usefull stuff */
            /* Resubmit read again later */
        } else {
            /* else error */
            perror("non-blocking read");
        }
    }

    return ret;
}

/**
 * ******************************************************
 * Write wrapper
 * * also check pwrite()
 * ******************************************************
**/
ssize_t wrapper_write(int fd, const void *buf, size_t count) {
    ssize_t nr;
    int ret;
    bool essential_data_only = true;

    nr = write(fd, buf, strlen(buf));
    if (nr == -1) {
        perror("write error");
    } else if (nr != count) {
        /* possible error, but errno is not set*/
    }

    // Alternativley, open with O_SYNC, O_DSYNC, O_RSYNC
    if (essential_data_only ) {
        ret = fdatasync(fd);
        if (ret == -1) perror("fdatasync error:");
    } else {
        ret = fsync(fd);
        if (ret == -1) perror("fsync error:");
    }

    return nr;
}


ssize_t wrapper_socket_write(int fd, const void *buf, size_t len) {
    ssize_t ret, total_ret;

    /* Trying to write more than SSIZE_MAX results in 
     * undefined behaviour */
    if (len > SSIZE_MAX) len = SSIZE_MAX;

    while (len != 0 && (ret = write (fd, buf, len)) != 0) {
        if (ret == -1) {
            if (errno == EINTR) continue; 
            /*else */
            perror ("write");
            break;
        }
#if 0
        // Uncomment this if you want to flush the data to the hdd
        // Alternativley, open with O_SYNC, O_DSYNC, O_RSYNC
        if (essential_data_only ) {
            ret = fdatasync(fd);
            if (ret == -1) perror("fdatasync error:");
        } else {
            ret = fsync(fd);
            if (ret == -1) perror("fsync error:");
        }
#endif

        len -= ret;
        buf += ret;
        total_ret += ret;
    }

    return total_ret;
}

/**
 * ******************************************************
 *  Close wrapper
 * ******************************************************
**/
int wrapper_close(int fd) {
    int ret = 0;
    if ( (ret = close(fd)) == -1) {
        perror ("close");
    }

    return ret;
}

/**
 * ******************************************************
 *  Lseek wrapper. curr + pos
 *
 *   - SEEK_CUR: if pos = 0, sets cursor to curr pos
 *   - SEEK_END: if pos = 0, sets cursor to end
 *   - SEEK_SET: if pos = 0, sets cursor to beginning
 *
 *   * also check pread() and pwrite()
 *
 * ******************************************************
**/
off_t wrapper_lseek(int fd, off_t pos, int origin) {
    off_t ret;

    /* Set the cursor to poistion pos */
    ret = lseek(fd, pos, SEEK_SET);
    if (ret == (off_t) -1) {
        perror("lseek:");
    }

    return ret;
}

/**
 * ******************************************************
 *  Truncate wrapper
 * ******************************************************
**/
int wrapper_truncate(const char* path, off_t len) {
    int ret;
    ret = truncate ("./pirate.txt", 45);
    if (ret == -1) {
        perror ("truncate");
        return -1;
    } 
    return 0;
}

/**
 * ******************************************************
 *  Main
 * ******************************************************
**/
int main(int argc, char ** argv) {
    int fd;
    char cwd[PATH_MAX];
    char abs_path[PATH_MAX + 30];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    fd = wrapper_open("/home/teach/pearl", O_WRONLY | O_TRUNC);
    
    strcat(abs_path, cwd);
    strcat(abs_path, "/perl");

    fd = wrapper_open((const char*)abs_path, O_WRONLY | O_TRUNC);
    (void)fd;

    wrapper_select();
    wrapper_poll();

    return 0;
}
