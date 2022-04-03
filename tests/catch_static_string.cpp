#include <catch2/catch_all.hpp>
#include <static_string.hpp>

TEST_CASE("static_string", "[static_string]")
{
    noarch::containers::StaticString<6> ss("Hello");
    REQUIRE(true);
}