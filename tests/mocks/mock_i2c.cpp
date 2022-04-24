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

#include <mock_i2c.hpp>



#include <iostream>
#include <chrono>
#include <array>
#include <string>
#include <thread>

using namespace std::chrono_literals;

namespace stm32::mock
{

I2C::I2C()
{
    // setup libfuse
    struct fuse_args args = FUSE_ARGS_INIT(m_fuse_argc, m_fuse_argv);
    fuse_opt_parse(&args, &m_fuse_options, m_fuse_option_spec, NULL);
    fuse_main(args.argc, args.argv, &hello_oper, NULL);


    i2c_handle = new I2C_TypeDef;
    i2c_handle->CR1 = i2c_handle->CR1 | I2C_CR1_PE_Msk;
}

void I2C::init_i2c_tx_fifo(std::future<bool> &tx_fifo_future, SlaveStatus expected_slave_response)
{
    tx_fifo_future = std::async(std::launch::async, mock_i2c_tx_fifo_empty, i2c_handle, expected_slave_response);
}

void I2C::init_i2c_start_condition(std::future<bool> &start_condition_future, uint8_t expected_address)
{
    start_condition_future = std::async(std::launch::async, mock_i2c_start_condition_generation, i2c_handle, expected_address);
}

/// @brief Mock function to test stm32::i2c::initalise_slave_device()
/// @param i2c_handle The mocked i2c peripheral
/// @return true if unit test disables the peripheral (upon test completion), false if i2c_handle is null_ptr
bool inline I2C::mock_i2c_start_condition_generation(I2C_TypeDef *i2c_handle, uint8_t expected_addr)
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
bool inline I2C::mock_i2c_tx_fifo_empty(I2C_TypeDef *i2c_handle, SlaveStatus slave_status)
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

//////////////////////////////
//// FUSE implementations ////
//////////////////////////////

// initialise static member
I2C::FuseOptions I2C::m_fuse_options;

void* I2C::hello_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
	(void) conn;
	cfg->kernel_cache = 1;
	return NULL;
}

int I2C::hello_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
	(void) fi;
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path+1, m_fuse_options.filename) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(m_fuse_options.contents);
	} else
		res = -ENOENT;

	return res;
}

int I2C::hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi,
			 enum fuse_readdir_flags flags)
{
	(void) offset;
	(void) fi;
	(void) flags;

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0, static_cast<fuse_fill_dir_flags>(0));
	filler(buf, "..", NULL, 0, static_cast<fuse_fill_dir_flags>(0));
	filler(buf, m_fuse_options.filename, NULL, 0, static_cast<fuse_fill_dir_flags>(0));

	return 0;
}

int I2C::hello_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path+1, m_fuse_options.filename) != 0)
		return -ENOENT;

	if ((fi->flags & O_ACCMODE) != O_RDONLY)
		return -EACCES;

	return 0;
}

int I2C::hello_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path+1, m_fuse_options.filename) != 0)
		return -ENOENT;

	len = strlen(m_fuse_options.contents);
	if (offset < static_cast<off_t>(len)) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, m_fuse_options.contents + offset, size);
	} else
		size = 0;

	return size;
}


} // namespace stm32::mock
    