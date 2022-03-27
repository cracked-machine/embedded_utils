
#include <catch_common.hpp>

/// @brief Simulate the SysTick counter (normally done by ARM HW).
/// See "Cortex-M0 Technical Ref Man - SysTick Control and Status Register"
/// @param systick 
/// @return false if null input, return true when fixture has terminated
bool testfixture_systick_sim(SysTick_Type *systick)
{
    using namespace std::chrono_literals;

    if (systick == nullptr)
    {
        return false;
    }
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

/// @brief Simulate the timer counter (normally done by STM32 HW) 
/// See STM32 Reference Manal Timer control register 1
/// @param timer The mocked STM32 registers
/// @return false if timer is null
bool testfixture_timer_sim(TIM_TypeDef *timer)
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