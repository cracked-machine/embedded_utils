target_sources(${BUILD_NAME} PRIVATE
    # put source files here
    src/ll_i2c_utils.cpp
    src/ll_spi_utils.cpp
    src/interrupt_manager_base.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE inc)