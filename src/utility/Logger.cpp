#include "Logger.h"

#include <cstdio>
#include <cstring>
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
    // format output time to string
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptn);
    // printf("%s\n", timestamp);
    // printf("%s\n", file);
    // printf("%d\n", line);
    /**
     * @brief 时间/日志级别/日志所在文件名称/文件行号
     */
    const char* fmt = "%s %s %s:%d";
    int size = snprintf(nullptr, 0, fmt, timestamp, s_level[level], file, line);
    if (size > 0) {
        char* buffer = new char[size + 1];
        snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
        // buffer[size] = '\0'
        // printf("%s\n", buffer);
        m_fout << buffer;
        delete[] buffer;
    }
}