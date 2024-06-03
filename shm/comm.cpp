#include "comm.h"

int common(size_t size, int flags) {
    key_t key = ftok(PATHNAME, PROJ_ID);
    if (key < 0) {
        perror("ftok");
        return -1;
    }
    int shmid = shmget(key, size, flags);  //得到共享内存的标识符shmid。
    if (shmid < 0) {
        perror("shmget");
        return -2;
    }
    return shmid;
}

int creat_shm(size_t size) {
    return common(size, IPC_CREAT | IPC_EXCL | 0x666);
}

int get_shm() {
    return common(0, IPC_CREAT);
}

int destroy_shm(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) < 0) {
        perror("shmctl");
        return -3;
    }
    return 0;
}
