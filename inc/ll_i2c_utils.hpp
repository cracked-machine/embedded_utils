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

#ifndef __LL_I2C_UTILS_HPP__
#define __LL_I2C_UTILS_HPP__

#include <cstdint>
#include <array>
#include <ll_tim_utils.hpp>
#include <memory>

namespace stm32::i2c
{

// @brief The I2C bus transaction status
enum class Status
{
    // @brief Slave device recognised the address
    ACK,
    // @brief Slave device is busy
    BUSY,
    // @brief Slave device did not recognise the address
    NACK,
	// @brief placeholder for misc errors
	NOSUPPORT
};

// @brief Specify the type of message to send with send_addr() function
enum class MsgType
{
	// @brief Send STOP after adddress. Use this to test if a slave device is present
	PROBE,
	// @brief Sends r/w bit high and sends a repeated START condition. Use this to change direction after WRITE.
    READ,
	// @brief Sends r/w bit low and keeps bus open for further writes
    WRITE
};

// @brief Send the address byte to the I2C slave device
// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
// @param addr The address byte to send to the slave device
// @param type PROBE: send STOP after adddress, WRITE: r/w bit low and keep open, READ: r/w bit high + repeated START
// @return Status The I2C slave device response
Status send_addr(std::unique_ptr<I2C_TypeDef> &i2c_handle, uint8_t addr, MsgType type );

// @brief Send the command byte to the I2C slave device
// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
// @param command The command byte to send
// @return Status The I2C slave device response
Status send_command(std::unique_ptr<I2C_TypeDef> &i2c_handle, uint8_t command);

// @brief Write the data byte(s) to I2C_TXDR register (transmit to the I2C slave device)
// @tparam BUFFER_SIZE The size of the buffer
// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
// @param buffer data byte(s) to transmit
// @return Status The I2C slave device response
template<std::size_t BUFFER_SIZE>
Status send_data(std::unique_ptr<I2C_TypeDef> &i2c_handle, std::array<uint8_t, BUFFER_SIZE> &buffer)
{
	Status res = Status::ACK;
	for (uint8_t &byte : buffer)
	{

		LL_I2C_TransmitData8(i2c_handle.get(), byte);
		while (!LL_I2C_IsActiveFlag_TXE(i2c_handle.get()))
		{
			// wait for byte to be sent
		}
		// command was not recognised by slave device
		if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle.get()) == SET) )
		{
			res = Status::NACK;
		}
	
	}
	
	return res;
}


// @brief Read the data byte from I2C_RXDR register (Received from the I2C slave device)
// @tparam BUFFER_SIZE The size of the buffer
// @param i2c_handle The unique_ptr to the CMSIS memory-mapped I2C device
// @param buffer Data byte(s) to receive
// @return Status The I2C slave device response
template<std::size_t BUFFER_SIZE>
Status receive_data(std::unique_ptr<I2C_TypeDef> &i2c_handle, std::array<uint8_t, BUFFER_SIZE> &buffer)
{

	buffer.at(0) = LL_I2C_ReceiveData8(i2c_handle.get());
	return Status::ACK;	

}

}   // namespace stm32::i2c



#endif // __LL_I2C_UTILS_HPP__