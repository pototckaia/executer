cmake_minimum_required(VERSION 3.9)
project(test)

set(CMAKE_CXX_FLAGS "-Wall")

add_executable(test_task task_test.cpp test_stuff.h ../thread_pool/random_string.h)
target_link_libraries(test_task catch executor)
target_compile_features(test_task PUBLIC cxx_std_17)

add_executable(sort_test sort_test.cpp)
target_link_libraries(sort_test catch sort Threads::Threads)
target_compile_features(sort_test PUBLIC cxx_std_17)
