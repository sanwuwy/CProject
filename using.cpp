#include <iostream>
using namespace std;

// 第一个命名空间
namespace first_space {
    void func() {
        cout << "Inside first_space" << endl;
    }
}  // namespace first_space

// 第二个命名空间
namespace second_space {
    void func() {
        cout << "Inside second_space" << endl;
    }
}  // namespace second_space

using namespace first_space;
int main() {
    // 调用第一个命名空间中的函数
    func();

    return 0;
}
