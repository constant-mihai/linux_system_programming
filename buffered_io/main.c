/***************************************** 

* File Name : main.c

* Creation Date : 14-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License :

******************************************/

/**
 *  Purpose:
 *
 */

/* fcloseall() */
#define _GNU_SOURCE

/* open(), creat(), lseek(), ftruncate */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>

/**
 * ******************************************************
 *  Fopen wrapper
 * ******************************************************
**/
void wrapper_fopen() {
    FILE *stream = NULL;

    stream = fopen("/etc/manifest", "r");
    if (!stream) perror("fopen");

    int close = fclose(stream);
    (void)close;
}

void wrapper_fdopen() {
    FILE *stream = NULL;
    int fd, close;

    fd = open("/tmp/test.txt", O_RDONLY);
    if (fd == -1) perror ("open:");

    stream = fdopen(fd, "r");
    if (!stream) perror("fopen");

    close = fclose(stream);
    (void)close;
}

/**
 * ******************************************************
 *  fgetc wrapper
 * ******************************************************
**/
void wrapper_getc() {
    FILE *stream = NULL;
    int c;

    c = fgetc(stream);

    if (c == EOF) perror("getc:");
    else printf("c=%c'n", (char) c);

    int ret = ungetc(c, stream);
    (void)ret;
}


/**
 * ******************************************************
 *  fgets() wrapper
 * ******************************************************
**/
void wrapper_fgets() {
    int line_max = 100;
    char buf[line_max];
    FILE *stream = NULL;

    if (!fgets (buf, line_max, stream)) {
        perror("fgets:");
    }
}

/**
 * ******************************************************
 *  Reads a line of data. Also stops reading at delimiter 
 *  'd'
 * ******************************************************
**/
void fgets_substitute(int n, char d, char* str, FILE *stream) {
    char *s;
    int c;

    s = str;

    while (--n > 0 && (c = fgetc(stream)) != EOF && (*s++ = c) != d)
        ;

    if (c == d)
        *--s = '\0';
    else
        *s ='\0' ;
}


/**
 * ******************************************************
 *  Fread() wrapper
 * ******************************************************
**/
void wrapper_fread() {
    FILE *stream = NULL;
    size_t size = 100;
    char buf[(int)size];
    size_t  nr = 1;

    stream = fopen("/etc/manifest", "r");
    if (!stream) perror("fopen");

    size_t ret = fread(buf, size, nr, stream);

    if (ret == 0) perror("fread:");

    int close = fclose(stream);
    (void)close;
}


/**
 * ******************************************************
 *  Wrappers for writing
 * ******************************************************
**/
void wrapper_fputc(FILE *stream) {
    if (fputc('p', stream) == EOF) {
    }
}

void wrapper_fputs() {
    FILE *stream;

    stream = fopen("/tmp/test.txt", "a");
    if (!stream) {
        perror("fopen:");
    }

    if (fputs("The ship is made of wood.\n", stream) == EOF) perror("fputs");

    if (fclose(stream) == EOF) perror("fclose:");
}

void wrapper_fwrite(void *buf, size_t size, size_t nr, FILE *stream) {
    size_t ret = fwrite(buf, size, nr, stream);
    (void) ret;
}


#if 0
#include <stdio.h>
int fseek (FILE *stream, long offset, int whence);
#endif

#if 0
#include <stdio.h>
int fsetpos (FILE *stream, fpos_t *pos);
#endif

#if 0
#include <stdio.h>
void rewind (FILE *stream);o

errno = 0;
rewind (stream);
if (errno)
        /* error */
#endif


#if 0
#include <stdio.h>
long ftell (FILE *stream);
#endif

#if 0
#include <stdioh.h>
int fgetpos (FILE *stream, fpos_t *pos);
#endif


#if 0
#include <stdio.h>
int fflush (FILE *stream);
#endif


#if 0
#include <stdio.h>
int ferror (FILE *stream);
#endif

#if 0
#include <stdio.h>

int feof (FILE *stream);
#endif

#if 0
#include <stdio.h>

void clearerr (FILE *stream);
#endif


#if 0
#include <stdio.h>

int fileno (FILE *stream);
#endif

#if 0
#include <stdio.h>

int setvbuf (FILE *stream, char *buf, int mode, size_t size);
#endif


int main (void)
{
        FILE *in, *out;
        struct pirate {
                char            name[100]; /* real name */
                unsigned long   booty;     /* in pounds sterling */
                unsigned int    beard_len; /* in inches */
        } p, blackbeard = { "Edward Teach", 950, 48 };

        out = fopen ("data", "w");
        if (!out) {
                perror ("fopen");
                return 1;
        }

        if (!fwrite (&blackbeard, sizeof (struct pirate), 1, out)) {
                perror ("fwrite");
                return 1;
        }

        if (fclose (out)) {
                perror ("fclose");
                return 1;
        }

        in = fopen ("data", "r");
        if (!in) {
                perror ("fopen");
                return 1;
        }

        if (!fread (&p, sizeof (struct pirate), 1, in)) {
                perror ("fread");
                return 1;
        }

        if (fclose (in)) {
                perror ("fclose");
                return 1;
        }

        printf ("name=\"%s\" booty=%lu beard_len=%u\n",
                p.name, p.booty, p.beard_len);

        return 0;
}
