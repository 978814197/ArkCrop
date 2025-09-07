#include "http.h"

/**
 * 生成一个文响应
 * @param st HTTP 状态码
 * @param body 响应体
 * @param keep_alive 是否保持连接
 * @param version HTTP 版本
 * @param content_type 内容类型
 * @return HTTP 响应对象
 */
static http::response<http::string_body> make_response(
    const http::status st,
    std::string body,
    const bool keep_alive,
    const unsigned version,
    const std::string_view content_type
)
{
    http::response<http::string_body> res{st, version};
    res.set(http::field::server, "ArkCrop/0.1");
    res.set(http::field::content_type, content_type);
    res.keep_alive(keep_alive);
    res.body() = std::move(body);
    res.prepare_payload();
    return res;
}


/**
 * 生成一个文本响应
 * @param st HTTP 状态码
 * @param body 响应体
 * @param keep_alive 是否保持连接
 * @param version HTTP 版本
 * @return HTTP 响应对象
 */
http::response<http::string_body> make_text(
    const http::status st,
    std::string body,
    const bool keep_alive,
    const unsigned version
)
{
    return make_response(st, std::move(body), keep_alive, version, "text/plain; charset=utf-8");
}

/**
 * 生成一个 JSON 响应
 * @param st HTTP 状态码
 * @param val JSON 值
 * @param keep_alive 是否保持连接
 * @param version HTTP 版本
 * @return HTTP 响应对象
 */
http::response<http::string_body> make_json(
    const http::status st,
    const json::value& val,
    const bool keep_alive,
    const unsigned version
)
{
    return make_response(st, json::serialize(val), keep_alive, version, "application/json");
}
