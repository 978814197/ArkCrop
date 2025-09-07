#pragma once

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace logging = boost::log;
namespace trivial = boost::log::trivial;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

/**
 * 初始化日志系统
 * @param level 日志级别
 */
void init_logging(trivial::severity_level level = trivial::info);