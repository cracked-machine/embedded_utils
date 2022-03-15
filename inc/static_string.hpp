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

#ifndef __STATIC_STRING_HPP__
#define __STATIC_STRING_HPP__

#include <array>
#include <cstring>

// https://godbolt.org/z/r4TevWxcz

namespace noarch::containers
{

/// @brief Statically allocated string using std::array
/// @tparam CAPACITY Should be n+1 to allow for null-termination. This size cannot be exceeded once set.
template<std::size_t CAPACITY>
class StaticString
{
public:
    // convenience alias
    using string_t = std::array<char, CAPACITY>;
    
    StaticString() 
    {
        std::memset(m_string.data(), 45, CAPACITY);
    }

    /// @brief Construct a StaticString object from literal c-string
    /// @param str The c-string literal input
    StaticString(const char (&str)[CAPACITY])
    {
        std::memcpy(m_string.data(), str, m_string.size());
    }

    /// @brief Add all std::array arg, starting at offset.
    /// Note, characters extending past the CAPACITY limit will be truncated.
    /// @tparam SIZES size of the StaticString parameters.
    /// @param offset add from this index position onwards. 
    /// @param static_str The StaticString object to add. Must not be const.
    template <std::size_t... SIZES>
    void join(int offset, StaticString<SIZES>&... static_strings)
    {
        uint8_t size{SIZES...};
        // if input size and offset exceed output bounds then crop is a positive value
        int16_t crop = (size + offset) - m_string.size();
        // otherwise crop is negative, so don't use it
        if (crop < 0) { crop = 0; }

        std::size_t index{0};
        ((std::copy_n(static_strings.array().begin(), SIZES, m_string.begin() + offset + index), index += SIZES), ...);
    }     

    /// @brief Add all StaticString arg, starting at offset.
    /// Note, characters extending past the CAPACITY limit will be truncated.
    /// @tparam SIZES size of the std::array parameters.
    /// @param offset add from this index position onwards. 
    /// @param array The std::array object to add.
    template <std::size_t... SIZES>
    void join(int offset, const std::array<char, SIZES>&... arrays)
    {
        uint8_t size{SIZES...};
        // if input size and offset exceed output bounds then crop is a positive value
        int16_t crop = (size + offset) - m_string.size();
        // otherwise crop is negative, so don't use it
        if (crop < 0) { crop = 0; }

        std::size_t index{0};

        ((std::copy_n(arrays.begin(), SIZES - crop, m_string.begin() + offset + index), index += SIZES), ...);
    }      

    template<std::size_t SIZE>
    void join(int offset, const char (&str)[SIZE])
    {
        // if input size and offset exceed output bounds then crop is a positive value
        int16_t crop = (SIZE + offset) - m_string.size();
        // otherwise crop is negative, so don't use it    
        if (crop < 0) { crop = 0; }

        std::copy_n(str, SIZE - crop, m_string.begin() + offset);
    }

    /// @brief Set the num object
    /// @tparam ANY_UNSIGNED_INT 
    /// @param value 
    template<typename ANY_UNSIGNED_INT>
    void set_num(ANY_UNSIGNED_INT value)
    {
        put_digit_at(value, 0);
        std::reverse(m_string.begin(), m_string.end());
    }

    /// @brief Get the std::array member
    /// @return string_t& 
    string_t& array() { return m_string; }
private:
    
    string_t m_string;

    void put_digit_at(int val, uint8_t offset)
    {
        offset++;
        if(val >= 10)
            put_digit_at(val / 10, offset);

        uint8_t digit = val % 10;
        if (offset < m_string.size())
            m_string.at(offset) = digit;
        
    }       
};

} // namespace noarch::containers

#endif // __STATIC_STRING_HPP__