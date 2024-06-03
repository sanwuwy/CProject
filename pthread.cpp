#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include "limlog/Log.h"

using namespace std;

#define NUM_THREADS 5

void* PrintHello(void* threadid) {
    // 对传入的参数进行强制类型转换，由无类型指针变为整形数指针，然后再读取
    int tid = *((int*)threadid);
    LOG_INFO << "Hello Runoob! 线程 ID, " << tid;
    pthread_exit(NULL);
}

int main() {
    setLogLevel(limlog::LogLevel::DEBUG);

    pthread_t threads[NUM_THREADS];
    int indexes[NUM_THREADS];  // 用数组来保存i的值
    int rc;
    int i;
    for (i = 0; i < NUM_THREADS; i++) {
        LOG_INFO << "main() : 创建线程, " << i;
        indexes[i] = i;  //先保存i的值
        // 传入的时候必须强制转换为void* 类型，即无类型指针
        rc = pthread_create(&threads[i], NULL,
                            PrintHello, (void*)&(indexes[i]));
        if (rc) {
            LOG_INFO << "Error:无法创建线程," << rc;
            exit(-1);
        }
    }
    pthread_exit(NULL);  // 等待所有子线程执行完再退出
}
