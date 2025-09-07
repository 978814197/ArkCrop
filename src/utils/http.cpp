#include "http.h"

/**
 * 生成一个文响应
 * @param st HTTP 状态码
 * @param req 请求对象
 * @param body 响应体
 * @param content_type 内容类型
 * @return HTTP 响应对象
 */
static http::response<http::string_body> make_response(
    const http::status st,
    const http::request<http::string_body>& req,
    std::string body,
    const std::string_view content_type
)
{
    // 是否保持连接
    const bool keep = req.keep_alive();
    // HTTP 版本
    const unsigned ver = req.version();

    http::response<http::string_body> res{st, keep};
    res.set(http::field::server, "ArkCrop/0.1");
    res.set(http::field::content_type, content_type);
    res.keep_alive(ver);
    res.body() = std::move(body);
    res.prepare_payload();
    return res;
}


/**
 * 生成一个文本响应
 * @param st HTTP 状态码
 * @param req 请求对象
 * @param body 响应体
 * @return HTTP 响应对象
 */
http::response<http::string_body> make_text(
    const http::status st,
    const http::request<http::string_body>& req,
    std::string body
)
{
    return make_response(st, req, std::move(body), "text/plain; charset=utf-8");
}

/**
 * 生成一个 JSON 响应
 * @param st HTTP 状态码
 * @param req 请求对象
 * @param val JSON 值
 * @return HTTP 响应对象
 */
http::response<http::string_body> make_json(
    const http::status st,
    const http::request<http::string_body>& req,
    const json::value& val
)
{
    return make_response(st, req, json::serialize(val), "application/json");
}