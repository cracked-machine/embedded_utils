#include <catch2/catch_all.hpp>
#include <static_string.hpp>
#include <vector>
#include <iostream>
#include <functional>
#include <array>

using namespace noarch::containers;

// enforce code coverage with explicit instances of func templates so that linker does not drop references
template StaticString<1>::StaticString();
template StaticString<1>::StaticString(const char (&str)[1]);
template std::size_t StaticString<1>::size();
template char& StaticString<1>::operator[](std::size_t);
template std::array<char, 1>& StaticString<1>::array(void);
template void StaticString<1>::concat<1>(int offset, const char (&str)[1]);
template void StaticString<1>::concat_int(int offset, int number);
template void StaticString<1>::concat<1>(int offset, const std::array<char, 1>&);
template void StaticString<1>::concat<1>(int offset, StaticString<1>&);


TEST_CASE("static_string - check constructor string literal", "[static_string]")
{
    std::cout << "static_string - check constructor string literal" << std::endl;
    std::vector<char> expected_chars {'H', 'e', 'l', 'l', 'o', '\000'};
    StaticString<6> test_string ("Hello");
    
    for (std::size_t idx = 0; idx < test_string.size(); idx++)
    {
       REQUIRE(test_string[idx] == expected_chars[idx]);
    }
}

TEST_CASE("static_string - check initialisor-list string literal", "[static_string]")
{
    std::cout << "static_string - check initialisor-list string literal" << std::endl;
    std::vector<char> expected_chars {'H', 'e', 'l', 'l', 'o', '\000'};
    
    StaticString<6> test_string {"Hello"};
    REQUIRE(test_string.size() == 6);

    for (std::size_t idx = 0; idx < test_string.size(); idx++)
    {
       REQUIRE(test_string[idx] == expected_chars[idx]);
    }
}


TEST_CASE("static_string - check constructor empty", "[static_string]")
{
    std::cout << "static_string - check constructor empty" << std::endl;
    StaticString<6> test_string;
    REQUIRE(test_string.size() == 6);
    for (std::size_t idx = 0; idx < test_string.size(); idx++)
    {
        REQUIRE(test_string[idx] == ' ');
    }
}

TEST_CASE("static_string - check access operator", "[static_string]")
{
    std::cout << "static_string - check access operator" << std::endl;
    StaticString<6> test_string;
    for (std::size_t idx = 0; idx < test_string.size(); idx++)
    {
        test_string[idx] = static_cast<uint8_t>(idx);
    }    

    for (std::size_t idx = 0; idx < test_string.size(); idx++)
    {
        REQUIRE(test_string[idx] == static_cast<uint8_t>(idx));
    }
    // overrunning the storage buffer should default to return of the last char in the buffer
    REQUIRE(test_string[test_string.size()] == test_string[test_string.size() - 1]);
}



