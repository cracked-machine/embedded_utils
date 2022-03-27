# This file is for running X86 unit tests only.
cmake_minimum_required(VERSION 3.13)

project(EMBEDDED_UTILS_TEST_SUITE VERSION 0.1 DESCRIPTION "EMBEDDED_UTILS_TEST_SUITE")

# if this is the main project then configure the build for test_suite only
if(CMAKE_PROJECT_NAME STREQUAL EMBEDDED_UTILS_TEST_SUITE)
    include(cmake/linux.cmake)

    # include(cmake/cppcheck.cmake)
    # BUILD_NAME is set in cmake/linux.cmake
    add_executable(${BUILD_NAME} "")

    target_compile_features(${BUILD_NAME} PUBLIC cxx_std_20)
    add_subdirectory(tests)

    # add catch2 test lib
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(${BUILD_NAME} PRIVATE Catch2::Catch2WithMain)

endif()

# if this is submodule in another project then just build as standalone
add_subdirectory(src)


