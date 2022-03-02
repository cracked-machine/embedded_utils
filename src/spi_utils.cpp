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


#include <spi_utils.hpp>
#include <timer_manager.hpp>
namespace stm32::spi
{

bool wait_for_txe_flag(SPI_TypeDef *spi_handle, uint32_t delay_us)
{

    if (spi_handle == nullptr)
    {
        return false;
    }
    #if not defined(X86_UNIT_TESTING_ONLY)

        // The TXE flag is set when transmission TXFIFO has enough space to store data to send.
        if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
        {
            // give TX FIFO a chance to clear before checking again
            stm32::TimerManager::delay_microsecond(delay_us);
            if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
            { 
                return false;
            }
            
        }
    #else
        delay_us++;
    #endif
    return true;
}        

bool wait_for_bsy_flag(SPI_TypeDef *spi_handle, uint32_t delay_us)
{
    if (spi_handle == nullptr)
    {
        return false;
    }
    #if not defined(X86_UNIT_TESTING_ONLY)
        // When BSY is set, it indicates that a data transfer is in progress on the SPI
        if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
        {
            // give SPI bus a chance to finish sending data before checking again
            stm32::TimerManager::delay_microsecond(delay_us);
            if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
            { 
                return false;
            }
        }    
    #else   
        delay_us++;
    #endif
    return true; 
}   


} // namespace stm32::spi