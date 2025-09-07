#include "image.h"

#include "utils/http.h"


/**
 * 自定义裁剪参数检测与转换
 * @param jv JSON值
 * @return 裁剪参数结构体
 */
inline CropParams tag_invoke(json::value_to_tag<CropParams>, json::value const& jv)
{
    // 1) 基础结构校验：必须是 object
    if (!jv.is_object())
        throw boost::system::system_error(json::error::not_object);

    auto const& o = jv.as_object();

    // 2) 取字段指针（不会插入默认值）
    auto const* in = o.if_contains("input_path");
    auto const* out = o.if_contains("output_path");
    auto const* s = o.if_contains("start_idx");
    auto const* e = o.if_contains("end_idx");
    auto const* t = o.if_contains("total_blocks");

    // 3) 类型检查：严格要求字符串/无符号 64 位整数
    if (!in || !in->is_string()) throw boost::system::system_error(json::error::not_string);
    if (!out || !out->is_string()) throw boost::system::system_error(json::error::not_string);
    if (!s || !s->is_int64()) throw boost::system::system_error(json::error::not_uint64);
    if (!e || !e->is_int64()) throw boost::system::system_error(json::error::not_uint64);
    if (!t || !t->is_int64()) throw boost::system::system_error(json::error::not_uint64);

    // 4) 构造强类型目标（将 string_view-like 拷贝为 std::string）
    CropParams p{
        std::string(in->as_string()),
        std::string(out->as_string()),
        s->as_int64(),
        e->as_int64(),
        t->as_int64()
    };

    // 5) 业务校验：索引范围/块计数等
    //    这里选用 invalid_number 代表“业务数值不合法”
    if (p.start_idx > p.end_idx)
        throw boost::system::system_error(json::error::size_mismatch);
    if (p.total_blocks == 0)
        throw boost::system::system_error(json::error::size_mismatch);

    return p;
}

/**
 * 解析裁剪参数
 * @param req 请求对象
 * @return 裁剪参数结构体
 */
static std::optional<CropParams> parse_crop_params(const http::request<http::string_body>& req)
{
    // 解析请求体
    std::error_code ec;
    const json::value jv = json::parse(req.body(), ec);
    if (ec) return std::nullopt;

    try
    {
        return json::value_to<CropParams>(jv);
    }
    catch (...)
    {
        return std::nullopt;
    }
}

/**
 * 处理图像裁剪请求
* @param req 请求对象
 * @return HTTP响应
 */
net::awaitable<http::response<http::string_body>> crop_image(const http::request<http::string_body>& req)
{
    // 解析请求体
    const auto params = parse_crop_params(req);
    if (!params)
    {
        co_return make_text(http::status::bad_request, req, "bad request");
    }

    co_return make_text(http::status::ok, req, "ok");
}
