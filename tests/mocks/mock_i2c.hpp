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


#ifndef __MOCK_I2C_HPP__
#define __MOCK_I2C_HPP__

#include <mock_stm32g0b1xx_defs.hpp>


#include <iostream>
#include <chrono>
#include <future>
#include <thread>

enum class SlaveStatus
{
    ACK,
    NACK
};

/// @brief Mock function to test stm32::i2c::initalise_slave_device()
/// @param i2c_handle The mocked i2c peripheral
/// @return true if unit test disables the peripheral (upon test completion), false if i2c_handle is null_ptr
bool inline mock_i2c_start_condition_generation(I2C_TypeDef *i2c_handle, uint8_t expected_addr)
{
    if (i2c_handle == nullptr)
    {
        return false;
    }
    // loop while peripheral is enabled
    while((i2c_handle->CR1 & I2C_CR1_PE_Msk) == I2C_CR1_PE_Msk)
    {
        // start condition was generated
        if ((i2c_handle->CR2 & I2C_CR2_START_Msk) == I2C_CR2_START_Msk)
        { 
            if ((i2c_handle->CR2 & I2C_CR2_SADD_Msk) == expected_addr)
            {
                // expected address was used
                return true;
            }
            else
            {
                // UNexpected address was used
                i2c_handle->ISR = i2c_handle->ISR | I2C_ISR_NACKF_Msk;
                return false;
            }
        }
    }

    return true;
}

/// @brief Mock function to test stm32::i2c::send_byte()
/// @param i2c_handle The mocked i2c peripheral
/// @return true if unit test disables the peripheral (upon test completion), false if i2c_handle is null_ptr
bool inline mock_i2c_tx_fifo_empty(I2C_TypeDef *i2c_handle, SlaveStatus slave_status)
{
    // input check
    if (i2c_handle == nullptr) { return false; }
    // reset the register to prevent false positive
    i2c_handle->ISR = i2c_handle->ISR & ~I2C_ISR_TXE;

    // loop while peripheral is enabled
    while((i2c_handle->CR1 & I2C_CR1_PE_Msk) == I2C_CR1_PE_Msk)
    {
        // wait test bytes to be copied into TX FIFO
        if (i2c_handle->TXDR != 0)
        {
            
            std::this_thread::sleep_for(1us);
            // mock the emptying of the TX FIFO
            i2c_handle->TXDR = 0;
            // mock the signaling that the TX FIFO is empty
            i2c_handle->ISR = i2c_handle->ISR | I2C_ISR_TXE;
            std::this_thread::sleep_for(1us);
            // simulate unhappy slave device, if requested by unit test
            if (slave_status == SlaveStatus::NACK) { i2c_handle->ISR = i2c_handle->ISR | I2C_ISR_NACKF_Msk; }
            
        }
    }

    return true;
}

#endif // __MOCK_I2C_HPP__