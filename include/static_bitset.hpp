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

namespace noarch::containers
{

#define NO_OF_BITS (8 * sizeof(unsigned int))


template<size_t SIZE>
class StaticBitset {
  private:
    int array[SIZE];

  public:
    StaticBitset();
    void set(unsigned int);
    void reset(unsigned int);
    void flip(unsigned int);
    int test(unsigned int);
};

template<size_t SIZE>
StaticBitset<SIZE>::StaticBitset() {
  memset(array, 0, sizeof(array));
}

template<size_t SIZE>
USED_API void StaticBitset<SIZE>::set(unsigned int k) {
  array[k/NO_OF_BITS] |= (1U << (k%NO_OF_BITS));
}

template<size_t SIZE>
USED_API void StaticBitset<SIZE>::reset(unsigned int k) {
  array[k/NO_OF_BITS] &= ~(1U << (k%NO_OF_BITS));
}

template<size_t SIZE>
USED_API int StaticBitset<SIZE>::test(unsigned int k) {
  return ( (array[k/NO_OF_BITS] & (1U<<(k%NO_OF_BITS))) != 0 );
}

template<size_t SIZE>
USED_API void StaticBitset<SIZE>::flip(unsigned int k) {
  array[k/NO_OF_BITS] ^= (1U <<(k%NO_OF_BITS));
}

} // namespace noarch::containers

#endif // __STATIC_BITSET_HPP__