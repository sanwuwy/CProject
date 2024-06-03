#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "spdlog/spdlog.h"

pthread_mutex_t m_lock;

void* thread_function1(void* arg) {
    int i;
    pthread_mutex_lock(&m_lock);
    for (i = 0; i < 8; i++) {
        spdlog::info("thread_function1: {}", i);
        sleep(1);
    }
    pthread_mutex_unlock(&m_lock);
    return NULL;
}

void* thread_function2(void* arg) {
    int i;
    pthread_mutex_lock(&m_lock);
    for (i = 0; i < 8; i++) {
        spdlog::info("thread_function2: {}", i);
        sleep(1);
    }
    pthread_mutex_unlock(&m_lock);
    return NULL;
}

size_t getTid(pthread_t t) {
    uint64_t tid;
    // There is no pthread_threadid_np prior to Mac OS X 10.6, and it is not supported on any PPC,
    // including 10.6.8 Rosetta. __POWERPC__ is Apple-specific define encompassing ppc and ppc64.
    #ifdef MAC_OS_X_VERSION_MAX_ALLOWED
    {
        spdlog::info("getTid 0 MAC_OS_X_VERSION_MAX_ALLOWED={}, MAC_OS_X_VERSION_MIN_REQUIRED={}", MAC_OS_X_VERSION_MAX_ALLOWED, MAC_OS_X_VERSION_MIN_REQUIRED);
        #if (MAC_OS_X_VERSION_MAX_ALLOWED < 1060) || defined(__POWERPC__)
        tid = pthread_mach_thread_np(t);
        spdlog::info("getTid 1 MAC_OS_X_VERSION_MAX_ALLOWED={}, tid={}", MAC_OS_X_VERSION_MAX_ALLOWED, tid);
        #elif MAC_OS_X_VERSION_MIN_REQUIRED < 1060
        if (&pthread_threadid_np) {
            pthread_threadid_np(t, &tid);
            spdlog::info("getTid 2 MAC_OS_X_VERSION_MIN_REQUIRED={}, tid={}", MAC_OS_X_VERSION_MIN_REQUIRED, tid);
        } else {
            tid = pthread_mach_thread_np(t);
            spdlog::info("getTid 3 tid={}", tid);
        }
        #else
        pthread_threadid_np(t, &tid);
        spdlog::info("getTid 4 tid={}", tid);
        #endif
    }
    #else
    pthread_threadid_np(t, &tid);
    spdlog::info("getTid 5 tid={}", tid);
    #endif
    return static_cast<size_t>(tid);
}

int main(void) {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // change log pattern
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [thread %t] %v");

    spdlog::info("main: start");

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&m_lock, &attr);

    pthread_mutexattr_destroy(&attr);

    pthread_t mythread1;
    pthread_t mythread2;

    if (pthread_create(&mythread1, NULL, thread_function1, NULL)) {
        spdlog::error("error creating thread.");
        abort();
    }
    if (pthread_create(&mythread2, NULL, thread_function2, NULL)) {
        spdlog::error("error creating thread.");
        abort();
    }
    sleep(2);
    auto tid1 = getTid(mythread1);
    auto tid2 = getTid(mythread2);
    spdlog::info("{}: tid:{}", __FUNCTION__, tid1);
    spdlog::info("{}: tid:{}", __FUNCTION__, tid2);
    spdlog::info("main: Thread work done!");
    pthread_exit(NULL);
    exit(0);
}