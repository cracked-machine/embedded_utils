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
#include <limits>

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

    /// @brief Default construct and fill the object with hypens. 
    StaticString() 
    {
        std::memset(m_string.data(), m_space, CAPACITY);
    }

    /// @brief Construct with literal string.
    /// @param str The c-string literal input. Must be of length = CAPACITY-1
    // Not explicit. Allow implicit conversion from 'const char [CAPACITY]' to 'StaticString<CAPACITY>&'
    // cppcheck-suppress noExplicitConstructor
    StaticString(const char (&str)[CAPACITY])
    {
        std::memcpy(m_string.data(), str, m_string.size());
    }

    /// @brief Convenience function to get the underlying size of the "string"
    /// @return size_t
    std::size_t size() { return m_string.size(); }

    /// @brief overloaded access operator. 
    /// @param idx The position of the string character.
    /// Passing an idx higher than m_string.size() will always return the last character in the buffer
    /// @return The character of the given idx. 
    char& operator[](size_t idx) 
    { 
        if (idx < m_string.size()) { return m_string[idx]; }
        else { return m_string[m_string.size()-1]; }
    }

    /// @brief Get the std::array member
    /// @return string_t& 
    string_t& array() { return m_string; }

    /// @brief concat a string literal
    /// @tparam SIZE The size of the literal
    /// @param offset The offset position in the string to add the literal
    /// @param str The string literal
    template<std::size_t SIZE>
    void concat(int offset, const char (&str)[SIZE]);

    /// @brief Add all std::array arg, starting at offset.
    /// Note, characters extending past the CAPACITY limit will be truncated.
    /// @tparam SIZES size of the StaticString parameters.
    /// @param offset add from this index position onwards. 
    /// @param static_str The StaticString object to add. Must not be const.
    template <std::size_t... SIZES>
    void concat(int offset, StaticString<SIZES>&... static_strings);

    /// @brief Add all StaticString arg, starting at offset.
    /// Note, characters extending past the CAPACITY limit will be truncated.
    /// @tparam SIZES size of the std::array parameters.
    /// @param offset add from this index position onwards. 
    /// @param array The std::array object to add.
    template <std::size_t... SIZES>
    void concat(int offset, const std::array<char, SIZES>&... arrays);

    /// @brief concat an integer into the string
    /// @tparam WIDTH The integer width, 8-, 16-, 32-bit
    /// @param offset The offset position in the string to add the integer
    /// @param number The integer value. 
    template<typename WIDTH>
    void concat_int(int offset, WIDTH number);


private:
    /// @brief The string data
    string_t m_string;

    static constexpr uint8_t m_space {0x20};
    static constexpr uint8_t m_delimit {0x1F};

};

template <std::size_t CAPACITY>
template<std::size_t SIZE>
void StaticString<CAPACITY>::concat(int offset, const char (&str)[SIZE])
{
    // if input size and offset exceed output bounds then crop is a positive value
    int16_t crop = (SIZE + offset) - m_string.size();
    // otherwise crop is negative, so don't use it    
    if (crop < 0) { crop = 0; }

    // std::copy_n(str, SIZE - crop, m_string.begin() + offset);
    std::memcpy(m_string.begin() + offset, str, SIZE - crop);
}

template <std::size_t CAPACITY>
template <std::size_t... SIZES>
void StaticString<CAPACITY>::concat(int offset, StaticString<SIZES>&... static_strings)
{
    uint8_t size{SIZES...};
    // if input size and offset exceed output bounds then crop is a positive value
    int16_t crop = (size + offset) - m_string.size();
    // otherwise crop is negative, so don't use it
    if (crop < 0) { crop = 0; }

    std::size_t index{0};
    // ((std::copy_n(static_strings.array().begin(), SIZES, m_string.begin() + offset + index), index += SIZES), ...);
    ((std::memcpy(m_string.begin() + offset + index, static_strings.array().begin(), SIZES - crop), index += SIZES), ...);
} 

template <std::size_t CAPACITY>
template <std::size_t... SIZES>
void StaticString<CAPACITY>::concat(int offset, const std::array<char, SIZES>&... arrays)
{
    uint8_t size{SIZES...};
    // if input size and offset exceed output bounds then crop is a positive value
    int16_t crop = (size + offset) - m_string.size();
    // otherwise crop is negative, so don't use it
    if (crop < 0) { crop = 0; }

    std::size_t index{0};

    // ((std::copy_n(arrays.begin(), SIZES - crop, m_string.begin() + offset + index), index += SIZES), ...);
    ((std::memcpy(m_string.begin() + offset + index, arrays.begin(), SIZES - crop), index += SIZES), ...);
}      


template <std::size_t CAPACITY>
template<typename WIDTH>
void StaticString<CAPACITY>::concat_int(int offset, WIDTH number)
{
    // create a temp storage array based on the max number digits for this number
    std::array<char, std::numeric_limits<WIDTH>::digits10 + 1> temp;
    // fill it with an empty marker '#'
    std::memset(temp.data(), '#', temp.size());
    // now extract each digit from 'number' and add it to the temp storage array
    for (uint8_t extract_idx = 0; number > 0; extract_idx++)
    {
        switch(number % 10)
        {
            case 0:
                temp[extract_idx] = '0';
                break;
            case 1:
                temp[extract_idx] = '1';
                break;
            case 2:
                temp[extract_idx] = '2';
                break;                
            case 3:
                temp[extract_idx] = '3';
                break;
            case 4:
                temp[extract_idx] = '4';
                break;
            case 5:
                temp[extract_idx] = '5';
                break;
            case 6:
                temp[extract_idx] = '6';
                break;
            case 7:
                temp[extract_idx] = '7';
                break;
            case 8:
                temp[extract_idx] = '8';
                break;
            case 9:
                temp[extract_idx] = '9';
                break;
        }

        number /= 10;
    }

    // temp storage array is in reverse order. Correct the order and 
    // filter out any '#' empty markers before writing char into m_string.
    size_t forward_idx = offset;
    for(int8_t reverse_idx = temp.size()-1; reverse_idx > -1; reverse_idx--)
    {
        if (temp[reverse_idx] != '#')
        {   
            m_string[forward_idx] = temp[reverse_idx];
            forward_idx++;           
        }
    }
}


} // namespace noarch::containers

#endif // __STATIC_STRING_HPP__