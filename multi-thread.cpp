#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "limlog/Log.h"

pthread_mutex_t m_lock;

void* thread_function1(void* arg) {
    int i;
    pthread_mutex_lock(&m_lock);
    for (i = 0; i < 8; i++) {
        LOG_INFO << "thread_function1: " << i;
        sleep(1);
    }
    pthread_mutex_unlock(&m_lock);
    return NULL;
}

void* thread_function2(void* arg) {
    int i;
    pthread_mutex_lock(&m_lock);
    for (i = 0; i < 8; i++) {
        LOG_INFO << "thread_function2: " << i;
        sleep(1);
    }
    pthread_mutex_unlock(&m_lock);
    return NULL;
}

int main(void) {
    setLogLevel(limlog::LogLevel::DEBUG);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&m_lock, &attr);

    pthread_mutexattr_destroy(&attr);

    pthread_t mythread1;
    pthread_t mythread2;

    if (pthread_create(&mythread1, NULL, thread_function1, NULL)) {
        printf("error creating thread.");
        abort();
    }
    if (pthread_create(&mythread2, NULL, thread_function2, NULL)) {
        printf("error creating thread.");
        abort();
    }
    sleep(2);
    printf("%s: tid:%lu\n", __FUNCTION__, mythread1);
    printf("%s: tid:%lu\n", __FUNCTION__, mythread2);
    LOG_INFO << "main: Thread work done!";
    pthread_exit(NULL);
    exit(0);
}