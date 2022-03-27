#include <catch2/catch_all.hpp>
#include <timer_manager.hpp>
#include <mock_cmsis.hpp>
#include <thread>
#include <iostream>
#include <chrono>
#include <future>

/// @brief Intialise the struct (normally done by the STM32 HW)
/// @param timer The mocked STM32 registers
void testfixture_init_timer(TIM_TypeDef *timer)
{
    timer->CR1 = 0;         // TIM control register 1,                   
    timer->CR2 = 0;         // TIM control register 2,                   
    timer->SMCR = 0;        // TIM slave mode control register,          
    timer->DIER = 0;        // TIM DMA/interrupt enable register,        
    timer->SR = 0;          // TIM status register,                      
    timer->EGR = 0;         // TIM event generation register,            
    timer->CCMR1 = 0;       // TIM capture/compare mode register 1,      
    timer->CCMR2 = 0;       // TIM capture/compare mode register 2,      
    timer->CCER = 0;        // TIM capture/compare enable register,      
    timer->CNT = 0;         // TIM counter register,                     
    timer->PSC = 0;         // TIM prescaler register,                   
    timer->ARR = 0;         // TIM auto-reload register,                 
    timer->RCR = 0;         // TIM repetition counter register,          
    timer->CCR1 = 0;        // TIM capture/compare register 1,           
    timer->CCR2 = 0;        // TIM capture/compare register 2,           
    timer->CCR3 = 0;        // TIM capture/compare register 3,           
    timer->CCR4 = 0;        // TIM capture/compare register 4,           
    timer->BDTR = 0;        // TIM break and dead-time register,         
    timer->DCR = 0;         // TIM DMA control register,                 
    timer->DMAR = 0;        // TIM DMA address for full transfer,        
    timer->OR1 = 0;         // TIM option register,                      
    timer->CCMR3 = 0;       // TIM capture/compare mode register 3,      
    timer->CCR5 = 0;        // TIM capture/compare register5,            
    timer->CCR6 = 0;        // TIM capture/compare register6,            
    timer->AF1 = 0;         // TIM alternate function register 1,        
    timer->AF2 = 0;         // TIM alternate function register 2,        
    timer->TISEL = 0;       // TIM Input Selection register,     

}

/// @brief Update the timer counter (normally done by STM32 HW) when CR1 register is enabled 
/// @param timer The mocked STM32 registers
/// @return false if registers are null
bool testfixture_timer_counter(TIM_TypeDef *timer)
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
    testfixture_init_timer(timer1);
    REQUIRE(stm32::TimerManager::initialise(timer1));

    // 3. Re-initialise
    TIM_TypeDef *timer2 = new TIM_TypeDef;
    testfixture_init_timer(timer2);
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
    testfixture_init_timer(timer);
    REQUIRE(stm32::TimerManager::initialise(timer));

    // start the testfixture that simulates HW timer counter
    std::future<bool> res = std::async(std::launch::async, testfixture_timer_counter, timer);
    
    // run the SUT; loops until 10ms is reached by timer counter
    REQUIRE(stm32::TimerManager::delay_microsecond(10));
    
    // SUT has returned so simulate disabling of the HW Timer
    timer->CR1 = 0;
    
    // TIM->CNT should match the time elapsed
    REQUIRE(stm32::TimerManager::get_count() == 10);

    // This will cause the testfixture to also return. Make sure it exited as expected.
    REQUIRE(res.get());
    
}