#include <catch2/catch_all.hpp>
#include <static_bitset.hpp>


template<size_t SIZE> class StaticBitset;

TEST_CASE("static_bitset", "[static_bitset]")
{
    noarch::containers::StaticBitset<8> sb;
    REQUIRE(true);
}