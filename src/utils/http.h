#pragma once

#include <string>
#include <boost/beast/http.hpp>
#include <boost/log/trivial.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;

/**
 * 生成一个文本响应
 * @param st HTTP 状态码
 * @param req 响应体
 * @param body 响应体
 * @return HTTP 响应对象
 */
http::response<http::string_body> make_text(
    http::status st,
    const http::request<http::string_body>& req,
    std::string body
);

/**
 * 生成一个 JSON 响应
 * @param st HTTP 状态码
 * @param req 响应体
 * @param val JSON 值
 * @return HTTP 响应对象
 */
http::response<http::string_body> make_json(
    http::status st,
    const http::request<http::string_body>& req,
    const json::value& val
);
