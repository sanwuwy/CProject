#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define XCC_UTIL_TEMP_FAILURE_RETRY(exp) ({         \
            __typeof__(exp) _rc;                    \
            do {                                    \
                errno = 0;                          \
                _rc = (exp);                        \
            } while (_rc == -1 && errno == EINTR);  \
            _rc; })


int my_fork() {
    pid_t pid = fork();
    printf("main 2, mypid = %d, pid = %d\n", getpid(), pid);

    if (pid == -1) {
        perror("fork error");
        exit(1);
    } else if (pid == 0) {  //son
        printf("I'm process child, pid = %d\n", getpid());
        sleep(3);
        exit(4);
    } else {  //parent
        printf("I'm process parent, pid = %d\n", getpid());
        return pid;
    }
}

int main(void) {
    pid_t cpid, wpid;
    int flg = 0;

    printf("main 1, mypid = %d\n", getpid());

    cpid = my_fork();

    printf("main, cpid = %d\n", cpid);


    if (cpid == -1) {
        printf("fork fail");
    }

    int status = 0;
    wpid = XCC_UTIL_TEMP_FAILURE_RETRY(waitpid(cpid, NULL, 0x40000000));

    if (wpid == -1) {
        printf("wait fail");
    }

    printf("main end...\n");

    return 0;
}
