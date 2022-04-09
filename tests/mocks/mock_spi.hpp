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


#ifndef __MOCK_SPI_HPP__
#define __MOCK_SPI_HPP__

#include <mock_stm32g0b1xx_defs.hpp>

#include <iostream>
#include <chrono>
#include <future>
#include <thread>
using namespace std::chrono_literals;

/// @brief Mock function to test stm32::spi::wait_for_txe_flag() and stm32::spi::wait_for_bsy_flag()
/// @param i2c_handle The mocked SPI peripheral
/// @return true if unit test disables the peripheral (upon test completion), false if spi_handle is null_ptr

bool inline mock_spi_data_register(SPI_TypeDef *spi_handle)
{

    if (spi_handle == nullptr) { return false; }

    // loop while peripheral is enabled
    while((spi_handle->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
    {
    
        // wait for test bytes to be copied into SPI FIFO
        if (spi_handle->DR != 0)
        {
            
            // exaggerate the delay for MCU to clear the BSY flag so that our SUT has chance to run coverage
            spi_handle->SR = spi_handle->SR | SPI_SR_BSY_Msk;            
            std::this_thread::sleep_for(500ms);
            spi_handle->SR = spi_handle->SR & ~SPI_SR_BSY_Msk;

            // exaggerate the delay for MCU to set the TXE flag so that our SUT has chance to run coverage
            std::this_thread::sleep_for(100ms);
            spi_handle->DR = 0;
            spi_handle->SR = spi_handle->SR | SPI_SR_TXE_Msk;
    
        }
    }

    return true;
}

#endif // __MOCK_SPI_HPP__