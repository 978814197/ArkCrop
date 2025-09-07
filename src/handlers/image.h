#pragma once

#include <boost/beast.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;

/**
 * 裁剪参数结构体
 */
struct CropParams
{
    // 输入文件路径
    std::string input_path;
    // 输出文件路径
    std::string output_path;
    // 起始块索引
    std::int64_t start_idx{};
    // 结束块索引
    std::int64_t end_idx{};
    // 总块数
    std::int64_t total_blocks{};
};

/**
* 处理图像裁剪请求
* @param req 请求对象
* @return HTTP响应
*/
net::awaitable<http::response<http::string_body>> crop_image(const http::request<http::string_body>& req);