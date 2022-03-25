#include <catch2/catch_all.hpp>
#include <bitset_utils.hpp>
#include <byte_utils.hpp>

/// @brief insert bit pattern starting from zero msb_offset argument
TEST_CASE("insert_bitset_at_offset - zero msb_offset", "[bitset_utils]")
{
    const size_t target_size{8};
    const size_t source_size{4};

    std::bitset<target_size> target("00000000");
    std::bitset<source_size> source("1111");

    std::bitset<target_size> expected_output("00001111");
    
    REQUIRE(noarch::bit_manip::insert_bitset_at_offset(target, source, 0));
    REQUIRE(target == expected_output);
}

/// @brief Insert at -1 offset (wraps around to 65535, which is handled by input checks)
TEST_CASE("insert_bitset_at_offset - msb_offset wraparound", "[bitset_utils]")
{
    const size_t target_size{8};
    const size_t source_size{4};

    std::bitset<target_size> target("00000000");
    std::bitset<source_size> source("1111");

    std::bitset<target_size> expected_output("00001111");
    std::bitset<target_size> original_target(target);

    // operation fails, target is not undated
    REQUIRE_FALSE(noarch::bit_manip::insert_bitset_at_offset(target, source, -1));
    REQUIRE_FALSE(target == expected_output);
    REQUIRE(target == original_target);
}

/// @brief Insert at offset larger than target bitset
TEST_CASE("insert_bitset_at_offset - oversized msb_offset", "[bitset_utils]")
{
    const size_t target_size{8};
    const size_t source_size{4};

    std::bitset<target_size> target("00000000");
    std::bitset<source_size> source("1111");

    std::bitset<target_size> expected_output("00001111");
    std::bitset<target_size> original_target(target);

    // operation fails, target is not undated
    REQUIRE_FALSE(noarch::bit_manip::insert_bitset_at_offset(target, source, target_size + 1));
    REQUIRE_FALSE(target == expected_output);
    REQUIRE(target == original_target);
}

/// @brief Insert at offset within tolerances for SOURCE to fit within TARGET
TEST_CASE("insert_bitset_at_offset - offset_index_at_limit_for_source_size", "[bitset_utils]")
{
    const size_t target_size{8};
    const size_t source_size{4};
    const size_t offset_index_at_limit_for_source_size = target_size - source_size;

    std::bitset<target_size> target("00000000");
    std::bitset<source_size> source("1111");

    std::bitset<target_size> expected_output("11110000");

    REQUIRE(noarch::bit_manip::insert_bitset_at_offset(target, source, offset_index_at_limit_for_source_size));
    REQUIRE(target == expected_output);

}

/// @brief Insert at offset that surpasses tolerances for SOURCE to fit within TARGET
TEST_CASE("insert_bitset_at_offset - offset_index_too_large_for_source_size", "[bitset_utils]")
{
    const size_t target_size{8};
    const size_t source_size{4};
    const size_t offset_index_too_large_for_source_size = target_size-(source_size - 1);

    std::bitset<target_size> target("00000000");
    std::bitset<source_size> source("1111");

    std::bitset<target_size> expected_output("00001111");
    std::bitset<target_size> original_target(target);

    // operation fails, target is not undated
    REQUIRE_FALSE(noarch::bit_manip::insert_bitset_at_offset(target, source, offset_index_too_large_for_source_size));
    REQUIRE_FALSE(target == expected_output);
    REQUIRE(target == original_target);
}

/// @brief SOURCE is too large to fit within TARGET
TEST_CASE("insert_bitset_at_offset - ", "[bitset_utils]")
{
    const size_t target_size{8};
    const size_t source_size{9};

    std::bitset<target_size> target("00000000");
    std::bitset<source_size> source("1111");

    std::bitset<target_size> expected_output("00001111");
    std::bitset<target_size> original_target(target);

    // operation fails, target is not undated
    REQUIRE_FALSE(noarch::bit_manip::insert_bitset_at_offset(target, source, 0));
    REQUIRE_FALSE(target == expected_output);
    REQUIRE(target == original_target);
}


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