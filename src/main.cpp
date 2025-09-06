#include "common/logger.h"

int main()
{
    init_logging();
    BOOST_LOG_TRIVIAL(trace) << "trace (will be filtered)";
    BOOST_LOG_TRIVIAL(debug) << "debug (will be filtered)";
    BOOST_LOG_TRIVIAL(info) << "hello console";
    BOOST_LOG_TRIVIAL(warning) << "heads up";
    BOOST_LOG_TRIVIAL(error) << "oops";
    BOOST_LOG_TRIVIAL(fatal) << "boom";
}
