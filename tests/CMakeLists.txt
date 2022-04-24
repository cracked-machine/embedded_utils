target_sources(${BUILD_NAME} PRIVATE
    catch_byte_utils.cpp
    catch_bitset_utils.cpp
    catch_timer_manager.cpp
    catch_i2c_utils.cpp
    catch_spi_utils.cpp
    catch_usart_utils.cpp
    catch_static_map.cpp
    catch_static_string.cpp

    mocks/mock_tim.cpp
    mocks/mock_i2c.cpp
    mocks/mock_fuse.cpp

)

target_include_directories(${BUILD_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/tests/mocks
    ${CMAKE_SOURCE_DIR}/include

)
