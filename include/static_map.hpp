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


#ifndef __STATIC_MAP_HPP__
#define __STATIC_MAP_HPP__

#include <array>

// @brief For working example see https://godbolt.org/z/deza1Ecnn

namespace noarch::containers
{

// @brief Associative container with contains key-value pairs that is allocated at compile-time
// @tparam Key The Key
// @tparam Value The Value
// @tparam Size The size of the map/number of Key/Value pairs. Must be constant.
template <typename Key, typename Value, std::size_t Size>
struct StaticMap {
    
    // @brief The dictionary
    std::array<std::pair<Key, Value>, Size> data;

    // @brief access specified element
    // @param key The key element to match
    // @return Value* Pointer to the value element, or nullptr if not found
    Value* find_key(const Key &key) {

        for (std::pair<Key, Value> &pair : data)
        {
            if (pair.first == key)
            {
                return &pair.second;
            }
        }
        // or return nullptr as the search completed without match
        return nullptr;
    }
};

} // namespace noarch::containers

#endif // __STATIC_MAP_HPP__
