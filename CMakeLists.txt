cmake_minimum_required(VERSION 3.14)

project(example-parse-many LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -g")

add_executable(generator app/src/generator.cpp)

find_package(GTest REQUIRED)
include(CheckIPOSupported)

add_library(
  service_0
  lib/src/replace_0.cpp
  lib/src/service_0.cpp
  lib/src/tokenize_0.cpp
  )

add_library(
  service_1
  lib/src/replace_0.cpp
  lib/src/service_1.cpp
  lib/src/tokenize_0.cpp
  )

add_library(
  service_2
  lib/src/replace_0.cpp
  lib/src/service_2.cpp
  lib/src/tokenize_0.cpp
  )

add_library(
  service_3
  lib/src/replace_0.cpp
  lib/src/service_3.cpp
  lib/src/tokenize_1.cpp
  )

add_library(
  service_4
  lib/src/replace_0.cpp
  lib/src/service_4.cpp
  lib/src/tokenize_2.cpp
  )

add_library(
  service_5
  lib/src/replace_0.cpp
  lib/src/service_5.cpp
  lib/src/tokenize_2.cpp
  )

add_library(
   service_6
   lib/src/replace_0.cpp
   lib/src/service_6.cpp
   lib/src/tokenize_3.cpp
   )

add_library(
  service_7
  lib/src/replace_0.cpp
  lib/src/service_7.cpp
  lib/src/tokenize_4.cpp
  )

add_library(
  service_8
  lib/src/replace_0.cpp
  lib/src/service_8.cpp
  lib/src/tokenize_4.cpp
  )

add_library(
  service_9
  lib/src/replace_1.cpp
  lib/src/service_9.cpp
  lib/src/tokenize_4.cpp
  )

add_library(
  service_10
  lib/src/replace_1.cpp
  lib/src/service_10.cpp
  lib/src/tokenize_4.cpp
  )

add_library(
  service_11
  lib/src/replace_1.cpp
  lib/src/service_11.cpp
  lib/src/tokenize_4.cpp
  )

add_library(
  service_12
  lib/src/replace_1.cpp
  lib/src/service_12.cpp
  lib/src/tokenize_4.cpp
  )

check_ipo_supported(RESULT has_ipo)

foreach(i 0 1 2 3 4 5 6 7 8 9 10 11 12)
  set_property(TARGET service_${i} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

  target_include_directories(
    service_${i}
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/lib/include"
    )

  add_executable(tests_${i} tests/src/service.cpp)
  target_link_libraries(
    tests_${i}
    PRIVATE
    service_${i}
    GTest::gtest
    GTest::gtest_main
    )

  add_executable(run_service_${i} app/src/service.cpp)
  target_link_libraries(
    run_service_${i}
    PRIVATE
    service_${i}
    )
endforeach()
