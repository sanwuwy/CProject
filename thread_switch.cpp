#include <functional>
#include <iostream>
#include <thread>
#include "spdlog/spdlog.h"

using namespace ::std;

function<void()> mainCmdFunc = nullptr;

static void CmdToMainThread(function<void()> cmdFunc) {
    mainCmdFunc = cmdFunc;
    while (mainCmdFunc != nullptr) {
        spdlog::info("子线程：执行中。。。");
    }
}

template <typename F>
static void ExcuteLambdaFunc(F cmdFunc) {
    cmdFunc();
}

static void childThread() {
    spdlog::info("老子是子线程");
    while (true) {
        CmdToMainThread([&]() {
             spdlog::info("老子是从子线程来的，牛皮不？");
        });
        spdlog::info("老子从主线程回来了");
        break;
    }
}

int main() {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    // change log pattern
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%L%$] [thread %t] %v");

    spdlog::info("主线程");
    thread* child_thread = new thread(childThread);

    while (true) {
        spdlog::info("主线程：执行中。。。");
        if (mainCmdFunc != nullptr) {
            spdlog::info("有子线程小弟想在我家放点东西的想法,没得问题");
            ExcuteLambdaFunc(mainCmdFunc);
            mainCmdFunc = nullptr;
            spdlog::info("子线程小弟放完赶紧滚");
            break;
        }
    }

    sleep(1);
    return 0;
}
