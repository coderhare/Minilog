#include <iostream>
#include "Minilog.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    minilog::initialize(minilog::GuaranteedLogger(), "/Users/wocaibujiaoquanmei/Minilog/", "Minilog", 1);
    LOG_WARN << "Hello";
    return 0;
}
