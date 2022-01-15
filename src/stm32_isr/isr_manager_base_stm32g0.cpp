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

#include <isr_manager_base_stm32g0.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvolatile"
    #include "main.h"
#pragma GCC diagnostic pop

#include <cassert>

namespace isr::stm32g0
{

void ISRManagerBaseSTM32G0::register_handler(ISRVectorTableEnums interrupt_number, std::unique_ptr<ISRManagerBaseSTM32G0> &interrupt_manager_instance)
{
    assert(static_cast<size_t>(interrupt_number) < ISRManagerBaseSTM32G0::ISRVectorTable.size());

    // add the interrupt manager instance to the array for the specified interrupt number
    ISRVectorTable[ static_cast<int>(interrupt_number) ] = std::move(interrupt_manager_instance);
}

extern "C" void EXTI4_15_IRQHandler(void)
{
    // Do not assign a value to ISRManagerBaseSTM32G0::ISRVectorTableEnums::isr_count
    static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::ISRVectorTableEnums::exti5_irqhandler) < ISRManagerBaseSTM32G0::ISRVectorTable.size());

    // call the ISR() of the registered interrupt class
    if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_5))
    {
        ISRManagerBaseSTM32G0::ISRVectorTable[ static_cast<int>(ISRManagerBaseSTM32G0::ISRVectorTableEnums::exti5_irqhandler) ]->ISR();
    }
}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
    // Do not assign a value to ISRManagerBaseSTM32G0::ISRVectorTableEnums::isr_count
    static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::ISRVectorTableEnums::dma_ch1_irqhandler) < ISRManagerBaseSTM32G0::ISRVectorTable.size());
    
    ISRManagerBaseSTM32G0::ISRVectorTable[ static_cast<int>(ISRManagerBaseSTM32G0::ISRVectorTableEnums::dma_ch1_irqhandler) ]->ISR();
}



} // namespace isr::stm32g0