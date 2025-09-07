#pragma once

#include <sstream>
#include <thread>
#include <string>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "args.h"

namespace logging = boost::log;
namespace trivial = boost::log::trivial;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

/**
 * 记录启动横幅信息
 * @param opt 应用程序选项
 */
void log_startup_banner(const AppOptions& opt);

/**
 * 初始化日志系统
 * @param level 日志级别
 */
void init_logging(trivial::severity_level level = trivial::info);
