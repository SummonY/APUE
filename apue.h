#ifndef _APUE_H
#define _APUE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/termios.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <time.h>

#define MAXLINE     4096


/*
 * Prototypes for our own function
 */
long open_max(void);


void err_msg(const char *, ...);
void err_quit(const char *, ...) __attribute__((noreturn));
void err_sys(const char *, ...) __attribute__((noreturn));
void err_ret(const char *, ...);
void err_exit(int ,const char *, ...) __attribute__((noreturn));




#endif
