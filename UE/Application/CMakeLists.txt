project(UeApplication)
cmake_minimum_required(VERSION 3.12)

aux_source_directory(. SRC_LIST)
aux_source_directory(Ports SRC_LIST)
aux_source_directory(States SRC_LIST)
aux_source_directory(Database SRC_LIST)
aux_source_directory(Database/Repository SRC_LIST)
aux_source_directory(Database/Entity SRC_LIST)
aux_source_directory(include/sqlite_orm SRC_LIST)

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Common)
target_link_libraries(${PROJECT_NAME} sqlite3)
