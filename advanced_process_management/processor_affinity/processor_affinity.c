/***************************************** 

* File Name : processor_affinity.c

* Creation Date : 18-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */


#define _GNU_SOURCE

#include <sched.h>
#include <error.h>
#include <stdio.h>

#if 0
typedef struct cpu_set_t;

size_t CPU_SETSIZE;

void CPU_SET (unsigned long cpu, cpu_set_t *set);
void CPU_CLR (unsigned long cpu, cpu_set_t *set);
int CPU_ISSET (unsigned long cpu, cpu_set_t *set);
void CPU_ZERO (cpu_set_t *set);

int sched_setaffinity (pid_t pid, size_t setsize,
                       const cpu_set_t *set);

int sched_getaffinity (pid_t pid, size_t setsize,
                       cpu_set_t *set);
#endif


/**
 * ******************************************************
 *  Checks if the cpu is set
 * ******************************************************
**/
void check_set_processors() {
    cpu_set_t set;
    int ret, i;

    CPU_ZERO (&set);
    ret = sched_getaffinity (0, sizeof (cpu_set_t), &set);
    if (ret == -1)
            perror ("sched_getaffinity");

    for (i = 0; i < 8 /*nr of cores*/; i++) {
            int cpu;

            cpu = CPU_ISSET (i, &set);
            printf ("cpu=%i is %s\n", i,
                    cpu ? "set" : "unset");
    }
}

/**
 * ******************************************************
 *  Set the hard affinity for the process
 * ******************************************************
**/
void set_hard_affinity() {
    cpu_set_t set;
    int ret, i;

    CPU_ZERO (&set);        /* clear all CPUs */
    CPU_SET (0, &set);      /* allow CPU #0 */
    //CPU_CLR (1, &set);      /* disallow CPU #1 */
                              /* clearing is not necessary */
    ret = sched_setaffinity (0, sizeof (cpu_set_t), &set);
    if (ret == -1)
            perror ("sched_setaffinity");

    for (i = 0; i < 8 /*nr of cores*/; i++) {
            int cpu;

            cpu = CPU_ISSET (i, &set);
            printf ("cpu=%i is %s\n", i,
                    cpu ? "set" : "unset");
    }
}

int main(int argc, char ** argv) {
    check_set_processors();
    set_hard_affinity();
}
