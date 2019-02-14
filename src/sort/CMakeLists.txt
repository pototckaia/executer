cmake_minimum_required(VERSION 3.9)
project(multi_sort)

set(CMAKE_CXX_FLAGS "-Wall")
set(SOURCE_FILES dump.h dump.cpp sort.h sort.cpp)

add_library(sort SHARED STATIC ${SOURCE_FILES})
target_compile_features(sort PUBLIC cxx_std_17)


install(FILES dump.h sort.h DESTINATION include/sort)
install(TARGETS sort DESTINATION lib)
