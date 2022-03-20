#include <catch2/catch_all.hpp>
#include <bitset_utils.hpp>
#include <byte_utils.hpp>

/// @brief check order is reversed as expected
TEST_CASE("bitset_to_bytearray - check MSB/LSB integrity", "[bitset_utils]")
{
    std::bitset<16> input_16bits(0xAAAA);                   //  10101010 10101010
    std::array<uint8_t, 2> expected_2byte{0x55, 0x55};      //  01010101 01010101
    std::array<uint8_t, 2> output_2byte;    
    REQUIRE(noarch::bit_manip::bitset_to_bytearray(output_2byte, input_16bits));
    // noarch::byte_manip::print_bytes(output_2byte);
    // noarch::byte_manip::print_bytes(expected_2byte);
    REQUIRE(output_2byte == expected_2byte);

}

/// @brief Check input bits are truncated if output byte array is too small
TEST_CASE("bitset_to_bytearray - param size mismatch: bits > bytes TRUNCATION", "[bitset_utils]")
{
    std::bitset<8> input_16bits(0xAAAA);                   //  10101010 10101010
    std::array<uint8_t, 1> expected_2byte{0x55};      //  01010101 01010101
    std::array<uint8_t, 1> output_2byte;    
    REQUIRE(noarch::bit_manip::bitset_to_bytearray(output_2byte, input_16bits));
    // noarch::byte_manip::print_bytes(output_2byte);
    // noarch::byte_manip::print_bytes(expected_2byte);    
    REQUIRE(output_2byte == expected_2byte);

}

/// @brief Check input bits are xero-padded if output byte array is too large
TEST_CASE("bitset_to_bytearray - param size mismatch: bytes > bits ZERO PADDING", "[bitset_utils]")
{
    std::bitset<8> input_16bits(0xAA);                      //  10101010 10101010
    std::array<uint8_t, 2> expected_2byte{0x55, 0x00};      //  01010101 00000000
    std::array<uint8_t, 2> output_2byte;    
    REQUIRE(noarch::bit_manip::bitset_to_bytearray(output_2byte, input_16bits));
    // noarch::byte_manip::print_bytes(output_2byte);
    // noarch::byte_manip::print_bytes(expected_2byte);
    REQUIRE(output_2byte == expected_2byte);
    
}