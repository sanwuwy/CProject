#include <functional>
#include <iostream>
#include <thread>
#include "limlog/Log.h"

using namespace ::std;

function<void()> mainCmdFunc = nullptr;

static void CmdToMainThread(function<void()> cmdFunc) {
    mainCmdFunc = cmdFunc;
    while (mainCmdFunc != nullptr) {
        LOG_INFO << "执行中。。。";
    }
}

template <typename F>
static void ExcuteLambdaFunc(F cmdFunc) {
    cmdFunc();
}

static void childThread() {
    LOG_INFO << "老子是子线程";
    while (true) {
        CmdToMainThread([&]() {
             LOG_INFO << "老子是从子线程来的，牛皮不？";
        });
        LOG_INFO << "老子从主线程回来了";
        break;
    }
}

int main() {
    setLogLevel(limlog::LogLevel::DEBUG);
    LOG_INFO << "主线程";
    thread* child_thread = new thread(childThread);

    while (true) {
        if (mainCmdFunc != nullptr) {
            LOG_INFO << "有子线程小弟有想在我家放点东西的想法,没得问题";
            ExcuteLambdaFunc(mainCmdFunc);
            mainCmdFunc = nullptr;
            LOG_INFO << "子线程小弟放完赶紧滚";
        }
    }
}
