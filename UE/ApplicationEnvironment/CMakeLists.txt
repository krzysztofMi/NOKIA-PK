project(UeApplicationEnvironment)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. SRC_LIST)
aux_source_directory(UeGui SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST})
