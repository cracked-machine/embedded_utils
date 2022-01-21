target_sources(${BUILD_NAME} PRIVATE
    # put source files here
    src/ll_i2c_utils.cpp
    src/ll_spi_utils.cpp
    src/ControlledBase.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE 
    inc
    inc/stm32_isr
)