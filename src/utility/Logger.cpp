#include "Logger.h"

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