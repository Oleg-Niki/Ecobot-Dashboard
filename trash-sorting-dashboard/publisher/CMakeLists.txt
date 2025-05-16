cmake_minimum_required(VERSION 3.10)
project(TrashSortingPublisher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(publisher
    src/main.cpp
)

target_include_directories(publisher PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/json/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/httplib/include
)
