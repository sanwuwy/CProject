#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "spdlog/spdlog.h"

int main(void) {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // change log pattern
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [thread %t] %v");

    pid_t pid, pid2, wpid;
    int flg = 0;

    pid = fork();
    // pid2 = fork();

    spdlog::info("main 1, mypid = {}, pid= {}", getpid(), pid);

    if (pid == -1) {
        spdlog::error("fork error");
        exit(1);
    } else if (pid == 0) {  //son
        spdlog::info("I'm process child, pid = {}", getpid());
        sleep(5);
        // exit(4);
    } else {  //parent
        do {
            spdlog::info("wait for sub process");
            wpid = waitpid(pid, NULL, 0); // 等待子进程执行完
            // int status;
            // wpid = wait(&status);
            //wpid = wait(NULL);
            spdlog::info("---wpid = {}--------{}", wpid, flg++);
            if (wpid == 0) {
                spdlog::warn("NO child exited\n");
                sleep(1);
            }
        } while (wpid == 0);  //子进程不可回收

        if (wpid == pid) {  //回收了指定子进程
            spdlog::info("I'm parent, I catched child process, pid = {}", wpid);
        } else {
            spdlog::info("other...\n");
        }
    }

    spdlog::info("main end...\n");

    return 0;
}
