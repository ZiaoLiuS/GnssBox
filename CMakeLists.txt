###################################################################
#
# CMake配置文件
#
# 参考资料：
#
# https://www.hahack.com/codes/cmake/
#
# Copyright(c)
#
# Shoujian Zhang, Wuhan University, 2020
###################################################################

# 编译该软件时使用的cmake版本
cmake_minimum_required(VERSION 3.10.2)

# 项目名称
project(gnss VERSION 1.0)

# 编译选项
# debug means a detailed output
# release will give a very brief hint during compilation
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")

# 外部依赖库
find_package(Eigen3 REQUIRED)

# 外部库的头文件路径
include_directories( ${EIGEN3_INCLUDE_DIR})
# 武大服务器上eigen地址:/home/shjzhang/bin/include/eigen3
#include_directories( /home/shjzhang/bin/include/eigen3 )

# 本地eigen地址
# include_directories( /home/zero/eigen-3.4.0)

# 添加头文件的路径
include_directories( ${PROJECT_SOURCE_DIR}/lib/util/ 
                     ${PROJECT_SOURCE_DIR}/lib/time/
                     ${PROJECT_SOURCE_DIR}/lib/math/
                     ${PROJECT_SOURCE_DIR}/lib/coord/
                     ${PROJECT_SOURCE_DIR}/lib/gnss/
                     )

# 递归搜索头文件名
file(GLOB_RECURSE HEADERS lib/*.hpp)

# 添加源文件名到变量中
aux_source_directory( ${PROJECT_SOURCE_DIR}/lib/util/ DIR_LIB_SRCS )
aux_source_directory( ${PROJECT_SOURCE_DIR}/lib/time/ DIR_LIB_SRCS )
aux_source_directory( ${PROJECT_SOURCE_DIR}/lib/math/ DIR_LIB_SRCS )
aux_source_directory( ${PROJECT_SOURCE_DIR}/lib/coord/ DIR_LIB_SRCS )
aux_source_directory( ${PROJECT_SOURCE_DIR}/lib/gnss/ DIR_LIB_SRCS )

# 根据源文件创建库文件
add_library(gnss SHARED ${DIR_LIB_SRCS} lib/gnss/LsqRTK.cpp lib/gnss/LsqRTK.hpp lib/gnss/ComputePrefit.cpp lib/gnss/ComputePrefit.hpp lib/gnss/DeltaOp.cpp lib/gnss/DeltaOp.hpp lib/gnss/Rtcm3NavStore.cpp lib/gnss/Rtcm3NavStore.hpp)

# 安装库文件
install(TARGETS gnss DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)

# 可执行程序子目录
add_subdirectory(apps)
add_subdirectory(script)
