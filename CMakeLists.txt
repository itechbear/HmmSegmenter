cmake_minimum_required(VERSION 2.8.4)

project(HmmSegmenter)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

include_directories(
  .
  )

add_subdirectory(src)

add_subdirectory(
  /usr/src/gtest
  ${CMAKE_BINARY_DIR}/third_party/gtest
)

add_executable(
  HmmSegmenter
  main.cc
  )

target_link_libraries(HmmSegmenter hmmsegmenter)