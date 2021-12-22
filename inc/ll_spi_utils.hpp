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

#ifndef __LL_SPI_UTILS_HPP__
#define __LL_SPI_UTILS_HPP__

#include "ll_tim_utils.hpp"

namespace embedded_utils
{

// @brief common functions for using with STM32 LL SPI

class LowLevelSPIUtils
{

public: 

    LowLevelSPIUtils() = default;

    // @brief Check and retry (with timeout) the SPIx_SR TXE register.
    // static resolved performance drop after moving this code to this external class
    // @param spi_handle Pointer to the STM32 LL SPI_HANDLE object
    // @param delay_us The timeout
    // @return true if TX FIFO is empty, false if TX FIFO is full
    template<typename SPI_HANDLE>
    static bool check_txe_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_us = 100);

    // @brief Check and retry (with timeout) the SPIx_SR BSY register.
    // static resolved performance drop after moving this code to this external class
    // @param spi_handle Pointer to the STM32 LL SPI_HANDLE object
    // @param delay_us The timeout
    // @return true if SPI bus is busy, false if SPI bus is not busy.
    template<typename SPI_HANDLE>
    static bool check_bsy_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_us = 100);

};

template<typename SPI_HANDLE>
bool LowLevelSPIUtils::check_txe_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_us)
{

    if (spi_handle == nullptr)
    {
        return false;
    }

    // The TXE flag is set when transmission TXFIFO has enough space to store data to send.
    if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
    {
        // give TX FIFO a chance to clear before checking again
        ll_uDelay::timeout(TIM14, delay_us);
        if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
        { 
            return false;
        }
        
    }
    return true;
}        

template<typename SPI_HANDLE>
bool LowLevelSPIUtils::check_bsy_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_us)
{
    if (spi_handle == nullptr)
    {
        return false;
    }
    // When BSY is set, it indicates that a data transfer is in progress on the SPI
    if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
    {
        // give SPI bus a chance to finish sending data before checking again
        ll_uDelay::timeout(TIM14, delay_us);
        if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
        { 
            return false;
        }
    }    
    return true; 
}   

} // namespace embedded_utils


#endif // __LL_SPI_UTILS_HPP__