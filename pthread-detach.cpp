#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "limlog/Log.h"

void* thread_function(void* arg) {
    int i;
    for (i = 0; i < 8; i++) {
        LOG_INFO << "Thread working: " << __FUNCTION__ << i;
        sleep(1);
    }
}

int main(void) {
    setLogLevel(limlog::LogLevel::DEBUG);

    pthread_t mythread;

    if (pthread_create(&mythread, NULL, thread_function, NULL)) {
        LOG_INFO << "error creating thread.";
        abort();
    }
    sleep(2);
    LOG_INFO << "tid: " << mythread << ", " << __FUNCTION__;
    LOG_INFO << "Thread work done!" << " " << __FUNCTION__;
    pthread_detach(mythread);
    exit(0);
}