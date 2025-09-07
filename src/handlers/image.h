#pragma once

#include <boost/beast.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;

/**
* 处理图像裁剪请求
* @param req 请求对象
* @return HTTP响应
*/
net::awaitable<http::response<http::string_body>> crop_image(const http::request<http::string_body>& req);