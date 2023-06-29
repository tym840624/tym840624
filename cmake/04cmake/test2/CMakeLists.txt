cmake_minimum_required(VERSION 3.0)

project(CALCTEST)

aux_source_directory(./ SRC)

include_directories(${HEAD_PATH})

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

add_executable(${APP_NAME_2} ${SRC})

#如果没有指定库的输出路径或者需要直接加载外部提供的库文件，此时就需要使用 link_directories 将库文件路径指定出来
link_directories(${LIB_PATH})
target_link_libraries(${APP_NAME_2} ${SORT_LIB})




