cmake_minimum_required(VERSION 3.16.3)
project(smart_fridge_api)

set(CMAKE_CXX_STANDARD 14)
set (THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(Pistache 0.0.2 REQUIRED)
find_package(nlohmann_json 3.7.0 REQUIRED)
find_package (SQLite3)
find_package(PahoMqttCpp REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/models.cpp src/models.h src/DatabaseAccess.cpp src/DatabaseAccess.h src/SmartFridgeService.cpp src/SmartFridgeService.h src/SmartFridgeMqttClient.cpp src/SmartFridgeMqttClient.h)
target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES} ${PAHO_CPP_LIB} paho-mqttpp3 paho-mqtt3as nlohmann_json::nlohmann_json pistache_shared)
