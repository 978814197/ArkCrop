#include "image.h"


/**
 * 处理图像裁剪请求
 * @param req HTTP请求
 * @return HTTP响应
 */
asio::awaitable<http::response<http::string_body>> crop_image(const http::request<http::string_body>& req)
{
    json::object not_found;
    not_found["code"] = 404;
    not_found["msg"] = "not found";
    http::response<http::string_body> res{http::status::ok, 11};
    res.set(http::field::content_type, "application/json; charset=utf-8");
    res.set(http::field::server, "asio-beast");
    res.keep_alive(req.keep_alive());
    res.body() = json::serialize(not_found);
    res.prepare_payload();
    co_return res;
}