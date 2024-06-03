#include "comm.h"

int main() {
    int shmid = creat_shm(4096);
    printf("shmid = %d\n", shmid);
    sleep(3);
    char* buf = static_cast<char*>(shmat(shmid, NULL, 0));  //挂接//shmat第二个参数为空，表示系统分配地址空间，返回一个虚拟地址空间。
    printf("buf = %p\n", buf);
    int i = 0;

    while (1) {
        buf[i] = 'A' + i % 26;
        i++;
        buf[i] = 0;
        sleep(2);
    }
    shmdt(buf);  //去挂接。
    sleep(3);
    destroy_shm(shmid);

    return 0;
}
