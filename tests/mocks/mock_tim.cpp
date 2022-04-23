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


#include <mock_tim.hpp>

namespace stm32::mock
{

TIM_TypeDef* Timer::init_timer(std::future<bool> &future, TimerType timer_type)
{
    TIM_TypeDef* timer = nullptr;
    switch(timer_type)
    {   
        case TimerType::SYSTICK_TYPE:
            SysTick = new SysTick_Type; 
            SysTick->CTRL = SysTick->CTRL | 1UL << 0UL;
            future = std::async(std::launch::async, run_systick_counter, SysTick); 
            break;
        case TimerType::TIM_TYPEDEF:    
            timer = new TIM_TypeDef;
            stm32::TimerManager::initialise(timer);
            future = std::async(std::launch::async, run_timer_counter, timer); 
            break;
    }
    return timer;
}

bool inline Timer::run_systick_counter(SysTick_Type *systick)
{
    using namespace std::chrono_literals;

    if (systick == nullptr) { return false; }

    auto systick_enabled = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk);
    while ((systick->CTRL & systick_enabled) == systick_enabled)
    {
        std::this_thread::sleep_for(1ms);
        std::cout << std::flush << ".";
        // simluate the counter by setting the COUNTFLAG
        systick->CTRL = systick->CTRL | SysTick_CTRL_COUNTFLAG_Msk;


    }
    std::cout << std::endl;
    return true;
}

bool inline Timer::run_timer_counter(TIM_TypeDef *timer)
{
    using namespace std::chrono_literals;
    if (timer == nullptr)
    {
        return false;
    }
    // timer enabled
    while (timer->CR1 == 1) 
    { 
        // simulate the 1ms timing
        std::this_thread::sleep_for(1ms);
        std::cout << std::flush << ".";
        timer->CNT = timer->CNT + 1; 
    }
    std::cout << std::endl;
    return true;
}

} // namespace stm32::mock
