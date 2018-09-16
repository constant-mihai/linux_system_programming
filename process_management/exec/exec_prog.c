/***************************************** 

* File Name : exec_prog.c

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
#include <errno.h>
#include <unistd.h>


int main(int argc, char ** argv) {

    char * const args[] = { "program_name_here", "/hooks.txt", NULL };
    int ret;

    ret = execv ("/usr/bin/vi", args);
    if (ret == -1) perror ("execvp");

    return 0;
}
