/***************************************** 

* File Name : multiplexed_io.c

* Creation Date : 14-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */


#include "multiplexed_io.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <poll.h>

#define TIMEOUT 5
#define BUF_LEN 1024

/**
 * ******************************************************
 * @brief 
 * 1. Tell me when any of these fds become ready for i/o
 * 2. Sleep until one or more fds are ready
 * 3. Woken up! What is ready?
 * 4. Handle all fds ready for i/o without blocking
 * 5. go back to 1.
 * ******************************************************
**/
int wrapper_select() {
    int ret;
    struct timeval tv;
    fd_set readfds;

    /* Wait on stdin for input. */
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    /* Wait up to five seconds */
    /* !!!! 
     * This needs to be reinitialized on subsequent calls.
     * pselect() leaves the structure alone. 
     * !!!! */
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    /* Now block */
    ret = select(STDIN_FILENO + 1,
                &readfds,
                0,
                0,
                &tv);

    if (ret == -1) {
        perror("select:");
        return 1;
    } else if (!ret) {
        printf("%d seconds elapsed.\n", TIMEOUT);
        /* The time structure is modified by the select() */
        printf("tv_sec %d.\n", (int)tv.tv_sec);
        printf("tv_usec %d.\n", (int)tv.tv_usec);
        return 0;
    }

    /* 
     * Is our fd ready to read?
     * Only one provided, so yes.
     * */
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buf[BUF_LEN + 1];
        int len;

        /* guaranteed to not block */
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            perror("read:");
            return 1;
        }

        if (len) {
            buf[len] = '\0';
            printf("read: %s.\n", buf);
            /* The time structure is modified by the select() */
            printf("tv_sec %d.\n", (int)tv.tv_sec);
            printf("tv_usec %d.\n", (int)tv.tv_usec);
        }

        return 0;
    }

    fprintf(stderr, "This should not happen!\n");
    return 1;
}

/*
 * There are three differences between pselect() and select():
 *  - pselect() uses the timespec structure, not the timeval structure, for its 
 *    timeout parameter. The timespec structure uses seconds and nanoseconds,
 *    not seconds and microseconds, providing theoretically superior timeout 
 *    resolution. In practice, however, neither call reliably provides even
 *    microsecond resolution.
 *  - A call to pselect() does not modify the timeout parameter.
 *    Consequently, this parameter does not need to be
 *    reinitialized on subsequent invocations.
 *  - The select() system call does not have the sigmask parameter.
 *    With respect to signals, when this parameter is set to NULL, pselect()
 *    behaves like select().
 *
 *    The motivation for pselect() are the signals. The app can block indefinetly
 *    if the signall arives between the check for the global signal flag
 *    and the call to select().
 *    sigmask in pselect() blocks signals. Blocked signals are not handled
 *    until they are unblocked. Once pselec() returns, the kernel restors the
 *    old signal mask.
 */



int wrapper_poll() {
    int ret;

    struct pollfd fds[2];
    /* watch stdin for input */
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    /* watch stdout for ability to write (almost always true) */
    //fds[1].fd = STDOUT_FILENO;
    //fds[1].events = POLLOUT;
    /* All set, block! */
    ret = poll (fds, 1, TIMEOUT * 1000);
    if (ret == -1) {
        perror ("poll");
        return 1;
    }
    if (!ret) {
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }
    if (fds[0].revents & POLLIN) {
        char buf[BUF_LEN + 1];
        int len;

        /* guaranteed to not block */
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            perror("read:");
            return 1;
        }

        if (len) {
            buf[len] = '\0';
            printf("read: %s.\n", buf);
        }
        printf ("stdin is readable\n");
    }
    if (fds[1].revents & POLLOUT)
        printf ("stdout is writable\n");
    return 0;
}
