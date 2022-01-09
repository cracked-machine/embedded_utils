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

#include <interrupt_manager_base.hpp>

#include <cassert>

namespace isr::stm32g0
{

void InterruptManagerBase::register_handler(ISRVectorTableEnums interrupt_number, std::unique_ptr<InterruptManagerBase> &interrupt_manager_instance)
{
    assert(static_cast<size_t>(interrupt_number) < InterruptManagerBase::ISRVectorTable.size());

    // add the interrupt manager instance to the array for the specified interrupt number
    ISRVectorTable[ static_cast<int>(interrupt_number) ] = std::move(interrupt_manager_instance);
}

extern "C" void EXTI4_15_IRQHandler(void)
{
    // Do not assign a value to InterruptManagerBase::ISRVectorTableEnums::isr_count
    static_assert( static_cast<std::size_t>(InterruptManagerBase::ISRVectorTableEnums::exti4_15_irqhandler) < InterruptManagerBase::ISRVectorTable.size());

    // call the ISR() of the registered interrupt class
    InterruptManagerBase::ISRVectorTable[ static_cast<int>(InterruptManagerBase::ISRVectorTableEnums::exti4_15_irqhandler) ]->ISR();
}

extern "C" void DMA1_Channel1_IRQHandler(void)
{
    // Do not assign a value to InterruptManagerBase::ISRVectorTableEnums::isr_count
    static_assert( static_cast<std::size_t>(InterruptManagerBase::ISRVectorTableEnums::dma_ch1_irqhandler) < InterruptManagerBase::ISRVectorTable.size());

    InterruptManagerBase::ISRVectorTable[ static_cast<int>(InterruptManagerBase::ISRVectorTableEnums::dma_ch1_irqhandler) ]->ISR();
}



} // namespace isr::stm32g0