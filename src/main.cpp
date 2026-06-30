#include "RedisCLI.h"
#include <iostream>

int main() {
    std::cout << "Starting Redis Lite CLI...\n";
    RedisCLI cli;
    cli.run();
    return 0;
}
