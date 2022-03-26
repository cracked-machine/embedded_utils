#include <catch2/catch_all.hpp>
#include <timer_manager.hpp>
#include <mock_cmsis.hpp>

TEST_CASE("Timer Manager", "[timer_manager]")
{
    TIM_TypeDef *null_timer = nullptr;
    REQUIRE_FALSE(stm32::TimerManager::initialise(null_timer));

    TIM_TypeDef *real_timer = reinterpret_cast<TIM_TypeDef *>(1);
    REQUIRE(stm32::TimerManager::initialise(real_timer));    
}