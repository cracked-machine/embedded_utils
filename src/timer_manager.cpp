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

#include <timer_manager.hpp>

namespace stm32
{


void delay_millisecond(uint32_t delay_ms)
{
    // Reading SysTick Control and Status Register clears the COUNTFLAG bit to 0
    [[maybe_unused]] __IOM uint32_t  tmp = SysTick->CTRL;  
    
    // make sure we have a delay of at least 1
    if (delay_ms == 0) {  delay_ms++; }

    while (delay_ms  != 0U)
    {
        if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
        {
            delay_ms --;
        }
        // simulate the "Clear on read by application or debugger."
        #ifdef X86_UNIT_TESTING_ONLY
            SysTick->CTRL = SysTick->CTRL & ~SysTick_CTRL_COUNTFLAG_Msk;
        #endif
    }
}


bool TimerManager::initialise(TIM_TypeDef *timer)
{

    if (timer == nullptr) { return false; }

    // stop the timer before re-assigning the pointer
    if (m_timer != nullptr)
    {
        m_timer->CR1 = m_timer->CR1 & ~(TIM_CR1_CEN); 
    }

    m_timer = timer; 

    reset();
    return true;
}

void TimerManager::reset()
{
    
    // ensure the timer is disabled before setup
    if ( (m_timer->CR1 & TIM_CR1_CEN) == TIM_CR1_CEN )
    { 
        m_timer->CR1 = m_timer->CR1 & ~(TIM_CR1_CEN); 
    }
    // setup the timer to 1 us resolution (depending on the system clock frequency)
    m_timer->PSC = SystemCoreClock / 1000000UL;

    // allow largest possible timeout
    m_timer->ARR = 0xFFFF-1;
    
    // reset CNT
    m_timer->CNT = 0;
    
    // start the timer and wait for the timeout
    m_timer->CR1 = m_timer->CR1 | (TIM_CR1_CEN); 
}

bool TimerManager::delay_microsecond(uint32_t delay_us)
{
    // wait in limbo if not initialised
    if (m_timer == nullptr) { return false; }

    // @TODO change the prescaler to allow longer delays, clamp for now
    if (delay_us > 0xFFFE) { delay_us = 0xFFFE; }

    // setup the timer for timeout function
    reset();
    while (m_timer->CNT < delay_us);
    return true;
}

uint32_t TimerManager::get_count()
{
    return m_timer->CNT;
}

// bool TimerManager::error_handler()
// {
//     #ifdef X86_UNIT_TESTING_ONLY
//         return false;
//     #else
//         while(1)
//         {
//             // stay here to allow stack trace to be shown in debugger...
//         }
//     #endif
// }

} // namespace stm32:
