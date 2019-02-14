cmake_minimum_required(VERSION 3.9)
project(executor)

set(CMAKE_CXX_FLAGS "-Wall ")
set(SOURCE_FILES thread_pool.cpp thread_pool.h task.h merge_sort_on_executor.h random_string.h logger.cpp logger.h)

find_package(glog 0.3.5 REQUIRED)

add_library(executor SHARED STATIC ${SOURCE_FILES})
target_link_libraries(executor ${CMAKE_THREAD_LIBS_INIT} glog::glog)


#install(FILES dump.h sort.h DESTINATION include/sort)
#install(TARGETS sort DESTINATION lib)
