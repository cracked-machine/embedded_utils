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

// https://godbolt.org/z/oTYo448zG

namespace noarch::containers
{

/// @brief Statically allocated string using std::array
/// @tparam SIZE Should be n+1 to allow for null-termination. This size cannot be exceeded once set.
template<std::size_t SIZE>
class StaticString
{
public:
    // convenience alias
    using string_t = std::array<char, SIZE>;
    
    /// @brief Construct a StaticString object from literal c-string
    /// @param str The c-string literal input
    StaticString(const char str[SIZE])
    {
        std::memcpy(m_string.data(), str, m_string.size());
    }

    void set(const char str[SIZE])
    {
        std::memcpy(m_string.data(), str, m_string.size());
    }    

    /// @brief Get the std::array member
    /// @return string_t& 
    string_t& array() { return m_string; }
private:
    
    string_t m_string;
};

} // namespace noarch::containers

#endif // __STATIC_STRING_HPP__