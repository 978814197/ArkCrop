#include "logger.h"

/**
 * 初始化日志系统
 * @param filter 日志过滤器，默认过滤掉低于 info 级别的日志
 */
void init_logging(logging::filter const& filter)
{
    // 添加控制台日志输出
    logging::add_console_log(
        std::clog,
        keywords::format = "[%TimeStamp%][%ThreadID%][%Severity%] %Message%"
    );

    // 设置日志过滤器
    logging::core::get()->set_filter(filter);
    // 添加常用属性，如时间戳和线程ID
    logging::add_common_attributes();
}