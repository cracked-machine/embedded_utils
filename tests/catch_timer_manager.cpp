#include <catch2/catch_all.hpp>
#include <timer_manager.hpp>
#include <mock_cmsis.hpp>
#include <thread>
#include <iostream>
#include <chrono>
#include <future>



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

/// @brief Update the timer counter (normally done by STM32 HW) when CR1 register is enabled 
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

TEST_CASE("Timer Manager - Init and Reset", "[timer_manager]")
{
    // 1. Invalid input check
    TIM_TypeDef *null_ptr = nullptr;
    REQUIRE_FALSE(stm32::TimerManager::initialise(null_ptr));

    // 2. Initialise once
    TIM_TypeDef *timer1 = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer1));

    // 3. Re-initialise
    TIM_TypeDef *timer2 = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer2));

    // 4. Register value check
    // Timer should be set for 1 microsecond resolution and reset
    REQUIRE(timer2->PSC == 64);  
    REQUIRE(timer2->ARR == 65534);
    REQUIRE(timer2->CNT == 0);
    // Timer was enabled
    REQUIRE(timer2->CR1 == 1);
}

TEST_CASE("Timer Manager - microsecond timer", "[timer_manager]")
{
    TIM_TypeDef *timer = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer));

    // start the testfixture that simulates HW timer counter
    std::future<bool> res = std::async(std::launch::async, testfixture_timer_sim, timer);
    
    // run the SUT; loops until 10ms is reached by timer counter
    REQUIRE(stm32::TimerManager::delay_microsecond(10));
    
    // SUT has returned so simulate disabling of the HW Timer
    timer->CR1 = 0;
    
    // TIM->CNT should match the time elapsed
    REQUIRE(stm32::TimerManager::get_count() == 10);

    // This will cause the testfixture to also return. Make sure it exited as expected.
    REQUIRE(res.get());
    
}

TEST_CASE("Timer Manager - Systick Delay", "[timer_manager]")
{
    // instantiate the global instance
    SysTick = new SysTick_Type;
    // enable the mocked SysTick counter
    SysTick->CTRL = SysTick->CTRL | 1UL << 0UL;
    // start the testfixture that simulates SysTick timer counter
    std::future<bool> res = std::async(std::launch::async, testfixture_systick_sim, SysTick);    
    
    stm32::delay_millisecond(10);
    
    // disable the mocked SysTick counter
    SysTick->CTRL = SysTick->CTRL & ~(1UL << 0UL);
    REQUIRE(res.get());

}