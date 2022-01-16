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
#include <functional>

#define ENABLE_EXTI_ISR
#define ENABLE_DMA_ISR
// #define ENABLE_USART_ISR
// #define ENABLE_I2C_ISR
// #define ENABLE_SPI_ISR
// #define ENABLE_TIM1_ISR
// #define ENABLE_TIM2_ISR
// #define ENABLE_TIM3_TIM4_ISR
// #define ENABLE_TIM6_ISR
// #define ENABLE_TIM7_LPTIM2_ISR
// #define ENABLE_TIM14_ISR
// #define ENABLE_TIM15_ISR
// #define ENABLE_TIM16_FDCAN_IT0_ISR
// #define ENABLE_TIM17_FDCAN_IT1_ISR
// #define ENABLE_ADC_COMP_ISR
// #define ENABLE_USB_ISR

namespace isr::stm32g0
{

// @brief common base class for managing a vector table of interrupt service routines (ISR)
// https://www.embedded.com/interrupts-in-c/
// 1. Add valid free "extern C" interupt handler functions declarations/definitions (see MCU ref man)
// 2. Add entry to InterruptType to represent that interrupt
// 3. Add code to the "extern C" interupt handler function to call the ISR() for that InterruptType.
// 4. Create a derived class to implement ISR()
class ISRManagerBaseSTM32G0
{
public:
    // @brief Construct a new Interrupt Manager Base object
    ISRManagerBaseSTM32G0() = default;

    // @brief Specific interrupt that can be selected. This is finer-grained than the NVIC Vector Table.
    enum class InterruptType
    {
        #ifdef ENABLE_EXTI_ISR        
            // @brief EXTI line interrupt definitions
            exti0,
            exti1,
            exti2,
            exti3,
            exti4,
            exti5,
            exti6,
            exti7,
            exti8,
            exti9,
            exti10,
            exti11,
            exti12,
            exti13,
            exti14,
            exti15,
            exti16,
            exti17,
            exti18,
            exti19,
            exti20,
            exti21,
            exti22,
            exti23,
            exti24,
            exti25,
            exti26,
            exti27,
            exti28,
            exti29,
            exti30,
            exti31,
        #endif // ENABLE_EXTI_ISR

        #ifdef ENABLE_DMA_ISR
            dma_ch1,     
            dma_ch2,
            dma_ch3,
            dma_ch4,
            dma_ch5,
            dma_ch6,
            dma_ch7,
        #endif // ENABLE_DMA_ISR

        #ifdef ENABLE_I2C_ISR
            // add I2C interrupt types here
        #endif

        #ifdef ENABLE_SPI_ISR
            // add SPI interrupt types here
        #endif // ENABLE_SPI_ISR

        #ifdef ENABLE_TIM_ISR
            // add Timer interrupt types here
        #endif // ENABLE_TIM_ISR

        #ifdef ENABLE_ADC_COMP_ISR
            // add ADC/COMP interrupt types here
        #endif // ENABLE_ADC_COMP_ISR
        
        #ifdef ENABLE_USB_ISR
            // add USB interrupt types here
        #endif // ENABLE_USB_ISR

        // @brief THIS IS NOT AN INTERRUPT HANDLER. DO NOT SELECT THIS!!! 
        // This element must always be last and must not be assigned a value.
        list_count,      
    };

    // @brief Register the interrupt handler. Transfer ownership to static inline std::array "ISRManagerBaseSTM32G0::m_interrupt_handlers"
    // @param interrupt_type The associated InterruptType that will trigger the callback
    // @param interrupt_manager_instance The instance that will be called when the associated interrupt_type is triggered.
    static void register_handler(InterruptType interrupt_type, std::unique_ptr<ISRManagerBaseSTM32G0> &interrupt_manager_instance);
    
    // @brief 
    virtual void ISR(void) = 0;

    // @brief Array of pointers to registered interrupt handlers. 
    // Use register_handler() to set an ISRManagerBaseSTM32G0 instance with an associated InterruptType.
    // Inline for this reason: https://stackoverflow.com/questions/52135456/why-i-cant-call-a-static-member-variable-in-an-static-member-function-like-this
    static inline std::array<std::unique_ptr<ISRManagerBaseSTM32G0>, static_cast<std::size_t>(InterruptType::list_count) > m_interrupt_handlers;

};

// @brief The STM32G0 interrupt handlers
extern "C" void WWDG_IRQHandler(void);                                  // @brief Window WatchDog         
extern "C" void PVD_VDDIO2_IRQHandler(void);                            // @brief PVD through EXTI Line detect
extern "C" void RTC_TAMP_IRQHandler(void);                              // @brief RTC through the EXTI line   
extern "C" void FLASH_IRQHandler(void);                                 // @brief FLASH                       
extern "C" void RCC_CRS_IRQHandler(void);                               // @brief RCC & CRS     

#ifdef ENABLE_EXTI_ISR             
    extern "C" void EXTI0_1_IRQHandler(void);                               // @brief EXTI Line 0 and 1           
    extern "C" void EXTI2_3_IRQHandler(void);                               // @brief EXTI Line 2 and 3           
    extern "C" void EXTI4_15_IRQHandler(void);                              // @brief EXTI Line 4 to 15           
#endif 

#ifdef ENABLE_USB_ISR
    extern "C" void USB_UCPD1_2_IRQHandler(void);                           // @brief USB, UCPD1, UCPD2  
#endif 

#ifdef ENABLE_DMA_ISR     
    extern "C" void DMA1_Channel1_IRQHandler(void);                         // @brief DMA1 Channel 1              
    extern "C" void DMA1_Channel2_3_IRQHandler(void);                       // @brief DMA1 Channel 2 and Channel 3
    extern "C" void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler(void);     // @brief DMA1 Ch4 to Ch7, DMA2 Ch1 to Ch5, DMAMUX1 overrun
#endif

#ifdef ENABLE_ADC_COMP_ISR
    extern "C" void ADC1_COMP_IRQHandler(void);                             // @brief ADC1, COMP1 and COMP2   
#endif 

#ifdef ENABLE_TIM1_ISR     
    extern "C" void TIM1_BRK_UP_TRG_COM_IRQHandler(void);                   // @brief TIM1 Break, Update, Trigger and Commutation
    extern "C" void TIM1_CC_IRQHandler(void);                               // @brief TIM1 Capture Compare     
#endif 
#ifdef ENABLE_TIM2_ISR
    extern "C" void TIM2_IRQHandler(void);                                  // @brief TIM2    
#endif 
#ifdef ENABLE_TIM3_TIM4_ISR
    extern "C" void TIM3_TIM4_IRQHandler(void);                             // @brief TIM3, TIM4                        
#endif 
#ifdef ENABLE_TIM6_ISR
    extern "C" void TIM6_DAC_LPTIM1_IRQHandler(void);                       // @brief TIM6, DAC and LPTIM1                  
#endif 
#ifdef ENABLE_TIM7_LPTIM2_ISR       
    extern "C" void TIM7_LPTIM2_IRQHandler(void);                           // @brief TIM7 and LPTIM2             
#endif 
#ifdef ENABLE_TIM14_ISR
    extern "C" void TIM14_IRQHandler(void);                                 // @brief TIM14                       
#endif 
#ifdef ENABLE_TIM15_ISR
    extern "C" void TIM15_IRQHandler(void);                                 // @brief TIM15                       
#endif 
#ifdef ENABLE_TIM16_FDCAN_IT0_ISR
    extern "C" void TIM16_FDCAN_IT0_IRQHandler(void);                       // @brief TIM16 & FDCAN1_IT0 & FDCAN2_IT0
#endif 
#ifdef ENABLE_TIM17_FDCAN_IT1_ISR
    extern "C" void TIM17_FDCAN_IT1_IRQHandler(void);                       // @brief TIM17 & FDCAN1_IT1 & FDCAN2_IT1
#endif 


#ifdef ENABLE_I2C_ISR
    extern "C" void I2C1_IRQHandler(void);                                  // @brief I2C1                        
    extern "C" void I2C2_3_IRQHandler(void);                                // @brief I2C2, I2C3                  
#endif 
#ifdef ENABLE_SPI_ISR
    extern "C" void SPI1_IRQHandler(void);                                  // @brief SPI1                        
    extern "C" void SPI2_3_IRQHandler(void);                                // @brief SPI2, SPI3                  
#endif 

#ifdef ENABLE_USART_ISR
    extern "C" void USART1_IRQHandler(void);                                // @brief USART1                      
    extern "C" void USART2_LPUART2_IRQHandler(void);                        // @brief USART2 & LPUART2            
    extern "C" void USART3_4_5_6_LPUART1_IRQHandler(void);                  // @brief USART3, USART4, USART5, USART6, LPUART1  
#endif

extern "C" void CEC_IRQHandler(void);                                   // @brief CEC                         

} // namespace isr::stm32g0

#endif // __INTERRUPT_MANAGER_BASE_HPP__