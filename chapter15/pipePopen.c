#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PAGER       "${PAGER:-more}"

// Pointer to array allocated at run_time
static pid_t *childpid = NULL;

// From open_max()
static int maxfd;

FILE *popen(const char *cmdstring, const char *type);
int pclose(FILE *fp);


int main(int argc, const char *argv[])
{
    char line[MAXLINE];
    FILE *fpin, *fpout;

    if (argc != 2) {
        err_quit("usage: exec <pathname>");
    }
    if ((fpin = fopen(argv[1], "r")) == NULL) {
        err_sys("Can't open %s\n", argv[1]);
    }
    if ((fpout = popen(PAGER, "w")) == NULL) {
        err_sys("popen error");
    }

    // copy argv[1] to pager
    while (fgets(line, MAXLINE, fpin) != NULL) {
        if (fputs(line, fpout) == EOF) {
            err_sys("fputs error to pipe");
        }
    }

    if (ferror(fpin)) {
        err_sys("fgets error");
    }
    if (pclose(fpout) == -1) {
        err_sys("pclose error");
    }
    exit(0);
}


/*
 * custom define popen function.
 */
FILE *popen(const char *cmdstring, const char *type)
{
    printf("custom define popen function.\n");
    int i;
    int pfd[2];
    pid_t pid;
    FILE *fp;

    // only allow "r" or "w"
    if ((type[0] != 'r' && type[0] != 'w') || type[1] != 0) {
        errno = EINVAL;
        return (NULL);
    }

    if (childpid == NULL) {
        // allocated zeroed out array for child pids
        maxfd = open_max();
        if ((childpid = calloc(maxfd, sizeof(pid_t))) == NULL) {
            return (NULL);
        }
    }
    
    if (pipe(pfd) < 0) {
        return (NULL);
    }
    if (pfd[0] >= maxfd || pfd[1] >= maxfd) {
        close(pfd[0]);
        close(pfd[1]);
        errno = EMFILE;
        return (NULL);
    }

    if ((pid = fork()) < 0) {
        return NULL;
    } else if (pid == 0) {
        if (*type == 'r') {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO) {
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        } else {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO) {
                dup2(pfd[0], STDIN_FILENO);
                close(pfd[0]);
            }
        }

        // close all descriptors in childpid[]
        for (i = 0; i < maxfd; ++i) {
            if (childpid[i] > 0) {
                close(i);
            }
        }

        execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
        _exit(127);
    }

    // parent continus
    if (*type == 'r') {
        close(pfd[1]);
        if ((fp = fdopen(pfd[0], type)) == NULL) {
            return (NULL);
        } else {
            close(pfd[0]);
            if ((fp = fdopen(pfd[1], type)) == NULL) {
                return (NULL);
            }
        }
    }
    printf("fileno(fp) = %d\n", fileno(fp));
    childpid[fileno(fp)] = pid;         // error: greater than openmax
    return (fp);
}


/*
 * custom define pclose function.
 */
int pclose(FILE *fp)
{
    printf("custom define pclose function.\n");
    int fd, stat;
    pid_t pid;
    
    if (childpid == NULL) {
        errno = EINVAL;
        return (-1);
    } 

    fd = fileno(fp);
    if (fd > maxfd) {
        errno = EINVAL;
        return (-1);
    }
    if ((pid = childpid[fd]) == 0) {
        errno = EINVAL;
        return (-1);
    }

    childpid[fd] = 0;
    if (fclose(fp) == EOF) {
        return (-1);
    }

    while (waitpid(pid, &stat, 0) < 0) {
        if (errno != EINTR) {
            return (-1);
        }
    }
    return (stat);
}
