#include <catch2/catch_all.hpp>

#include <byte_utils.hpp>
#include <algorithm>

TEST_CASE("Empty byte array", "[byte_utils]")
{
    std::array<uint8_t, 0> bytes;
    REQUIRE_FALSE(noarch::byte_manip::print_bytes(bytes));
}

TEST_CASE("Initialised byte array", "[byte_utils]")
{
    const size_t array_size{64};
    std::array<uint8_t, array_size> input_bytes;
    std::fill(input_bytes.begin(), input_bytes.end(), 10);
    REQUIRE(noarch::byte_manip::print_bytes(input_bytes));
}