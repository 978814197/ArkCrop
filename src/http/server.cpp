#include "server.h"

#include <boost/beast/core.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/log/trivial.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;


// --------- 监听器：接受连接并 co_spawn 会话 ----------
static awaitable<void> listener(const std::string host, unsigned short port)
{
    auto ex = co_await boost::asio::this_coro::executor;

    tcp::endpoint ep{boost::asio::ip::make_address(host), port};
    tcp::acceptor acc(ex);

    acc.open(ep.protocol());
    acc.set_option(tcp::acceptor::reuse_address(true));
    acc.bind(ep);
    acc.listen();

    for (;;)
    {
        tcp::socket sock(ex);
        co_await acc.async_accept(sock, use_awaitable);
        // boost::asio::co_spawn(ex, http_session(std::move(sock)), boost::asio::detached);
    }
}

/**
 * 启动HTTP服务器
 * @param host 监听地址
 * @param port 监听端口
 */
void run_server(const std::string& host, const int port)
{
    boost::asio::io_context io;

    // 支持 Ctrl+C / kill 优雅退出
    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { io.stop(); });

    boost::asio::co_spawn(io, listener(host, static_cast<unsigned short>(port)), boost::asio::detached);
    io.run();
}
