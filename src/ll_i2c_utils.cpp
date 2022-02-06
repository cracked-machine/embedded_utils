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

#if defined(X86_UNIT_TESTING_ONLY)
#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wvolatile"
		#include "main.h"
		#include "i2c.h"	
	#pragma GCC diagnostic pop

#endif

#include <ll_i2c_utils.hpp>
#include <timer_manager.hpp>

namespace stm32::i2c
{

Status send_addr(std::unique_ptr<I2C_TypeDef> &i2c_handle [[maybe_unused]], uint8_t addr [[maybe_unused]], MsgType type [[maybe_unused]])
{	
#if not defined(X86_UNIT_TESTING_ONLY)
	// setup the common transaction options
	LL_I2C_SetMasterAddressingMode(i2c_handle.get(), LL_I2C_ADDRESSING_MODE_7BIT);
	LL_I2C_SetSlaveAddr(i2c_handle.get(), addr);
	
	if (type == MsgType::PROBE)
	{
		// generate START with AUTO-END enabled
		LL_I2C_SetTransferRequest(i2c_handle.get(), LL_I2C_REQUEST_WRITE);
		LL_I2C_EnableAutoEndMode(i2c_handle.get());
	}
	else if (type == MsgType::WRITE)
	{
		// generate START with AUTO-END disabled
		LL_I2C_SetTransferRequest(i2c_handle.get(), LL_I2C_REQUEST_WRITE);
		LL_I2C_DisableReloadMode(i2c_handle.get());
		LL_I2C_DisableAutoEndMode(i2c_handle.get());
	}
	else if (type == MsgType::READ)
	{
		// generate REPEATED START
		LL_I2C_SetTransferRequest(i2c_handle.get(), LL_I2C_REQUEST_READ);		
		LL_I2C_DisableReloadMode(i2c_handle.get());
		LL_I2C_DisableAutoEndMode(i2c_handle.get());		
	}

	// send the address byte to slave
	LL_I2C_GenerateStartCondition(i2c_handle.get());

	// give slave a chance to respond
	// stm32::tim::ll_delay_microsecond(TIM14, 1000);
	stm32::TimerManager::delay_microsecond(1000);

	// addr was not recognised by slave device
	if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle.get()) == SET) )
	{
		return Status::NACK;
	}
#endif
	return Status::ACK;

}



Status receive_byte(std::unique_ptr<I2C_TypeDef> &i2c_handle [[maybe_unused]], uint8_t &rx_byte [[maybe_unused]])
{
#if not defined(X86_UNIT_TESTING_ONLY)
	rx_byte = LL_I2C_ReceiveData8(i2c_handle.get());
#endif
	return Status::ACK;	

}


Status send_byte(std::unique_ptr<I2C_TypeDef> &i2c_handle [[maybe_unused]], uint8_t tx_byte [[maybe_unused]])
{
#if not defined(X86_UNIT_TESTING_ONLY)
	LL_I2C_TransmitData8(i2c_handle.get(), tx_byte);
	while (!LL_I2C_IsActiveFlag_TXE(i2c_handle.get()))
	{
		// wait for byte to be sent
	}
	// command was not recognised by slave device
	if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle.get()) == SET) )
	{
		return Status::NACK;
	}
#endif
	return Status::ACK;
}

} // namespace stm32::i2c