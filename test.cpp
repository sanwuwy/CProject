#include <iostream>
#include "limlog/Log.h"

using namespace std;


struct _jmethodID;
typedef struct _jmethodID* jmethodID;

int main() {
    setLogLevel(limlog::LogLevel::DEBUG);

    // int flags = 193;

    // if ((flags & O_WRONLY) == O_WRONLY || (flags & O_RDWR) == O_RDWR) {
    //     LOG_DEBUG << "Yes";
    // } else {
    //     LOG_DEBUG << "No";
    // }

    long i = 0;
    jmethodID open = reinterpret_cast<jmethodID>(i);

    if (open == nullptr) {
        LOG_DEBUG << "Yes";
    } else {
        LOG_DEBUG << "No";
    }



    return 0;
}