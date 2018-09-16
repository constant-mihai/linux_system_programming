/***************************************** 

* File Name : wait_call.c

* Creation Date : 16-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

#include <stdlib.h>
#include <signal.h>


#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * ******************************************************
 *  system(): A short hand function for fork() -> exec()
 * ******************************************************
**/
void wrapper_system() {
    /*
     * During execution of the command, SIGCHLD is blocked, 
     * and SIGINT and SIGQUIT are ignored. Ignoring SIGINT 
     * and SIGQUIT has several implications,
     * particularly if system() is invoked inside a loop. 
     * If calling system() from within a loop, you should 
     * ensure that the program properly checks the exit status of the child.
     * For example:
     */
    do {
        int ret;

        ret = system ("pidof rudderd");
        if (WIFSIGNALED (ret) &&
            (WTERMSIG (ret) == SIGINT ||
             WTERMSIG (ret) == SIGQUIT))
                break; /* or otherwise handle */
    } while (1);
}

/**
 * ******************************************************
 *  The waitid(), more options than waitpid().
 * ******************************************************
**/
void wrapper_waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options) {
    int ret = waitid (idtype, id, infop, options);

    (void)ret;
}

int main(int argc, char ** argv) {
    int status;
    pid_t pid;

    if (!fork ())
            return 1;

    //pid = wait (&status); // Weaksauce

    pid = waitpid(-1, &status, 0); // Better
    /*
     * < −1
     * Wait for any child process whose process group ID 
     * is equal to the absolute value of this value. 
     * For example, passing −500 waits for any process in process group 500.
     *
     * −1
     * Wait for any child process. This is the same behavior as wait().
     *
     * 0
     * Wait for any child process that belongs to the same process 
     * group as the calling process.
     *
     * > 0
     * Wait for any child process whose pid is exactly the value provided.
     * For example, passing 500 waits for the child process with pid 500.
     */

    /*
     * The options parameter is a binary OR of zero or more of the following 
     * options:
     * 
     *  - WNOHANG
     * Do not block, but return immediately if no matching child process has 
     * already terminated (or stopped or continued).
     *  
     *  - WUNTRACED
     * If set, the WIFSTOPPED bit in the returned status parameter is set,
     * even if the calling process is not tracing the child process. 
     * This flag allows for the implementation of more general job control,
     * as in a shell.
     *
     *  - WCONTINUED
     * If set, the WIFCONTINUED bit in the returned status parameter is
     * set even if the calling process is not tracing the child process.
     * As with WUNTRACED, this flag is useful for implementing a shell.
     */
    if (pid == -1)
            perror ("wait");

    printf ("pid=%d\n", pid);

    if (WIFEXITED (status))
            printf ("Normal termination with exit status=%d\n",
                    WEXITSTATUS (status));

    if (WIFSIGNALED (status))
            printf ("Killed by signal=%d%s\n",
                    WTERMSIG (status),
                    WCOREDUMP (status) ? " (dumped core)" : "");

    if (WIFSTOPPED (status))
            printf ("Stopped by signal=%d\n",
                    WSTOPSIG (status));

    if (WIFCONTINUED (status))
            printf ("Continued\n");

   return 0;
}
