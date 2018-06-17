cmake_minimum_required(VERSION 2.8)

project(BRM)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if ("$ENV{TESTING}" EQUAL 1)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D TEST")
  enable_testing()
  add_subdirectory("${PROJECT_SOURCE_DIR}/test")
endif()

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
include_directories("${PROJECT_SOURCE_DIR}/include")

find_package(PkgConfig)
pkg_check_modules(JSONCPP jsoncpp)
pkg_check_modules(OPENSSL openssl)
find_package(Threads REQUIRED)

link_libraries(
  ${JSONCPP_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)

add_library(BRM STATIC ${SOURCES})

target_link_libraries(BRM Threads::Threads)

add_executable(brm ${SOURCES})

target_link_libraries(brm BRM)

if ("$ENV{DEBUGGING}" EQUAL 1)
  target_compile_definitions(brm PRIVATE LOG_LEVEL=3)
else()
  target_compile_definitions(brm PRIVATE LOG_LEVEL=2)
endif()