#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main() {
    string s = "hello world";
    assert(s.size() == s.length());
    printf("size: %d, length: %d\n", s.size(), s.length());
}