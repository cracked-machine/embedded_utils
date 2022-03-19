# add this to your STM32 project's top-level CMakeFiles.txt:
# add_subdirectory(embedded_utils/lib)

target_sources(${BUILD_NAME} PRIVATE
    # put source files here
    i2c_utils.cpp
    spi_utils.cpp
    usart_utils.cpp
    restricted_base.cpp
    timer_manager.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE 
    ../include

)