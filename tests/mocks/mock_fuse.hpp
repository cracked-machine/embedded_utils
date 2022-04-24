
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


#ifndef __MOCK_FUSE_HPP__
#define __MOCK_FUSE_HPP__

#define FUSE_USE_VERSION 32
#include <fuse3/fuse.h>
#include <fuse3/fuse_lowlevel.h>  /* for fuse_cmdline_opts */
#include <cstring>
#include <cstddef>

#include <catch2/catch_all.hpp>

namespace stm32::mock
{

#define OPTION(t, p) { t, offsetof(struct FuseOptions, p), 1 }

class MockFuse 
{

public:

    MockFuse() = default;
    virtual ~MockFuse();

    bool init_session();
    static bool start_async_session();

    static inline bool running {false};

    struct fuse_session *m_fuse_session {nullptr};
    struct fuse_cmdline_opts m_fuse_opts;
    struct fuse_args m_fuse_args;
    struct fuse_loop_config m_fuse_config;

    static constexpr char m_mock_mnt_path[11] = "/tmp/fuse";
    std::array<char*, 2> m_mock_input_args 
    { 
        (char*)"I2C", 
        (char*)m_mock_mnt_path 
    };

    static struct FuseOptions {
        const char *filename { strdup("hello") };
        const char *contents { strdup("Hello World!\n") };
        int show_help {1};
    } m_fuse_options;    

    struct dirbuf {
        char *p;
        size_t size;
    };    

    static int stat(fuse_ino_t ino, struct stat *stbuf);
    static int reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize, off_t off, size_t maxsize);
    static void dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name, fuse_ino_t ino);

    static void ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name);
    static void ll_getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
    static void ll_unlink(fuse_req_t req, fuse_ino_t parent, const char *name);
    static void ll_open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
    static void ll_read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);
    static void ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);

    static inline const struct fuse_lowlevel_ops fuse_ll_oper = {
        .init               = nullptr,
        .destroy            = nullptr,
        .lookup             = ll_lookup,
        .forget             = nullptr,
        .getattr	        = ll_getattr,
        .setattr            = nullptr,
        .readlink           = nullptr,
        .mknod              = nullptr,
        .mkdir              = nullptr,
        .unlink             = ll_unlink,
        .rmdir              = nullptr,
        .symlink            = nullptr,
        .rename             = nullptr,
        .link               = nullptr,
        .open		        = ll_open,
        .read		        = ll_read,
        .write              = nullptr,
        .flush              = nullptr,
        .release            = nullptr, 
        .fsync              = nullptr,
        .opendir            = nullptr,  
        .readdir	        = ll_readdir,
        .releasedir         = nullptr,
        .fsyncdir           = nullptr,
        .statfs             = nullptr,
        .setxattr           = nullptr,
        .getxattr           = nullptr,
        .listxattr          = nullptr,
        .removexattr        = nullptr,        
        .access             = nullptr,
        .create             = nullptr,
        .getlk              = nullptr,
        .setlk              = nullptr,
        .bmap               = nullptr,
        .ioctl              = nullptr,
        .poll               = nullptr,
        .write_buf          = nullptr,
        .retrieve_reply     = nullptr,
        .forget_multi       = nullptr,
        .flock              = nullptr,
        .fallocate          = nullptr,
        .readdirplus        = nullptr,
        .copy_file_range    = nullptr,
        .lseek              = nullptr
    };  

};

} // namespace stm32::mock

#endif // __MOCK_FUSE_HPP__