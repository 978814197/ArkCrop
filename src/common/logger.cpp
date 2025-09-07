#include "logger.h"

/**
 * 初始化日志系统
 * @param level 日志级别
 */
void init_logging(trivial::severity_level level)
{
    // 添加控制台日志输出
    logging::add_console_log(
        std::clog,
        keywords::format = "[%TimeStamp%][%ThreadID%][%Severity%] %Message%"
    );

    // 设置日志过滤器
    logging::core::get()->set_filter(trivial::severity >= level);
    // 添加常用属性，如时间戳和线程ID
    logging::add_common_attributes();
}