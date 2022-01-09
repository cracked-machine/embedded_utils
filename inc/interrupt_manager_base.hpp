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

#ifndef __INTERRUPT_MANAGER_BASE_HPP__
#define __INTERRUPT_MANAGER_BASE_HPP__

#include <array>
#include <memory>
namespace isr::stm32g0
{

// @brief common base class for managing a vector table of interrupt service routines (ISR)
// https://www.embedded.com/interrupts-in-c/
// 1. Add valid free "extern C" interupt handler functions declarations/definitions (see MCU ref man)
// 2. Add entry to ISRVectorTableEnums to represent that interrupt
// 3. Add code to the "extern C" interupt handler function to call the ISR() for that ISRVectorTableEnums.
// 4. Create a derived class to implement ISR()
class InterruptManagerBase
{
public:
    // @brief Construct a new Interrupt Manager Base object
    InterruptManagerBase() = default;

    // @brief ISR that can be selected by derived classes implementing a specific type of ISR manager.
    enum class ISRVectorTableEnums
    {
        // @brief EXTI line 4 to 15 interrupt Handler 
        exti4_15_irqhandler = 0,
        // @brief THIS IS NOT AN INTERRUPT HANDLER. DO NOT SELECT THIS!!! 
        isr_count = 1,
    };

    // @brief Register the derived interrupt manager to specific interrupt handler function
    // @param interrupt_number 
    // @param intThisPtr 
    static void Register(ISRVectorTableEnums interrupt_number, std::unique_ptr<InterruptManagerBase> &interrupt_manager_instance);
    
    // @brief 
    virtual void ISR(void) = 0;

    // https://stackoverflow.com/questions/52135456/why-i-cant-call-a-static-member-variable-in-an-static-member-function-like-this
    static inline std::array<std::unique_ptr<InterruptManagerBase>, static_cast<std::size_t>(ISRVectorTableEnums::isr_count) > ISRVectorTable;
};

// @brief The STM32G0 interrupt handler for EXTI4_15
extern "C" void EXTI4_15_IRQHandler(void);


} // namespace isr::stm32g0

#endif // __INTERRUPT_MANAGER_BASE_HPP__