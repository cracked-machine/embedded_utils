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

#ifndef __SPI_UTILS_HPP__
#define __SPI_UTILS_HPP__

#if defined(X86_UNIT_TESTING_ONLY)
	// this file should contain SPI bit definitions
	#include <mock_cmsis.hpp>
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvolatile"
	// add groups for device family
	#if defined (STM32G071xx) || defined (STM32G081xx) || defined (STM32G070xx) \
	|| defined (STM32G030xx) || defined (STM32G031xx) || defined (STM32G041xx) \
	|| defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx) \
	|| defined (STM32G050xx) || defined (STM32G051xx) || defined (STM32G061xx)		
		#include <stm32g0xx.h>
	#endif
	#pragma GCC diagnostic pop

#endif

namespace stm32::spi
{

void enable_spi(SPI_TypeDef *spi_handle, bool enable = true);

bool send_byte(SPI_TypeDef *spi_handle, uint8_t byte);

// @brief Check and retry (with timeout) the SPIx_SR TXE register.
// @param spi_handle Pointer to the CMSIS mem-mapped SPI device
// @param delay_us The timeout
// @return true if TX FIFO is empty, false if TX FIFO is full
bool wait_for_txe_flag(SPI_TypeDef *spi_handle, uint32_t delay_us = 100);

// @brief Check and retry (with timeout) the SPIx_SR BSY register.
// @param spi_handle Pointer to the CMSIS mem-mapped SPI device
// @param delay_us The timeout
// @return true if SPI bus is busy, false if SPI bus is not busy.
bool wait_for_bsy_flag(SPI_TypeDef *spi_handle, uint32_t delay_us = 100);


void set_prescaler(SPI_TypeDef *spi_handle, uint32_t new_value);

} // namespace stm32::spi

#endif // __SPI_UTILS_HPP__