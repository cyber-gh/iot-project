cmake_minimum_required(VERSION 3.16.3)
project(smart_fridge_api)

set(CMAKE_CXX_STANDARD 14)
find_package(Pistache 0.0.2 REQUIRED)
find_package(nlohmann_json 3.7.0 REQUIRED)
find_package (SQLite3)

include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/models.cpp src/models.h src/DatabaseAccess.cpp src/DatabaseAccess.h)
target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES} nlohmann_json::nlohmann_json pistache_shared)
