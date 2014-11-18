cmake_minimum_required(VERSION 2.8)
project(thread-safe-lru)

add_executable(benchmark benchmark.cpp)

include(FindPkgConfig)
pkg_search_module(TBB REQUIRED tbb)
target_link_libraries(benchmark ${TBB_LIBRARIES})
include_directories(${TBB_INCLUDE_DIRS})

# Boost is needed for the benchmark only
include(FindBoost)
find_package(Boost REQUIRED COMPONENTS locale)
target_link_libraries(benchmark ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
