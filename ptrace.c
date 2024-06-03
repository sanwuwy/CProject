#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>

int main()
{   pid_t child;
    struct user_regs_struct regs;

    child = fork();  // 创建一个子进程
    if(child == 0) { // 子进程
        ptrace(PTRACE_TRACEME, 0, NULL, NULL); // 表示当前进程进入被追踪状态
        execl("/bin/ls", "ls", NULL);          // 执行 `/bin/ls` 程序
    }
    else { // 父进程
        wait(NULL); // 等待子进程发送一个 SIGCHLD 信号
        ptrace(PTRACE_GETREGS, child, NULL, &regs); // 获取子进程的各个寄存器的值
        printf("Register: rdi[%ld], rsi[%ld], rdx[%ld], rax[%ld], orig_rax[%ld]\n",
                regs.rdi, regs.rsi, regs.rdx, regs.rax, regs.orig_rax); // 打印寄存器的值
        ptrace(PTRACE_CONT, child, NULL, NULL); // 继续运行子进程
        sleep(1);
    }
    return 0;
}
