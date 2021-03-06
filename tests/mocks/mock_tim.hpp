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


#ifndef __MOCK_TIM_HPP__
#define __MOCK_TIM_HPP__

#include <stm32g0xx.h>
#include <timer_manager.hpp>
#include <iostream>
#include <chrono>
#include <future>
#include <thread>
using namespace std::chrono_literals;

namespace stm32::mock
{

/// @brief The type of timer structure to use for the mock timer
enum class TimerType
{
    TIM_TYPEDEF,
    SYSTICK_TYPE
};

class Timer {
public:
    Timer() = default;

    /// @brief Initialise and run the mock counter functionality within an asynchronous thread.
    /// @param future The future result of the async function
    /// @param timer_type stm32::mock::TimerType::TIM_TYPEDEF (periperhal timer) or stm32::mock::TimerType::SYSTICK_TYPE (global system timer)
    /// @return TIM_TypeDef* Instance of the peripheral timer or nullptr if global system timer
    TIM_TypeDef* init_timer(std::future<bool> &future, TimerType timer_type = TimerType::TIM_TYPEDEF);

    /// @brief Simulate the SysTick counter (normally done by ARM HW).
    /// See "Cortex-M0 Technical Ref Man - SysTick Control and Status Register"
    /// @param systick 
    /// @return false if null input, return true when fixture has terminated
    bool static inline run_systick_counter(SysTick_Type *systick);

    /// @brief Simulate the timer counter (normally done by STM32 HW) 
    /// See STM32 Reference Manal Timer control register 1
    /// @param timer The mocked STM32 registers
    /// @return false if timer is null
    bool static inline run_timer_counter(TIM_TypeDef *timer);
};

} // namespace mock_timer

#endif // __MOCK_TIM_HPP__