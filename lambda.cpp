// #include <iostream>
// using namespace std;

// int main()
// {
//     int i = 1024, j = 2048;

//     cout << "i:" << &i << endl;
//     cout << "j:" << &j << endl;

//     auto fun1 = [=, &i]{ // 默认拷贝外部所有变量，但引用变量 i
//         cout << "i:" << &i << endl;
//         cout << "j:" << &j << endl;
//     };
//     fun1();
// }

// #include <iostream>
// using namespace std;

// int main()
// {
//     int i = 1024, j = 2048;

//     cout << "outside i value:" << i << " addr:" << &i << endl;

//     auto fun1 = [i]{
//         cout << "inside  i value:" << i << " addr:" << &i << endl;
//         // cout << j << endl; // j 未捕获
//     };
//     fun1();
// }

#include <iostream>
using namespace std;

class test {
   public:
    void hello() {
        cout << "test hello!n" << endl;
    };
    void lambda() {
        auto fun = [this] {  // 捕获了 this 指针
            this->hello();   // 这里 this 调用的就是 class test 的对象了
        };
        fun();
    }
};

int main() {
    test t;
    t.lambda();
}
