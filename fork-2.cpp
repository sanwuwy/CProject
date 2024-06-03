#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "spdlog/spdlog.h"

#define XCC_UTIL_TEMP_FAILURE_RETRY(exp) ({         \
            __typeof__(exp) _rc;                    \
            do {                                    \
                errno = 0;                          \
                _rc = (exp);                        \
            } while (_rc == -1 && errno == EINTR);  \
            _rc; })


int my_fork() {

    pid_t pid = fork();
    spdlog::info("main 2, mypid = {}, pid = {}", getpid(), pid);

    if (pid == -1) {
        spdlog::error("fork error");
        exit(1);
    } else if (pid == 0) {  //son
        spdlog::error("I'm process child, pid = {}", getpid());
        sleep(3);
        exit(4);
    } else {  //parent
        spdlog::info("I'm process parent, pid = {}", getpid());
        return pid;
    }
}

int main(void) {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // change log pattern
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [thread %t] %v");

    pid_t cpid, wpid;
    int flg = 0;

    spdlog::info("main 1, mypid = {}", getpid());

    cpid = my_fork();

    spdlog::info("main 2, cpid = {}", cpid);


    if (cpid == -1) {
        spdlog::error("fork fail");
    }

    int status = 0;
    wpid = XCC_UTIL_TEMP_FAILURE_RETRY(waitpid(cpid, NULL, 0));

    if (wpid == -1) {
        spdlog::error("wait fail");
    }

    spdlog::info("main end...\n");

    return 0;
}
