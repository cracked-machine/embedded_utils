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

// https://godbolt.org/z/166f51dEb
namespace noarch::containers
{
template<size_t SIZE>
class StaticBitset 
{

public:
    StaticBitset();
    StaticBitset(unsigned long long value);

    bool set(unsigned long long pos, const bool value = true);    
    void reset(unsigned long long pos);

    StaticBitset<SIZE>& flip(unsigned long long);
    StaticBitset<SIZE>& flip();

    bool test(unsigned long long pos) const;

    size_t size() const { return m_word.size(); }

    bool operator== (const StaticBitset<SIZE> &rhs) const;
    bool operator!= (const StaticBitset<SIZE> &rhs) const;
    constexpr bool operator[] (size_t pos) const;
    
    StaticBitset<SIZE>& operator>>= (size_t pos);
    StaticBitset<SIZE>& operator<<= (size_t pos);

private:
    static const unsigned long long bits_per_word = std::numeric_limits<unsigned char>::digits * sizeof(unsigned long long);
    std::array<unsigned long long, SIZE> m_word{0};
    bool get(unsigned long long pos, bool &value);
};

template<size_t SIZE>
USED_API StaticBitset<SIZE>::StaticBitset() 
{
}

template<size_t SIZE>
USED_API StaticBitset<SIZE>::StaticBitset([[maybe_unused]] unsigned long long value) 
: m_word{value}
{
}

template<size_t SIZE>
USED_API bool StaticBitset<SIZE>::set(unsigned long long pos, const bool value) 
{
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word; 
    if (pos >= SIZE) { return false; }
    if (value)
    {

        m_word[wshift] = m_word[wshift] | (1U << (offset));
    }
    else
    {
        m_word[wshift] = m_word[wshift] & ~(1U << (offset));
    }
    return true;
}

template<size_t SIZE>
USED_API bool StaticBitset<SIZE>::get(unsigned long long pos, bool &value) 
{
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word; 
    if (pos >= SIZE) { return false; }

    value = m_word[wshift] & (1U << (offset));
    return true;
}

template<size_t SIZE>
USED_API void StaticBitset<SIZE>::reset(unsigned long long pos) 
{
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word;       
    m_word[wshift] &= ~(1U << (offset));
}

template<size_t SIZE>
USED_API bool StaticBitset<SIZE>::test(unsigned long long pos) const
{
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word;     
    return ( (m_word[wshift] & (1U << (offset))) != 0 );
}

template<size_t SIZE>
USED_API StaticBitset<SIZE>& StaticBitset<SIZE>::flip(unsigned long long pos) 
{
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word; 
    m_word[wshift] ^= (1U << (offset));
    return *this;
}

template<size_t SIZE>
USED_API StaticBitset<SIZE>& StaticBitset<SIZE>::flip() 
{
    for (size_t pos = 0; pos < m_word.size(); pos++)
    {    
        const size_t wshift = pos / bits_per_word;
        const size_t offset = pos % bits_per_word;        
        m_word[wshift] ^= (1U << (offset));
    }
    return *this;
}

template<size_t SIZE>
USED_API bool StaticBitset<SIZE>::operator== (const StaticBitset<SIZE> &rhs [[maybe_unused]]) const
{
    for (size_t pos = 0; pos < m_word.size(); pos++)
    {
        if (rhs.test(pos) != this->test(pos)) { return false; }
    }
    return true;
}

template<size_t SIZE>
USED_API bool StaticBitset<SIZE>::operator!= (const StaticBitset<SIZE> &rhs [[maybe_unused]]) const
{
    if (rhs == *this) { return false; }
    else { return true; }
}

template<size_t SIZE>
USED_API constexpr bool StaticBitset<SIZE>::operator[] (size_t pos) const
{ 
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word;
    return (m_word[wshift] & (1U << (offset)));
}


template<size_t SIZE>
USED_API StaticBitset<SIZE>& StaticBitset<SIZE>::operator<<= (size_t pos)
{
    const size_t wshift = pos / bits_per_word;
    const size_t offset = pos % bits_per_word;    
    if (offset == 0)
    {
        for (size_t idx = m_word.size() - 1; idx >= wshift; --idx)
        {   
            bool wshifted;
            this->get(idx - wshift, wshifted);
            this->set(idx, wshifted);
        }
    }
    else
    {
        const size_t sub_offset = bits_per_word - offset;
        for (size_t idx = m_word.size() - 1; idx >= wshift; --idx)
        {
            bool wshifted {false};
            this->get(idx - wshift, wshifted);
            bool wshifted_1 {false};
            this->get(idx - wshift -1, wshifted_1);
            this->set(idx, ((wshifted << offset) | (wshifted_1 << sub_offset))); 
            // this->set(idx, ((this[idx - wshift] << offset) | (this[idx - wshift -1] >> sub_offset)));
        }
    }
    return *this;
}

template<size_t SIZE>
USED_API StaticBitset<SIZE>& StaticBitset<SIZE>::operator>>= (size_t pos [[maybe_unused]])
{
    return *this;
}


} // namespace noarch::containers

#endif // __STATIC_BITSET_HPP__