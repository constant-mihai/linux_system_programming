/***************************************** 

* File Name : scheduling.c

* Creation Date : 18-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

#include <sched.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>



void wrapper_sched_yield() {
    while(1) {
        sched_yield();
    }
}

/**
 * ******************************************************
 *  Sets the processor niceness:
 *  -20 to 19
 * ******************************************************
**/
void wrapper_nice() {
    int ret, val;

    /* get current nice value */
    val = nice (0);

    /* we want a nice value of 10 */
    val = 10 - val;
    errno = 0;
    ret = nice (val);
    if (ret == -1 && errno != 0)
            perror ("nice");
    else
            printf ("nice value is now %d\n", ret);
}

/**
 * ******************************************************
 *  which: PRIO_PROCESS
 *         PRIO_PGRP
 *         PRIO_USER
 *
 *  who: PID
 *       0 means current process, group or user
 * ******************************************************
**/
void wrapper_getpriority(int which, int who) {
    int ret = getpriority(which, who);
    printf("nice value is %d\n", ret);
}

void wrapper_setpriority(int which, int who, int prio) {
    int ret = setpriority(which, who, prio);
    if (ret == -1) perror ("setpriority");
}


int main(int argc, char ** argv) {

}
