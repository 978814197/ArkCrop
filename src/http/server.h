#pragma once

#include <string>
#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::use_awaitable;

namespace http = boost::beast::http;
namespace json = boost::json;


/**
 * 启动HTTP服务器
 * @param host 监听地址
 * @param port 监听端口
 */
void run_server(const std::string& host, int port);