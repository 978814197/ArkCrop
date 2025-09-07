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
 * @param body 响应体
 * @param keep_alive 是否保持连接
 * @param version HTTP 版本
 * @return HTTP 响应对象
 */
static http::response<http::string_body> make_text(
    http::status st,
    std::string body,
    bool keep_alive,
    unsigned version
);

/**
 * 生成一个 JSON 响应
 * @param st HTTP 状态码
 * @param val JSON 值
 * @param keep_alive 是否保持连接
 * @param version HTTP 版本
 * @return HTTP 响应对象
 */
static http::response<http::string_body> make_json(
    http::status st,
    const json::value& val,
    bool keep_alive,
    unsigned version
);
