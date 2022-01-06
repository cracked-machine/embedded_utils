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

enum class MsgType
{
	PROBE,
    READ,
    WRITE
};

// @brief Send the address byte to the slave ending with write bit.
// @param i2c_handle Pointer to the mem-mapped I2C device. 
// @param write_addr The slave device address, check device datasheet for write-specific address
// @param type PROBE will auto-send a STOP condition, WRITE will not. 
// @return Status ACK for valid slave device address, NACK for invalid slave address.
template<typename I2C_HANDLE>
Status send_addr(I2C_HANDLE *i2c_handle, uint8_t addr, MsgType type )
{	
	// setup the common transaction options
	LL_I2C_SetMasterAddressingMode(i2c_handle, LL_I2C_ADDRESSING_MODE_7BIT);
	
	LL_I2C_SetSlaveAddr(i2c_handle, addr);
	
	if (type == MsgType::PROBE)
	{
		// generate START with AUTO-END enabled
		LL_I2C_SetTransferRequest(i2c_handle, LL_I2C_REQUEST_WRITE);
		LL_I2C_EnableAutoEndMode(i2c_handle);
	}
	else if (type == MsgType::WRITE)
	{
		// generate START with AUTO-END disabled
		LL_I2C_SetTransferRequest(i2c_handle, LL_I2C_REQUEST_WRITE);
		LL_I2C_DisableReloadMode(i2c_handle);
		LL_I2C_DisableAutoEndMode(i2c_handle);
	}
	else if (type == MsgType::READ)
	{
		// generate REPEATED START
		LL_I2C_SetTransferRequest(i2c_handle, LL_I2C_REQUEST_READ);		
		LL_I2C_DisableReloadMode(i2c_handle);
		LL_I2C_DisableAutoEndMode(i2c_handle);		
	}

	// send the address byte to slave
	LL_I2C_GenerateStartCondition(i2c_handle);

	// give slave a chance to respond
	embed_utils::tim::ll_delay_microsecond(TIM14, 1000);

	// addr was not recognised by slave device
	if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle) == SET) )
	{
		return Status::NACK;
	}

	return Status::ACK;

}

template<typename I2C_HANDLE>
Status send_command(I2C_HANDLE *i2c_handle, uint8_t command)
{

	// send the command byte
	LL_I2C_TransmitData8(i2c_handle, command);
	while (!LL_I2C_IsActiveFlag_TXE(i2c_handle))
	{
		// wait for byte to be sent
	}
	// command was not recognised by slave device
	if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle) == SET) )
	{
		return Status::NACK;
	}

	return Status::ACK;
}


template<typename I2C_HANDLE, std::size_t BUFFER_SIZE>
Status send_data(I2C_HANDLE *i2c_handle, std::array<uint8_t, BUFFER_SIZE> &buffer)
{
	Status res = Status::ACK;
	for (uint8_t &byte : buffer)
	{

		LL_I2C_TransmitData8(i2c_handle, byte);
		while (!LL_I2C_IsActiveFlag_TXE(i2c_handle))
		{
			// wait for byte to be sent
		}
		// command was not recognised by slave device
		if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle) == SET) )
		{
			res = Status::NACK;
		}
	
	}
	
	return res;
}



template<typename I2C_HANDLE, std::size_t BUFFER_SIZE>
Status receive_data(    
    I2C_HANDLE *i2c_handle, 
    std::array<uint8_t, BUFFER_SIZE> &buffer [[maybe_unused]])
{

	buffer.at(0) = LL_I2C_ReceiveData8(i2c_handle);
	return Status::ACK;	

}

}   // namespace stm32::i2c



#endif // __LL_I2C_UTILS_HPP__