#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief 共享内存之——mmap内存映射
 * https://www.jianshu.com/p/096e1b58c678
 */
int main(int argc, char* argv[]) {
    int fd, nread;
    struct stat sb;
    char* mapped;

    //打开文件
    if ((fd = open("test.txt", O_RDWR)) < 0) {
        perror("open");
    }

    //获取文件的属性
    if ((fstat(fd, &sb)) == -1) {
        perror("fstat");
    }

    //将文件映射至进程的地址空间
    if ((mapped = static_cast<char*>(mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == (void*)-1) {
        perror("mmap");
    }

    //修改一个字符，同步到磁盘文件
    mapped[2] = '2';
    if ((msync((void*)mapped, sb.st_size, MS_SYNC)) == -1) {
        perror("msync");

        //释放存储映射区
        if ((munmap((void*)mapped, sb.st_size)) == -1) {
            perror("munmap");
        }

        return 0;
    }
}