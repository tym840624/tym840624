cmake_minimum_required(VERSION 3.0.0)

project(CALCLIB)

#aux_source_directory：搜索当前目录（calc 目录）下的所有源文件 放在SRC变量中
aux_source_directory(./ SRC)

#include_directories：包含头文件路径，HEAD_PATH 是在根节点文件中定义的
include_directories(${HEAD_PATH})

#set: 设置库的生成的路径，LIB_PATH 是在根节点文件中定义的
set(LIBRARY_OUTPUT_PATH ${LIB_PATH})

#add_library：生成静态库，静态库名字 CALC_LIB 是在根节点文件中定义的
add_library(${CALC_LIB} STATIC ${SRC})


