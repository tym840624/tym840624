cmake_minimum_required(VERSION 3.0)

project(CALCTEST)

aux_source_directory(./ SRC)

include_directories(${HEAD_PATH})

link_libraries(${CALC_LIB})

set(EXECUTABLE_OUTPUT_PATH ${EXEC_PATH})

add_executable(${APP_NAME_1} ${SRC})


