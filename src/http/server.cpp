#include "server.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;


/**
 * 监听器协程
 * @param host 监听地址
 * @param port 监听端口
 * @return 协程句柄
 */
net::awaitable<void> listener(const std::string& host, std::uint16_t port)
{
    // 获取当前协程的执行器（executor）
    auto ex = co_await net::this_coro::executor;

    // 创建并配置 TCP 接受器
    tcp::endpoint ep{net::ip::make_address(host), port};
    tcp::acceptor acc(ex);

    acc.open(ep.protocol());
    acc.set_option(tcp::acceptor::reuse_address(true));
    acc.bind(ep);
    acc.listen();

    for (;;)
    {
        tcp::socket sock(ex);
        co_await acc.async_accept(sock, net::use_awaitable);

        sock.set_option(net::ip::tcp::no_delay(true));

        // TODO: 启动会话协程
        // net::co_spawn(ex, http_session(std::move(sock)), net::detached);
    }
}


/**
 * 运行 HTTP 服务器
 * @param host 监听地址
 * @param port 监听端口
 */
void run_server(const std::string& host, const int port)
{
    net::io_context io;

    // 支持 Ctrl+C / kill 优雅退出
    net::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](auto /*ec*/, auto /*signo*/) { io.stop(); });

    // 启动监听器协程
    net::co_spawn(io, listener(host, static_cast<std::uint16_t>(port)), net::detached);

    // 运行 IO 上下文
    io.run();
}
