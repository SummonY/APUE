#include "apue.h"

int main(int argc, const char *argv[])
{
    int i;
    for (i = 0; argv[i] != NULL; ++i)
        printf("argv[%d] = %s\n", i, argv[i]);
    
    for (i = 0; i < argc; ++i)
        printf("argv[%d] = %s\n", i, argv[i]);
    exit(0);
}