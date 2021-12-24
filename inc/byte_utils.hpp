// MIT License

// Copyright (c) 2021 Chris Sutton

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

#ifndef __BYTE_UTILS_HPP__
#define __BYTE_UTILS_HPP__

#include <stdint.h>


namespace embed_utils
{

namespace byte_manip
{

template<std::size_t BYTE_ARRAY_SIZE> 
void print_bytes(std::array<uint8_t, BYTE_ARRAY_SIZE> &bytes __attribute__((unused)))
{
    #ifdef USE_RTT
        for (uint16_t idx = 0; idx < bytes.size(); idx++)
        {
            
                if (idx % 16 == 0)
                    SEGGER_RTT_printf(0, "\n"); 

                SEGGER_RTT_printf(0, "0x%02x ", +bytes[idx]);
            
        }
        SEGGER_RTT_printf(0, "\n");       
    #endif    
    
}

}   // namespace byte_manip

}   // namespace embed_utils

#endif // __BYTE_UTILS_HPP__
