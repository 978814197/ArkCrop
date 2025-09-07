# 寻找并启用所需的 Boost 组件：Asio(网络/协程)、Beast(HTTP)、Json、Log、ProgramOptions
find_package(Boost REQUIRED COMPONENTS asio beast json log log_setup program_options)

# 生成核心静/共享库：arkcrop_lib
# 把通用代码(common)与HTTP服务端(http)的实现编进这个库，方便可执行程序链接复用
add_library(
        arkcrop_lib
        common/logger.cpp
        common/args.cpp
        common/args.h
        http/server.cpp
        http/server.h
        handlers/image.cpp
        handlers/image.h
        utils/http.cpp
        utils/http.h
)

# 让库的使用者（例如本工程里的可执行程序）能找到这些头文件
target_include_directories(arkcrop_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 链接 Boost 组件到库上（PUBLIC：向外传播依赖，便于最终可执行程序自动继承）
target_link_libraries(arkcrop_lib PUBLIC
        Boost::asio
        Boost::beast
        Boost::json
        Boost::log
        Boost::log_setup
        Boost::program_options
)

# -------------------------- Linux (非 macOS) 上强制启用 io_uring --------------------------
# 这里把 io_uring 作为**硬性依赖**：找不到头或库就直接中止配置/编译
if (UNIX AND NOT APPLE)
    # 查找 liburing 的头文件与库。通常发行版的开发包名为 liburing-dev
    find_path(LIBURING_INCLUDE_DIR liburing.h)
    find_library(LIBURING_LIBRARY uring)

    # 如未找到，立刻报错并终止：你的需求是“io_uring 不存在就直接失败”
    if (NOT LIBURING_INCLUDE_DIR OR NOT LIBURING_LIBRARY)
        message(FATAL_ERROR "io_uring (liburing) is REQUIRED on Linux, but not found. Install 'liburing-dev' and try again.")
    endif ()

    # 告诉 Boost.Asio：启用 io_uring 后端（主要影响文件 I/O：stream_file/random_access_file 等）
    target_compile_definitions(arkcrop_lib PUBLIC BOOST_ASIO_HAS_IO_URING)

    # 可选：禁用 epoll，使 socket/timer 等也走 io_uring（提交/完成队列模型）
    # 注意：禁用 epoll 后，网络 I/O 也会通过 io_uring，确保系统内核与 liburing 版本足够新。
    target_compile_definitions(arkcrop_lib PUBLIC BOOST_ASIO_DISABLE_EPOLL)

    # 传播 liburing 的包含路径与链接库给依赖方（使用 PUBLIC，保证最终可执行也能链接到）
    target_include_directories(arkcrop_lib PUBLIC ${LIBURING_INCLUDE_DIR})
    target_link_libraries(arkcrop_lib PUBLIC ${LIBURING_LIBRARY})

    message(STATUS "io_uring is required and enabled (include: ${LIBURING_INCLUDE_DIR})")
endif ()

# -------------------------- Windows 专用链接项（IOCP 后端） --------------------------
# 在 Windows 上，Asio 默认使用 IOCP；下面这两个系统库是常见的网络所需
if (WIN32)
    target_link_libraries(arkcrop_lib PUBLIC ws2_32 mswsock)
endif ()

# -------------------------- 可执行程序 --------------------------
# 生成最终可执行文件 arkcrop，主入口在 main.cpp
add_executable(arkcrop main.cpp)

# 指定可执行程序的输出目录（bin 下）；便于区分构建产物
set_target_properties(arkcrop PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 链接上面构建好的库（PRIVATE：只在 arkcrop 内部使用，不向外传播）
target_link_libraries(arkcrop PRIVATE arkcrop_lib)
