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

namespace stm32::i2c
{

template<typename I2C_HANDLE>
bool init_i2c_connect(I2C_HANDLE *i2c_handle, uint8_t addr)
{
	if (LL_I2C_IsActiveFlag_BUSY(i2c_handle) == SET) { return false; }
	
	LL_I2C_SetMasterAddressingMode(i2c_handle, LL_I2C_ADDRESSING_MODE_7BIT);
	LL_I2C_SetSlaveAddr(i2c_handle, addr);
	LL_I2C_SetTransferRequest(i2c_handle, LL_I2C_REQUEST_WRITE);
	LL_I2C_EnableAutoEndMode(i2c_handle);
	LL_I2C_GenerateStartCondition(i2c_handle);

	// give slave a chance to respond
	LL_mDelay(1);

	// addr was not recognised by slave device
	if ( (LL_I2C_IsActiveFlag_NACK(i2c_handle) == SET) )
	{
		return false;
	}

	return true;

}


}   // namespace stm32::i2c



#endif // __LL_I2C_UTILS_HPP__