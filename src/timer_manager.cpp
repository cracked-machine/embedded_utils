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

#include <timer_manager.hpp>

namespace stm32
{

void TimerManager::initialise(TIM_TypeDef *timer)
{
    if (m_timer == nullptr) { m_timer = std::unique_ptr<TIM_TypeDef>(timer); }
    else { error_handler(); }
    reset();
}

void TimerManager::reset()
{
    // wait in limbo if not initialised
    if (m_timer == nullptr) { error_handler(); }
#if not defined(X86_UNIT_TESTING_ONLY)
    // ensure the timer is disabled before setup
    if (LL_TIM_IsEnabledCounter(m_timer.get())) { LL_TIM_DisableCounter(m_timer.get()); }
    // setup the timer to 1 us resolution (depending on the system clock frequency)
    LL_TIM_SetPrescaler(m_timer.get(), SystemCoreClock / 1000000UL);
    // allow largest possible timeout
    LL_TIM_SetAutoReload(m_timer.get(), 0xFFFF-1);
    // reset CNT
    LL_TIM_SetCounter(m_timer.get(), 0);
    // start the timer and wait for the timeout
    LL_TIM_EnableCounter(m_timer.get());    
#endif
}

void TimerManager::delay_microsecond(uint32_t delay_us)
{
    // wait in limbo if not initialised
    if (m_timer == nullptr) { error_handler(); }

    // @TODO change the prescaler to allow longer delays, clamp for now
    if (delay_us > 0xFFFE) { delay_us = 0xFFFE; }
    
    // setup the timer for timeout function
    reset();
#if not defined(X86_UNIT_TESTING_ONLY)
    while (LL_TIM_GetCounter(m_timer.get()) < delay_us);
#endif
}

uint32_t TimerManager::get_count()
{
#if not defined(X86_UNIT_TESTING_ONLY)
    if (!LL_TIM_IsEnabledCounter(m_timer.get())) { LL_TIM_EnableCounter(m_timer.get()); }
    return LL_TIM_GetCounter(m_timer.get());
#else
    return 1;
#endif
}

void TimerManager::error_handler()
{
    while(1)
    {
        // stay here to allow stack trace to be shown in debugger...
    }
}

} // namespace stm32:
