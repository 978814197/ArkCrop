cmake_minimum_required(VERSION 4.0)
project(ArkCrop)

set(CMAKE_CXX_STANDARD 23)

# 允许 IDE（VSCode/CLion/Visual Studio）中按文件夹显示 targets
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)
