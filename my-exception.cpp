#include <exception>
#include <iostream>
using namespace std;

struct MyException : public exception {
    const char* what() const throw() { // throw() 是异常规格说明符。括号内写该函数可抛出的异常类型，不填的话就是不抛出任何异常
        return "C++ Exception";
    }
};

int main() {
    try {
        throw MyException();
    } catch (MyException& e) {
        std::cout << "MyException caught" << std::endl;
        std::cout << e.what() << std::endl;
    } catch (std::exception& e) {
        //其他的错误
    }
}
