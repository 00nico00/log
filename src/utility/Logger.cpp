#include "Logger.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <cstdarg>
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
    m_fout << " ";
    
    std::va_list arg_ptr;  // store variablie param
    va_start(arg_ptr, format);  // init
    // 先知道了字符串的长度，才能动态分配出一个缓冲区来存储
    size = vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr); // end
    if (size > 1) {
        char* content = new char[size + 1];
        va_start(arg_ptr, format); // 每次调用vsnprintf都需要处理一下
        vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        // printf("%s\n", content);
        m_fout << content;
    }

    m_fout << "\n";
    m_fout.flush(); // 刷新缓存区，使数据写入硬盘
}