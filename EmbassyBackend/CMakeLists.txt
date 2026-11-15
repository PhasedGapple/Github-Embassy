
cmake_minimum_required(VERSION 3.20)

project(EmbassyBackend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(Threads REQUIRED)

add_executable(EmbassyBackend
    EmbassyBackend.cpp
)

target_link_libraries(EmbassyBackend PRIVATE
    nlohmann_json::nlohmann_json
    httplib::httplib
    Threads::Threads
)
