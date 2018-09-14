/****************************************** 

* File Name : thread_saftey.h

* Creation Date : 14-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef _THREAD_SAFTEY_H_
#define _THREAD_SAFTEY_H_

/**
 * ******************************************************
 * #include <stdio.h>

void flockfile (FILE *stream);
 * ******************************************************
**/

/**
 * ******************************************************
 * #include <stdio.h>

void funlockfile (FILE *stream);
 * ******************************************************
**/


/**
 * ******************************************************
 *  #include <stdio.h>

int ftrylockfile (FILE *stream);
 * ******************************************************
**/


/**
 * ******************************************************
 * 
flockfile (stream);

fputs ("List of treasure:\n", stream);
fputs ("    (1) 500 gold coins\n", stream);
fputs ("    (2) Wonderfully ornate dishware\n", stream);

funlockfile (stream);
 * ******************************************************
**/


/**
 * ******************************************************
 * 
 * #define _GNU_SOURCE

#include <stdio.h>

int fgetc_unlocked (FILE *stream);
char *fgets_unlocked (char *str, int size, FILE *stream);
size_t fread_unlocked (void *buf, size_t size, size_t nr,
                       FILE *stream);
int fputc_unlocked (int c, FILE *stream);
int fputs_unlocked (const char *str, FILE *stream);
size_t fwrite_unlocked (void *buf, size_t size, size_t nr,
                        FILE *stream);
int fflush_unlocked (FILE *stream);
int feof_unlocked (FILE *stream);
int ferror_unlocked (FILE *stream);
int fileno_unlocked (FILE *stream);
void clearerr_unlocked (FILE *stream);
 * ******************************************************
**/


#endif
