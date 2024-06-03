#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include "spdlog/spdlog.h"

void *thread_function(void *arg)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        spdlog::info("Thread working: {0} {1}", __FUNCTION__, i);
        sleep(1);
    }
    return NULL;
}

int main(void)
{
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // change log pattern
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [thread %t] %v");

    pthread_t mythread;

    if (pthread_create(&mythread, NULL, thread_function, NULL))
    {
        spdlog::info("error creating thread.");
        abort();
    }
    if (pthread_join(mythread, NULL)) // 使主线程等待子线程执行完再继续执行
    {
        spdlog::info("error join thread.");
        abort();
    }
    spdlog::info("Thread done! {}", __FUNCTION__);
    exit(0);
}
