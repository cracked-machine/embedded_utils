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

#include <mock_fuse.hpp>

#include <filesystem>
#include <unistd.h>
#include <assert.h>
#include <algorithm>
#include <thread>
#include <future>
using namespace std::chrono_literals;

namespace stm32::mock {


bool MockFuse::init_session()
{
    // setup libfuse
    if (!std::filesystem::exists(m_mock_mnt_path)) { std::filesystem::create_directories(m_mock_mnt_path); }

    m_fuse_args = FUSE_ARGS_INIT(static_cast<int>(m_mock_input_args.size()), m_mock_input_args.data());

    if (fuse_parse_cmdline(&m_fuse_args, &m_fuse_opts) != 0) { return false; }

	m_fuse_session = fuse_session_new(&m_fuse_args, &fuse_ll_oper, sizeof(fuse_ll_oper), NULL);

	if (fuse_set_signal_handlers(m_fuse_session) != 0) { return false; }
	if (fuse_session_mount(m_fuse_session, m_fuse_opts.mountpoint) != 0) { return false; }

	if (fuse_daemonize(true) != 0) { return false; }



	return true;
}

bool MockFuse::start_async_session()
{
	while(running)
	{
		std::this_thread::sleep_for(10us);
	}
	return true;
}

MockFuse::~MockFuse()
{
	if(m_fuse_session != nullptr)
	{
		fuse_session_unmount(m_fuse_session);
		fuse_remove_signal_handlers(m_fuse_session);
		fuse_session_destroy(m_fuse_session);
	}
	free(m_fuse_opts.mountpoint);
	fuse_opt_free_args(&m_fuse_args);

}

//////////////////////////////
//// FUSE implementations ////
//////////////////////////////

// initialise static member
MockFuse::FuseOptions MockFuse::m_fuse_options;

int MockFuse::stat(fuse_ino_t ino, struct stat *stbuf)
{
	stbuf->st_ino = ino;
	switch (ino) {
	case 1:
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		break;

	case 2:
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(m_fuse_options.contents);
		break;

	default:
		return -1;
	}
	return 0;
}

int MockFuse::reply_buf_limited(fuse_req_t req, const char *buf, size_t bufsize, off_t off, size_t maxsize)
{
	if (off < static_cast<off_t>(bufsize))
	{
		return fuse_reply_buf(req, buf + off, std::min(bufsize - off, maxsize));
	}
	else
	{
		return fuse_reply_buf(req, NULL, 0);
	}
}

void MockFuse::dirbuf_add(fuse_req_t req, struct dirbuf *b, const char *name, fuse_ino_t ino)
{
	struct stat stbuf;
	size_t oldsize = b->size;
	b->size += fuse_add_direntry(req, NULL, 0, name, NULL, 0);
	b->p = (char *) realloc(b->p, b->size);
	memset(&stbuf, 0, sizeof(stbuf));
	stbuf.st_ino = ino;
	fuse_add_direntry(req, b->p + oldsize, b->size - oldsize, name, &stbuf,
			  b->size);
}

void MockFuse::ll_getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	struct stat stbuf;

	(void) fi;

	memset(&stbuf, 0, sizeof(stbuf));
	if (stat(ino, &stbuf) == -1)
		fuse_reply_err(req, ENOENT);
	else
		fuse_reply_attr(req, &stbuf, 1.0);
}

void MockFuse::ll_lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	struct fuse_entry_param e;

	if (parent != 1 || strcmp(name, m_fuse_options.filename) != 0)
		fuse_reply_err(req, ENOENT);
	else {
		memset(&e, 0, sizeof(e));
		e.ino = 2;
		e.attr_timeout = 1.0;
		e.entry_timeout = 1.0;
		stat(e.ino, &e.attr);

		fuse_reply_entry(req, &e);
	}

}

void MockFuse::ll_unlink([[maybe_unused]] fuse_req_t req, [[maybe_unused]] fuse_ino_t parent, [[maybe_unused]] const char *name)
{
	// int res;

	// res = unlinkat(lo_fd(req, parent), name, 0);

	// fuse_reply_err(req, res == -1 ? errno : 0);
}

void MockFuse::ll_open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	if (ino != 2)
		fuse_reply_err(req, EISDIR);
	else if ((fi->flags & O_ACCMODE) != O_RDONLY)
		fuse_reply_err(req, EACCES);
	else
		fuse_reply_open(req, fi);
}

void MockFuse::ll_read(fuse_req_t req, [[maybe_unused]] fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	assert(ino == 2);
	reply_buf_limited(req, m_fuse_options.contents, strlen(m_fuse_options.contents), off, size);
}

void MockFuse::ll_readdir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	(void) fi;

	if (ino != 1)
		fuse_reply_err(req, ENOTDIR);
	else {
		struct dirbuf b;

		memset(&b, 0, sizeof(b));
		dirbuf_add(req, &b, ".", 1);
		dirbuf_add(req, &b, "..", 1);
		dirbuf_add(req, &b, m_fuse_options.filename, 2);
		reply_buf_limited(req, b.p, b.size, off, size);
		free(b.p);
	}
}

} // namespace stm32::mock