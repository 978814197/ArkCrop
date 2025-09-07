#pragma once

#include <string>

/**
 * 启动HTTP服务器
 * @param host 监听地址
 * @param port 监听端口
 */
void run_server(const std::string& host, int port);
