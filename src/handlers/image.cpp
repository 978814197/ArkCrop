#include "image.h"

#include "utils/http.h"

/**
 * 处理图像裁剪请求
* @param req 请求对象
 * @return HTTP响应
 */
net::awaitable<http::response<http::string_body>> crop_image(const http::request<http::string_body>& req)
{
    // 解析请求体
    json::value v;
    try
    {
        v = json::parse(req.body());
    }
    catch (...)
    {
        co_return make_text(http::status::bad_request, req, "bad request");
    }
    co_return make_json(http::status::ok, req, v);
}