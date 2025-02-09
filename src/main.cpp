#include "CoreService.hpp"
#include "NetService.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <core|net>\n";
        return 1;
    }

    std::string serviceType = argv[1];

    if (serviceType == "core") {
        CoreService core;
        core.start();
    } else if (serviceType == "net") {
        NetService net;
        net.start();
    } else {
        std::cerr << "Invalid argument. Use 'core' or 'net'.\n";
        return 1;
    }

    return 0;
}
