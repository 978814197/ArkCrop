#include "common/logger.h"
#include "common/args.h"

int main(const int argc, char** argv)
{
    // 创建参数结构体
    AppOptions opt;
    // 解析命令行参数
    parse_args(argc, argv, opt);

    // 初始化日志系统
    init_logging(opt.log_level);


    BOOST_LOG_TRIVIAL(info) << "host:" << opt.host;
    BOOST_LOG_TRIVIAL(info) << "port:" << opt.port;
    BOOST_LOG_TRIVIAL(info) << "thread:" << opt.threads;
    BOOST_LOG_TRIVIAL(info) << "level:" << opt.log_level;
}
