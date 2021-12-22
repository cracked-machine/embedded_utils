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

#ifndef __LL_TIM_UTILS_HPP__
#define __LL_TIM_UTILS_HPP__

namespace embedded_utils
{

// @brief common functions for using with STM32 LL TIM
class ll_uDelay
{
public:
    ll_uDelay() = default;

    // @brief microsecond timeout using periperhal timer
    // @param tim_handle Make sure the timer is initialised
    // @param delay_us 
    template<typename TIM_HANDLE>
    static void timeout(TIM_HANDLE *tim_handle, uint32_t delay_us)
    {
        // ensure the timer is disabled before setup
        if (LL_TIM_IsEnabledCounter(tim_handle)) { LL_TIM_DisableCounter(tim_handle); }
        // setup the timer to 1 us resolution (depending on the system clock frequency)
        LL_TIM_SetPrescaler(tim_handle, SystemCoreClock / 1000000UL);
        // allow largest possible timeout
        LL_TIM_SetAutoReload(tim_handle, 0xFFFF-1);
        // reset CNT
        LL_TIM_SetCounter(tim_handle, 0);
        // start the timer and wait for the timeout
        LL_TIM_EnableCounter(tim_handle);
        while (LL_TIM_GetCounter(tim_handle) < delay_us);
    }
};

} // namespace embedded_utils

#endif // __LL_TIM_UTILS_HPP__