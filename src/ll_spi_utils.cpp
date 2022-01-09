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

#if defined(USE_SSD1306_HAL_DRIVER) || defined(USE_SSD1306_LL_DRIVER)

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvolatile"
		#include "main.h"
		#include "spi.h"	
	#pragma GCC diagnostic pop

#endif

#include <ll_spi_utils.hpp>

namespace stm32::spi
{

bool ll_wait_for_txe_flag(std::unique_ptr<SPI_TypeDef> &spi_handle, uint32_t delay_us)
{

    if (spi_handle == nullptr)
    {
        return false;
    }

    // The TXE flag is set when transmission TXFIFO has enough space to store data to send.
    if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
    {
        // give TX FIFO a chance to clear before checking again
        tim::ll_delay_microsecond(TIM14, delay_us);
        if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
        { 
            return false;
        }
        
    }
    return true;
}        

bool ll_wait_for_bsy_flag(std::unique_ptr<SPI_TypeDef> &spi_handle, uint32_t delay_us)
{
    if (spi_handle == nullptr)
    {
        return false;
    }
    // When BSY is set, it indicates that a data transfer is in progress on the SPI
    if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
    {
        // give SPI bus a chance to finish sending data before checking again
        tim::ll_delay_microsecond(TIM14, delay_us);
        if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
        { 
            return false;
        }
    }    
    return true; 
}   


} // namespace stm32::spi