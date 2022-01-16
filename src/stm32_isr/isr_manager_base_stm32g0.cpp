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

void ISRManagerBaseSTM32G0::register_handler(InterruptType interrupt_type, std::unique_ptr<ISRManagerBaseSTM32G0> &interrupt_manager_instance)
{
    assert(static_cast<size_t>(interrupt_type) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());

    // add the interrupt manager instance to the array for the specified interrupt number
    m_interrupt_handlers[ static_cast<int>(interrupt_type) ] = std::move(interrupt_manager_instance);
}

////
//// Interrupt function definitions 
////

#ifdef ENABLE_EXTI_ISR
    extern "C" void EXTI4_15_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::exti5) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());

        // call the ISR() of the registered interrupt class
        if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_5))
        {
            ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::exti5) ]->ISR();
        }
    }
#endif // ENABLE_EXTI_ISR

#ifdef ENABLE_DMA_ISR
    extern "C" void DMA1_Channel1_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::dma_ch1) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::dma_ch1) ]->ISR();
    }
#endif // ENABLE_DMA_ISR

#ifdef ENABLE_I2C_ISR
    extern "C" void I2C1_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();                
    }                                          
    extern "C" void I2C2_3_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();                
    }                             
#endif

#ifdef ENABLE_SPI_ISR
    // add SPI interrupt function definitions here
#endif // ENABLE_SPI_ISR

#ifdef ENABLE_TIM1_ISR
    extern "C" void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
    extern "C" void TIM1_CC_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM1_ISR
#ifdef ENABLE_TIM2_ISR
    extern "C" void TIM2_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM2_ISR
#ifdef ENABLE_TIM3_TIM4_ISR
    extern "C" void TIM3_TIM4_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM3_TIM4_ISR
#ifdef ENABLE_TIM6_ISR
    extern "C" void TIM6_DAC_LPTIM1_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM6_ISR
#ifdef ENABLE_TIM7_LPTIM2_ISR
    extern "C" void TIM7_LPTIM2_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM7_LPTIM2_ISR
#ifdef ENABLE_TIM14_ISR
    extern "C" void TIM14_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM14_ISR
#ifdef ENABLE_TIM15_ISR
    extern "C" void TIM15_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM15_ISR    
#ifdef ENABLE_TIM16_FDCAN_IT0_ISR
    extern "C" void TIM16_FDCAN_IT0_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM16_ISR
#ifdef ENABLE_TIM17_FDCAN_IT1_ISR
    extern "C" void TIM17_FDCAN_IT1_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();     
    }
#endif // ENABLE_TIM17_FDCAN_IT1_ISR

#ifdef ENABLE_ADC_COMP_ISR
    extern "C" void ADC1_COMP_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();                
    }
#endif // ENABLE_ADC_COMP_ISR

#ifdef ENABLE_USB_ISR
    extern "C" void USB_UCPD1_2_IRQHandler(void)
    {
        // Do not assign a value to ISRManagerBaseSTM32G0::InterruptType::list_count
        static_assert( static_cast<std::size_t>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) < ISRManagerBaseSTM32G0::m_interrupt_handlers.size());
        // call the ISR function of the associated instance
        ISRManagerBaseSTM32G0::m_interrupt_handlers[ static_cast<int>(ISRManagerBaseSTM32G0::InterruptType::ADD_INTERRUPT_TYPE_HERE) ]->ISR();        
    }
#endif // ENABLE_USB_ISR

} // namespace isr::stm32g0