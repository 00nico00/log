#include <iostream>

#include "utility/Logger.h"

using namespace nico::utility;

int main(int argc, char **argv) {
    Logger::instance().open("/mnt/src/cpp/log/Log/test.log");
    Logger::instance().log(Logger::DEBUG, __FILE__, __LINE__, "hello world");
    
    return 0;
}
