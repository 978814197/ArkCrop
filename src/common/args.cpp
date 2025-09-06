#include "args.h"
#include "common/logger.h"
#include <boost/program_options.hpp>
#include <boost/json.hpp>

namespace po = boost::program_options;
namespace json = boost::json;
namespace trivial = boost::log::trivial;

/**
 * @brief 解析命令行参数并做三源合并；非法参数会抛出 std::runtime_error。
 * @param argc 参数数量
 * @param argv 参数内容
 * @param opt 输出的参数结构体
 */
void parse_args(const int argc, char** argv, AppOptions& opt)
{
    // 定义参数描述
    po::options_description desc("Options");
    // 添加参数
    desc.add_options()
    (
        "help,h",
        "显示帮助"
    )
    (
        "host",
        po::value<std::string>(&opt.host)->value_name("IP")->default_value(opt.host),
        "监听地址（默认 0.0.0.0）"
    )
    (
        "port,p",
        po::value<unsigned short>()
        ->value_name("PORT")
        ->default_value(opt.port)
        ->notifier([&](unsigned short v)
        {
            if (v == 0 || v > 65535)
                throw po::validation_error(po::validation_error::invalid_option_value, "port");
            opt.port = v;
        }),
        "监听端口（1..65535，默认 8000）"
    )
    (
        "threads,t",
        po::value<int>()
        ->value_name("N")
        ->default_value(opt.threads)
        ->notifier([&](const int v)
        {
            if (v <= 0)
                throw po::validation_error(po::validation_error::invalid_option_value, "threads");
            opt.threads = v;
        }),
        "线程数（>0，默认 4）"
    )
    (
        "log-level,l",
        po::value<trivial::severity_level>()
        ->value_name("LEVEL")
        ->default_value(opt.log_level, "info")
        ->notifier([&](trivial::severity_level v)
        {
            opt.log_level = v;
        }),
        "日志级别：trace|debug|info|warning|error|fatal（默认 info）"
    );

    po::variables_map vm;

    // 命令行
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);

    if (vm.contains("help"))
    {
        std::cout << "用法: " << argv[0] << " [options]\n\n" << desc << std::endl;
        std::exit(0);
    }

    // 触发 notifiers / 验证
    po::notify(vm);
}
