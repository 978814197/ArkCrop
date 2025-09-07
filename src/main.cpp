#include "common/logger.h"
#include "common/args.h"
#include "http/server.h"

int main(const int argc, char** argv)
{
    // 创建参数结构体
    AppOptions opt;
    // 解析命令行参数
    parse_args(argc, argv, opt);

    // 初始化日志系统
    init_logging(opt.log_level);
    // 记录启动横幅
    log_startup_banner(opt);

    // 启动 HTTP 服务器
    run_server(opt.host, opt.port);
}
