#pragma once

#include <fstream>
#include <stdexcept>
#include <string>

namespace nico {
namespace utility {

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

private:
    Logger();
    ~Logger();

private:
    /**
     * @brief 表示要记录到哪个文件
     */
    std::string m_filename;
    std::ofstream m_fout;
    static const char* s_level[LEVEL_COUNT];
};

}  // namespace utility
}  // namespace nico