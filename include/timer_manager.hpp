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

#ifndef __TIMER_MANAGER_HPP__
#define __TIMER_MANAGER_HPP__

#if defined(X86_UNIT_TESTING_ONLY)
    // This file should contain TIM bit definitions
    #include <mock_cmsis.hpp>
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvolatile"
	// add groups for device family    
	#if defined (STM32G071xx) || defined (STM32G081xx) || defined (STM32G070xx) \
	|| defined (STM32G030xx) || defined (STM32G031xx) || defined (STM32G041xx) \
	|| defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx) \
	|| defined (STM32G050xx) || defined (STM32G051xx) || defined (STM32G061xx)	    
        #include <stm32g0xx.h>
    #endif
	#pragma GCC diagnostic pop
#endif

// #include <memory>
#include <restricted_base.hpp>



namespace stm32
{

#define LL_MAX_DELAY                  0xFFFFFFFFU
void        delay_millisecond(uint32_t Delay);

// @brief Object to manage timer instance used for microsecond timeouts and debouncing
class TimerManager : public RestrictedBase
{

public:
    // @brief Set up the timer instance once and only once. Call this in main() setup.
    // @param timer The pointer to TIM_TypeDef
    static bool initialise(TIM_TypeDef *timer);

    // @brief wait for a microsecond delay
    // @param delay_us the delay to wait in microseconds
    static bool delay_microsecond(uint32_t delay_us);

    // @brief Get the current count of the timer
    // @param value_usecs The count value returned
    static uint32_t get_count();
    
private:
    // @brief Setup the timer
    static void reset();
    // @brief Loop here if something is wrong. Return false during x86 tests.
    static bool error_handler();
    // @brief The timer instance
    // static inline std::unique_ptr<TIM_TypeDef> m_timer;
    static inline TIM_TypeDef* m_timer;
};

} // namespace stm32

#endif // __TIMER_MANAGER_HPP__