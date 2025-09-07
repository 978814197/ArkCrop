#include "server.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/json.hpp>

#include "utils/http.h"
#include "handlers/image.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

/**
 * 简易调度
 * @param req 请求对象
 * @return 响应对象
 */
static net::awaitable<http::response<http::string_body>> handle_request(http::request<http::string_body>&& req)
{
    // 请求路径
    const std::string target{req.target()};

    // 只允许 POST，其他 405
    if (req.method() != http::verb::post)
    {
        auto r = make_text(http::status::method_not_allowed, req, "method not allowed");
        r.set(http::field::allow, "POST");
        co_return r;
    }

    // 只允许 / 其他404
    if (target != "/")
    {
        co_return make_text(http::status::not_found, req, "not found");
    }

    // 调用函数
    auto res = co_await crop_image(req);
    co_return res;
}

/**
 * socket的 HTTP 请求处理函数
 * @param sock TCP 套接字
 * @return 协程句柄
 */
static net::awaitable<void> http_session(tcp::socket sock)
{
    beast::tcp_stream stream(std::move(sock));
    beast::flat_buffer buffer;

    try
    {
        for (;;)
        {
            http::request<http::string_body> req;
            // 读取一个 HTTP 请求
            co_await http::async_read(stream, buffer, req, net::use_awaitable);

            // 路由处理，生成响应
            auto res = co_await handle_request(std::move(req));

            // 写回响应
            co_await http::async_write(stream, res, net::use_awaitable);

            // 按 keep-alive 语义决定是否关闭
            if (!req.keep_alive()) break;
        }
    }
    catch (const beast::system_error& se)
    {
        if (se.code() != http::error::end_of_stream && se.code() != net::error::operation_aborted)
        {
            BOOST_LOG_TRIVIAL(error) << "session error: " << se.code().message();
        }
    }

    // 关闭写半部（优雅断开）
    beast::error_code ec;
    stream.socket().shutdown(tcp::socket::shutdown_send, ec);
    co_return;
}


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

        net::co_spawn(ex, http_session(std::move(sock)), net::detached);
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
