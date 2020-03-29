project(Database)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Entity SRC_LIST)
aux_source_directory(Repository SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Common)
