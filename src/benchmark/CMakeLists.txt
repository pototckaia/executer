cmake_minimum_required(VERSION 3.9)
project(benchmark_multi_threaded_sorting)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(SOURCE_FILES sort_benchmark.cpp)
include_directories(${SORT_INCLUDE_DIR})

add_executable(sort_benchmark ${SOURCE_FILES})
target_link_libraries(sort_benchmark sort executor benchmark Threads::Threads executor)
target_compile_features(sort_benchmark PUBLIC cxx_std_17)
