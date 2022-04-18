#include <catch2/catch_all.hpp>
#include <timer_manager.hpp>
#include <mock.hpp>

TEST_CASE("Timer Manager - microsecond timer: Null Input", "[timer_manager]")
{
    std::cout << "timer_manager - microsecond timer: Null Input" << std::endl;

    SECTION("Null Input")
    {
        TIM_TypeDef *null_timer = nullptr;
        REQUIRE_FALSE(stm32::TimerManager::initialise(null_timer));
        REQUIRE_FALSE(stm32::TimerManager::delay_microsecond(10));
    }
}

TEST_CASE("Timer Manager - Init and Reset", "[timer_manager]")
{
    std::cout << "timer_manager - init and reset" << std::endl;

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
    std::cout << "timer_manager - microsecond timer" << std::endl;

 

    SECTION("Instantiated Input")
    {
        // start the testfixture that simulates HW timer counter
        stm32::mock::Timer mt;
        std::future<bool> tim_res;
        TIM_TypeDef *timer = nullptr;
        timer = mt.mock_init_timer(tim_res);
        
        // run the SUT; loops until 10ms is reached by timer counter
        REQUIRE(stm32::TimerManager::delay_microsecond(10));
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;
        
        // TIM->CNT should match the time elapsed
        REQUIRE(stm32::TimerManager::get_count() == 10);

        // This will cause the testfixture to also return. Make sure it exited as expected.
        REQUIRE(tim_res.get());
    }    
}

TEST_CASE("Timer Manager - Systick Delay", "[timer_manager]")
{
    std::cout << "timer_manager - systick milisecond delay" << std::endl;

    // enable the mocked SysTick counter
    
    stm32::mock::Timer mt;
    std::future<bool> tim_res;
    mt.mock_init_timer(tim_res, true);   
    
    // call the SUT function
    SECTION("Zero delay")
    {
        stm32::delay_millisecond(0);
    }
    
    SECTION("Normal delay")
    {
        stm32::delay_millisecond(10);
    }    
    
    // disable the mocked SysTick counter
    SysTick->CTRL = SysTick->CTRL & ~(1UL << 0UL);
    REQUIRE(tim_res.get());

}