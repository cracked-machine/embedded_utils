target_sources(${BUILD_NAME} PRIVATE
    # put source files here
    src/i2c_utils.cpp
    src/spi_utils.cpp
    src/usart_utils.cpp
    src/allocation_restricted_base.cpp
    src/timer_manager.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE 
    inc
    inc/stm32_isr
)