#include <iostream>

#include "utility/Logger.h"

using namespace nico::utility;

int main(int argc, char **argv) {
    Logger::instance().open("/mnt/src/cpp/log/Log/test.log");
    Logger::instance().level(Logger::INFO);
    Logger::instance().max(1024);
    DEBUG("hello log");
    INFO("hello log");
    WARN("hello log");
    ERROR("hello log");
    FATAL("hello log");
    return 0;
}
