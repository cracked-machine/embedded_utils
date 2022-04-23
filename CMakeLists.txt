# This file is for running X86 unit tests only.
cmake_minimum_required(VERSION 3.13)

project(EMBEDDED_UTILS_TEST_SUITE VERSION 0.1 DESCRIPTION "EMBEDDED_UTILS_TEST_SUITE")

# if this is the main project then configure the build for test_suite only
if(CMAKE_PROJECT_NAME STREQUAL EMBEDDED_UTILS_TEST_SUITE)
    
    # X86 Linux compiler settings
    include(cmake/linux.cmake)

    # The define will let unit tests load the mocked stm32g0xx.h (tests/mocks/stm32g0xx.h)
    add_compile_definitions(${BUILD_NAME} STM32G0B1xx)

    # libfuse definitions
    add_compile_definitions(_FILE_OFFSET_BITS=64)

    # include these in all source files for convenient stdout with unit tests
    add_definitions(-include iostream)  
    add_definitions(-include algorithm)
    add_definitions(-include iomanip)       

    # include(cmake/cppcheck.cmake)
    
    # BUILD_NAME is set in cmake/linux.cmake
    add_executable(${BUILD_NAME} "")

    target_compile_features(${BUILD_NAME} PUBLIC cxx_std_20)
    add_subdirectory(tests)

    # add catch2 test lib
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(${BUILD_NAME} PRIVATE Catch2::Catch2WithMain)

else() # build options for using as a submodule
    
    
endif()

# if this is submodule in another project then just build as standalone
add_subdirectory(src)


