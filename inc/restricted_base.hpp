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

#ifndef __RESTRICTED_BASE_HPP__
#define __RESTRICTED_BASE_HPP__

#include <new>

// All dyanmic allocation is disabled
class RestrictedBase
{

public: 
    RestrictedBase() = default;
    
    // prevent dynamic allocation of this class
    void* operator new(std::size_t size) noexcept = delete;
    void* operator new[](std::size_t size) noexcept = delete;
    void operator delete(void* ptr) noexcept = delete;
    void operator delete[](void* ptr) noexcept = delete;

    // prevent copy of this class
    RestrictedBase &operator=(const RestrictedBase&) = delete;
    RestrictedBase(const RestrictedBase&) = delete;

    // prevent move of this class
    RestrictedBase &operator=(RestrictedBase&& other) = delete;
    RestrictedBase(RestrictedBase&& other) = delete;

    ~RestrictedBase() = default;

};

// @brief Trap any global use of operator new (including *some* C++ STL) 
// so it can be caught by debugger
void invalid_allocation_error_handler();

// This definition must be in header so it can be seen 
// (and must be inline to prevent multiple definition linker errors)
inline void * operator new (std::size_t)  
{
    void *p = nullptr;
    invalid_allocation_error_handler();
    return p;
}





#endif // __RESTRICTED_BASE_HPP__