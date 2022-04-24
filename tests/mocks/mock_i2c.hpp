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

#include <stm32g0xx.h>
#include <future>

#define FUSE_USE_VERSION 30
#include <fuse3/fuse.h>
#include <cstring>
#include <cstddef>

namespace stm32::mock
{

enum class SlaveStatus
{
    ACK,
    NACK
};



#define OPTION(t, p) { t, offsetof(struct FuseOptions, p), 1 }



class I2C 
{

public:

    I2C();

    void init_i2c_tx_fifo(std::future<bool> &tx_fifo_future, SlaveStatus expected_slave_response);
    void init_i2c_start_condition(std::future<bool> &start_condition_future, uint8_t expected_address);

    //////////////////////////////
    ////   FUSE prototypes    ////
    //////////////////////////////   

    static const int m_fuse_argc {1}; 
    static inline char *m_fuse_argv[1] { (char*)"I2C" }; // nasty c-style cast!
    static struct FuseOptions {
        const char *filename { strdup("hello") };
        const char *contents { strdup("Hello World!\n") };
        int show_help {1};
    } m_fuse_options;    
    // static FuseOptions m_fuse_options;

    static inline const struct fuse_opt m_fuse_option_spec[5] = {
        OPTION("--name=%s", filename),
        OPTION("--contents=%s", contents),
        OPTION("-h", show_help),
        OPTION("--help", show_help),
        FUSE_OPT_END
    };

    static void* hello_init(struct fuse_conn_info *conn, struct fuse_config *cfg);
    static int hello_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi);
    static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi, enum fuse_readdir_flags flags);
    static int hello_open(const char *path, struct fuse_file_info *fi);
    static int hello_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi);

    /// @brief Mock function to test stm32::i2c::initalise_slave_device()
    /// @param i2c_handle The mocked i2c peripheral
    /// @return true if unit test disables the peripheral (upon test completion), false if i2c_handle is null_ptr
    bool static mock_i2c_start_condition_generation(I2C_TypeDef *i2c_handle, uint8_t expected_addr);  

    /// @brief Mock function to test stm32::i2c::send_byte()
    /// @param i2c_handle The mocked i2c peripheral
    /// @return true if unit test disables the peripheral (upon test completion), false if i2c_handle is null_ptr
    bool static mock_i2c_tx_fifo_empty(I2C_TypeDef *i2c_handle, SlaveStatus slave_status);

    I2C_TypeDef* get_handle() { return i2c_handle; }
    
private:
    I2C_TypeDef *i2c_handle {nullptr};


};


} // namespace stm32::mock


#endif // __MOCK_I2C_HPP__