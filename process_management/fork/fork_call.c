/***************************************** 

* File Name : fork_call.c

* Creation Date : 16-09-2018

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
#include <unistd.h>
#include <stdlib.h>

void parent_out() {
    printf("parent atexit() succeeded!\n");
}

void child_out() {
    printf("child atexit() succeeded!\n");
}


int main(int argc, char ** argv) {

    pid_t pid;

    pid = fork ();
    if (pid == -1) perror ("fork");
    else if (pid > 0) {
        printf("I am the parent of pid=%d.\n", pid);
        printf("My pid is =%d.\n", getpid());

        /* Will call the parent_out function when falling of the 
         * edge. */
        if (atexit(parent_out)) {
            fprintf(stderr, "atexit() failed!\n");
        }

        return 0;
    }
    else if (!pid) {
        /* the child ... */
        printf(" -- I am the the child of pid=%d.\n", getppid());
        printf(" -- My pid is =%d.\n", getpid());
        printf(" -- I list files:\n");
        char *const args[] = { "list_child", NULL };
        int ret;

        if (atexit(child_out)) {
            fprintf(stderr, "atexit() failed!\n");
        }
        /* If I uncomment the line below then , the program ends with
         * "atexit() succeeded." Otherwise the execv() flushes all 
         * registered functions with atexit(). */
        //exit(0);

        ret = execv ("/bin/ls", args);
        if (ret == -1) {
                perror ("execv");
                exit (EXIT_FAILURE);
        }

        /* Other things which are lost on exec() family commands:
         *  - Any pending signals are lost.
         *  - Any signals that the process is catching (see Chapter 10)
         *    are returned to their default behavior, as the signal handlers
         *    no longer exist in the process’s address space. 
         *  - Any memory locks (see Chapter 9) are dropped. 
         *  - Most thread attributes are returned to the default values. 
         *  - Most process statistics are reset.  
         *  - Anything related to the process’s memory address space, including any mapped files, 
         *    is cleared. 
         *  - Anything that exists solely in user space, including features of the C library,
         *    such as atexit() behavior, is cleared.  
         *
         * Some properties of the process, 
         * however, do not change. For example, the pid, parent pid, priority, and owning 
         * user and group all remain the same.  Normally, open files are inherited across an exec.
         * This means the newly executed program has full access to all of the files open in the 
         * original process, assuming it knows the file descriptor values. However, this is
         * often not the desired behavior. 
         *
         * The usual practice is to close files before the exec, 
         * although it is also possible to instruct the kernel to 
         * do so automatically via fcntl().  
         */

        return 0;
    }

}
