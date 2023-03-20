#pragma once

#include <fstream>
#include <string>

namespace nico {
namespace utility {

// ##用在__VA_ARGS__宏前面是为了使得可变参数没有时也能取消掉','符号
#ifndef DEBUG
#define DEBUG(format, ...)                                            \
    Logger::instance().log(Logger::DEBUG, __FILE__, __LINE__, format, \
                           ##__VA_ARGS__)
#endif

#ifndef INFO
#define INFO(format, ...)                                            \
    Logger::instance().log(Logger::INFO, __FILE__, __LINE__, format, \
                           ##__VA_ARGS__)
#endif

#ifndef WARN
#define WARN(format, ...)                                            \
    Logger::instance().log(Logger::WARN, __FILE__, __LINE__, format, \
                           ##__VA_ARGS__)
#endif

#ifndef ERROR
#define ERROR(format, ...)                                            \
    Logger::instance().log(Logger::ERROR, __FILE__, __LINE__, format, \
                           ##__VA_ARGS__)
#endif

#ifndef FATAL
#define FATAL(format, ...)                                            \
    Logger::instance().log(Logger::FATAL, __FILE__, __LINE__, format, \
                           ##__VA_ARGS__)
#endif

class Logger {
public:
    enum Level { DEBUG = 0, INFO, WARN, ERROR, FATAL, LEVEL_COUNT };

    /**
     * @brief 将日志写入文件
     *
     * @param level 日志等级
     * @param file 日志所在的文件
     * @param line 日志函数所在的行号
     * @param format 日志内容格式
     * @param ... 日志内容
     */
    void log(Level level, const char* file, int line, const char* format, ...);

    // 单例模式
    static Logger& instance();

    void open(const std::string& filename);
    void close();

    void level(Level level) { m_level = level; }
    void max(int bytes) { m_max = bytes; }

private:
    Logger();
    ~Logger();

    /**
     * @brief 日志翻滚功能
     */
    void rotate();

private:
    /**
     * @brief 表示要记录到哪个文件
     */
    std::string m_filename;
    std::ofstream m_fout;
    Level m_level;
    int m_max;  // 一个日志容纳最大长度
    int m_len;  // 记录当前日志已经有多长了
    static const char* s_level[LEVEL_COUNT];
};

}  // namespace utility
}  // namespace nico