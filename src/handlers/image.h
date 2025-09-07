#pragma once

#include <boost/beast.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/json.hpp>

namespace http = boost::beast::http;
namespace asio = boost::asio;
namespace json = boost::json;

/**
* 处理图像裁剪请求
* @param req HTTP请求
* @return HTTP响应
*/
asio::awaitable<http::response<http::string_body>> crop_image(const http::request<http::string_body>& req);
