#include <iostream>
#include "Minilog.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    minilog::initialize(minilog::GuaranteedLogger(), "/Users/wocaibujiaoquanmei/Minilog/", "Minilog", 1);
    LOG_INFO << "Hello";
    return 0;
}
