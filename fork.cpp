#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t pid, pid2, wpid;
    int flg = 0;

    pid = fork();
    // pid2 = fork();

    printf("main 1, mypid = %d, pid= %d\n", getpid(), pid);

    if (pid == -1) {
        perror("fork error");
        exit(1);
    } else if (pid == 0) {  //son
        printf("I'm process child, pid = %d\n", getpid());
        // sleep(5);
        // exit(4);
    } else {  //parent
        do {
            wpid = waitpid(pid, NULL, 0); // 等待子进程执行完
            // int status;
            // wpid = wait(&status);
            //wpid = wait(NULL);
            printf("---wpid = %d--------%d\n", wpid, flg++);
            if (wpid == 0) {
                printf("NO child exited\n");
                sleep(1);
            }
        } while (wpid == 0);  //子进程不可回收

        if (wpid == pid) {  //回收了指定子进程
            printf(
                "I'm parent, I catched child process,"
                "pid = %d\n",
                wpid);
        } else {
            printf("other...\n");
        }
    }

    printf("main end...\n");

    return 0;
}
