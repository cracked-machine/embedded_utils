// MIT License

// Copyright (c) 2022 Chris Sutton

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __BITSET_UTILS_HPP__
#define __BITSET_UTILS_HPP__

// used for arm target debug mode only.
#ifdef USE_RTT
  #include <SEGGER_RTT.h>
#endif

#include <stdint.h>
#include <bitset>
#include <array>

namespace noarch::bit_manip
{


// @brief Adds source std::bitset to target std::bitset with msb_offset
// @tparam TARGET_SIZE The size of the source bitset container
// @tparam SOURCE_SIZE The size of the target bitset container
// @param target The target bitset container to copy into 
// @param source The source bitset container to copy from
// @param msb_offset insertion index starting from the right-most position
template<std::size_t TARGET_SIZE, std::size_t SOURCE_SIZE> 
bool insert_bitset_at_offset(  std::bitset<TARGET_SIZE> &target,  
                                        const std::bitset<SOURCE_SIZE> &source, 
                                        const uint16_t &msb_offset) 
{
    // protect against oversized msb_offset or SOURCE params
    if (msb_offset + SOURCE_SIZE > TARGET_SIZE)
    {
        return false;
    }
    // iterate over the source bitset pattern
    for (uint16_t idx = 0; idx < source.size(); idx++)
    {
        // start from the common register msb and work backwards towards lsb,
        // ...minus the offset
        if (source.test(idx))
        {
            // don't use std::bitset.set(), this will force exception handling to bloat the linked .elf
            target[msb_offset + (source.size() - 1)  - idx] = true;
        }
        else
        {
            // don't use std::bitset.set(), this will force exception handling to bloat the linked .elf
            target[msb_offset + (source.size() - 1) - idx] = false;
        }
    }
    return true;
}

// @brief Converts bits to same sized byte array LSB first. 0101 becomes 1010. 
// Oversized bitsets are truncated, undersized bitsets are zero-padded
// @tparam TARGET_SIZE The size of the target_array std::array
// @tparam SOURCE_SIZE The size of the source_bitset std::bitset
// @param target_array The std::array object copied to. Caution, all pre-existing contents is destroyed.
// @param source_bitset The std::bitset object copied from. 
template<std::size_t TARGET_SIZE, std::size_t SOURCE_SIZE> 
bool bitset_to_bytearray(std::array<uint8_t, TARGET_SIZE> &target_array, const std::bitset<SOURCE_SIZE> &source_bitset)
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
            else
            {
                target_array[byte_array_idx] |= 0;
                bit_offset_within_byte--;
            }
        }        
    }
    return true;
}

// @brief Print out the provided bitset as bytes
// @param pattern The bitset to print
template<std::size_t BITSET_SIZE>
void print_bits(std::bitset<BITSET_SIZE> &pattern [[maybe_unused]])
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
        SEGGER_RTT_printf(0, "\n");       
    #endif
    #ifdef X86_UNIT_TESTING_ONLY
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
            std::cout << std::noboolalpha << pattern.test(idx);
        }
        std::cout << std::endl;
    #endif
}

} // namespace noarch::bit_manip


#endif // __BITSET_UTILS_HPP__