# 设置 cmake 最小版本号
cmake_minimum_required(VERSION 3.10)

# 设置工程名
project(MathFunctions)
message(${PROJECT_BINARY_DIR})
add_library(MathFunctions mysqrt.cpp)

target_include_directories(MathFunctions PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}"
                          )