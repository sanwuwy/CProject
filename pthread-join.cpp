#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "limlog/Log.h"

void *thread_function(void *arg)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        LOG_INFO << "Thread working: " << __FUNCTION__ << i;
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    setLogLevel(limlog::LogLevel::DEBUG);

    pthread_t mythread;

    if (pthread_create(&mythread, NULL, thread_function, NULL))
    {
        LOG_INFO << "error creating thread.";
        abort();
    }
    if (pthread_join(mythread, NULL)) // 使主线程等待子线程执行完再继续执行
    {
        LOG_INFO << "error join thread.";
        abort();
    }
    LOG_INFO << "Thread done!" << __FUNCTION__;
    exit(0);
}
