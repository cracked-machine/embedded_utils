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

#if defined(USE_SSD1306_HAL_DRIVER) || defined(USE_SSD1306_LL_DRIVER)

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvolatile"
		#include "main.h"
		#include "i2c.h"	
	#pragma GCC diagnostic pop

#endif

namespace isr::stm32g0
{

void InterruptManagerBase::Register(ISRVectorTableEnums interrupt_number, std::unique_ptr<InterruptManagerBase> &interrupt_manager_instance)
{
    // add the interrupt manager instance to the array for the specified interrupt number
    ISRVectorTable[ static_cast<int>(interrupt_number) ] = std::move(interrupt_manager_instance);
}

extern "C" void EXTI4_15_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_5) != RESET)
    {
        LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_5);
    }
    InterruptManagerBase::ISRVectorTable[ static_cast<int>(InterruptManagerBase::ISRVectorTableEnums::exti4_15_irqhandler) ]->ISR();
}

} // namespace isr::stm32g0