cmake_minimum_required(VERSION 3.13)


# configure the build
set(CMAKE_CXX_STANDARD  20)
set(TARGET_TYPE GNU)
include(tests/linux.cmake)
project(EMBEDDED_UTILS_TEST_SUITE VERSION 0.1 DESCRIPTION "EMBEDDED_UTILS_TEST_SUITE")
add_executable(${BUILD_NAME} "")
add_subdirectory(tests)
add_subdirectory(lib)


# configure catch2 test lib
find_package(Catch2 3 REQUIRED)
target_link_libraries(${BUILD_NAME} PRIVATE Catch2::Catch2WithMain)
# add_custom_target(size ALL ${CMAKE_SIZE} ${BUILD_NAME} DEPENDS ${BUILD_NAME})
