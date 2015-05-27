#include "apue.h"
#include <sys/shm.h>

#define ARRAY_SIZE      40000
#define MALLOC_SIZE     100000
#define SHM_SIZE        100000
#define SHM_MODE        0600

char array[ARRAY_SIZE];

int main(void)
{
    int shmid;
    char *ptr, *shmptr;

    printf("array[] from %p to %p\n", (void *)&array[0], (void *)&array[ARRAY_SIZE]);
    printf("stack around %p\n", (void *)&shmid);

    // 分配内存空间
    if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
        err_sys("malloc error");
    }
    printf("malloced from %p to %p\n", (void *)ptr, (void *)ptr+MALLOC_SIZE);

    // 获取共享存储标识符
    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
        err_sys("shmget error");
    }

    // 将共享存储段连接到其地址空间
    if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1) {
        err_sys("shmat error");
    }
    printf("shared memory attached from %p to %p\n", (void *)shmptr, (void *)shmptr+SHM_SIZE);

    // 对共享存储段执行操作
    if (shmctl(shmid, IPC_PRIVATE, 0) < 0) {
        err_sys("shmctl error");
    }
    
    exit(0);
}
