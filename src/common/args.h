#pragma once

#include <string>
#include <boost/log/trivial.hpp>

namespace trivial = boost::log::trivial;

/**
 * 命令行参数
 */
struct AppOptions
{
    // 监听的地址
    std::string host = "0.0.0.0";
    // 监听的端口
    unsigned short port = 8000;
    // 线程数
    int threads = 4;
    // 日志级别
    trivial::severity_level log_level = trivial::info;
};

/**
 * @brief 解析命令行参数并做三源合并；非法参数会抛出 std::runtime_error。
 * @param argc 参数数量
 * @param argv 参数内容
 * @param opt 输出的参数结构体
 */
void parse_args(int argc, char** argv, AppOptions& opt);
