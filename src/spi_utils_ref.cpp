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

#include <spi_utils_ref.hpp>
#include <timer_manager.hpp>
namespace stm32::spi_ref
{

bool enable_spi(SPI_TypeDef &spi_handle, bool enable)
{

  if (enable)
  {
    spi_handle.CR1 = spi_handle.CR1 | SPI_CR1_SPE;
  }
  else
  {
    spi_handle.CR1 = spi_handle.CR1 & ~SPI_CR1_SPE;
  }

  return true;
}

bool send_byte(SPI_TypeDef &spi_handle, uint8_t byte)
{

  volatile uint8_t *spidr = ((volatile uint8_t *)&spi_handle.DR);
  *spidr                  = byte;
  // wait for SPI periph ready-state
  while (!stm32::spi_ref::wait_for_bsy_flag(spi_handle, 10))
    ;
  while (!stm32::spi_ref::wait_for_txe_flag(spi_handle, 10))
    ;

  return true;
}

bool wait_for_txe_flag(SPI_TypeDef &spi_handle, uint32_t delay_us)
{
  // The TXE flag is set when transmission TXFIFO has enough space to store data to send.
  if ((spi_handle.SR & SPI_SR_TXE) != (SPI_SR_TXE))
  {
    // give TX FIFO a chance to clear before checking again
    stm32::TimerManager::delay_microsecond(delay_us);
    return false;
  }
  return true;
}

bool wait_for_bsy_flag(SPI_TypeDef &spi_handle, uint32_t delay_us)
{
  // When BSY is set, it indicates that a data transfer is in progress on the SPI
  if ((spi_handle.SR & SPI_SR_BSY) == SPI_SR_BSY)
  {
    // give SPI bus a chance to finish sending data before checking again
    stm32::TimerManager::delay_microsecond(delay_us);
    return false;
  }
  return true;
}

bool set_prescaler(SPI_TypeDef &spi_handle, uint32_t new_value)
{

  spi_handle.CR1 = spi_handle.CR1 & ~(SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0);
  spi_handle.CR1 = spi_handle.CR1 | (new_value);
  return true;
}

} // namespace stm32::spi_ref