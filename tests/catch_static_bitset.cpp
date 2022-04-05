#include <catch2/catch_all.hpp>
#include <static_bitset.hpp>
#include <iostream>
#include <vector>


template<size_t SIZE> class StaticBitset;

TEST_CASE("static_bitset - check zero initialization", "[static_bitset]")
{
    std::cout << "static_bitset - check zero initialization" << std::endl;
    const size_t bitset_size {256};
    noarch::containers::StaticBitset<bitset_size> bits;

    // check internal array is correctly zero-initialised
    for (size_t idx = 0; idx < bits.size(); idx++)
    {
        REQUIRE(bits[idx] == 0);
    }    

}

TEST_CASE("static_bitset - check set() input sanitize", "[static_bitset]")
{
    std::cout << "static_bitset - check set() input sanitize" << std::endl;
    const size_t bitset_size {256};
    noarch::containers::StaticBitset<bitset_size> bits;

    // check the set function input length is sanitized
    REQUIRE(bits.size() == bitset_size);
    REQUIRE(bits.set(bitset_size - 1, true));
    REQUIRE_FALSE(bits.set(bitset_size, true));

}

TEST_CASE("static_bitset - set value with ctor", "[static_bitset]")
{
    std::cout << "static_bitset - set value with ctor" << std::endl;
    const size_t bitset_size {256};
    std::vector<bool> test_samples;

    SECTION("Initialise and verify partial capcity")
    {
        // setup test test_samples for 1010 1010 1010 1010 and the rest zero-initialized.
        const size_t partial_size {16};
        for (size_t idx = 0; idx < partial_size; idx++)
        {
            if ( idx % 2) { test_samples.push_back(true); }
            else { test_samples.push_back(false); }
        }
        for (size_t idx = partial_size; idx < bitset_size; idx++)
        {
            test_samples.push_back(false);            
        }        

        noarch::containers::StaticBitset<bitset_size> bits(0xAAAA);
        REQUIRE(bits.size() == bitset_size);
        for (size_t idx = 0; idx < bits.size(); idx++)
        {
            std::cout << bits[idx];
            // REQUIRE(bits[idx] == test_samples[idx]);
        }        
        std::cout << std::endl;
        for (size_t idx = 0; idx < test_samples.size(); idx++)
        {
            std::cout << test_samples[idx];
            // REQUIRE(bits[idx] == test_samples[idx]);
        }                
    }
    
}