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

#ifndef __STATIC_BITSET_HPP__
#define __STATIC_BITSET_HPP__

#include <gnuc_ext_defs.hpp>
#include <limits>
#include <array>

namespace noarch::containers
{




template<size_t SIZE>
class StaticBitset 
{
private:
    static const uint32_t bits_u32 = std::numeric_limits<unsigned char>::digits * sizeof(uint32_t);
    std::array<uint32_t, SIZE> array;

public:
    StaticBitset();
    StaticBitset(uint32_t value);
    void set(uint32_t, bool value = true);
    void reset(uint32_t);
    // void flip(uint32_t);
    StaticBitset<SIZE>& flip(uint32_t);
    uint32_t test(uint32_t) const;
    USED_API size_t size() const { return array.size(); }

    USED_API bool operator== (const StaticBitset<SIZE> &rhs) const;
    USED_API uint32_t operator[] (size_t idx) const { return array[idx]; }

};

template<size_t SIZE>
USED_API StaticBitset<SIZE>::StaticBitset() 
{
    memset(array.data(), 0, sizeof(array.data()));
}

template<size_t SIZE>
USED_API StaticBitset<SIZE>::StaticBitset(uint32_t value) 
{
    memset(array.data(), value, sizeof(array.data()));
}

template<size_t SIZE>
USED_API void StaticBitset<SIZE>::set(uint32_t k, bool value) 
{
    if (value)
    {
        array[k / bits_u32] = array[k / bits_u32] | (1U << (k % bits_u32));
    }
    else
    {
        array[k / bits_u32] = array[k / bits_u32] & ~(1U << (k % bits_u32));
    }
    
}

template<size_t SIZE>
USED_API void StaticBitset<SIZE>::reset(uint32_t k) 
{
    array[k / bits_u32] &= ~(1U << (k % bits_u32));
}

template<size_t SIZE>
USED_API uint32_t StaticBitset<SIZE>::test(uint32_t k) const
{
    return ( (array[k / bits_u32] & (1U << (k % bits_u32))) != 0 );
}

// template<size_t SIZE>
// USED_API void StaticBitset<SIZE>::flip(uint32_t k) 
// {
//     array[k / bits_u32] ^= (1U << (k % bits_u32));
// }

template<size_t SIZE>
USED_API StaticBitset<SIZE>& StaticBitset<SIZE>::flip(uint32_t k) 
{
    array[k / bits_u32] ^= (1U << (k % bits_u32));
    return *this;
}

template<size_t SIZE>
USED_API bool StaticBitset<SIZE>::operator== (const StaticBitset<SIZE> &rhs [[maybe_unused]]) const
{
    bool result {true};
    for (uint16_t idx = 0; idx < array.size(); idx++)
    {
        if (rhs[idx] != array[idx])
        {
            result = false;
        }
    }
    return result;
}

} // namespace noarch::containers

#endif // __STATIC_BITSET_HPP__