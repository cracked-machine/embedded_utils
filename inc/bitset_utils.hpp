
#ifndef __BITSET_UTILS_HPP__
#define __BITSET_UTILS_HPP__

// used for x86-based testing
#ifdef USE_COUT
    #include <iostream>
#endif

// used for arm target debug mode only.
#ifdef USE_RTT
  #include <SEGGER_RTT.h>
#endif

#include <stdint.h>
#include <bitset>
#include <array>

namespace embedded_utils
{

class bitset_utils
{
public:
    template<std::size_t TARGET_SIZE, std::size_t SOURCE_SIZE> 
    void add_bitset(std::bitset<TARGET_SIZE> &target,  const std::bitset<SOURCE_SIZE> &source, const uint16_t &msb_offset);
    

    template<std::size_t TARGET_SIZE, std::size_t SOURCE_SIZE> 
    void bitset_to_bytearray(std::array<uint8_t, TARGET_SIZE> &target_array, const std::bitset<SOURCE_SIZE> &source_bitset);  

    
    template<std::size_t BITSET_SIZE>
    void print_bits(std::bitset<BITSET_SIZE> &reg);    
};

template<std::size_t TARGET_SIZE, std::size_t SOURCE_SIZE> 
void bitset_utils::add_bitset(std::bitset<TARGET_SIZE> &target,  const std::bitset<SOURCE_SIZE> &source, const uint16_t &msb_offset) 
{
    // iterate over the source bitset pattern
    for (uint16_t idx = 0; idx < source.size(); idx++)
    {
        // start from the common register msb and work backwards towards lsb,
        // ...minus the offset
        if (source.test(idx))
        {
            target.set(msb_offset + (source.size() - 1)  - idx, true);
        }
        else
        {
            target.set(msb_offset + (source.size() - 1) - idx , false);
        }
    }
}

// @brief Takes each 8bits from source_bitset and puts into successive bytes from target_array.
// User must ensure: target_array.size() > source_bitset.size() * 8.
// @tparam TARGET_SIZE The size of the target_array std::array
// @tparam SOURCE_SIZE The size of the source_bitset std::bitset
// @param target_array The std::array object copied to. Caution, all pre-existing contents is destroyed.
// @param source_bitset The std::bitset object copied from. 
template<std::size_t TARGET_SIZE, std::size_t SOURCE_SIZE> 
void bitset_utils::bitset_to_bytearray(std::array<uint8_t, TARGET_SIZE> &target_array, const std::bitset<SOURCE_SIZE> &source_bitset)
{
    // 8-bit byte
    const uint8_t word_size_bits = 8; 

    // clear the array before starting
    target_array.fill(0);

    // iterate each byte in the array and fill it
    for (uint16_t byte_array_idx = 0; byte_array_idx < target_array.size(); byte_array_idx++)
    {
        // This is the current position within the bitset, relative to the current byte
        uint16_t bit_offset_within_pattern = byte_array_idx * word_size_bits;

        // used to bitshift the individual bits into the current byte
        int8_t bit_offset_within_byte = word_size_bits - 1;

        // iterate the bitset position [n -> n + 8)
        for (uint16_t pattern_idx = bit_offset_within_pattern; pattern_idx < bit_offset_within_pattern + word_size_bits; pattern_idx++)
        {   
            // double check we haven't overshot the input bitset length
            if (pattern_idx < source_bitset.size())
            {

                target_array[byte_array_idx] |= (source_bitset.test(pattern_idx) << bit_offset_within_byte);
                bit_offset_within_byte--;
            }
        }        
    }
}

template<std::size_t BITSET_SIZE>
void bitset_utils::print_bits(std::bitset<BITSET_SIZE> &pattern __attribute__((unused)))
{
    
    #ifdef USE_RTT
        for (uint16_t idx = 0; idx < pattern.size(); idx++)
        {
            if (idx % 8 == 0)
                SEGGER_RTT_printf(0, " ");
            if (idx % 64 == 0)
                SEGGER_RTT_printf(0, "\n");            
            SEGGER_RTT_printf(0, "%u ", +pattern.test(idx));
        }
    #endif
    #ifdef USE_COUT
        for (uint16_t idx = 0; idx < pattern.size(); idx++)
        {
            if (idx % 8 == 0)
            {
                std::cout << " ";
            }
            if (idx % 64 == 0)
            {
                std::cout << std::endl;
            }
            std::cout << pattern.test(idx);
        }
        std::cout << std::endl;
    #endif
}

}  // namespace embedded_utils


#endif // __BITSET_UTILS_HPP__