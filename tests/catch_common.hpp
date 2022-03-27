#ifndef __CATCH_COMMON_HPP__
#define __CATCH_COMMON_HPP__

#include <mock_cmsis.hpp>
#include <thread>
#include <iostream>
#include <chrono>
#include <future>

/// @brief Simulate the SysTick counter (normally done by ARM HW).
/// See "Cortex-M0 Technical Ref Man - SysTick Control and Status Register"
/// @param systick 
/// @return false if null input, return true when fixture has terminated
bool testfixture_systick_sim(SysTick_Type *systick);

/// @brief Simulate the timer counter (normally done by STM32 HW) 
/// See STM32 Reference Manal Timer control register 1
/// @param timer The mocked STM32 registers
/// @return false if timer is null
bool testfixture_timer_sim(TIM_TypeDef *timer);


#endif // __CATCH_COMMON_HPP__