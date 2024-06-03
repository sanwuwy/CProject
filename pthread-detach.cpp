#include <iostream>
#include <pthread.h>
#include <unistd.h> // for sleep
#include <spdlog/spdlog.h>
#include <thread> // for std::this_thread::get_id

void* thread_function(void* arg) {
    int i;
    for (i = 0; i < 8; i++) {
        spdlog::info("Thread working: {0} {1}", __FUNCTION__, i);
        sleep(1);
    }
    return nullptr; // 确保返回一个值
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

    pthread_t mythread;

    if (pthread_create(&mythread, NULL, thread_function, NULL)) {
        spdlog::info("error creating thread.");
        abort();
    }
    sleep(2);

    // 获取当前线程ID
    auto thread_id = getTid(mythread);

    spdlog::info("tid: {}, {}", thread_id, __FUNCTION__);
    spdlog::info("Thread work done! {}", __FUNCTION__);
    pthread_detach(mythread);
    exit(0);
}
