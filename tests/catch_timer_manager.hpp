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


#ifndef __CATCH_TIMER_MANAGER_HPP__
#define __CATCH_TIMER_MANAGER_HPP__

#include <timer_manager.hpp>
#include <mock_cmsis.hpp>
#include <thread>
#include <iostream>
#include <chrono>
#include <future>

/// @brief Simulate the SysTick counter (normally done by ARM HW).
/// See "Cortex-M0 Technical Ref Man - SysTick Control and Status Register"
/// @param systick 
/// @return false if null input, return true when fixture has terminated
bool mock_systick(SysTick_Type *systick);

/// @brief Simulate the timer counter (normally done by STM32 HW) 
/// See STM32 Reference Manal Timer control register 1
/// @param timer The mocked STM32 registers
/// @return false if timer is null
bool mock_timer_count(TIM_TypeDef *timer);


#endif // __CATCH_TIMER_MANAGER_HPP__