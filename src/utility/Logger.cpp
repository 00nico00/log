#include "Logger.h"

#include <ctime>
#include <stdexcept>

using namespace nico::utility;

const char* Logger::s_level[LEVEL_COUNT] = {"DEBUG", "INFO", "WARN", "ERROR",
                                            "FATAL"};

Logger::Logger() {}

Logger::~Logger() {}

Logger& Logger::instance() {
    static Logger m_instance;
    return m_instance;
}

void Logger::open(const std::string& filename) {
    m_filename = filename;                 // record filename
    m_fout.open(filename, std::ios::app);  // append
    if (m_fout.fail()) {
        throw std::logic_error("open file failed " + filename);
    }
}

void Logger::close() { m_fout.close(); }

void Logger::log(Level level, const char* file, int line, const char* format,
                 ...) {
    if (m_fout.fail()) {
        throw std::logic_error("open file failed " + m_filename);
    }
    // get current time
    std::time_t ticks = time(nullptr);
    struct tm* ptn = localtime(&ticks);
    char timestamp[32];
    // format output time
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptn);
}