target_sources(${BUILD_NAME} PRIVATE
    catch_byte_utils.cpp
    catch_bitset_utils.cpp
    catch_static_map.cpp
)

target_include_directories(${BUILD_NAME} PRIVATE 
    .
)
