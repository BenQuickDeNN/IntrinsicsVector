# 基本配置
cmake_minimum_required(VERSION 3.0)
project(intrin_helper_example CXX)

# 头文件位置
include_directories(../)

# 编译选项
set(CMAKE_BUILD_TYPE Debug)

# 在我的PC上构建，我的PC最高支持AVX指令集
# set(CMAKE_CXX_FLAGS "-O3 -fopenmp -ftree-vectorize -mavx -Wall -std=c++17")

# 在学校服务器上构建，学校服务器最高支持AVX512F指令集
set(CMAKE_CXX_FLAGS "-O3 -fopenmp -ftree-vectorize -mavx512f -Wall -std=c++17")

# 添加源文件目录到SRC变量
# set(SRC1 src/arrayadd.cpp)

# 生成可执行文件
add_executable(vadd src/vadd.cpp)
add_executable(compare_vc_iv src/compare_vc_iv.cpp)