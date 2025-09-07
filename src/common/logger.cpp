#include "logger.h"

#if defined(_WIN32)
// Windows：获取进程 ID 的头文件与适配
#include <process.h>
// 跨平台的 get_pid 实现（Windows 版本）
static inline int get_pid() { return _getpid(); }
// 平台字符串，用于启动横幅输出
static constexpr const char* kPlatform = "Windows";
#else
// Linux：获取进程 ID 的头文件
#include <unistd.h>
// 跨平台的 get_pid 实现（Linux 版本）
static inline int get_pid() { return getpid(); }
// 平台字符串，用于启动横幅输出
static constexpr const char* kPlatform = "Linux";
#endif

// -----------------------------------------------------------------------------
// 检测 Asio 后端（仅用于“展示”）
// 会根据编译宏判断：
//   - Windows：IOCP（socket 与文件）
//   - Linux：是否启用了 BOOST_ASIO_HAS_IO_URING；
//            以及是否禁用了 EPOLL（BOOST_ASIO_DISABLE_EPOLL）
// 注意：这是“展示层”的字符串，不改变运行时行为。
// -----------------------------------------------------------------------------
static std::string detect_asio_backend()
{
#if defined(_WIN32)
    return "IOCP (sockets & files)";
#elif defined(BOOST_ASIO_HAS_IO_URING) && defined(BOOST_ASIO_DISABLE_EPOLL)
    return "io_uring (sockets & files)";
#elif defined(BOOST_ASIO_HAS_IO_URING)
    return "epoll (sockets) + io_uring (files)";
#else
    return "epoll (sockets) + file I/O via thread-pool/offload";
#endif
}

// -----------------------------------------------------------------------------
// 构建模式展示：Debug / Release
// -----------------------------------------------------------------------------
static const char* build_mode()
{
#ifdef NDEBUG
    return "Release";
#else
    return "Debug";
#endif
}


void log_startup_banner(const AppOptions& opt)
{
    const auto backend = detect_asio_backend();
    const auto hc = std::max(1u, std::thread::hardware_concurrency());

    std::ostringstream oss;
    oss << "\n"
        << "ArkCrop HTTP Server\n\n"
        << "listen     : " << opt.host << ":" << opt.port << "\n"
        << "threads    : " << opt.threads << " (hw=" << hc << ")\n"
        << "log-level  : " << opt.log_level << "\n"
        << "backend    : " << backend << "\n"
        << "platform   : " << kPlatform << " | build=" << build_mode() << "\n"
        << "pid        : " << get_pid() << "\n";
    BOOST_LOG_TRIVIAL(info) << oss.str();
}

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
