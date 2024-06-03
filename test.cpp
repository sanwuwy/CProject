#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;


struct _jmethodID;
typedef struct _jmethodID* jmethodID;

int main() {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // change log pattern
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [thread %t] %v");

    // int flags = 193;

    // if ((flags & O_WRONLY) == O_WRONLY || (flags & O_RDWR) == O_RDWR) {
    //     LOG_DEBUG << "Yes";
    // } else {
    //     LOG_DEBUG << "No";
    // }

    long i = 0;
    jmethodID open = reinterpret_cast<jmethodID>(i);

    if (open == nullptr) {
        spdlog::info("Yes");
    } else {
        spdlog::info("No");
    }



    return 0;
}