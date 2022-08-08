/* 
   Demonstrate the use of a pipe to connect two filters. We use fork()
   to create two children. The first one execs ls(1), which writes to
   the pipe, the second execs wc(1) to read from the pipe.
*/
#include <sys/wait.h>
//#include "tlpi_hdr.h
#include "unistd.h"
#include <stdlib.h>
#define write_end 1
#define read_end 0

int main(int argc, char *argv[])
{
    int pfd[2];                                     /* Pipe file descriptors */

    if (pipe(pfd) == -1)                            /* Create pipe */
        exit(0);

    switch (fork()) {
    case -1:
        exit(0);

    case 0:             /* First child: exec 'ls' to write to pipe */
        if (close(pfd[0]) == -1)                    /* Read end is unused */
            exit(0);

        /* Duplicate stdout on write end of pipe; close duplicated descriptor */

        if (pfd[1] != STDOUT_FILENO) {              /* Defensive check */
            if (dup2(pfd[1], STDOUT_FILENO) == -1)
                exit(0);
            if (close(pfd[1]) == -1)
                exit(0);
        }

        execlp("./email_filter", NULL, NULL);          /* Writes to pipe */
        exit(0);

    default:            /* Parent falls through to create next child */
        break;
    }

    switch (fork()) {
    case -1:
        exit(0);

    case 0:             /* Second child: exec 'wc' to read from pipe */
        if (close(pfd[1]) == -1)                    /* Write end is unused */
            exit(0);

        /* Duplicate stdin on read end of pipe; close duplicated descriptor */

        if (pfd[0] != STDIN_FILENO) {               /* Defensive check */
            if (dup2(pfd[0], STDIN_FILENO) == -1)
                exit(0);
            if (close(pfd[0]) == -1)
                exit(0);
        }

        execlp("./calendar_filter", NULL, NULL);
        exit(0);

    default: /* Parent falls through */
        break;
    }

    /* Parent closes unused file descriptors for pipe, and waits for children */

    if (close(pfd[0]) == -1)
        exit(0);
    if (close(pfd[1]) == -1)
        exit(0);
    if (wait(NULL) == -1)
        exit(0);
    if (wait(NULL) == -1)
        exit(0);

    exit(0);
}
