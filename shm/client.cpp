#include "comm.h"

int main() {
    int shmid = get_shm();
    sleep(3);
    char* buf = static_cast<char*>(shmat(shmid, NULL, 0));  //新的进程挂接到那块共享内存空间，能看到共享内存中的东西。

    while (1) {
        printf("%s\n", buf);
        sleep(2);
    }
    shmdt(buf);
    return 0;
}
