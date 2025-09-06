find_package(Boost REQUIRED COMPONENTS asio beast json log log_setup program_options)
add_library(
        arkcrop_lib
        common/logger.cpp
        common/args.cpp
        common/args.h
)
target_include_directories(arkcrop_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(arkcrop_lib PUBLIC
        Boost::asio
        Boost::beast
        Boost::json
        Boost::log
        Boost::log_setup
        Boost::program_options
)

add_executable(arkcrop main.cpp)
set_target_properties(arkcrop PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(arkcrop PRIVATE arkcrop_lib)
