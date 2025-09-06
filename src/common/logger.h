#pragma once

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/expressions/filter.hpp>

namespace logging = boost::log;
namespace trivial = boost::log::trivial;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

/**
 * 初始化日志系统
 * @param filter 日志过滤器，默认过滤掉低于 info 级别的日志
 */
void init_logging(logging::filter const& filter = trivial::severity >= trivial::info);
