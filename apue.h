#ifndef _APUE_H
#define _APUE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE     4096




void err_sys(const char *, ...) __attribute__((noreturn));

#endif