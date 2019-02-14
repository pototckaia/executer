cmake_minimum_required(VERSION 3.9)
project(sort-root)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra")
#set(SORT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/sort)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)
include_directories(src/thread_pool)
include_directories(src/sort)

add_subdirectory(lib/catch)

add_subdirectory(src/thread_pool)
add_subdirectory(src/sort)
add_subdirectory(src/benchmark)
add_subdirectory(src/test)
