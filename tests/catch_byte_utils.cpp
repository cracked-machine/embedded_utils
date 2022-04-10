#include <catch2/catch_all.hpp>

#include <byte_utils.hpp>
#include <algorithm>
#include <mock.hpp>

// enforce code coverage with explicit instances of func templates so that linker does not drop references
namespace noarch::byte_manip
{
template bool print_bytes(std::array<uint8_t, 1> &bytes);
}

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