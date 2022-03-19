
#include <catch2/catch_all.hpp>

#include <byte_utils.hpp>

TEST_CASE("Test Embedded Utils", "[embedded_utils]")
{
    std::array<uint8_t, 8> bytes;
    noarch::byte_manip::print_bytes(bytes);
    REQUIRE(true);
}