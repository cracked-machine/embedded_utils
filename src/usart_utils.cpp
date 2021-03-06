// MIT License

// Copyright (c) 2022 Chris Sutton

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


#include <usart_utils.hpp>
#include <timer_manager.hpp>
namespace stm32::usart
{

bool enable_usart(USART_TypeDef *usart_handle)
{
    if (usart_handle == nullptr) { return false; }
    
    usart_handle->CR1 = usart_handle->CR1 | USART_CR1_UE;
    return true;
}

bool transmit_byte(USART_TypeDef *usart_handle, uint8_t byte)
{
    if (usart_handle == nullptr) { return false; }

    while(!wait_for_bsy_flag(usart_handle));
    while(!wait_for_tc_flag(usart_handle));
    
    usart_handle->TDR = byte;
    return true;
}

bool wait_for_tc_flag(USART_TypeDef *usart_handle, uint32_t delay_us)
{

    if (usart_handle == nullptr) { return false; }
    // Check the previous tranmission has completed
    if ((usart_handle->ISR & USART_ISR_TC) != (USART_ISR_TC))
    {
        // if not then wait before checking again
        stm32::TimerManager::delay_microsecond(delay_us);
        return false;
        
    }

    return true;
}        

bool wait_for_bsy_flag(USART_TypeDef *usart_handle, uint32_t delay_us)
{
    if (usart_handle == nullptr)
    {
        return false;
    }
    // When BSY is set, it indicates that a data transfer is in progress on the USART
    if ((usart_handle->ISR & USART_ISR_BUSY) == (USART_ISR_BUSY))
    {
        // give USART bus a chance to finish sending data before checking again
        stm32::TimerManager::delay_microsecond(delay_us);
        return false;
    }    
    return true; 
}   

} // namespace stm32::spi