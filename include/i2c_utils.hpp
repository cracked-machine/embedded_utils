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

#ifndef __I2C_UTILS_HPP__
#define __I2C_UTILS_HPP__

#include <cstdint>
#include <array>



// add groups for device family
#if defined (STM32G071xx) || defined (STM32G081xx) || defined (STM32G070xx) \
|| defined (STM32G030xx) || defined (STM32G031xx) || defined (STM32G041xx) \
|| defined (STM32G0B0xx) || defined (STM32G0B1xx) || defined (STM32G0C1xx) \
|| defined (STM32G050xx) || defined (STM32G051xx) || defined (STM32G061xx)		
	#include <stm32g0xx.h>
#endif



namespace stm32::i2c
{

/// @brief The I2C bus transaction status
enum class Status
{
    /// @brief Slave device recognised the address
    ACK,
    /// @brief Slave device is busy
    BUSY,
    /// @brief Slave device did not recognise the address
    NACK,

};

/// @brief Specify the type of message to send with initialise_slave_device() function
enum class StartType
{
	/// @brief Send STOP after adddress. Use this to test if a slave device is present
	PROBE,
	/// @brief Sends r/w bit high and sends a repeated START condition. Use this to change direction after WRITE.
    READ,
	/// @brief Sends r/w bit low and keeps bus open for further writes
    WRITE
};

/// @brief Send the address byte to the I2C slave device and use start_type to generate a start condition
/// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
/// @param addr The address byte to send to the slave device
/// @param start_type PROBE: send STOP after adddress, WRITE: r/w bit low and keep open, READ: r/w bit high + repeated START
/// @return Status The I2C slave device response
Status initialise_slave_device(I2C_TypeDef* i2c_handle, uint8_t addr, StartType start_type );

/// @brief Write single data byte to I2C_TXDR register (transmit to the I2C slave device) 
/// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
/// @param buffer_byte Data byte to transmit
/// @return Status The I2C slave device response
Status send_byte(I2C_TypeDef* i2c_handle, uint8_t tx_byte);

/// @brief Read single byte from I2C_RXDR register (Received from the I2C slave device)
/// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
/// @param buffer_byte Data byte to receive
/// @return Status The I2C slave device response
Status receive_byte(I2C_TypeDef* i2c_handle, uint8_t &rx_byte);

/// @brief Generation restart/start condition now
/// The type of generated start condition will depend on the start_type argument used with initialise_slave_device()
/// @param i2c_handle pointer to I2C Interface
void generate_start_condition(I2C_TypeDef* i2c_handle);

/// @brief Generate a stop condition after active transfer has completed.
/// @param i2c_handle pointer to I2C Interface
void generate_stop_condition(I2C_TypeDef* i2c_handle);

/// @brief Set the number of bytes for the tx/rx transaction
/// Changing these bits when the START bit is set is not allowed.
/// @param i2c_handle pointer to I2C Interface
/// @param nbytes The number of bytes to be transmitted/received. This field is don’t care in slave mode
void set_numbytes(I2C_TypeDef* i2c_handle, uint32_t nbytes);

void send_ack(I2C_TypeDef* i2c_handle);
void send_nack(I2C_TypeDef* i2c_handle);


}   // namespace stm32::i2c



#endif // __LL_I2C_UTILS_HPP__