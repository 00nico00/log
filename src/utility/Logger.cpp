#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <stdexcept>

using namespace nico::utility;

const char* Logger::s_level[LEVEL_COUNT] = {"DEBUG", "INFO", "WARN", "ERROR",
                                            "FATAL"};

Logger::Logger() : m_len(0), m_max(0), m_level(DEBUG) {}

Logger::~Logger() { close(); }

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

    // 获取当前文件内容的长度
    m_fout.seekp(0, std::ios::end);  // 定位到文件尾
    m_len += static_cast<int>(
        m_fout.tellp());  // 获取当前读写位置到文件头的偏移, 单位为字节
}

void Logger::close() { m_fout.close(); }

void Logger::log(Level level, const char* file, int line, const char* format,
                 ...) {
    // 屏蔽掉设置的level以下的级别的日志
    if (m_level > level) return;

    if (m_fout.fail()) {
        throw std::logic_error("open file failed " + m_filename);
    }
    // get current time
    std::time_t ticks = time(nullptr);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    // format output time to string
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);
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
        m_len += size;  // update current len
        m_fout << buffer;
        delete[] buffer;
    }
    m_fout << " ";

    std::va_list arg_ptr;       // store variablie param
    va_start(arg_ptr, format);  // init
    // 先知道了字符串的长度，才能动态分配出一个缓冲区来存储
    size = vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr);  // end
    if (size > 0) {
        char* content = new char[size + 1];
        va_start(arg_ptr, format);  // 每次调用vsnprintf都需要处理一下
        vsnprintf(content, size + 1, format, arg_ptr);
        va_end(arg_ptr);
        // printf("%s\n", content);
        m_len += size;  // update current len
        m_fout << content;
        delete[] content;
    }
    m_fout << "\n";
    m_fout.flush();  // 刷新缓存区，使数据写入硬盘

    if (m_max > 0 && m_len >= m_max) {
        rotate();
    }
}

void Logger::rotate() {
    close();
    // 给文件名设置时间戳
    // get current time
    std::time_t ticks = time(nullptr);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];
    // format output time to string
    memset(timestamp, 0, sizeof(timestamp));
    strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptm);

    // 将源文件改名并且新建一个和源文件一样的文件
    std::string filename = m_filename + timestamp;
    if (std::rename(m_filename.c_str(), filename.c_str()) != 0) {
        throw std::logic_error("rename log file failed " +
                               std::string(strerror(errno)));
    }
    open(m_filename);
}