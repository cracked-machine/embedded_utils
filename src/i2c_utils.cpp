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

#include <i2c_utils.hpp>
#include <timer_manager.hpp>

namespace stm32::i2c
{

Status send_addr(I2C_TypeDef* i2c_handle, uint8_t addr, MsgType type)
{	
#if not defined(X86_UNIT_TESTING_ONLY)

	// Set the master to operate in 7-bit addressing mode. Clear ADD10 bit[11] 
	i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_ADD10);

	// Set the address for the slave device. Set SADD bits[7:1].
	// The bits SADD[9],SADD[8] and SADD[0] are don't care. 
	i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_SADD);
	i2c_handle->CR2 = i2c_handle->CR2 | (addr << 0);	
	
	if (type == MsgType::PROBE) // generate START with AUTO-END enabled
	{
		// Master requests a write transfer
		i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_RD_WRN);
		
		// Enable AUTOEND Mode. A STOP condition is automatically sent when NBYTES data are transferred.
		i2c_handle->CR2 = i2c_handle->CR2 | I2C_CR2_AUTOEND;
		
	}
	else if (type == MsgType::WRITE) // generate START with AUTO-END disabled
	{
		// Master requests a write transfer
		i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_RD_WRN);
		
		// Disable RELOAD Mode. The transfer is completed after the NBYTES data transfer (STOP or RESTART follows).
		i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_RELOAD);
		
		// Disable AUTOEND Mode. TC flag is set when NBYTES data are transferred, stretching SCL low.
		i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_AUTOEND);
	}
	else if (type == MsgType::READ) // generate REPEATED START
	{
		
		// Master requests a read transfer
		i2c_handle->CR2 = i2c_handle->CR2 | (I2C_CR2_RD_WRN);	
		
		// Disable RELOAD Mode. The transfer is completed after the NBYTES data transfer (STOP or RESTART follows).
		i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_RELOAD);
		
		// Disable AUTOEND Mode. TC flag is set when NBYTES data are transferred, stretching SCL low.
		i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_AUTOEND);	
	}

	// Generate the restart/start condition
	generate_start_condition(i2c_handle);

	// give slave a chance to respond
	stm32::TimerManager::delay_microsecond(1000);

	// check if addr was not recognised by slave device
	if ( (i2c_handle->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF )
	{
		return Status::NACK;
	}
#endif
	// otherwise slave device is happy
	return Status::ACK;

}



Status receive_byte(I2C_TypeDef* i2c_handle, uint8_t &rx_byte)
{
#if not defined(X86_UNIT_TESTING_ONLY)
	rx_byte = i2c_handle->RXDR & I2C_RXDR_RXDATA;
#endif
	return Status::ACK;	

}


Status send_byte(I2C_TypeDef* i2c_handle, uint8_t tx_byte)
{
#if not defined(X86_UNIT_TESTING_ONLY)
	i2c_handle->TXDR = tx_byte;
	
	// wait for TX FIFO to be transmitted before continuing
	while (((i2c_handle->ISR & I2C_ISR_TXE) == I2C_ISR_TXE) == false)
	{
		// do nothing
		stm32::TimerManager::delay_microsecond(10);
	}
	// check if slave device responded with NACK
	if (((i2c_handle->ISR & I2C_ISR_NACKF) == I2C_ISR_NACKF) == true)
	{
		return Status::NACK;
	}
#endif
	return Status::ACK;
}

void generate_stop_condition(I2C_TypeDef* i2c_handle)
{
	i2c_handle->CR2 = i2c_handle->CR2 | (I2C_CR2_STOP);
}

void generate_start_condition(I2C_TypeDef* i2c_handle)
{
	i2c_handle->CR2 = i2c_handle->CR2 | (I2C_CR2_START);
}

void set_numbytes(I2C_TypeDef* i2c_handle, uint32_t nbytes)
{
	i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_NBYTES);
	i2c_handle->CR2 = i2c_handle->CR2 | (nbytes << I2C_CR2_NBYTES_Pos);
}

void send_ack(I2C_TypeDef* i2c_handle)
{
	i2c_handle->CR2 = i2c_handle->CR2 & ~(I2C_CR2_NACK);
}

void send_nack(I2C_TypeDef* i2c_handle)
{
	i2c_handle->CR2 = i2c_handle->CR2 | (I2C_CR2_NACK);
}

} // namespace stm32::i2c